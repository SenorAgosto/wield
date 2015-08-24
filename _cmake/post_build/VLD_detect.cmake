if(WIN32)
	# scan memory report file for leak message, fail the build if there is a leak.
	add_custom_command(TARGET wield-UT
					   POST_BUILD
					   COMMAND ${CMAKE_COMMAND} -DMemoryLeakReportFilename=${CMAKE_CURRENT_BINARY_DIR}/memory_leak_report.txt -P ${PROJECT_SOURCE_DIR}/_cmake/FailOnMemoryLeak.cmake ) 
endif()
