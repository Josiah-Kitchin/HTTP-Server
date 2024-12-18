
#include <iostream> 
#include <stdexcept>
#include "server.hpp"
#include "config.hpp"
#include "client.hpp"
#include "request_builder.hpp"



Server::Server(int port) {
    /* Initalize the server by creating a socket, binding the server to the address and port then 
       listening to the socket */

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

Request Server::get_request(int client_socket_fd) { 
    char buffer[CONFIG.REQUEST_BYTES_LIMIT];
    recv(client_socket_fd, buffer, CONFIG.REQUEST_BYTES_LIMIT, 0);
    return build_request(buffer);
}


void Server::run() { 
    while (true) { 
        Client client(socket_fd); 
        Request request = get_request(client.socket_fd);
        array<string, 2> method_route_pair {request.method, request.route};
        void(*handle_request)(const Request&, Response&) = route_mapper[method_route_pair];
        Response response; 
        handle_request(request, response);
        const char* response_str = response.body.c_str(); 
        send(client.socket_fd, response_str, response.body.length(), 0);
    }
}

void Server::get(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[array<string, 2>{"GET", route}] = route_function;
}

