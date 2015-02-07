//
//  AnimaLightsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaLightsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaLightsManager::AnimaLightsManager(AnimaStage* stage)
{
	_stage = stage;
	
	_lights = nullptr;
	_lightsNumber = 0;
}

AnimaLightsManager::~AnimaLightsManager()
{
	ClearLights();
}

AnimaAmbientLight* AnimaLightsManager::CreateAmbientLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_lightsNumber > 0)
	{
		AnimaLight** tmpOldLights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber; i++)
			tmpOldLights[i] = _lights[i];

		ClearLights(false, false);

		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber - 1; i++)
			_lights[i] = tmpOldLights[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetLightsAllocator()), tmpOldLights);
		tmpOldLights = nullptr;
	}
	else
	{
		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);
	}

	_lights[_lightsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaAmbientLight>(*(_stage->GetLightsAllocator()), _stage->GetLightsAllocator(), _stage->GetDataGeneratorsManager(), name);

	_lightsMap[name] = (AUint)(_lightsNumber - 1);

	return (AnimaAmbientLight*)_lights[_lightsNumber - 1];
}

AnimaAmbientLight* AnimaLightsManager::CreateAmbientLight(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreateAmbientLight(str);
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_lightsNumber > 0)
	{
		AnimaLight** tmpOldLights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber; i++)
			tmpOldLights[i] = _lights[i];

		ClearLights(false, false);

		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber - 1; i++)
			_lights[i] = tmpOldLights[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetLightsAllocator()), tmpOldLights);
		tmpOldLights = nullptr;
	}
	else
	{
		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);
	}

	_lights[_lightsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaDirectionalLight>(*(_stage->GetLightsAllocator()), _stage->GetLightsAllocator(), _stage->GetDataGeneratorsManager(), name);

	_lightsMap[name] = (AUint)(_lightsNumber - 1);

	return (AnimaDirectionalLight*)_lights[_lightsNumber - 1];
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreateDirectionalLight(str);
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_lightsNumber > 0)
	{
		AnimaLight** tmpOldLights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber; i++)
			tmpOldLights[i] = _lights[i];

		ClearLights(false, false);

		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber - 1; i++)
			_lights[i] = tmpOldLights[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetLightsAllocator()), tmpOldLights);
		tmpOldLights = nullptr;
	}
	else
	{
		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);
	}

	_lights[_lightsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaPointLight>(*(_stage->GetLightsAllocator()), _stage->GetLightsAllocator(), _stage->GetDataGeneratorsManager(), name);

	_lightsMap[name] = (AUint)(_lightsNumber - 1);

	return (AnimaPointLight*)_lights[_lightsNumber - 1];
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreatePointLight(str);
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const AnimaString& name)
{
	if (_lightsMap.find(name) != _lightsMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_lightsNumber > 0)
	{
		AnimaLight** tmpOldLights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber; i++)
			tmpOldLights[i] = _lights[i];

		ClearLights(false, false);

		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);

		for (int i = 0; i < _lightsNumber - 1; i++)
			_lights[i] = tmpOldLights[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetLightsAllocator()), tmpOldLights);
		tmpOldLights = nullptr;
	}
	else
	{
		_lightsNumber++;
		_lights = AnimaAllocatorNamespace::AllocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lightsNumber);
	}

	_lights[_lightsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaSpotLight>(*(_stage->GetLightsAllocator()), _stage->GetLightsAllocator(), _stage->GetDataGeneratorsManager(), name);

	_lightsMap[name] = (AUint)(_lightsNumber - 1);

	return (AnimaSpotLight*)_lights[_lightsNumber - 1];
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreateSpotLight(str);
}

void AnimaLightsManager::ClearLights(bool bDeleteObjects, bool bResetNumber)
{
	if (_lights != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_lightsNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_stage->GetLightsAllocator()), _lights[i]);
				_lights[i] = nullptr;
			}
		}
		
		AnimaAllocatorNamespace::DeallocateArray<AnimaLight*>(*(_stage->GetLightsAllocator()), _lights);
		_lights = nullptr;
	}
	
	if (bResetNumber)
		_lightsNumber = 0;
}

AnimaLight* AnimaLightsManager::GetLight(AUint index)
{
	ANIMA_ASSERT(index >= 0 && index < _lightsNumber);
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
	AnimaString str(name, _stage->GetStringAllocator());
	return GetLightFromName(str);
}

AInt AnimaLightsManager::GetTotalLightsCount()
{
	return (AInt)_lightsNumber;
}

AInt AnimaLightsManager::GetAmbientLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lightsNumber; i++)
	{
		if (_lights[i]->IsAmbientLight())
			c++;
	}
	return c;
}

AInt AnimaLightsManager::GetDirectionalLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lightsNumber; i++)
	{
		if (_lights[i]->IsDirectionalLight())
			c++;
	}
	return c;
}

AInt AnimaLightsManager::GetPointLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lightsNumber; i++)
	{
		if (_lights[i]->IsPointLight())
			c++;
	}
	return c;
}

AInt AnimaLightsManager::GetSpotLightsCount()
{
	AInt c = 0;
	for (ASizeT i = 0; i < _lightsNumber; i++)
	{
		if (_lights[i]->IsSpotLight())
			c++;
	}
	return c;
}

END_ANIMA_ENGINE_NAMESPACE