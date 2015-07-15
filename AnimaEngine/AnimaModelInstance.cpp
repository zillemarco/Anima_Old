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
	_meshes.clear();

	if (meshes != nullptr)
	{
		_meshes = *meshes;

		AInt meshesCount = _meshes.size();
		for (AInt i = 0; i < meshesCount; i++)
			_meshes[i]->SetParentObject(this);
	}
}

AInt AnimaModelInstance::GetMeshesCount() const
{
	return _meshes.size();
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

void AnimaModelInstance::UpdateChildrenTransformation()
{
	AnimaSceneObject::UpdateChildrenTransformation();
	
	AInt meshesCount = _meshes.size();
	for (AInt i = 0; i < meshesCount; i++)
		_meshes[i]->GetTransformation()->UpdateMatrix();
}

END_ANIMA_ENGINE_NAMESPACE
