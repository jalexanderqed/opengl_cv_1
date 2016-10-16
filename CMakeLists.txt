cmake_minimum_required(VERSION 3.3)
project(glhw_01)
include(ExternalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp util/ShaderProgram.cpp util/ShaderProgram.h shapes/Box.cpp shapes/Box.h util/DrawState.cpp util/DrawState.h util/const.h)

add_executable(glhw_01 ${SOURCE_FILES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#set(BUILD_SHARED_LIBS ON)
add_subdirectory(lib/glfw-3.2.1)
target_link_libraries(glhw_01 glfw)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/assimp-3.3.1)
target_link_libraries(glhw_01 assimp)

find_package(OpenGL REQUIRED)
target_link_libraries(glhw_01 ${OPENGL_gl_LIBRARY})

find_package(GLEW REQUIRED)
target_link_libraries(glhw_01 ${GLEW_LIBRARIES})

add_custom_command(TARGET glhw_01 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:glhw_01>/shaders)

add_custom_command(TARGET glhw_01 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:glhw_01>/textures)
