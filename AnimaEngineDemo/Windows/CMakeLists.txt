project(AnimaEngineDemo C CXX)

set(${PROJECT_NAME}_headers main.h)
set(${PROJECT_NAME}_sources main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../../AnimaEngine/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	include_directories(${ANIMA_ENGINE_NVTOOLSEXT_INCLUDE})
endif()

add_definitions("/wd4521")
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
	SET(plugin_dest_dir ${CMAKE_INSTALL_PREFIX}/AnimaEditor/plugins)
	SET(qtconf_dest_dir AnimaEditor)
	SET(APPS "\${CMAKE_INSTALL_PREFIX}/AnimaEngineDemo/${PROJECT_NAME}.exe")
	set(DIRS 	"${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib"
				"${PROJECT_SOURCE_DIR}/../../Dependencies/OpenGL/Lib"
				"${CMAKE_INSTALL_PREFIX}/AnimaEngineDemo/bin")

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION AnimaEngineDemo COMPONENT Runtime
    								RUNTIME DESTINATION AnimaEngineDemo COMPONENT Runtime)
endif()