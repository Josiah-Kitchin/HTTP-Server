
# C++ HTTP Server (Version 1.0) Library

## Overview

This is a multithreaded **C++ HTTP Server Library** implemented for learning purposes. The server is designed to support **HTTP 1.0** requests and responses. The goal of this project is to understand the fundamentals of HTTP, socket programming, and how web servers operate under the hood.

## Features

- Currently Implementing a HTTP 1.0 complient server.
- Basic handling of HTTP requests and responses.
- Supports **GET**, **POST**, and other HTTP 1.0 request methods.
- Supports custom user functions for methods and middleware 

## Prerequisites

Before using this HTTP server library, make sure you have the following installed:

- **C++ Compiler** (e.g., GCC, Clang, MSVC)
- **CMake** (for building the project)
- **C++17 or higher** (for modern C++ features)

The tests depend on GoogleTest to build and run 

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/cpp-http-server.git
   cd cpp-http-server
   ```

2. **Build static library**: 
    ```bash
    cmake -S . -B build
    ```

3. **Include the library in your project (example with cmake)**
    ```cmake
    target_link_libraries(your-project "path-to-repo/build/libuoserve.a")
    target_include_directories(your-project PRIVATE "path-to-repo/include")

    ```



