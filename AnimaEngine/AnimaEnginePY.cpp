#include "AnimaEnginePY.h"

#if defined ANIMA_ENGINE_EXPORT_TO_PYTHON

#include "AnimaTypes.h"
#include "AnimaString.h"
#include "AnimaVertex.h"
#include "AnimaGC.h"
#include "AnimaEngine.h"
#include "AnimaAllocators.h"
#include "AnimaShadersManager.h"
#include "AnimaScenesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaNodesManager.h"
#include "AnimaLightsManager.h"
#include "AnimaGeometriesManager.h"
#include "AnimaNodeInstancesManager.h"
#include "AnimaGeometryInstancesManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaAnimationsManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaRenderer.h"
#include "AnimaTimer.h"

#include "AnimaScene.h"
#include "AnimaNode.h"
#include "AnimaAnimation.h"
#include "AnimaGeometry.h"
#include "AnimaNodeInstance.h"
#include "AnimaGeometryInstance.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"
#include "AnimaCamera.h"
#include "AnimaLight.h"
#include "AnimaFirstPersonCamera.h"
#include "AnimaThirdPersonCamera.h"
#include "AnimaCamera.h"
#include "AnimaAnimation.h"

#include <boost/python.hpp>
using namespace boost::python;

struct AnimaString_to_python_str
{
	static PyObject* convert(Anima::AnimaString const& str)
	{
		return incref(object(str.c_str()).ptr());
	}
};

struct AnimaString_from_python_str
{
	AnimaString_from_python_str()
	{
		converter::registry::push_back(&convertible, &construct, type_id<Anima::AnimaString>());
	}

	static void* convertible(PyObject* obj_ptr)
	{
		if (!PyString_Check(obj_ptr))
			return NULL;
		return obj_ptr;
	}

	static void construct(PyObject* obj_ptr, converter::rvalue_from_python_stage1_data* data)
	{
		const char* value = PyString_AsString(obj_ptr);
		ANIMA_ASSERT(value);

		void* storage = ((converter::rvalue_from_python_storage<Anima::AnimaString>*)data)->storage.bytes;

		new (storage)Anima::AnimaString(value);

		data->convertible = storage;
	}
};

Anima::AnimaScene* (Anima::AnimaScenesManager::*CreateSceneString)(const Anima::AnimaString&) = &Anima::AnimaScenesManager::CreateScene;
Anima::AnimaScene* (Anima::AnimaScenesManager::*GetSceneInt)(Anima::AUint) = &Anima::AnimaScenesManager::GetScene;
Anima::AnimaScene* (Anima::AnimaScenesManager::*GetSceneString)(const Anima::AnimaString&) = &Anima::AnimaScenesManager::GetSceneFromName;

Anima::AnimaNode* (Anima::AnimaNodesManager::*LoadNodeFromExternalFileStringString)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaNodesManager::LoadNodeFromExternalFile;
Anima::AnimaNode* (Anima::AnimaNodesManager::*CreateNodeString)(const Anima::AnimaString&, bool) = &Anima::AnimaNodesManager::CreateNode;
Anima::AnimaNode* (Anima::AnimaNodesManager::*GetNodeFromNameString)(const Anima::AnimaString&, bool) = &Anima::AnimaNodesManager::GetNodeFromName;

Anima::AnimaAnimation* (Anima::AnimaAnimationsManager::*GetAnimationFromNameString)(const Anima::AnimaString&) = &Anima::AnimaAnimationsManager::GetAnimationFromName;

Anima::AnimaGeometry* (Anima::AnimaNode::*GetGeometryFromNameString)(const Anima::AnimaString&) = &Anima::AnimaNode::GetGeometryFromName;
void (Anima::AnimaNode::*SetOriginFileNameString)(const Anima::AnimaString&) = &Anima::AnimaNode::SetOriginFileName;
void (Anima::AnimaNode::*SetAnimationNodeNameString)(const Anima::AnimaString&) = &Anima::AnimaNode::SetAnimationNodeName;

