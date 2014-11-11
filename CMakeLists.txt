cmake_minimum_required(VERSION 3.0.2)
project(Anima C CXX)

set(ANIMA_ENGINE_VERSION_MAJOR "0")
set(ANIMA_ENGINE_VERSION_MINOR "0")
set(ANIMA_ENGINE_VERSION_PATCH "0")

set(ANIMA_ENGINE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/bin)

set(LIB_SUFFIX "" CACHE STRING "Takes an empty string or 64. Directory where lib will be installed: lib or lib64")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ANIMA_ENGINE_BUILD_DOCS "Build the ANIMA_ENGINE documentation" ON)
option(ANIMA_ENGINE_INSTALL "Generate installation target" ON)

if (WIN32)
    option(ANIMA_ENGINE_USE_DWM_SWAP_INTERVAL "Set swap interval even when DWM compositing is enabled" OFF)
    option(ANIMA_ENGINE_USE_OPTIMUS_HPG "Force use of high-performance GPU on Optimus systems" OFF)
endif()

if (APPLE)
    option(ANIMA_ENGINE_BUILD_UNIVERSAL "Build ANIMA_ENGINE as a Universal Binary" OFF)
    option(ANIMA_ENGINE_USE_CHDIR "Make AnimaEngineInit chdir to Contents/Resources" ON)
    option(ANIMA_ENGINE_USE_MENUBAR "Populate the menu bar on first window creation" ON)
    option(ANIMA_ENGINE_USE_RETINA "Use the full resolution of Retina displays" ON)
else()
    option(ANIMA_ENGINE_USE_EGL "Use EGL for context creation" OFF)
endif()

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON)
endif()

if (BUILD_SHARED_LIBS)
    set(_ANIMA_ENGINE_BUILD_DLL 1)
endif()

set(_ANIMA_ENGINE_USE_OPENGL 1)

if (_ANIMA_ENGINE_USE_OPENGL)
    find_package(OpenGL REQUIRED)
else()
    message(FATAL_ERROR "Graphic drivers not specified")
endif()

find_package(Threads REQUIRED)

