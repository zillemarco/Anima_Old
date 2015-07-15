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
#include "AnimaTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelInstancesManager::AnimaModelInstancesManager(AnimaScene* scene, AnimaModelsManager* modelsManager, AnimaMeshInstancesManager* meshInstancesManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(modelsManager != nullptr);
	ANIMA_ASSERT(meshInstancesManager != nullptr);

	_scene = scene;
	_modelsManager = modelsManager;
	_meshInstancesManager = meshInstancesManager;
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

	AnimaModelInstance* modelInstance = CreateInstanceFromModel(instanceName, srcModel);
	_topLevelModelInstances.Add(instanceName, modelInstance);

	return modelInstance;
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstance(const AnimaString& instanceName, const AnimaString& srcModelName)
{
	AnimaModel* srcModel = _modelsManager->GetModelFromName(srcModelName);
	if (srcModel == nullptr)
		return nullptr;
	return CreateInstance(instanceName, srcModel);
}

AnimaModelInstance* AnimaModelInstancesManager::CreateInstanceFromModel(const AnimaString& instanceName, AnimaModel* srcModel, bool useSrcModelName)
{
	if (srcModel == nullptr)
		return nullptr;

	AnimaString completeInstanceName;
	if (useSrcModelName)
		completeInstanceName = srcModel->GetName() + "." + instanceName;
	else
		completeInstanceName = instanceName;
	AnimaString modelName = srcModel->GetName();
	AnimaString modelInstanceName;
	AnimaModelInstance* newInstance = nullptr;
	int nameOffset = 0;
	while (newInstance == nullptr)
	{
		if (nameOffset > 0)
		{
			if (useSrcModelName)
				modelInstanceName = FormatString("%s.%s%d", modelName.c_str(), instanceName.c_str(), nameOffset);
			else
				modelInstanceName = FormatString("%s%d", instanceName.c_str(), nameOffset);
		}
		else
		{
			if (useSrcModelName)
				modelInstanceName = FormatString("%s.%s", modelName.c_str(), instanceName.c_str());
			else
				modelInstanceName = FormatString("%s", instanceName.c_str());
		}

		newInstance = CreateEmptyInstance(modelInstanceName);
		nameOffset++;
	}

	newInstance->CopyData(*srcModel);
	newInstance->SetMeshes(_meshInstancesManager->CreateInstances(srcModel));

	AnimaTimer timer;
	AInt childrenCount = srcModel->GetChildrenCount();
	for (AInt i = 0; i < childrenCount; i++)
	{
		AnimaModelInstance* newChild = CreateInstanceFromModel(instanceName, (AnimaModel*)srcModel->GetChild(i), true);
		newInstance->AddChild(newChild);
	}
	timer.PrintElapsed();

	newInstance->UpdateChildrenTransformation();

	return newInstance;
}

AnimaModelInstance* AnimaModelInstancesManager::CreateEmptyInstance(const AnimaString& instanceName)
{
	AInt index = _modelIntances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaModelInstance* modelInstance = AnimaAllocatorNamespace::AllocateNew<AnimaModelInstance>(*(_scene->GetModelInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetModelInstancesAllocator());
	_modelIntances.Add(instanceName, modelInstance);

	return modelInstance;
}

AInt AnimaModelInstancesManager::GetModelInstancesCount()
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