#ifndef LAB8_SERVER_H
#define LAB8_SERVER_H

#include <progbase-cpp/net.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <CounriesStorage.h>

enum {
    MAX_RESPONSE_LEN = 2048,
    MAX_REQUEST_LEN = 1024,
    GET = 0xFFFF,
    NOT_SUPPORTED,
    SERVER_TIMEOUT = 10,
    CLEANER_TIMEOUT = 1000
};
using namespace progbase::net;

typedef struct {
    std::thread *thread;
    std::mutex alive;
    TcpClient *client;
    int id;
} Processor;

typedef struct {
    int method;
    std::string path;
    TcpClient *client;
} HTTPRequest;

typedef struct {
    std::string header;
    int status;
    std::string answer;
    TcpClient *client;
} HTTPResponse;

class Server {
    TcpListener *listener;
    IpAddress *address;
    std::thread *serverThread;
    bool isAlive;
    std::mutex processorsMutex;
    std::mutex sender;
    std::vector<Processor *> userReqProcessors;
    std::string serverName;
    std::string developer;
    CounriesStorage *storage;

    Server(TcpListener *listener, IpAddress *address, const std::string &server, const std::string &dev,
           CounriesStorage *storage);

    void exec();

    void acceptClient(TcpClient **client, bool *connected);

    void connectProxyClient();

    void processClientRequest(Processor *processor);

    HTTPRequest *getRequest(TcpClient *client);

    void addClient(TcpClient *client);

    void processorsClean();

    void freeProcessor(Processor *p);

    void cleaner(bool *status, int timeout);

    std::string getDate();

    std::string headerHTTP(int errorCode);

public:
    static Server *
    create(int port, const std::string &serverName, const std::string &developer, const std::string &filename);

    bool start();

    bool status();

    ~Server();

    void stop();

    int port();

    std::string ip();

    std::string toString();

    std::string getPath(const std::string &req);

    std::string getRequestStr(TcpClient *client);

    int getMethod(const std::string &request);

    void sendString(TcpClient *client, const std::string &str);

    void sendResponse(HTTPResponse *response);

    HTTPResponse *getResponse(HTTPRequest *request);

    std::string get(const std::string &path);

    std::string getErrorByCode(int code);

    bool isValidCharacter(char c);

    std::string info();

    std::string getItems(const std::string &path);
};


#endif //LAB8_SERVER_H
