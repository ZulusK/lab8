#include <iostream>
#include <Server.h>
#include <CountriesStorage.h>
#include <TextProcessor.h>

using namespace std;

void connect(int port) {
//    system(("curl 127.0.0.1:" + to_string(port)).c_str());
    TcpClient client;
    try {
        client.connect(IpAddress("127.0.0.1", port));
        NetMessage msg(10);
        msg.setDataString("stop");
        client.send(msg);
        client.receive(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}

int main(void) {
    int port = 4320;
    Server *server = NULL;
    while (!server) {
        port++;
        server = Server::create(port, "Lancelot", "Kazimirov Danil", "../data/countries.json","../data/data.txt");
    }
    server->start();
    cout<<server->toString()<<endl;
    cin >> port;

    server->stop();
    delete server;
    return 0;
}