if (ANIMA_ENGINE_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
if (UNIX)
    add_definitions(-Wall)

    if (BUILD_SHARED_LIBS)
        add_definitions(-fvisibility=hidden)
    endif()
endif()

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach (flag CMAKE_C_FLAGS
                        CMAKE_C_FLAGS_DEBUG
                        CMAKE_C_FLAGS_RELEASE
                        CMAKE_C_FLAGS_MINSIZEREL
                        CMAKE_C_FLAGS_RELWITHDEBINFO)

            if (${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
            if (${flag} MATCHES "/MDd")
                string(REGEX REPLACE "/MDd" "/MTd" ${flag} "${${flag}}")
            endif()

        endforeach()
    endif()
endif()

if (MINGW)
    # Enable link-time exploit mitigation features enabled by default on MSVC

    include(CheckCCompilerFlag)

    # Compatibility with data execution prevention (DEP)
    set(CMAKE_REQUIRED_FLAGS "-Wl,--nxcompat")
    check_c_compiler_flag("" _ANIMA_ENGINE_HAS_DEP)
    if (_ANIMA_ENGINE_HAS_DEP)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--nxcompat ${CMAKE_SHARED_LINKER_FLAGS}")
    endif()

    # Compatibility with address space layout randomization (ASLR)
    set(CMAKE_REQUIRED_FLAGS "-Wl,--dynamicbase")
    check_c_compiler_flag("" _ANIMA_ENGINE_HAS_ASLR)
    if (_ANIMA_ENGINE_HAS_ASLR)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--dynamicbase ${CMAKE_SHARED_LINKER_FLAGS}")
    endif()

    # Compatibility with 64-bit address space layout randomization (ASLR)
    set(CMAKE_REQUIRED_FLAGS "-Wl,--high-entropy-va")
    check_c_compiler_flag("" _ANIMA_ENGINE_HAS_64ASLR)
    if (_ANIMA_ENGINE_HAS_64ASLR)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--high-entropy-va ${CMAKE_SHARED_LINKER_FLAGS}")
    endif()
endif()

#--------------------------------------------------------------------
# Detect and select backend APIs
#--------------------------------------------------------------------
if (WIN32)
    set(_ANIMA_ENGINE_WIN32 1)
    message(STATUS "Using Win32 for window creation")
    set(_ANIMA_ENGINE_WGL 1)
    message(STATUS "Using WGL for context creation")
elseif (APPLE)
    set(_ANIMA_ENGINE_COCOA 1)
    message(STATUS "Using Cocoa for window creation")
    set(_ANIMA_ENGINE_NSGL 1)
    message(STATUS "Using NSGL for context creation")
elseif (UNIX)
    set(_ANIMA_ENGINE_X11 1)
    message(STATUS "Using X11 for window creation")
    set(_ANIMA_ENGINE_GLX 1)
    message(STATUS "Using GLX for context creation")
else()
    message(FATAL_ERROR "No supported platform was detected")
endif()

#--------------------------------------------------------------------
# Use Win32 for window creation
#--------------------------------------------------------------------
if (_ANIMA_ENGINE_WIN32)

    set(ANIMA_ENGINE_PKG_LIBS "${ANIMA_ENGINE_PKG_LIBS} -lgdi32")

    if (ANIMA_ENGINE_USE_DWM_SWAP_INTERVAL)
        set(_ANIMA_ENGINE_USE_DWM_SWAP_INTERVAL 1)
    endif()
    if (ANIMA_ENGINE_USE_OPTIMUS_HPG)
        set(_ANIMA_ENGINE_USE_OPTIMUS_HPG 1)
    endif()

    # HACK: When building on MinGW, WINVER and UNICODE need to be defined before
    # the inclusion of stddef.h (by AnimaEngine3.h), which is itself included before
    # win32_platform.h.  We define them here until a saner solution can be found
    # NOTE: MinGW-w64 and Visual C++ do /not/ need this hack.
    if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
        add_definitions(-DUNICODE -DWINVER=0x0501)
    endif()
endif()

#--------------------------------------------------------------------
# Use WGL for context creation
#--------------------------------------------------------------------
if (_ANIMA_ENGINE_WGL)
    set(ANIMA_ENGINE_PKG_LIBS "${ANIMA_ENGINE_PKG_LIBS} -lopengl32")
    list(APPEND AnimaEngine_INCLUDE_DIRS "${OPENGL_INCLUDE_DIR}")
    list(APPEND AnimaEngine_LIBRARIES "${OPENGL_gl_LIBRARY}")
endif()

#--------------------------------------------------------------------
# Use X11 for window creation
#--------------------------------------------------------------------
if (_ANIMA_ENGINE_X11)

    find_package(X11 REQUIRED)

    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} x11")

    # Set up library and include paths
    list(APPEND AnimaEngine_INCLUDE_DIRS "${X11_X11_INCLUDE_PATH}")
    list(APPEND AnimaEngine_LIBRARIES "${X11_X11_LIB}" "${CMAKE_THREAD_LIBS_INIT}")
    if (UNIX AND NOT APPLE)
        list(APPEND AnimaEngine_LIBRARIES "${RT_LIBRARY}")
    endif()

    # Check for XRandR (modern resolution switching and gamma control)
    if (NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "The RandR library and headers were not found")
    endif()

    list(APPEND AnimaEngine_INCLUDE_DIRS "${X11_Xrandr_INCLUDE_PATH}")
    list(APPEND AnimaEngine_LIBRARIES "${X11_Xrandr_LIB}")
    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} xrandr")

    # Check for Xinerama (legacy multi-monitor support)
    if (NOT X11_Xinerama_FOUND)
        message(FATAL_ERROR "The Xinerama library and headers were not found")
    endif()

    list(APPEND AnimaEngine_INCLUDE_DIRS "${X11_Xinerama_INCLUDE_PATH}")
    list(APPEND AnimaEngine_LIBRARIES "${X11_Xinerama_LIB}")
    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} xinerama")

    # Check for XInput (high-resolution cursor motion)
    if (NOT X11_Xinput_FOUND)
        message(FATAL_ERROR "The XInput library and headers were not found")
    endif()

    list(APPEND AnimaEngine_INCLUDE_DIRS "${X11_Xinput_INCLUDE_PATH}")

    if (X11_Xinput_LIB)
        list(APPEND AnimaEngine_LIBRARIES "${X11_Xinput_LIB}")
    else()
        # Backwards compatibility (bug in CMake 2.8.7)
        list(APPEND AnimaEngine_LIBRARIES Xi)
    endif()
    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} xi")

    # Check for Xf86VidMode (fallback gamma control)
    if (NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "The Xf86VidMode library and headers were not found")
    endif()

    list(APPEND AnimaEngine_INCLUDE_DIRS "${X11_xf86vmode_INCLUDE_PATH}")
    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} xxf86vm")

    if (X11_Xxf86vm_LIB)
        list(APPEND AnimaEngine_LIBRARIES "${X11_Xxf86vm_LIB}")
    else()
        # Backwards compatibility (see CMake bug 0006976)
        list(APPEND AnimaEngine_LIBRARIES Xxf86vm)
    endif()

    # Check for Xkb (X keyboard extension)
    if (NOT X11_Xkb_FOUND)
        message(FATAL_ERROR "The X keyboard extension headers were not found")
    endif()

    list(APPEND AnimaEngine_INCLUDE_DIR "${X11_Xkb_INCLUDE_PATH}")

    find_library(RT_LIBRARY rt)
    mark_as_advanced(RT_LIBRARY)
    if (RT_LIBRARY)
        list(APPEND AnimaEngine_LIBRARIES "${RT_LIBRARY}")
        set(ANIMA_ENGINE_PKG_LIBS "${ANIMA_ENGINE_PKG_LIBS} -lrt")
    endif()

    find_library(MATH_LIBRARY m)
    mark_as_advanced(MATH_LIBRARY)
    if (MATH_LIBRARY)
        list(APPEND AnimaEngine_LIBRARIES "${MATH_LIBRARY}")
        set(ANIMA_ENGINE_PKG_LIBS "${ANIMA_ENGINE_PKG_LIBS} -lm")
    endif()

    # Check for Xcursor
    if (NOT X11_Xcursor_FOUND)
        message(FATAL_ERROR "The Xcursor libraries and headers were not found")
    endif()

    list(APPEND AnimaEngine_INCLUDE_DIR "${X11_Xcursor_INCLUDE_PATH}")
    list(APPEND AnimaEngine_LIBRARIES "${X11_Xcursor_LIB}")
    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} xcursor")

