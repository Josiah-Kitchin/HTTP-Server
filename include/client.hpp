
#ifndef CLINET_HPP
#define CLINET_HPP

#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 

class Client { 
public: 
    Client(int); 
    ~Client(); 
    int socket_fd;
    sockaddr_in address; 
    socklen_t address_len;
};

#endif




