//
//  AnimaMaterialsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaMaterialsManager__
#define __Anima__AnimaMaterialsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaMaterial.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterialsManager
{
public:
	AnimaMaterialsManager(AnimaEngine* engine);
	~AnimaMaterialsManager();
	
	AnimaMaterial* CreateMaterial(const AnimaString& name);
	AnimaMaterial* CreateMaterial(const char* name);

	AnimaMaterial* GetMaterial(AUint index);
	AnimaMaterial* GetMaterial(const AnimaString& name);
	AnimaMaterial* GetMaterial(const char* name);

private:
	void ClearMaterials(bool bDeleteObjects = true, bool bResetNumber = true);
	
private:
	AnimaEngine* _engine;
	
	AnimaMaterial** _materials;
	ASizeT _materialsNumber;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _materialsMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterialsManager__) */
