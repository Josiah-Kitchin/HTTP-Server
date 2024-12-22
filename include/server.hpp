
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

    public: 
        Server(const ServerConfig&);  
        ~Server();
        void run(); 
        void GET(const string&, void(*)(const Request&, Response&));
        void POST(const string&, void(*)(const Request&, Response&));
        void HEAD(const string&, void(*)(const Request&, Response&));
        void PUT(const string&, void(*)(const Request&, Response&));
        void DELETE(const string&, void(*)(const Request&, Response&));
        void OPTIONS(const string&, void(*)(const Request&, Response&));
        void TRACE(const string&, void(*)(const Request&, Response&));

        void UNMATCHED(void(*)(const Request&, Response&));
        string get_page(const string&);

        void add_middleware(void(*)(Request&));

    private: 
        ServerConfig config; 
        int socket_fd; 
        sockaddr_in server_address; 
        Request get_request(int); 
        //Routes an array of Method and Route to functions set by the user 
        map<array<string, 2>, void(*)(const Request&, Response&)> route_mapper; 
        vector<void(*)(Request&)> middleware; 
        void send_response(const Client&, Request&);

    };
}


#endif 