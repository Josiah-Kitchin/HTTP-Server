
#include <gtest/gtest.h> 
#include "req_res.hpp"


using namespace uoserve; 
TEST(build_response, response) { 
    Response response; 
    response.status = 201; 
    response.body = "<p>test</p>";

    string expected_response = "HTTP/1.1 201 Created\r\n"
                               "Connection: close\r\n"
                               "Content-Length: 11\r\n"
                               "Content-Type: text/html\r\n\r\n"
                               "<p>test</p>"; 

    ASSERT_EQ(build_response(response), expected_response);
}