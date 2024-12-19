
#include "req_res.hpp"
#include <stdexcept>
#include <array>
#include <vector> 

using namespace uoserve; 

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


std::ostream& operator<<(std::ostream& out, const Request& request) { 
    out << "Method: " << request.method << '\n';
    out << "Route: " << request.route << '\n'; 
    return out; 
}

std::string get_status_message(int status_code) {
    switch (status_code) {
        case 100: return "Continue";
        case 200: return "OK"; 
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";

        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "Unused";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";

        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Range Not Satisfiable";
        case 417: return "Expectation Failed";

        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";

        default: return "Unknown Status Code";
    }
}

string build_response(const Response& response) { 
    /* Returns a string of a response object complient with HTTP */

    string message = get_status_message(response.status);
    string response_str =
        response.version + " " + 
        to_string(response.status) + " " + message + "\r\n" + 
        "Content-Type: text/html\r\n\r\n" +
        response.body; 
    return response_str; 
}


string build_404_response() { 
     return "HTTP/1.0 404 Not Found\r\n"
           "Content-Type: text/html\r\n"
           "Content-Length: 48\r\n"
           "Connection: close\r\n"
           "\r\n"
           "<html><body><h1>404 Not Found</h1></body></html>\r\n";
}

string build_400_response() { 
    return  "HTTP/1.0 400 Bad Request\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 58\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>400 Bad Request</h1><p>Your request is invalid.</p></body></html>\r\n";
}