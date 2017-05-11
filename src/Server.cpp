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

void Server::exec() {
    NetMessage msg(2048);
    listener->start();
    TcpClient *client = NULL;
    thread *acceptingThread = new thread(&Server::acceptClient, this, &client);
    while (status) {
        cout << "wait for connection " << this->address->address() << ":" << this->address->port() << " ..." << endl;
        if (!acceptingThread->joinable()) {
            client->receive(msg);
            cout << msg.dataAsString() << endl;
            delete client;
            delete acceptingThread;
            client = NULL;
            acceptingThread = new thread(&Server::acceptClient, this, &client);
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
    connectProxy();
    acceptingThread->join();
    delete acceptingThread;
    if (client) {
        delete client;
    }
}

void Server::connectProxy() {
    TcpClient client;
    client.connect(*address);
    NetMessage msg(10);
    msg.setDataString("stop");
    client.send(msg);
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