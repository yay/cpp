﻿# CMakeList.txt : CMake project for strings, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("strings")

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (strings "strings.cpp")

# TODO: Add tests and install targets if needed.
