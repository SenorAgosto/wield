file(GLOB interface_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB implementation_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)

include_directories("\\lib\\x64\\vc110\\UnitTestxx\\1.4\\include")
include_directories("\\lib\\x64\\vc110\\VLD\\2.2.3\\include")

link_directories("\\lib\\x64\\vc110\\Boost\\1.50.0\\lib")
link_directories("\\lib\\x64\\vc110\\VLD\\2.2.3\\lib")

add_executable( wield-UT ${implementation_files} ${interface_files})

target_link_libraries( wield-UT
					   wield 
					   "\\lib\\x64\\vc110\\UnitTestxx\\1.4\\lib\\debug\\UnitTest++.lib"
					 )

add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/$(ConfigurationName)/wield-UT.exe )