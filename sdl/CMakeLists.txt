cmake_minimum_required(VERSION 3.8)
project(sdl)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(sdl ${SOURCE_FILES})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(sdl PRIVATE SDL2::SDL2main SDL2::SDL2-static)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(sdl PRIVATE SDL2::SDL2_image)