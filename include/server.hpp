
#ifndef SERVER_HPP
#define SERVER_HPP

#include <string> 
#include <cstring> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h> 
#include "req_res.hpp"
#include <map> 
#include <array> 
#include <vector> 
#include "config.hpp"
#include "client.hpp"


using namespace std; 
namespace uoserve {

    class Server { 
    /* The server object should be initialized once globally and will handle all server configurations and operations*/

    public: 
    /* Initialization: 
        The server initalizes the socket and binds the address 
        A ServerConfig object will be passed in for specific configurations such as port */

        Server(const ServerConfig&);  
        ~Server();
        void run(); 

    /* Route handler function setters: 
        The user will use methods to set user defined functions to a method and a route. 
        For example, server.GET("/route", handle_get) will set the handle_get function to be called on the 
        request and response if a get request is sent to /route */

        void GET(const string&, void(*)(const Request&, Response&));
        void POST(const string&, void(*)(const Request&, Response&));
        void HEAD(const string&, void(*)(const Request&, Response&));
        void PUT(const string&, void(*)(const Request&, Response&));
        void DELETE(const string&, void(*)(const Request&, Response&));
        void OPTIONS(const string&, void(*)(const Request&, Response&));
        void TRACE(const string&, void(*)(const Request&, Response&));
        void UNMATCHED(void(*)(const Request&, Response&));
        //Calls a userdefined function on every request before it reaches the function mapped to the given route 
        void add_middleware(void(*)(Request&));

    /* Route handler utils: 
        Server methods that can be used in user defined route handlers to help serve content */

        //Returns a string of the contents of a file with a given path, relative to the path given 
        //for page_dir in the ServerConfig object at initialization 
        string get_page(const string&);



    private: 
        ServerConfig config; 
        int socket_fd = -1; 
        sockaddr_in server_address; 
        Request get_request(int); 

        //Routes an array of an http methods and routes to functions set by the user 
        map<array<string, 2>, void(*)(const Request&, Response&)> route_mapper; 

        //A vector of middleware so that each middleware function can be called on the request before reaching the route mapper
        vector<void(*)(Request&)> middleware; 

        void send_response(const Client&, Request&);

        //For graceful interupt, allows for Ctrl C to close the server socket 
        static void handle_sigint(int); 
        void close_socket();
        
        

    };
}


#endif 