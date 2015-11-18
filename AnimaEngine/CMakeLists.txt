project(AnimaEngine C CXX)

include_directories("${PROJECT_SOURCE_DIR}" ${AnimaEngine_INCLUDE_DIRS})

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	include_directories(${ANIMA_ENGINE_NVTOOLSEXT_INCLUDE})
	add_definitions(-DANIMA_ENGINE_USE_NVTOOLS)
endif()

add_definitions(-DANIMA_ENGINE_EXPORT_ENABLED)
add_definitions(-DGL_GLEXT_PROTOTYPES)
add_definitions(-D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0)

set(ANIMA_ENGINE_NODES_PATH "${PROJECT_SOURCE_DIR}/data/nodes/")
set(ANIMA_ENGINE_SHADERS_PATH "${PROJECT_SOURCE_DIR}/data/shaders/")
set(ANIMA_ENGINE_TEXTURES_PATH "${PROJECT_SOURCE_DIR}/data/textures/")

configure_file("${PROJECT_SOURCE_DIR}/AnimaEngineConfig.h.in" "${PROJECT_SOURCE_DIR}/AnimaEngineConfig.h")

set(common_HEADERS	AnimaEngineCore.h
					AnimaEngine.h
					AnimaAssert.h 
					AnimaTypes.h 
					AnimaAllocator.h 
					AnimaStackAllocator.h
                    AnimaLinearAllocator.h
                    AnimaFreeListAllocator.h
                    AnimaPoolAllocator.h
                    AnimaProxyAllocator.h
                    AnimaVertex.h
                    AnimaNodesManager.h
                    AnimaGeometry.h
                    AnimaFace.h
                    AnimaTimer.h
                    AnimaMaterial.h
                    AnimaString.h
					AnimaMatrix.h
					AnimaMath.h
					AnimaShadersManager.h
					AnimaShader.h
					AnimaShaderProgram.h
					AnimaQuaternion.h
					AnimaCamera.h
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
					AnimaGeometryCreator.h
					AnimaSceneObject.h
					AnimaArray.h
					AnimaFrustum.h
					AnimaMappedArray.h
					AnimaTypeMappedArray.h
					AnimaGeometriesManager.h
					AnimaNodeInstancesManager.h
					AnimaNode.h
					AnimaNodeInstance.h
					AnimaGeometryInstance.h
					AnimaGeometryInstancesManager.h
					AnimaAnimation.h
					AnimaAnimationsManager.h
					AnimaNamedObject.h
					AnimaGC.h
					AnimaXmlTranslators.h
					AnimaMD5.h
					AnimaTextureGenerator.h
					AnimaShaderData.h
					AnimaRendererDrawableGeometry.h
					AnimaRendererDrawableGeometryInstances.h
					AnimaShaderInclude.h
					AnimaShaderGroupData.h
					AnimaLogger.h
					AnimaRandom.h
					AnimaParallelProgram.h
					AnimaParallelProgramsManager.h
					AnimaTools.h
					AnimaPhysicsDebugDrawer.h
					IAnimaInteractor.h
					IAnimaEvents.h
					AnimaMouseInteractor.h
					AnimaKeyboardInteractor.h
					AnimaJoystickInteractor.h
					AnimaDefaultMouseInteractor.h)

set(common_SOURCES	AnimaEngineCore.cpp
					AnimaEngine.cpp
					AnimaAssert.cpp 
					AnimaStackAllocator.cpp
                    AnimaLinearAllocator.cpp
                    AnimaFreeListAllocator.cpp
                    AnimaPoolAllocator.cpp
                    AnimaProxyAllocator.cpp
					AnimaVertex.cpp
                    AnimaNodesManager.cpp
                    AnimaGeometry.cpp
                    AnimaFace.cpp
                    AnimaTimer.cpp
                    AnimaMaterial.cpp
                    AnimaString.cpp
					AnimaMatrix.cpp
					AnimaMath.cpp
					AnimaShadersManager.cpp
					AnimaShader.cpp
					AnimaShaderProgram.cpp
					AnimaQuaternion.cpp
					AnimaCamera.cpp
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
					AnimaGeometryCreator.cpp
					AnimaSceneObject.cpp
					AnimaArray.cpp
					AnimaFrustum.cpp
					AnimaMappedArray.cpp
					AnimaTypeMappedArray.cpp
					AnimaGeometriesManager.cpp
					AnimaNodeInstancesManager.cpp
					AnimaNode.cpp
					AnimaNodeInstance.cpp
					AnimaGeometryInstance.cpp
					AnimaGeometryInstancesManager.cpp
					AnimaAnimation.cpp
					AnimaAnimationsManager.cpp
					AnimaNamedObject.cpp
					AnimaXmlTranslators.cpp
					AnimaMD5.cpp
					AnimaTextureGenerator.cpp
					AnimaShaderData.cpp
					AnimaRendererDrawableGeometry.cpp
					AnimaRendererDrawableGeometryInstances.cpp
					AnimaShaderInclude.cpp
					AnimaShaderGroupData.cpp
					AnimaLogger.cpp
					AnimaRandom.cpp
					AnimaParallelProgram.cpp
					AnimaParallelProgramsManager.cpp
					AnimaTools.cpp
					AnimaPhysicsDebugDrawer.cpp
					IAnimaInteractor.cpp
					IAnimaEvents.cpp
					AnimaDefaultMouseInteractor.cpp)

