cmake_minimum_required(VERSION 3.8)
project(btree VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_executable(btree binarytrees.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(${PROJECT_NAME} apr-1)
find_package(apr-1 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} apr-1)