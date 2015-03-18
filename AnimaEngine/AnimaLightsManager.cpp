//
//  AnimaLightsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaLightsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaLightsManager::AnimaLightsManager(AnimaScene* scene)
{
	_scene = scene;
}

AnimaLightsManager::~AnimaLightsManager()
{
	ClearLights();
}

AnimaAmbientLight* AnimaLightsManager::CreateAmbientLight(const AnimaString& name)
{
	return CreateLight<AnimaAmbientLight>(name);
}

AnimaAmbientLight* AnimaLightsManager::CreateAmbientLight(const char* name)
{
	return CreateLight<AnimaAmbientLight>(name);
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const AnimaString& name)
{
	return CreateLight<AnimaDirectionalLight>(name);
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const char* name)
{
	return CreateLight<AnimaDirectionalLight>(name);
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const AnimaString& name)
{
	return CreateLight<AnimaPointLight>(name);
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const char* name)
{
	return CreateLight<AnimaPointLight>(name);
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const AnimaString& name)
{
	return CreateLight<AnimaSpotLight>(name);
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const char* name)
{
	return CreateLight<AnimaSpotLight>(name);
}

void AnimaLightsManager::ClearLights()
{
	for (auto pair : _lightsMap)
	{
		if (pair.second != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetLightsAllocator()), pair.second);
			pair.second = nullptr;
		}
	}

	_lightsMap.clear();
}

AnimaLight* AnimaLightsManager::GetLightFromName(const AnimaString& name)
{
	auto namesPair = _lightsName.find(name);

	if (namesPair != _lightsMap.end())
	{
		boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>* lightsMap = namesPair->second->GetLightsMap();
		AnimaArray<AnimaLight*, AnimaLight*>* lightsArray = namesPair->second->GetLightsArray();

		auto lightsPair = lightsMap->find(name);

		if (lightsPair != lightsMap->end())
			return lightsArray->ElementAt(lightsPair->second);
	}

	return nullptr;
}

AnimaLight* AnimaLightsManager::GetLightFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetLightFromName(str);
}

AInt AnimaLightsManager::GetTotalLightsCount()
{
	AInt count = 0;
	for (auto pair : _lightsMap)
	{
		if (pair.second != nullptr)
			count += pair.second->GetLightsArray()->GetSize();
	}

	return count;
}

boost::unordered_map<AnimaString, AnimaLightsMapData*, AnimaString::Hasher>* AnimaLightsManager::GetLightsMap()
{
	return &_lightsMap;
}

AnimaLightsMapData::AnimaLightsMapData(AnimaAllocator* allocator)
	: _lights(allocator)
{
	_allocator = allocator;
}

AnimaLightsMapData::AnimaLightsMapData(AnimaLightsMapData& src)
	: _lights(src._allocator)
{
	_allocator = src._allocator;
	_lights = src._lights;
	_lightsMap = src._lightsMap;
}

AnimaLightsMapData::AnimaLightsMapData(AnimaLightsMapData&& src)
	: _lights(src._allocator)
{
	_allocator = src._allocator;
	_lights = src._lights;
	_lightsMap = src._lightsMap;
}

AnimaLightsMapData::~AnimaLightsMapData()
{
	for (AInt i = 0; i < _lights.GetSize(); i++)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _lights[i]);
		_lights[i] = nullptr;
	}

	_lights.RemoveAll();
	_lightsMap.clear();
}

AnimaLightsMapData& AnimaLightsMapData::operator=(AnimaLightsMapData& src)
{
	if (this != &src)
	{
		_lights = src._lights;
		_lightsMap = src._lightsMap;
	}

	return *this;
}

AnimaLightsMapData& AnimaLightsMapData::operator=(AnimaLightsMapData&& src)
{
	if (this != &src)
	{
		_lights = src._lights;
		_lightsMap = src._lightsMap;
	}

	return *this;
}

AnimaArray<AnimaLight*, AnimaLight*>* AnimaLightsMapData::GetLightsArray()
{
	return &_lights;
}

boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>* AnimaLightsMapData::GetLightsMap()
{
	return &_lightsMap;
}

END_ANIMA_ENGINE_NAMESPACE