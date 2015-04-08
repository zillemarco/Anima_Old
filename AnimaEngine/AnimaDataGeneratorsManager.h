//
//  AnimaDataGeneratorsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaDataGeneratorsManager__
#define __Anima__AnimaDataGeneratorsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaColorGenerator.h"
#include "AnimaVectorGenerator.h"
#include "AnimaDataGenerator.h"
#include "AnimaScene.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaTypeMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaDataGeneratorsManager
{
public:
	AnimaDataGeneratorsManager(AnimaScene* scene);
	~AnimaDataGeneratorsManager();
	
	template<class T> T* CreateDataGenerator(const AnimaString& name);
	template<class T> T* CreateDataGenerator(const char* name);
	
	template<class T> AnimaArray<AnimaDataGenerator*>* GetDataGeneratorsArrayOfType();
	template<class T> AnimaDataGenerator* GetDataGeneratorOfTypeFromName(const AnimaString& name);
	template<class T> AnimaDataGenerator* GetDataGeneratorOfTypeFromName(const char* name);
	
	AnimaColorGenerator* CreateColorGenerator(const AnimaString& name);
	AnimaColorGenerator* CreateColorGenerator(const char* name);

	AnimaVectorGenerator* CreateVectorGenerator(const AnimaString& name);
	AnimaVectorGenerator* CreateVectorGenerator(const char* name);

	AnimaDataGenerator* GetGenerator(const AnimaString& name);
	AnimaDataGenerator* GetGenerator(const char* name);
	
	AInt GetTotalDataGeneratorsCount();
	
	AnimaTypeMappedArray<AnimaDataGenerator*>* GetDataGeneratorsTypeMappedArray();
	
	void UpdateValues();

private:
	void ClearGenerators();
	
private:
	AnimaScene* _scene;
	AnimaTypeMappedArray<AnimaDataGenerator*> _dataGenerators;
};

template<class T>
T* AnimaDataGeneratorsManager::CreateDataGenerator(const AnimaString& name)
{
	AnimaDataGenerator* dataGenerator = _dataGenerators.Contains(name);
	if (dataGenerator != nullptr)
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	T* newDataGenerator = AnimaAllocatorNamespace::AllocateNew<T>(*(_scene->GetDataGeneratorsAllocator()), _scene->GetDataGeneratorsAllocator());
	_dataGenerators.Add<T*>(name, newDataGenerator);
	
	return newDataGenerator;
}

template<class T>
T* AnimaDataGeneratorsManager::CreateDataGenerator(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return CreateDataGenerator<T>(str);
}

template<class T>
AnimaArray<AnimaDataGenerator*>* AnimaDataGeneratorsManager::GetDataGeneratorsArrayOfType()
{
	return _dataGenerators.GetMappedArrayArrayOfType<T*>();
}

template<class T>
AnimaDataGenerator* AnimaDataGeneratorsManager::GetDataGeneratorOfTypeFromName(const AnimaString& name)
{
	return _dataGenerators.GetWithNameAndType<T*>(name);
}

template<class T>
AnimaDataGenerator* AnimaDataGeneratorsManager::GetDataGeneratorOfTypeFromName(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
	return GetDataGeneratorOfTypeFromName<T*>(str);
}


END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaDataGeneratorsManager__) */
