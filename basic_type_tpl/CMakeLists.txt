cmake_minimum_required(VERSION 3.29)
project(basic_type_tpl VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()

add_executable(basic_type_tpl main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
