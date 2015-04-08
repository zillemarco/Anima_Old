//
//  AnimaScenesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaScenesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScenesManager::AnimaScenesManager(AnimaAllocator* allocator)
: _scenes(allocator)
{
	_allocator = allocator;
	
//	_scenes = nullptr;
//	_scenesNumber = 0;
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
	
	AnimaScene* scene = AnimaAllocatorNamespace::AllocateNew<AnimaScene>(*_allocator);
	_scenes.Add(name, scene);
	return scene;
	
//	if (_scenesMap.find(name) != _scenesMap.end())
//		return nullptr;
//
//	ANIMA_ASSERT(_allocator != nullptr);
//	if (_scenesNumber > 0)
//	{
//		AnimaScene** tmpOldScenes = AnimaAllocatorNamespace::AllocateArray<AnimaScene*>(*_allocator, _scenesNumber);
//
//		for (int i = 0; i < _scenesNumber; i++)
//			tmpOldScenes[i] = _scenes[i];
//
//		ClearScenes(false, false);
//
//		_scenesNumber++;
//		_scenes = AnimaAllocatorNamespace::AllocateArray<AnimaScene*>(*_allocator, _scenesNumber);
//
//		for (int i = 0; i < _scenesNumber - 1; i++)
//			_scenes[i] = tmpOldScenes[i];
//
//		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldScenes);
//		tmpOldScenes = nullptr;
//	}
//	else
//	{
//		_scenesNumber++;
//		_scenes = AnimaAllocatorNamespace::AllocateArray<AnimaScene*>(*_allocator, _scenesNumber);
//	}
//
//	_scenes[_scenesNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaScene>(*_allocator);
//
//	_scenesMap[name] = (AUint)(_scenesNumber - 1);
//
//	return (AnimaScene*)_scenes[_scenesNumber - 1];
}

AnimaScene* AnimaScenesManager::CreateScene(const char* name)
{
	AnimaString str(name, _allocator);
	return CreateScene(str);
}

void AnimaScenesManager::ClearScenes()
{
	AInt count = _scenes.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaScene* scene = _scenes[i];
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, scene);
		scene = nullptr;
	}
	
	_scenes.RemoveAll();
//	if (_scenes != nullptr)
//	{
//		if (bDeleteObjects)
//		{
//			for (int i = 0; i < (int)_scenesNumber; i++)
//			{
//				AnimaAllocatorNamespace::DeallocateObject(*_allocator, _scenes[i]);
//				_scenes[i] = nullptr;
//			}
//		}
//		
//		AnimaAllocatorNamespace::DeallocateArray<AnimaScene*>(*_allocator, _scenes);
//		_scenes = nullptr;
//	}
//	
//	if (bResetNumber)
//		_scenesNumber = 0;
}

AnimaScene* AnimaScenesManager::GetScene(AUint index)
{
//	ANIMA_ASSERT(index >= 0 && index < _scenesNumber);
	return _scenes[index];
}

AnimaScene* AnimaScenesManager::GetScene(const AnimaString& name)
{
	return _scenes[name];
//	if (_scenesMap.find(name) == _scenesMap.end())
//		return nullptr;
//	return GetStage(_scenesMap[name]);
}

AnimaScene* AnimaScenesManager::GetScene(const char* name)
{
	AnimaString str(name, _allocator);
	return GetScene(str);
}

END_ANIMA_ENGINE_NAMESPACE