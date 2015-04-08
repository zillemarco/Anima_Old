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
	: _lights(scene->GetLightsAllocator())
{
	_scene = scene;
}

AnimaLightsManager::~AnimaLightsManager()
{
	ClearLights();
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
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = _lights.GetArraysMap();
	for (auto lightsPair : (*lightsMap))
	{
		AnimaMappedArray<AnimaLight*>* lightsArray = lightsPair.second;
		AInt count = lightsArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaLight* light = (*lightsArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetLightsAllocator()), light);
			light = nullptr;
		}
	}

	_lights.RemoveAll();
}

AnimaLight* AnimaLightsManager::GetLightFromName(const AnimaString& name)
{
	return _lights.GetWithName(name);
}

AnimaLight* AnimaLightsManager::GetLightFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetLightFromName(str);
}

AInt AnimaLightsManager::GetTotalLightsCount()
{
	AInt count = 0;
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = _lights.GetArraysMap();
	for (auto lightsPair : (*lightsMap))
		count += lightsPair.second->GetSize();

	return count;
}

AnimaTypeMappedArray<AnimaLight*>* AnimaLightsManager::GetLights()
{
	return &_lights;
}

void AnimaLightsManager::UpdateLightsMatrix(AnimaCamera* activeCamera)
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = _lights.GetArraysMap();
	for (auto lightsPair : (*lightsMap))
	{
		AnimaMappedArray<AnimaLight*>* lightsArray = lightsPair.second;
		AInt count = lightsArray->GetSize();
		for (AInt i = 0; i < count; i++)
			lightsArray->Get(i)->ComputeLightMatrix(activeCamera);
	}
}

END_ANIMA_ENGINE_NAMESPACE