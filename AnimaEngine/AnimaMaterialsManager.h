//
//  AnimaMaterialsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaMaterialsManager__
#define __Anima__AnimaMaterialsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaMaterial.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaTypeMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterialsManager
{
public:
	AnimaMaterialsManager(AnimaEngine* engine);
	~AnimaMaterialsManager();
	
	template<class T> T* CreateMaterial(const AnimaString& name);
	template<class T> T* CreateMaterial(const char* name);
	
	template<class T> AnimaArray<AnimaMaterial*>* GetMaterialsArrayOfType();
	template<class T> AnimaMaterial* GetMaterialOfTypeFromName(const AnimaString& name);
	template<class T> AnimaMaterial* GetMaterialOfTypeFromName(const char* name);
	
	AnimaMaterial* CreateGenericMaterial(const AnimaString& name);
	AnimaMaterial* CreateGenericMaterial(const char* name);

	AnimaMaterial* GetMaterialFromName(const AnimaString& name);
	AnimaMaterial* GetMaterialFromName(const char* name);
	
	AInt GetTotalMaterialsCount();
	
	AnimaTypeMappedArray<AnimaMaterial*>* GetMaterials();
	
private:
	void ClearMaterials();
	
private:
	AnimaEngine* _engine;
	AnimaTypeMappedArray<AnimaMaterial*> _materials;
};

template<class T>
T* AnimaMaterialsManager::CreateMaterial(const AnimaString& name)
{
	AnimaMaterial* material = _materials.Contains(name);
	if (material != nullptr)
		return nullptr;
	
	ANIMA_ASSERT(_engine != nullptr);
	T* newMaterial = AnimaAllocatorNamespace::AllocateNew<T>(*(_engine->GetMaterialsAllocator()), _engine->GetMaterialsAllocator(), _engine->GetDataGeneratorsManager(), name);
	_materials.Add<T*>(name, newMaterial);
	
	return newMaterial;
}

template<class T>
T* AnimaMaterialsManager::CreateMaterial(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return CreateMaterial<T>(str);
}

template<class T>
AnimaArray<AnimaMaterial*>* AnimaMaterialsManager::GetMaterialsArrayOfType()
{
	return _materials.GetMappedArrayArrayOfType<T*>();
}

template<class T>
AnimaMaterial* AnimaMaterialsManager::GetMaterialOfTypeFromName(const AnimaString& name)
{
	return _materials.GetWithNameAndType<T*>(name);
}

template<class T>
AnimaMaterial* AnimaMaterialsManager::GetMaterialOfTypeFromName(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return GetMaterialOfTypeFromName<T*>(str);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterialsManager__) */
