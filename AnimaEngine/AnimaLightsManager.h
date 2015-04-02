//
//  AnimaLightsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaLightsManager__
#define __Anima__AnimaLightsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaLight.h"
#include "AnimaScene.h"
#include "AnimaArray.h"
#include "AnimaCamera.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaLightsMapData
{
public:
	AnimaLightsMapData(AnimaAllocator* allocator);
	AnimaLightsMapData(const AnimaLightsMapData& src);
	AnimaLightsMapData(AnimaLightsMapData&& src);
	virtual ~AnimaLightsMapData();

	AnimaLightsMapData& operator=(const AnimaLightsMapData& src);
	AnimaLightsMapData& operator=(AnimaLightsMapData&& src);

	AnimaArray<AnimaLight*>* GetLightsArray();
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>* GetLightsMap();

protected:
	AnimaAllocator* _allocator;

	AnimaArray<AnimaLight*> _lights;
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _lightsMap;
#pragma warning (default: 4251)
};

class ANIMA_ENGINE_EXPORT AnimaLightsManager
{
public:
	AnimaLightsManager(AnimaScene* scene);
	~AnimaLightsManager();

	template<class T> T* CreateLight(const AnimaString& name);
	template<class T> T* CreateLight(const char* name);

	template<class T> AnimaArray<AnimaLight*>* GetLightsArrayOfType();
	template<class T> AnimaLight* GetLightOfTypeFromName(const AnimaString& name);
	template<class T> AnimaLight* GetLightOfTypeFromName(const char* name);
	
	template<class T> AInt GetLightOfTypeCount();
	
	AnimaDirectionalLight* CreateDirectionalLight(const AnimaString& name);
	AnimaDirectionalLight* CreateDirectionalLight(const char* name);

	AnimaPointLight* CreatePointLight(const AnimaString& name);
	AnimaPointLight* CreatePointLight(const char* name);

	AnimaSpotLight* CreateSpotLight(const AnimaString& name);
	AnimaSpotLight* CreateSpotLight(const char* name);

	AnimaLight* GetLightFromName(const AnimaString& name);
	AnimaLight* GetLightFromName(const char* name);

	AInt GetTotalLightsCount();

	boost::unordered_map<AnimaString, AnimaLightsMapData*, AnimaString::Hasher>* GetLightsMap();

	void UpdateLightsMatrix(AnimaCamera* activeCamera);
			
private:
	void ClearLights();
	
private:
	AnimaScene* _scene;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaLightsMapData*, AnimaString::Hasher> _lightsMap;
	boost::unordered_map<AnimaString, AnimaLightsMapData*, AnimaString::Hasher> _lightsName;
#pragma warning (default: 4251) 
};

template<class T> 
T* AnimaLightsManager::CreateLight(const AnimaString& name)
{
	if (_lightsName.find(name) != _lightsName.end())
		return nullptr;

	AnimaString type(typeid(T).name(), _scene->GetStringAllocator());
	auto pair = _lightsMap.find(type);

	AnimaLightsMapData* lightsMapData = nullptr;

	if (pair != _lightsMap.end())
		lightsMapData = pair->second;
	else
	{
		lightsMapData = AnimaAllocatorNamespace::AllocateNew<AnimaLightsMapData>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator());
		_lightsMap[type] = lightsMapData;
	}
		
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>* lightsMap = lightsMapData->GetLightsMap();
	AnimaArray<AnimaLight*>* lightsArray = lightsMapData->GetLightsArray();

	if (lightsMap->find(name) != lightsMap->end())
		return nullptr;

	_lightsName[name] = lightsMapData;

	ANIMA_ASSERT(_scene != nullptr);

	T* newLight = AnimaAllocatorNamespace::AllocateNew<T>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator(), _scene->GetDataGeneratorsManager(), name);

	AUint index = (AUint)lightsArray->Add(newLight);
	(*lightsMap)[name] = index;
	
	return (T*)lightsArray->ElementAt(index);
}

template<class T> 
T* AnimaLightsManager::CreateLight(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateLight<T>(str);
}

template<class T> 
AnimaArray<AnimaLight*>* AnimaLightsManager::GetLightsArrayOfType()
{
	AnimaString type(typeid(T).name(), _scene->GetStringAllocator());
	auto pair = _lightsMap.find(type);

	if (pair != _lightsMap.end())
		return pair->second->GetLightsArray();
	return nullptr;
}

template<class T> 
AnimaLight* AnimaLightsManager::GetLightOfTypeFromName(const AnimaString& name)
{
	AnimaString type(typeid(T).name(), _scene->GetStringAllocator());
	auto pair = _lightsMap.find(type);

	if (pair != _lightsMap.end())
	{
		boost::unordered_map<AnimaString, AUint, AnimaString::Hasher>* lightsMap = pair->second->GetLightsMap();
		AnimaArray<AnimaLight*>* lightsArray = pair->second->GetLightsArray();

		auto lightsPair = lightsMap->find(name);

		if (lightsPair != lightsMap->end())
			return lightsArray->ElementAt(lightsPair->second);
	}
	return nullptr;
}

template<class T> 
AnimaLight* AnimaLightsManager::GetLightOfTypeFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetLightOfTypeFromName<T>(str);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLightsManager__) */
