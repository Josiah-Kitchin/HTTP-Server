

#include <iostream> 
#include <cstdlib> 
#include "server.hpp"


uoserve::ServerConfig config = {
    .port = 8080, 
    .serve_dir = string(std::getenv("HOME")) + "/Projects/server/example/pages"
};

uoserve::Server server(config);


int main() { 
    server.add_middleware([](uoserve::Request& request){ 
        std::cout << request << std::endl;
    });

    server.GET("/", [](const uoserve::Request& request, uoserve::Response& response) {
        response.body = server.get_page("index.html");
    });

    server.GET("/about", [](const uoserve::Request& request, uoserve::Response& response) { 
        response.body = server.get_page("about.html");
    });


    std::cout << "Running server on http://localhost:" << config.port << std::endl;
    server.run(); 

    return 0;
}


