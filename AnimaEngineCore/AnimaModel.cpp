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
#include <vector>

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaModel::AnimaModel(AnimaAllocator* allocator)
{
	ANIMA_ASSERT(allocator != nullptr)
	_allocator = allocator;
	
	_modelChildren = nullptr;
	_vertices = nullptr;
	_normals = nullptr;
	_textureCoords = nullptr;
	_indexes = nullptr;
	
	_modelChildrenNumber = 0;
	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_indexesNumber = 0;
}

AnimaModel::AnimaModel(const AnimaModel& src)
{
	_allocator = src._allocator;
	
	SetVertices(src._vertices, src._verticesNumber);
	SetNormals(src._normals, src._normalsNumber);
	SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
	SetIndexes(src._indexes, src._indexesNumber);
	SetChildren(src._modelChildren, src._modelChildrenNumber);
}

AnimaModel::AnimaModel(AnimaModel&& src)
: _modelChildren(src._modelChildren)
, _modelChildrenNumber(src._modelChildrenNumber)
, _vertices(src._vertices)
, _verticesNumber(src._verticesNumber)
, _normals(src._normals)
, _normalsNumber(src._normalsNumber)
, _textureCoords(src._textureCoords)
, _textureCoordsNumber(src._textureCoordsNumber)
, _indexes(src._indexes)
, _indexesNumber(src._indexesNumber)
, _allocator(src._allocator)
{
	src._vertices = nullptr;
	src._normals = nullptr;
	src._textureCoords = nullptr;
	src._indexes = nullptr;
	src._modelChildren = nullptr;
	
	src._verticesNumber = 0;
	src._normalsNumber = 0;
	src._textureCoordsNumber = 0;
	src._indexesNumber = 0;
	src._modelChildrenNumber = 0;
}

AnimaModel::~AnimaModel()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearChildren();
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearIndexes();
}

AnimaModel& AnimaModel::operator=(const AnimaModel& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		SetVertices(src._vertices, src._verticesNumber);
		SetNormals(src._normals, src._normalsNumber);
		SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
		SetIndexes(src._indexes, src._indexesNumber);
		SetChildren(src._modelChildren, src._modelChildrenNumber);
	}
	
	return *this;
}

AnimaModel& AnimaModel::operator=(AnimaModel&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		_vertices = src._vertices;
		_normals = src._normals;
		_textureCoords = src._textureCoords;
		_indexes = src._indexes;
		_modelChildren = src._modelChildren;
		
		_verticesNumber = src._verticesNumber;
		_normalsNumber = src._normalsNumber;
		_textureCoordsNumber = src._textureCoordsNumber;
		_indexesNumber = src._indexesNumber;
		_modelChildrenNumber = src._modelChildrenNumber;
		
		src._vertices = nullptr;
		src._normals = nullptr;
		src._textureCoords = nullptr;
		src._indexes = nullptr;
		src._modelChildren = nullptr;
		
		src._verticesNumber = 0;
		src._normalsNumber = 0;
		src._textureCoordsNumber = 0;
		src._indexesNumber = 0;
		src._modelChildrenNumber = 0;
	}
	
	return *this;
}

void AnimaModel::ClearVertices()
{
	if(_vertices != nullptr && _verticesNumber > 0)
	{
		//_allocator->Deallocate(_vertices);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _vertices);
		_vertices = nullptr;
		_verticesNumber = 0;
	}
}

void AnimaModel::ClearNormals()
{
	if(_normals != nullptr && _normalsNumber > 0)
	{
		//_allocator->Deallocate(_normals);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _normals);
		_normals = nullptr;
		_normalsNumber = 0;
	}
}

void AnimaModel::ClearTextureCoords()
{
	if(_textureCoords != nullptr && _textureCoordsNumber > 0)
	{
		//_allocator->Deallocate(_textureCoords);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);
		_textureCoords = nullptr;
		_textureCoordsNumber = 0;
	}
}

void AnimaModel::ClearIndexes()
{
	if(_indexes != nullptr && _indexesNumber > 0)
	{
		//_allocator->Deallocate(_indexes);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _indexes);
		_indexes = nullptr;
		_indexesNumber = 0;
	}
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

void AnimaModel::SetVertices(AnimaVertex4f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();
	
	_verticesNumber = n;
	_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, n, _allocator);
	
	for (int i = 0; i < _verticesNumber; i++)
		_vertices[i] = v[i];
	//memcpy(_vertices, v, sizeof(AnimaVertex4f) * _verticesNumber);
}

void AnimaModel::SetVertices(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();

	_verticesNumber = n;
	_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, n, _allocator);
	
	for(int i = 0; i < _verticesNumber; i++)
	{
		_vertices[i][0] = v[i][0];
		_vertices[i][1] = v[i][1];
		_vertices[i][2] = v[i][2];
		_vertices[i][0] = 1.0f;
	}
}

void AnimaModel::AddVertex(const AnimaVertex4f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaVertex4f* tmpOldVertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _verticesNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _verticesNumber; i++)
		tmpOldVertices[i] = _vertices[i];
	
	//_allocator->Deallocate(_vertices);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _vertices);
	
	_verticesNumber++;
	_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _verticesNumber, _allocator);
	
	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _verticesNumber - 1; i++)
		_vertices[i] = tmpOldVertices[i];

	_vertices[_verticesNumber - 1] = v;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldVertices);
}

