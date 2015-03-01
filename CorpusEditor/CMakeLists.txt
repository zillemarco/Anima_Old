project(CorpusEditor C CXX)

set(${PROJECT_NAME}_headers CorpusMainWindow.h
							EditWindow.h
							CorpusOGLWindowBase.h
							NewProjectWindow.h
							CorpusDocument.h
							ResourceManagerTab.h
							WorldEditorTab.h
							CRModelViewer.h
							Window.h)
set(${PROJECT_NAME}_sources main.cpp 
							CorpusMainWindow.cpp
							EditWindow.cpp
							CorpusOGLWindowBase.cpp
							NewProjectWindow.cpp
							CorpusDocument.cpp
							ResourceManagerTab.cpp
							WorldEditorTab.cpp
							CRModelViewer.cpp
							Window.cpp)

set(UI_FILES "")
set(RC_FILES CorpusEditor.qrc)

qt5_wrap_ui(UI_HDRS ${UI_FILES})
qt5_add_resources(RC_SRC_FILES ${RC_FILES})

include_directories(${PROJECT_SOURCE_DIR}/../AnimaEngine/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	include_directories(${ANIMA_ENGINE_NVTOOLSEXT_INCLUDE})
endif()

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

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	target_link_libraries(${PROJECT_NAME} ${ANIMA_ENGINE_NVTOOLSEXT_LIB})
endif()

qt5_use_modules(${PROJECT_NAME} Widgets Gui OpenGL)

if (WIN32)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimpd.dll"
						COMMENT "Copia assimpd.dll")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimp.dll"
						COMMENT "Copia assimp.dll")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/glew32.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/glew32.dll"
						COMMENT "Copia assimp.dll")
	if(ANIMA_ENGINE_USE_NVTOOLSEXT)
		INSTALL(FILES ${ANIMA_ENGINE_NVTOOLSEXT_BIN} DESTINATION AnimaEngine/bin)
		add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
							COMMAND ${CMAKE_COMMAND} ARGS -E copy ${ANIMA_ENGINE_NVTOOLSEXT_BIN} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/nvToolsExt64_1.dll"
							COMMENT "Copia nvtoolsext.dll")
	endif()
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
	IF(APPLE)
		SET(plugin_dest_dir ${CMAKE_INSTALL_PREFIX}/AnimaCorpus/${PROJECT_NAME}.app/Contents/plugins)
		SET(qtconf_dest_dir AnimaCorpus/${PROJECT_NAME}.app/Contents/Resources)
		SET(APPS "\${CMAKE_INSTALL_PREFIX}/AnimaCorpus/${PROJECT_NAME}.app")
		set(DIRS 	"${ANIMA_ENGINE_QT5_ROOT_PATH}/bin" 
					"${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64"
					"${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib"
					"${CMAKE_INSTALL_PREFIX}/AnimaEngine/lib")
	ENDIF(APPLE)
	
	IF(WIN32)
		SET(plugin_dest_dir ${CMAKE_INSTALL_PREFIX}/AnimaCorpus/plugins)
		SET(qtconf_dest_dir AnimaCorpus)
		SET(APPS "\${CMAKE_INSTALL_PREFIX}/AnimaCorpus/${PROJECT_NAME}.exe")
		set(DIRS 	"${ANIMA_ENGINE_QT5_ROOT_PATH}/bin"
					"${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64"
					"${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib" 
					"${CMAKE_INSTALL_PREFIX}/AnimaEngine/bin")
	ENDIF(WIN32)

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION AnimaCorpus COMPONENT Runtime
    								RUNTIME DESTINATION AnimaCorpus COMPONENT Runtime)

	INSTALL(DIRECTORY "${ANIMA_ENGINE_QT5_ROOT_PATH}/plugins/platforms" DESTINATION ${plugin_dest_dir} COMPONENT Runtime)

	INSTALL(CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")" COMPONENT Runtime)
					
	INSTALL(CODE "file(GLOB_RECURSE QTPLUGINS \"${plugin_dest_dir}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
			include(BundleUtilities)
			fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")" 
			COMPONENT Runtime)

	set(CPACK_BINARY_DRAGNDROP ON)
	include(CPack)
endif()