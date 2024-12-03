cmake_minimum_required(VERSION 3.16)
project(makeHeader CXX)
add_executable(makeHeader makeHeader.cpp)
target_compile_features(makeHeader PRIVATE cxx_std_17)
