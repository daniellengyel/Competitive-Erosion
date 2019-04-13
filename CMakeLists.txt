cmake_minimum_required(VERSION 3.13)
project(Competitive_Erosion)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Competitive_Erosion
        CMakeLists.txt
        Graph.cpp
        Graph.h)
