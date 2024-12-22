
#include <gtest/gtest.h> 
#include "req_res.hpp"

using namespace uoserve; 
TEST(build_request, get_request) { 
    
    char request_buffer[] = 
        "GET /index.html HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "\r\n";
    
    Request request = build_request(request_buffer);
    ASSERT_EQ(request.method, "GET");
    ASSERT_EQ(request.route, "/index.html");
    ASSERT_EQ(request.version, "HTTP/1.0");
    ASSERT_EQ(request.headers["Host"], "www.example.com");
    ASSERT_EQ(request.headers["User-Agent"], "Mozilla/5.0");
}

TEST(build_request, post_request) { 
    char request_buffer[] = 
        "POST /submit_form HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "name=JohnDoe&age=25\r\n";

    Request request = build_request(request_buffer);
    ASSERT_EQ(request.method, "POST");
    ASSERT_EQ(request.body, "name=JohnDoe&age=25");
}

TEST(is_valid_request, valid_request) { 
    char request_buffer[] = 
        "POST /submit_form HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "name=JohnDoe&age=25\r\n";

    Request request = build_request(request_buffer);
    ASSERT_TRUE(is_valid_request(request));
}


TEST(is_valid_request, invalid_method) { 
    char request_buffer[] = 
        "HITHERE /submit_form HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "name=JohnDoe&age=25\r\n";

    Request request = build_request(request_buffer);
    ASSERT_FALSE(is_valid_request(request));
}

TEST(is_valid_request, invalid_version) { 
    char request_buffer[] = 
        "POST /submit_form\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: 27\r\n"
        "\r\n"
        "name=JohnDoe&age=25\r\n";

    Request request = build_request(request_buffer);
    ASSERT_FALSE(is_valid_request(request));
}

TEST(is_valid_request, invalid_route) { 
    char request_buffer[] = 
        "GET /../index.html HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0\r\n"
        "\r\n";

    Request request = build_request(request_buffer);
    ASSERT_FALSE(is_valid_request(request));
}