void AnimaModel::AddVertex(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaVertex4f* tmpOldVertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _verticesNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _verticesNumber; i++)
		tmpOldVertices[i] = _vertices[i];

	//_allocator->Deallocate(_vertices);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _vertices);

	_verticesNumber++;
	_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _verticesNumber, _allocator);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _verticesNumber - 1; i++)
		_vertices[i] = tmpOldVertices[i];

	_vertices[_verticesNumber - 1][0] = v[0];
	_vertices[_verticesNumber - 1][1] = v[1];
	_vertices[_verticesNumber - 1][2] = v[2];
	_vertices[_verticesNumber - 1][3] = 1.0f;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldVertices);
}

void AnimaModel::SetNormals(AnimaVertex4f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	_normalsNumber = n;
	_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);
	
	//memcpy(_normals, v, sizeof(AnimaVertex4f) * _normalsNumber);
	for (int i = 0; i < _normalsNumber; i++)
		_normals[i] = v[i];
}

void AnimaModel::SetNormals(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	_normalsNumber = n;
	_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);

	for (int i = 0; i < _normalsNumber; i++)
	{
		_normals[i][0] = v[i][0];
		_normals[i][1] = v[i][1];
		_normals[i][2] = v[i][2];
		_normals[i][0] = 1.0f;
	}
}

void AnimaModel::AddNormal(const AnimaVertex4f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaVertex4f* tmpOldNormals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _normalsNumber; i++)
		tmpOldNormals[i] = _normals[i];

	//_allocator->Deallocate(_normals);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _normals);

	_normalsNumber++;
	_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _normalsNumber - 1; i++)
		_normals[i] = tmpOldNormals[i];

	_normals[_normalsNumber - 1] = v;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldNormals);
}

void AnimaModel::AddNormal(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaVertex4f* tmpOldNormals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _normalsNumber; i++)
		tmpOldNormals[i] = _normals[i];

	//_allocator->Deallocate(_normals);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _normals);

	_normalsNumber++;
	_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _normalsNumber - 1; i++)
		_normals[i] = tmpOldNormals[i];

	_normals[_normalsNumber - 1][0] = v[0];
	_normals[_normalsNumber - 1][1] = v[1];
	_normals[_normalsNumber - 1][2] = v[2];
	_normals[_normalsNumber - 1][3] = 1.0;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldNormals);
}

void AnimaModel::SetTextureCoords(AnimaVertex2f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0);
	ANIMA_ASSERT(_allocator != nullptr);
	ClearTextureCoords();
	
	_textureCoordsNumber = n;
	_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber, _allocator);
	
	//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
	for (int i = 0; i < _textureCoordsNumber; i++)
		_textureCoords[i] = v[i];
}

void AnimaModel::AddTextureCoord(const AnimaVertex2f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaVertex2f* tmpOldTextureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _textureCoordsNumber; i++)
		tmpOldTextureCoords[i] = _textureCoords[i];

	//_allocator->Deallocate(_normals);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);

	_textureCoordsNumber++;
	_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber, _allocator);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _textureCoordsNumber - 1; i++)
		_textureCoords[i] = tmpOldTextureCoords[i];

	_textureCoords[_textureCoordsNumber - 1] = v;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldTextureCoords);
}

void AnimaModel::SetIndexes(ASizeT* indexes, ASizeT n)
{
	ANIMA_ASSERT(indexes != nullptr && n > 0);
	ANIMA_ASSERT(_allocator != nullptr);
	ClearIndexes();

	_indexesNumber = n;
	_indexes = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _indexesNumber);

	//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
	for (int i = 0; i < _indexesNumber; i++)
		_indexes[i] = indexes[i];
}

void AnimaModel::AddIndex(const ASizeT& index)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ASizeT* tmpOldIndexes = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _indexesNumber);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _indexesNumber; i++)
		tmpOldIndexes[i] = _indexes[i];

	//_allocator->Deallocate(_normals);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);

	_indexesNumber++;
	_indexes = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _indexesNumber);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _indexesNumber - 1; i++)
		_indexes[i] = tmpOldIndexes[i];

	_indexes[_indexesNumber - 1] = index;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldIndexes);
}


void AnimaModel::SetChildren(AnimaModel* children, ASizeT n)
{
	ANIMA_ASSERT(children != nullptr && n > 0);
	ANIMA_ASSERT(_allocator != nullptr);
	ClearChildren();

	_modelChildrenNumber = n;
	_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

	//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
	for (int i = 0; i < _indexesNumber; i++)
		_modelChildren[i] = children[i];
}

void AnimaModel::AddChild(const AnimaModel& child)
{
	ANIMA_ASSERT(_allocator != nullptr);
	AnimaModel* tmpOldChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

	//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
	for (int i = 0; i < _modelChildrenNumber; i++)
		tmpOldChildren[i] = _modelChildren[i];

	//_allocator->Deallocate(_normals);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);

	_modelChildrenNumber++;
	_modelChildren = AnimaAllocatorNamespace::AllocateArray<AnimaModel>(*_allocator, _modelChildrenNumber, _allocator);

	//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
	for (int i = 0; i < _modelChildrenNumber - 1; i++)
		_modelChildren[i] = tmpOldChildren[i];

	_modelChildren[_modelChildrenNumber - 1] = child;

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldChildren);
}

END_ANIMA_ENGINE_CORE_NAMESPACE




