//
//  AnimaDataGeneratorsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaDataGeneratorsManager.h"

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
	return CreateDataGenerator<AnimaColorGenerator>(name);
}

AnimaColorGenerator* AnimaDataGeneratorsManager::CreateColorGenerator(const char* name)
{
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetStringAllocator() : _scene->GetStringAllocator();
	AnimaString str(name, allocator);
	return CreateColorGenerator(str);
}

AnimaVectorGenerator* AnimaDataGeneratorsManager::CreateVectorGenerator(const AnimaString& name)
{
	return CreateDataGenerator<AnimaVectorGenerator>(name);
}

AnimaVectorGenerator* AnimaDataGeneratorsManager::CreateVectorGenerator(const char* name)
{
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetStringAllocator() : _scene->GetStringAllocator();
	AnimaString str(name, allocator);
	return CreateVectorGenerator(str);
}

void AnimaDataGeneratorsManager::ClearGenerators()
{
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetDataGeneratorsAllocator() : _scene->GetDataGeneratorsAllocator();

	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaDataGenerator*>*, AnimaString::Hasher>* dataGeneratorsMap = _dataGenerators.GetArraysMap();
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

AnimaDataGenerator* AnimaDataGeneratorsManager::GetGenerator(const char* name)
{
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetStringAllocator() : _scene->GetStringAllocator();
	AnimaString str(name, allocator);
	return GetGenerator(str);
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

bool AnimaDataGeneratorsManager::RemoveGenerator(const char* name)
{
	AnimaAllocator* allocator = _scene == nullptr ? _engine->GetStringAllocator() : _scene->GetStringAllocator();
	AnimaString str(name, allocator);
	return RemoveGenerator(str);
}

bool AnimaDataGeneratorsManager::RemoveGenerator(AnimaDataGenerator* generator)
{
	return RemoveGenerator(generator->GetAnimaName());
}

void AnimaDataGeneratorsManager::UpdateValues()
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaDataGenerator*>*, AnimaString::Hasher>* dataGeneratorsMap = _dataGenerators.GetArraysMap();
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
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaDataGenerator*>*, AnimaString::Hasher>* dataGeneratorsMap = _dataGenerators.GetArraysMap();
	for (auto dataGeneratorsPair : (*dataGeneratorsMap))
		count += dataGeneratorsPair.second->GetSize();
	
	return count;
}

AnimaTypeMappedArray<AnimaDataGenerator*>* AnimaDataGeneratorsManager::GetDataGeneratorsTypeMappedArray()
{
	return &_dataGenerators;
}

END_ANIMA_ENGINE_NAMESPACE