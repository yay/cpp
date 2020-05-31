# CMakeList.txt : CMake project for maze-backtracking, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("maze-backtracking")

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (maze-backtracking "maze-backtracking.cpp")

# TODO: Add tests and install targets if needed.
