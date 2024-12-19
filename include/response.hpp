
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string> 
using namespace std; 

struct Response { 
    string version = "HTTP/1.1";
    int status = 200; 
    string body; 
};

#endif