#ifndef LAB8_SERVER_H
#define LAB8_SERVER_H

#include <progbase-cpp/net.h>
#include <string>
#include <thread>
#include <mutex>

using namespace progbase::net;

class Server {
    TcpListener *listener;
    IpAddress *address;
    std::thread *serverThread;
    bool status;

    Server(TcpListener *listener, IpAddress *address);

    void exec();


public:
    static Server *create(int port);

    bool start();

    bool isAlive();

    ~Server();

    void stop();

    int port();

    std::string ip();
};


#endif //LAB8_SERVER_H
