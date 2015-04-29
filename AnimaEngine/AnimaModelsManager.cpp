//
//  AnimaEngineModelsManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaBenchmarkTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelsManager::AnimaModelsManager(AnimaScene* scene, AnimaMeshesManager* meshesManager, AnimaMaterialsManager* materialsManager)
: _models(scene->GetModelsAllocator())
, _topLevelModels(scene->GetModelsAllocator())
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(meshesManager != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);

	_scene = scene;
	_meshesManager = meshesManager;
	_materialsManager = materialsManager;
}

AnimaModelsManager::~AnimaModelsManager()
{
	ClearModels();
}

AnimaModel* AnimaModelsManager::LoadModel(const char* modelPath, const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadModel(modelPath, str);
}

AnimaModel* AnimaModelsManager::LoadModel(const char* modelPath, const AnimaString& name)
{
	AnimaString str(modelPath, _scene->GetStringAllocator());
	return LoadModel(str, name);
}

AnimaModel* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return LoadModel(modelPath, str);
}

AnimaModel* AnimaModelsManager::LoadModel(const AnimaString& modelPath, const AnimaString& name)
{
	AInt index = _topLevelModels.Contains(name);
	if (index >= 0)
		return nullptr;
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelPath.GetConstBuffer(), aiProcessPreset_TargetRealtime_Quality);
	
	if (scene == nullptr)
		return nullptr;
	
	AnimaModel* newTopLevelModel = nullptr;

	_materialsManager->LoadMaterialsFromModel(scene, name);
	AnimaArray<AnimaString*>* materialNames = _materialsManager->GetLastMaterialsIndexMap();

	if (_meshesManager->LoadMeshesFromModel(scene, name, materialNames))
	{
		AnimaArray<AnimaString*>* meshesName = _meshesManager->GetLastMeshesIndexMap();		
		newTopLevelModel = LoadModelFromScene(scene, scene->mRootNode, meshesName, name);
		
		newTopLevelModel->SetOriginFileName(modelPath);
		_topLevelModels.Add(name, newTopLevelModel);
	}

	importer.FreeScene();
	return newTopLevelModel;
}

AnimaModel* AnimaModelsManager::LoadModelFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString*>* meshesMap, const AnimaString& modelName)
{
	AnimaString newModelName(_scene->GetStringAllocator());
	if (!modelName.IsEmpty())
		newModelName = modelName;
	else
		newModelName = sceneNode->mName.C_Str();

	AnimaModel* currentModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), newModelName, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_models.Add(currentModel);

	AnimaMatrix modelMatrix;
	modelMatrix.m[0] = sceneNode->mTransformation.a1;	modelMatrix.m[1] = sceneNode->mTransformation.a2;	modelMatrix.m[2] = sceneNode->mTransformation.a3;	modelMatrix.m[3] = sceneNode->mTransformation.a4;
	modelMatrix.m[4] = sceneNode->mTransformation.b1;	modelMatrix.m[5] = sceneNode->mTransformation.b2;	modelMatrix.m[6] = sceneNode->mTransformation.b3;	modelMatrix.m[7] = sceneNode->mTransformation.b4;
	modelMatrix.m[8] = sceneNode->mTransformation.c1;	modelMatrix.m[9] = sceneNode->mTransformation.c2;	modelMatrix.m[10] = sceneNode->mTransformation.c3;	modelMatrix.m[11] = sceneNode->mTransformation.c4;
	modelMatrix.m[12] = sceneNode->mTransformation.d1;	modelMatrix.m[13] = sceneNode->mTransformation.d2;	modelMatrix.m[14] = sceneNode->mTransformation.d3;	modelMatrix.m[15] = sceneNode->mTransformation.d4;

	currentModel->GetTransformation()->SetTransformationMatrix(modelMatrix.Transposed());
	
	for (AUint n = 0; n < sceneNode->mNumMeshes; n++)
	{
		AInt meshIndex = (AInt)sceneNode->mMeshes[n];
		AnimaString* meshName = meshesMap->GetAt(meshIndex);
		AnimaMesh* mesh = _meshesManager->GetMeshFromName(*meshName);
		currentModel->AddMesh(mesh);
	}

	for (AUint n = 0; n < sceneNode->mNumChildren; n++)
	{
		AnimaModel* child = LoadModelFromScene(scene, sceneNode->mChildren[n], meshesMap, AnimaString());
		currentModel->AddChild(child);
	}

	return currentModel;
}

AnimaModel* AnimaModelsManager::CreateModel(const AnimaString& name)
{
	AInt index = _topLevelModels.Contains(name);
	if (index >= 0)
		return nullptr;

	AnimaModel* newModel = AnimaAllocatorNamespace::AllocateNew<AnimaModel>(*(_scene->GetModelsAllocator()), name, _scene->GetDataGeneratorsManager(), _scene->GetModelsAllocator());
	_topLevelModels.Add(name, newModel);
	return newModel;
}

AnimaModel* AnimaModelsManager::CreateModel(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateModel(str);
}

AInt AnimaModelsManager::GetModelsNumber()
{
	return _topLevelModels.GetSize();
}

AnimaModel* AnimaModelsManager::GetModel(AInt index)
{
	return _topLevelModels[index];
}

AnimaModel* AnimaModelsManager::GetModelFromName(const AnimaString& name)
{
	return _topLevelModels[name];
}

AnimaModel* AnimaModelsManager::GetModelFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetModelFromName(str);
}

void AnimaModelsManager::ClearModels()
{
	AInt count = _models.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaModel* model = _models[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetModelsAllocator()), model);
		model = nullptr;
	}

	_models.RemoveAll();
	_topLevelModels.RemoveAll();
}

END_ANIMA_ENGINE_NAMESPACE
