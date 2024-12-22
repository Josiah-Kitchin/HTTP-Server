
#ifndef REQ_RES_HPP 
#define REQ_RES_HPP
#include <string> 
#include <iostream> 
#include <map> 
using namespace std; 


namespace uoserve { 
    struct Request { 
        string method; 
        string route; 
        string version; 
        string body;
        map<string, string> headers; 
    };

    struct Response { 
        string version;
        int status; 
        string body; 
        map<string, string> headers; 

        Response(); 
    };
}
    //For logging purposes 
    std::ostream& operator<<(std::ostream&, const uoserve::Request&); 
    uoserve::Request build_request(char*);
    bool is_valid_request(const uoserve::Request&);

    std::string build_response(uoserve::Response&);
    std::string build_404_response(); 
    std::string build_400_response(); 



#endif