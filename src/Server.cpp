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
    }
}

void Server::exec() {
    NetMessage msg(2048);
    listener->start();
    while (status) {
        cout << "wait for connection " << this->address->address() << ":" << this->address->port() << " ..." << endl;
        TcpClient *client = listener->accept();
        client->receive(msg);
        cout << msg.dataAsString() << endl;
        client->send(msg);
//        this_thread::sleep_for(chrono::seconds(1));
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