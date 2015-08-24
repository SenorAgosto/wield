find_package(arbiter REQUIRED)
include_directories(${arbiter_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(queue_stress)
