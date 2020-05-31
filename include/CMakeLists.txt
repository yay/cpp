cmake_minimum_required(VERSION 3.0.0)
project(include VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_executable(include main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# set_target_properties(include PROPERTIES COMPILE_FLAGS "-S")
# Uncomment the above for CMakeFiles/include.dir/main.cpp.o to be assembly output
# instead of binary
