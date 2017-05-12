//
// Created by zulus on 11.05.17.
//

#include <Server.h>
#include <iostream>
#include <vector>

using namespace std;

Server::Server(TcpListener *listener, IpAddress *address) {
    this->listener = listener;
    this->address = address;
    this->isAlive = false;
    this->serverThread = NULL;
    srand(time(NULL));
}

Server *Server::create(int port) {
    auto address = new IpAddress("127.0.0.1", port);
    auto listener = new TcpListener();
    try {
        listener->bind(*address);
    } catch (NetException e) {
        cout << e.what() << endl;
        delete listener;
        delete address;
        return NULL;
    }
    return new Server(listener, address);
}

void Server::stop() {
    if (isAlive) {
        this->isAlive = false;
        //wait for end of server thread
        this->serverThread->join();
        delete serverThread;
        this->serverThread = NULL;
    }
}

void Server::processClientRequest(Processor *processor) {
    processor->alive.lock();
    string userReq = getRequest(processor->client);
    this_thread::sleep_for(chrono::seconds(rand() % 3));
    sendAnswer(processor->client, userReq);
    processor->alive.unlock();
}

void Server::processorsClean() {
    processorsMutex.lock();
    for (auto it = userReqProcessors.begin(); it != userReqProcessors.end(); it++) {
        Processor *p = *it;
        p->alive.lock();
        freeProcessor(p);
    }
    userReqProcessors.clear();
    processorsMutex.unlock();

}

void Server::freeProcessor(Processor *p) {
    delete p->thread;
    delete p->client;
    delete p;
}

void Server::addClient(TcpClient *client) {
    Processor *p = new Processor();
    p->alive.lock();
    p->client = client;
    p->thread = new thread(&Server::processClientRequest, this, p);
    p->thread->detach();
    p->alive.unlock();

    processorsMutex.lock();
    userReqProcessors.push_back(p);
    processorsMutex.unlock();
}

void Server::cleaner(bool *status, int timeout) {
    while (*status) {
        int i = 0;
        int size = userReqProcessors.size();
        vector<int> rmInd;
        for (auto it = userReqProcessors.begin(); i < size && it != userReqProcessors.end() && *status;) {
            auto next = it + 1;
            i++;
            //delete thread
            Processor *p = *it;
            //if it cancel execute
            if (p->alive.try_lock()) {
                processorsMutex.lock();
                freeProcessor(p);
                userReqProcessors.erase(it);
                processorsMutex.unlock();
            }
            it = next;
        }
        cout << "Clients processing: " << userReqProcessors.size() << endl;
        this_thread::sleep_for(chrono::milliseconds(timeout));
    }
}

void Server::exec() {
    cout << "waiting for connection " << this->address->address() << ":" << this->address->port() << " ..." << endl;
    listener->start();
    bool connected = false;
    bool clean = true;
    //client to connect
    TcpClient *client = NULL;
    //thread for not-blocking listening accepting clients
    thread *acceptingThread = new thread(&Server::acceptClient, this, &client, &connected);
    //start garbage collector
    thread cleaner(&Server::cleaner, this, &clean, CLEANER_TIMEOUT);

    while (isAlive) {
        //if someone connect to server
        if (connected) {
            //not-blocking processing of client request
            addClient(client);
            acceptingThread->detach();
            delete acceptingThread;
            //run thread again
            if (isAlive)
                acceptingThread = new thread(&Server::acceptClient, this, &client, &connected);
            else break;
        }
        this_thread::sleep_for(chrono::milliseconds(SERVER_TIMEOUT));
    }
    //connect client, to terminate thread
    connectProxyClient();

    //end cleaner
    clean = false;
    cleaner.join();

    //wait to end of thread
    acceptingThread->join();

    //clear references
    delete acceptingThread;
    if (client) {
        delete client;
    }

    //clear all threads
    processorsClean();
}

void Server::connectProxyClient() {
    TcpClient client;
    try {
        client.connect(*address);
        NetMessage msg(10);
        msg.setDataString("stop");
        client.send(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}

bool Server::status() {
    return isAlive;
}

bool Server::start() {
    if (!isAlive) {
        this->isAlive = true;
        try {
            this->serverThread = new thread(&Server::exec, this);
        } catch (NetException e) {
            cout << e.what() << endl;
            return false;
        }
        return true;
    } else {
        return false;
    }
}

void Server::acceptClient(TcpClient **client, bool *connected) {
    *connected = false;
    *client = listener->accept();
    cout << "Connection success ... " << endl;
    *connected = true;
}

string Server::getRequest(TcpClient *client) {
    NetMessage message(1024);
    string req = "";
    try {
        client->receive(message);
        req += message.dataAsString();
    } catch (NetException e) {
        cout << e.what() << endl;
    }
    return req;
}

void Server::sendAnswer(TcpClient *client, const string &str) {
    NetMessage msg(1024);
    msg.setDataString(str);
    try {
        client->send(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}

Server::~Server() {
    stop();
    if (serverThread)
        delete serverThread;
    delete listener;
    delete address;
}

int Server::port() {
    return address->port();
}

string Server::ip() {
    return address->address();
}