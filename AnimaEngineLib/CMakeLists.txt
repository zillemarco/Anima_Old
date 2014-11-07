project(AnimaEngineLib C CXX)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/../../install)
add_definitions(-DANIMA_ENGINE_EXPORT_ENABLED)

set(${PROJECT_NAME}_headers AnimaEngineLib.h AnimaAssert.h AnimaTypes.h)
set(${PROJECT_NAME}_sources AnimaAssert.cpp)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

set_target_properties(AnimaEngineLib PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES})

qt5_use_modules(${PROJECT_NAME} Widgets)

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib/static)