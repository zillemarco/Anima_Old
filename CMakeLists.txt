cmake_minimum_required(VERSION 3.0.2)
set(CMAKE_CXX_STANDARD 11)

project(Anima C CXX)

set(ANIMA_ENGINE_VERSION_MAJOR "0")
set(ANIMA_ENGINE_VERSION_MINOR "0")
set(ANIMA_ENGINE_VERSION_PATCH "0")

set(ANIMA_ENGINE_VERSION "${ANIMA_ENGINE_VERSION_MAJOR}.${ANIMA_ENGINE_VERSION_MINOR}.${ANIMA_ENGINE_VERSION_PATCH}")

set(ANIMA_EDITOR_VERSION_MAJOR "0")
set(ANIMA_EDITOR_VERSION_MINOR "0")
set(ANIMA_EDITOR_VERSION_PATCH "0")

set(ANIMA_EDITOR_VERSION "${ANIMA_EDITOR_VERSION_MAJOR}.${ANIMA_EDITOR_VERSION_MINOR}.${ANIMA_EDITOR_VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/bin)

set(LIB_SUFFIX "" CACHE STRING "Takes an empty string or 64. Directory where lib will be installed: lib or lib64")
set(ANIMA_ENGINE_BOOST_ROOT_PATH "" CACHE STRING "Path where CMake can find boost")
set(ANIMA_ENGINE_QT5_ROOT_PATH "" CACHE STRING "Path where CMake can find Qt5")
set(ANIMA_ENGINE_NVTOOLSEXT_ROOT_PATH "" CACHE STRING "Path where CMake can find NVToolsExt")
set(ANIMA_ENGINE_PYTHON_ROOT_PATH "" CACHE STRING "Path where CMake can find Python")

if(APPLE)
    set(ANIMA_ENGINE_PYTHON_INC_DIR_NAME "python2.7" CACHE STRING "Name of Python include directory")
else()
	set(ANIMA_ENGINE_PYTHON_LIB_NAME "" CACHE STRING "Name of Python library")
endif()

option(ANIMA_ENGINE_BUILD_DOCS "Build the ANIMA_ENGINE documentation" OFF)
option(ANIMA_ENGINE_INSTALL "Generate installation target" OFF)
option(BUILD_SHARED_LIBS "Build AnimaEngine as shared library" OFF)
option(ANIMA_ENGINE_USE_NVTOOLSEXT "Enable NVToolsExt for debugging" OFF)
option(ANIMA_ENGINE_EXPORT_TO_PYTHON "Enable AnimaEngine export to Python modulde" OFF)
option(ANIMA_ENGINE_BUILD_DEMO "Enable AnimaEngine demos build" OFF)

if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON)
endif()

if (BUILD_SHARED_LIBS)
    set(_ANIMA_ENGINE_BUILD_DLL 1)
endif()

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/Dependencies/CMake)

find_package(OpenGL REQUIRED)
find_package(OCL)
find_package(Threads REQUIRED)

