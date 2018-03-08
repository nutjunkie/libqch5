cmake_minimum_required(VERSION 3.3)
project(qch5)

#add_subdirectory(hdf5-1.10.1)
include_directories(hdf5-1.10.1/c++/src hdf5-1.10.1/src build/hdf5-1.10.1)
include_directories(st_tree/include)


#include_directories(quaternions/include)
include_directories(src)

link_directories(build/hdf5-1.10.1/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build/bin)
SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_subdirectory(tests)
add_subdirectory(src)
