cmake_minimum_required (VERSION 2.8)

project (opencvtest0)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(opencvtest0 hello.cpp)

target_link_libraries(opencvtest0 ${OpenCV_LIBS})

