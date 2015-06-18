#include "AnimaEnginePY.h"

#if defined ANIMA_ENGINE_EXPORT_TO_PYTHON

#include "AnimaTypes.h"
#include "AnimaString.h"
#include "AnimaGC.h"
#include "AnimaEngine.h"
#include "AnimaAllocators.h"
#include "AnimaShadersManager.h"
#include "AnimaScenesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaModelsManager.h"
#include "AnimaLightsManager.h"
#include "AnimaMeshesManager.h"
#include "AnimaModelInstancesManager.h"
#include "AnimaMeshInstancesManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaAnimationsManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaRenderer.h"

#include "AnimaScene.h"
#include "AnimaModel.h"
#include "AnimaAnimation.h"
#include "AnimaMesh.h"
#include "AnimaModelInstance.h"
#include "AnimaMeshInstance.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"
#include "AnimaCamera.h"
#include "AnimaLight.h"
#include "AnimaFirstPersonCamera.h"
#include "AnimaThirdPersonCamera.h"
#include "AnimaCamera.h"

#include <boost/python.hpp>
using namespace boost::python;

struct AnimaString_to_python_str
{
	static PyObject* convert(Anima::AnimaString const& str)
	{
		return incref(object(str.GetConstBuffer()).ptr());
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

		new (storage)Anima::AnimaString(value, nullptr);

		data->convertible = storage;
	}
};

Anima::AnimaGC* wrap_CreateContext(int windowId, Anima::AnimaGCContextConfig ctxconfig, Anima::AnimaGCFrameBufferConfig fbconfig)
{
	return Anima::AnimaGC::CreateContext((void*)&(windowId), ctxconfig, fbconfig);
}

Anima::AnimaScene* (Anima::AnimaScenesManager::*CreateSceneChar)(const char*) = &Anima::AnimaScenesManager::CreateScene;
Anima::AnimaScene* (Anima::AnimaScenesManager::*GetSceneInt)(Anima::AUint) = &Anima::AnimaScenesManager::GetScene;
Anima::AnimaScene* (Anima::AnimaScenesManager::*GetSceneString)(const Anima::AnimaString&) = &Anima::AnimaScenesManager::GetScene;
Anima::AnimaScene* (Anima::AnimaScenesManager::*GetSceneChar)(const char*) = &Anima::AnimaScenesManager::GetScene;

Anima::AnimaModel* (Anima::AnimaModelsManager::*LoadModelCharString)(const char*, const Anima::AnimaString&) = &Anima::AnimaModelsManager::LoadModel;
Anima::AnimaModel* (Anima::AnimaModelsManager::*LoadModelCharChar)(const char*, const char*) = &Anima::AnimaModelsManager::LoadModel;
Anima::AnimaModel* (Anima::AnimaModelsManager::*LoadModelStringChar)(const Anima::AnimaString&, const char*) = &Anima::AnimaModelsManager::LoadModel;
Anima::AnimaModel* (Anima::AnimaModelsManager::*LoadModelStringString)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaModelsManager::LoadModel;
Anima::AnimaModel* (Anima::AnimaModelsManager::*CreateModelString)(const Anima::AnimaString&) = &Anima::AnimaModelsManager::CreateModel;
Anima::AnimaModel* (Anima::AnimaModelsManager::*CreateModelChar)(const char*) = &Anima::AnimaModelsManager::CreateModel;
Anima::AnimaModel* (Anima::AnimaModelsManager::*GetModelFromNameString)(const Anima::AnimaString&) = &Anima::AnimaModelsManager::GetModelFromName;
Anima::AnimaModel* (Anima::AnimaModelsManager::*GetModelFromNameChar)(const char*) = &Anima::AnimaModelsManager::GetModelFromName;

Anima::AnimaAnimation* (Anima::AnimaAnimationsManager::*GetAnimationFromNameString)(const Anima::AnimaString&) = &Anima::AnimaAnimationsManager::GetAnimationFromName;
Anima::AnimaAnimation* (Anima::AnimaAnimationsManager::*GetAnimationFromNameChar)(const char*) = &Anima::AnimaAnimationsManager::GetAnimationFromName;

