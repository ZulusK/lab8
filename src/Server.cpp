//
// Created by zulus on 11.05.17.
//

#include <Server.h>
#include <iostream>
#include <vector>

using namespace std;

Server::Server(TcpListener *listener, IpAddress *address, const string &server, const string &developer,
               CounriesStorage *storage) {
    this->listener = listener;
    this->address = address;
    this->isAlive = false;
    this->developer = developer;
    this->serverName = server;
    this->serverThread = NULL;
    this->storage = storage;

    cout << this->storage->get("id", "131") << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << this->storage->get("name", "") << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << this->storage->get("errr", "Philippines") << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    srand(time(NULL));
}

Server *Server::create(int port, const std::string &serverName, const std::string &developer, const string &filename) {
    CounriesStorage *storage = CounriesStorage::load(filename);
    if (storage == NULL) {
        return NULL;
    }
    auto address = new IpAddress("127.0.0.1", port);
    auto listener = new TcpListener();
    try {
        listener->bind(*address);
    } catch (NetException e) {
        cout << e.what() << endl;
        delete listener;
        delete address;
        return NULL;
    }

    return new Server(listener, address, serverName, developer, storage);
}

void Server::stop() {
    if (isAlive) {
        this->isAlive = false;
        //wait for end of server thread
        this->serverThread->join();
        delete serverThread;
        this->serverThread = NULL;
    }
}

void Server::processorsClean() {
    processorsMutex.lock();
    for (auto it = userReqProcessors.begin(); it != userReqProcessors.end(); it++) {
        Processor *p = *it;
        p->alive.lock();
        freeProcessor(p);
    }
    userReqProcessors.clear();
    processorsMutex.unlock();

}

void Server::freeProcessor(Processor *p) {
    delete p->thread;
    delete p->client;
    delete p;
}

void Server::addClient(TcpClient *client) {
    Processor *p = new Processor();
    p->alive.lock();
    p->client = client;
    p->thread = new thread(&Server::processClientRequest, this, p);
    p->thread->detach();
    p->alive.unlock();

    processorsMutex.lock();
    userReqProcessors.push_back(p);
    processorsMutex.unlock();
}

void Server::cleaner(bool *status, int timeout) {
    while (*status) {
        int i = 0;
        int size = userReqProcessors.size();
        vector<int> rmInd;
        for (auto it = userReqProcessors.begin(); i < size && it != userReqProcessors.end() && *status;) {
            auto next = it + 1;
            i++;
            //delete thread
            Processor *p = *it;
            //if it cancel execute
            if (p->alive.try_lock()) {
                processorsMutex.lock();
                freeProcessor(p);
                userReqProcessors.erase(it);
                processorsMutex.unlock();
            }
            it = next;
        }
//        cout << "Clients processing: " << userReqProcessors.size() << endl;
        this_thread::sleep_for(chrono::milliseconds(timeout));
    }
}

void Server::exec() {
    cout << "waiting for connection " << this->address->address() << ":" << this->address->port() << " ..." << endl;
    listener->start();
    bool connected = false;
    bool clean = true;
    //client to connect
    TcpClient *client = NULL;
    //thread for not-blocking listening accepting clients
    thread *acceptingThread = new thread(&Server::acceptClient, this, &client, &connected);
    //start garbage collector
    thread cleaner(&Server::cleaner, this, &clean, CLEANER_TIMEOUT);

    while (isAlive) {
        //if someone connect to server
        if (connected) {
            //not-blocking processing of client request
            addClient(client);
            acceptingThread->detach();
            delete acceptingThread;
            //run thread again
            if (isAlive)
                acceptingThread = new thread(&Server::acceptClient, this, &client, &connected);
            else break;
        }
        this_thread::sleep_for(chrono::milliseconds(SERVER_TIMEOUT));
    }
    //connect client, to terminate thread
    connectProxyClient();

    //end cleaner
    clean = false;
    cleaner.join();

    //wait to end of thread
    acceptingThread->join();

    //clear references
    delete acceptingThread;
    if (client) {
        delete client;
    }

    //clear all threads
    processorsClean();
}

