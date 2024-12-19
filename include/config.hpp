
#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

struct ServerConfig { 
    int port = 8080; 
    int request_bytes_limit = 1500;
    string serve_dir = ""; 
};

#endif