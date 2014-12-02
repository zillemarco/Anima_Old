project(CorpusEditor C CXX)

set(${PROJECT_NAME}_headers )
set(${PROJECT_NAME}_sources main.cpp)

include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngine/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} AnimaEngine)
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
target_link_libraries(${PROJECT_NAME} optimized ${AnimaEngine_DEP_ASSIMP_RELEASE} debug ${AnimaEngine_DEP_ASSIMP_DEBUG})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_BOOST_LIBS})

qt5_use_modules(${PROJECT_NAME} Widgets)

if (WIN32)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimpd.dll"
						COMMENT "Copia assimpd.dll")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimp.dll"
						COMMENT "Copia assimp.dll")
elseif(APPLE)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E cp ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimpd.3.1.1.dylib"
						COMMENT "Copia libassimpd.3.1.1.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E cp ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimpd.3.dylib"
						COMMENT "Copia libassimpd.3.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E cp ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimpd.dylib"
						COMMENT "Copia libassimpd.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E cp ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.1.1.dylib"
						COMMENT "Copia libassimp.3.1.1.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E cp ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.dylib"
						COMMENT "Copia libassimp.3.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E cp ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.dylib"
						COMMENT "Copia libassimp.dylib")
endif()

if (ANIMA_ENGINE_INSTALL)
	FILE(GLOB install_include "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	INSTALL(FILES ${install_include} DESTINATION CorpusEditor/include)
	
	# Debug installation
	if(WIN32)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor.exe DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor.ilk DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor.pdb DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor.exe DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor.ilk DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor.pdb DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
	else()
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)
	endif()
	
	# Release installation
	if(WIN32)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/CorpusEditor.exe DESTINATION CorpusEditor/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/CorpusEditor.exe DESTINATION Anima/bin/Release CONFIGURATIONS Release)
		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll DESTINATION CorpusEditor/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll DESTINATION Anima/bin/Release CONFIGURATIONS Release)
	else()
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib DESTINATION CorpusEditor/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib DESTINATION CorpusEditor/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib DESTINATION CorpusEditor/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib DESTINATION Anima/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib DESTINATION Anima/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib DESTINATION Anima/bin/Release CONFIGURATIONS Release)
	endif()
	
	# RelWithDebInfo installation
	if(WIN32)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor.exe DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor.ilk DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor.pdb DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor.exe DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor.ilk DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor.pdb DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
	else()
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
	endif()
	
	# MinSizeRel installation
	if(WIN32)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel/CorpusEditor.exe DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel/CorpusEditor.exe DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
	else()
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
	endif()
endif()