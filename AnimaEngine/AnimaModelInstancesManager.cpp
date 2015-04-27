//
//  AnimaModelInstancesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaModelInstancesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaBenchmarkTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelInstancesManager::AnimaModelInstancesManager(AnimaScene* scene, AnimaModelsManager* modelsManager, AnimaMaterialsManager* materialsManager)
: _modelIntances(scene->GetModelInstancesAllocator())
, _topLevelModelInstances(scene->GetModelInstancesAllocator())
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(modelsManager != nullptr);
	ANIMA_ASSERT(materialsManager != nullptr);

	_scene = scene;
	_modelsManager = modelsManager;
	_materialsManager = materialsManager;
}

AnimaModelInstancesManager::~AnimaModelInstancesManager()
{
	ClearInstances();
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const AnimaString& instanceName, AnimaModel* srcModel)
{
	AInt index = _topLevelModelInstances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaModelInstance* modelInstace = CreateInstanceFromModel(instanceName, srcModel);
	_topLevelModelInstances.Add(instanceName, modelInstace);

	return modelInstace;
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const char* instanceName, AnimaModel* srcModel)
{
	AnimaString str(instanceName, _scene->GetStringAllocator());
	return CreateInstance(str, srcModel);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const AnimaString& instanceName, const AnimaString& srcModelName)
{
	AnimaModel* srcModel = _modelsManager->GetModelFromName(srcModelName);
	if (srcModel == nullptr)
		return nullptr;
	return CreateInstance(instanceName, srcModel);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const char* instanceName, const AnimaString& srcModelName)
{
	AnimaString str(instanceName, _scene->GetStringAllocator());
	return CreateInstance(str, srcModelName);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const AnimaString& instanceName, const char* srcModelName)
{
	AnimaString str(srcModelName, _scene->GetStringAllocator());
	return CreateInstance(instanceName, str);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const char* instanceName, const char* srcModelName)
{
	AnimaString strInstanceName(instanceName, _scene->GetStringAllocator());
	AnimaString strModelName(srcModelName, _scene->GetStringAllocator());
	return CreateInstance(strInstanceName, strModelName);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstanceFromModel(const AnimaString& instanceName, AnimaModel* srcModel, bool useSrcModelName)
{
	if (srcModel == nullptr)
		return nullptr;

	AnimaString completeInstanceName(_scene->GetStringAllocator());
	completeInstanceName = instanceName;
	if (useSrcModelName)
		completeInstanceName += "." + srcModel->GetAnimaName();

	AnimaModelInstance* modelInstance = AnimaAllocatorNamespace::AllocateNew<AnimaModelInstance>(*(_scene->GetModelInstancesAllocator()), completeInstanceName, _scene->GetDataGeneratorsManager(), _scene->GetModelInstancesAllocator());
	modelInstance->CopyData(*srcModel);
	modelInstance->SetMeshes(srcModel->_meshes.GetArray());
	modelInstance->SetMaterial(srcModel->GetMaterial());

	AInt childrenNumber = srcModel->GetChildrenNumber();
	for (AInt i = 0; i < childrenNumber; i++)
	{
		AnimaModelInstance* newChild = CreateInstanceFromModel(instanceName, (AnimaModel*)srcModel->GetChild(i), true);
		modelInstance->AddChild(newChild);
	}

	_modelIntances.Add(modelInstance);
	return modelInstance;
}

AInt AnimaModelInstancesManager::GetModelInstancesNumber()
{
	return _topLevelModelInstances.GetSize();
}

AnimaModelInstance* AnimaModelInstancesManager::GetModelInstance(AInt index)
{
	return _topLevelModelInstances[index];
}

AnimaModelInstance* AnimaModelInstancesManager::GetModelInstanceFromName(const AnimaString& name)
{
	return _topLevelModelInstances[name];
}

AnimaModelInstance* AnimaModelInstancesManager::GetModelInstanceFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetModelInstanceFromName(str);
}

void AnimaModelInstancesManager::ClearInstances()
{
	AInt count = _modelIntances.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaModelInstance* instance = _modelIntances[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetModelInstancesAllocator()), instance);
		instance = nullptr;
	}

	_modelIntances.RemoveAll();
	_topLevelModelInstances.RemoveAll();
}

END_ANIMA_ENGINE_NAMESPACE