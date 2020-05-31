cmake_minimum_required(VERSION 3.8)
project(sdl)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(sdl ${SOURCE_FILES})

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        /usr/local/Cellar/sdl2/2.0.8/include/SDL2
)
link_directories(${PROJECT_SOURCE_DIR}/lib)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
