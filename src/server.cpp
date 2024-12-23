
#include <iostream> 
#include <stdexcept>
#include "server.hpp"
#include "config.hpp"
#include "client.hpp"
#include "req_res.hpp"
#include <filesystem> 
#include <fstream>
#include <sstream>
#include <csignal> 

    
using namespace uoserve; 

//The server instance is for closing sockets on Ctrl C
static Server* server_instance = nullptr; 

/* ---------------------- INTERFACE FUNCTIONS ----------------------- */


Server::Server(const ServerConfig& user_config) {
    /* Initalize the server by creating a socket, binding the server to the address and port then 
       listening to the socket */
    signal(SIGINT, handle_sigint);

    config = user_config; 
    response_404 = default_404_response(); 

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) { 
        throw std::runtime_error("Server failed to create a socket");
    }
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = INADDR_ANY; 
    server_address.sin_port = htons(config.port);

    if (bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) { 
        throw std::runtime_error("Server failed to use port " + to_string(config.port));
    }

    if (listen(socket_fd, 5) == -1) { 
        throw std::runtime_error("Server failed to listen on the socket");
    }
    server_instance = this; 
}

void Server::run() { 
    while (true) { 
        Client client(socket_fd); 
        Request request = get_request(client.socket_fd);
        send_response(client, request);
    }
}

/* Server route and middleware setters */

void Server::GET(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"GET", route}] = route_function;
}
void Server::POST(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"POST", route}] = route_function;
}
void Server::HEAD(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"HEAD", route}] = route_function;
}
void Server::PUT(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"PUT", route}] = route_function;
}
void Server::DELETE(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"DELETE", route}] = route_function;
}
void Server::OPTIONS(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"OPTIONS", route}] = route_function;
}
void Server::TRACE(const string& route, void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"TRACE", route}] = route_function;
}
void Server::UNMATCHED(void(*route_function)(const Request&, Response&)) { 
    route_mapper[pair<string, string>{"ANY", "UNMATCHED"}] = route_function; 
}



void Server::add_middleware(void(*middleware_function)(Request&)) { 
    middleware.push_back(middleware_function);
}

void Server::set_404_page(const string& relative_page_path) { 
    string page_content = get_page(relative_page_path);
    Response response;
    response.status = 404; 
    response.body = page_content; 
    response_404 = response; 
}


/* Server function utils */

string Server::get_page(const string& relative_file_path) { 
    /* Returns a string of the pages contents 
       Paths are searched relative to the page_dir string in the object 
       It is not recurisve, the full path starting from page_dir must be given 

       Throws a runtime error if the page is not found or there is an error navigating the filesystem  */

    namespace fs = std::filesystem; 
    fs::path page_dir = config.serve_dir; 
    try { 
        for (const auto& entry: fs::directory_iterator(page_dir)) { 
            if (entry.is_regular_file() && entry.path().filename() == relative_file_path) { 
                ifstream file(entry.path());
                stringstream buffer; 
                buffer << file.rdbuf(); 
                return buffer.str(); 
            }
        }
        throw runtime_error("File not found: " + relative_file_path);
    } catch (const fs::filesystem_error& error) { 
        throw runtime_error("Filesystem error :" + string(error.what()));
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


void Server::send_response(const Client& client, Request& request) { 
    /* Send a response to a client based on the clients request */

    if (!is_valid_request(request)) { 
        string invalid_response = build_response(response_404); 
        send(client.socket_fd, invalid_response.c_str(), invalid_response.length(), 0);
    }

    //Execute middleware functions
    for (const auto& middleware_function : middleware) { 
        middleware_function(request);
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
            response_str = build_response(response_404);
        }
    }
    send(client.socket_fd, response_str.c_str(), response_str.length(), 0);
}

void Server::handle_sigint(int sig) { 
    //Closes the socket of the global pointer to the server 
    if (server_instance != nullptr) { 
        server_instance->close_socket();
    }
    exit(0);
}

void Server::close_socket() { 
    if (socket_fd != -1) {
        close(socket_fd);
    }
}




