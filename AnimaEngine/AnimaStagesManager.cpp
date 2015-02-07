//
//  AnimaStagesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaStagesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaStagesManager::AnimaStagesManager(AnimaAllocator* allocator)
{
	_allocator = allocator;
	
	_stages = nullptr;
	_stagesNumber = 0;
}

AnimaStagesManager::~AnimaStagesManager()
{
	ClearStages();
}

AnimaStage* AnimaStagesManager::CreateStage(const AnimaString& name)
{
	if (_stagesMap.find(name) != _stagesMap.end())
		return nullptr;

	ANIMA_ASSERT(_allocator != nullptr);
	if (_stagesNumber > 0)
	{
		AnimaStage** tmpOldStages = AnimaAllocatorNamespace::AllocateArray<AnimaStage*>(*_allocator, _stagesNumber);

		for (int i = 0; i < _stagesNumber; i++)
			tmpOldStages[i] = _stages[i];

		ClearStages(false, false);

		_stagesNumber++;
		_stages = AnimaAllocatorNamespace::AllocateArray<AnimaStage*>(*_allocator, _stagesNumber);

		for (int i = 0; i < _stagesNumber - 1; i++)
			_stages[i] = tmpOldStages[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldStages);
		tmpOldStages = nullptr;
	}
	else
	{
		_stagesNumber++;
		_stages = AnimaAllocatorNamespace::AllocateArray<AnimaStage*>(*_allocator, _stagesNumber);
	}

	_stages[_stagesNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaStage>(*_allocator);

	_stagesMap[name] = (AUint)(_stagesNumber - 1);

	return (AnimaStage*)_stages[_stagesNumber - 1];
}

AnimaStage* AnimaStagesManager::CreateStage(const char* name)
{
	AnimaString str(name, _allocator);
	return CreateStage(str);
}

void AnimaStagesManager::ClearStages(bool bDeleteObjects, bool bResetNumber)
{
	if (_stages != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_stagesNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*_allocator, _stages[i]);
				_stages[i] = nullptr;
			}
		}
		
		AnimaAllocatorNamespace::DeallocateArray<AnimaStage*>(*_allocator, _stages);
		_stages = nullptr;
	}
	
	if (bResetNumber)
		_stagesNumber = 0;
}

AnimaStage* AnimaStagesManager::GetStage(AUint index)
{
	ANIMA_ASSERT(index >= 0 && index < _stagesNumber);
	return _stages[index];
}

AnimaStage* AnimaStagesManager::GetStage(const AnimaString& name)
{
	if (_stagesMap.find(name) == _stagesMap.end())
		return nullptr;
	return GetStage(_stagesMap[name]);
}

AnimaStage* AnimaStagesManager::GetStage(const char* name)
{
	AnimaString str(name, _allocator);
	return GetStage(str);
}

END_ANIMA_ENGINE_NAMESPACE