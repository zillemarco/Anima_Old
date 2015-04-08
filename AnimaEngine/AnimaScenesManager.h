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
#include "AnimaMappedArray.h"
#include "AnimaArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaScenesManager
{
public:
	AnimaScenesManager(AnimaAllocator* allocator);
	~AnimaScenesManager();
	
	AnimaScene* CreateScene(const AnimaString& name);
	AnimaScene* CreateScene(const char* name);

	AnimaScene* GetScene(AUint index);
	AnimaScene* GetScene(const AnimaString& name);
	AnimaScene* GetScene(const char* name);

private:
	void ClearScenes();
	
private:
	AnimaAllocator* _allocator;
	AnimaMappedArray<AnimaScene*> _scenes;
	
//	AnimaScene** _scenes;
//	ASizeT _scenesNumber;
//	
//#pragma warning (disable: 4251)
//	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _scenesMap;
//#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScenesManager__) */
