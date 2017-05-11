//
// Created by zulus on 11.05.17.
//

#include <Server.h>
#include <iostream>

using namespace std;

Server::Server(TcpListener *listener, IpAddress *address) {
    this->listener = listener;
    this->address = address;
    this->status = false;
    this->serverThread = NULL;
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

Server::~Server() {
    stop();
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

void Server::stop() {
    if (status) {
        status = false;
        this->serverThread->join();
        if (serverThread) delete serverThread;
        serverThread = NULL;
    }
}

void Server::acceptClient(TcpClient **client) {
    *client = listener->accept();
}

string Server::getRequest(TcpClient *client) {
    NetMessage message(1024);
    string req = "";
    try {
        do {
            client->receive(message);
            req += message.dataAsString();
        } while (!message.isEmpty());
    } catch (NetException e) {
        cout << e.what() << endl;
    }
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

void Server::processClientRequest(TcpClient *client) {
    string userReq = getRequest(client);
    cout << "Use request:" << endl;
    cout << userReq << endl;
    sendAnswer(client, userReq);
    delete client;
}

void Server::exec() {
    cout << "waiting for connection " << this->address->address() << ":" << this->address->port() << " ..." << endl;
    listener->start();
    //client to connect
    TcpClient *client = NULL;
    //thread for not-blocking listening accepting clients
    thread acceptingThread = thread(&Server::acceptClient, this, &client);

    while (status) {
        //if someone connect to server
        if (!acceptingThread.joinable()) {
            cout << "client connected" << endl;
            //not-blocking process client request
            thread(&Server::processClientRequest, this, client);
            //run thread again
            acceptingThread = thread(&Server::acceptClient, this, &client);
        }
    }
    //connect client, to terminate thread
    connectProxyClient();
    //wait to end of thread
    acceptingThread.join();
    //clear references
    if (client) {
        delete client;
    }
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

bool Server::isAlive() {
    return status;
}

bool Server::start() {
    if (!status) {
        this->status = true;
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