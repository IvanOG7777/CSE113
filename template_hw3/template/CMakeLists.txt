cmake_minimum_required(VERSION 3.28)
project(Homework3)

set(CMAKE_CXX_STANDARD 23)

add_executable(part1
        part1/main.cpp
        part1/checker.h
        part1/CQueue.h
        part1/CQueueSync.h
        part1/part3_thread_funcs.h
)

add_executable(part2
        part2/main.cpp
        part2/IOQueue.h
        part2/part1_static.h
        part2/part2_global.h
        part2/part3_local.h
        part2/part4_local32.h
        part2/util.h
)