project(AnimaEngine C CXX)

include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

add_definitions(-DANIMA_ENGINE_EXPORT_ENABLED)
add_definitions(-DGL_GLEXT_PROTOTYPES)

set(common_HEADERS	AnimaEngineCore.h
					AnimaEngine.h
					AnimaWindow_Data.h 
					AnimaWindow.h 
					AnimaAssert.h 
					AnimaTypes.h 
					AnimaAllocator.h 
					AnimaStackAllocator.h
                    AnimaLinearAllocator.h
                    AnimaFreeListAllocator.h
                    AnimaPoolAllocator.h
                    AnimaProxyAllocator.h
                    AnimaVertex.h
                    AnimaModel.h
                    AnimaModelsManager.h
                    AnimaMesh.h
                    AnimaFace.h
                    AnimaBenchmarkTimer.h
                    AnimaMaterialProperty.h
                    AnimaString.h
					AnimaMatrix.h
					AnimaMath.h
					AnimaShadersManager.h
					AnimaShader.h
					AnimaShaderProgram.h)

set(common_SOURCES	AnimaEngineCore.cpp 
					AnimaEngine.cpp
					AnimaAssert.cpp 
					AnimaWindow.cpp 
					AnimaStackAllocator.cpp
                    AnimaLinearAllocator.cpp
                    AnimaFreeListAllocator.cpp
                    AnimaPoolAllocator.cpp
                    AnimaProxyAllocator.cpp
					AnimaVertex.cpp
                    AnimaModel.cpp
                    AnimaModelsManager.cpp
                    AnimaMesh.cpp
                    AnimaFace.cpp
                    AnimaBenchmarkTimer.cpp
                    AnimaMaterialProperty.cpp
                    AnimaString.cpp
					AnimaMatrix.cpp
					AnimaMath.cpp
					AnimaShadersManager.cpp
					AnimaShader.cpp
					AnimaShaderProgram.cpp)
		
if (_ANIMA_ENGINE_COCOA)
    set(AnimaEngineCore_HEADERS	${common_HEADERS}
                                AnimaWindow_CocoaJoystick.h
                                AnimaWindow_CocoaPlatform.h
                                AnimaWindow_CocoaTLS.h)
    set(AnimaEngineCore_SOURCES	${common_SOURCES}
                                AnimaWindow_CocoaJoystick.mm
                                AnimaWindow_CocoaPlatform.mm
                                AnimaWindow_CocoaTLS.cpp)
elseif (_ANIMA_ENGINE_WIN32)
    set(AnimaEngineCore_HEADERS	${common_HEADERS} 
								AnimaWindow_WindowsJoystick.h 
								AnimaWindow_WindowsPlatform.h 
								AnimaWindow_WindowsTLS.h)
    set(AnimaEngineCore_SOURCES	${common_SOURCES} 
								AnimaWindow_WindowsJoystick.cpp 
								AnimaWindow_WindowsPlatform.cpp 
								AnimaWindow_WindowsTLS.cpp)
endif()

if (_ANIMA_ENGINE_NSGL)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} AnimaWindow_NSGLContext.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} AnimaWindow_NSGLContext.mm)
elseif (_ANIMA_ENGINE_WGL)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} AnimaWindow_WGLContext.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} AnimaWindow_WGLContext.cpp)
endif()

if (APPLE)
    # For some reason, CMake doesn't know about .m
    set(CMAKE_CXX_FLAGS "-x objective-c++")
    set_source_files_properties(${AnimaEngineCore_SOURCES} PROPERTIES LANGUAGE CXX)
    set_source_files_properties(${AnimaEngineCore_HEADERS} PROPERTIES LANGUAGE CXX)
endif()

if(BUILD_SHARED_LIBS)
	add_library(${PROJECT_NAME} SHARED ${AnimaEngineCore_SOURCES} ${AnimaEngineCore_HEADERS})
else()
	add_library(${PROJECT_NAME} ${AnimaEngineCore_SOURCES} ${AnimaEngineCore_HEADERS})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${ANIMA_ENGINE_LIB_NAME} POSITION_INDEPENDENT_CODE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(BUILD_SHARED_LIBS)
	# Include version information in the output
	set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${ANIMA_ENGINE_VERSION})
	
	if (WIN32)
		# The ANIMA_ENGINE DLL needs a special compile-time macro and import library name
		set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" IMPORT_PREFIX "")
		set_target_properties(${PROJECT_NAME} PROPERTIES IMPORT_SUFFIX "dll.lib")
	elseif (APPLE)
		# Append -fno-common to the compile flags to work around a bug in
		# Apple's GCC
		get_target_property(AnimaEngineWS_CFLAGS ${PROJECT_NAME} COMPILE_FLAGS)
		if (NOT AnimaEngineCore_CFLAGS)
			set(AnimaEngineCore_CFLAGS "")
		endif()
		set_target_properties(${PROJECT_NAME} PROPERTIES
							COMPILE_FLAGS "${AnimaEngineCore_CFLAGS} -fno-common"
							INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/AnimaEngine/lib${LIB_SUFFIX}")
		set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${ANIMA_ENGINE_VERSION})
	endif()
endif()

target_link_libraries(${PROJECT_NAME} ${AnimaEngine_LIBRARIES})
target_link_libraries(${PROJECT_NAME} optimized ${AnimaEngine_DEP_ASSIMP_RELEASE} debug ${AnimaEngine_DEP_ASSIMP_DEBUG})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_GLEW})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_BOOST_LIBS})

if (ANIMA_ENGINE_INSTALL)
	FILE(GLOB install_include "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	INSTALL(FILES ${install_include} DESTINATION AnimaEngine/include)

		INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION AnimaEngine/bin
										LIBRARY DESTINATION AnimaEngine/lib
										ARCHIVE DESTINATION AnimaEngine/lib)

	if(WIN32)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll 	DESTINATION AnimaEngine/bin CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimpd.dll 	DESTINATION AnimaEngine/bin CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll 	DESTINATION AnimaEngine/bin CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll 	DESTINATION AnimaEngine/bin CONFIGURATIONS MinSizeRel)
	else()		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib 	DESTINATION AnimaEngine/bin CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib 		DESTINATION AnimaEngine/bin CONFIGURATIONS Debug)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib 		DESTINATION AnimaEngine/bin CONFIGURATIONS Debug)		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.1.1.dylib 	DESTINATION AnimaEngine/bin CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.3.dylib 		DESTINATION AnimaEngine/bin CONFIGURATIONS RelWithDebInfo)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimpd.dylib 		DESTINATION AnimaEngine/bin CONFIGURATIONS RelWithDebInfo)		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib 	DESTINATION AnimaEngine/bin CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib 		DESTINATION AnimaEngine/bin CONFIGURATIONS Release)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib 			DESTINATION AnimaEngine/bin CONFIGURATIONS Release)		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib 	DESTINATION AnimaEngine/bin CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib 		DESTINATION AnimaEngine/bin CONFIGURATIONS MinSizeRel)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib 			DESTINATION AnimaEngine/bin CONFIGURATIONS MinSizeRel)
	endif()
endif()