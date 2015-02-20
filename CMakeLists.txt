cmake_minimum_required(VERSION 2.8)
Project("wield")
include(_cmake/CompilerFlags.cmake)

include_directories(${CMAKE_SOURCE_DIR}/../UsingIntrusivePtrIn)
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(wield)
