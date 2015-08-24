cmake_minimum_required(VERSION 2.8)
Project("wield")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/_cmake")
include(_cmake/Dependencies.cmake)

add_subdirectory(wield)
