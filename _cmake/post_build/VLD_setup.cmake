# On windows, we do memory leak detection with VLD. Setup post build events for this.
if(WIN32)
	add_custom_command(
		TARGET wield-UT 
		POST_BUILD
		COMMAND @echo off
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_test/configs/vld.ini ${CMAKE_CURRENT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_test/configs/ ${CMAKE_CURRENT_BINARY_DIR}/configs/ 
		COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\vld_x64.dll" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
		COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\dbghelp.dll" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
		COMMAND ${CMAKE_COMMAND} -E copy "\\lib\\x64\\vc110\\VLD\\2.2.3\\lib\\debug\\Microsoft.DTfW.DHL.manifest" ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}
	)
endif()
