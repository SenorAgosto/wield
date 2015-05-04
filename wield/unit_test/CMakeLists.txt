file(GLOB interface_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file(GLOB implementation_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c *.cpp)
file(GLOB platform_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} platform/*.h platform/*.hpp)
file(GLOB test_application_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.h test/*.hpp test/*.c test/*.cpp)
file(GLOB test_adapter_app_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_adapter/*.h test_adapter/*.hpp test_adapter/*.c test_adapter/*.cpp)
file(GLOB test_raw_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_raw/*.h test_raw/*.hpp test_raw/*.c test_raw/*.cpp)
file(GLOB test_color_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_color/*.h test_color/*.hpp test_color/*.c test_color/*.cpp)
file(GLOB test_color_minus_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_color_minus/*.h test_color_minus/*.hpp test_color_minus/*.c test_color_minus/*.cpp)

source_group("Platform" FILES ${platform_files})
source_group("Source" FILES ${interface_files} ${implementation_files})
source_group("Test" FILES ${test_application_files})
source_group("TestColor" FILES ${test_color_files})
source_group("TestColorMinus" FILES ${test_color_minus_files})
source_group("TestWithAdapter" FILES ${test_adapter_app_files})
source_group("TestRawPointer" FILES ${test_raw_files})

include_directories("test")
include_directories("platform")

if(WIN32)
	include_directories("\\lib\\x64\\vc110\\UnitTestxx\\1.4\\include")
	include_directories("\\lib\\x64\\vc110\\VLD\\2.2.3\\include")

	link_directories("\\lib\\x64\\vc110\\Boost\\1.52.0\\lib")
	link_directories("\\lib\\x64\\vc110\\VLD\\2.2.3\\lib")
	
	set(UNITTEST++_LIBRARIES "\\lib\\x64\\vc110\\UnitTestxx\\1.4\\lib\\debug\\UnitTest++.lib")
else()
	include_directories("/Volumes/austirg/homebrew/include/")
	set(UNITTEST++_LIBRARIES "/Volumes/austirg/homebrew/lib/libUnitTest++.a")
	#find_package(UnitTest++ REQUIRED)

	#include_directories("/Volumes/austirg/homebrew/Cellar/tbb/4.3-20140724/include")
	#set(TBB_LIBRARIES "/Volumes/austirg/homebrew/Cellar/tbb/4.3-20140724/lib/libtbb.dylib" "/Volumes/austirg/homebrew/Cellar/tbb/4.3-20140724/lib/libtbbmalloc.dylib")

	#link_directories("/Volumes/austirg/homebrew/Cellar/tbb/4.2/lib")
	find_package(tbb 4.2 REQUIRED)
endif()

add_executable( wield-UT ${implementation_files} ${interface_files} ${platform_files} ${test_application_files} ${test_adapter_app_files} ${test_raw_files} ${test_color_files} ${test_color_minus_files})

target_link_libraries( wield-UT
					   wield 
					   ${UNITTEST++_LIBRARIES}
					   ${Boost_LIBRARIES}
					   ${TBB_LIBRARIES}
					 )
					
if(WIN32) 
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
					   COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\vld_x64.dll" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR} )
	add_custom_command(TARGET wield-UT
					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\dbghelp.dll" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR} )
	add_custom_command(TARGET wield-UT
					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\Microsoft.DTfW.DHL.manifest" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR} )

endif()

add_custom_command(TARGET wield-UT
				   POST_BUILD
				   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/wield-UT )

if(WIN32)
	#scan memory report file for leak message, fail the build if there is a leak.
	add_custom_command(TARGET wield-UT
					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -DMemoryLeakReportFilename=${CMAKE_CURRENT_BINARY_DIR}/memory_leak_report.txt -P ${PROJECT_SOURCE_DIR}/_cmake/FailOnMemoryLeak.cmake ) 
endif()
