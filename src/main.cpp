

#include <iostream> 
#include "server.hpp"


void send_html(const Request& request, Response& response) { 
    const std::string http_response =  
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>Hello from C++ Web Server!</h1></body></html>";
    response.body = http_response; 
}

int main() { 

    Server server(8080); 

    server.get("/index.html", send_html);

    std::cout << "Running server on http://localhost:8080" << std::endl;
    server.run(); 


    return 0;
}


