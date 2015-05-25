# Try to find arbiter
# Once done this will define
# arbiter_FOUND - system has the lib 
# arbiter_INCLUDE_DIRS - the path to include directory 
# arbiter_LIBRARIES - the libraries needed to use
# arbiter_DEFINITIONS - compiler switches required for using arbiter

find_package(PkgConfig)
pkg_check_modules(PC_arbiter QUIET libarbiter)
set(arbiter_DEFINITIONS ${PC_arbiter_CFLAGS_OTHER})

# see if there is a brew path (useful when it is not /usr/local/)
set(PC_HOMEBREW_PATH $ENV{HOMEBREW})
if(PC_HOMEBREW_PATH)
    set(PC_HOMEBREW_INCLUDE_DIRS "$ENV{HOMEBREW}/include")
    set(PC_HOMEBREW_LIBRARY_DIRS "$ENV{HOMEBREW}/lib")
endif()

find_path(arbiter_INCLUDE_DIR arbiter/SequenceArbiter.hpp
    HINTS ${PC_arbiter_INCLUDEDIR} ${PC_arbiter_INCLUDE_DIRS} ${PC_HOMEBREW_INCLUDE_DIRS}
    PATH_SUFFIXES arbiter )

find_library(arbiter_LIBRARY NAMES arbiter libarbiter
    HINTS ${PC_arbiter_LIBDIR} ${PC_arbiter_LIBRARY_DIRS} ${PC_HOMEBREW_LIBRARY_DIRS} )

set(arbiter_LIBRARIES ${arbiter_LIBRARY} )
set(arbiter_INCLUDE_DIRS ${arbiter_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs) 
find_package_handle_standard_args(arbiter DEFAULT_MSG
    arbiter_LIBRARY arbiter_INCLUDE_DIR)

mark_as_advanced(arbiter_INCLUDE_DIR arbiter_LIBRARY)
 
