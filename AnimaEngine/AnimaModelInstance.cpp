//
//  AnimaModelInstance.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaModelInstance.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModelInstance::AnimaModelInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
	, _materialName(allocator)
	, _generatorModelName(allocator)
	, _meshes(allocator)
{
	_material = nullptr;
	_generatorModel = nullptr;
}

AnimaModelInstance::AnimaModelInstance(const AnimaModelInstance& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _generatorModelName(src._generatorModelName)
	, _meshes(src._meshes)
{
	_material = src._material;
	_generatorModel = src._generatorModel;
}

AnimaModelInstance::AnimaModelInstance(AnimaModelInstance&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _generatorModelName(src._generatorModelName)
	, _meshes(src._meshes)
{
	_material = src._material;
	_generatorModel = src._generatorModel;
}

AnimaModelInstance::~AnimaModelInstance()
{
}

AnimaModelInstance& AnimaModelInstance::operator=(const AnimaModelInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		_meshes = src._meshes;
		
		_generatorModel = src._generatorModel;
		_generatorModelName = src._generatorModelName;
	}
	
	return *this;
}

AnimaModelInstance& AnimaModelInstance::operator=(AnimaModelInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		_meshes = src._meshes;

		_generatorModel = src._generatorModel;
		_generatorModelName = src._generatorModelName;
	}
	
	return *this;
}

void AnimaModelInstance::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	if (_material)
		_materialName = material->GetAnimaName();
	else
		_materialName = "";
}

AnimaMaterial* AnimaModelInstance::GetMaterial()
{
	return _material;
}

void AnimaModelInstance::SetMeshes(AnimaArray<AnimaMesh*>* meshes)
{
	_meshes.Copy(*meshes);
}

AInt AnimaModelInstance::GetMeshesCount() const
{
	return _meshes.GetSize();
}

AnimaMesh* AnimaModelInstance::GetMesh(AInt index)
{
	return _meshes[index];
}

void AnimaModelInstance::SetGeneratorModel(AnimaModel* model)
{
}

AnimaModel* AnimaModelInstance::GetGeneratorModel() const
{
	return _generatorModel;
}

END_ANIMA_ENGINE_NAMESPACE
