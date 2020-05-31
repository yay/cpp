cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Release)

project(singleton VERSION 1.0
                  DESCRIPTION "Singleton"
                  LANGUAGES CXX)

add_executable(singleton main.cpp)