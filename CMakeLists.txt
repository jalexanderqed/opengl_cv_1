cmake_minimum_required(VERSION 3.6)
project(opengl_0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    nehe_lessons/lesson01_linux/lesson1.c
    nehe_lessons/lesson01_linuxGLX/lesson01/lesson01.c
    nehe_lessons/lesson01_linuxSDL/lesson01/lesson01.c
    main.cpp)

add_executable(opengl_0 ${SOURCE_FILES})