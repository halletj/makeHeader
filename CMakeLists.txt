cmake_minimum_required(VERSION 3.6)
project(makeHeader CXX)
add_executable(makeHeader makeHeader.cpp)
target_compile_features(makeHeader PRIVATE cxx_std_17)
