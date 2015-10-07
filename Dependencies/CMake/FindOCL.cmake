# - Try to find OpenCL
# Once done this will define
#  
#  OPENCL_FOUND        - system has OpenCL
#  OPENCL_INCLUDE_DIR  - the OpenCL include directory
#  OPENCL_LIBRARIES    - link these to use OpenCL
#
# WIN32 should work, but is untested

IF (WIN32)
    set(OCL_LIBRARIES ${PROJECT_SOURCE_DIR}/Dependencies/OpenCL/Lib/OpenCL.lib)
ELSE (WIN32)
    message("lib path: $ENV{LD_LIBRARY_PATH}")
    FIND_LIBRARY(OCL_LIBRARIES OpenCL ENV LD_LIBRARY_PATH)
    message("opencl_libraries: ${OCL_LIBRARIES}")
ENDIF (WIN32)

list(APPEND OCL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Dependencies/OpenCL/Include")

SET(OCL_FOUND "NO")
IF(OCL_LIBRARIES )
    SET(OCL_FOUND "YES" )
ENDIF(OCL_LIBRARIES)

MARK_AS_ADVANCED(
  OCL_INCLUDE_DIR
)