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

AnimaModel::AnimaModel(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr)
	_engine = engine;
	
	_modelChildren = nullptr;
	_modelMeshes = nullptr;
	
	_modelChildrenNumber = 0;
	_modelMeshesNumber = 0;
}

AnimaModel::AnimaModel(const AnimaModel& src)
{
	_engine = src._engine;
	
	_modelChildren = nullptr;
	_modelMeshes = nullptr;
	
	_modelChildrenNumber = 0;
	_modelMeshesNumber = 0;
	
	SetChildren(src._modelChildren, src._modelChildrenNumber);
	SetMeshes(src._modelMeshes, src._modelMeshesNumber);
}

AnimaModel::AnimaModel(AnimaModel&& src)
: _modelChildren(src._modelChildren)
, _modelChildrenNumber(src._modelChildrenNumber)
, _modelMeshes(src._modelMeshes)
, _modelMeshesNumber(src._modelMeshesNumber)
, _engine(src._engine)
{
	src._modelChildren = nullptr;
	src._modelMeshes = nullptr;
	
	src._modelChildrenNumber = 0;
	src._modelMeshesNumber = 0;
}

AnimaModel::~AnimaModel()
{
	ANIMA_ASSERT(_engine != nullptr);
	
	ClearChildren();
	ClearMeshes();
}

AnimaModel& AnimaModel::operator=(const AnimaModel& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		
		SetChildren(src._modelChildren, src._modelChildrenNumber);
		SetMeshes(src._modelMeshes, src._modelMeshesNumber);
	}
	
	return *this;
}

AnimaModel& AnimaModel::operator=(AnimaModel&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		
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
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _modelChildren);
		_modelChildren = nullptr;
		_modelChildrenNumber = 0;
	}
}

void AnimaModel::ClearMeshes()
{
	if(_modelMeshes != nullptr && _modelMeshesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _modelMeshes);
		_modelMeshes = nullptr;
		_modelMeshesNumber = 0;
	}
}

void AnimaModel::SetChildren(AnimaModel* children, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr);
	ClearChildren();
	
	if(children != nullptr && n > 0)
	{
		_modelChildrenNumber = n;
		_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelChildrenNumber, _engine);

		//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
		for (int i = 0; i < _modelChildrenNumber; i++)
			_modelChildren[i] = children[i];
	}
}

void AnimaModel::AddChild(const AnimaModel& child)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_modelChildrenNumber > 0)
	{
		AnimaModel* tmpOldChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelChildrenNumber, _engine);

		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
		for (int i = 0; i < _modelChildrenNumber; i++)
			tmpOldChildren[i] = _modelChildren[i];

		//_allocator->Deallocate(_normals);
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _modelChildren);

		_modelChildrenNumber++;
		_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelChildrenNumber, _engine);

		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
		for (int i = 0; i < _modelChildrenNumber - 1; i++)
			_modelChildren[i] = tmpOldChildren[i];

		_modelChildren[_modelChildrenNumber - 1] = child;

		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), tmpOldChildren);
	}
	else
	{
		_modelChildrenNumber++;
		_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*(_engine->GetModelsAllocator()), _modelChildrenNumber, _engine);
		
		_modelChildren[_modelChildrenNumber - 1] = child;
	}
}

void AnimaModel::SetMeshes(AnimaMesh* meshes, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr);
	ClearMeshes();
	
	if(meshes != nullptr && n > 0)
	{
		_modelMeshesNumber = n;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_engine->GetModelsAllocator()), _modelMeshesNumber, _engine);
	
		for (int i = 0; i < _modelMeshesNumber; i++)
			_modelMeshes[i] = meshes[i];
	}
}

void AnimaModel::AddMesh(const AnimaMesh& mesh)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_modelMeshesNumber > 0)
	{
		AnimaMesh* tmpOldMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_engine->GetModelsAllocator()), _modelMeshesNumber, _engine);
	
		for (int i = 0; i < _modelMeshesNumber; i++)
			tmpOldMeshes[i] = _modelMeshes[i];
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), _modelMeshes);
	
		_modelMeshesNumber++;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_engine->GetModelsAllocator()), _modelMeshesNumber, _engine);
	
		for (int i = 0; i < _modelMeshesNumber - 1; i++)
			_modelMeshes[i] = tmpOldMeshes[i];
	
		_modelMeshes[_modelMeshesNumber - 1] = mesh;
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelsAllocator()), tmpOldMeshes);
	}
	else
	{
		_modelMeshesNumber++;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*(_engine->GetModelsAllocator()), _modelMeshesNumber, _engine);
		
		_modelMeshes[_modelMeshesNumber - 1] = mesh;
	}
}

ASizeT AnimaModel::GetChildrenNumber()
{
	return _modelChildrenNumber;
}

AnimaModel AnimaModel::GetChild(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelChildrenNumber);
	return _modelChildren[index];
}

AnimaModel* AnimaModel::GetPChild(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelChildrenNumber);
	return &_modelChildren[index];
}

AnimaModel* AnimaModel::GetChildren()
{
	return _modelChildren;
}

ASizeT AnimaModel::GetMeshesNumber()
{
	return _modelMeshesNumber;
}

AnimaMesh AnimaModel::GetMesh(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelMeshesNumber);
	return _modelMeshes[index];
}

AnimaMesh* AnimaModel::GetPMesh(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _modelMeshesNumber);
	return &_modelMeshes[index];
}

AnimaMesh* AnimaModel::GetMeshes()
{
	return _modelMeshes;
}

END_ANIMA_ENGINE_CORE_NAMESPACE




