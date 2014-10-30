project(AnimaEngine C CXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../../install)

set(${PROJECT_NAME}_headers "")
set(${PROJECT_NAME}_sources main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngineLib/)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} AnimaEngineLib)

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib/static)