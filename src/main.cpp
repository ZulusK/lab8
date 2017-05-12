#include <iostream>
#include <Server.h>

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
        server = Server::create(port, "Lancelot", "Kazimirov Danil");
    }
    server->start();
//    this_thread::sleep_for(chrono::seconds(1));
//    for (int i = 0; i < 100; i++) {
//        auto T=new thread(connect, port);
//    }
//    this_thread::sleep_for(chrono::seconds(1));
    cin >> port;
    server->stop();
    delete server;
}