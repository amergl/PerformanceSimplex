cmake_minimum_required(VERSION 3.6)
project(PerformanceSimplex)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES
    main.cxx)

add_executable(PerformanceSimplex ${SOURCE_FILES})