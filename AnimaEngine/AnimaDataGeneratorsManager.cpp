//
//  AnimaDataGeneratorsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaDataGeneratorsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaDataGeneratorsManager::AnimaDataGeneratorsManager(AnimaEngine* engine)
{
	_engine = engine;
	
	_generators = nullptr;
	_generatorsNumber = 0;
}

AnimaDataGeneratorsManager::~AnimaDataGeneratorsManager()
{
	ClearGenerators();
}

AnimaColorGenerator* AnimaDataGeneratorsManager::CreateColorGenerator(const AnimaString& name)
{
	if (_generatorsMap.find(name) != _generatorsMap.end())
		return nullptr;

	ANIMA_ASSERT(_engine != nullptr);
	if (_generatorsNumber > 0)
	{
		AnimaDataGenerator** tmpOldGenerators = AnimaAllocatorNamespace::AllocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generatorsNumber);

		for (int i = 0; i < _generatorsNumber; i++)
			tmpOldGenerators[i] = _generators[i];

		ClearGenerators(false, false);

		_generatorsNumber++;
		_generators = AnimaAllocatorNamespace::AllocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generatorsNumber);

		for (int i = 0; i < _generatorsNumber - 1; i++)
			_generators[i] = tmpOldGenerators[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), tmpOldGenerators);
		tmpOldGenerators = nullptr;
	}
	else
	{
		_generatorsNumber++;
		_generators = AnimaAllocatorNamespace::AllocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generatorsNumber);
	}

	_generators[_generatorsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaColorGenerator>(*(_engine->GetGenericAllocator()), _engine);

	_generatorsMap[name] = (AUint)(_generatorsNumber - 1);

	return (AnimaColorGenerator*)_generators[_generatorsNumber - 1];
}

AnimaColorGenerator* AnimaDataGeneratorsManager::CreateColorGenerator(const char* name)
{
	AnimaString str(name, _engine);
	return CreateColorGenerator(str);
}

AnimaVectorGenerator* AnimaDataGeneratorsManager::CreateVectorGenerator(const AnimaString& name)
{
	if (_generatorsMap.find(name) != _generatorsMap.end())
		return nullptr;

	ANIMA_ASSERT(_engine != nullptr);
	if (_generatorsNumber > 0)
	{
		AnimaDataGenerator** tmpOldGenerators = AnimaAllocatorNamespace::AllocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generatorsNumber);

		for (int i = 0; i < _generatorsNumber; i++)
			tmpOldGenerators[i] = _generators[i];

		ClearGenerators(false, false);

		_generatorsNumber++;
		_generators = AnimaAllocatorNamespace::AllocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generatorsNumber);

		for (int i = 0; i < _generatorsNumber - 1; i++)
			_generators[i] = tmpOldGenerators[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), tmpOldGenerators);
		tmpOldGenerators = nullptr;
	}
	else
	{
		_generatorsNumber++;
		_generators = AnimaAllocatorNamespace::AllocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generatorsNumber);
	}

	_generators[_generatorsNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaVectorGenerator>(*(_engine->GetGenericAllocator()), _engine);

	_generatorsMap[name] = (AUint)(_generatorsNumber - 1);

	return (AnimaVectorGenerator*)_generators[_generatorsNumber - 1];
}

AnimaVectorGenerator* AnimaDataGeneratorsManager::CreateVectorGenerator(const char* name)
{
	AnimaString str(name, _engine);
	return CreateVectorGenerator(str);
}

void AnimaDataGeneratorsManager::ClearGenerators(bool bDeleteObjects, bool bResetNumber)
{
	if (_generators != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_generatorsNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetGenericAllocator()), _generators[i]);
				_generators[i] = nullptr;
			}
		}
		
		AnimaAllocatorNamespace::DeallocateArray<AnimaDataGenerator*>(*(_engine->GetGenericAllocator()), _generators);
		_generators = nullptr;
	}
	
	if (bResetNumber)
		_generatorsNumber = 0;
}

AnimaDataGenerator* AnimaDataGeneratorsManager::GetGenerator(AUint index)
{
	ANIMA_ASSERT(index >= 0 && index < _generatorsNumber);
	return _generators[index];
}

AnimaDataGenerator* AnimaDataGeneratorsManager::GetGenerator(const AnimaString& name)
{
	if (_generatorsMap.find(name) == _generatorsMap.end())
		return nullptr;
	return GetGenerator(_generatorsMap[name]);
}

AnimaDataGenerator* AnimaDataGeneratorsManager::GetGenerator(const char* name)
{
	AnimaString str(name, _engine);
	return GetGenerator(str);
}

void AnimaDataGeneratorsManager::UpdateValues()
{
	for (ASizeT i = 0; i < _generatorsNumber; i++)
		_generators[i]->UpdateValue();
}

END_ANIMA_ENGINE_NAMESPACE