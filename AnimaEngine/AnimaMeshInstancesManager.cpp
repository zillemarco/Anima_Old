//
//  AnimaMeshInstancesManager.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include "AnimaMeshInstancesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaBenchmarkTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshInstancesManager::AnimaMeshInstancesManager(AnimaScene* scene, AnimaMeshesManager* meshesManager)
: _meshIntances(scene->GetMeshInstancesAllocator())
, _lastInstancesFromModel(scene->GetMeshInstancesAllocator())
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(meshesManager != nullptr);

	_scene = scene;
	_meshesManager = meshesManager;
}

AnimaMeshInstancesManager::~AnimaMeshInstancesManager()
{
	ClearInstances();
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, AnimaMesh* srcMesh)
{
	AInt index = _meshIntances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaMeshInstance* meshInstance = AnimaAllocatorNamespace::AllocateNew<AnimaMeshInstance>(*(_scene->GetMeshInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetMeshInstancesAllocator());
	meshInstance->CopyData(*srcMesh);
	meshInstance->SetMaterial(srcMesh->GetMaterial());
	meshInstance->SetMesh(srcMesh);
	srcMesh->AddInstance(meshInstance);

	_meshIntances.Add(instanceName, meshInstance);

	return meshInstance;
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const char* instanceName, AnimaMesh* srcMesh)
{
	AnimaString str(instanceName, _scene->GetStringAllocator());
	return CreateInstance(str, srcMesh);
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, const AnimaString& srcMeshName)
{
	AnimaMesh* srcMesh = _meshesManager->GetMeshFromName(srcMeshName);
	if (srcMesh == nullptr)
		return nullptr;
	return CreateInstance(instanceName, srcMesh);
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const char* instanceName, const AnimaString& srcMeshName)
{
	AnimaString str(instanceName, _scene->GetStringAllocator());
	return CreateInstance(str, srcMeshName);
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, const char* srcMeshName)
{
	AnimaString str(srcMeshName, _scene->GetStringAllocator());
	return CreateInstance(instanceName, str);
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const char* instanceName, const char* srcMeshName)
{
	AnimaString strInstanceName(instanceName, _scene->GetStringAllocator());
	AnimaString strModelName(srcMeshName, _scene->GetStringAllocator());
	return CreateInstance(strInstanceName, strModelName);
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateEmptyInstance(const AnimaString& instanceName)
{
	AInt index = _meshIntances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaMeshInstance* meshInstance = AnimaAllocatorNamespace::AllocateNew<AnimaMeshInstance>(*(_scene->GetMeshInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetMeshInstancesAllocator());
	_meshIntances.Add(instanceName, meshInstance);

	return meshInstance;
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateEmptyInstance(const char* instanceName)
{
	AnimaString str(instanceName, _scene->GetStringAllocator());
	return CreateEmptyInstance(str);
}

AnimaArray<AnimaMeshInstance*>* AnimaMeshInstancesManager::CreateInstances(AnimaModel* srcModel)
{
	ClearLastInstancesFromModel();

	if (srcModel == nullptr)
		return nullptr;
	
	AInt modelMeshesNumber = srcModel->GetMeshesCount();
	for (AInt i = 0; i < modelMeshesNumber; i++)
	{
		AnimaMesh* mesh = srcModel->GetMesh(i);

		AnimaString meshName = mesh->GetAnimaName();
		AnimaString meshInstanceName(_scene->GetStringAllocator());
		AnimaMeshInstance* newInstance = nullptr;
		int nameOffset = 0;
		while (newInstance == nullptr)
		{
			if (nameOffset > 0)
				meshInstanceName.Format("%s.instance%d", meshName.GetConstBuffer(), i + nameOffset);
			else
				meshInstanceName.Format("%s.instance%d", meshName.GetConstBuffer(), i);

			newInstance = CreateEmptyInstance(meshInstanceName);
			nameOffset++;
		}

		newInstance->CopyData(*mesh);
		newInstance->SetMaterial(mesh->GetMaterial());
		newInstance->SetMesh(mesh);
		mesh->AddInstance(newInstance);

		_lastInstancesFromModel.Add(newInstance);
	}

	return &_lastInstancesFromModel;
}

AInt AnimaMeshInstancesManager::GetMeshInstancesNumber()
{
	return _meshIntances.GetSize();
}

AnimaMeshInstance* AnimaMeshInstancesManager::GetMeshInstance(AInt index)
{
	return _meshIntances[index];
}

AnimaMeshInstance* AnimaMeshInstancesManager::GetMeshInstanceFromName(const AnimaString& name)
{
	return _meshIntances[name];
}

AnimaMeshInstance* AnimaMeshInstancesManager::GetMeshInstanceFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetMeshInstanceFromName(str);
}

void AnimaMeshInstancesManager::ClearInstances()
{
	AInt count = _meshIntances.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaMeshInstance* instance = _meshIntances[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetMeshInstancesAllocator()), instance);
		instance = nullptr;
	}

	_meshIntances.RemoveAll();
}

void AnimaMeshInstancesManager::ClearLastInstancesFromModel()
{
	_lastInstancesFromModel.RemoveAll();
}

END_ANIMA_ENGINE_NAMESPACE