if(ANIMA_ENGINE_EXPORT_TO_PYTHON)
	set(AnimaEngineCore_HEADERS	${common_HEADERS} AnimaEnginePY.h)

	if(APPLE)
		set(AnimaEngineCore_SOURCES	${common_SOURCES} AnimaEnginePY.cpp AnimaGC.mm AnimaMouseInteractor.mm AnimaKeyboardInteractor.mm AnimaJoystickInteractor.mm)
	else()
		set(AnimaEngineCore_SOURCES	${common_SOURCES} AnimaEnginePY.cpp AnimaGC.cpp AnimaMouseInteractor.cpp AnimaKeyboardInteractor.cpp AnimaJoystickInteractor.cpp)
	endif()
	
	source_group(Engine FILES AnimaEngine.h AnimaEngine.cpp AnimaEngineConfig.h AnimaEngineCore.h AnimaEngineCore.cpp AnimaEnginePY.cpp AnimaEnginePY.h)
else()
	set(AnimaEngineCore_HEADERS	${common_HEADERS})

	if(APPLE)
		set(AnimaEngineCore_SOURCES	${common_SOURCES} AnimaGC.mm AnimaMouseInteractor.mm AnimaKeyboardInteractor.mm AnimaJoystickInteractor.mm)
	else()
		set(AnimaEngineCore_SOURCES	${common_SOURCES} AnimaGC.cpp AnimaMouseInteractor.cpp AnimaKeyboardInteractor.cpp AnimaJoystickInteractor.cpp)
	endif()

	source_group(Engine FILES AnimaEngine.h AnimaEngine.cpp AnimaEngineConfig.h AnimaEngineCore.h AnimaEngineCore.cpp AnimaEnginePY.cpp AnimaEnginePY.h)
endif()

if(APPLE)
	source_group("Engine\\Graphic Context" FILES AnimaGC.h AnimaGC.mm)
	source_group(Engine\\Interactors FILES AnimaMouseInteractor.mm AnimaKeyboardInteractor.mm AnimaJoystickInteractor.mm)
else()
	source_group("Engine\\Graphic Context" FILES AnimaGC.h AnimaGC.cpp)
	source_group(Engine\\Interactors FILES AnimaMouseInteractor.cpp AnimaKeyboardInteractor.cpp AnimaJoystickInteractor.cpp)
endif()

source_group(Engine\\Tools FILES AnimaAssert.h AnimaAssert.cpp AnimaTimer.h AnimaTimer.cpp AnimaFPSTimer.h AnimaFPSTimer.cpp AnimaXmlTranslators.h AnimaXmlTranslators.cpp AnimaMD5.h AnimaMD5.cpp AnimaLogger.cpp AnimaLogger.h AnimaRandom.cpp AnimaRandom.h AnimaTools.h AnimaTools.cpp)
source_group(Engine\\Types FILES AnimaTypes.h)

source_group(Engine\\Managers FILES AnimaNodesManager.h AnimaShadersManager.h AnimaCamerasManager.h AnimaTexturesManager.h AnimaDataGeneratorsManager.h AnimaMaterialsManager.h AnimaLightsManager.h AnimaScenesManager.h AnimaNodesManager.cpp AnimaShadersManager.cpp AnimaCamerasManager.cpp AnimaTexturesManager.cpp AnimaDataGeneratorsManager.cpp AnimaMaterialsManager.cpp AnimaLightsManager.cpp AnimaScenesManager.cpp AnimaGeometriesManager.h AnimaGeometriesManager.cpp AnimaNodeInstancesManager.h AnimaNodeInstancesManager.cpp AnimaGeometryInstancesManager.h AnimaGeometryInstancesManager.cpp AnimaAnimationsManager.cpp AnimaAnimationsManager.h AnimaParallelProgramsManager.h AnimaParallelProgramsManager.cpp)

