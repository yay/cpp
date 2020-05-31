cmake_minimum_required(VERSION 3.0.0)
project(json VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

include(CTest)
enable_testing()

add_library(simdjson simdjson.cpp simdjson.h)
add_executable(json main.cpp)

target_link_libraries(json PRIVATE simdjson)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
