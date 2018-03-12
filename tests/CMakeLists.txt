cmake_minimum_required(VERSION 3.1)

add_executable(mytest mytest.C)

target_link_libraries(mytest qch5 hdf5_cpp-static hdf5_hl-static )
