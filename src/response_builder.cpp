

#include "response_builder.hpp"
#include <iostream> 


using namespace std; 
string build_response(const Response& response) { 
    string response_str =
        response.version + " " + 
        to_string(response.status) + " OK\r\n" + 
        "Content-Type: text/html\r\n\r\n" +
        response.body; 
    return response_str; 
}