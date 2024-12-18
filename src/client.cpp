

#include "client.hpp"

Client::Client(int server_socket_fd) { 
    address_len = sizeof(address);
    socket_fd = accept(server_socket_fd, (struct sockaddr*)&address, &address_len); 
}

Client::~Client() { 
    close(socket_fd);
}
