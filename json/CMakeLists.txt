cmake_minimum_required(VERSION 3.0.0)
project(json VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Release)

# This also makes the resulting executable 19% smaller (clang-900.0.39.2).
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-exceptions")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-exceptions")

include(CTest)
enable_testing()

# include_directories(/usr/local/Cellar/nlohmann_json/3.3.0/include)
include_directories(./include/)

add_executable(json main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
