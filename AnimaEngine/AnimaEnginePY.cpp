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

void (Anima::AnimaScenesManager::*SaveSceneToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaScenesManager::SaveSceneToFile;
void (Anima::AnimaScenesManager::*SaveSceneToFileWithScene)(Anima::AnimaScene*, const Anima::AnimaString&, bool) = &Anima::AnimaScenesManager::SaveSceneToFile;

void (Anima::AnimaNodesManager::*SaveNodeToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaNodesManager::SaveNodeToFile;
void (Anima::AnimaNodesManager::*SaveNodeToFileWithNode)(Anima::AnimaNode*, const Anima::AnimaString&, bool) = &Anima::AnimaNodesManager::SaveNodeToFile;

void (Anima::AnimaNodeInstancesManager::*SaveNodeInstanceToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaNodeInstancesManager::SaveNodeInstanceToFile;
void (Anima::AnimaNodeInstancesManager::*SaveNodeInstanceToFileWithNode)(Anima::AnimaNodeInstance*, const Anima::AnimaString&, bool) = &Anima::AnimaNodeInstancesManager::SaveNodeInstanceToFile;

Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateNodeInstanceWithName)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaNodeInstancesManager::CreateNodeInstance;
Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateNodeInstanceWithNode)(const Anima::AnimaString&, Anima::AnimaNode*) = &Anima::AnimaNodeInstancesManager::CreateNodeInstance;
Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateAssetInstanceWithName)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaNodeInstancesManager::CreateAssetInstance;
Anima::AnimaNodeInstance* (Anima::AnimaNodeInstancesManager::*CreateAssetInstanceWithAsset)(const Anima::AnimaString&, Anima::AnimaNode*) = &Anima::AnimaNodeInstancesManager::CreateAssetInstance;

