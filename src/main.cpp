#include <iostream>
#include <Server.h>

using namespace std;

int main(void) {
    int port = 4320;
    Server *server = NULL;
    while (!server) {
        port++;
        server = Server::create(port, "Lancelot", "Kazimirov Danil", "../data/countries.json", "../data/data.txt");
    }
    server->start();
    cout << server->toString() << endl;
    cin >> port;
    server->stop();
    delete server;
    return 0;
}