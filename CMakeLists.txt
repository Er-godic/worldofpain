cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)

#project name
project(worldofpain)

#set paths
set(src "${PROJECT_SOURCE_DIR}/src")
set(thrd "${PROJECT_SOURCE_DIR}/thirdparty")
set(glfw "${thrd}/glfw")
set(glad "${thrd}/glad")

#add source files
file(GLOB_RECURSE source_files "${src}/*.cpp")

#executable file location
set(EXECUTABLE_OUTPUT_PATH "../bin/")

#add the executable 
add_executable(${PROJECT_NAME} 
	${source_files}
	"${glad}/src/glad.c"
	)

# location of CMakeLists for libraries
add_subdirectory(${glfw})

# precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE 
	"$<$<COMPILE_LANGUAGE:CXX>:${src}/core/stdafx.hpp>")

#include
target_include_directories(${PROJECT_NAME} PUBLIC 
	${src}
	"${thrd}/spdlog/include"
	"${glfw}/include"
        "${glad}/include"
	"${thrd}/glm"
	"${thrd}/entt/single_include"
	"${thrd}/stb"
	"${thrd}/lua"
	)

#link
target_link_directories(${PROJECT_NAME} PRIVATE "${glfw}/src" "${thrd}/lua")
target_link_libraries(${PROJECT_NAME} glfw lua)
