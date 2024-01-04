cmake_minimum_required(VERSION 3.17)
project(tbb_test)

set(CMAKE_CXX_STANDARD 17)

find_package(TBB REQUIRED)

function(TestExample exename filename)
add_executable(${exename} ${filename})
target_link_libraries(${exename} TBB::tbb)
endfunction()

TestExample(tbb_basic tbb_basic_test.cpp)
TestExample(tbb_benchmark tbb_bench.cpp)
TestExample(tbb_arena tbb_arena.cpp)
TestExample(tbb_task task_partition.cpp)
TestExample(tbb_divide tbb_divide_conquer.cpp)
TestExample(tbb_pipeline tbb_pipeline.cpp)