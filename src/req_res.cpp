
#include "req_res.hpp"
#include <stdexcept>
#include <array>
#include <vector> 
#include <sstream> 

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
    /*
        Parses a http requet c string to a request object 
        
        "GET /index.html HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "\r\n"; -> Request
            Request.method == GET 
            Request.rotue == /index.html
            Request.version == HTTP/1.0
            Request["Host"] == www.example.com
            Request["User-Agent"] == Mozilla/5.0

        if an error is thrown, the request will just be returned empty and will be caught as invalid by 
        is_vaild_request function 

    */

    try { 
        std::string request_str(request_buffer);
        std::istringstream requestStream(request_str);
        std::string line;
        Request request; 

        // Parse the request line (GET /index.html HTTP/1.0)
        std::getline(requestStream, line);
        std::istringstream requestLineStream(line);
        requestLineStream >> request.method >> request.route >> request.version;

        // Parse headers (until we get an empty line)
        while (std::getline(requestStream, line) && line != "\r") {
            if (line.empty()) continue;  // Skip empty lines

            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                std::string headerName = line.substr(0, pos);
                std::string headerValue = line.substr(pos + 1);
                // Trim leading/trailing spaces
                headerValue.erase(0, headerValue.find_first_not_of(" \t"));
                headerValue.erase(headerValue.find_last_not_of(" \t") + 1);
                headerValue.erase(headerValue.find_last_of("\r"));
                request.headers[headerName] = headerValue;
            }
        }

        // Parse the body (if any)
        std::ostringstream bodyStream;
        bodyStream << requestStream.rdbuf();
        request.body = bodyStream.str();
        request.body.erase(request.body.find_last_not_of("\r\n") + 1);

        return request;
    } catch (const exception& error) { 
        Request request; 
        request.method = "invalid";
        return request; 
    }
}


std::ostream& operator<<(std::ostream& out, const Request& request) { 
    //For request logging 
    out << "Method: " << request.method << '\n';
    out << "Route: " << request.route << '\n'; 
    out << "Version: " << request.version << '\n';
    out << "Body: " << request.body << '\n';
    for (const auto& pair: request.headers) { 
        out << pair.first << ": " << pair.second << "\n"; 
    }
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

Response::Response() { 
    //Sets default response members and hetters
    version = "HTTP/1.1";
    status = 200; 
    headers["Content-Type"] = "text/html";
    headers["Connection"] = "close";
}

string build_response(Response& response) { 
    /* Returns a string of a response object complient with HTTP */
    response.headers["Content-Length"] = to_string(response.body.size());

    string message = get_status_message(response.status);
    string headers; 
    for (const auto& pair: response.headers) { 
        headers += pair.first + ": " + pair.second + "\r\n";
    }
    headers += "\r\n";
    
    string response_str =
        response.version + " " + to_string(response.status) + " " + message + "\r\n" + 
        headers + response.body;  

    return response_str; 
}

bool is_valid_request(const Request& request) { 
    /* Check each of the members to see if not empty and is valid http*/

    array<string, 7> methods{"GET", "POST", "HEAD", "PUT", "DELETE", "OPTIONS", "TRACE"};
    bool has_valid_method = false; 
    for (const string& method : methods) { 
        if (request.method == method) { 
            has_valid_method = true; 
            break; 
        }
    }
    if (!has_valid_method) { 
        return false; 
    }

    array<string, 16> forbidden_route_chars = {
        "%00",   // Null byte
        "%20",   // Space
        "%22",   // Double quote (")
        "%23",   // Hash (#)
        "%25",   // Percent (%)
        "%26",   // Ampersand (&)
        "%27",   // Single quote (')
        "%28",   // Left parenthesis (
        "%29",   // Right parenthesis )
        "%3C",   // Less than (<)
        "%3E",   // Greater than (>)
        "%5C",   // Backslash (\)
        "%5E",   // Circumflex accent (^)
        "%60",   // Grave accent (`)
        "..", 
        "~"     
    };

    for (const string& chars : forbidden_route_chars) { 
        if (request.route.find(chars) != string::npos) { 
            return false; 
        }
    }

    if (request.version != "HTTP/1.0" && request.version != "HTTP/1.1") { 
        return false; 
    }
    return true; 
}


Response default_404_response() { 
    Response response; 
    response.status = 404; 
    response.body = "<html><body><h1>404 Not Found</h1></body></html>\r\n";
    return response; 
}


Response default_400_response() { 
    Response response; 
    response.status = 400;
    response.body = "<html><body><h1>400 Bad Request</h1><p>Your request is invalid.</p></body></html>\r\n";
    return response; 
}