Anima::AnimaMesh* (Anima::AnimaModel::*GetMeshFromNameString)(const Anima::AnimaString&) = &Anima::AnimaModel::GetMeshFromName;
Anima::AnimaMesh* (Anima::AnimaModel::*GetMeshFromNameChar)(const char*) = &Anima::AnimaModel::GetMeshFromName;
void (Anima::AnimaModel::*SetOriginFileNameString)(const Anima::AnimaString&) = &Anima::AnimaModel::SetOriginFileName;
void (Anima::AnimaModel::*SetOriginFileNameChar)(const char*) = &Anima::AnimaModel::SetOriginFileName;
void (Anima::AnimaModel::*SetAnimationNodeNameString)(const Anima::AnimaString&) = &Anima::AnimaModel::SetAnimationNodeName;
void (Anima::AnimaModel::*SetAnimationNodeNameChar)(const char*) = &Anima::AnimaModel::SetAnimationNodeName;

void (Anima::AnimaRenderer::*RendererStart)(Anima::AnimaScene*) = &Anima::AnimaRenderer::Start;
void (Anima::AnimaRenderer::*RendererDrawMesh)(Anima::AnimaMesh*) = &Anima::AnimaRenderer::DrawMesh;
void (Anima::AnimaRenderer::*RendererDrawMeshInstance)(Anima::AnimaMeshInstance*) = &Anima::AnimaRenderer::DrawMesh;
void (Anima::AnimaRenderer::*RendererDrawModel)(Anima::AnimaModel*) = &Anima::AnimaRenderer::DrawModel;
void (Anima::AnimaRenderer::*RendererDrawModelInstance)(Anima::AnimaModelInstance*) = &Anima::AnimaRenderer::DrawModel;

Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderCharString)(const char*, const Anima::AnimaString&, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderCharChar)(const char*, const char*, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderStringChar)(const Anima::AnimaString&, const char*, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderStringInfo)(const Anima::AnimaString&, Anima::AnimaShaderProgram::AnimaShaderInfo) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderCharInfo)(const char*, Anima::AnimaShaderProgram::AnimaShaderInfo) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileCharString)(const char*, const Anima::AnimaString&, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileCharChar)(const char*, const char*, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileStringChar)(const Anima::AnimaString&, const char*, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShader::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*CreateShaderString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*CreateShaderChar)(const char*) = &Anima::AnimaShadersManager::CreateShader;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*CreateProgramString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateProgram;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*CreateProgramChar)(const char*) = &Anima::AnimaShadersManager::CreateProgram;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*GetProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::GetProgramFromName;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*GetProgramFromNameChar)(const char*) = &Anima::AnimaShadersManager::GetProgramFromName;
void (Anima::AnimaShadersManager::*SetActiveProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::SetActiveProgramFromName;
void (Anima::AnimaShadersManager::*SetActiveProgramFromNameChar)(const char*) = &Anima::AnimaShadersManager::SetActiveProgramFromName;

Anima::AnimaFirstPersonCamera* (Anima::AnimaCamerasManager::*CreateFirstPersonCameraString)(const Anima::AnimaString&) = &Anima::AnimaCamerasManager::CreateCamera <Anima::AnimaFirstPersonCamera>;
Anima::AnimaFirstPersonCamera* (Anima::AnimaCamerasManager::*CreateFirstPersonCameraChar)(const char*) = &Anima::AnimaCamerasManager::CreateCamera <Anima::AnimaFirstPersonCamera>;
Anima::AnimaThirdPersonCamera* (Anima::AnimaCamerasManager::*CreateThirdPersonCameraString)(const Anima::AnimaString&) = &Anima::AnimaCamerasManager::CreateCamera <Anima::AnimaThirdPersonCamera>;
Anima::AnimaThirdPersonCamera* (Anima::AnimaCamerasManager::*CreateThirdPersonCameraChar)(const char*) = &Anima::AnimaCamerasManager::CreateCamera <Anima::AnimaThirdPersonCamera>;
Anima::AnimaCamera* (Anima::AnimaCamerasManager::*GetCameraFromNameString)(const Anima::AnimaString&) = &Anima::AnimaCamerasManager::GetCameraFromName;
Anima::AnimaCamera* (Anima::AnimaCamerasManager::*GetCameraFromNameChar)(const char*) = &Anima::AnimaCamerasManager::GetCameraFromName;

BOOST_PYTHON_MODULE(AnimaEngine)
{
	// Inizializzazione dei converters
	to_python_converter<Anima::AnimaString, AnimaString_to_python_str>();
	AnimaString_from_python_str();

	// AnimaEngine
	class_<Anima::AnimaEngine>("AnimaEngine")
		.def("Initialize", &Anima::AnimaEngine::Initialize)
		.def("Terminate", &Anima::AnimaEngine::Terminate)
		.def("GetModelsAllocator", &Anima::AnimaEngine::GetModelsAllocator, return_value_policy<reference_existing_object>())
		.def("GetGenericAllocator", &Anima::AnimaEngine::GetGenericAllocator, return_value_policy<reference_existing_object>())
		.def("GetMeshesAllocator", &Anima::AnimaEngine::GetMeshesAllocator, return_value_policy<reference_existing_object>())
		.def("GetModelInstancesAllocator", &Anima::AnimaEngine::GetModelInstancesAllocator, return_value_policy<reference_existing_object>())
		.def("GetMeshInstancesAllocator", &Anima::AnimaEngine::GetMeshInstancesAllocator, return_value_policy<reference_existing_object>())
		.def("GetStringAllocator", &Anima::AnimaEngine::GetStringAllocator, return_value_policy<reference_existing_object>())
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
		.def("CreateScene", CreateSceneChar, return_value_policy<reference_existing_object>())
		.def("GetScene", GetSceneInt, return_value_policy<reference_existing_object>())
		.def("GetScene", GetSceneString, return_value_policy<reference_existing_object>())
		.def("GetScene", GetSceneChar, return_value_policy<reference_existing_object>());
	
	// AnimaScene
	class_<Anima::AnimaScene>("AnimaScene", no_init)
		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
		.def("GetModelsManager", &Anima::AnimaScene::GetModelsManager, return_value_policy<reference_existing_object>())
		.def("GetMeshesManager", &Anima::AnimaScene::GetMeshesManager, return_value_policy<reference_existing_object>())
		.def("GetModelInstancesManager", &Anima::AnimaScene::GetModelInstancesManager, return_value_policy<reference_existing_object>())
		.def("GetMeshInstancesManager", &Anima::AnimaScene::GetMeshInstancesManager, return_value_policy<reference_existing_object>())
		.def("GetShadersManager", &Anima::AnimaScene::GetShadersManager, return_value_policy<reference_existing_object>())
		.def("GetCamerasManager", &Anima::AnimaScene::GetCamerasManager, return_value_policy<reference_existing_object>())
		.def("GetTexturesManager", &Anima::AnimaScene::GetTexturesManager, return_value_policy<reference_existing_object>())
		.def("GetDataGeneratorsManager", &Anima::AnimaScene::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
		.def("GetMaterialsManager", &Anima::AnimaScene::GetMaterialsManager, return_value_policy<reference_existing_object>())
		.def("GetLightsManager", &Anima::AnimaScene::GetLightsManager, return_value_policy<reference_existing_object>())
		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
		.def("GetAnimationsManager", &Anima::AnimaScene::GetAnimationsManager, return_value_policy<reference_existing_object>())
		.def("GetMeshesAllocator", &Anima::AnimaScene::GetMeshesAllocator, return_value_policy<reference_existing_object>())
		.def("GetModelInstancesAllocator", &Anima::AnimaScene::GetModelInstancesAllocator, return_value_policy<reference_existing_object>())
		.def("GetMeshInstancesAllocator", &Anima::AnimaScene::GetMeshInstancesAllocator, return_value_policy<reference_existing_object>())
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
		.def("GetAnimationsNumber", &Anima::AnimaAnimationsManager::GetAnimationsNumber)
		.def("GetAnimation", &Anima::AnimaAnimationsManager::GetAnimation, return_value_policy<reference_existing_object>())
		.def("GetAnimationFromName", GetAnimationFromNameString, return_value_policy<reference_existing_object>())
		.def("GetAnimationFromName", GetAnimationFromNameChar, return_value_policy<reference_existing_object>())
		.def("GetLastLoadedAnimations", &Anima::AnimaAnimationsManager::GetLastLoadedAnimations, return_value_policy<reference_existing_object>())
		.def("ClearAnimations", &Anima::AnimaAnimationsManager::ClearAnimations)
		.def("ClearLastLoadedAnimations", &Anima::AnimaAnimationsManager::ClearLastLoadedAnimations);
	
	// AnimaModelsManager
	class_<Anima::AnimaModelsManager>("AnimaModelsManager", no_init)
		.def("LoadModel", LoadModelCharString, return_value_policy<reference_existing_object>())
		.def("LoadModel", LoadModelCharChar, return_value_policy<reference_existing_object>())
		.def("LoadModel", LoadModelStringChar, return_value_policy<reference_existing_object>())
		.def("LoadModel", LoadModelStringString, return_value_policy<reference_existing_object>())
		.def("CreateModel", CreateModelString, return_value_policy<reference_existing_object>())
		.def("CreateModel", CreateModelChar, return_value_policy<reference_existing_object>())
		.def("GetModelFromName", GetModelFromNameString, return_value_policy<reference_existing_object>())
		.def("GetModelFromName", GetModelFromNameChar, return_value_policy<reference_existing_object>())
		.def("GetModel", &Anima::AnimaModelsManager::GetModel, return_value_policy<reference_existing_object>())
		.def("ClearModels", &Anima::AnimaModelsManager::ClearModels);

	// AnimaModel
	class_<Anima::AnimaModel>("AnimaModel", init<const Anima::AnimaString&, Anima::AnimaDataGeneratorsManager*, Anima::AnimaAllocator*>())
		.def("SetMaterial", &Anima::AnimaModel::SetMaterial)
		.def("GetMaterial", &Anima::AnimaModel::GetMaterial, return_value_policy<reference_existing_object>())
		.def("GetMeshesCount", &Anima::AnimaModel::GetMeshesCount)
		.def("AddMesh", &Anima::AnimaModel::AddMesh)
		.def("GetMesh", &Anima::AnimaModel::GetMesh, return_value_policy<reference_existing_object>())
		.def("GetMeshFromName", GetMeshFromNameString, return_value_policy<reference_existing_object>())
		.def("GetMeshFromName", GetMeshFromNameChar, return_value_policy<reference_existing_object>())
		.def("GetAnimationsCount", &Anima::AnimaModel::GetAnimationsCount)
		.def("SetAnimations", &Anima::AnimaModel::SetAnimations)
		.def("AddAnimation", &Anima::AnimaModel::AddAnimation)
		.def("GetAnimation", &Anima::AnimaModel::GetAnimation, return_value_policy<reference_existing_object>())
		.def("SetOriginFileName", SetOriginFileNameString)
		.def("SetOriginFileName", SetOriginFileNameChar)
		.def("GetAnimaOriginFileName", &Anima::AnimaModel::GetAnimaOriginFileName)
		.def("GetOriginFileName", &Anima::AnimaModel::GetOriginFileName)
		.def("SetAnimationNodeName", SetAnimationNodeNameString)
		.def("SetAnimationNodeName", SetAnimationNodeNameChar)
		.def("GetAnimaAnimationNodeName", &Anima::AnimaModel::GetAnimaAnimationNodeName)
		.def("GetAnimationNodeName", &Anima::AnimaModel::GetAnimationNodeName)
		.def("GetMeshesBonesInfo", &Anima::AnimaModel::GetMeshesBonesInfo, return_value_policy<reference_existing_object>())
		.def("SetMeshesBonesInfo", &Anima::AnimaModel::SetMeshesBonesInfo)
		.def("ClearMeshesBonesInfo", &Anima::AnimaModel::ClearMeshesBonesInfo)
		.def("UpdateAnimation", &Anima::AnimaModel::UpdateAnimation)
		.def("SetActiveAnimation", &Anima::AnimaModel::SetActiveAnimation)
		.def("StopAnimation", &Anima::AnimaModel::StopAnimation);

	// AnimaRenderer
	class_<Anima::AnimaRenderer, boost::noncopyable>("AnimaRenderer", init<Anima::AnimaEngine*, Anima::AnimaAllocator*>())
		.def("Start", RendererStart)
		.def("DrawAll", &Anima::AnimaRenderer::DrawAll)
		.def("DrawMesh", RendererDrawMesh)
		.def("DrawMesh", RendererDrawMeshInstance)
		.def("DrawModel", RendererDrawModel)
		.def("DrawModel", RendererDrawModelInstance)
		.def("AddPrimitive", &Anima::AnimaRenderer::AddPrimitive)
		.def("UpdateModelsVisibility", &Anima::AnimaRenderer::UpdateModelsVisibility)
		.def("InitTextureSlots", &Anima::AnimaRenderer::InitTextureSlots)
		.def("InitRenderingTargets", &Anima::AnimaRenderer::InitRenderingTargets)
		.def("InitRenderingUtilities", &Anima::AnimaRenderer::InitRenderingUtilities);

	// AnimaShadersManager
	class_<Anima::AnimaShadersManager>("AnimaShadersManager", no_init)
		.def("LoadShader", LoadShaderCharString, return_value_policy<reference_existing_object>())
		.def("LoadShader", LoadShaderCharChar, return_value_policy<reference_existing_object>())
		.def("LoadShader", LoadShaderStringChar, return_value_policy<reference_existing_object>())
		.def("LoadShader", LoadShaderStringString, return_value_policy<reference_existing_object>())
		.def("LoadShaderFromFile", LoadShaderFromFileCharString, return_value_policy<reference_existing_object>())
		.def("LoadShaderFromFile", LoadShaderFromFileCharChar, return_value_policy<reference_existing_object>())
		.def("LoadShaderFromFile", LoadShaderFromFileStringChar, return_value_policy<reference_existing_object>())
		.def("LoadShaderFromFile", LoadShaderFromFileStringString, return_value_policy<reference_existing_object>())
		.def("LoadShader", LoadShaderStringInfo, return_value_policy<reference_existing_object>())
		.def("LoadShader", LoadShaderCharInfo, return_value_policy<reference_existing_object>())
		.def("CreateShader", CreateShaderString, return_value_policy<reference_existing_object>())
		.def("CreateShader", CreateShaderChar, return_value_policy<reference_existing_object>())
		.def("CreateProgram", CreateProgramString, return_value_policy<reference_existing_object>())
		.def("CreateProgram", CreateProgramChar, return_value_policy<reference_existing_object>())
		.def("GetProgram", &Anima::AnimaShadersManager::GetProgram, return_value_policy<reference_existing_object>())
		.def("GetProgramFromName", GetProgramFromNameString, return_value_policy<reference_existing_object>())
		.def("GetProgramFromName", GetProgramFromNameChar, return_value_policy<reference_existing_object>())
		.def("SetActiveProgram", &Anima::AnimaShadersManager::SetActiveProgram, return_value_policy<reference_existing_object>())
		.def("SetActiveProgramFromName", SetActiveProgramFromNameString, return_value_policy<reference_existing_object>())
		.def("SetActiveProgramFromName", SetActiveProgramFromNameChar, return_value_policy<reference_existing_object>())
		.def("GetActiveProgram", &Anima::AnimaShadersManager::GetActiveProgram, return_value_policy<reference_existing_object>());

	// AnimaShader
	enum_<Anima::AnimaShader::AnimaShaderType>("AnimaShaderType")
		.value("INVALID", Anima::AnimaShader::AnimaShaderType::INVALID)
		.value("VERTEX", Anima::AnimaShader::AnimaShaderType::VERTEX)
		.value("TESSELLATION_CONTROL", Anima::AnimaShader::AnimaShaderType::TESSELLATION_CONTROL)
		.value("TESSELLATION_EVALUATION", Anima::AnimaShader::AnimaShaderType::TESSELLATION_EVALUATION)
		.value("GEOMETRY", Anima::AnimaShader::AnimaShaderType::GEOMETRY)
		.value("FRAGMENT", Anima::AnimaShader::AnimaShaderType::FRAGMENT);

	class_<Anima::AnimaShader>("AnimaShader", no_init);

	// AnimaShaderProgram
	enum_<Anima::AnimaShaderProgram::AnimaShaderInfoType>("AnimaShaderInfoType")
		.value("SHADER_FILE", Anima::AnimaShaderProgram::AnimaShaderInfoType::SHADER_FILE)
		.value("SHADER_TEXT", Anima::AnimaShaderProgram::AnimaShaderInfoType::SHADER_TEXT);

	class_<Anima::AnimaShaderProgram::AnimaShaderInfo>("AnimaShaderInfo")
		.def_readwrite("text", &Anima::AnimaShaderProgram::AnimaShaderInfo::_text)
		.def_readwrite("infoType", &Anima::AnimaShaderProgram::AnimaShaderInfo::_infoType)
		.def_readwrite("shaderType", &Anima::AnimaShaderProgram::AnimaShaderInfo::_shaderType);

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
		.def("CreateFirstPersonCamera", CreateFirstPersonCameraString, return_value_policy<reference_existing_object>())
		.def("CreateFirstPersonCamera", CreateFirstPersonCameraChar, return_value_policy<reference_existing_object>())
		.def("CreateThirdPersonCamera", CreateThirdPersonCameraString, return_value_policy<reference_existing_object>())
		.def("CreateThirdPersonCamera", CreateThirdPersonCameraChar, return_value_policy<reference_existing_object>())
		.def("GetActiveCamera", &Anima::AnimaCamerasManager::GetActiveCamera, return_value_policy<reference_existing_object>())
		.def("GetCameraFromName", GetCameraFromNameString, return_value_policy<reference_existing_object>())
		.def("GetCameraFromName", GetCameraFromNameChar, return_value_policy<reference_existing_object>())
		.def("GetTotalCamerasCount", &Anima::AnimaCamerasManager::GetTotalCamerasCount)
		.def("GetCameras", &Anima::AnimaCamerasManager::GetCameras, return_value_policy<reference_existing_object>())
		.def("UpdatePerspectiveCameras", &Anima::AnimaCamerasManager::UpdatePerspectiveCameras)
		.def("UpdateOrthoCameras", &Anima::AnimaCamerasManager::UpdateOrthoCameras);

	// AnimaCamera
	class_<Anima::AnimaCamera, boost::noncopyable>("AnimaCamera", no_init)
		.def("GetPosition", &Anima::AnimaCamera::GetPosition)
		.def("Zoom", pure_virtual(&Anima::AnimaCamera::Zoom))
		.def("Activate", &Anima::AnimaCamera::Activate)
		.def("Deactivate", &Anima::AnimaCamera::Deactivate);

	// AnimaFirstPersonCamera
	class_<Anima::AnimaFirstPersonCamera, bases<Anima::AnimaCamera> >("AnimaFirstPersonCamera", no_init)
		.def("Activate", &Anima::AnimaFirstPersonCamera::Activate)
		.def("Deactivate", &Anima::AnimaFirstPersonCamera::Deactivate);
	
	// AnimaGC
	class_<Anima::AnimaGC>("AnimaGC", no_init)
		.def("CreateContext", &wrap_CreateContext, return_value_policy<manage_new_object>())
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

	// AnimaAllocator
	class_<Anima::AnimaAllocator, boost::noncopyable>("AnimaAllocator", no_init);
}

#endif