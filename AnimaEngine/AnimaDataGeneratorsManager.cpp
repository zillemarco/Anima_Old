//
//  AnimaDataGeneratorsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaDataGeneratorsManager.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaDataGeneratorsManager::AnimaDataGeneratorsManager(AnimaScene* scene, AnimaEngine* engine)
: _dataGenerators(scene == nullptr ? engine->GetDataGeneratorsAllocator() : scene->GetEngine()->GetDataGeneratorsAllocator())
{
	_engine = engine;
	_scene = scene;
}

AnimaDataGeneratorsManager::~AnimaDataGeneratorsManager()
{
	ClearGenerators();
}

AnimaColorGenerator* AnimaDataGeneratorsManager::CreateColorGenerator(const AnimaString& name)
{
	AnimaDataGenerator* dataGenerator = _dataGenerators.Contains(name);
	if (dataGenerator != nullptr)
		return nullptr;
	
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetDataGeneratorsAllocator() : _scene->GetDataGeneratorsAllocator();
	ANIMA_ASSERT(allocator != nullptr);
	AnimaColorGenerator* newDataGenerator = AnimaAllocatorNamespace::AllocateNew<AnimaColorGenerator>(*allocator, name, allocator);
	_dataGenerators.Add<AnimaColorGenerator*>(name, newDataGenerator);
	
	return newDataGenerator;
}

AnimaVectorGenerator* AnimaDataGeneratorsManager::CreateVectorGenerator(const AnimaString& name)
{
	AnimaDataGenerator* dataGenerator = _dataGenerators.Contains(name);
	if (dataGenerator != nullptr)
		return nullptr;
	
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetDataGeneratorsAllocator() : _scene->GetDataGeneratorsAllocator();
	ANIMA_ASSERT(allocator != nullptr);
	AnimaVectorGenerator* newDataGenerator = AnimaAllocatorNamespace::AllocateNew<AnimaVectorGenerator>(*allocator, name, allocator);
	_dataGenerators.Add<AnimaVectorGenerator*>(name, newDataGenerator);
	
	return newDataGenerator;
}

AnimaTextureGenerator* AnimaDataGeneratorsManager::CreateTextureGenerator(const AnimaString& name)
{
	AnimaDataGenerator* dataGenerator = _dataGenerators.Contains(name);
	if (dataGenerator != nullptr)
		return nullptr;
	
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetDataGeneratorsAllocator() : _scene->GetDataGeneratorsAllocator();
	ANIMA_ASSERT(allocator != nullptr);
	AnimaTextureGenerator* newDataGenerator = AnimaAllocatorNamespace::AllocateNew<AnimaTextureGenerator>(*allocator, name, allocator, _scene != nullptr ? _scene->GetTexturesManager() : nullptr);
	_dataGenerators.Add<AnimaTextureGenerator*>(name, newDataGenerator);
	
	return newDataGenerator;
}

void AnimaDataGeneratorsManager::ClearGenerators()
{
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetDataGeneratorsAllocator() : _scene->GetDataGeneratorsAllocator();

	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaDataGenerator*>*, AnimaStringHasher>* dataGeneratorsMap = _dataGenerators.GetArraysMap();
	for (auto dataGeneratorsPair : (*dataGeneratorsMap))
	{
		AnimaMappedArray<AnimaDataGenerator*>* dataGeneratorsArray = dataGeneratorsPair.second;
		AInt count = dataGeneratorsArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaDataGenerator* dataGenerator = (*dataGeneratorsArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*allocator, dataGenerator);
			dataGenerator = nullptr;
		}
	}
	
	_dataGenerators.RemoveAll();
}

AnimaDataGenerator* AnimaDataGeneratorsManager::GetGenerator(const AnimaString& name)
{
	return _dataGenerators.GetWithName(name);
}

bool AnimaDataGeneratorsManager::RemoveGenerator(const AnimaString& name)
{
	AnimaMappedArray<AnimaDataGenerator*>* mappedArray = _dataGenerators.GetMappedArrayFromName(name);
	if (mappedArray == nullptr)
		return false;

	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetDataGeneratorsAllocator() : _scene->GetDataGeneratorsAllocator();

	AnimaDataGenerator* dataGenerator = (*mappedArray)[name];
	AnimaAllocatorNamespace::DeallocateObject(*allocator, dataGenerator);
	dataGenerator = nullptr;

	return mappedArray->Remove(name);
}

bool AnimaDataGeneratorsManager::RemoveGenerator(AnimaDataGenerator* generator)
{
	return RemoveGenerator(generator->GetName());
}

void AnimaDataGeneratorsManager::UpdateValues(AFloat elapsedTime)
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaDataGenerator*>*, AnimaStringHasher>* dataGeneratorsMap = _dataGenerators.GetArraysMap();
	for (auto dataGeneratorsPair : (*dataGeneratorsMap))
	{
		AnimaMappedArray<AnimaDataGenerator*>* dataGeneratorsArray = dataGeneratorsPair.second;
		AInt count = dataGeneratorsArray->GetSize();
		for (AInt i = 0; i < count; i++)
			dataGeneratorsArray->Get(i)->UpdateValue();
	}
}

AInt AnimaDataGeneratorsManager::GetTotalDataGeneratorsCount()
{
	AInt count = 0;
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaDataGenerator*>*, AnimaStringHasher>* dataGeneratorsMap = _dataGenerators.GetArraysMap();
	for (auto dataGeneratorsPair : (*dataGeneratorsMap))
		count += dataGeneratorsPair.second->GetSize();
	
	return count;
}

AnimaTypeMappedArray<AnimaDataGenerator*>* AnimaDataGeneratorsManager::GetDataGeneratorsTypeMappedArray()
{
	return &_dataGenerators;
}

END_ANIMA_ENGINE_NAMESPACE