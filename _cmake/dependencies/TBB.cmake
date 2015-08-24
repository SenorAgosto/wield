# TBB for unit testing 
if(WIN32)
	# windows uses MS's Concurrency::concurrent_queue
else()
	find_package(tbb 4.2 REQUIRED)
	include_directories(${TBB_INCLUDE_DIRS})
endif()
