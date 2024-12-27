
#ifndef CLINET_HPP
#define CLINET_HPP

#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 

/* 
   Client object to be used by the server object 
   Accepts a connection from a given server socket file descripter
   The socket_fd will then be used to serve content 
*/

namespace uoserve { 
    class Client { 
    public: 
        Client(); 
        Client(Client&&) noexcept;
        Client& operator=(Client&&) noexcept;
        ~Client(); 
        int socket_fd;
        sockaddr_in address; 
        socklen_t address_len;
        void wait_to_accept(int); 
    };
    
}

#endif




