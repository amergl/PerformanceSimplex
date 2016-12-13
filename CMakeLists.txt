cmake_minimum_required(VERSION 3.6)
project(PerformanceSimplex)

add_subdirectory(benchmark)

include_directories(benchmark/include)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -mtune=native -march=native -flto -fopt-info-vec")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb")

set(COMMON_SOURCE_FILES
        simplex.hxx)

set(TEST_SOURCE_FILES
    test_simplex.cxx)


add_executable(test_simplex ${COMMON_SOURCE_FILES} ${TEST_SOURCE_FILES})
add_executable(benchmark_simplex benchmark_simplex.cpp ${COMMON_SOURCE_FILES})

target_link_libraries(benchmark_simplex benchmark)

enable_testing()
add_test(simplex test_simplex)
add_test(simplex_performance benchmark_simplex)

target_link_libraries(test_simplex cppunit)
