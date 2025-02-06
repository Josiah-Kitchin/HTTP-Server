
#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP
#include <string> 

namespace uoserve 
{ 
    /* Passed in at intialization to the server */
    struct ServerConfig 
    { 
        int port = 8080; 
        int request_bytes_limit = 1500;
        std::string serve_dir = ""; 
    };
}

#endif
