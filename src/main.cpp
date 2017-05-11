#include <iostream>
#include <Server.h>

using namespace std;

int main(void) {
    auto server = Server::create(4321);
    cout << server->ip() << endl;
    cout << server->port() << endl;
    delete server;
}