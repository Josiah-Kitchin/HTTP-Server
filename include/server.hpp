
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


class Server { 

public: 
    Server(int port); //Initalizes the server on a given port 
    ~Server();
    void run(); 
    void get(const string&, void(*)(const Request&, Response&));
    
private: 
    int socket_fd; 
    sockaddr_in server_address; 
    Request get_request(int); 
    std::map<std::array<std::string, 2>, void(*)(const Request&, Response&)> route_mapper; 
    
};



#endif 