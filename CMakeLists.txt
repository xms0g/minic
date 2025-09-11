cmake_minimum_required(VERSION 4.0)
project(mt C)

set(CMAKE_C_STANDARD 11)

add_library(${PROJECT_NAME} SHARED src/mt.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(test)
