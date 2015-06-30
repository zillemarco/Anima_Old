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
	: _scenes(engine->GetScenesAllocator())
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

AnimaScene* AnimaScenesManager::CreateScene(const char* name)
{
	AnimaString str = name;
	return CreateScene(str);
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

AnimaScene* AnimaScenesManager::GetScene(const char* name)
{
	AnimaString str = name;
	return GetScene(str);
}

END_ANIMA_ENGINE_NAMESPACE