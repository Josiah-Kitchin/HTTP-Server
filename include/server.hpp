
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string> 
#include <cstring> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "request.hpp"
#include "response.hpp"
#include <map> 
#include <array> 
#include "config.hpp"


using namespace std; 
class Server { 

public: 
    Server(const ServerConfig&);  
    ~Server();
    void run(); 
    void GET(const string&, void(*)(const Request&, Response&));
    string get_page(const string&);

private: 
    ServerConfig config; 
    int socket_fd; 
    sockaddr_in server_address; 
    Request get_request(int); 
    //Routes an array of Method and Route to functions set by the user 
    map<array<string, 2>, void(*)(const Request&, Response&)> route_mapper; 

};



#endif 