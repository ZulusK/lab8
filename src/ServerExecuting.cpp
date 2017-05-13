#include <Server.h>
#include <iostream>
#include <vector>

using namespace std;

Server::Server(TcpListener *listener, IpAddress *address, const std::string &server, const std::string &dev,
               CounriesStorage *storage, const std::string &filename) {
    this->listener = listener;
    this->address = address;
    this->isAlive = false;
    this->developer = developer;
    this->serverName = server;
    this->serverThread = NULL;
    this->storage = storage;
    this->filename = filename;
    srand(time(NULL));
}

Server *Server::create(int port, const std::string &serverName, const std::string &developer, const string &filename,const string &dataFilename) {
    CounriesStorage *storage = CounriesStorage::load(filename);
    if (storage == NULL) {
        return NULL;
    }
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
    return new Server(listener, address, serverName, developer, storage, dataFilename);
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
    cout << "DELETE client [" << p->id << "]" << endl;
    delete p->thread;
    delete p->client;
    delete p;
}

void Server::addClient(TcpClient *client) {
    Processor *p = new Processor();
    p->alive.lock();
    p->client = client;
    p->id = clock();
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
            //if it canceled execute
            if (p->alive.try_lock()) {
                processorsMutex.lock();
                freeProcessor(p);
                userReqProcessors.erase(it);
                processorsMutex.unlock();
            }
            it = next;
        }
        this_thread::sleep_for(chrono::milliseconds(timeout));
    }
}

void Server::exec() {
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
//            acceptingThread->detach();
            acceptingThread->join();
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
        NetMessage msg(5);
        msg.setDataString("");
        client.send(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}

bool Server::start() {
    if (!isAlive) {
        cout << "===================Server started===================" << endl;
        this->isAlive = true;
        try {
            this->serverThread = new thread(&Server::exec, this);
        } catch (NetException e) {
            cout << e.what() << endl;
            return false;
        }
        return true;
    }
    return false;
}

void Server::acceptClient(TcpClient **client, bool *connected) {
    *connected = false;
    *client = listener->accept();
    *connected = true;
}

void Server::sendString(TcpClient *client, const string &str) {
    NetMessage msg(MAX_RESPONSE_LEN);
    msg.setDataString(str);
    sender.lock();
    cout << "SEND" << endl << str << endl;
    try {
        client->send(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
    sender.unlock();
}

string Server::getRequestStr(TcpClient *client) {
    NetMessage message(MAX_REQUEST_LEN);
    string req = "";
    try {
        client->receive(message);
        req += message.dataAsString();
    } catch (NetException e) {
        cout << e.what() << endl;
    }
    return req;
}

Server::~Server() {
    stop();
    delete listener;
    delete address;
    delete this->storage;
}

void Server::stop() {
    if (isAlive) {
        this->isAlive = false;
        this_thread::sleep_for(chrono::milliseconds(100));
        connectProxyClient();
        //wait for end of server thread
        this->serverThread->join();
        delete serverThread;
        this->serverThread = NULL;
        cout << "===================Server stopped===================" << endl;
    }
}

bool Server::status() {
    return isAlive;
}

int Server::port() {
    return address->port();
}

string Server::ip() {
    return address->address();
}

