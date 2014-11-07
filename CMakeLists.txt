cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_PREFIX_PATH ~/Qt/5.3/clang_64)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)

# info sul progetto
project(Anima C CXX)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")

set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
#set(CMAKE_CXX_FLAGS “${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}”)

# impostazioni ambiente di sviluppo
#find_package(PkgConfig REQUIRED)
#find_package(GTK2 2.10 REQUIRED gtk)

#pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
#include_directories(${GTK3_INCLUDE_DIRS})
#link_directories(${GTK3_LIBRARY_DIRS})
#add_definitions(${GTK3_CFLAGS_OTHER})

#include_directories(${GTK2_INCLUDE_DIRS})
#add_definitions(${GTK2_DEFINITIONS})

# aggiunta delle sottocartelle
add_subdirectory(AnimaEngineLib)
add_subdirectory(AnimaEngine)