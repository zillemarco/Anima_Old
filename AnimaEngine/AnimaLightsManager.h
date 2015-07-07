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
#include "AnimaArray.h"
#include "AnimaCamera.h"
#include "AnimaMappedArray.h"
#include "AnimaTypeMappedArray.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaLightsManager
{
public:
	AnimaLightsManager(AnimaScene* scene);
	~AnimaLightsManager();

	template<class T> T* CreateLight(const AnimaString& name);
	
	template<class T> AnimaArray<AnimaLight*>* GetLightsArrayOfType();
	template<class T> AnimaLight* GetLightOfTypeFromName(const AnimaString& name);
		
	AnimaDirectionalLight* CreateDirectionalLight(const AnimaString& name);

	AnimaPointLight* CreatePointLight(const AnimaString& name);

	AnimaSpotLight* CreateSpotLight(const AnimaString& name);

	AnimaLight* GetLightFromName(const AnimaString& name);

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
AnimaArray<AnimaLight*>* AnimaLightsManager::GetLightsArrayOfType()
{
	return _lights.GetMappedArrayArrayOfType<T*>();
}

template<class T> 
AnimaLight* AnimaLightsManager::GetLightOfTypeFromName(const AnimaString& name)
{
	return _lights.GetWithNameAndType<T*>(name);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLightsManager__) */
