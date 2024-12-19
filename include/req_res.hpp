
#ifndef REQ_RES_HPP 
#define REQ_RES_HPP
#include <string> 
#include <iostream> 
using namespace std; 


namespace uoserve { 
    struct Request { 
        string method; 
        string route; 
        bool valid; 
    };

    struct Response { 
        string version = "HTTP/1.1";
        int status = 200; 
        string body; 
    };
}
    //For logging purposes 
    std::ostream& operator<<(std::ostream&, const uoserve::Request&); 
    uoserve::Request build_request(char* request_buffer);

    std::string build_response(const uoserve::Response&);
    std::string build_404_response(); 
    std::string build_400_response(); 



#endif