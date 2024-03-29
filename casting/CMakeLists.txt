cmake_minimum_required(VERSION 3.29)
project(casting VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()

add_executable(casting main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