source_group(Engine\\DataStructures\\Arrays FILES AnimaArray.h AnimaMappedArray.h AnimaTypeMappedArray.h AnimaArray.cpp AnimaMappedArray.cpp AnimaTypeMappedArray.cpp)
source_group(Engine\\DataStructures\\MappedValues FILES AnimaMappedValues.h AnimaMappedValues.cpp)
source_group(Engine\\DataStructures\\Objects FILES AnimaNamedObject.h AnimaNamedObject.cpp)
source_group(Engine\\DataStructures\\Strings FILES AnimaString.h AnimaString.cpp)
source_group(Engine\\DataStructures\\Transformations FILES AnimaTransformation.h AnimaTransformation.cpp)
source_group("Engine\\DataStructures\\Graphics and Math" FILES AnimaVertex.h AnimaVertex.cpp AnimaFace.h AnimaFace.cpp AnimaMatrix.h AnimaMatrix.cpp AnimaMath.h AnimaMath.cpp AnimaFrustum.h AnimaFrustum.cpp AnimaGBuffer.h AnimaGBuffer.cpp AnimaQuaternion.h AnimaQuaternion.cpp)

source_group(Engine\\Interfaces FILES IAnimaInteractor.h IAnimaInteractor.cpp IAnimaEvents.h IAnimaEvents.cpp)

source_group(Engine\\Interactors FILES AnimaMouseInteractor.h AnimaKeyboardInteractor.h AnimaDefaultMouseInteractor.h AnimaDefaultMouseInteractor.cpp AnimaJoystickInteractor.h)

source_group(Engine\\Scene FILES AnimaSceneObject.h AnimaSceneObject.cpp AnimaScene.h AnimaScene.cpp)
source_group(Engine\\Scene\\Rendering FILES AnimaRenderer.h AnimaRenderer.cpp AnimaRendererDrawableGeometry.h AnimaRendererDrawableGeometryInstances.h AnimaRendererDrawableGeometry.cpp AnimaRendererDrawableGeometryInstances.cpp)
source_group(Engine\\Scene\\Data\\Animations FILES AnimaAnimation.cpp AnimaAnimation.h)
source_group(Engine\\Scene\\Data\\Objects\\Cameras FILES AnimaCamera.h AnimaCamera.cpp)
source_group(Engine\\Scene\\Data\\Objects\\Graphics FILES AnimaGeometry.h AnimaGeometry.cpp AnimaGeometryCreator.h AnimaGeometryCreator.cpp AnimaNode.h AnimaNode.cpp AnimaNodeInstance.h AnimaNodeInstance.cpp AnimaGeometryInstance.h AnimaGeometryInstance.cpp)
source_group(Engine\\Scene\\Data\\Objects\\Lights FILES AnimaLight.h AnimaLight.cpp)
source_group(Engine\\Scene\\Data\\Shaders FILES AnimaShader.h AnimaShaderProgram.h AnimaShader.cpp AnimaShaderProgram.cpp AnimaShaderData.h AnimaShaderData.cpp AnimaShaderInclude.h AnimaShaderInclude.cpp AnimaShaderGroupData.h AnimaShaderGroupData.cpp)
source_group(Engine\\Scene\\Data\\Textures FILES AnimaTexture.h AnimaTexture.cpp)
source_group(Engine\\Scene\\Data\\Materials FILES AnimaMaterial.h AnimaMaterial.cpp)
source_group(Engine\\Scene\\Physics\\Debug FILES AnimaPhysicsDebugDrawer.h AnimaPhysicsDebugDrawer.cpp)
source_group(Engine\\Scene\\Data\\DataGenerators FILES AnimaColorGenerator.h AnimaColorGenerator.cpp AnimaDataGenerator.h AnimaDataGenerator.cpp AnimaVectorGenerator.h AnimaVectorGenerator.cpp AnimaTextureGenerator.h AnimaTextureGenerator.cpp)
source_group("Engine\\Parallel Programming" FILES AnimaParallelProgram.cpp AnimaParallelProgram.h)

source_group(Engine\\Allocators FILES AnimaAllocator.h AnimaStackAllocator.h AnimaLinearAllocator.h AnimaFreeListAllocator.h AnimaPoolAllocator.h AnimaProxyAllocator.h AnimaAllocator.cpp AnimaStackAllocator.cpp AnimaLinearAllocator.cpp AnimaFreeListAllocator.cpp AnimaPoolAllocator.cpp AnimaProxyAllocator.cpp)

if (APPLE)
    # For some reason, CMake doesn't know about .m
    set(CMAKE_CXX_FLAGS "-x objective-c++")
    set_source_files_properties(${AnimaEngineCore_SOURCES} PROPERTIES LANGUAGE CXX)
    set_source_files_properties(${AnimaEngineCore_HEADERS} PROPERTIES LANGUAGE CXX)
