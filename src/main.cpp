

#include <iostream> 
#include "server.hpp"

int main() { 

    Server server(8080); 

    std::cout << "Running server on http://localhost:8080" << std::endl;
    server.run(); 

    return 0;
}