if (ANIMA_ENGINE_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

set(BOOST_ROOT ${ANIMA_ENGINE_BOOST_ROOT_PATH})
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
find_package(Boost 1.57.0 REQUIRED COMPONENTS thread system date_time chrono python filesystem)

if(Boost_FOUND)
	list(APPEND AnimaEngine_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
	set(AnimaEngine_BOOST_LIBS ${Boost_LIBRARIES})
endif()

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	find_path(NVTEXT_INC NAMES "include/nvToolsExt.h" PATHS ${ANIMA_ENGINE_NVTOOLSEXT_ROOT_PATH} NO_DEFAULT_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
	set(ANIMA_ENGINE_NVTOOLSEXT_INCLUDE "${NVTEXT_INC}/include")
	find_path(NVTEXT_BIN NAMES "bin/x64/nvToolsExt64_1.dll" PATHS ${ANIMA_ENGINE_NVTOOLSEXT_ROOT_PATH} NO_DEFAULT_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
	set(ANIMA_ENGINE_NVTOOLSEXT_BIN "${NVTEXT_BIN}/bin/x64/nvToolsExt64_1.dll")
	find_path(NVTEXT_LIB NAMES "lib/x64/nvToolsExt64_1.lib" PATHS ${ANIMA_ENGINE_NVTOOLSEXT_ROOT_PATH} NO_DEFAULT_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_PATH NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
	set(ANIMA_ENGINE_NVTOOLSEXT_LIB "${NVTEXT_LIB}/lib/x64/nvToolsExt64_1.lib")
	
	if ("${NVTEXT_INC}" STREQUAL "NVTEXT_INC-NOTFOUND" OR "${NVTEXT_BIN}" STREQUAL "NVTEXT_BIN-NOTFOUND" OR "${NVTEXT_LIB}" STREQUAL "NVTEXT_LIB-NOTFOUND")
		unset(NVTEXT_INC CACHE)
		unset(NVTEXT_BIN CACHE)
		unset(NVTEXT_LIB CACHE)
		message(FATAL_ERROR "Trying to use NVToolsExt but can't find the directory")
	endif()
	
	unset(NVTEXT_INC CACHE)
	unset(NVTEXT_BIN CACHE)
	unset(NVTEXT_LIB CACHE)
endif()

#--------------------------------------------------------------------
# Set compiler specific flags
#--------------------------------------------------------------------
if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    if (NOT USE_MSVC_RUNTIME_LIBRARY_DLL)
        foreach (flag 	CMAKE_C_FLAGS
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
    set(ANIMA_ENGINE_PKG_LIBS "${ANIMA_ENGINE_PKG_LIBS} -lopengl32 -lglu32.lib")
    list(APPEND AnimaEngine_INCLUDE_DIRS "${OPENGL_INCLUDE_DIR}")
    list(APPEND AnimaEngine_LIBRARIES "${OPENGL_gl_LIBRARY}")
	list(APPEND AnimaEngine_LIBRARIES glu32.lib)
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
# Choose library output name
#--------------------------------------------------------------------
set(ANIMA_ENGINE_LIB_NAME AnimaEngine)
set(ANIMA_EDITOR_NAME AnimaEditor)

#--------------------------------------------------------------------
# AnimaEngine dependencies
#--------------------------------------------------------------------
if (WIN32)	
	set(AnimaEngine_DEP_ASSIMP ${PROJECT_SOURCE_DIR}/Dependencies/Assimp/Lib/assimp.lib)
	set(AnimaEngine_DEP_GLEW ${PROJECT_SOURCE_DIR}/Dependencies/OpenGL/Lib/glew32.lib)
	set(AnimaEngine_DEP_BULLET 	${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/Bullet2FileLoader.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/Bullet3Collision.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/Bullet3Common.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/Bullet3Dynamics.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/Bullet3Geometry.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/Bullet3OpenCL_clew.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/BulletCollision.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/BulletDynamics.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/BulletSoftBody.lib
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/Win/LinearMath.lib)
elseif(APPLE)
	set(AnimaEngine_DEP_ASSIMP ${PROJECT_SOURCE_DIR}/Dependencies/Assimp/Lib/libassimp.3.1.1.dylib)
	set(AnimaEngine_DEP_GLEW ${PROJECT_SOURCE_DIR}/Dependencies/OpenGL/Lib/libGLEW.a)
	set(AnimaEngine_DEP_BULLET 	${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBullet2FileLoader.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBullet3Collision.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBullet3Common.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBullet3Dynamics.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBullet3Geometry.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBullet3OpenCL_clew.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBulletCollision.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBulletDynamics.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBulletFileLoader.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBulletSoftBody.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBulletWorldImporter.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libBulletXmlWorldImporter.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libConvexDecomposition.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libGIMPACTUtils.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libHACD.a
								${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Lib/MacOS/libLinearMath.a)
endif()

list(APPEND AnimaEngine_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Dependencies/OpenGL")
list(APPEND AnimaEngine_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Dependencies/Assimp/Include")
list(APPEND AnimaEngine_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/Dependencies/Bullet/Include")

# Dipendenze da OpenCL
list(APPEND AnimaEngine_INCLUDE_DIRS "${OCL_INCLUDE_DIR}")
set(AnimaEngine_DEP_OPENCL "${OCL_LIBRARIES}")

#--------------------------------------------------------------------
# Export ANIMA_ENGINE library dependencies
#--------------------------------------------------------------------
set(ANIMA_ENGINE_LIBRARIES ${AnimaEngine_LIBRARIES} CACHE STRING "Dependencies of ANIMA_ENGINE")

#--------------------------------------------------------------------
# Add subdirectories
#--------------------------------------------------------------------
add_subdirectory(AnimaEngine)

if(ANIMA_ENGINE_BUILD_DEMO)
	add_subdirectory(AnimaEngineDemo)
endif()

if (DOXYGEN_FOUND AND ANIMA_ENGINE_BUILD_DOCS)
    add_subdirectory(docs)
endif()