endif()

#--------------------------------------------------------------------
# Use GLX for context creation
#--------------------------------------------------------------------
if (_ANIMA_ENGINE_GLX)

    list(APPEND AnimaEngine_INCLUDE_DIRS "${OPENGL_INCLUDE_DIR}")
    list(APPEND AnimaEngine_LIBRARIES "${OPENGL_gl_LIBRARY}")

    set(ANIMA_ENGINE_PKG_DEPS "${ANIMA_ENGINE_PKG_DEPS} gl")

    include(CheckFunctionExists)

    set(CMAKE_REQUIRED_LIBRARIES "${OPENGL_gl_LIBRARY}")
    check_function_exists(glXGetProcAddress _ANIMA_ENGINE_HAS_GLXGETPROCADDRESS)
    check_function_exists(glXGetProcAddressARB _ANIMA_ENGINE_HAS_GLXGETPROCADDRESSARB)
    check_function_exists(glXGetProcAddressEXT _ANIMA_ENGINE_HAS_GLXGETPROCADDRESSEXT)

    if (NOT _ANIMA_ENGINE_HAS_GLXGETPROCADDRESS AND
        NOT _ANIMA_ENGINE_HAS_GLXGETPROCADDRESSARB AND
        NOT _ANIMA_ENGINE_HAS_GLXGETPROCADDRESSEXT)
        message(WARNING "No glXGetProcAddressXXX variant found")

        # Check for dlopen support as a fallback
        set(CMAKE_REQUIRED_LIBRARIES "${CMAKE_DL_LIBS}")
        check_function_exists(dlopen _ANIMA_ENGINE_HAS_DLOPEN)
        if (NOT _ANIMA_ENGINE_HAS_DLOPEN)
            message(FATAL_ERROR "No entry point retrieval mechanism found")
        endif()

        if (CMAKE_DL_LIBS)
            list(APPEND AnimaEngine_LIBRARIES "${CMAKE_DL_LIBS}")
            set(ANIMA_ENGINE_PKG_LIBS "${ANIMA_ENGINE_PKG_LIBS} -l${CMAKE_DL_LIBS}")
        endif()
    endif()

endif()

