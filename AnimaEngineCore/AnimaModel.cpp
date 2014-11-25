//
//  AnimaEngineModel.cpp
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#include <stdio.h>
#include <string.h>
#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaModel::AnimaModel(AnimaAllocator* allocator)
{
	ANIMA_ASSERT(allocator != nullptr)
	_allocator = allocator;
	
	_modelChildren = nullptr;
	_modelMeshes = nullptr;
	
	_modelChildrenNumber = 0;
	_modelMeshesNumber = 0;
}

AnimaModel::AnimaModel(const AnimaModel& src)
{
	_allocator = src._allocator;
	
	SetChildren(src._modelChildren, src._modelChildrenNumber);
	SetMeshes(src._modelMeshes, src._modelMeshesNumber);
}

AnimaModel::AnimaModel(AnimaModel&& src)
: _modelChildren(src._modelChildren)
, _modelChildrenNumber(src._modelChildrenNumber)
, _modelMeshes(src._modelMeshes)
, _modelMeshesNumber(src._modelMeshesNumber)
, _allocator(src._allocator)
{
	src._modelChildren = nullptr;
	src._modelMeshes = nullptr;
	
	src._modelChildrenNumber = 0;
	src._modelMeshesNumber = 0;
}

AnimaModel::~AnimaModel()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearChildren();
	ClearMeshes();
}

AnimaModel& AnimaModel::operator=(const AnimaModel& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		SetChildren(src._modelChildren, src._modelChildrenNumber);
		SetMeshes(src._modelMeshes, src._modelMeshesNumber);
	}
	
	return *this;
}

AnimaModel& AnimaModel::operator=(AnimaModel&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		_modelChildren = src._modelChildren;
		_modelMeshes = src._modelMeshes;
		
		_modelChildrenNumber = src._modelChildrenNumber;
		_modelMeshesNumber = src._modelMeshesNumber;
		
		src._modelChildren = nullptr;
		src._modelMeshes = nullptr;
		
		src._modelChildrenNumber = 0;
		src._modelMeshesNumber = 0;
	}
	
	return *this;
}

void AnimaModel::ClearChildren()
{
	if(_modelChildren != nullptr && _modelChildrenNumber > 0)
	{
		//_allocator->Deallocate(_modelChildren);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _modelChildren);
		_modelChildren = nullptr;
		_modelChildrenNumber = 0;
	}
}

void AnimaModel::ClearMeshes()
{
	if(_modelMeshes != nullptr && _modelMeshesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _modelMeshes);
		_modelMeshes = nullptr;
		_modelMeshesNumber = 0;
	}
}

void AnimaModel::SetChildren(AnimaModel* children, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearChildren();
	
	if(children != nullptr && n > 0)
	{
		_modelChildrenNumber = n;
		_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

		//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
		for (int i = 0; i < _modelChildrenNumber; i++)
			_modelChildren[i] = children[i];
	}
}

void AnimaModel::AddChild(const AnimaModel& child)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaModel* tmpOldChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _modelChildrenNumber; i++)
		tmpOldChildren[i] = _modelChildren[i];

	//_allocator->Deallocate(_normals);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _modelChildren);

	_modelChildrenNumber++;
	_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _modelChildrenNumber - 1; i++)
		_modelChildren[i] = tmpOldChildren[i];

	_modelChildren[_modelChildrenNumber - 1] = child;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldChildren);
}

void AnimaModel::SetMeshes(AnimaMesh* meshes, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearMeshes();
	
	if(meshes != nullptr && n > 0)
	{
		_modelMeshesNumber = n;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);
	
		for (int i = 0; i < _modelMeshesNumber; i++)
			_modelMeshes[i] = meshes[i];
	}
}

void AnimaModel::AddMesh(const AnimaMesh& mesh)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaMesh* tmpOldMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);
	
	for (int i = 0; i < _modelMeshesNumber; i++)
		tmpOldMeshes[i] = _modelMeshes[i];
	
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _modelMeshes);
	
	_modelMeshesNumber++;
	_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);
	
	for (int i = 0; i < _modelMeshesNumber - 1; i++)
		_modelMeshes[i] = tmpOldMeshes[i];
	
	_modelMeshes[_modelMeshesNumber - 1] = mesh;
	
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldMeshes);
}

END_ANIMA_ENGINE_CORE_NAMESPACE




