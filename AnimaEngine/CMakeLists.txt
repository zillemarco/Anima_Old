project(AnimaEngine C CXX)

include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	include_directories(${ANIMA_ENGINE_NVTOOLSEXT_INCLUDE})
	add_definitions(-DANIMA_ENGINE_USE_NVTOOLS)
endif()

add_definitions(-DANIMA_ENGINE_EXPORT_ENABLED)
add_definitions(-DGL_GLEXT_PROTOTYPES)

set(ANIMA_ENGINE_MODELS_PATH "${PROJECT_SOURCE_DIR}/data/models/")
set(ANIMA_ENGINE_SHADERS_PATH "${PROJECT_SOURCE_DIR}/data/shaders/")
set(ANIMA_ENGINE_TEXTURES_PATH "${PROJECT_SOURCE_DIR}/data/textures/")

configure_file("${PROJECT_SOURCE_DIR}/AnimaEngineConfig.h.in" "${PROJECT_SOURCE_DIR}/AnimaEngineConfig.h")

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
                    AnimaModelsManager.h
                    AnimaMesh.h
                    AnimaFace.h
                    AnimaBenchmarkTimer.h
                    AnimaMaterial.h
                    AnimaString.h
					AnimaMatrix.h
					AnimaMath.h
					AnimaShadersManager.h
					AnimaShader.h
					AnimaShaderProgram.h
					AnimaQuaternion.h
					AnimaCamera.h
					AnimaFirstPersonCamera.h
					AnimaThirdPersonCamera.h
					AnimaCamerasManager.h
					AnimaTexturesManager.h
					AnimaTexture.h
					AnimaRenderer.h
					AnimaDataGenerator.h
					AnimaColorGenerator.h
					AnimaDataGeneratorsManager.h
					AnimaMaterialsManager.h
					AnimaTransformation.h
					AnimaMappedValues.h
					AnimaLight.h
					AnimaLightsManager.h
					AnimaVectorGenerator.h
					AnimaScene.h
					AnimaScenesManager.h
					AnimaGBuffer.h
					AnimaFPSTimer.h
					AnimaMeshCreator.h
					AnimaSceneObject.h
					AnimaArray.h
					AnimaFrustum.h
					AnimaMappedArray.h
					AnimaTypeMappedArray.h
					AnimaMeshesManager.h
					AnimaModelInstancesManager.h
					AnimaModel.h
					AnimaModelInstance.h
					AnimaMeshInstance.h
					AnimaMeshInstancesManager.h)

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
                    AnimaModelsManager.cpp
                    AnimaMesh.cpp
                    AnimaFace.cpp
                    AnimaBenchmarkTimer.cpp
                    AnimaMaterial.cpp
                    AnimaString.cpp
					AnimaMatrix.cpp
					AnimaMath.cpp
					AnimaShadersManager.cpp
					AnimaShader.cpp
					AnimaShaderProgram.cpp
					AnimaQuaternion.cpp
					AnimaCamera.cpp
					AnimaFirstPersonCamera.cpp
					AnimaThirdPersonCamera.cpp
					AnimaCamerasManager.cpp
					AnimaTexturesManager.cpp
					AnimaTexture.cpp
					AnimaRenderer.cpp
					AnimaDataGenerator.cpp
					AnimaColorGenerator.cpp
					AnimaDataGeneratorsManager.cpp
					AnimaMaterialsManager.cpp
					AnimaTransformation.cpp
					AnimaMappedValues.cpp
					AnimaLight.cpp
					AnimaLightsManager.cpp
					AnimaVectorGenerator.cpp
					AnimaScene.cpp
					AnimaScenesManager.cpp
					AnimaGBuffer.cpp
					AnimaFPSTimer.cpp
					AnimaMeshCreator.cpp
					AnimaSceneObject.cpp
					AnimaArray.cpp
					AnimaFrustum.cpp
					AnimaMappedArray.cpp
					AnimaTypeMappedArray.cpp
					AnimaMeshesManager.cpp
					AnimaModelInstancesManager.cpp
					AnimaModel.cpp
					AnimaModelInstance.cpp
					AnimaMeshInstance.cpp
					AnimaMeshInstancesManager.cpp)

source_group(Engine FILES AnimaEngine.h AnimaEngine.cpp AnimaEngineConfig.h AnimaEngineCore.h AnimaEngineCore.cpp ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/AnimaEngine_automoc.cpp)

