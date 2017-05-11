//
// Created by zulus on 11.05.17.
//

#include <Server.h>
#include <iostream>

using namespace std;

Server::Server(TcpListener *listener, IpAddress *address) {
    this->listener = listener;
    this->address = address;
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

bool Server::start() {
    try {
        this->listener->start();
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}