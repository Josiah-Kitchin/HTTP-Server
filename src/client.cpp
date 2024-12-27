

#include "client.hpp"

using namespace uoserve; 
Client::Client() { 
    address_len = sizeof(address);
    socket_fd = -1; 
}

Client::Client(Client&& other) noexcept { 
    address_len = other.address_len;
    socket_fd = other.socket_fd;
    other.socket_fd = -1; 
}

Client& Client::operator=(Client&& other) noexcept { 
    if (this != &other) { 
        close(socket_fd);
        address_len = other.address_len; 
        socket_fd = other.socket_fd;
    }
    return *this;
}

void Client::wait_to_accept(int server_socket_fd) {
    socket_fd = accept(server_socket_fd, (struct sockaddr*)&address, &address_len); 
}

Client::~Client() { 
    close(socket_fd);
}
