cmake_minimum_required(VERSION 2.8)
Project("wield")

include(_cmake/CompilerFlags.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/_cmake")

include_directories(${CMAKE_SOURCE_DIR}/../UsingIntrusivePtrIn)
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(wield)
add_subdirectory(_acceptance_tests)
