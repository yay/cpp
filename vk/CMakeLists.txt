cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

# include(CTest)
# enable_testing()

add_executable(${PROJECT_NAME} main.cpp triangle.h triangle.cpp)

find_package(Vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

# find_package(nlohmann_json CONFIG REQUIRED)
# target_link_libraries(main PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
