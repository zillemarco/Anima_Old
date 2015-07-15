//
//  AnimaScenesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaScenesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScenesManager::AnimaScenesManager(AnimaEngine* engine)
{
	_engine = engine;
}

AnimaScenesManager::~AnimaScenesManager()
{
	ClearScenes();
}

AnimaScene* AnimaScenesManager::CreateScene(const AnimaString& name)
{
	AInt index = _scenes.Contains(name);
	if(index >= 0)
		return nullptr;
	
	AnimaScene* scene = AnimaAllocatorNamespace::AllocateNew<AnimaScene>(*(_engine->GetScenesAllocator()), _engine);
	_scenes.Add(name, scene);
	return scene;
}

void AnimaScenesManager::ClearScenes()
{
	AInt count = _scenes.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaScene* scene = _scenes[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetScenesAllocator()), scene);
		scene = nullptr;
	}
	
	_scenes.RemoveAll();
}

AnimaScene* AnimaScenesManager::GetScene(AUint index)
{
	return _scenes[index];
}

AnimaScene* AnimaScenesManager::GetScene(const AnimaString& name)
{
	return _scenes[name];
}

END_ANIMA_ENGINE_NAMESPACE