
#ifndef HTTP_PARSER_HPP
#define HTTP_PARSE_HPP
#include <string> 
#include "request.hpp"


namespace request_builder { 
    
    Request build_request(char* request_buffer);

}








#endif