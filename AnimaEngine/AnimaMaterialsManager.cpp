//
//  AnimaMaterialsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaMaterialsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMaterialsManager::AnimaMaterialsManager(AnimaEngine* engine)
{
	_engine = engine;
	
	_materials = nullptr;
	_materialsNumber = 0;
}

AnimaMaterialsManager::~AnimaMaterialsManager()
{
	ClearMaterials();
}

AnimaMaterial* AnimaMaterialsManager::CreateMaterial(const AnimaString& name)
{
	if (_materialsMap.find(name) != _materialsMap.end())
		return nullptr;

	ANIMA_ASSERT(_engine != nullptr);
	if (_materialsNumber > 0)
	{
		AnimaMaterial** tmpOldMaterials = AnimaAllocatorNamespace::AllocateArray<AnimaMaterial*>(*(_engine->GetGenericAllocator()), _materialsNumber);

		for (int i = 0; i < _materialsNumber; i++)
			tmpOldMaterials[i] = _materials[i];

		ClearMaterials(false, false);

		_materialsNumber++;
		_materials = AnimaAllocatorNamespace::AllocateArray<AnimaMaterial*>(*(_engine->GetGenericAllocator()), _materialsNumber);

		for (int i = 0; i < _materialsNumber - 1; i++)
			_materials[i] = tmpOldMaterials[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), tmpOldMaterials);
		tmpOldMaterials = nullptr;
	}
	else
	{
		_materialsNumber++;
		_materials = AnimaAllocatorNamespace::AllocateArray<AnimaMaterial*>(*(_engine->GetGenericAllocator()), _materialsNumber);
	}

	_materials[_materialsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaMaterial>(*(_engine->GetGenericAllocator()), _engine);

	_materialsMap[name] = (AUint)(_materialsNumber - 1);

	return (AnimaMaterial*)_materials[_materialsNumber - 1];
}

AnimaMaterial* AnimaMaterialsManager::CreateMaterial(const char* name)
{
	AnimaString str(name, _engine);
	return CreateMaterial(str);
}

void AnimaMaterialsManager::ClearMaterials(bool bDeleteObjects, bool bResetNumber)
{
	if (_materials != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_materialsNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetGenericAllocator()), _materials[i]);
				_materials[i] = nullptr;
			}
		}
		
		AnimaAllocatorNamespace::DeallocateArray<AnimaMaterial*>(*(_engine->GetGenericAllocator()), _materials);
		_materials = nullptr;
	}
	
	if (bResetNumber)
		_materialsNumber = 0;
}

AnimaMaterial* AnimaMaterialsManager::GetMaterial(AUint index)
{
	ANIMA_ASSERT(index >= 0 && index < _materialsNumber);
	return _materials[index];
}

AnimaMaterial* AnimaMaterialsManager::GetMaterial(const AnimaString& name)
{
	if (_materialsMap.find(name) == _materialsMap.end())
		return nullptr;
	return GetMaterial(_materialsMap[name]);
}

AnimaMaterial* AnimaMaterialsManager::GetMaterial(const char* name)
{
	AnimaString str(name, _engine);
	return GetMaterial(str);
}

END_ANIMA_ENGINE_NAMESPACE