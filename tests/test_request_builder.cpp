
#include <gtest/gtest.h> 
#include "req_res.hpp"

using namespace uoserve; 
TEST(build_request, get) { 
    
    char request_str[] = "GET /index.html HTTP/0.9";
    
    Request request = build_request(request_str);
    ASSERT_EQ(request.method, "GET");
    ASSERT_TRUE(request.valid);
}

TEST(build_request, invalid_method) { 
    char request_str[] = "MET /index.html HTTP/0.9";
    Request request = build_request(request_str);
    ASSERT_FALSE(request.valid);
}

TEST(build_request, route) { 
    char request_str[] = "GET /index.html HTTP/0.9";
    Request request = build_request(request_str);
    ASSERT_EQ("/index.html", request.route);
}

TEST(build_request, invalid_route) { 
    char request_str[] = "GET ../index.html HTTP/0.9";
    Request request = build_request(request_str);
    ASSERT_FALSE(request.valid);
}

TEST(build_request, no_request) { 
    char request_str[] = "";
    Request request = build_request(request_str); 
    ASSERT_FALSE(request.valid);
}

TEST(build_request, no_route) {
    char request_str[] = "GET /HTTP";
    Request request = build_request(request_str);
    ASSERT_FALSE(request.valid);
}






