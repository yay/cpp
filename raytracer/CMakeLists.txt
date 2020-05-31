cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_BUILD_TYPE Release)

project(raytracer VERSION 1.0
                  DESCRIPTION "Raytracer"
                  LANGUAGES CXX)

add_executable(raytracer main.cpp vec3.h ray.h)