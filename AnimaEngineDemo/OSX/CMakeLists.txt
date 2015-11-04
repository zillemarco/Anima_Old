project(AnimaEngineDemo C CXX)

set(${PROJECT_NAME}_headers AppDelegate.h AnimaEngineDemoView.h)
set(${PROJECT_NAME}_sources main.mm AppDelegate.mm AnimaEngineDemoView.mm)

set(${PROJECT_NAME}_XIB_FILES AnimaEngineDemo)
set(${PROJECT_NAME}_XIB_FILES_WE AnimaEngineDemo.xib)

include_directories(${PROJECT_SOURCE_DIR}/../../AnimaEngine/)
include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

#set_source_files_properties(${PROJECT_NAME}_sources PROPERTIES LANGUAGE CXX)
#set_source_files_properties(${PROJECT_NAME}_headers PROPERTIES LANGUAGE CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(MACOSX_BUNDLE_BUNDLE_NAME AnimaEngineDemo)
set(MACOSX_BUNDLE_GUI_IDENTIFIER "AnimaEngineDemo")
#set( MACOSX_BUNDLE_ICON_FILE AnimaEditor.icns)
#set( MACOSX_BUNDLE_INFO_STRING "${ANIMA_EDITOR_VERSION_MAJOR}.${ANIMA_EDITOR_VERSION_MINOR}.${ANIMA_EDITOR_VERSION_PATCH}, Copyright 2015 AnimaEngine team")
#set( MACOSX_BUNDLE_SHORT_VERSION_STRING "${ANIMA_EDITOR_VERSION_MAJOR}.${ANIMA_EDITOR_VERSION_MINOR}" )
##set( MACOSX_BUNDLE_LONG_VERSION_STRING "${ANIMA_EDITOR_VERSION_MAJOR}.${ANIMA_EDITOR_VERSION_MINOR}.${ANIMA_EDITOR_VERSION_PATCH}" )
##set( MACOSX_BUNDLE_BUNDLE_VERSION "${ANIMA_EDITOR_VERSION_MAJOR}.${ANIMA_EDITOR_VERSION_MINOR}.${ANIMA_EDITOR_VERSION_PATCH}" )
set(MACOSX_BUNDLE_COPYRIGHT "(C) 2015 Zille Marco")
set(MACOSX_BUNDLE_NSMAIN_NIB_FILE "MainMenu")
set(MACOSX_BUNDLE_NSPRINCIPAL_CLASS "NSApplication")
#set( MACOSX_BUNDLE_INFO_PLIST AnimaEngineDemoBundleInfo.plist.in)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources} ${${PROJECT_NAME}_XIB_FILES_WE})

set(CMAKE_CXX_FLAGS "-x objective-c++")
set(CMAKE_Co_FLAGS "-x objective-c++")

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST Info.plist)
#set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

target_link_libraries(${PROJECT_NAME} AnimaEngine)
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_ASSIMP})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_BOOST_LIBS})

add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/libassimp.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.1.1.dylib"
					COMMENT "Copia libassimp.3.1.1.dylib")
					
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/libassimp.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.dylib"
					COMMENT "Copia libassimp.3.dylib")
					
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib/libassimp.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.dylib"
					COMMENT "Copia libassimp.dylib")

add_custom_command (TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/${PROJECT_NAME}.app/Contents/Resources)

find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
if (${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
  	message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with 
                    	the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
endif()

foreach(xib ${${PROJECT_NAME}_XIB_FILES})
 	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD 
                      	COMMAND ${IBTOOL} 	--errors --warnings --notices --output-format human-readable-text 
                              				--compile ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/${PROJECT_NAME}.app/Contents/Resources/${xib}.nib 
                             				${PROJECT_SOURCE_DIR}/${xib}.xib
                      	COMMENT "Compiling ${xib}.xib")

endforeach()


if (ANIMA_ENGINE_INSTALL)
	SET(APPS "\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/${PROJECT_NAME}.app")
	set(DIRS 	"${PROJECT_SOURCE_DIR}/../../Dependencies/Assimp/Lib"
				"${PROJECT_SOURCE_DIR}/../../Dependencies/OpenGL/Lib"
				"${CMAKE_INSTALL_PREFIX}/AnimaEngineDemo/bin")

    INSTALL(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION ${PROJECT_NAME} COMPONENT Runtime
    								RUNTIME DESTINATION ${PROJECT_NAME} COMPONENT Runtime)
endif()