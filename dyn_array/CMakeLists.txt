cmake_minimum_required(VERSION 3.0.0)
project(dyn_array VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
include(CTest)
enable_testing()

add_executable(dyn_array main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(MSVC)
    target_compile_options(dyn_array PRIVATE /W4 /WX)
else()
    target_compile_options(dyn_array PRIVATE -Wall -Wextra -pedantic -Werror)
endif()