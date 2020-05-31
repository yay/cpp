cmake_minimum_required(VERSION 3.12)
project(hash VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

include(CTest)
enable_testing()

set(SOURCES
    hashmap.c
    main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)