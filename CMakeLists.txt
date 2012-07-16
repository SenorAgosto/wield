file( GLOB interface_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file( GLOB implementation_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h src/*.hpp src/*.c src/*.cpp)

source_group("Source" FILES ${implementation_files})
source_group("Interface" FILES ${interface_files})

add_library( wield ${implementation_files} ${interface_files})
include_directories("\\lib\\x64\\vc110\\Boost\\1.50.0\\include")

add_subdirectory(unit_test)
