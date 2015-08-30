# On windows, we're using VLD for memory leak detection
include(${PROJECT_SOURCE_DIR}/_cmake/post_build/VLD_setup.cmake)

MAKE_LIBRARY(wield
	DEPENDENCIES	
		${Boost_LIBRARIES}		# boost::intrusive_ptr
	UNITTEST_LIBS
		${TBB_LIBRARIES} 		# only defined on Unix
)

include(${PROJECT_SOURCE_DIR}/_cmake/post_build/VLD_detect.cmake)
