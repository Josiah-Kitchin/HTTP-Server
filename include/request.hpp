
#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>  
#include <iostream> 
using namespace std; 

struct Request { 
    string method; 
    string route; 
    bool valid; 
};

//For logging purposes 
std::ostream& operator<<(std::ostream&, const Request&); 

#endif