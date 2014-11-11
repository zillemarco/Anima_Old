project(AnimaEngine C CXX)

set(${PROJECT_NAME}_headers )
set(${PROJECT_NAME}_sources main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngineCore/)
include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngineWS/)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} AnimaEngineCore ${EXTRA_LIBS})
target_link_libraries(${PROJECT_NAME} AnimaEngineWS ${EXTRA_LIBS})

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib/static)