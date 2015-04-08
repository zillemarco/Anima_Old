//
//  AnimaMaterialsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaMaterialsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMaterialsManager::AnimaMaterialsManager(AnimaScene* scene)
	: _materials(scene->GetMaterialsAllocator())
{
	_scene = scene;
}

AnimaMaterialsManager::~AnimaMaterialsManager()
{
	ClearMaterials();
}

AnimaMaterial* AnimaMaterialsManager::CreateGenericMaterial(const AnimaString& name)
{
	return CreateMaterial<AnimaMaterial>(name);
}

AnimaMaterial* AnimaMaterialsManager::CreateGenericMaterial(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateGenericMaterial(str);
}

void AnimaMaterialsManager::ClearMaterials()
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaMaterial*>*, AnimaString::Hasher>* materialsMap = _materials.GetArraysMap();
	for (auto materialsPair : (*materialsMap))
	{
		AnimaMappedArray<AnimaMaterial*>* materialsArray = materialsPair.second;
		AInt count = materialsArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaMaterial* material = (*materialsArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetMaterialsAllocator()), material);
			material = nullptr;
		}
	}
	
	_materials.RemoveAll();
}

AnimaMaterial* AnimaMaterialsManager::GetMaterialFromName(const AnimaString& name)
{
	return _materials.GetWithName(name);
}

AnimaMaterial* AnimaMaterialsManager::GetMaterialFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetMaterialFromName(str);
}

AInt AnimaMaterialsManager::GetTotalMaterialsCount()
{
	AInt count = 0;
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaMaterial*>*, AnimaString::Hasher>* materialsMap = _materials.GetArraysMap();
	for (auto materialsPair : (*materialsMap))
		count += materialsPair.second->GetSize();
	
	return count;
}

AnimaTypeMappedArray<AnimaMaterial*>* AnimaMaterialsManager::GetMaterials()
{
	return &_materials;
}

END_ANIMA_ENGINE_NAMESPACE