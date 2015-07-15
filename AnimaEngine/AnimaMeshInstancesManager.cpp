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
#include "AnimaTimer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMeshInstancesManager::AnimaMeshInstancesManager(AnimaScene* scene, AnimaMeshesManager* meshesManager)
{
	ANIMA_ASSERT(scene != nullptr);
	ANIMA_ASSERT(meshesManager != nullptr);

	_scene = scene;
	_meshesManager = meshesManager;
}

AnimaMeshInstancesManager::~AnimaMeshInstancesManager()
{
	ClearInstances();
	ClearLastInstancesFromModel();
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, AnimaMesh* srcMesh)
{
	AInt index = _meshIntances.Contains(instanceName);
	if (index >= 0)
		return nullptr;

	AnimaMaterial* oldMaterial = srcMesh->GetMaterial();
	AnimaMaterial* newMaterial = _scene->GetMaterialsManager()->CreateMaterial(instanceName + ".material");
	newMaterial->CopyData(*oldMaterial);

	for (AInt ns = 0; ns < oldMaterial->GetShadersCount(); ns++)
		newMaterial->AddShader(oldMaterial->GetShaderName(ns));

	AnimaMeshInstance* meshInstance = AnimaAllocatorNamespace::AllocateNew<AnimaMeshInstance>(*(_scene->GetMeshInstancesAllocator()), instanceName, _scene->GetDataGeneratorsManager(), _scene->GetMeshInstancesAllocator());
	meshInstance->CopyData(*srcMesh);
	meshInstance->SetMaterial(newMaterial);
	meshInstance->SetMesh(srcMesh);

	for (AInt ns = 0; ns < srcMesh->GetShadersCount(); ns++)
		meshInstance->AddShader(srcMesh->GetShaderName(ns));

	srcMesh->AddInstance(meshInstance);

	_meshIntances.Add(instanceName, meshInstance);

	return meshInstance;
}

AnimaMeshInstance* AnimaMeshInstancesManager::CreateInstance(const AnimaString& instanceName, const AnimaString& srcMeshName)
{
	AnimaMesh* srcMesh = _meshesManager->GetMeshFromName(srcMeshName);
	if (srcMesh == nullptr)
		return nullptr;
	return CreateInstance(instanceName, srcMesh);
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

AnimaArray<AnimaMeshInstance*>* AnimaMeshInstancesManager::CreateInstances(AnimaModel* srcModel)
{
	ClearLastInstancesFromModel();

	if (srcModel == nullptr)
		return nullptr;
	
	AInt modelMeshesCount = srcModel->GetMeshesCount();
	for (AInt i = 0; i < modelMeshesCount; i++)
	{
		AnimaMesh* mesh = srcModel->GetMesh(i);

		AnimaString meshName = mesh->GetName();
		AnimaString meshInstanceName;
		AnimaMeshInstance* newInstance = nullptr;
		int nameOffset = 0;
		while (newInstance == nullptr)
		{
			if (nameOffset > 0)
				meshInstanceName = FormatString("%s.instance%d", meshName.c_str(), i + nameOffset);
			else
				meshInstanceName = FormatString("%s.instance%d", meshName.c_str(), i);

			newInstance = CreateEmptyInstance(meshInstanceName);
			nameOffset++;
		}
		
		AnimaMaterial* oldMaterial = mesh->GetMaterial();
		AnimaMaterial* newMaterial = _scene->GetMaterialsManager()->CreateMaterial(meshInstanceName + ".material");
		newMaterial->CopyData(*oldMaterial);
		for (AInt ns = 0; ns < oldMaterial->GetShadersCount(); ns++)
			newMaterial->AddShader(oldMaterial->GetShaderName(ns));

		newInstance->CopyData(*mesh);
		newInstance->SetMaterial(newMaterial);
		newInstance->SetMesh(mesh);

		for (AInt ns = 0; ns < mesh->GetShadersCount(); ns++)
			newInstance->AddShader(mesh->GetShaderName(ns));

		mesh->AddInstance(newInstance);

		_lastInstancesFromModel.push_back(newInstance);
	}

	return &_lastInstancesFromModel;
}

AInt AnimaMeshInstancesManager::GetMeshInstancesCount()
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
	_lastInstancesFromModel.clear();
}

END_ANIMA_ENGINE_NAMESPACE