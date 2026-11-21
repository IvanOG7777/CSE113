cmake_minimum_required(VERSION 3.28)

project(HW5)

set(CMAKE_CXX_STANDARD 23)

add_executable(part1
        template/part1/main.cpp
        template/part1/sense_reversal_barrier.cpp
        template/part1/spawn_join.cpp
        template/part1/SRBarrier.h
        template/part1/SROBarrier.h
)