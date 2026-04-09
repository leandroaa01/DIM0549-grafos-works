cmake_minimum_required(VERSION 3.10)

project(DIM0549-grafos-works)

set(CMAKE_CXX_STANDARD 17)

include_directories(include
                    include/parser
)

add_executable( grafos main.cpp  )
