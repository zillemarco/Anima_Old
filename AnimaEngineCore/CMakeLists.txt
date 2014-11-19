project(AnimaEngineCore C CXX)

include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

add_definitions(-DANIMA_ENGINE_CORE_EXPORT_ENABLED)

set(common_HEADERS	AnimaEngineCore.h
					AnimaEngine.h
					AnimaWindow_Data.h 
					AnimaWindow_Base.h 
					AnimaAssert.h 
					AnimaTypes.h 
					AnimaAllocator.h 
					AnimaStackAllocator.h
                    AnimaLinearAllocator.h
                    AnimaFreeListAllocator.h
                    AnimaPoolAllocator.h
                    AnimaProxyAllocator.h
                    AnimaVertex.hpp)

set(common_SOURCES	AnimaEngineCore.cpp 
					AnimaEngine.cpp
					AnimaAssert.cpp 
					AnimaWindow_Base.cpp 
					AnimaStackAllocator.cpp
                    AnimaLinearAllocator.cpp
                    AnimaFreeListAllocator.cpp
                    AnimaPoolAllocator.cpp
                    AnimaProxyAllocator.cpp)
		
if (_ANIMA_ENGINE_COCOA)
    set(AnimaEngineCore_HEADERS	${common_HEADERS}
                                AnimaWindow_CocoaJoystick.h
                                AnimaWindow_CocoaPlatform.h
                                AnimaWindow_CocoaTLS.h)
    set(AnimaEngineCore_SOURCES	${common_SOURCES}
                                AnimaWindow_CocoaJoystick.mm
                                AnimaWindow_CocoaPlatform.mm
                                AnimaWindow_CocoaTLS.cpp)
elseif (_ANIMA_ENGINE_WIN32)
    set(AnimaEngineCore_HEADERS	${common_HEADERS} 
								AnimaWindow_WindowsJoystick.h 
								AnimaWindow_WindowsPlatform.h 
								AnimaWindow_WindowsTLS.h)
    set(AnimaEngineCore_SOURCES	${common_SOURCES} 
								AnimaWindow_WindowsJoystick.cpp 
								AnimaWindow_WindowsPlatform.cpp 
								AnimaWindow_WindowsTLS.cpp)
elseif (_ANIMA_ENGINE_X11)
    set(AnimaEngineCore_HEADERS	${common_HEADERS})
    set(AnimaEngineCore_SOURCES	${common_SOURCES})
endif()

if (_ANIMA_ENGINE_NSGL)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} AnimaWindow_NSGLContext.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} AnimaWindow_NSGLContext.mm)
elseif (_ANIMA_ENGINE_WGL)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} AnimaWindow_WGLContext.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} AnimaWindow_WGLContext.cpp)
elseif (_ANIMA_ENGINE_X11)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} glx_context.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} glx_context.c)
endif()

if (APPLE)
    # For some reason, CMake doesn't know about .m
    set(CMAKE_CXX_FLAGS "-x objective-c++")
    set_source_files_properties(${AnimaEngineCore_SOURCES} PROPERTIES LANGUAGE CXX)
    set_source_files_properties(${AnimaEngineCore_HEADERS} PROPERTIES LANGUAGE CXX)
endif()

add_library(${PROJECT_NAME} SHARED ${AnimaEngineCore_SOURCES} ${AnimaEngineCore_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" POSITION_INDEPENDENT_CODE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Include version information in the output
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${ANIMA_ENGINE_VERSION})
if (UNIX)
	set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${ANIMA_ENGINE_VERSION_MAJOR})
endif()

if (WIN32)
	# The ANIMA_ENGINE DLL needs a special compile-time macro and import library name
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" IMPORT_PREFIX "")

	if (MINGW)
		set_target_properties(${PROJECT_NAME} PROPERTIES IMPORT_SUFFIX "dll.a")
	else()
		set_target_properties(${PROJECT_NAME} PROPERTIES IMPORT_SUFFIX "dll.lib")
	endif()
elseif (APPLE)
	# Append -fno-common to the compile flags to work around a bug in
	# Apple's GCC
	get_target_property(AnimaEngineWS_CFLAGS ${PROJECT_NAME} COMPILE_FLAGS)
	if (NOT AnimaEngineCore_CFLAGS)
		set(AnimaEngineCore_CFLAGS "")
	endif()
	set_target_properties(${PROJECT_NAME} PROPERTIES
						  COMPILE_FLAGS "${AnimaEngineCore_CFLAGS} -fno-common"
						  INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")
endif()

target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
target_link_libraries(${PROJECT_NAME} optimized ${AnimaEngine_DEPS_LIBS_RELEASE} debug ${AnimaEngine_DEPS_LIBS_DEBUG})

#if (ANIMA_ENGINE_INSTALL)
#    install(TARGETS AnimaEngine EXPORT AnimaEngineTargets DESTINATION lib${LIB_SUFFIX})
#endif()

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin
		ARCHIVE DESTINATION lib/static)