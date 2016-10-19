cmake_minimum_required(VERSION 3.3)
project(cv_calibrate)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(cv_calibrate ${SOURCE_FILES})

find_package( OpenCV REQUIRED )
target_link_libraries(cv_calibrate ${OpenCV_LIBS})