cmake_minimum_required(VERSION 3.15)
project(task_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(task_1 main.cpp IGraph.h MatrixGraph.h SetGraph.h ArcGraph.h ListGraph.h ListGraph.cpp MatrixGraph.cpp SetGraph.cpp ArcGraph.cpp )