void Server::connectProxyClient() {
    TcpClient client;
    try {
        client.connect(*address);
        NetMessage msg(10);
        msg.setDataString("stop");
        client.send(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}

bool Server::status() {
    return isAlive;
}

bool Server::start() {
    if (!isAlive) {
        this->isAlive = true;
        try {
            this->serverThread = new thread(&Server::exec, this);
        } catch (NetException e) {
            cout << e.what() << endl;
            return false;
        }
        return true;
    } else {
        return false;
    }
}

void Server::acceptClient(TcpClient **client, bool *connected) {
    *connected = false;
    *client = listener->accept();
    *connected = true;
}

string Server::getRequest(TcpClient *client) {
    NetMessage message(1024);
    string req = "";
    try {
        client->receive(message);
        req += message.dataAsString();
    } catch (NetException e) {
        cout << e.what() << endl;
    }
    return req;
}

void Server::sendAnswer(TcpClient *client, const string &str) {
    NetMessage msg(2048);
    msg.setDataString(str);
    try {
        client->send(msg);
    } catch (NetException e) {
        cout << e.what() << endl;
    }
}

Server::~Server() {
    stop();
    if (serverThread)
        delete serverThread;
    delete listener;
    delete address;
    delete this->storage;
}

int Server::port() {
    return address->port();
}

string Server::ip() {
    return address->address();
}

bool isValidCharacter(char c) {
    return isalnum(c) || c == '?' || c == '!' || c == '/' || c == '+' || c == '-' || c == '.';
}

string Server::getPathFromReq(const string &req) {
    string path = "/";
    int pos = req.find("GET") + 4;
    int pathPos = req.find("/", pos);
    int httpPos = req.find("HTTP/1.");
    if (pos == -1 || pos > req.length() || httpPos == req.npos) {
        return "";
    }
    if (pathPos == req.npos || pathPos < pos || pathPos > req.find("HTTP/1.")) {
        return "";
    }
    int i;
    for (i = pathPos + 1; i < httpPos; i++) {
        if (!isValidCharacter(req[i])) {
            break;
        }
    }
    path = req.substr(pathPos, i - pathPos);
    return path;
}

void Server::processClientRequest(Processor *processor) {
    processor->alive.lock();
    cout << "Connection success ... " << endl;
    string userReq = getRequest(processor->client);
    string path = getPathFromReq(userReq);
    string answer = processPathReq(path);
    cout << answer << endl;
    sendAnswer(processor->client, answer);
    cout << "Send answer ... " << endl;
    processor->alive.unlock();
}

string Server::getDate() {
    std::time_t rawtime;
    std::tm *timeinfo;
    char buffer[80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
    return buffer;
}

string Server::createHTTPHeader(int errorCode) {
    auto http = "HTTP/1.1 " + to_string(errorCode) + " ";
    switch (errorCode) {
        case 400:
            http += "Bad Request";
            break;
        case
            401:
            http += "Unauthorized";
            break;
        case 403:
            http += "Forbidden";
            break;
        case 404:
            http += "Not Found";
            break;
        case 410:
            http += "Gone";
            break;
        case 413:
            http += "Request Entity Too Large";
            break;
        case 414:
            http += "Request-URI Too Large";
            break;
        case 415:
            http += "Unsupported Media Type";
            break;
        case 416:
            http += "Requested Range Not Satisfiable";
            break;
        case 417:
            http += "Expectation Failed";
            break;
        case 423:
            http += "Locked";
            break;
    }
    http += "\r\n";
    http += "Date: " + getDate() + "\r\n";
    return http;
}

string Server::processPathReq(const string &path) {
    try {
        if (path.length() == 0) {
            throw 400;
        }
        if (path.compare("/") == 0) {
//            return createHTTPHeader(getRoot());
        }
        if (path.find("/favourites") == 0) {
//            return createHTTPHeader(getFavourites(path.substr(10)));
        }
        if (path.find("/file") == 0) {
//            return createHTTPHeader(getFile(path.substr(5)));
        }
        throw 404;
    } catch (int e) {
        return createHTTPHeader(e);
    }
}