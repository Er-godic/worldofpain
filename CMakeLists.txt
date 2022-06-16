cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)

#project name
project(worldofpain)

#set paths
set(src "${PROJECT_SOURCE_DIR}/src")

#add source files
file(GLOB_RECURSE source_files "${src}/*.cpp")

#executable file location
set(EXECUTABLE_OUTPUT_PATH "../bin/")

#add the executable 
add_executable(${PROJECT_NAME} ${source_files})

#include
target_include_directories(${PROJECT_NAME} PUBLIC ${src})
