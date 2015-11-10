//
//  AnimaAnimationsManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaAnimationsManager__
#define __Anima__AnimaAnimationsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaAnimation.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaAnimationsManager
{
public:
	AnimaAnimationsManager(AnimaScene* scene);
	~AnimaAnimationsManager();

public:
	bool LoadAnimations(const aiScene* scene);

	AInt GetAnimationsCount();

	AnimaAnimation* GetAnimation(AInt index);
	AnimaAnimation* GetAnimationFromName(const AnimaString& name);
	
	AnimaMappedArray<AnimaAnimation*>* GetLastLoadedAnimations();

protected:
	AnimaAnimation* CreateEmptyAnimation(const AnimaString& animationName);

public:
	void ClearAnimations();
	void ClearLastLoadedAnimations();
			
private:
	AnimaScene*	_scene;
	AnimaMappedArray<AnimaAnimation*> _animations;

	AnimaMappedArray<AnimaAnimation*>	_lastLoadedAnimations;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaAnimationsManager__) */