#--------------------------------------------------------------------
# Use Cocoa for window creation and NSOpenGL for context creation
#--------------------------------------------------------------------
if (_ANIMA_ENGINE_COCOA AND _ANIMA_ENGINE_NSGL)

    if (ANIMA_ENGINE_USE_MENUBAR)
        set(_ANIMA_ENGINE_USE_MENUBAR 1)
    endif()

    if (ANIMA_ENGINE_USE_CHDIR)
        set(_ANIMA_ENGINE_USE_CHDIR 1)
    endif()

    if (ANIMA_ENGINE_USE_RETINA)
        set(_ANIMA_ENGINE_USE_RETINA 1)
    endif()

    if (ANIMA_ENGINE_BUILD_UNIVERSAL)
        message(STATUS "Building ANIMA_ENGINE as Universal Binaries")
        set(CMAKE_OSX_ARCHITECTURES i386;x86_64)
    else()
        message(STATUS "Building ANIMA_ENGINE only for the native architecture")
    endif()

    # Set up library and include paths
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
    list(APPEND AnimaEngine_LIBRARIES "${COCOA_FRAMEWORK}"
                               "${OPENGL_gl_LIBRARY}"
                               "${IOKIT_FRAMEWORK}"
                               "${CORE_FOUNDATION_FRAMEWORK}"
                               "${CORE_VIDEO_FRAMEWORK}")

    set(ANIMA_ENGINE_PKG_DEPS "")
    set(ANIMA_ENGINE_PKG_LIBS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreFoundation -framework CoreVideo")
endif()

#--------------------------------------------------------------------
# Export ANIMA_ENGINE library dependencies
#--------------------------------------------------------------------
set(ANIMA_ENGINE_LIBRARIES ${AnimaEngine_LIBRARIES} CACHE STRING "Dependencies of ANIMA_ENGINE")

#--------------------------------------------------------------------
# Choose library output name
#--------------------------------------------------------------------
if (BUILD_SHARED_LIBS AND UNIX)
    # On Unix-like systems, shared libraries can use the soname system.
    set(ANIMA_ENGINE_LIB_NAME AnimaEngine)
else()
    set(ANIMA_ENGINE_LIB_NAME AnimaEngine)
endif()

#--------------------------------------------------------------------
# Create generated files
#--------------------------------------------------------------------
#if (ANIMA_ENGINE_BUILD_DOCS)
#    configure_file("${ANIMA_ENGINE_SOURCE_DIR}/docs/Doxyfile.in"
#                   "${ANIMA_ENGINE_BINARY_DIR}/docs/Doxyfile" @ONLY)
#endif()

#configure_file("${ANIMA_ENGINE_SOURCE_DIR}/src/AnimaEngine_config.h.in"
#               "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngine_config.h" @ONLY)

#configure_file("${ANIMA_ENGINE_SOURCE_DIR}/src/AnimaEngineConfig.cmake.in"
#               "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngineConfig.cmake" @ONLY)

#configure_file("${ANIMA_ENGINE_SOURCE_DIR}/src/AnimaEngineConfigVersion.cmake.in"
#               "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngineConfigVersion.cmake" @ONLY)

#configure_file("${ANIMA_ENGINE_SOURCE_DIR}/src/AnimaEngine3.pc.in"
#               "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngine3.pc" @ONLY)

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------

list(APPEND AnimaEngine_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Dependencies")

add_subdirectory(AnimaEngine)
#add_subdirectory(AnimaEngineWS)
add_subdirectory(AnimaEngineCore)

if (DOXYGEN_FOUND AND ANIMA_ENGINE_BUILD_DOCS)
    add_subdirectory(docs)
endif()

#--------------------------------------------------------------------
# Install files other than the library
# The library is installed by src/CMakeLists.txt
#--------------------------------------------------------------------
if (ANIMA_ENGINE_INSTALL)
#    install(DIRECTORY include/ANIMA_ENGINE DESTINATION include
#            FILES_MATCHING PATTERN AnimaEngine3.h PATTERN AnimaEngine3native.h)
#
#    install(FILES "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngineConfig.cmake"
#                  "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngineConfigVersion.cmake"
#            DESTINATION lib${LIB_SUFFIX}/cmake/AnimaEngine)

#    install(EXPORT AnimaEngineTargets DESTINATION lib${LIB_SUFFIX}/cmake/AnimaEngine)
#    install(FILES "${ANIMA_ENGINE_BINARY_DIR}/src/AnimaEngine3.pc"
#            DESTINATION lib${LIB_SUFFIX}/pkgconfig)

#    # Only generate this target if no higher-level project already has
#    if (NOT TARGET uninstall)
#        configure_file("${ANIMA_ENGINE_SOURCE_DIR}/cmake_uninstall.cmake.in"
#                       "${ANIMA_ENGINE_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)

#        add_custom_target(uninstall
#                          "${CMAKE_COMMAND}" -P
#                          "${ANIMA_ENGINE_BINARY_DIR}/cmake_uninstall.cmake")
#    endif()
endif()