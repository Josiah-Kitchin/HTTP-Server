
#include <iostream> 
#include <stdexcept>
#include "server.hpp"
#include "config.hpp"
#include "client.hpp"



Server::Server(int port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) { 
        throw std::runtime_error("Server failed to create a socket");
    }
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons(port);

    if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) { 
        throw std::runtime_error("Server failed to bind the socket to the server address");
    }

    if (listen(socket_fd, 5) == -1) { 
        throw std::runtime_error("Server failed to listen on the socket");
    }
}

Server::~Server() { 
    close(socket_fd);
} 

void handle_request(int client_socket) { 
    const std::string http_response =  
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>Hello from C++ Web Server!</h1></body></html>";
    send(client_socket, http_response.c_str(), http_response.length(), 0); 
}

void Server::run() { 
    while (true) { 
        Client client(socket_fd); 
        char buffer[CONFIG.REQUEST_BYTES_LIMIT];
        recv(client.socket_fd, buffer, CONFIG.REQUEST_BYTES_LIMIT, 0);
        std::cout << buffer << std::endl; 
        
        handle_request(client.socket_fd);
    }
}

