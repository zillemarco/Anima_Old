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
#include "AnimaMappedArray.h"
#include "AnimaTypeMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

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
		
	AnimaDirectionalLight* CreateDirectionalLight(const AnimaString& name);
	AnimaDirectionalLight* CreateDirectionalLight(const char* name);

	AnimaPointLight* CreatePointLight(const AnimaString& name);
	AnimaPointLight* CreatePointLight(const char* name);

	AnimaSpotLight* CreateSpotLight(const AnimaString& name);
	AnimaSpotLight* CreateSpotLight(const char* name);

	AnimaLight* GetLightFromName(const AnimaString& name);
	AnimaLight* GetLightFromName(const char* name);

	AInt GetTotalLightsCount();

	AnimaTypeMappedArray<AnimaLight*>* GetLights();

	void UpdateLightsMatrix(AnimaCamera* activeCamera);
			
private:
	void ClearLights();
	
private:
	AnimaScene* _scene;
	AnimaTypeMappedArray<AnimaLight*> _lights;
};

template<class T> 
T* AnimaLightsManager::CreateLight(const AnimaString& name)
{
	AnimaLight* light = _lights.Contains(name);
	if (light != nullptr)
		return nullptr;

	ANIMA_ASSERT(_scene != nullptr);
	T* newLight = AnimaAllocatorNamespace::AllocateNew<T>(*(_scene->GetLightsAllocator()), _scene->GetLightsAllocator(), _scene->GetDataGeneratorsManager(), name);
	_lights.Add<T*>(name, newLight);

	return newLight;
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
	return _lights.GetMappedArrayArrayOfType<T*>();
}

template<class T> 
AnimaLight* AnimaLightsManager::GetLightOfTypeFromName(const AnimaString& name)
{
	return _lights.GetWithNameAndType<T*>(name);
}

template<class T> 
AnimaLight* AnimaLightsManager::GetLightOfTypeFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetLightOfTypeFromName<T*>(str);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLightsManager__) */