source_group(Engine\\Tools FILES AnimaAssert.h AnimaAssert.cpp AnimaBenchmarkTimer.h AnimaBenchmarkTimer.cpp AnimaFPSTimer.h AnimaFPSTimer.cpp)
source_group(Engine\\Types FILES AnimaTypes.h)

source_group(Engine\\Managers FILES AnimaModelsManager.h AnimaShadersManager.h AnimaCamerasManager.h AnimaTexturesManager.h AnimaDataGeneratorsManager.h AnimaMaterialsManager.h AnimaLightsManager.h AnimaScenesManager.h AnimaModelsManager.cpp AnimaShadersManager.cpp AnimaCamerasManager.cpp AnimaTexturesManager.cpp AnimaDataGeneratorsManager.cpp AnimaMaterialsManager.cpp AnimaLightsManager.cpp AnimaScenesManager.cpp AnimaMeshesManager.h AnimaMeshesManager.cpp AnimaModelInstancesManager.h AnimaModelInstancesManager.cpp AnimaMeshInstancesManager.h AnimaMeshInstancesManager.cpp)

source_group(Engine\\DataStructures\\Arrays FILES AnimaArray.h AnimaMappedArray.h AnimaTypeMappedArray.h AnimaArray.cpp AnimaMappedArray.cpp AnimaTypeMappedArray.cpp)
source_group(Engine\\DataStructures\\MappedValues FILES AnimaMappedValues.h AnimaMappedValues.cpp)
source_group(Engine\\DataStructures\\Strings FILES AnimaString.h AnimaString.cpp)
source_group(Engine\\DataStructures\\Transformations FILES AnimaTransformation.h AnimaTransformation.cpp)
source_group("Engine\\DataStructures\\Graphics and Math" FILES AnimaVertex.h AnimaVertex.cpp AnimaFace.h AnimaFace.cpp AnimaMatrix.h AnimaMatrix.cpp AnimaMath.h AnimaMath.cpp AnimaFrustum.h AnimaFrustum.cpp AnimaGBuffer.h AnimaGBuffer.cpp AnimaQuaternion.h AnimaQuaternion.cpp)

source_group(Engine\\Scene FILES AnimaSceneObject.h AnimaSceneObject.cpp AnimaScene.h AnimaScene.cpp)
source_group(Engine\\Scene\\Rendering FILES AnimaRenderer.h AnimaRenderer.cpp)
source_group(Engine\\Scene\\Objects\\Cameras FILES AnimaCamera.h AnimaFirstPersonCamera.h AnimaThirdPersonCamera.h AnimaCamera.cpp AnimaFirstPersonCamera.cpp AnimaThirdPersonCamera.cpp)
source_group(Engine\\Scene\\Objects\\Graphics FILES AnimaMesh.h AnimaMesh.cpp AnimaMeshCreator.h AnimaMeshCreator.cpp AnimaModel.h AnimaModel.cpp AnimaModelInstance.h AnimaModelInstance.cpp AnimaMeshInstance.h AnimaMeshInstance.cpp)
source_group(Engine\\Scene\\Objects\\Lights FILES AnimaLight.h AnimaLight.cpp)
source_group(Engine\\Scene\\Data\\Shaders FILES AnimaShader.h AnimaShaderProgram.h AnimaShader.cpp AnimaShaderProgram.cpp)
source_group(Engine\\Scene\\Data\\Textures FILES AnimaTexture.h AnimaTexture.cpp)
source_group(Engine\\Scene\\Data\\Materials FILES AnimaMaterial.h AnimaMaterial.cpp)
source_group(Engine\\Scene\\Data\\DataGenerators FILES AnimaColorGenerator.h AnimaColorGenerator.cpp AnimaDataGenerator.h AnimaDataGenerator.cpp AnimaVectorGenerator.h AnimaVectorGenerator.cpp)

source_group(Engine\\Allocators FILES AnimaAllocator.h AnimaStackAllocator.h AnimaLinearAllocator.h AnimaFreeListAllocator.h AnimaPoolAllocator.h AnimaProxyAllocator.h AnimaAllocator.cpp AnimaStackAllocator.cpp AnimaLinearAllocator.cpp AnimaFreeListAllocator.cpp AnimaPoolAllocator.cpp AnimaProxyAllocator.cpp)

