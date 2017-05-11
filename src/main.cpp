#include <iostream>
#include <Server.h>

using namespace std;

int main(void) {
    int port = 4321;
    Server *server = NULL;
    while (!server) {
        server = Server::create(port++);
    }
    server->start();
    string s = "";
    while (s.compare("stop") != 0) {
        cin >> s;
    }
    server->stop();
    delete server;
}