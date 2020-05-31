# CMakeList.txt : CMake project for optional, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project ("optional")

# Add source to this project's executable.
add_executable (optional "optional.cpp")

# TODO: Add tests and install targets if needed.
