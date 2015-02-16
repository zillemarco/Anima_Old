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
#include "AnimaModelsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#ifndef min
#	define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#	define max(a,b) (a > b ? a : b)
#endif

AnimaModel::AnimaModel(AnimaAllocator* allocator)
: _modelName(allocator)
, _modelFileName(allocator)
{
	ANIMA_ASSERT(allocator != nullptr)
	_allocator = allocator;
	
	_modelChildren = nullptr;
	_modelMeshes = nullptr;

	_parentModel = nullptr;
	_drawingEnabled = true;
	
	_modelChildrenNumber = 0;
	_modelMeshesNumber = 0;
	_modelName = "AnimaModel";
}

AnimaModel::AnimaModel(const AnimaModel& src)
: _modelName(src._modelName)
, _modelFileName(src._modelFileName)
, _transformation(src._transformation)
{
	_allocator = src._allocator;
	
	_modelChildren = nullptr;
	_modelMeshes = nullptr;
	
	_modelChildrenNumber = 0;
	_modelMeshesNumber = 0;
	_drawingEnabled = src._drawingEnabled;

	_parentModel = src._parentModel;
	
	SetChildren(src._modelChildren, src._modelChildrenNumber);
	SetMeshes(src._modelMeshes, src._modelMeshesNumber);
}

AnimaModel::AnimaModel(AnimaModel&& src)
: _modelChildren(src._modelChildren)
, _modelChildrenNumber(src._modelChildrenNumber)
, _parentModel(src._parentModel)
, _modelMeshes(src._modelMeshes)
, _modelMeshesNumber(src._modelMeshesNumber)
, _modelName(src._modelName)
, _modelFileName(src._modelFileName)
, _transformation(src._transformation)
, _allocator(src._allocator)
{
	_drawingEnabled = src._drawingEnabled;

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
		
		_transformation = src._transformation;
		_parentModel = src._parentModel;

		_drawingEnabled = src._drawingEnabled;
		
		SetModelName(src._modelName);
		SetModelFileName(src._modelFileName);
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

		_drawingEnabled = src._drawingEnabled;

		_modelChildrenNumber = src._modelChildrenNumber;
		_modelMeshesNumber = src._modelMeshesNumber;
		
		_modelName = src._modelName;
		_modelFileName = src._modelFileName;
		_parentModel = src._parentModel;

		_transformation = src._transformation;
		
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

		for (int i = 0; i < _modelChildrenNumber; i++)
			_modelChildren[i] = children[i];
	}
}

void AnimaModel::AddChild(const AnimaModel& child)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if(_modelChildrenNumber > 0)
	{
		AnimaModel* tmpOldChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

		for (int i = 0; i < _modelChildrenNumber; i++)
			tmpOldChildren[i] = _modelChildren[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _modelChildren);

		_modelChildrenNumber++;
		_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

		for (int i = 0; i < _modelChildrenNumber - 1; i++)
			_modelChildren[i] = tmpOldChildren[i];

		_modelChildren[_modelChildrenNumber - 1] = child;

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldChildren);
	}
	else
	{
		_modelChildrenNumber++;
		_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);
		
		_modelChildren[_modelChildrenNumber - 1] = child;
	}
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
	if(_modelMeshesNumber > 0)
	{
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
	else
	{
		_modelMeshesNumber++;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);
		
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

AnimaMesh* AnimaModel::CreateMesh()
{
	ANIMA_ASSERT(_allocator != nullptr);
	if (_modelMeshesNumber > 0)
	{
		AnimaMesh* tmpOldMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);

		for (int i = 0; i < _modelMeshesNumber; i++)
			tmpOldMeshes[i] = _modelMeshes[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _modelMeshes);

		_modelMeshesNumber++;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);

		for (int i = 0; i < _modelMeshesNumber - 1; i++)
			_modelMeshes[i] = tmpOldMeshes[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldMeshes);
	}
	else
	{
		_modelMeshesNumber++;
		_modelMeshes = AnimaAllocatorNamespace::AllocateArray<AnimaMesh>(*_allocator, _modelMeshesNumber, _allocator);
	}

	return &_modelMeshes[_modelMeshesNumber - 1];
}

void AnimaModel::SetModelName(const AnimaString& name)
{
	_modelName = name;
}

void AnimaModel::SetModelName(const char* name)
{
	_modelName = name;
}

AnimaString AnimaModel::GetAnimaModelName()
{
	return _modelName;
}

const char* AnimaModel::GetModelName()
{
	return _modelName.GetConstBuffer();
}

void AnimaModel::SetModelFileName(const AnimaString& name)
{
	_modelFileName = name;
}

