file( GLOB interface_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h *.hpp)
file( GLOB implementation_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h src/*.hpp src/*.c src/*.cpp)
file( GLOB details_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} details/*.h details/*.hpp)
file( GLOB message_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} message/*.h message/*.hpp)
file( GLOB stage_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} stage/*.h stage/*.hpp)
file( GLOB platform_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} platform/*.h platform/*.hpp)

source_group("Source" FILES ${implementation_files})
source_group("Interface" FILES ${interface_files})
source_group("Interface\\Details" FILES ${details_files})
source_group("Interface\\Message" FILES ${message_headers})
source_group("Interface\\Platform" FILES ${platform_headers})
source_group("Interface\\Stage" FILES ${stage_headers})

if(WIN32)
	include_directories("\\lib\\x64\\vc110\\Boost\\1.52.0\\include")
else()
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME OFF)

	find_package(Boost COMPONENTS iostreams filesystem system REQUIRED) 
	
	if(Boost_FOUND)
		include_directories(${Boost_INCLUDE_DIR})
		#link_directories(${Boost_LIBRARY_DIR})
		add_definitions("-DHAS_BOOST")
	endif() 	
endif()

# include Intel's Thread Building Blocks
find_package(TBB REQUIRED)
if(TBB_FOUND)
    include_directories(${TBB_INCLUDE_DIR})
    link_directories(${TBB_LIBRARY_DIRS})
endif()

find_package(arbiter REQUIRED)

add_executable(queue_stress  ${implementation_files} ${interface_files} ${platform_headers} ${stage_headers} ${message_headers} ${details_files})

target_link_libraries(queue_stress
    ${Boost_LIBRARIES}
    ${TBB_LIBRARIES}
    ${arbiter_LIBRARIES}
    wield
)

