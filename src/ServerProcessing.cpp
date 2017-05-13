#include <Server.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <TextProcessor.h>

#define HEND "\r\n"
#define HHEND "\r\n\r\n"
using namespace std;

string Server::getErrorByCode(int code) {
    switch (code) {
        case 200:
            return "OK";
            break;
        case 201:
            return "Created";
            break;
        case 202 :
            return "Accepted";
            break;
        case 203 :
            return "Non-Authoritative Information";
            break;
        case 204 :
            return "No Content";
            break;
        case 205 :
            return "Reset Content";
            break;
        case 206 :
            return "Partial Content";
            break;
        case 207 :
            return "Multi-Status";
            break;
        case 226 :
            return "IM Used";
            break;
        case 400:
            return "Bad Request";
            break;
        case 401:
            return "Unauthorized";
            break;
        case 402:
            return "Payment Required ";
            break;
        case 403:
            return "Forbidden";
            break;
        case 404:
            return "Not Found";
            break;
        case 405:
            return "Method Not Allowed";
            break;
        case 406:
            return "Not Acceptable";
            break;
        case 407:
            return "Proxy Authentication Required";
            break;
        case 408:
            return "Request Timeout";
            break;
        case 409:
            return "Conflict";
            break;
        case 410:
            return "Gone";
            break;
        case 411:
            return "Length Required";
            break;
        case 412:
            return "Precondition Failed";
            break;
        case 413:
            return "Request Entity Too Large";
            break;
        case 414:
            return "Request-URI Too Large";
            break;
        case 415:
            return "Unsupported Media Type";
            break;
        case 416:
            return "Requested Range Not Satisfiable";
            break;
        case 417:
            return "Expectation Failed";
            break;
        case 422:
            return "Unprocessable Entity";
            break;
        case 423:
            return "Locked";
            break;
        case 424:
            return "Failed Dependency";
            break;
        case 425:
            return "Unordered Collection";
            break;
        case 426:
            return "Upgrade Required";
            break;
        case 428:
            return "Precondition Required";
            break;
        case 429:
            return "Too Many Requests";
            break;
        case 431:
            return "Request Header Fields Too Large";
            break;
        case 449:
            return "Retry With";
            break;
        case 451:
            return "Unavailable For Legal Reasons";
            break;
        default:
            return "Not Found";
            break;
    }
}

bool Server::isValidCharacter(char c) {
    return isalnum(c) || c == '?' || c == '!' || c == '/' || c == '+' || c == '-' || c == '.' || c == '=';
}

void Server::processClientRequest(Processor *processor) {
    processor->alive.lock();
    cout << "Connection  success [" << processor->id << "]" << endl;

    HTTPRequest *request = getRequest(processor->client);
    HTTPResponse *response = getResponse(request);
    sendResponse(response);
    delete response;
    delete request;

    cout << "Response sent [" << processor->id << "]" << endl;
    processor->alive.unlock();
}

