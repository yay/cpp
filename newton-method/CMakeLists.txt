# CMakeList.txt : CMake project for newton-method, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("newton-method")

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (newton-method "newton-method.cpp")

# TODO: Add tests and install targets if needed.
