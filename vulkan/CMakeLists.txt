cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

# include(CTest)
# enable_testing()

add_executable(main main.cpp)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

find_path(CPP_HTTPLIB_INCLUDE_DIRS "httplib.h")
# target_precompile_headers(main PRIVATE ${CPP_HTTPLIB_INCLUDE_DIRS}/httplib.h)
target_include_directories(main PRIVATE ${CPP_HTTPLIB_INCLUDE_DIRS})

find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(main PRIVATE Vulkan::Vulkan SDL2::SDL2main SDL2::SDL2-static)