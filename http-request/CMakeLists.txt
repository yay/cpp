cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project(http-request)

# vcpkg install openssl:x64-windows
#find_package(OpenSSL REQUIRED)

add_executable(http-request "httplib.h" "http-request.cpp")

# OpenSSL::SSL OpenSSL::Crypto
target_link_libraries(http-request PRIVATE)