cmake_minimum_required(VERSION 3.0.2)

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

# impostazioni ambiente di sviluppo
#add_definitions(-D)									# qua si impostano le definizioni globali da compilatore
#include_directories(${PROJECT_SOURCE_DIR}/inc)			# qua si imposta la cartella di include visibile a tutti i sottoprogetti
#link_directories(${PROJECT_SOURCE_DIR}/install)

message("Binary tree path: ${PROJECT_BINARY_DIR}")

# aggiunta delle sottocartelle
add_subdirectory(AnimaEngineLib)
add_subdirectory(AnimaEngine)