string Server::getPath(const string &req) {
    string path = "/";
    int pos = req.find("GET") + 4;
    int pathPos = req.find("/", pos);
    int httpPos = req.find("HTTP/1.");
    if (pos == -1 || pos > req.length() || httpPos == req.npos) {
        return "";
    }
    if (pathPos == req.npos || pathPos < pos || pathPos > httpPos) {
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

string Server::getDate() {
    std::time_t rawtime;
    std::tm *timeinfo;
    char buffer[80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
    return buffer;
}

string Server::headerHTTP(int errorCode) {
    auto http = "HTTP/1.1 " + to_string(errorCode) + " " + getErrorByCode(errorCode) + HEND;
    http += "Date: " + getDate() + HEND;
    http += "Server: " + serverName + "/" + developer + HEND;
    if (errorCode >= 200 && errorCode < 400) {
        http += string("Content-Type: text/json");
    }
    return http;
}

string Server::toString() {
    string s = "Server <" + serverName + "> created by " + developer + "\n";
    s += "run at ip " + string(address->address()) + ":" + to_string(address->port()) + "\n";
    s += "item at storage " + to_string(storage->size()) + "\n";
    s += "current clients " + to_string(this->userReqProcessors.size());
    return s;
}

HTTPRequest *Server::getRequest(TcpClient *client) {
    string requestStr = getRequestStr(client);
    HTTPRequest *request = new HTTPRequest();
    request->method = getMethod(requestStr);
    request->path = getPath(requestStr);
    request->client = client;
    return request;
}

string Server::info() {
    auto jobj = json_object();
    json_object_set_new(jobj, "title", json_string(serverName.c_str()));
    json_object_set_new(jobj, "developer", json_string(developer.c_str()));
    json_object_set_new(jobj, "time", json_string(getDate().c_str()));
    // create JSON document string
    char *jsonString = json_dumps(jobj, JSON_INDENT(2));
    string jstr(jsonString);
    free(jsonString);
    return jstr;
}

inline bool isInteger(const std::string &s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

    char *p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

string Server::getItems(const string &path) {
    string key = path.substr(0, path.find('='));
    if (key.length() + 1 >= path.length() && isInteger(key)) {
        return storage->get(stoi(key));
    } else if (key.length() + 1 < path.length()) {
        string val = path.substr(path.find("=") + 1);
        return storage->get(key, val);
    }
    return "";
}

string Server::fileInfo() {
    auto jobj = json_object();
    string text = TextProcessor::read(this->filename);
    long size = TextProcessor::fileSize(this->filename);
    json_object_set_new(jobj, "filename", json_string(filename.c_str()));
    json_object_set_new(jobj, "size", json_integer(size));
    json_object_set_new(jobj, "text", json_string(text.c_str()));

    char *jsonString = json_dumps(jobj, JSON_INDENT(2));
    string jstr(jsonString);
    free(jsonString);
    return jstr;
}

string Server::dataInfo() {
    auto jobj = json_object();
    string text = TextProcessor::read(this->filename);
    long max;
    long count;
    TextProcessor::process(text, max, count);
    json_object_set_new(jobj, "filename", json_string(filename.c_str()));
    json_object_set_new(jobj, "max", json_integer(max));
    json_object_set_new(jobj, "count", json_integer(count));

    char *jsonString = json_dumps(jobj, JSON_INDENT(2));
    string jstr(jsonString);
    free(jsonString);
    return jstr;
}

string Server::get(const string &path) {
    if (path.compare("/") == 0) return info();
    if (path.compare("/file") == 0)return fileInfo();
    if (path.compare("/file/data") == 0)return dataInfo();
    if (path.find("/favorites?") == 0) return getItems(path.substr(11));
    if (path.compare("/favorites") == 0) return storage->get("", "");
    else return "";
}

HTTPResponse *Server::getResponse(HTTPRequest *request) {
    HTTPResponse *response = new HTTPResponse();
    response->client = request->client;
    if (request->method == GET && request->path.length() > 0) {
        response->answer = get(request->path);
        //if some error detected
        if (response->answer.length() < 4) {
            response->status = 404;
            response->header = headerHTTP(404);
            response->answer = "";
        } else {
            response->status = 200;
            response->header = headerHTTP(200);
        }
    } else {
        response->status = 405;
        response->header = headerHTTP(405);
        response->answer = "";
    }
    return response;
}

string to_hex_string(size_t a) {
    char buff[20];
    sprintf(buff, "%x", a);
    return buff;
}

void Server::sendChunkedResponse(HTTPResponse *response) {

    string header = response->header;
    size_t pos = 0;
    size_t size = 0;
    do {
        string chunk;
        //get size of the next chunk
        size = min(response->answer.length() - pos + header.length(), (size_t) MAX_RESPONSE_LEN - 70) - header.length();
        if (size == 0) {
//            chunk = header;
//            chunk += HEND;
//            chunk += "Connection: closed";
//            chunk += HHEND;
            chunk += to_hex_string(size);
            chunk += HEND;
            chunk += HEND;
        } else {
            if (header.length() > 0) {
                chunk = header;
                header = "";
                chunk += HEND;
                chunk += "Transfer-Encoding: chunked";
                chunk += HEND;
                chunk += "Connection: keep-alive";
                chunk += HHEND;
            } else {
                chunk = "";
            }
            chunk += to_hex_string(size);
            chunk += HEND;
            chunk += response->answer.substr(pos, size);
            chunk += HEND;
        }
        pos += size;
        sendString(response->client, chunk);
    } while (size != 0);
}

void Server::sendResponse(HTTPResponse *response) {
    string message = response->header + "Connection: Closed" + HHEND + response->answer;
    if (message.length() < MAX_RESPONSE_LEN) {
        sendString(response->client, message);
    } else {
        sendChunkedResponse(response);
    }
}

int Server::getMethod(const string &request) {
    if (request.find("GET ") == 0) {
        return GET;
    } else {
        return NOT_SUPPORTED;
    }
}