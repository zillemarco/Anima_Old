project(AnimaEngine C CXX)

set(${PROJECT_NAME}_headers CustomWindow.h)
set(${PROJECT_NAME}_sources main.cpp CustomWindow.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngineCore/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} AnimaEngineCore ${EXTRA_LIBS})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib/static)