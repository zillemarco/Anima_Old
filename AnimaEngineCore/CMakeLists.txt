project(AnimaEngineCore C CXX)

add_definitions(-DANIMA_ENGINE_CORE_EXPORT_ENABLED)

set(${PROJECT_NAME}_headers AnimaEngineCore.h AnimaAssert.h AnimaTypes.h AnimaAllocator.h AnimaStackAllocator.h)
set(${PROJECT_NAME}_sources AnimaAssert.cpp AnimaEngineCore.cpp AnimaStackAllocator.cpp)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib/static)