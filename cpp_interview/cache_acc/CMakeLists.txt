cmake_minimum_required(VERSION 3.17)
# set(CMAKE_C_COMPILER "/usr/bin/clang-15")
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++-15")
project(cache_acc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)


find_package(TBB REQUIRED)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found")
else()
    message(STATUS "OpenMP not found")
endif()
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Turn off test")
add_subdirectory(benchmark)

function(TestExample exename filename)
add_executable(${exename} ${filename})
target_include_directories(${exename} PUBLIC benchmark)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${exename} PUBLIC  OpenMP::OpenMP_CXX)
endif()
target_link_libraries(${exename} PUBLIC benchmark::benchmark TBB::tbb)
endfunction()



TestExample(basic basic_benchmark_test.cpp)
TestExample(aosoa aosoa.cpp)
TestExample(fetch fetch.cpp)
TestExample(loop loop_merging.cpp)