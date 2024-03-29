cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -g")

project(binary_trees VERSION 0.1.0)

add_executable(binary_trees main.cpp)