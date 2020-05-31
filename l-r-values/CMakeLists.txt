cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*,modernize-*)

project(values VERSION 1.0
                  DESCRIPTION "lvalues and rvalues"
                  LANGUAGES CXX)

add_executable(values main.cpp)