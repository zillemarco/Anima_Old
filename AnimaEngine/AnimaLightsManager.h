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

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaLightsManager
{
public:
	AnimaLightsManager(AnimaEngine* engine);
	~AnimaLightsManager();
	
	AnimaAmbientLight* CreateAmbientLight(const AnimaString& name);
	AnimaAmbientLight* CreateAmbientLight(const char* name);

	AnimaDirectionalLight* CreateDirectionalLight(const AnimaString& name);
	AnimaDirectionalLight* CreateDirectionalLight(const char* name);

	AnimaPointLight* CreatePointLight(const AnimaString& name);
	AnimaPointLight* CreatePointLight(const char* name);

	AnimaSpotLight* CreateSpotLight(const AnimaString& name);
	AnimaSpotLight* CreateSpotLight(const char* name);

	AnimaLight* GetLight(AUint index);
	AnimaLight* GetLightFromName(const AnimaString& name);
	AnimaLight* GetLightFromName(const char* name);

	AInt GetTotalLightsCount();
	AInt GetAmbientLightsCount();
	AInt GetDirectionalLightsCount();
	AInt GetPointLightsCount();
	AInt GetSpotLightsCount();

private:
	void ClearLights(bool bDeleteObjects = true, bool bResetNumber = true);
	
private:
	AnimaEngine* _engine;
	
	AnimaLight** _lights;
	ASizeT _lightsNumber;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _lightsMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaLightsManager__) */
