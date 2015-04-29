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
#include "AnimaTexturesManager.h"
#include "AnimaScene.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterialsManager
{
public:
	AnimaMaterialsManager(AnimaScene* scene, AnimaTexturesManager* texturesManager);
	~AnimaMaterialsManager();
	
public:
	template<class T> T* CreateMaterial(const AnimaString& name);
	template<class T> T* CreateMaterial(const char* name);
	
	template<class T> AnimaArray<AnimaMaterial*>* GetMaterialsArrayOfType();
	template<class T> AnimaMaterial* GetMaterialOfTypeFromName(const AnimaString& name);
	template<class T> AnimaMaterial* GetMaterialOfTypeFromName(const char* name);
	
	AnimaMaterial* CreateGenericMaterial(const AnimaString& name);
	AnimaMaterial* CreateGenericMaterial(const char* name);

	AnimaMaterial* GetMaterial(AInt index);
	AnimaMaterial* GetMaterialFromName(const AnimaString& name);
	AnimaMaterial* GetMaterialFromName(const char* name);
	
	AInt GetTotalMaterialsCount();
	
	AnimaTypeMappedArray<AnimaMaterial*>* GetMaterials();

public:
	bool LoadMaterialsFromModel(const aiScene* scene, const AnimaString& modelName);
	AnimaArray<AnimaString*>* GetLastMaterialsIndexMap();
	void ClearLastMaterialsIndexMap();
	
private:
	void ClearMaterials();
	
private:
	AnimaScene* _scene;
	AnimaTexturesManager* _texturesManager;
	AnimaTypeMappedArray<AnimaMaterial*> _materials;

	AnimaArray<AnimaString*> _lastMaterialsIndexMap;
};

template<class T>
T* AnimaMaterialsManager::CreateMaterial(const AnimaString& name)
{
	AnimaMaterial* material = _materials.Contains(name);
	if (material != nullptr)
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	T* newMaterial = AnimaAllocatorNamespace::AllocateNew<T>(*(_scene->GetMaterialsAllocator()), _scene->GetMaterialsAllocator(), _scene->GetDataGeneratorsManager(), name);
	_materials.Add<T*>(name, newMaterial);
	
	return newMaterial;
}

template<class T>
T* AnimaMaterialsManager::CreateMaterial(const char* name)
{
	AnimaString str(name, _scene->GetStringAllocator());
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
	AnimaString str(name, _scene->GetStringAllocator());
	return GetMaterialOfTypeFromName<T*>(str);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterialsManager__) */
