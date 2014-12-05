project(CorpusEditor C CXX)

set(${PROJECT_NAME}_headers CorpusMainWindow.h
							EditWindow.h
							CorpusOGLWindow.h
							NewProjectWindow.h
							CorpusDocument.h
							ResourceManagerTab.h
							WorldEditorTab.h)
set(${PROJECT_NAME}_sources main.cpp 
							CorpusMainWindow.cpp
							EditWindow.cpp
							CorpusOGLWindow.cpp
							NewProjectWindow.cpp
							CorpusDocument.cpp
							ResourceManagerTab.cpp
							WorldEditorTab.cpp)

set(UI_FILES "")
set(RC_FILES CorpusEditor.qrc)

qt5_wrap_ui(UI_HDRS ${UI_FILES})
qt5_add_resources(RC_SRC_FILES ${RC_FILES})

include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngine/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

if(APPLE)
	set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
	set(CorpusEditor_ICON ${CMAKE_CURRENT_SOURCE_DIR}/Res/CorpusEditor.icns)
	set_source_files_properties(${CorpusEditor_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	set( MACOSX_BUNDLE_BUNDLE_NAME CorpusEditor)
	set( MACOSX_BUNDLE_GUI_IDENTIFIER "CorpusEditor")
	set( MACOSX_BUNDLE_ICON_FILE CorpusEditor.icns)
	set( MACOSX_BUNDLE_INFO_STRING "${CORPUS_EDITOR_VERSION_MAJOR}.${CORPUS_EDITOR_VERSION_MINOR}.${CORPUS_EDITOR_VERSION_PATCH}, Copyright 2014 AnimaEngine team")
	set( MACOSX_BUNDLE_SHORT_VERSION_STRING "${CORPUS_EDITOR_VERSION_MAJOR}.${CORPUS_EDITOR_VERSION_MINOR}" )
	set( MACOSX_BUNDLE_LONG_VERSION_STRING "${CORPUS_EDITOR_VERSION_MAJOR}.${CORPUS_EDITOR_VERSION_MINOR}.${CORPUS_EDITOR_VERSION_PATCH}" )
	set( MACOSX_BUNDLE_BUNDLE_VERSION "${CORPUS_EDITOR_VERSION_MAJOR}.${CORPUS_EDITOR_VERSION_MINOR}.${CORPUS_EDITOR_VERSION_PATCH}" )
	set( MACOSX_BUNDLE_COPYRIGHT "(C) 2014 Zille Marco")
	set( MACOSX_BUNDLE_INFO_PLIST CorpusEditorBundleInfo.plist.in)
	
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${CorpusEditor_ICON} ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources} ${RC_SRC_FILES} ${UI_HDRS} )
else()
	add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources} ${RC_SRC_FILES} ${UI_HDRS} CorpusEditor.rc)
endif()

target_link_libraries(${PROJECT_NAME} AnimaEngine)
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
target_link_libraries(${PROJECT_NAME} optimized ${AnimaEngine_DEP_ASSIMP_RELEASE} debug ${AnimaEngine_DEP_ASSIMP_DEBUG})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_BOOST_LIBS})

qt5_use_modules(${PROJECT_NAME} Widgets Gui)

if (WIN32)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimpd.dll"
						COMMENT "Copia assimpd.dll")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimp.dll"
						COMMENT "Copia assimp.dll")
elseif(APPLE)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimpd.3.1.1.dylib"
						COMMENT "Copia libassimpd.3.1.1.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimpd.3.dylib"
						COMMENT "Copia libassimpd.3.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimpd.dylib"
						COMMENT "Copia libassimpd.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.1.1.dylib"
						COMMENT "Copia libassimp.3.1.1.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.dylib"
						COMMENT "Copia libassimp.3.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.dylib"
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
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor DESTINATION CorpusEditor/bin/Debug CONFIGURATIONS Debug)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/CorpusEditor DESTINATION Anima/bin/Debug CONFIGURATIONS Debug)

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
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/CorpusEditor DESTINATION CorpusEditor/bin/Release CONFIGURATIONS Release)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/CorpusEditor DESTINATION Anima/bin/Release CONFIGURATIONS Release)

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
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor DESTINATION CorpusEditor/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/CorpusEditor DESTINATION Anima/bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)

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
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel/CorpusEditor DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel/CorpusEditor DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)

		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib DESTINATION CorpusEditor/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib DESTINATION Anima/bin/MinSizeRel CONFIGURATIONS MinSizeRel)
	endif()
endif()