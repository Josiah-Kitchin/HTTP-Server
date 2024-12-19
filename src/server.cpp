
#include <iostream> 
#include <stdexcept>
#include "server.hpp"
#include "config.hpp"
#include "client.hpp"
#include "req_res.hpp"
#include <filesystem> 
#include <fstream>
#include <sstream>

    
using namespace uoserve; 
/* ---------------------- INTERFACE FUNCTIONS ----------------------- */

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

void Server::run() { 
    while (true) { 
        Client client(socket_fd); 
        Request request = get_request(client.socket_fd);
        send_response(client, request);
    }
}

/* Server route setters */

void Server::GET(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[array<string, 2>{"GET", route}] = route_function;
}

void Server::UNMATCHED(void(*route_function)(const Request&, Response&)) { 
    route_mapper[array<string, 2>{"ANY", "UNMATCHED"}] = route_function; 
}

/* Server function utils */

string Server::get_page(const string& relative_file_path) { 
    /* Returns a string of the pages contents 
       Paths are searched relative to the page_dir string in the object 
       It is not recurisve, the full path starting from page_dir must be given */

    namespace fs = std::filesystem; 
    fs::path page_dir = config.serve_dir; 
    try { 
        for (const auto& entry: fs::directory_iterator(page_dir)) { 
            cout << "serching " << entry.path() << endl;
            if (entry.is_regular_file() && entry.path().filename() == relative_file_path) { 
                ifstream file(entry.path());
                stringstream buffer; 
                buffer << file.rdbuf(); 
                cout << "found " << buffer.str() << endl;
                return buffer.str(); 
            }
        }
        return "";
    } catch (const fs::filesystem_error& error) { 
        return "";
    }
}


/* ------------------ PRIVATE FUNCTIONS ---------------------- */

Server::~Server() { 
    close(socket_fd);
} 

Request Server::get_request(int client_socket_fd) { 
    char buffer[config.request_bytes_limit];
    recv(client_socket_fd, buffer, config.request_bytes_limit, 0);
    return build_request(buffer);
}


void Server::send_response(const Client& client, const Request& request) { 
    if (!request.valid) { 
        string invalid_response = build_400_response(); 
        send(client.socket_fd, invalid_response.c_str(), invalid_response.length(), 0);
    }

    Response response; 
    string response_str; 

    if (route_mapper.find({ request.method, request.route }) != route_mapper.end()) { 
        //handle the request to a user set function corresponding to the route
        void(*handle_request)(const Request&, Response&) = route_mapper[{ request.method, request.route }];
        handle_request(request, response);
        response_str = build_response(response);
    } else { 
        if (route_mapper.find({ "ANY", "UNMATCHED" }) != route_mapper.end()) { 
            //handle the request to a user set function correspoinding to unmatched routes 
            void(*handle_request)(const Request&, Response&) = route_mapper[{ "ANY", "UNMATCHED" }];
            handle_request(request, response);
            response_str = build_response(response);
            send(client.socket_fd, response_str.c_str(), response_str.length(), 0);
        } else { 
            //if the user has not set an unmatched route, send a default 404
            response_str = build_404_response(); 
        }
    }
    send(client.socket_fd, response_str.c_str(), response_str.length(), 0);
}