else()
	set(CMAKE_CXX_FLAGS "/wd4251 /wd4996 /EHsc")
endif()

if(ANIMA_ENGINE_EXPORT_TO_PYTHON)
	add_definitions(-DANIMA_ENGINE_EXPORT_TO_PYTHON)
	
	if(APPLE)
		set(PYTHON_LIBRARY "${ANIMA_ENGINE_PYTHON_ROOT_PATH}/Python")
		set(PYTHON_INCLUDE_DIR "${ANIMA_ENGINE_PYTHON_ROOT_PATH}/include/${ANIMA_ENGINE_PYTHON_INC_DIR_NAME}")
	else()
		set(PYTHON_LIBRARY "${ANIMA_ENGINE_PYTHON_ROOT_PATH}/libs/${ANIMA_ENGINE_PYTHON_LIB_NAME}")
		set(PYTHON_INCLUDE_DIR "${ANIMA_ENGINE_PYTHON_ROOT_PATH}/include")
	endif()
	
	find_package(PythonLibs)
	include_directories(${PYTHON_INCLUDE_PATH})
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
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_ASSIMP})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_GLEW})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_OPENCL})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_DEP_BULLET})
target_link_libraries(${PROJECT_NAME} ${AnimaEngine_BOOST_LIBS})

if(ANIMA_ENGINE_EXPORT_TO_PYTHON)
	add_definitions(-DBOOST_PYTHON_STATIC_LIB)
	target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARY})
endif()

if(ANIMA_ENGINE_USE_NVTOOLSEXT)
	target_link_libraries(${PROJECT_NAME} ${ANIMA_ENGINE_NVTOOLSEXT_LIB})
endif()

if (WIN32)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/glew32.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/glew32.dll"
						COMMENT "Copia glew32.dll")
	
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/assimp.dll "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/assimp.dll"
						COMMENT "Copia assimp.dll")
						
	if(ANIMA_ENGINE_EXPORT_TO_PYTHON)
		add_custom_command(	TARGET "${PROJECT_NAME}" POST_BUILD
							COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_BINARY_DIR}/Build/bin/$<CONFIGURATION>/AnimaEngine.dll "${CMAKE_SOURCE_DIR}/AnimaEditor_PY/AnimaEngine/AnimaEngine.pyd"
							COMMENT "Copia AnimaEngine.pyd")
	endif()
elseif(APPLE)
	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/libassimp.3.1.1.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.3.1.1.dylib"
						COMMENT "Copia libassimp.3.1.1.dylib")

	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/libassimp.3.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/llibassimp.3.dylib"
						COMMENT "Copia libassimp.3.dylib")

	add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
						COMMAND ${CMAKE_COMMAND} ARGS -E copy ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/libassimp.dylib "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/libassimp.dylib"
						COMMENT "Copia libassimp.dylib")

	if(ANIMA_ENGINE_EXPORT_TO_PYTHON)
		add_custom_command(	TARGET "${PROJECT_NAME}" POST_BUILD
							COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_BINARY_DIR}/Build/lib/$<CONFIGURATION>/libAnimaEngine.${ANIMA_ENGINE_VERSION}.dylib "${CMAKE_SOURCE_DIR}/AnimaEditor_PY/AnimaEngine/AnimaEngine.so"
							COMMENT "Copia AnimaEngine.so")
	endif()
endif()

if (ANIMA_ENGINE_INSTALL)
	FILE(GLOB install_include "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
	INSTALL(FILES ${install_include} DESTINATION AnimaEngine/include)
	
	FILE(GLOB install_ogl_include "${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/GL/*.h")
	INSTALL(FILES ${install_ogl_include} DESTINATION AnimaEngine/include/GL)
		
	INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION AnimaEngine/bin
									LIBRARY DESTINATION AnimaEngine/lib
									ARCHIVE DESTINATION AnimaEngine/lib)
		
	if(WIN32)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/assimp.dll 	DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/glew32.dll 	DESTINATION AnimaEngine/bin)
		if(ANIMA_ENGINE_USE_NVTOOLSEXT)
			INSTALL(FILES ${ANIMA_ENGINE_NVTOOLSEXT_BIN} DESTINATION AnimaEngine/bin)
		endif()
	else()		
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/libassimp.3.1.1.dylib 	DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/libassimp.3.dylib 		DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/Assimp/Lib/libassimp.dylib 			DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/libGLEW.1.12.0.dylib 	DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/libGLEW.1.12.dylib 		DESTINATION AnimaEngine/bin)
		INSTALL(FILES ${PROJECT_SOURCE_DIR}/../Dependencies/OpenGL/Lib/libGLEW.dylib 			DESTINATION AnimaEngine/bin)
	endif()
endif()