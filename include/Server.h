/** @file
    @brief HTTP server
*/

#ifndef LAB8_SERVER_H
#define LAB8_SERVER_H

#include <progbase-cpp/net.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <CountriesStorage.h>

/**
 @enum
 @brief usefull constatn
 */
enum {
    MAX_RESPONSE_LEN = 2048,
    MAX_REQUEST_LEN = 1024,
    GET = 0xFFFF,
    NOT_SUPPORTED,
    SERVER_TIMEOUT = 10,
    CLEANER_TIMEOUT = 1000
};

using namespace progbase::net;
/**
 @struct
 @brief defines a client's session process
 */
typedef struct {
    std::thread *thread;
    std::mutex alive;
    TcpClient *client;
    int id;
} Session;

/**
 @struct
 @brief defines a HTTP client request
 */
typedef struct {
    int method;
    std::string path;
    TcpClient *client;
} HTTPRequest;

/**
 @struct
 @brief defines a HTTP server response
 */
typedef struct {
    std::string header;
    int status;
    std::string answer;
    TcpClient *client;
} HTTPResponse;

/**
 @class
 @brief defines a HTTP server
 */
class Server {
    TcpListener *listener;
    IpAddress *address;
    std::thread *serverThread;
    bool isAlive;
    std::mutex processorsMutex;
    std::mutex sender;
    std::vector<Session *> userReqProcessors;
    std::string serverName;
    std::string developer;
    CountriesStorage *storage;
    std::string filename;

    /**
     @brief private constructor of HTTP server
     @param listener reference to TCP listener
     @param address address of server ot listen
     @param server  name of server
     @param dev name of developer
     @param storage path to JSON-country storage
     @param filename path to text data storage
     */
    Server(TcpListener *listener, IpAddress *address, const std::string &server, const std::string &dev,
           CountriesStorage *storage, const std::string &filename);

    /**
     @brief execute server( blocking call)
     */
    void exec();

    /**
     @brief wait for next client (blocking call)
     @param client double reference to TCPClient to write a binded client
     @param connected reference to variable, that represent status of method
     */
    void acceptClient(TcpClient **client, bool *connected);

    /**
     @brief connect empty client  to stop method acceptClient
     */
    void connectProxyClient();

    /**
     @brief processing client's request to server
     @param processor reference to session with client
     */
    void processClientRequest(Session *processor);

    /**
     @brief get HTTP request of client
     @param client reference to client
     @return HTTPRequest - parsed HTTP client's request
     */
    HTTPRequest *getRequest(TcpClient *client);

    /**
     @brief init new session with client, add it to queue
     @param client reference to new binded client
     */
    void addClient(TcpClient *client);

    /**
     @brief wait(cancel) and clear all client's session (blocking call)
     */
    void cleanSessions();

    /**
     @brief delete client's session
     @param p reference to session
     */
    void freeSession(Session *p);

    /**
     @brief garbage collector, deletes ended client's sessions
     @param status pointer to flag (true - cleaner works, false - stop, return control)
     @param timeout - timeout between loop iteration
     */
    void cleaner(bool *status, int timeout);

    /**
     @brief get current time and date at server
     @return string-presenattion of current server's date
     */
    std::string getDate();

    /**
     @brief create HTTP header by standard error code( 2xx,4xx)
     @param errorCode HTTP error code
     @return  string - header's part with error status
     */
    std::string headerHTTP(int errorCode);

    /**
     @brief get text data storage info
     @return JSON-string (filename, size, text)
     */
    std::string fileInfo();

    /**
     @brief get text data info
     @return JSON-string (filename, max, count)
     */
    std::string dataInfo();

    /**
     @brief get path from client's HTTP request's header(GET supported)
     @param req  string HTTP request
     @return parsed path
     */
    std::string getPath(const std::string &req);

    /**
     @brief listen client and his request
     @param client reference to TCPClient
     @return client's request
     */
    std::string getRequestStr(TcpClient *client);

    /**
     @brief get method from client's request
     @param request string request of user
     @return enum GET or  NOT_SUPPORTED
     */
    int getMethod(const std::string &request);

    /**
     @brief send string response to client
     @param client reference to client
     @param str serverresponse
     */
    void sendString(TcpClient *client, const std::string &str);

    /**
     @brief send server response to client
     @param response  reference to server response
     */
    void sendResponse(HTTPResponse *response);

    /**
     @brief process client request and returns server response
     @param request reference to client request
     @return reference to created response
     */
    HTTPResponse *getResponse(HTTPRequest *request);

    /**
     @brief get data by path from client's request
     @param path path to data
     @return empty string, if path didn't find, or JSON-string with data
     */
    std::string get(const std::string &path);

    /**
     @brief get error string by it's error
     @param code 2xx or 4xx HTTP error code
     @return string - text of HTTP error
     */
    static std::string getErrorByCode(int code);

    /**
     @brief get server info in JSON-string
     @return current server information as JSON-string ( name, time, developer)
     */
    std::string info();

    /**
     @brief get items from CountryStorage by path
     @param path path to data ( request should start with /favorite/
     @return empty string if path didn't find, or data from path
     */
    std::string getItems(const std::string &path);

    /**
     @brief send response to client by chunk, if it too large
     @param response reference to server response
     */
    void sendChunkedResponse(HTTPResponse *response);

public:
    /**
     @brief init new HTTP server (fabric method)
     @param port positive integer port for server
     @param serverName name of server
     @param developer name of server developer
     @param filename country JSON-file path
     @param dataFilename data text storage path
     @return if success created server, else - false
     */
    static Server *
    create(int port, const std::string &serverName, const std::string &developer, const std::string &filename,
           const std::string &dataFilename);

    /**
     @brief start executing server at his own thread
     @return true, if success, else - false
     */
    bool start();

    /**
     @brief get server executing status
     @return  true - working, false - stopped
     */
    bool status();

    /**
     @brief destructor of server
     */
    ~Server();

    /**
     @brief try to stop server, wait while server working
     */
    void stop();

    /**
     @brief get port of server
     @return integer-value - port at which server started
     */
    int port();

    /**
     @brief get ip of server
     @return ipv4 at which server started
     */
    std::string ip();

    /**
     @brief get string presenattion of server
     @return  string presenattion of server
     */
    std::string toString();

    /**
     @brief check, is character is correct for server
     @param c - character to check
     @return true, if c is correct, else - false
     */
    static bool isValidCharacter(char c);
};


#endif //LAB8_SERVER_H
