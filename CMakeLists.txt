cmake_minimum_required(VERSION 3.3)
project(glhw_01)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp util/ShaderProgram.cpp util/ShaderProgram.h)
set(glfw3_DIR "bin/glfw/usr/local/lib/cmake/glfw3")

add_executable(glhw_01 ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_include_directories(glhw_01 PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(glhw_01 ${OPENGL_gl_LIBRARY})

target_include_directories(glhw_01 PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(glhw_01 glfw)

target_include_directories(glhw_01 PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(glhw_01 ${GLEW_LIBRARIES})

add_custom_command(TARGET glhw_01 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:glhw_01>/shaders)

add_custom_command(TARGET glhw_01 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:glhw_01>/textures)
