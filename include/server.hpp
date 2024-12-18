
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string> 
#include <cstring> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 

class Server { 

public: 
    Server(int port);
    ~Server(); 
    void run(); 
    
private: 
    int socket_fd; 
    sockaddr_in server_address; 
};



#endif 