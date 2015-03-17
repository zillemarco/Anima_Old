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
	
//	_lights = nullptr;
//	_lightsNumber = 0;
}

AnimaLightsManager::~AnimaLightsManager()
{
	ClearLights();
}

AnimaAmbientLight* AnimaLightsManager::CreateAmbientLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	
	AnimaAmbientLight* newLight = AnimaAllocatorNamespace::AllocateNew<AnimaAmbientLight>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator(), _scene->GetDataGeneratorsManager(), name);

	AUint index = (AUint)_lights.Add(newLight);
	_lightsMap[name] = index;
	
	return (AnimaAmbientLight*)_lights[index];
}

AnimaAmbientLight* AnimaLightsManager::CreateAmbientLight(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateAmbientLight(str);
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	
	AnimaDirectionalLight* newLight = AnimaAllocatorNamespace::AllocateNew<AnimaDirectionalLight>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator(), _scene->GetDataGeneratorsManager(), name);
	
	AUint index = (AUint)_lights.Add(newLight);
	_lightsMap[name] = index;
	
	return (AnimaDirectionalLight*)_lights[index];
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateDirectionalLight(str);
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	
	AnimaPointLight* newLight = AnimaAllocatorNamespace::AllocateNew<AnimaPointLight>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator(), _scene->GetDataGeneratorsManager(), name);
	
	AUint index = (AUint)_lights.Add(newLight);
	_lightsMap[name] = index;
	
	return (AnimaPointLight*)_lights[index];
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreatePointLight(str);
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	
	AnimaSpotLight* newLight = AnimaAllocatorNamespace::AllocateNew<AnimaSpotLight>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator(), _scene->GetDataGeneratorsManager(), name);
	
	AUint index = (AUint)_lights.Add(newLight);
	_lightsMap[name] = index;
	
	return (AnimaSpotLight*)_lights[index];
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateSpotLight(str);
}

void AnimaLightsManager::ClearLights()
{
	for(AInt i = 0; i < _lights.GetSize(); i++)
	{
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetLightsAllocator()), _lights[i]);
		_lights[i] = nullptr;
	}
	
	_lights.RemoveAll();
}

AnimaLight* AnimaLightsManager::GetLight(AUint index)
{
	return _lights[index];
}

AnimaLight* AnimaLightsManager::GetLightFromName(const AnimaString& name)
{
	if (_lightsMap.find(name) == _lightsMap.end())
		return nullptr;
	return GetLight(_lightsMap[name]);
}

AnimaLight* AnimaLightsManager::GetLightFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetLightFromName(str);
}

AInt AnimaLightsManager::GetTotalLightsCount()
{
	return _lights.GetSize();
}

AInt AnimaLightsManager::GetAmbientLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lights.GetSize(); i++)
	{
		if (_lights[i]->IsAmbientLight())
			c++;
	}
	return c;
}

AInt AnimaLightsManager::GetDirectionalLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lights.GetSize(); i++)
	{
		if (_lights[i]->IsDirectionalLight())
			c++;
	}
	return c;
}

AInt AnimaLightsManager::GetPointLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lights.GetSize(); i++)
	{
		if (_lights[i]->IsPointLight())
			c++;
	}
	return c;
}

AInt AnimaLightsManager::GetSpotLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lights.GetSize(); i++)
	{
		if (_lights[i]->IsSpotLight())
			c++;
	}
	return c;
}

END_ANIMA_ENGINE_NAMESPACE