void (Anima::AnimaRenderer::*RendererStart)(Anima::AnimaScene*) = &Anima::AnimaRenderer::Start;

Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*CreateShaderString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateShader;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*CreateProgramString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateProgram;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*GetProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::GetProgramFromName;
void (Anima::AnimaShadersManager::*SetActiveProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::SetActiveProgramFromName;

Anima::AnimaCamera* (Anima::AnimaCamerasManager::*CreateCameraString)(const Anima::AnimaString&) = &Anima::AnimaCamerasManager::CreateCamera;
Anima::AnimaCamera* (Anima::AnimaCamerasManager::*GetCameraFromNameString)(const Anima::AnimaString&) = &Anima::AnimaCamerasManager::GetCameraFromName;

BOOST_PYTHON_MODULE(AnimaEngine)
{
	// Inizializzazione dei converters
	to_python_converter<Anima::AnimaString, AnimaString_to_python_str>();
	AnimaString_from_python_str();

	// AnimaEngine
	class_<Anima::AnimaEngine>("AnimaEngine")
		.def("Initialize", &Anima::AnimaEngine::Initialize)
		.def("Terminate", &Anima::AnimaEngine::Terminate)
		.def("GetNodesAllocator", &Anima::AnimaEngine::GetNodesAllocator, return_value_policy<reference_existing_object>())
		.def("GetGenericAllocator", &Anima::AnimaEngine::GetGenericAllocator, return_value_policy<reference_existing_object>())
		.def("GetGeometriesAllocator", &Anima::AnimaEngine::GetGeometriesAllocator, return_value_policy<reference_existing_object>())
		.def("GetNodeInstancesAllocator", &Anima::AnimaEngine::GetNodeInstancesAllocator, return_value_policy<reference_existing_object>())
		.def("GetGeometryInstancesAllocator", &Anima::AnimaEngine::GetGeometryInstancesAllocator, return_value_policy<reference_existing_object>())
		//	Deprecato	.def("GetStringAllocator", &Anima::AnimaEngine::GetStringAllocator, return_value_policy<reference_existing_object>())
		.def("GetShadersAllocator", &Anima::AnimaEngine::GetShadersAllocator, return_value_policy<reference_existing_object>())
		.def("GetCamerasAllocator", &Anima::AnimaEngine::GetCamerasAllocator, return_value_policy<reference_existing_object>())
		.def("GetTexturesAllocator", &Anima::AnimaEngine::GetTexturesAllocator, return_value_policy<reference_existing_object>())
		.def("GetMaterialsAllocator", &Anima::AnimaEngine::GetMaterialsAllocator, return_value_policy<reference_existing_object>())
		.def("GetLightsAllocator", &Anima::AnimaEngine::GetLightsAllocator, return_value_policy<reference_existing_object>())
		.def("GetDataGeneratorsAllocator", &Anima::AnimaEngine::GetDataGeneratorsAllocator, return_value_policy<reference_existing_object>())
		.def("GetScenesAllocator", &Anima::AnimaEngine::GetScenesAllocator, return_value_policy<reference_existing_object>())
		.def("GetAnimationsAllocator", &Anima::AnimaEngine::GetAnimationsAllocator, return_value_policy<reference_existing_object>())
		.def("GetManagersAllocator", &Anima::AnimaEngine::GetManagersAllocator, return_value_policy<reference_existing_object>())
		.def("GetAnimationsAllocator", &Anima::AnimaEngine::GetAnimationsAllocator, return_value_policy<reference_existing_object>())
		.def("GetShadersManager", &Anima::AnimaEngine::GetShadersManager, return_value_policy<reference_existing_object>())
		.def("GetScenesManager", &Anima::AnimaEngine::GetScenesManager, return_value_policy<reference_existing_object>())
		.def("GetDataGeneratorsManager", &Anima::AnimaEngine::GetDataGeneratorsManager, return_value_policy<reference_existing_object>());
	
	// AnimaScenesManager
	class_<Anima::AnimaScenesManager>("AnimaScenesManager", no_init)
		.def("CreateScene", CreateSceneString, return_value_policy<reference_existing_object>())
		.def("GetScene", GetSceneInt, return_value_policy<reference_existing_object>())
		.def("GetScene", GetSceneString, return_value_policy<reference_existing_object>());
	
	// AnimaScene
	class_<Anima::AnimaScene>("AnimaScene", no_init)
		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
		.def("GetNodesManager", &Anima::AnimaScene::GetNodesManager, return_value_policy<reference_existing_object>())
		.def("GetGeometriesManager", &Anima::AnimaScene::GetGeometriesManager, return_value_policy<reference_existing_object>())
		.def("GetNodeInstancesManager", &Anima::AnimaScene::GetNodeInstancesManager, return_value_policy<reference_existing_object>())
		.def("GetGeometryInstancesManager", &Anima::AnimaScene::GetGeometryInstancesManager, return_value_policy<reference_existing_object>())
		.def("GetShadersManager", &Anima::AnimaScene::GetShadersManager, return_value_policy<reference_existing_object>())
		.def("GetCamerasManager", &Anima::AnimaScene::GetCamerasManager, return_value_policy<reference_existing_object>())
		.def("GetTexturesManager", &Anima::AnimaScene::GetTexturesManager, return_value_policy<reference_existing_object>())
		.def("GetDataGeneratorsManager", &Anima::AnimaScene::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
		.def("GetMaterialsManager", &Anima::AnimaScene::GetMaterialsManager, return_value_policy<reference_existing_object>())
		.def("GetLightsManager", &Anima::AnimaScene::GetLightsManager, return_value_policy<reference_existing_object>())
		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
		.def("GetAnimationsManager", &Anima::AnimaScene::GetAnimationsManager, return_value_policy<reference_existing_object>())
		.def("GetGeometriesAllocator", &Anima::AnimaScene::GetGeometriesAllocator, return_value_policy<reference_existing_object>())
		.def("GetNodeInstancesAllocator", &Anima::AnimaScene::GetNodeInstancesAllocator, return_value_policy<reference_existing_object>())
		.def("GetGeometryInstancesAllocator", &Anima::AnimaScene::GetGeometryInstancesAllocator, return_value_policy<reference_existing_object>())
		.def("GetShadersAllocator", &Anima::AnimaScene::GetShadersAllocator, return_value_policy<reference_existing_object>())
		.def("GetCamerasAllocator", &Anima::AnimaScene::GetCamerasAllocator, return_value_policy<reference_existing_object>())
		.def("GetTexturesAllocator", &Anima::AnimaScene::GetTexturesAllocator, return_value_policy<reference_existing_object>())
		.def("GetDataGeneratorsAllocator", &Anima::AnimaScene::GetDataGeneratorsAllocator, return_value_policy<reference_existing_object>())
		.def("GetMaterialsAllocator", &Anima::AnimaScene::GetMaterialsAllocator, return_value_policy<reference_existing_object>())
		.def("GetLightsAllocator", &Anima::AnimaScene::GetLightsAllocator, return_value_policy<reference_existing_object>())
		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
		.def("GetAnimationsAllocator", &Anima::AnimaScene::GetAnimationsAllocator, return_value_policy<reference_existing_object>());

	// AnimaAnimationsManager
	class_<Anima::AnimaAnimationsManager>("AnimaAnimationsManager", no_init)
		.def("LoadAnimations", &Anima::AnimaAnimationsManager::LoadAnimations)
		.def("GetAnimationsCount", &Anima::AnimaAnimationsManager::GetAnimationsCount)
		.def("GetAnimation", &Anima::AnimaAnimationsManager::GetAnimation, return_value_policy<reference_existing_object>())
		.def("GetAnimationFromName", GetAnimationFromNameString, return_value_policy<reference_existing_object>())
		.def("GetLastLoadedAnimations", &Anima::AnimaAnimationsManager::GetLastLoadedAnimations, return_value_policy<reference_existing_object>())
		.def("ClearAnimations", &Anima::AnimaAnimationsManager::ClearAnimations)
		.def("ClearLastLoadedAnimations", &Anima::AnimaAnimationsManager::ClearLastLoadedAnimations);

	// AnimaAnimation
	class_<Anima::AnimaAnimation>("AnimaAnimation", no_init)
		.def("UpdateAnimation", &Anima::AnimaAnimation::UpdateAnimation);
	
	// AnimaNodesManager
	class_<Anima::AnimaNodesManager>("AnimaNodesManager", no_init)
		.def("LoadNodesFromExternalFile", LoadNodeFromExternalFileStringString, return_value_policy<reference_existing_object>())
		.def("CreateNode", CreateNodeString, return_value_policy<reference_existing_object>())
		.def("GetNodeFromName", GetNodeFromNameString, return_value_policy<reference_existing_object>())
		.def("GetNode", &Anima::AnimaNodesManager::GetNode, return_value_policy<reference_existing_object>())
		.def("ClearNodes", &Anima::AnimaNodesManager::ClearNodes);

	// AnimaNode
	class_<Anima::AnimaNode>("AnimaNode", init<const Anima::AnimaString&, Anima::AnimaDataGeneratorsManager*, Anima::AnimaAllocator*>())
		.def("SetMaterial", &Anima::AnimaNode::SetMaterial)
		.def("GetMaterial", &Anima::AnimaNode::GetMaterial, return_value_policy<reference_existing_object>())
		.def("GetGeometriesCount", &Anima::AnimaNode::GetGeometriesCount)
		.def("AddGeometry", &Anima::AnimaNode::AddGeometry)
		.def("GetGeometry", &Anima::AnimaNode::GetGeometry, return_value_policy<reference_existing_object>())
		.def("GetGeometryFromName", GetGeometryFromNameString, return_value_policy<reference_existing_object>())
		.def("GetAnimationsCount", &Anima::AnimaNode::GetAnimationsCount)
		.def("SetAnimations", &Anima::AnimaNode::SetAnimations)
		.def("AddAnimation", &Anima::AnimaNode::AddAnimation)
		.def("GetAnimation", &Anima::AnimaNode::GetAnimation, return_value_policy<reference_existing_object>())
		.def("SetOriginFileName", SetOriginFileNameString)
		.def("GetAnimaOriginFileName", &Anima::AnimaNode::GetAnimaOriginFileName)
		.def("SetAnimationNodeName", SetAnimationNodeNameString)
		.def("GetAnimaAnimationNodeName", &Anima::AnimaNode::GetAnimaAnimationNodeName)
		.def("GetGeometriesBonesInfo", &Anima::AnimaNode::GetGeometriesBonesInfo, return_value_policy<reference_existing_object>())
		.def("SetGeometriesBonesInfo", &Anima::AnimaNode::SetGeometriesBonesInfo)
		.def("ClearGeometriesBonesInfo", &Anima::AnimaNode::ClearGeometriesBonesInfo)
		.def("UpdateAnimation", &Anima::AnimaNode::UpdateAnimation)
		.def("SetActiveAnimation", &Anima::AnimaNode::SetActiveAnimation)
		.def("StopAnimation", &Anima::AnimaNode::StopAnimation);

	// AnimaRenderer
	class_<Anima::AnimaRenderer, boost::noncopyable>("AnimaRenderer", init<Anima::AnimaEngine*, Anima::AnimaAllocator*>())
		.def("Start", RendererStart)
		.def("DrawAll", &Anima::AnimaRenderer::Render)
		//.def("AddPrimitive", &Anima::AnimaRenderer::AddPrimitive)
		.def("InitTextureSlots", &Anima::AnimaRenderer::InitTextureSlots)
		.def("InitRenderingTargets", &Anima::AnimaRenderer::InitRenderingTargets)
		.def("InitRenderingUtilities", &Anima::AnimaRenderer::InitRenderingUtilities);

	// AnimaShadersManager
	class_<Anima::AnimaShadersManager>("AnimaShadersManager", no_init)
		.def("LoadShader", LoadShaderStringString, return_value_policy<reference_existing_object>())
		.def("LoadShaderFromFile", LoadShaderFromFileStringString, return_value_policy<reference_existing_object>())
		.def("CreateShader", CreateShaderString, return_value_policy<reference_existing_object>())
		.def("CreateProgram", CreateProgramString, return_value_policy<reference_existing_object>())
		.def("GetProgram", &Anima::AnimaShadersManager::GetProgram, return_value_policy<reference_existing_object>())
		.def("GetProgramFromName", GetProgramFromNameString, return_value_policy<reference_existing_object>())
		.def("SetActiveProgram", &Anima::AnimaShadersManager::SetActiveProgram, return_value_policy<reference_existing_object>())
		.def("SetActiveProgramFromName", SetActiveProgramFromNameString, return_value_policy<reference_existing_object>())
		.def("GetActiveProgram", &Anima::AnimaShadersManager::GetActiveProgram, return_value_policy<reference_existing_object>());

	// AnimaShader
	enum_<Anima::AnimaShaderType>("AnimaShaderType")
		.value("INVALID", Anima::INVALID)
		.value("VERTEX", Anima::VERTEX)
		.value("TESSELLATION_CONTROL", Anima::TESSELLATION_CONTROL)
		.value("TESSELLATION_EVALUATION", Anima::TESSELLATION_EVALUATION)
		.value("GEOMETRY", Anima::GEOMETRY)
		.value("FRAGMENT", Anima::FRAGMENT);

	class_<Anima::AnimaShader>("AnimaShader", no_init);

	// AnimaShaderProgram
	enum_<Anima::AnimaShaderInfoType>("AnimaShaderInfoType")
		.value("SHADER_FILE", Anima::SHADER_FILE)
		.value("SHADER_TEXT", Anima::SHADER_TEXT);

	class_<Anima::AnimaShaderProgram>("AnimaShaderProgram", no_init)
		.def("AddShader", &Anima::AnimaShaderProgram::AddShader)
		.def("SetShaders", &Anima::AnimaShaderProgram::SetShaders)
		.def("CompileShaders", &Anima::AnimaShaderProgram::CompileShaders)
		.def("ClearShaders", &Anima::AnimaShaderProgram::ClearShaders)
		.def("Create", &Anima::AnimaShaderProgram::Create)
		.def("Link", &Anima::AnimaShaderProgram::Link)
		.def("Use", &Anima::AnimaShaderProgram::Use)
		.def("Delete", &Anima::AnimaShaderProgram::Delete)
		.def("IsCreated", &Anima::AnimaShaderProgram::IsCreated)
		.def("IsLinked", &Anima::AnimaShaderProgram::IsLinked)
		.def("GetID", &Anima::AnimaShaderProgram::GetID);

	// AnimaCamerasManager
	class_<Anima::AnimaCamerasManager>("AnimaCamerasManager", no_init)
		.def("CreateCamera", CreateCameraString, return_value_policy<reference_existing_object>())
		.def("GetActiveCamera", &Anima::AnimaCamerasManager::GetActiveCamera, return_value_policy<reference_existing_object>())
		.def("GetCameraFromName", GetCameraFromNameString, return_value_policy<reference_existing_object>())
		.def("GetCamerasCount", &Anima::AnimaCamerasManager::GetCamerasCount)
		.def("UpdatePerspectiveCameras", &Anima::AnimaCamerasManager::UpdatePerspectiveCameras)
		.def("UpdateOrthoCameras", &Anima::AnimaCamerasManager::UpdateOrthoCameras);

	//// AnimaCamera
	//class_<Anima::AnimaCamera, boost::noncopyable>("AnimaCamera", no_init)
	//	.def("GetPosition", &Anima::AnimaCamera::GetPosition)
	//	.def("SetPosition", AnimaCameraSetPositionVector)
	//	.def("SetPosition", AnimaCameraSetPositionFloat)
	//	.def("IsActive", &Anima::AnimaCamera::IsActive)
	//	.def("Zoom", pure_virtual(&Anima::AnimaCamera::Zoom))
	//	.def("Move", pure_virtual(AnimaCameraMoveVector))
	//	.def("Move", pure_virtual(AnimaCameraMoveFloat))
	//	.def("LookAt", pure_virtual(AnimaCameraLookAtVector))
	//	.def("LookAt", pure_virtual(AnimaCameraLookAtFloat))
	//	.def("GetViewMatrix", &Anima::AnimaCamera::GetViewMatrix)
	//	.def("CalculateProjectionMatrix", AnimaCameraCalculatePerspectiveProjectionMatrix)
	//	.def("CalculateProjectionMatrix", AnimaCameraCalculateOrthographicProjectionMatrix)
	//	.def("GetProjectionMatrix", &Anima::AnimaCamera::GetProjectionMatrix)
	//	.def("GetProjectionViewMatrix", &Anima::AnimaCamera::GetProjectionViewMatrix)
	//	.def("GetInversedProjectionViewMatrix", &Anima::AnimaCamera::GetInversedProjectionViewMatrix)
	//	.def("Activate", &Anima::AnimaCamera::Activate)
	//	.def("Deactivate", &Anima::AnimaCamera::Deactivate)
	//	.def("IsPerspectiveProjectionType", &Anima::AnimaCamera::IsPerspectiveProjectionType)
	//	.def("IsOrthoProjectionType", &Anima::AnimaCamera::IsOrthographicProjectionType)
	//	.def("GetFrustum", &Anima::AnimaCamera::GetFrustum, return_value_policy<reference_existing_object>());

	// AnimaGC
	class_<Anima::AnimaGC>("AnimaGC", no_init)
		.def("CreateContext", &Anima::AnimaGC::CreateContext, return_value_policy<manage_new_object>())
		.def("DestroyContext", &Anima::AnimaGC::DestroyContext)
		.def("GetDefaultContextConfig", &Anima::AnimaGC::GetDefaultContextConfig)
		.def("GetDefaultFrameBufferConfig", &Anima::AnimaGC::GetDefaultFrameBufferConfig)
		.def("InitializeGLEWExtensions", &Anima::AnimaGC::InitializeGLEWExtensions)
		.def("MakeCurrent", &Anima::AnimaGC::MakeCurrent)
		.def("SwapBuffers", &Anima::AnimaGC::SwapBuffers)
		.def("ClearColor", &Anima::AnimaGC::ClearColor)
		.staticmethod("CreateContext")
		.staticmethod("DestroyContext")
		.staticmethod("GetDefaultContextConfig")
		.staticmethod("GetDefaultFrameBufferConfig")
		.staticmethod("InitializeGLEWExtensions");

	class_<Anima::AnimaGCFrameBufferConfig>("AnimaGCFrameBufferConfig")
		.def_readwrite("redBits", &Anima::AnimaGCFrameBufferConfig::_redBits)
		.def_readwrite("greenBits", &Anima::AnimaGCFrameBufferConfig::_greenBits)
		.def_readwrite("blueBits", &Anima::AnimaGCFrameBufferConfig::_blueBits)
		.def_readwrite("alphaBits", &Anima::AnimaGCFrameBufferConfig::_alphaBits)
		.def_readwrite("depthBits", &Anima::AnimaGCFrameBufferConfig::_depthBits)
		.def_readwrite("stencilBits", &Anima::AnimaGCFrameBufferConfig::_stencilBits)
		.def_readwrite("accumRedBits", &Anima::AnimaGCFrameBufferConfig::_accumRedBits)
		.def_readwrite("accumGreenBits", &Anima::AnimaGCFrameBufferConfig::_accumGreenBits)
		.def_readwrite("accumBlueBits", &Anima::AnimaGCFrameBufferConfig::_accumBlueBits)
		.def_readwrite("accumAlphaBits", &Anima::AnimaGCFrameBufferConfig::_accumAlphaBits)
		.def_readwrite("auxBuffers", &Anima::AnimaGCFrameBufferConfig::_auxBuffers)
		.def_readwrite("stereo", &Anima::AnimaGCFrameBufferConfig::_stereo)
		.def_readwrite("samples", &Anima::AnimaGCFrameBufferConfig::_samples)
		.def_readwrite("sRGB", &Anima::AnimaGCFrameBufferConfig::_sRGB)
		.def_readwrite("doubleBuffer", &Anima::AnimaGCFrameBufferConfig::_doublebuffer);

	class_<Anima::AnimaGCContextConfig>("AnimaGCContextConfig")
		.def_readwrite("api", &Anima::AnimaGCContextConfig::_api)
		.def_readwrite("major", &Anima::AnimaGCContextConfig::_major)
		.def_readwrite("minor", &Anima::AnimaGCContextConfig::_minor)
		.def_readwrite("forward", &Anima::AnimaGCContextConfig::_forward)
		.def_readwrite("debug", &Anima::AnimaGCContextConfig::_debug)
		.def_readwrite("profile", &Anima::AnimaGCContextConfig::_profile)
		.def_readwrite("robustness", &Anima::AnimaGCContextConfig::_robustness)
		.def_readwrite("release", &Anima::AnimaGCContextConfig::_release);

	// AnimaTimer
	class_<Anima::AnimaTimer>("AnimaTimer")
		.def("Reset", &Anima::AnimaTimer::Reset)
		.def("Elapsed", &Anima::AnimaTimer::Elapsed);

	// AnimaAllocator
	class_<Anima::AnimaAllocator, boost::noncopyable>("AnimaAllocator", no_init);

	// AnimaVertex2f
	class_<Anima::AnimaVertex2f>("AnimaVertex2f")
		.def(init<>())
		.def(init<Anima::AFloat, Anima::AFloat>())
		.def(init<Anima::AnimaVertex3f&>())
		.def(init<Anima::AnimaVertex4f&>())
		.def_readwrite("x", &Anima::AnimaVertex2f::x)
		.def_readwrite("y", &Anima::AnimaVertex2f::y)
		.def_readwrite("u", &Anima::AnimaVertex2f::u)
		.def_readwrite("v", &Anima::AnimaVertex2f::v);
}

#endif