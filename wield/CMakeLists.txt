# On windows, we're using VLD for memory leak detection
include(${PROJECT_SOURCE_DIR}/_cmake/post_build/VLD_setup.cmake)
message("Boost: ${Boost_LIBRARIES}")
MAKE_LIBRARY(wield
	LINK_LIBS
		${Boost_LIBRARIES}		# boost::intrusive_ptr
	UNITTEST_LIBS 
		${UNITTEST++_LIBRARIES}
		${TBB_LIBRARIES} 		# only defined on Unix
)

include(${PROJECT_SOURCE_DIR}/_cmake/post_build/VLD_detect.cmake)
