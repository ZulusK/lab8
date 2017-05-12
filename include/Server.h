#ifndef LAB8_SERVER_H
#define LAB8_SERVER_H

#include <progbase-cpp/net.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <CounriesStorage.h>

enum {
    SERVER_TIMEOUT = 10,
    CLEANER_TIMEOUT = 1000
};
using namespace progbase::net;

typedef struct {
    std::thread *thread;
    std::mutex alive;
    TcpClient *client;
} Processor;

class Server {
    TcpListener *listener;
    IpAddress *address;
    std::thread *serverThread;
    bool isAlive;
    std::mutex processorsMutex;
    std::vector<Processor *> userReqProcessors;
    std::string serverName;
    std::string developer;
    CounriesStorage *storage;

    Server(TcpListener *listener, IpAddress *address, const std::string &server, const std::string &dev,
           CounriesStorage *storage);

    void exec();

    void acceptClient(TcpClient **client, bool *connected);

    void connectProxyClient();

public:
    static Server *
    create(int port, const std::string &serverName, const std::string &developer, const std::string &filename);

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

    void cleaner(bool *status, int timeout);

    std::string getPathFromReq(const std::string &req);

    std::string processPathReq(const std::string &path);

    std::string getDate();

    std::string createHTTPHeader(int errorCode);
};


#endif //LAB8_SERVER_H
