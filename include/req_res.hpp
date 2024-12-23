
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

        Response(); //The response constructor sets a default response 
    };
}
    //For logging purposes 
    std::ostream& operator<<(std::ostream&, const uoserve::Request&); 

    //Takes a raw http request and turns it into a request object 
    uoserve::Request build_request(char*);
    //Must be called before using any request, will validate the members are not empty and propper http
    bool is_valid_request(const uoserve::Request&);
    //Takes a response object and turns it into raw http 
    std::string build_response(uoserve::Response&);
    //Default status code responses 

    uoserve::Response default_404_response(); 
    uoserve::Response default_400_response();



#endif