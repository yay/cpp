cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

add_executable(main main.cpp)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX)
endif()