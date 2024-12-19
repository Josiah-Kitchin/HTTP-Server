
#include <iostream> 
#include <stdexcept>
#include "server.hpp"
#include "config.hpp"
#include "client.hpp"
#include "request_builder.hpp"
#include "response_builder.hpp"
#include <filesystem> 
#include <fstream>
#include <sstream>



Server::Server(const ServerConfig& user_config) {
    /* Initalize the server by creating a socket, binding the server to the address and port then 
       listening to the socket */

    config = user_config; 

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) { 
        throw std::runtime_error("Server failed to create a socket");
    }
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons(config.port);

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
    char buffer[config.request_bytes_limit];
    recv(client_socket_fd, buffer, config.request_bytes_limit, 0);
    return build_request(buffer);
}



void Server::run() { 
    while (true) { 
        Client client(socket_fd); 
        Request request = get_request(client.socket_fd);
        if (route_mapper.find({ request.method, request.route }) == route_mapper.end()) { 
            cout << "Invalid request" << endl; 
        } else {
            void(*handle_request)(const Request&, Response&) = route_mapper[{ request.method, request.route }];
            Response response; 
            handle_request(request, response);
            string response_str = build_response(response);
            cout << "response string: " << response_str << endl;
            cout << "response c string: " << response_str.c_str();
            send(client.socket_fd, response_str.c_str(), response_str.length(), 0);
        }
    }
}

void Server::GET(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[array<string, 2>{"GET", route}] = route_function;
}

string Server::get_page(const string& relative_file_path) { 
    /* Returns a string of the pages contents */
    namespace fs = std::filesystem; 
    fs::path page_dir = config.serve_dir; 
    try { 
        for (const auto& entry: fs::recursive_directory_iterator(page_dir)) { 
            cout << "serching " << entry.path() << endl;
            if (entry.is_regular_file() && entry.path().filename() == relative_file_path) { 
                ifstream file(entry.path());
                stringstream buffer; 
                buffer << file.rdbuf(); 
                cout << "found " << buffer.str() << endl;
                return buffer.str(); 
            }
        }
        cout << "found nothing " << endl;
        return "";
    } catch (const fs::filesystem_error& error) { 
        cout << "error " << error.what() << endl;
        return "";
    }
}



