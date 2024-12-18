
#include "request_builder.hpp"
#include <stdexcept>
#include <array>

#define INVALID "INVALID"

Request request_builder::build_request(const char* request_buffer) { 
    Request request; 
    try { 
        string request_str(request_buffer);
        
        request.method = get_method(request_str);
    
    
    } catch (const std::exception& error) { 
        
    }
}


string get_method(const string& request) { 
    std::array<string, 4> methods{"GET"};
    for (const auto& method : methods) { 
        if (request.compare(0, method.length(), method) == 0) { 
            return method; 
        }
    }
    throw std::runtime_error("Invalid request");
}

string get_content_type(const string& request) { 


}