//
//  AnimaMesh.cpp
//  Anima
//
//  Created by Marco Zille on 25/11/14.
//
//

#include <stdio.h>
#include <string.h>
#include "AnimaMesh.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaMesh::AnimaMesh(AnimaAllocator* allocator)
{
	ANIMA_ASSERT(allocator != nullptr)
	_allocator = allocator;
	
	_vertices = nullptr;
	_normals = nullptr;
	_textureCoords = nullptr;
	_indexes = nullptr;
	
	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_indexesNumber = 0;
}

AnimaMesh::AnimaMesh(const AnimaMesh& src)
{
	_allocator = src._allocator;
	
	SetVertices(src._vertices, src._verticesNumber);
	SetNormals(src._normals, src._normalsNumber);
	SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
	SetIndexes(src._indexes, src._indexesNumber);
}

AnimaMesh::AnimaMesh(AnimaMesh&& src)
: _vertices(src._vertices)
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
	
	src._verticesNumber = 0;
	src._normalsNumber = 0;
	src._textureCoordsNumber = 0;
	src._indexesNumber = 0;
}

AnimaMesh::~AnimaMesh()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearIndexes();
}

AnimaMesh& AnimaMesh::operator=(const AnimaMesh& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		SetVertices(src._vertices, src._verticesNumber);
		SetNormals(src._normals, src._normalsNumber);
		SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
		SetIndexes(src._indexes, src._indexesNumber);
	}
	
	return *this;
}

AnimaMesh& AnimaMesh::operator=(AnimaMesh&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		_vertices = src._vertices;
		_normals = src._normals;
		_textureCoords = src._textureCoords;
		_indexes = src._indexes;
		
		_verticesNumber = src._verticesNumber;
		_normalsNumber = src._normalsNumber;
		_textureCoordsNumber = src._textureCoordsNumber;
		_indexesNumber = src._indexesNumber;
		
		src._vertices = nullptr;
		src._normals = nullptr;
		src._textureCoords = nullptr;
		src._indexes = nullptr;
		
		src._verticesNumber = 0;
		src._normalsNumber = 0;
		src._textureCoordsNumber = 0;
		src._indexesNumber = 0;
	}
	
	return *this;
}

void AnimaMesh::ClearVertices()
{
	if(_vertices != nullptr && _verticesNumber > 0)
	{
		//_allocator->Deallocate(_vertices);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _vertices);
		_vertices = nullptr;
		_verticesNumber = 0;
	}
}

void AnimaMesh::ClearNormals()
{
	if(_normals != nullptr && _normalsNumber > 0)
	{
		//_allocator->Deallocate(_normals);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _normals);
		_normals = nullptr;
		_normalsNumber = 0;
	}
}

void AnimaMesh::ClearTextureCoords()
{
	if(_textureCoords != nullptr && _textureCoordsNumber > 0)
	{
		//_allocator->Deallocate(_textureCoords);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);
		_textureCoords = nullptr;
		_textureCoordsNumber = 0;
	}
}

void AnimaMesh::ClearIndexes()
{
	if(_indexes != nullptr && _indexesNumber > 0)
	{
		//_allocator->Deallocate(_indexes);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _indexes);
		_indexes = nullptr;
		_indexesNumber = 0;
	}
}

void AnimaMesh::SetVertices(AnimaVertex4f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();
	
	if(v != nullptr && n > 0)
	{
		_verticesNumber = n;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, n, _allocator);
	
		for (int i = 0; i < _verticesNumber; i++)
			_vertices[i] = v[i];
	}
	//memcpy(_vertices, v, sizeof(AnimaVertex4f) * _verticesNumber);
}

void AnimaMesh::SetVertices(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();
	
	if(v != nullptr && n > 0)
	{
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
}

void AnimaMesh::AddVertex(const AnimaVertex4f& v)
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

void AnimaMesh::AddVertex(const AnimaVertex3f& v)
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

void AnimaMesh::SetNormals(AnimaVertex4f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	
	if(v != nullptr && n > 0)
	{
		_normalsNumber = n;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*_allocator, _normalsNumber, _allocator);
	
		//memcpy(_normals, v, sizeof(AnimaVertex4f) * _normalsNumber);
		for (int i = 0; i < _normalsNumber; i++)
			_normals[i] = v[i];
	}
}

void AnimaMesh::SetNormals(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	if(v != nullptr && n > 0)
	{
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
}

void AnimaMesh::AddNormal(const AnimaVertex4f& v)
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

void AnimaMesh::AddNormal(const AnimaVertex3f& v)
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

void AnimaMesh::SetTextureCoords(AnimaVertex2f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearTextureCoords();
	
	if(v != nullptr && n > 0)
	{
		_textureCoordsNumber = n;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber, _allocator);
	
		//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
		for (int i = 0; i < _textureCoordsNumber; i++)
			_textureCoords[i] = v[i];
	}
}

void AnimaMesh::AddTextureCoord(const AnimaVertex2f& v)
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

void AnimaMesh::SetIndexes(ASizeT* indexes, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearIndexes();
	
	if(indexes != nullptr && n > 0)
	{
		_indexesNumber = n;
		_indexes = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _indexesNumber);
	
		//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
		for (int i = 0; i < _indexesNumber; i++)
			_indexes[i] = indexes[i];
	}
}

void AnimaMesh::AddIndex(const ASizeT& index)
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

END_ANIMA_ENGINE_CORE_NAMESPACE