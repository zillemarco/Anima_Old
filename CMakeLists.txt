cmake_minimum_required(VERSION 3.0.2)

IF(WIN32)
	set(CMAKE_PREFIX_PATH C:\\Qt\\5.3\\msvc2013_64)
ELSEIF(UNIX)
	set(CMAKE_PREFIX_PATH ~/Qt/5.3/clang_64)
ELSE()
	message("System not recognized")
	message(FATAL_ERROR)
ENDIF()

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

project(Anima C CXX)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")

set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Output/bin)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

IF(NOT Qt5Widgets_FOUND)
	message("Qt5 needs to be installed")

	IF(WIN32)
		message("If Qt5 is installed check if the installation path is at 'C:\Qt'")
	ELSEIF(UNIX)
		message("If Qt5 is installed check if the installation path is at '~/Qt/5.3/clang_64'")
	ENDIF()

	message(FATAL_ERROR)
ENDIF()


IF(WIN32)
	message("Be sure that 'C:\\Qt\\5.3\\msvc2013_64\\bin' is included by the Windows PATH environment variable")
ENDIF()

add_subdirectory(AnimaEngineLib)
add_subdirectory(AnimaEngine)