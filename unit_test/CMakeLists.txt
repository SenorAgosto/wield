file(GLOB interface_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB implementation_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)

add_executable( wield-UT ${implementation_files} ${interface_files})
include_directories("\\lib\\x64\\vc110\\UnitTestxx\\1.4\\include")
target_link_libraries( wield-UT
					   wield 
					   "\\lib\\x64\\vc110\\UnitTestxx\\1.4\\lib\\debug\\UnitTest++.lib"
					 )