void (Anima::AnimaGeometriesManager::*SaveGeometryToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaGeometriesManager::SaveGeometryToFile;
void (Anima::AnimaGeometriesManager::*SaveGeometryToFileWithGeometry)(Anima::AnimaGeometry*, const Anima::AnimaString&, bool) = &Anima::AnimaGeometriesManager::SaveGeometryToFile;

void (Anima::AnimaGeometryInstancesManager::*SaveGeometryInstanceToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaGeometryInstancesManager::SaveGeometryInstanceToFile;
void (Anima::AnimaGeometryInstancesManager::*SaveGeometryInstanceToFileWithGeometry)(Anima::AnimaGeometryInstance*, const Anima::AnimaString&, bool) = &Anima::AnimaGeometryInstancesManager::SaveGeometryInstanceToFile;

Anima::AnimaGeometryInstance* (Anima::AnimaGeometryInstancesManager::*CreateGeometryInstanceWithName)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaGeometryInstancesManager::CreateGeometryInstance;
Anima::AnimaGeometryInstance* (Anima::AnimaGeometryInstancesManager::*CreateGeometryInstanceWithGeometry)(const Anima::AnimaString&, Anima::AnimaGeometry*) = &Anima::AnimaGeometryInstancesManager::CreateGeometryInstance;

void (Anima::AnimaCamerasManager::*SaveCameraToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaCamerasManager::SaveCameraToFile;
void (Anima::AnimaCamerasManager::*SaveCameraToFileWithCamera)(Anima::AnimaCamera*, const Anima::AnimaString&, bool) = &Anima::AnimaCamerasManager::SaveCameraToFile;

void (Anima::AnimaLightsManager::*SaveLightToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaLightsManager::SaveLightToFile;
void (Anima::AnimaLightsManager::*SaveLightToFileWithLight)(Anima::AnimaLight*, const Anima::AnimaString&, bool) = &Anima::AnimaLightsManager::SaveLightToFile;

void (Anima::AnimaMaterialsManager::*SaveMaterialToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaMaterialsManager::SaveMaterialToFile;
void (Anima::AnimaMaterialsManager::*SaveMaterialToFileWithMaterial)(Anima::AnimaMaterial*, const Anima::AnimaString&, bool) = &Anima::AnimaMaterialsManager::SaveMaterialToFile;

Anima::AnimaTexture* (Anima::AnimaTexturesManager::*LoadTextureFromAnimaTextureFile)(const Anima::AnimaString&) = &Anima::AnimaTexturesManager::LoadTextureFromFile;
Anima::AnimaTexture* (Anima::AnimaTexturesManager::*LoadTextureFromImageFile)(const Anima::AnimaString&, const Anima::AnimaString&) = &Anima::AnimaTexturesManager::LoadTextureFromFile;
void (Anima::AnimaTexturesManager::*SaveTextureToFileWithName)(const Anima::AnimaString&, const Anima::AnimaString&, bool) = &Anima::AnimaTexturesManager::SaveTextureToFile;
void (Anima::AnimaTexturesManager::*SaveTextureToFileWithTexture)(Anima::AnimaTexture*, const Anima::AnimaString&, bool) = &Anima::AnimaTexturesManager::SaveTextureToFile;

bool (Anima::AnimaDataGeneratorsManager::*RemoveGeneratorWithName)(const Anima::AnimaString&) = &Anima::AnimaDataGeneratorsManager::RemoveGenerator;
bool (Anima::AnimaDataGeneratorsManager::*RemoveGeneratorWithGenerator)(Anima::AnimaDataGenerator*) = &Anima::AnimaDataGeneratorsManager::RemoveGenerator;

void (Anima::AnimaMappedValues::*AMV_AddTexture1)(const Anima::AnimaString&, Anima::AnimaTextureGenerator*) = &Anima::AnimaMappedValues::AddTexture;
void (Anima::AnimaMappedValues::*AMV_AddTexture2)(const Anima::AnimaString&, Anima::AnimaTexture*) = &Anima::AnimaMappedValues::AddTexture;
void (Anima::AnimaMappedValues::*AMV_SetTexture1)(const Anima::AnimaString&, Anima::AnimaTextureGenerator*) = &Anima::AnimaMappedValues::SetTexture;
void (Anima::AnimaMappedValues::*AMV_SetTexture2)(const Anima::AnimaString&, Anima::AnimaTexture*) = &Anima::AnimaMappedValues::SetTexture;
void (Anima::AnimaMappedValues::*AMV_AddColor1)(const Anima::AnimaString&, Anima::AnimaColorGenerator*) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor2)(const Anima::AnimaString&, Anima::AnimaColor3f) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor4)(const Anima::AnimaString&, Anima::AnimaColor4f) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_AddColor5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddColor;
void (Anima::AnimaMappedValues::*AMV_SetColor1)(const Anima::AnimaString&, Anima::AnimaColorGenerator*) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor2)(const Anima::AnimaString&, Anima::AnimaColor3f) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor4)(const Anima::AnimaString&, Anima::AnimaColor4f) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_SetColor5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetColor;
void (Anima::AnimaMappedValues::*AMV_AddVector1)(const Anima::AnimaString&, Anima::AnimaVectorGenerator*) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector2)(const Anima::AnimaString&, Anima::AnimaVertex2f) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector4)(const Anima::AnimaString&, Anima::AnimaVertex3f) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector6)(const Anima::AnimaString&, Anima::AnimaVertex4f) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_AddVector7)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::AddVector;
void (Anima::AnimaMappedValues::*AMV_SetVector1)(const Anima::AnimaString&, Anima::AnimaVectorGenerator*) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector2)(const Anima::AnimaString&, Anima::AnimaVertex2f) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector3)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector4)(const Anima::AnimaString&, Anima::AnimaVertex3f) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector5)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector6)(const Anima::AnimaString&, Anima::AnimaVertex4f) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_SetVector7)(const Anima::AnimaString&, Anima::AFloat, Anima::AFloat, Anima::AFloat, Anima::AFloat) = &Anima::AnimaMappedValues::SetVector;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray1)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVectorGenerator*>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray2)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex2f>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray3)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex3f>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddVectorArray4)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex4f>*) = &Anima::AnimaMappedValues::AddVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray1)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVectorGenerator*>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray2)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex2f>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray3)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex3f>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_SetVectorArray4)(const Anima::AnimaString&, Anima::AnimaArray<Anima::AnimaVertex4f>*) = &Anima::AnimaMappedValues::SetVectorArray;
void (Anima::AnimaMappedValues::*AMV_AddMatrix1)(const Anima::AnimaString&, const Anima::AnimaMatrix&) = &Anima::AnimaMappedValues::AddMatrix;
void (Anima::AnimaMappedValues::*AMV_AddMatrix2)(const Anima::AnimaString&, Anima::AFloat[16]) = &Anima::AnimaMappedValues::AddMatrix;
void (Anima::AnimaMappedValues::*AMV_SetMatrix1)(const Anima::AnimaString&, const Anima::AnimaMatrix&) = &Anima::AnimaMappedValues::SetMatrix;
void (Anima::AnimaMappedValues::*AMV_SetMatrix2)(const Anima::AnimaString&, Anima::AFloat[16]) = &Anima::AnimaMappedValues::SetMatrix;




Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShader;
Anima::AnimaShader* (Anima::AnimaShadersManager::*LoadShaderFromFileStringString)(const Anima::AnimaString&, const Anima::AnimaString&, Anima::AnimaShaderType) = &Anima::AnimaShadersManager::LoadShaderFromFile;
Anima::AnimaShader* (Anima::AnimaShadersManager::*CreateShaderString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateShader;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*CreateProgramString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::CreateProgram;
Anima::AnimaShaderProgram* (Anima::AnimaShadersManager::*GetProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::GetProgramFromName;
void (Anima::AnimaShadersManager::*SetActiveProgramFromNameString)(const Anima::AnimaString&) = &Anima::AnimaShadersManager::SetActiveProgramFromName;

BOOST_PYTHON_MODULE(AnimaEngine)
{
	// Inizializzazione dei converters
	to_python_converter<Anima::AnimaString, AnimaString_to_python_str>();
	AnimaString_from_python_str();

	// AnimaEngine
	class_<Anima::AnimaEngine>("AnimaEngine")
	.def("Initialize", &Anima::AnimaEngine::Initialize)
	.def("Terminate", &Anima::AnimaEngine::Terminate)
	.def("GetDataGeneratorsManager", &Anima::AnimaEngine::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
	.def("GetParallelProgramsManager", &Anima::AnimaEngine::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
	.def("GetScenesManager", &Anima::AnimaEngine::GetScenesManager, return_value_policy<reference_existing_object>())
	.def("GetShadersManager", &Anima::AnimaEngine::GetShadersManager, return_value_policy<reference_existing_object>())
	;
	
	// AnimaScenesManager
	class_<Anima::AnimaScenesManager>("AnimaScenesManager", no_init)
	.def("CreateScene", &Anima::AnimaScenesManager::CreateScene, return_value_policy<reference_existing_object>())
	.def("GetActiveScene", &Anima::AnimaScenesManager::GetActiveScene, return_value_policy<reference_existing_object>())
	.def("GetScene", &Anima::AnimaScenesManager::GetScene, return_value_policy<reference_existing_object>())
	.def("GetScene", &Anima::AnimaScenesManager::GetSceneFromName, return_value_policy<reference_existing_object>())
	.def("LoadSceneFromFile", &Anima::AnimaScenesManager::LoadSceneFromFile, return_value_policy<reference_existing_object>())
	.def("LoadSceneFromXml", &Anima::AnimaScenesManager::LoadSceneFromXml, return_value_policy<reference_existing_object>())
	.def("LoadScenes", &Anima::AnimaScenesManager::LoadScenes)
	.def("SaveSceneToFile", SaveSceneToFileWithName)
	.def("SaveSceneToFile", SaveSceneToFileWithScene)
	.def("SaveScenes", &Anima::AnimaScenesManager::SaveScenes)
	;
	
	// AnimaNodesManager
	class_<Anima::AnimaNodesManager>("AnimaNodesManager", no_init)
	.def("CreateNode", &Anima::AnimaNodesManager::CreateNode, return_value_policy<reference_existing_object>())
	.def("GetNode", &Anima::AnimaNodesManager::GetNode, return_value_policy<reference_existing_object>())
	.def("GetNodeFromName", &Anima::AnimaNodesManager::GetNodeFromName, return_value_policy<reference_existing_object>())
	.def("GetNodesCount", &Anima::AnimaNodesManager::GetNodesCount)
	.def("GetAsset", &Anima::AnimaNodesManager::GetAsset, return_value_policy<reference_existing_object>())
	.def("GetAssetFromName", &Anima::AnimaNodesManager::GetAssetFromName, return_value_policy<reference_existing_object>())
	.def("GetAssetsCount", &Anima::AnimaNodesManager::GetAssetsCount)
	.def("LoadAssetFromExternalFile", &Anima::AnimaNodesManager::LoadAssetFromExternalFile, return_value_policy<reference_existing_object>())
	.def("LoadNodeFromFile", &Anima::AnimaNodesManager::LoadNodeFromFile, return_value_policy<reference_existing_object>())
	.def("LoadNodeFromXml", &Anima::AnimaNodesManager::LoadNodeFromXml, return_value_policy<reference_existing_object>())
	.def("LoadNodes", &Anima::AnimaNodesManager::LoadNodes)
	.def("SaveNodeToFile", SaveNodeToFileWithName)
	.def("SaveNodeToFile", SaveNodeToFileWithNode)
	.def("SaveNodes", &Anima::AnimaNodesManager::SaveNodes)
	;
	
	// AnimaNodeInstancesManager
	class_<Anima::AnimaNodeInstancesManager>("AnimaNodeInstancesManager", no_init)
	.def("CreateNodeInstance", CreateNodeInstanceWithName, return_value_policy<reference_existing_object>())
	.def("CreateNodeInstance", CreateNodeInstanceWithNode, return_value_policy<reference_existing_object>())
	.def("GetNodeInstance", &Anima::AnimaNodeInstancesManager::GetNodeInstance, return_value_policy<reference_existing_object>())
	.def("GetNodeInstanceFromName", &Anima::AnimaNodeInstancesManager::GetNodeInstanceFromName, return_value_policy<reference_existing_object>())
	.def("GetNodeInstancesCount", &Anima::AnimaNodeInstancesManager::GetNodeInstancesCount)
	.def("CreateAssetInstance", CreateAssetInstanceWithName, return_value_policy<reference_existing_object>())
	.def("CreateAssetInstance", CreateAssetInstanceWithAsset, return_value_policy<reference_existing_object>())
	.def("GetAssetInstance", &Anima::AnimaNodeInstancesManager::GetAssetInstance, return_value_policy<reference_existing_object>())
	.def("GetAssetInstanceFromName", &Anima::AnimaNodeInstancesManager::GetAssetInstanceFromName, return_value_policy<reference_existing_object>())
	.def("GetAssetInstancesCount", &Anima::AnimaNodeInstancesManager::GetAssetInstancesCount)
	.def("LoadNodeInstanceFromFile", &Anima::AnimaNodesManager::LoadNodeFromFile, return_value_policy<reference_existing_object>())
	.def("LoadNodeInstanceFromXml", &Anima::AnimaNodesManager::LoadNodeFromXml, return_value_policy<reference_existing_object>())
	.def("LoadNodesInstances", &Anima::AnimaNodesManager::LoadNodes)
	.def("SaveNodeInstanceToFile", SaveNodeInstanceToFileWithName)
	.def("SaveNodeInstanceToFile", SaveNodeInstanceToFileWithNode)
	.def("SaveNodesInstances", &Anima::AnimaNodeInstancesManager::SaveNodesInstances)
	;
	
	// AnimaGeometriesManager
	class_<Anima::AnimaGeometriesManager>("AnimaGeometriesManager", no_init)
	.def("CreateGeometry", &Anima::AnimaGeometriesManager::CreateGeometry, return_value_policy<reference_existing_object>())
	.def("GetGeometry", &Anima::AnimaGeometriesManager::GetGeometry, return_value_policy<reference_existing_object>())
	.def("GetGeometryFromName", &Anima::AnimaGeometriesManager::GetGeometryFromName, return_value_policy<reference_existing_object>())
	.def("GetGeometriesCount", &Anima::AnimaGeometriesManager::GetGeometriesCount)
	.def("LoadGeometryFromFile", &Anima::AnimaGeometriesManager::LoadGeometryFromFile, return_value_policy<reference_existing_object>())
	.def("LoadGeometryFromXml", &Anima::AnimaGeometriesManager::LoadGeometryFromXml, return_value_policy<reference_existing_object>())
	.def("LoadGeometries", &Anima::AnimaGeometriesManager::LoadGeometries)
	.def("SaveGeometryToFile", SaveGeometryToFileWithName)
	.def("SaveGeometryToFile", SaveGeometryToFileWithGeometry)
	.def("SaveGeometries", &Anima::AnimaGeometriesManager::SaveGeometries)
	;
	
	// AnimaGeometryInstancesManager
	class_<Anima::AnimaGeometryInstancesManager>("AnimaGeometryInstancesManager", no_init)
	.def("CreateGeometryInstance", CreateGeometryInstanceWithName, return_value_policy<reference_existing_object>())
	.def("CreateGeometryInstance", CreateGeometryInstanceWithGeometry, return_value_policy<reference_existing_object>())
	.def("CreateInstancesFromNode", &Anima::AnimaGeometryInstancesManager::CreateInstancesFromNode, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstance", &Anima::AnimaGeometryInstancesManager::GetGeometryInstance, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstanceFromName", &Anima::AnimaGeometryInstancesManager::GetGeometryInstanceFromName, return_value_policy<reference_existing_object>())
	.def("GetGeometryInstancesCount", &Anima::AnimaGeometryInstancesManager::GetGeometryInstancesCount)
	.def("LoadGeometryInstanceFromFile", &Anima::AnimaGeometryInstancesManager::LoadGeometryInstanceFromFile, return_value_policy<reference_existing_object>())
	.def("LoadGeometryInstanceFromXml", &Anima::AnimaGeometryInstancesManager::LoadGeometryInstanceFromXml, return_value_policy<reference_existing_object>())
	.def("LoadGeometriesInstances", &Anima::AnimaGeometryInstancesManager::LoadGeometriesInstances)
	.def("SaveGeometryInstanceToFile", SaveGeometryInstanceToFileWithName)
	.def("SaveGeometryInstanceToFile", SaveGeometryInstanceToFileWithGeometry)
	.def("SaveGeometriesInstances", &Anima::AnimaGeometryInstancesManager::SaveGeometriesInstances)
	;
	
	// AnimaCamerasManager
	class_<Anima::AnimaCamerasManager>("AnimaCamerasManager", no_init)
	.def("CreateCamera", &Anima::AnimaCamerasManager::CreateCamera, return_value_policy<reference_existing_object>())
	.def("GetCamera", &Anima::AnimaCamerasManager::GetCamera, return_value_policy<reference_existing_object>())
	.def("GetCameraFromName", &Anima::AnimaCamerasManager::GetCameraFromName, return_value_policy<reference_existing_object>())
	.def("GetActiveCamera", &Anima::AnimaCamerasManager::GetActiveCamera, return_value_policy<reference_existing_object>())
	.def("GetCamerasCount", &Anima::AnimaCamerasManager::GetCamerasCount)
	.def("LoadCameraFromFile", &Anima::AnimaCamerasManager::LoadCameraFromFile, return_value_policy<reference_existing_object>())
	.def("LoadCameraFromXml", &Anima::AnimaCamerasManager::LoadCameraFromXml, return_value_policy<reference_existing_object>())
	.def("LoadCameras", &Anima::AnimaCamerasManager::LoadCameras)
	.def("SaveCameraToFile", SaveCameraToFileWithName)
	.def("SaveCameraToFile", SaveCameraToFileWithCamera)
	.def("SaveCameras", &Anima::AnimaCamerasManager::SaveCameras)
	.def("UpdateCameras", &Anima::AnimaCamerasManager::UpdateCameras)
	.def("UpdateOrthographicCameras", &Anima::AnimaCamerasManager::UpdateOrthographicCameras)
	.def("UpdatePerspectiveCameras", &Anima::AnimaCamerasManager::UpdatePerspectiveCameras)
	;
	
	// AnimaLightsManager
	class_<Anima::AnimaLightsManager>("AnimaLightsManager", no_init)
	.def("CreateDirectionalLight", &Anima::AnimaLightsManager::CreateDirectionalLight, return_value_policy<reference_existing_object>())
	.def("CreatePointLight", &Anima::AnimaLightsManager::CreatePointLight, return_value_policy<reference_existing_object>())
	.def("CreateSpotLight", &Anima::AnimaLightsManager::CreateSpotLight, return_value_policy<reference_existing_object>())
	.def("CreateHemisphereLight", &Anima::AnimaLightsManager::CreateHemisphereLight, return_value_policy<reference_existing_object>())
	.def("GetLightFromName", &Anima::AnimaLightsManager::GetLightFromName, return_value_policy<reference_existing_object>())
	.def("GetLightsCount", &Anima::AnimaLightsManager::GetTotalLightsCount)
	.def("LoadLightFromFile", &Anima::AnimaLightsManager::LoadLightFromFile, return_value_policy<reference_existing_object>())
	.def("LoadLightFromXml", &Anima::AnimaLightsManager::LoadLightFromXml, return_value_policy<reference_existing_object>())
	.def("LoadLights", &Anima::AnimaLightsManager::LoadLights)
	.def("SaveLightToFile", SaveLightToFileWithName)
	.def("SaveLightToFile", SaveLightToFileWithLight)
	.def("SaveLights", &Anima::AnimaLightsManager::SaveLights)
	.def("UpdateLightsMatrix", &Anima::AnimaLightsManager::UpdateLightsMatrix)
	;

	// AnimaMaterialsManager
	class_<Anima::AnimaMaterialsManager>("AnimaMaterialsManager", no_init)
	.def("CreateMaterial", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("GetDefaultMaterial", &Anima::AnimaMaterialsManager::GetDefaultMaterial, return_value_policy<reference_existing_object>())
	.def("GetMaterial", &Anima::AnimaMaterialsManager::GetMaterial, return_value_policy<reference_existing_object>())
	.def("GetMaterialFromName", &Anima::AnimaMaterialsManager::GetMaterialFromName, return_value_policy<reference_existing_object>())
	.def("GetMaterials", &Anima::AnimaMaterialsManager::GetMaterials, return_value_policy<reference_existing_object>())
	.def("GetMaterialsCount", &Anima::AnimaMaterialsManager::GetMaterialsCount)
	.def("LoadMaterialFromFile", &Anima::AnimaMaterialsManager::LoadMaterialFromFile, return_value_policy<reference_existing_object>())
	.def("LoadMaterialFromXml", &Anima::AnimaMaterialsManager::LoadMaterialFromXml, return_value_policy<reference_existing_object>())
	.def("LoadMaterials", &Anima::AnimaMaterialsManager::LoadMaterials)
	.def("SaveMaterialToFile", SaveMaterialToFileWithName)
	.def("SaveMaterialToFile", SaveMaterialToFileWithMaterial)
	.def("SaveMaterials", &Anima::AnimaMaterialsManager::SaveMaterials)
	;

	// AnimaTexturesManager
	class_<Anima::AnimaTexturesManager>("AnimaTexturesManager", no_init)
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("CreateTexture", &Anima::AnimaMaterialsManager::CreateMaterial, return_value_policy<reference_existing_object>())
	.def("GetTexture", &Anima::AnimaTexturesManager::GetTexture, return_value_policy<reference_existing_object>())
	.def("GetTextureFromName", &Anima::AnimaTexturesManager::GetTextureFromName, return_value_policy<reference_existing_object>())
	.def("GetTexturesCount", &Anima::AnimaTexturesManager::GetTexturesCount)
	.def("LoadTextureFromBMPFile", &Anima::AnimaTexturesManager::LoadTextureFromBMPFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromDDSFile", &Anima::AnimaTexturesManager::LoadTextureFromDDSFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromTGAFile", &Anima::AnimaTexturesManager::LoadTextureFromTGAFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromFile", LoadTextureFromAnimaTextureFile, return_value_policy<reference_existing_object>())
	.def("LoadTextureFromFile", LoadTextureFromImageFile, return_value_policy<reference_existing_object>())
	.def("LoadTextures", &Anima::AnimaTexturesManager::LoadTextures)
	.def("SaveTextures", &Anima::AnimaTexturesManager::SaveTextures)
	.def("SaveTextureToFile", SaveTextureToFileWithName)
	.def("SaveTextureToFile", SaveTextureToFileWithTexture)
	;

	// AnimaDataGeneratorsManager
	class_<Anima::AnimaDataGeneratorsManager>("AnimaDataGeneratorsManager", no_init)
	.def("CreateColorGenerator", &Anima::AnimaDataGeneratorsManager::CreateColorGenerator, return_value_policy<reference_existing_object>())
	.def("CreateTextureGenerator", &Anima::AnimaDataGeneratorsManager::CreateTextureGenerator, return_value_policy<reference_existing_object>())
	.def("CreateVectorGenerator", &Anima::AnimaDataGeneratorsManager::CreateVectorGenerator, return_value_policy<reference_existing_object>())
	.def("GetGenerator", &Anima::AnimaDataGeneratorsManager::GetGenerator, return_value_policy<reference_existing_object>())
	.def("RemoveGenerator", RemoveGeneratorWithName)
	.def("RemoveGenerator", RemoveGeneratorWithGenerator)
	;

	// AnimaNamedObject
	class_<Anima::AnimaNamedObject>("AnimaNamedObject", init<const Anima::AnimaString&, Anima::AnimaAllocator*>())
	.def("GetName", &Anima::AnimaNamedObject::GetName)
	.def("SetName", &Anima::AnimaNamedObject::SetName)
	;

	// AnimaMappedValues
	class_<Anima::AnimaMappedValues, bases<Anima::AnimaNamedObject> >("AnimaMappedValues", no_init)
	.def("AddTexture", AMV_AddTexture1)
	.def("AddTexture", AMV_AddTexture2)
	.def("SetTexture", AMV_SetTexture1)
	.def("SetTexture", AMV_SetTexture2)
	.def("GetTexture", &Anima::AnimaMappedValues::GetTexture, return_value_policy<reference_existing_object>())
	.def("HasTexture", &Anima::AnimaMappedValues::HasTexture)
	.def("AddColor", AMV_AddColor1)
	.def("AddColor", AMV_AddColor2)
	.def("AddColor", AMV_AddColor3)
	.def("AddColor", AMV_AddColor4)
	.def("AddColor", AMV_AddColor5)
	.def("SetColor", AMV_SetColor1)
	.def("SetColor", AMV_SetColor2)
	.def("SetColor", AMV_SetColor3)
	.def("SetColor", AMV_SetColor4)
	.def("SetColor", AMV_SetColor5)
	.def("GetColor3f", &Anima::AnimaMappedValues::GetColor3f)
	.def("GetColor4f", &Anima::AnimaMappedValues::GetColor4f)
	.def("HasColor", &Anima::AnimaMappedValues::HasColor)
	.def("AddVector", AMV_AddVector1)
	.def("AddVector", AMV_AddVector2)
	.def("AddVector", AMV_AddVector3)
	.def("AddVector", AMV_AddVector4)
	.def("AddVector", AMV_AddVector5)
	.def("AddVector", AMV_AddVector6)
	.def("AddVector", AMV_AddVector7)
	.def("SetVector", AMV_SetVector1)
	.def("SetVector", AMV_SetVector2)
	.def("SetVector", AMV_SetVector3)
	.def("SetVector", AMV_SetVector4)
	.def("SetVector", AMV_SetVector5)
	.def("SetVector", AMV_SetVector6)
	.def("SetVector", AMV_SetVector7)
	.def("GetVector2f", &Anima::AnimaMappedValues::GetVector2f)
	.def("GetVector3f", &Anima::AnimaMappedValues::GetVector3f)
	.def("GetVector4f", &Anima::AnimaMappedValues::GetVector4f)
	.def("HasVector", &Anima::AnimaMappedValues::HasVector)
	.def("AddVectorArray", AMV_AddVectorArray1)
	.def("AddVectorArray", AMV_AddVectorArray2)
	.def("AddVectorArray", AMV_AddVectorArray3)
	.def("AddVectorArray", AMV_AddVectorArray4)
	.def("SetVectorArray", AMV_SetVectorArray1)
	.def("SetVectorArray", AMV_SetVectorArray2)
	.def("SetVectorArray", AMV_SetVectorArray3)
	.def("SetVectorArray", AMV_SetVectorArray4)
	.def("GetVectorArray", &Anima::AnimaMappedValues::GetVectorArray, return_value_policy<reference_existing_object>())
	.def("HasVectorArray", &Anima::AnimaMappedValues::HasVectorArray)
	.def("AddFloat", &Anima::AnimaMappedValues::AddFloat)
	.def("SetFloat", &Anima::AnimaMappedValues::SetFloat)
	.def("GetFloat", &Anima::AnimaMappedValues::GetFloat)
	.def("HasFloat", &Anima::AnimaMappedValues::HasFloat)
	.def("AddBoolean", &Anima::AnimaMappedValues::AddBoolean)
	.def("SetBoolean", &Anima::AnimaMappedValues::SetBoolean)
	.def("GetBoolean", &Anima::AnimaMappedValues::GetBoolean)
	.def("HasBoolean", &Anima::AnimaMappedValues::HasBoolean)
	.def("AddInteger", &Anima::AnimaMappedValues::AddInteger)
	.def("SetInteger", &Anima::AnimaMappedValues::SetInteger)
	.def("GetInteger", &Anima::AnimaMappedValues::GetInteger)
	.def("HasInteger", &Anima::AnimaMappedValues::HasInteger)
	.def("AddMatrix", AMV_AddMatrix1)
	.def("AddMatrix", AMV_AddMatrix2)
	.def("SetMatrix", AMV_SetMatrix1)
	.def("SetMatrix", AMV_SetMatrix2)
	.def("GetMatrix", &Anima::AnimaMappedValues::GetMatrix)
	.def("HasMatrix", &Anima::AnimaMappedValues::HasMatrix)
	.def("AddMatrixArray", &Anima::AnimaMappedValues::AddMatrixArray)
	.def("SetMatrixArray", &Anima::AnimaMappedValues::SetMatrixArray)
	.def("GetMatrixArray", &Anima::AnimaMappedValues::GetMatrixArray, return_value_policy<reference_existing_object>())
	.def("HasMatrixArray", &Anima::AnimaMappedValues::HasMatrixArray)
	;
		
//	// AnimaScene
//	class_<Anima::AnimaScene>("AnimaScene", no_init)
//		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
//		.def("GetNodesManager", &Anima::AnimaScene::GetNodesManager, return_value_policy<reference_existing_object>())
//		.def("GetGeometriesManager", &Anima::AnimaScene::GetGeometriesManager, return_value_policy<reference_existing_object>())
//		.def("GetNodeInstancesManager", &Anima::AnimaScene::GetNodeInstancesManager, return_value_policy<reference_existing_object>())
//		.def("GetGeometryInstancesManager", &Anima::AnimaScene::GetGeometryInstancesManager, return_value_policy<reference_existing_object>())
//		.def("GetShadersManager", &Anima::AnimaScene::GetShadersManager, return_value_policy<reference_existing_object>())
//		.def("GetCamerasManager", &Anima::AnimaScene::GetCamerasManager, return_value_policy<reference_existing_object>())
//		.def("GetTexturesManager", &Anima::AnimaScene::GetTexturesManager, return_value_policy<reference_existing_object>())
//		.def("GetDataGeneratorsManager", &Anima::AnimaScene::GetDataGeneratorsManager, return_value_policy<reference_existing_object>())
//		.def("GetMaterialsManager", &Anima::AnimaScene::GetMaterialsManager, return_value_policy<reference_existing_object>())
//		.def("GetLightsManager", &Anima::AnimaScene::GetLightsManager, return_value_policy<reference_existing_object>())
//		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
//		.def("GetAnimationsManager", &Anima::AnimaScene::GetAnimationsManager, return_value_policy<reference_existing_object>())
//		.def("GetGeometriesAllocator", &Anima::AnimaScene::GetGeometriesAllocator, return_value_policy<reference_existing_object>())
//		.def("GetNodeInstancesAllocator", &Anima::AnimaScene::GetNodeInstancesAllocator, return_value_policy<reference_existing_object>())
//		.def("GetGeometryInstancesAllocator", &Anima::AnimaScene::GetGeometryInstancesAllocator, return_value_policy<reference_existing_object>())
//		.def("GetShadersAllocator", &Anima::AnimaScene::GetShadersAllocator, return_value_policy<reference_existing_object>())
//		.def("GetCamerasAllocator", &Anima::AnimaScene::GetCamerasAllocator, return_value_policy<reference_existing_object>())
//		.def("GetTexturesAllocator", &Anima::AnimaScene::GetTexturesAllocator, return_value_policy<reference_existing_object>())
//		.def("GetDataGeneratorsAllocator", &Anima::AnimaScene::GetDataGeneratorsAllocator, return_value_policy<reference_existing_object>())
//		.def("GetMaterialsAllocator", &Anima::AnimaScene::GetMaterialsAllocator, return_value_policy<reference_existing_object>())
//		.def("GetLightsAllocator", &Anima::AnimaScene::GetLightsAllocator, return_value_policy<reference_existing_object>())
//		.def("GetEngine", &Anima::AnimaScene::GetEngine, return_value_policy<reference_existing_object>())
//		.def("GetAnimationsAllocator", &Anima::AnimaScene::GetAnimationsAllocator, return_value_policy<reference_existing_object>());

//	// AnimaAnimationsManager
//	class_<Anima::AnimaAnimationsManager>("AnimaAnimationsManager", no_init)
//		.def("LoadAnimations", &Anima::AnimaAnimationsManager::LoadAnimations)
//		.def("GetAnimationsCount", &Anima::AnimaAnimationsManager::GetAnimationsCount)
//		.def("GetAnimation", &Anima::AnimaAnimationsManager::GetAnimation, return_value_policy<reference_existing_object>())
//		.def("GetAnimationFromName", GetAnimationFromNameString, return_value_policy<reference_existing_object>())
//		.def("GetLastLoadedAnimations", &Anima::AnimaAnimationsManager::GetLastLoadedAnimations, return_value_policy<reference_existing_object>())
//		.def("ClearAnimations", &Anima::AnimaAnimationsManager::ClearAnimations)
//		.def("ClearLastLoadedAnimations", &Anima::AnimaAnimationsManager::ClearLastLoadedAnimations);
//
//	// AnimaAnimation
//	class_<Anima::AnimaAnimation>("AnimaAnimation", no_init)
//		.def("UpdateAnimation", &Anima::AnimaAnimation::UpdateAnimation);
//
//	// AnimaNode
//	class_<Anima::AnimaNode>("AnimaNode", init<const Anima::AnimaString&, Anima::AnimaDataGeneratorsManager*, Anima::AnimaAllocator*>())
//		.def("SetMaterial", &Anima::AnimaNode::SetMaterial)
//		.def("GetMaterial", &Anima::AnimaNode::GetMaterial, return_value_policy<reference_existing_object>())
//		.def("GetGeometriesCount", &Anima::AnimaNode::GetGeometriesCount)
//		.def("AddGeometry", &Anima::AnimaNode::AddGeometry)
//		.def("GetGeometry", &Anima::AnimaNode::GetGeometry, return_value_policy<reference_existing_object>())
//		.def("GetGeometryFromName", GetGeometryFromNameString, return_value_policy<reference_existing_object>())
//		.def("GetAnimationsCount", &Anima::AnimaNode::GetAnimationsCount)
//		.def("SetAnimations", &Anima::AnimaNode::SetAnimations)
//		.def("AddAnimation", &Anima::AnimaNode::AddAnimation)
//		.def("GetAnimation", &Anima::AnimaNode::GetAnimation, return_value_policy<reference_existing_object>())
//		.def("SetOriginFileName", SetOriginFileNameString)
//		.def("GetAnimaOriginFileName", &Anima::AnimaNode::GetAnimaOriginFileName)
//		.def("SetAnimationNodeName", SetAnimationNodeNameString)
//		.def("GetAnimaAnimationNodeName", &Anima::AnimaNode::GetAnimaAnimationNodeName)
//		.def("GetGeometriesBonesInfo", &Anima::AnimaNode::GetGeometriesBonesInfo, return_value_policy<reference_existing_object>())
//		.def("SetGeometriesBonesInfo", &Anima::AnimaNode::SetGeometriesBonesInfo)
//		.def("ClearGeometriesBonesInfo", &Anima::AnimaNode::ClearGeometriesBonesInfo)
//		.def("UpdateAnimation", &Anima::AnimaNode::UpdateAnimation)
//		.def("SetActiveAnimation", &Anima::AnimaNode::SetActiveAnimation)
//		.def("StopAnimation", &Anima::AnimaNode::StopAnimation);
//
//	// AnimaRenderer
//	class_<Anima::AnimaRenderer, boost::noncopyable>("AnimaRenderer", init<Anima::AnimaEngine*, Anima::AnimaAllocator*>())
//		.def("Start", RendererStart)
//		.def("DrawAll", &Anima::AnimaRenderer::Render)
//		//.def("AddPrimitive", &Anima::AnimaRenderer::AddPrimitive)
//		.def("InitTextureSlots", &Anima::AnimaRenderer::InitTextureSlots)
//		.def("InitRenderingTargets", &Anima::AnimaRenderer::InitRenderingTargets)
//		.def("InitRenderingUtilities", &Anima::AnimaRenderer::InitRenderingUtilities);
//
//	// AnimaShadersManager
//	class_<Anima::AnimaShadersManager>("AnimaShadersManager", no_init)
//		.def("LoadShader", LoadShaderStringString, return_value_policy<reference_existing_object>())
//		.def("LoadShaderFromFile", LoadShaderFromFileStringString, return_value_policy<reference_existing_object>())
//		.def("CreateShader", CreateShaderString, return_value_policy<reference_existing_object>())
//		.def("CreateProgram", CreateProgramString, return_value_policy<reference_existing_object>())
//		.def("GetProgram", &Anima::AnimaShadersManager::GetProgram, return_value_policy<reference_existing_object>())
//		.def("GetProgramFromName", GetProgramFromNameString, return_value_policy<reference_existing_object>())
//		.def("SetActiveProgram", &Anima::AnimaShadersManager::SetActiveProgram, return_value_policy<reference_existing_object>())
//		.def("SetActiveProgramFromName", SetActiveProgramFromNameString, return_value_policy<reference_existing_object>())
//		.def("GetActiveProgram", &Anima::AnimaShadersManager::GetActiveProgram, return_value_policy<reference_existing_object>());
//
//	// AnimaShader
//	enum_<Anima::AnimaShaderType>("AnimaShaderType")
//		.value("INVALID", Anima::INVALID)
//		.value("VERTEX", Anima::VERTEX)
//		.value("TESSELLATION_CONTROL", Anima::TESSELLATION_CONTROL)
//		.value("TESSELLATION_EVALUATION", Anima::TESSELLATION_EVALUATION)
//		.value("GEOMETRY", Anima::GEOMETRY)
//		.value("FRAGMENT", Anima::FRAGMENT);
//
//	class_<Anima::AnimaShader>("AnimaShader", no_init);
//
//	// AnimaShaderProgram
//	enum_<Anima::AnimaShaderInfoType>("AnimaShaderInfoType")
//		.value("SHADER_FILE", Anima::SHADER_FILE)
//		.value("SHADER_TEXT", Anima::SHADER_TEXT);
//
//	class_<Anima::AnimaShaderProgram>("AnimaShaderProgram", no_init)
//		.def("AddShader", &Anima::AnimaShaderProgram::AddShader)
//		.def("SetShaders", &Anima::AnimaShaderProgram::SetShaders)
//		.def("CompileShaders", &Anima::AnimaShaderProgram::CompileShaders)
//		.def("ClearShaders", &Anima::AnimaShaderProgram::ClearShaders)
//		.def("Create", &Anima::AnimaShaderProgram::Create)
//		.def("Link", &Anima::AnimaShaderProgram::Link)
//		.def("Use", &Anima::AnimaShaderProgram::Use)
//		.def("Delete", &Anima::AnimaShaderProgram::Delete)
//		.def("IsCreated", &Anima::AnimaShaderProgram::IsCreated)
//		.def("IsLinked", &Anima::AnimaShaderProgram::IsLinked)
//		.def("GetID", &Anima::AnimaShaderProgram::GetID);
//
//	// AnimaCamerasManager
//	class_<Anima::AnimaCamerasManager>("AnimaCamerasManager", no_init)
//		.def("CreateCamera", CreateCameraString, return_value_policy<reference_existing_object>())
//		.def("GetActiveCamera", &Anima::AnimaCamerasManager::GetActiveCamera, return_value_policy<reference_existing_object>())
//		.def("GetCameraFromName", GetCameraFromNameString, return_value_policy<reference_existing_object>())
//		.def("GetCamerasCount", &Anima::AnimaCamerasManager::GetCamerasCount)
//		.def("UpdatePerspectiveCameras", &Anima::AnimaCamerasManager::UpdatePerspectiveCameras)
//		.def("UpdateOrthoCameras", &Anima::AnimaCamerasManager::UpdateOrthoCameras);

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