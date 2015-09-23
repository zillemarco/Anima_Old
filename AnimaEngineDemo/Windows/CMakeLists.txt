project(AnimaEngineDemo C CXX)

set(${PROJECT_NAME}_headers main.h)
set(${PROJECT_NAME}_sources main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../../AnimaEngine/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	include_directories(${ANIMA_ENGINE_NVTOOLSEXT_INCLUDE})
endif()

set(CMAKE_CXX_FLAGS "/wd4251 /wd4996 /EHsc")

add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} AnimaEngine)
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_ASSIMP})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_BOOST_LIBS})

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	target_link_libraries(${PROJECT_NAME} ${ANIMA_ENGINE_NVTOOLSEXT_LIB})
endif()

add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/assimp.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimp.dll"
					COMMENT "Copia assimp.dll")
if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	INSTALL(FILES ${ANIMA_ENGINE_NVTOOLSEXT_BIN} DESTINATION AnimaEngine/bin)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${ANIMA_ENGINE_NVTOOLSEXT_BIN} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/nvToolsExt64_1.dll"
						COMMENT "Copia nvtoolsext.dll")
endif()

if (ANIMA_ENGINE_INSTALL)
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION AnimaEngineDemo
									LIBRARY DESTINATION AnimaEngineDemo
									ARCHIVE DESTINATION AnimaEngineDemo)

	INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/../../AnimaEngine/data 	DESTINATION AnimaEngineDemo)
		
	if(WIN32)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/assimp.dll 	DESTINATION AnimaEngineDemo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/OpenGL/Lib/glew32.dll 	DESTINATION AnimaEngineDemo)
		INSTALL(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/AnimaEngine.dll" DESTINATION AnimaEngineDemo)
		if(ANIMA_ENGINE_USE_NVTOOLSEXT)
			INSTALL(FILES ${ANIMA_ENGINE_NVTOOLSEXT_BIN} DESTINATION AnimaEngineDemo)
		endif()
	else()		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/libassimp.3.1.1.dylib 	DESTINATION AnimaEngineDemo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/libassimp.3.dylib 		DESTINATION AnimaEngineDemo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/libassimp.dylib 			DESTINATION AnimaEngineDemo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/OpenGL/Lib/libGLEW.1.12.0.dylib 		DESTINATION AnimaEngineDemo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/OpenGL/Lib/libGLEW.1.12.dylib 		DESTINATION AnimaEngineDemo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../../Dependencies/OpenGL/Lib/libGLEW.dylib 			DESTINATION AnimaEngineDemo)
	endif()
endif()