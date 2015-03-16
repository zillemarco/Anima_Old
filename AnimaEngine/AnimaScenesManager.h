//
//  AnimaScenesManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaScenesManager__
#define __Anima__AnimaScenesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaScenesManager
{
public:
	AnimaScenesManager(AnimaAllocator* allocator);
	~AnimaScenesManager();
	
	AnimaScene* CreateStage(const AnimaString& name);
	AnimaScene* CreateStage(const char* name);

	AnimaScene* GetStage(AUint index);
	AnimaScene* GetStage(const AnimaString& name);
	AnimaScene* GetStage(const char* name);

private:
	void ClearScenes(bool bDeleteObjects = true, bool bResetNumber = true);
	
private:
	AnimaAllocator* _allocator;
	
	AnimaScene** _scenes;
	ASizeT _scenesNumber;
	
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _scenesMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScenesManager__) */