if (_ANIMA_ENGINE_COCOA)
    set(AnimaEngineCore_HEADERS	${common_HEADERS}
                                AnimaWindow_CocoaJoystick.h
                                AnimaWindow_CocoaPlatform.h
                                AnimaWindow_CocoaTLS.h)
    set(AnimaEngineCore_SOURCES	${common_SOURCES}
                                AnimaWindow_CocoaJoystick.mm
                                AnimaWindow_CocoaPlatform.mm
                                AnimaWindow_CocoaTLS.cpp)

	source_group("Engine\\Window System" FILES AnimaWindow_Data.h AnimaWindow.h AnimaWindow_CocoaJoystick.h AnimaWindow_CocoaPlatform.h AnimaWindow_CocoaTLS.h AnimaWindow_CocoaJoystick.mm AnimaWindow_CocoaPlatform.mm AnimaWindow_CocoaTLS.cpp)

elseif (_ANIMA_ENGINE_WIN32)
    set(AnimaEngineCore_HEADERS	${common_HEADERS} 
								AnimaWindow_WindowsJoystick.h 
								AnimaWindow_WindowsPlatform.h 
								AnimaWindow_WindowsTLS.h)
    set(AnimaEngineCore_SOURCES	${common_SOURCES} 
								AnimaWindow_WindowsJoystick.cpp 
								AnimaWindow_WindowsPlatform.cpp 
								AnimaWindow_WindowsTLS.cpp)

	source_group("Engine\\Window System" FILES AnimaWindow_Data.h AnimaWindow.h AnimaWindow_WindowsJoystick.h AnimaWindow_WindowsPlatform.h AnimaWindow_WindowsTLS.h AnimaWindow.cpp AnimaWindow_WindowsJoystick.cpp AnimaWindow_WindowsPlatform.cpp AnimaWindow_WindowsTLS.cpp)
endif()

if (_ANIMA_ENGINE_NSGL)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} AnimaWindow_NSGLContext.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} AnimaWindow_NSGLContext.mm)

	source_group("Engine\\Window System" FILES AnimaWindow_NSGLContext.h AnimaWindow_NSGLContext.mm)

elseif (_ANIMA_ENGINE_WGL)
    list(APPEND AnimaEngineCore_HEADERS ${common_HEADERS} AnimaWindow_WGLContext.h)
    list(APPEND AnimaEngineCore_SOURCES ${common_SOURCES} AnimaWindow_WGLContext.cpp)

	source_group("Engine\\Window System" FILES AnimaWindow_WGLContext.h AnimaWindow_WGLContext.cpp)
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

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	target_link_libraries(${PROJECT_NAME} ${ANIMA_ENGINE_NVTOOLSEXT_LIB})
endif()


if (WIN32)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/glew32.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/glew32.dll"
						COMMENT "Copia glew32.dll")
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimp.dll"
						COMMENT "Copia assimp.dll")
elseif(APPLE)						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/libGLEW.1.11.0.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libGLEW.1.11.0.dylib"
						COMMENT "Copia libGLEW.1.11.0.dylib")

	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/libGLEW.1.11.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libGLEW.1.11.dylib"
						COMMENT "Copia libGLEW.1.11.dylib")
						
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/libGLEW.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libGLEW.dylib"
						COMMENT "Copia libGLEW.dylib")

	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.1.1.dylib"
						COMMENT "Copia libassimp.3.1.1.dylib")

	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/llibassimp.3.dylib"
						COMMENT "Copia libassimp.3.dylib")

	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.dylib"
						COMMENT "Copia libassimp.dylib")
endif()

if (ANIMA_ENGINE_INSTALL)
	FILE(GLOB install_include "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	INSTALL(FILES ${install_include} DESTINATION AnimaEngine/include)

		INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION AnimaEngine/bin
										LIBRARY DESTINATION AnimaEngine/lib
										ARCHIVE DESTINATION AnimaEngine/lib)

	if(WIN32)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Win/x64/assimp.dll 	DESTINATION AnimaEngine/bin)
		if(ANIMA_ENGINE_USE_NVTOOLSEXT)
			INSTALL(FILES ${ANIMA_ENGINE_NVTOOLSEXT_BIN} DESTINATION AnimaEngine/bin)
		endif()
	else()		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.1.1.dylib 	DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.3.dylib 		DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Mac/x64/libassimp.dylib 			DESTINATION AnimaEngine/bin)
	endif()
endif()