
#include "request.hpp"


std::ostream& operator<<(std::ostream& out, const Request& request) { 
    out << "Method: " << request.method << '\n';
    out << "Route: " << request.route << '\n'; 
    return out; 
}