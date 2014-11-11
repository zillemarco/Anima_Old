project(AnimaEngineWS C CXX)

#include_directories("${ANIMA_ENGINE_SOURCE_DIR}/AnimaEngineWS"
#                    "${ANIMA_ENGINE_BINARY_DIR}/AnimaEngineWS"
#                    ${AnimaEngine_INCLUDE_DIRS})
include_directories("${PROJECT_SOURCE_DIR}"
                    ${AnimaEngine_INCLUDE_DIRS})

#add_definitions(-D_ANIMA_ENGINE_USE_CONFIG_H)
add_definitions(-DANIMA_ENGINE_WS_EXPORT_ENABLED)

set(common_HEADERS AnimaEngineWS.h AnimaWindow.h AnimaWindow_Data.h AnimaWindow_Base.h)
set(common_SOURCES AnimaEngineWS.cpp AnimaWindow.cpp)
#set(common_HEADERS "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngine_config.h" internal.h
#                   "${ANIMA_ENGINE_SOURCE_DIR}/include/ANIMA_ENGINE/AnimaEngine3.h"
#                   "${ANIMA_ENGINE_SOURCE_DIR}/include/ANIMA_ENGINE/AnimaEngine3native.h")
#set(common_SOURCES context.c init.c input.c monitor.c window.c)

if (_ANIMA_ENGINE_COCOA)
#    set(AnimaEngine_HEADERS ${common_HEADERS} cocoa_platform.h iokit_joystick.h posix_tls.h)
#    set(AnimaEngine_SOURCES ${common_SOURCES} cocoa_init.m cocoa_monitor.m cocoa_window.m iokit_joystick.m mach_time.c posix_tls.c)
    set(AnimaEngineWS_HEADERS ${common_HEADERS} AnimaWindow_CocoaPlatform.h AnimaWindow_CocoaWindow.h)
    set(AnimaEngineWS_SOURCES ${common_SOURCES} AnimaWindow_CocoaWindow.mm)
elseif (_ANIMA_ENGINE_WIN32)
    set(AnimaEngineWS_HEADERS ${common_HEADERS} win32_platform.h win32_tls.h winmm_joystick.h)
    set(AnimaEngineWS_SOURCES ${common_SOURCES} win32_init.c win32_monitor.c win32_time.c win32_tls.c win32_window.c winmm_joystick.c)
elseif (_ANIMA_ENGINE_X11)
    set(AnimaEngineWS_HEADERS ${common_HEADERS} x11_platform.h xkb_unicode.h linux_joystick.h posix_time.h posix_tls.h)
    set(AnimaEngineWS_SOURCES ${common_SOURCES} x11_init.c x11_monitor.c x11_window.c xkb_unicode.c linux_joystick.c posix_time.c posix_tls.c)
endif()

#if (_ANIMA_ENGINE_NSGL)
#    list(APPEND AnimaEngineWS_HEADERS ${common_HEADERS} nsgl_context.h)
#    list(APPEND AnimaEngineWS_SOURCES ${common_SOURCES} nsgl_context.m)
#elseif (_ANIMA_ENGINE_WGL)
#    list(APPEND AnimaEngineWS_HEADERS ${common_HEADERS} wgl_context.h)
#    list(APPEND AnimaEngineWS_SOURCES ${common_SOURCES} wgl_context.c)
#elseif (_ANIMA_ENGINE_X11)
#    list(APPEND AnimaEngineWS_HEADERS ${common_HEADERS} glx_context.h)
#    list(APPEND AnimaEngineWS_SOURCES ${common_SOURCES} glx_context.c)
#endif()

if (APPLE)
    # For some reason, CMake doesn't know about .m
    set(CMAKE_CXX_FLAGS "-x objective-c++")
    set_source_files_properties(${AnimaEngineWS_SOURCES} PROPERTIES LANGUAGE CXX)
    set_source_files_properties(${AnimaEngineWS_HEADERS} PROPERTIES LANGUAGE CXX)
endif()

add_library(${PROJECT_NAME} ${AnimaEngineWS_SOURCES} ${AnimaEngineWS_HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}" POSITION_INDEPENDENT_CODE ON)

if (BUILD_SHARED_LIBS)
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
        if (NOT AnimaEngineWS_CFLAGS)
            set(AnimaEngineWS_CFLAGS "")
        endif()
        set_target_properties(${PROJECT_NAME} PROPERTIES
                              COMPILE_FLAGS "${AnimaEngineWS_CFLAGS} -fno-common"
                              INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")
    endif()

    target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
endif()

#if (ANIMA_ENGINE_INSTALL)
#    install(TARGETS AnimaEngine EXPORT AnimaEngineTargets DESTINATION lib${LIB_SUFFIX})
#endif()

