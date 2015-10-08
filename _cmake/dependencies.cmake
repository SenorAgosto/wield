# include add our project to include path
include_directories(${CMAKE_SOURCE_DIR})
include(_cmake/AddLibrary/Dependencies.cmake)

# our project dependencies 
include(_cmake/dependencies/Boost.cmake)
include(_cmake/dependencies/TBB.cmake)
include(_cmake/dependencies/UnitTest++.cmake)
include(_cmake/dependencies/UsingIntrusivePtrIn.cmake)
include(_cmake/dependencies/VLD.cmake)
