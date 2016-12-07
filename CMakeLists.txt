cmake_minimum_required(VERSION 3.6)
project(PerformanceSimplex)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES
    simplex.hxx
    test_simplex.cxx)

add_executable(test_simplex ${SOURCE_FILES})

target_link_libraries(test_simplex cppunit)