void AnimaModel::SetModelFileName(const char* name)
{
	_modelFileName = name;
}

AnimaString AnimaModel::GetAnimaModelFileName()
{
	return _modelFileName;
}

const char* AnimaModel::GetModelFileName()
{
	return _modelFileName.GetConstBuffer();
}

//AnimaMatrix AnimaModel::GetTransformationMatrix()
//{
//	return _transformationMatrix;
//}
//
//AnimaMatrix* AnimaModel::GetPTransformationMatrix()
//{
//	return &_transformationMatrix;
//}
//
//const AFloat* AnimaModel::GetTransformationMatrixFlt()
//{
//	return _transformationMatrix.GetConstData();
//}
//
//void AnimaModel::GetTransformationMatrixFlt(AFloat* m)
//{
//	_transformationMatrix.GetData(m);
//}
//
//void AnimaModel::SetTransformationMatrix(AnimaMatrix m)
//{
//	_transformationMatrix = m;
//}
//
//void AnimaModel::SetTransformationMatrix(AFloat* m)
//{
//	_transformationMatrix.SetData(m);
//}

AnimaTransformation* AnimaModel::GetTransformation()
{
	return &_transformation;
}

AnimaTransformation AnimaModel::GetTransformationCopy()
{
	return _transformation;
}

void AnimaModel::SetParent(AnimaModel* parent)
{
	_parentModel = parent;
}

AnimaModel* AnimaModel::GetParent() const
{
	return _parentModel;
}

void AnimaModel::ComputeBoundingBox(bool updateRecursively)
{
	if (updateRecursively)
	{
		for (ASizeT i = 0; i < _modelChildrenNumber; i++)
			_modelChildren[i].ComputeBoundingBox(updateRecursively);

		for (ASizeT i = 0; i < _modelMeshesNumber; i++)
			_modelMeshes[i].ComputeBoundingBox();
	}

	if (_modelChildrenNumber <= 0 && _modelMeshesNumber <= 0)
	{
		_boundingBoxMin.x = _boundingBoxMax.x = 0.0f;
		_boundingBoxMin.y = _boundingBoxMax.y = 0.0f;
		_boundingBoxMin.z = _boundingBoxMax.z = 0.0f;
		return;
	}

	if (_modelMeshesNumber > 0)
	{
		_boundingBoxMin = _modelMeshes[0].GetBoundingBoxMin();
		_boundingBoxMax = _modelMeshes[0].GetBoundingBoxMax();

		for (ASizeT i = 1; i < _modelMeshesNumber; i++)
		{
			_boundingBoxMin.x = min(_boundingBoxMin.x, _modelMeshes[i].GetBoundingBoxMin().x);
			_boundingBoxMin.y = min(_boundingBoxMin.y, _modelMeshes[i].GetBoundingBoxMin().y);
			_boundingBoxMin.z = min(_boundingBoxMin.z, _modelMeshes[i].GetBoundingBoxMin().z);

			_boundingBoxMax.x = min(_boundingBoxMax.x, _modelMeshes[i].GetBoundingBoxMax().x);
			_boundingBoxMax.y = min(_boundingBoxMax.y, _modelMeshes[i].GetBoundingBoxMax().y);
			_boundingBoxMax.z = min(_boundingBoxMax.z, _modelMeshes[i].GetBoundingBoxMax().z);
		}
	}
	else
	{
		_boundingBoxMin = _modelChildren[0].GetBoundingBoxMin();
		_boundingBoxMax = _modelChildren[0].GetBoundingBoxMax();
	}

	for (ASizeT i = 1; i < _modelChildrenNumber; i++)
	{
		_boundingBoxMin.x = min(_boundingBoxMin.x, _modelChildren[i].GetBoundingBoxMin().x);
		_boundingBoxMin.y = min(_boundingBoxMin.y, _modelChildren[i].GetBoundingBoxMin().y);
		_boundingBoxMin.z = min(_boundingBoxMin.z, _modelChildren[i].GetBoundingBoxMin().z);

		_boundingBoxMax.x = max(_boundingBoxMax.x, _modelChildren[i].GetBoundingBoxMax().x);
		_boundingBoxMax.y = max(_boundingBoxMax.y, _modelChildren[i].GetBoundingBoxMax().y);
		_boundingBoxMax.z = max(_boundingBoxMax.z, _modelChildren[i].GetBoundingBoxMax().z);
	}
}

AnimaVertex3f AnimaModel::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaModel::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

void AnimaModel::EnableDrawing(bool bEnable)
{
	_drawingEnabled = bEnable;
}

bool AnimaModel::DrawingEnabled() const
{
	return _drawingEnabled;
}

END_ANIMA_ENGINE_NAMESPACE




