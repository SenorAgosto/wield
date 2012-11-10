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
				   COMMAND @echo off)

#setup VLD.ini file
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/configs/vld.ini ${CMAKE_CURRENT_BINARY_DIR} )

#setup unit test config files
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configs/ ${CMAKE_CURRENT_BINARY_DIR}/configs/ )

#setup VLD DLL in prebuild step.
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\vld_x64.dll" ${CMAKE_CURRENT_BINARY_DIR}/$(ConfigurationName) )
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\dbghelp.dll" ${CMAKE_CURRENT_BINARY_DIR}/$(ConfigurationName) )
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\Microsoft.DTfW.DHL.manifest" ${CMAKE_CURRENT_BINARY_DIR}/$(ConfigurationName) )
				   
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/$(ConfigurationName)/wield-UT.exe )

#scan memory report file for leak message, fail the build if there is a leak.
add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -DMemoryLeakReportFilename=${CMAKE_CURRENT_BINARY_DIR}/memory_leak_report.txt -P ${PROJECT_SOURCE_DIR}/_cmake/FailOnMemoryLeak.cmake ) 
