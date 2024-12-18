
#include "request_builder.hpp"
#include <stdexcept>
#include <array>
#include <vector> 



bool is_valid_route(const string& route) { 
    std::array<std::string, 7> forbidden_chars = {
        " ",   
        "\t", 
        "\n",
        "\r", 
        "*",  
        "~", 
        "..",
    };
    for (string ch : forbidden_chars) { 
        if (route.find(ch) != string::npos) { 
            return false; 
        }
     }
     return true; 
}

Request build_request(char* request_buffer) { 
    /* The position of some data depends on the position of other data, so all parsing is set to one function 
       to show the progression of gathering data from the request 

       GET /index.html HTTP/1.0
       finds GET
       finds HTTP/1.0
       uses the positions of the method and version to find the route 

       if the request is invalid, it will return a request object with the valid flag set to false
       
    */ 
    Request request; 
    string request_str(request_buffer);
    std::array<string, 1> methods{"GET"};
    bool method_found = false; 
    for (const auto& method : methods) { 
        if (request_str.compare(0, method.length(), method) == 0) { 
            request.method = method; 
            method_found = true; 
        }
    }
    if (!method_found) {
        return request; 
    }

    size_t version_position = request_str.find(" HTTP/", request.method.length());
    if (version_position == string::npos) {
        return request; 
    }

    string route = request_str.substr(request.method.length() + 1, version_position - request.method.length() - 1);
    if (!is_valid_route(route)) { 
        return request; 
    }
    request.route = route; 

    request.valid = true; 
    return request; 
}




