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
	, _modelName(allocator)
	, _meshes(allocator)
{
	_model = nullptr;
}

AnimaModelInstance::AnimaModelInstance(const AnimaModelInstance& src)
	: AnimaSceneObject(src)
	, _modelName(src._modelName)
	, _meshes(src._meshes)
{
	_model = src._model;
}

AnimaModelInstance::AnimaModelInstance(AnimaModelInstance&& src)
	: AnimaSceneObject(src)
	, _modelName(src._modelName)
	, _meshes(src._meshes)
{
	_model = src._model;
}

AnimaModelInstance::~AnimaModelInstance()
{
}

AnimaModelInstance& AnimaModelInstance::operator=(const AnimaModelInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_meshes = src._meshes;		
		_model = src._model;
		_modelName = src._modelName;
	}
	
	return *this;
}

AnimaModelInstance& AnimaModelInstance::operator=(AnimaModelInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_meshes = src._meshes;
		_model = src._model;
		_modelName = src._modelName;
	}
	
	return *this;
}

void AnimaModelInstance::SetMeshes(AnimaArray<AnimaMeshInstance*>* meshes)
{
	if (meshes == nullptr)
		_meshes.RemoveAll();
	else
		_meshes.Copy(*meshes);
}

AInt AnimaModelInstance::GetMeshesCount() const
{
	return _meshes.GetSize();
}

AnimaMeshInstance* AnimaModelInstance::GetMesh(AInt index)
{
	return _meshes[index];
}

void AnimaModelInstance::SetModel(AnimaModel* model)
{
	_model = model;
}

AnimaArray<AnimaMeshInstance*>* AnimaModelInstance::GetMeshes() const
{
	return (AnimaArray<AnimaMeshInstance*>*)&_meshes;
}

AnimaModel* AnimaModelInstance::GetModel() const
{
	return _model;
}

END_ANIMA_ENGINE_NAMESPACE
