cmake_minimum_required(VERSION 3.5)
project(TSP)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



add_executable(TSP main.cpp Ant.cpp City.cpp)