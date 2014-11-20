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
	_indices = nullptr;
	
	_modelChildrenNumber = 0;
	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_indicesNumber = 0;
}

AnimaModel::~AnimaModel()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearChildren();
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearIndices();
}

void AnimaModel::ClearVertices()
{
	if(_vertices != nullptr && _verticesNumber > 0)
	{
		_allocator->Deallocate(_vertices);
		_vertices = nullptr;
		_verticesNumber = 0;
	}
}

void AnimaModel::ClearNormals()
{
	if(_normals != nullptr && _normalsNumber > 0)
	{
		_allocator->Deallocate(_normals);
		_normals = nullptr;
		_normalsNumber = 0;
	}
}

void AnimaModel::ClearTextureCoords()
{
	if(_textureCoords != nullptr && _textureCoordsNumber > 0)
	{
		_allocator->Deallocate(_textureCoords);
		_textureCoords = nullptr;
		_textureCoordsNumber = 0;
	}
}

void AnimaModel::ClearIndices()
{
	if(_indices != nullptr && _indicesNumber > 0)
	{
		_allocator->Deallocate(_indices);
		_indices = nullptr;
		_indicesNumber = 0;
	}
}

void AnimaModel::SetVertices(AnimaVertex4f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();
	
	_verticesNumber = n;
	_vertices = (AnimaVertex4f*)_allocator->Allocate(sizeof(AnimaVertex4f) * _verticesNumber, ANIMA_ENGINE_CORE_ALIGN_OF(AnimaVertex4f));
	
	memcpy(_vertices, v, sizeof(AnimaVertex4f) * _verticesNumber);
}

void AnimaModel::SetVertices(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();
	
	_verticesNumber = n;
	_vertices = (AnimaVertex4f*)_allocator->Allocate(sizeof(AnimaVertex4f) * _verticesNumber, ANIMA_ENGINE_CORE_ALIGN_OF(AnimaVertex4f));
	
	for(int i = 0; i < _verticesNumber; i++)
	{
		_vertices[i][0] = v[i][0];
		_vertices[i][1] = v[i][1];
		_vertices[i][2] = v[i][2];
		_vertices[i][0] = 1.0f;
	}
}

void AnimaModel::SetNormals(AnimaVertex4f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	_normalsNumber = n;
	_normals = (AnimaVertex4f*)_allocator->Allocate(sizeof(AnimaVertex4f) * _normalsNumber, ANIMA_ENGINE_CORE_ALIGN_OF(AnimaVertex4f));
	
	memcpy(_normals, v, sizeof(AnimaVertex4f) * _normalsNumber);
}

void AnimaModel::SetNormals(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	_normalsNumber = n;
	_normals = (AnimaVertex4f*)_allocator->Allocate(sizeof(AnimaVertex4f) * _normalsNumber, ANIMA_ENGINE_CORE_ALIGN_OF(AnimaVertex4f));
	
	for(int i = 0; i < _verticesNumber; i++)
	{
		(_normals[i])[0] = (v[i])[0];
		(_normals[i])[1] = (v[i])[1];
		(_normals[i])[2] = (v[i])[2];
		(_normals[i])[0] = 1.0f;
	}
}

void AnimaModel::SetTextureCoords(AnimaVertex2f* v, ASizeT n)
{
	ANIMA_ASSERT(v != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearTextureCoords();
	
	_textureCoordsNumber = n;
	_textureCoords = (AnimaVertex2f*)_allocator->Allocate(sizeof(AnimaVertex2f) * _textureCoordsNumber, ANIMA_ENGINE_CORE_ALIGN_OF(AnimaVertex2f));
	
	memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
}

void AnimaModel::SetIndices(ASizeT* indices, ASizeT n)
{
	ANIMA_ASSERT(indices != nullptr && n > 0)
	ANIMA_ASSERT(_allocator != nullptr)
	ClearIndices();
	
	_indicesNumber = n;
	_indices = (ASizeT*)_allocator->Allocate(sizeof(ASizeT) * _indicesNumber, ANIMA_ENGINE_CORE_ALIGN_OF(ASizeT));
	
	memcpy(_indices, indices, sizeof(ASizeT) * _indicesNumber);
}

END_ANIMA_ENGINE_CORE_NAMESPACE




