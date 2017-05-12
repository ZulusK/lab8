#ifndef LAB8_SERVER_H
#define LAB8_SERVER_H

#include <progbase-cpp/net.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>

using namespace progbase::net;

class Server {
    typedef struct {
        std::thread *thread;
        std::mutex alive;
        TcpClient *client;
    } Processor;

    TcpListener *listener;
    IpAddress *address;
    std::thread *serverThread;
    bool isAlive;
    std::mutex processorsMutex;
    std::vector<Processor *> userReqProcessors;

    Server(TcpListener *listener, IpAddress *address);

    void exec();

    void acceptClient(TcpClient **client, bool *connected);

    void connectProxyClient();

public:
    static Server *create(int port);

    bool start();

    bool status();

    ~Server();

    void stop();

    int port();

    std::string ip();

    void processClientRequest(Processor *processor);

    std::string getRequest(TcpClient *client);

    void sendAnswer(TcpClient *client, const std::string &str);

    void addClient(TcpClient *client);

    void processorsClean();

    void freeProcessor(Processor *p);
};


#endif //LAB8_SERVER_H
