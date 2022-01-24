# Try to find the SISL librairies
#  SISL_FOUND - system has SISL lib
#  SISL_INCLUDE_DIR - the SISL include directory
#  SISL_LIBRARIES - Libraries needed to use SISL

if (SISL_INCLUDE_DIR AND SISL_LIBRARIES)
  # Already in cache, be silent
  set(SISL_FIND_QUIETLY TRUE)
endif (SISL_INCLUDE_DIR AND SISL_LIBRARIES)

find_path(SISL_INCLUDE_DIR 
	  NAMES "sisl/sisl-config.hpp" 
	  PATHS ${SEARCH_DIR}/sisl/include )

find_library(SISL_LIBRARIES 
             NAMES sisl
             PATHS ${CMAKE_CURRENT_BINARY_DIR}/lib)


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SISL DEFAULT_MSG SISL_INCLUDE_DIR SISL_LIBRARIES)

mark_as_advanced(SISL_INCLUDE_DIR SISL_LIBRARIES)
