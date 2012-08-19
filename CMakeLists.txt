file( GLOB interface_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file( GLOB config_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} config/*.h config/*.hpp)
file( GLOB logging_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} logging/*.h logging/*.hpp)
file( GLOB implementation_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h src/*.hpp src/*.c src/*.cpp)

source_group("Source" FILES ${implementation_files})
source_group("Logging" FILES ${logging_headers})
source_group("Config" FILES ${config_headers})
source_group("Interface" FILES ${interface_files})

add_library( wield ${implementation_files} ${interface_files} ${config_headers} ${logging_headers})
include_directories("\\lib\\x64\\vc110\\Boost\\1.50.0\\include")
include_directories("\\lib\\x64\\vc110\\yaml-cpp\\0.3.0\\include")

add_subdirectory(unit_test)
