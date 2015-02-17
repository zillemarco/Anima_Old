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
#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#ifndef min
#	define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#	define max(a,b) (a > b ? a : b)
#endif

AnimaMesh::AnimaMesh(AnimaAllocator* allocator)
: _meshName(allocator)
{
	ANIMA_ASSERT(allocator != nullptr)
	_allocator = allocator;
	
	_vertices = nullptr;
	_normals = nullptr;
	_textureCoords = nullptr;
	_tangents = nullptr;
	_bitangents = nullptr;
	_faces = nullptr;

	_indexesBufferObject = 0;
	_verticesBufferObject = 0;
	//_colorsBufferObject = 0;
	_normalsBufferObject = 0;
	_textureCoordsBufferObject = 0;
	_tangentsBufferObject = 0;
	_bitangentsBufferObject = 0;
	_vertexArrayObject = 0;
	_needsBuffersUpdate = true;

	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_tangentsNumber = 0;
	_bitangentsNumber = 0;
	_facesNumber = 0;
	_meshName = "_mesh_";
	_parentModel = nullptr;

	_material = nullptr;
}

AnimaMesh::AnimaMesh(const AnimaMesh& src)
	: _meshName(src._meshName)
{
	_allocator = src._allocator;

	_indexesBufferObject = src._indexesBufferObject;
	_verticesBufferObject = src._verticesBufferObject;
	//_colorsBufferObject = src._colorsBufferObject;
	_normalsBufferObject = src._normalsBufferObject;
	_textureCoordsBufferObject = src._textureCoordsBufferObject;
	_tangentsBufferObject = src._tangentsBufferObject;
	_bitangentsBufferObject = src._bitangentsBufferObject;
	_vertexArrayObject = src._vertexArrayObject;
	_needsBuffersUpdate = src._needsBuffersUpdate;
	_material = src._material;
	
	_vertices = nullptr;
	_normals = nullptr;
	_textureCoords = nullptr;
	_tangents = nullptr;
	_bitangents = nullptr;
	_faces = nullptr;
	
	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_tangents = 0;
	_bitangents = 0;
	_facesNumber = 0;

	_parentModel = src._parentModel;
	
	SetVertices(src._vertices, src._verticesNumber);
	SetNormals(src._normals, src._normalsNumber);
	SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
	SetTangents(src._tangents, src._tangentsNumber);
	SetBitangents(src._bitangents, src._bitangentsNumber);
	SetFaces(src._faces, src._facesNumber);
}

AnimaMesh::AnimaMesh(AnimaMesh&& src)
	: _vertices(src._vertices)
	, _verticesNumber(src._verticesNumber)
	, _normals(src._normals)
	, _normalsNumber(src._normalsNumber)
	, _textureCoords(src._textureCoords)
	, _textureCoordsNumber(src._textureCoordsNumber)
	, _tangents(src._tangents)
	, _tangentsNumber(src._tangentsNumber)
	, _bitangents(src._bitangents)
	, _bitangentsNumber(src._bitangentsNumber)
	, _faces(src._faces)
	, _facesNumber(src._facesNumber)
	, _material(src._material)
	, _meshName(src._meshName)
	, _parentModel(src._parentModel)
	, _vertexArrayObject(src._vertexArrayObject)
	, _indexesBufferObject(src._indexesBufferObject)
	, _verticesBufferObject(src._verticesBufferObject)
	//, _colorsBufferObject(src._colorsBufferObject)
	, _normalsBufferObject(src._normalsBufferObject)
	, _textureCoordsBufferObject(src._textureCoordsBufferObject)
	, _tangentsBufferObject(src._tangentsBufferObject)
	, _allocator(src._allocator)
	, _needsBuffersUpdate(src._needsBuffersUpdate)
{
	src._vertices = nullptr;
	src._normals = nullptr;
	src._textureCoords = nullptr;
	src._faces = nullptr;
	src._tangents = nullptr;
	src._bitangents = nullptr;
	
	src._verticesNumber = 0;
	src._normalsNumber = 0;
	src._textureCoordsNumber = 0;
	src._tangentsNumber = 0;
	src._bitangentsNumber = 0;
	src._facesNumber = 0;
}

AnimaMesh::~AnimaMesh()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearTangents();
	ClearBitangents();
	ClearFaces();
}

AnimaMesh& AnimaMesh::operator=(const AnimaMesh& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		//_colorsBufferObject = src._colorsBufferObject;
		_normalsBufferObject = src._normalsBufferObject;
		_textureCoordsBufferObject = src._textureCoordsBufferObject;
		_tangentsBufferObject = src._tangentsBufferObject;
		_bitangentsBufferObject = src._bitangentsBufferObject;
		_vertexArrayObject = src._vertexArrayObject;
		_needsBuffersUpdate = src._needsBuffersUpdate;
		_material = src._material;
		_parentModel = src._parentModel;
		
		SetMeshName(src._meshName);
		SetVertices(src._vertices, src._verticesNumber);
		SetNormals(src._normals, src._normalsNumber);
		SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
		SetTangents(src._tangents, src._tangentsNumber);
		SetBitangents(src._bitangents, src._bitangentsNumber);
		SetFaces(src._faces, src._facesNumber);
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
		_tangents = src._tangents;
		_bitangents = src._bitangents;
		_faces = src._faces;
		
		_verticesNumber = src._verticesNumber;
		_normalsNumber = src._normalsNumber;
		_textureCoordsNumber = src._textureCoordsNumber;
		_tangentsNumber = src._tangentsNumber;
		_bitangentsNumber = src._bitangentsNumber;
		_facesNumber = src._facesNumber;

		_material = src._material;
		_parentModel = src._parentModel;

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		//_colorsBufferObject = src._colorsBufferObject;
		_normalsBufferObject = src._normalsBufferObject;
		_textureCoordsBufferObject = src._textureCoordsBufferObject;
		_tangentsBufferObject = src._tangentsBufferObject;
		_bitangentsBufferObject = src._bitangentsBufferObject;
		_vertexArrayObject = src._vertexArrayObject;
		_needsBuffersUpdate = src._needsBuffersUpdate;

		_meshName = src._meshName;
		
		src._vertices = nullptr;
		src._normals = nullptr;
		src._textureCoords = nullptr;
		src._tangents = nullptr;
		src._bitangents = nullptr;
		src._faces = nullptr;
		
		src._verticesNumber = 0;
		src._normalsNumber = 0;
		src._textureCoordsNumber = 0;
		src._tangentsNumber = 0;
		src._bitangentsNumber = 0;
		src._facesNumber = 0;
	}
	
	return *this;
}

void AnimaMesh::ClearVertices()
{
	if(_vertices != nullptr && _verticesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _vertices);
		_vertices = nullptr;
		_verticesNumber = 0;
	}
}

void AnimaMesh::ClearNormals()
{
	if(_normals != nullptr && _normalsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _normals);
		_normals = nullptr;
		_normalsNumber = 0;
	}
}

void AnimaMesh::ClearTextureCoords()
{
	if(_textureCoords != nullptr && _textureCoordsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);
		_textureCoords = nullptr;
		_textureCoordsNumber = 0;
	}
}

void AnimaMesh::ClearTangents()
{
	if (_tangents != nullptr && _tangentsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _tangents);
		_tangents = nullptr;
		_tangentsNumber = 0;
	}
}

void AnimaMesh::ClearBitangents()
{
	if (_bitangents != nullptr && _bitangentsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _bitangents);
		_bitangents = nullptr;
		_bitangentsNumber = 0;
	}
}

void AnimaMesh::ClearFaces()
{
	if(_faces != nullptr && _facesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _faces);
		_faces = nullptr;
		_facesNumber = 0;
	}
}

void AnimaMesh::SetVertices(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearVertices();
	
	if(v != nullptr && n > 0)
	{
		_verticesNumber = n;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, n);
		
		for(int i = 0; i < _verticesNumber; i++)
			_vertices[i] = v[i];
	}
}

void AnimaMesh::AddVertex(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if(_verticesNumber > 0)
	{
		AnimaVertex3f* tmpOldVertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _verticesNumber);
	
		for (int i = 0; i < _verticesNumber; i++)
			tmpOldVertices[i] = _vertices[i];
	
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _vertices);
	
		_verticesNumber++;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _verticesNumber);
	
		for (int i = 0; i < _verticesNumber - 1; i++)
			_vertices[i] = tmpOldVertices[i];

		_vertices[_verticesNumber - 1] = v;
	
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldVertices);
	}
	else
	{
		_verticesNumber++;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _verticesNumber);
		
		_vertices[_verticesNumber - 1] = v;
	}
}

void AnimaMesh::SetNormals(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr)
	ClearNormals();
	
	if(v != nullptr && n > 0)
	{
		_normalsNumber = n;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _normalsNumber);
	
		for (int i = 0; i < _normalsNumber; i++)
			_normals[i] = v[i];
	}
}

void AnimaMesh::AddNormal(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if(_normalsNumber > 0)
	{
		AnimaVertex3f* tmpOldNormals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _normalsNumber);
	
		for (int i = 0; i < _normalsNumber; i++)
			tmpOldNormals[i] = _normals[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _normals);
	
		_normalsNumber++;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _normalsNumber);
	
		for (int i = 0; i < _normalsNumber - 1; i++)
			_normals[i] = tmpOldNormals[i];
	
		_normals[_normalsNumber - 1] = v;
	
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldNormals);
	}
	else
	{
		_normalsNumber++;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _normalsNumber);

		_normals[_normalsNumber - 1] = v;
	}
}

void AnimaMesh::SetTextureCoords(AnimaVertex2f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearTextureCoords();
	
	if(v != nullptr && n > 0)
	{
		_textureCoordsNumber = n;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber);
	
		for (int i = 0; i < _textureCoordsNumber; i++)
			_textureCoords[i] = v[i];
	}
}

void AnimaMesh::AddTextureCoord(const AnimaVertex2f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if(_textureCoordsNumber > 0)
	{
		AnimaVertex2f* tmpOldTextureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber);
		
		for (int i = 0; i < _textureCoordsNumber; i++)
			tmpOldTextureCoords[i] = _textureCoords[i];
	
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);
	
		_textureCoordsNumber++;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber);
	
		for (int i = 0; i < _textureCoordsNumber - 1; i++)
			_textureCoords[i] = tmpOldTextureCoords[i];
	
		_textureCoords[_textureCoordsNumber - 1] = v;
	
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldTextureCoords);
	}
	else
	{
		_textureCoordsNumber++;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _textureCoordsNumber);
		
		_textureCoords[_textureCoordsNumber - 1] = v;
	}
}

void AnimaMesh::SetTangents(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearTangents();

	if (v != nullptr && n > 0)
	{
		_tangentsNumber = n;
		_tangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _tangentsNumber);

		for (int i = 0; i < _tangentsNumber; i++)
			_tangents[i] = v[i];
	}
}

void AnimaMesh::AddTangent(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if (_tangentsNumber > 0)
	{
		AnimaVertex2f* tmpOldTangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _tangentsNumber);

		for (int i = 0; i < _tangentsNumber; i++)
			tmpOldTangents[i] = _tangents[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _tangents);

		_tangentsNumber++;
		_tangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _tangentsNumber);

		for (int i = 0; i < _tangentsNumber - 1; i++)
			_textureCoords[i] = tmpOldTangents[i];

		_tangents[_tangentsNumber - 1] = v;

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldTangents);
	}
	else
	{
		_tangentsNumber++;
		_tangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _tangentsNumber);

		_tangents[_tangentsNumber - 1] = v;
	}
}

void AnimaMesh::SetBitangents(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearBitangents();

	if (v != nullptr && n > 0)
	{
		_bitangentsNumber = n;
		_bitangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _bitangentsNumber);

		for (int i = 0; i < _bitangentsNumber; i++)
			_bitangents[i] = v[i];
	}
}

void AnimaMesh::AddBitangent(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if (_bitangentsNumber > 0)
	{
		AnimaVertex2f* tmpOldBitangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, _bitangentsNumber);

		for (int i = 0; i < _bitangentsNumber; i++)
			tmpOldBitangents[i] = _bitangents[i];

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _bitangents);

		_bitangentsNumber++;
		_bitangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _bitangentsNumber);

		for (int i = 0; i < _bitangentsNumber - 1; i++)
			_textureCoords[i] = tmpOldBitangents[i];

		_bitangents[_bitangentsNumber - 1] = v;

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldBitangents);
	}
	else
	{
		_bitangentsNumber++;
		_bitangents = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, _bitangentsNumber);

		_bitangents[_bitangentsNumber - 1] = v;
	}
}

void AnimaMesh::SetFaces(AnimaFace* faces, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearFaces();
	
	if(faces != nullptr && n > 0)
	{
		_facesNumber = n;
		_faces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*_allocator, _facesNumber, _allocator);
	
		for (int i = 0; i < _facesNumber; i++)
			_faces[i] = faces[i];
	}
}

void AnimaMesh::AddFace(const AnimaFace& index)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if(_facesNumber > 0)
	{
		AnimaFace* tmpOldFaces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*_allocator, _facesNumber, _allocator);
	
		for (int i = 0; i < _facesNumber; i++)
			tmpOldFaces[i] = _faces[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureCoords);
	
		_facesNumber++;
		_faces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*_allocator, _facesNumber, _allocator);
	
		for (int i = 0; i < _facesNumber - 1; i++)
			_faces[i] = tmpOldFaces[i];
	
		_faces[_facesNumber - 1] = index;
	
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldFaces);
	}
	else
	{
		_facesNumber++;
		_faces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*_allocator, _facesNumber, _allocator);
		
		_faces[_facesNumber - 1] = index;
	}
}

ASizeT AnimaMesh::GetVerticesNumber()
{
	return _verticesNumber;
}

AnimaVertex3f AnimaMesh::GetVertex(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _verticesNumber);
	return _vertices[index];
}

AnimaVertex3f* AnimaMesh::GetPVertex(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _verticesNumber);
	return &_vertices[index];
}

AnimaVertex3f* AnimaMesh::GetVertices()
{
	return _vertices;
}

ASizeT AnimaMesh::GetNormalsNumber()
{
	return _normalsNumber;
}

AnimaVertex3f AnimaMesh::GetNormal(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _normalsNumber);
	return _normals[index];
}

AnimaVertex3f* AnimaMesh::GetPNormal(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _normalsNumber);
	return &_normals[index];
}

AnimaVertex3f* AnimaMesh::GetNormals()
{
	return _normals;
}

ASizeT AnimaMesh::GetTextureCoordsNumber()
{
	return _textureCoordsNumber;
}

AnimaVertex2f AnimaMesh::GetTextureCoord(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _textureCoordsNumber);
	return _textureCoords[index];
}

AnimaVertex2f* AnimaMesh::GetPTextureCoord(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _textureCoordsNumber);
	return &_textureCoords[index];
}

AnimaVertex2f* AnimaMesh::GetTextureCoords()
{
	return _textureCoords;
}

ASizeT AnimaMesh::GetTangentsNumber()
{
	return _tangentsNumber;
}

AnimaVertex3f AnimaMesh::GetTangent(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _tangentsNumber);
	return _tangents[index];
}

AnimaVertex3f* AnimaMesh::GetPTangent(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _tangentsNumber);
	return &_tangents[index];
}

AnimaVertex3f* AnimaMesh::GetTangents()
{
	return _tangents;
}

ASizeT AnimaMesh::GetBitangentsNumber()
{
	return _bitangentsNumber;
}

AnimaVertex3f AnimaMesh::GetBitangent(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _bitangentsNumber);
	return _bitangents[index];
}

AnimaVertex3f* AnimaMesh::GetPBitangent(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _bitangentsNumber);
	return &_bitangents[index];
}

AnimaVertex3f* AnimaMesh::GetBitangents()
{
	return _bitangents;
}

ASizeT AnimaMesh::GetFacesNumber()
{
	return _facesNumber;
}

AnimaFace AnimaMesh::GetFace(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _facesNumber);
	return _faces[index];
}

AnimaFace* AnimaMesh::GetPFace(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _facesNumber);
	return &_faces[index];
}

AnimaFace* AnimaMesh::GetFaces()
{
	return _faces;
}

void AnimaMesh::SetMeshName(const AnimaString& name)
{
	_meshName = name;
}

void AnimaMesh::SetMeshName(const char* name)
{
	_meshName = name;
}

AnimaString AnimaMesh::GetAnimaMeshName()
{
	return _meshName;
}

const char* AnimaMesh::GetMeshName()
{
	return _meshName.GetConstBuffer();
}

bool AnimaMesh::CreateBuffers()
{
	if (!CreateVertexArrayObject())
		return false;

	if (!CreateIndicesBuffer())
		return false;

	if (!CreateVerticesBuffer())
		return false;

	if (!CreateNormalsBuffer())
		return false;

	if (!CreateTangentsBuffer())
		return false;

	if (!CreateBitangentsBuffer())
		return false;

	//if (!CreateColorsBuffer())
	//	return false;

	return CreateTextureCoordsBuffer();
}

void AnimaMesh::UpdateBuffers()
{
	if (!AreBuffersCreated())
	{
		ANIMA_ASSERT(CreateBuffers());
	}
	else
	{
		glInvalidateBufferData(_verticesBufferObject);
		glInvalidateBufferData(_indexesBufferObject);
		//glInvalidateBufferData(_colorsBufferObject);
		glInvalidateBufferData(_normalsBufferObject);
		glInvalidateBufferData(_textureCoordsBufferObject);
		glInvalidateBufferData(_tangentsBufferObject);
		glInvalidateBufferData(_bitangentsBufferObject);
	}
	
	glBindVertexArray(_vertexArrayObject);
	
	AUint* indexes = GetFacesIndices();
	ANIMA_ASSERT(indexes != nullptr);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AUint) * GetFacesIndicesCount(), indexes, GL_STATIC_DRAW);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, indexes);
	indexes = nullptr;
	
	float* vertices = GetFloatVertices();
	ANIMA_ASSERT(vertices != nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetFloatVerticesCount(), vertices, GL_STATIC_DRAW);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, vertices);
	vertices = nullptr;

	if (GetFloatVerticesNormalCount() > 0)
	{
		float* normals = GetFloatVerticesNormal();
		glBindBuffer(GL_ARRAY_BUFFER, _normalsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetFloatVerticesNormalCount(), normals, GL_STATIC_DRAW);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, normals);
		normals = nullptr;
	}
	
	//if (GetFloatVerticesColorCount() > 0)
	//{
	//	float* colors = GetFloatVerticesColor();
	//	glBindBuffer(GL_ARRAY_BUFFER, _colorsBufferObject);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetFloatVerticesColorCount(), colors, GL_STATIC_DRAW);

	//	for (int i = 0; i < GetFloatVerticesColorCount(); i += 3)
	//		printf("VertCol:\t%f\t%f\t%f\n", colors[i], colors[i + 1], colors[i + 2]);

	//	AnimaAllocatorNamespace::DeallocateArray(*_allocator, colors);
	//	colors = nullptr;
	//}
	
	if (GetFloatVerticesTextureCoordCount() > 0)
	{
		float* textureCoords = GetFloatVerticesTextureCoord();
		glBindBuffer(GL_ARRAY_BUFFER, _textureCoordsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetFloatVerticesTextureCoordCount(), textureCoords, GL_STATIC_DRAW);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, textureCoords);
		textureCoords = nullptr;
	}

	if (GetFloatVerticesTangentsCount() > 0)
	{
		float* tangents = GetFloatVerticesTangents();
		glBindBuffer(GL_ARRAY_BUFFER, _tangentsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetFloatVerticesTangentsCount(), tangents, GL_STATIC_DRAW);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tangents);
		tangents = nullptr;
	}

	if (GetFloatVerticesBitangentsCount() > 0)
	{
		float* bitangents = GetFloatVerticesBitangents();
		glBindBuffer(GL_ARRAY_BUFFER, _bitangentsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * GetFloatVerticesBitangentsCount(), bitangents, GL_STATIC_DRAW);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, bitangents);
		bitangents = nullptr;
	}

	_needsBuffersUpdate = false;
}

bool AnimaMesh::AreBuffersCreated()
{
	return IsIndicesBufferCreated() && IsVerticesBufferCreated() && IsVertexArrayObjectCreated() && IsTextureCoordsBufferCreated() && IsNormalsBufferCreated() && IsTangentsBufferCreated() && IsBitangentsBufferCreated();// && IsColorsBufferCreated();
}

bool AnimaMesh::IsIndicesBufferCreated()
{
	return _indexesBufferObject > 0;
}

bool AnimaMesh::IsVerticesBufferCreated()
{
	return _verticesBufferObject > 0;
}

//bool AnimaMesh::IsColorsBufferCreated()
//{
//	return _colorsBufferObject > 0;
//}

bool AnimaMesh::IsNormalsBufferCreated()
{
	return _normalsBufferObject > 0;
}

bool AnimaMesh::IsTextureCoordsBufferCreated()
{
	return _textureCoordsBufferObject > 0;
}

bool AnimaMesh::IsTangentsBufferCreated()
{
	return _tangentsBufferObject > 0;
}

bool AnimaMesh::IsBitangentsBufferCreated()
{
	return _bitangentsBufferObject > 0;
}

bool AnimaMesh::IsVertexArrayObjectCreated()
{
	return _vertexArrayObject > 0;
}

bool AnimaMesh::CreateIndicesBuffer()
{
	if (IsIndicesBufferCreated())
		return true;

	glGenBuffers(1, &_indexesBufferObject);
	if (_indexesBufferObject <= 0)
		return false;

	return true;
}

bool AnimaMesh::CreateNormalsBuffer()
{
	if (IsNormalsBufferCreated())
		return true;

	glGenBuffers(1, &_normalsBufferObject);
	if (_normalsBufferObject <= 0)
		return false;

	return true;
}

//bool AnimaMesh::CreateColorsBuffer()
//{
//	if (IsColorsBufferCreated())
//		return true;
//
//	glGenBuffers(1, &_colorsBufferObject);
//	if (_colorsBufferObject <= 0)
//		return false;
//
//	return true;
//}

bool AnimaMesh::CreateTextureCoordsBuffer()
{
	if (IsTextureCoordsBufferCreated())
		return true;

	glGenBuffers(1, &_textureCoordsBufferObject);
	if (_textureCoordsBufferObject <= 0)
		return false;

	return true;
}

bool AnimaMesh::CreateTangentsBuffer()
{
	if (IsTangentsBufferCreated())
		return true;

	glGenBuffers(1, &_tangentsBufferObject);
	if (_tangentsBufferObject <= 0)
		return false;

	return true;
}

bool AnimaMesh::CreateBitangentsBuffer()
{
	if (IsBitangentsBufferCreated())
		return true;

	glGenBuffers(1, &_bitangentsBufferObject);
	if (_bitangentsBufferObject <= 0)
		return false;

	return true;
}

bool AnimaMesh::CreateVerticesBuffer()
{
	if (IsVerticesBufferCreated())
		return true;
	
	glGenBuffers(1, &_verticesBufferObject);
	if (_verticesBufferObject <= 0)
		return false;
	
	return true;
}

bool AnimaMesh::CreateVertexArrayObject()
{
	if (IsVertexArrayObjectCreated())
		return true;

	glGenVertexArrays(1, &_vertexArrayObject);
	if (_vertexArrayObject <= 0)
		return false;

	return true;
}

void AnimaMesh::SetUpdateBuffers(bool bUpdate)
{
	_needsBuffersUpdate = bUpdate;
}

bool AnimaMesh::NeedsBuffersUpdate()
{
	return _needsBuffersUpdate;
}

AUint AnimaMesh::GetFacesIndicesCount()
{
	AUint count = 0;
	for (int i = 0; i < _facesNumber; i++)
		count += (AUint)_faces[i].GetIndexesCount();
	return count;
}

AUint* AnimaMesh::GetFacesIndices()
{
	AUint* indexes = nullptr;
	ASizeT count = GetFacesIndicesCount();
	ASizeT copied = 0;
	ASizeT offset = 0;

	if (count > 0)
	{
		indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, count);

		for (int i = 0; i < _facesNumber; i++)
		{
			copied = _faces[i].GetIndexesCount();
			_faces[i].GetConstIndexes(indexes + offset, copied);
			offset += copied;
		}
	}

	return indexes;
}

AUint AnimaMesh::GetFloatVerticesCount()
{
	return (AUint)_verticesNumber * 3;
}

float* AnimaMesh::GetFloatVertices()
{
	float* vertices = nullptr;
	ASizeT count = GetFloatVerticesCount();
	ASizeT offset = 0;

	if (count > 0)
	{
		vertices = AnimaAllocatorNamespace::AllocateArray<float>(*_allocator, count);
		
		for (int i = 0; i < _verticesNumber; i++)
		{
			memcpy(vertices + offset, _vertices[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return vertices;
}

AUint AnimaMesh::GetFloatVerticesNormalCount()
{
	return (AUint)_normalsNumber * 3;
}

float* AnimaMesh::GetFloatVerticesNormal()
{
	float* normals = nullptr;
	ASizeT count = GetFloatVerticesNormalCount();
	ASizeT offset = 0;

	if (count > 0)
	{
		normals = AnimaAllocatorNamespace::AllocateArray<float>(*_allocator, count);

		for (int i = 0; i < _normalsNumber; i++)
		{
			memcpy(normals + offset, _normals[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return normals;
}

//AUint AnimaMesh::GetFloatVerticesColorCount()
//{
//	return 0;
//}
//
//float* AnimaMesh::GetFloatVerticesColor()
//{
//	return nullptr;
//}

AUint AnimaMesh::GetFloatVerticesTextureCoordCount()
{
	return (AUint)_textureCoordsNumber * 2;
}

float* AnimaMesh::GetFloatVerticesTextureCoord()
{
	float* textureCoords = nullptr;
	ASizeT count = GetFloatVerticesTextureCoordCount();
	ASizeT offset = 0;

	if (count > 0)
	{
		textureCoords = AnimaAllocatorNamespace::AllocateArray<float>(*_allocator, count);

		for (int i = 0; i < _verticesNumber; i++)
		{
			memcpy(textureCoords + offset, _textureCoords[i].vec, sizeof(AFloat) * 2);
			offset += 2;
		}
	}

	return textureCoords;
}

AUint AnimaMesh::GetFloatVerticesTangentsCount()
{
	return (AUint)_tangentsNumber * 3;
}

float* AnimaMesh::GetFloatVerticesTangents()
{
	float* tangents = nullptr;
	ASizeT count = GetFloatVerticesTangentsCount();
	ASizeT offset = 0;

	if (count > 0)
	{
		tangents = AnimaAllocatorNamespace::AllocateArray<float>(*_allocator, count);

		for (int i = 0; i < _verticesNumber; i++)
		{
			memcpy(tangents + offset, _tangents[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return tangents;
}

AUint AnimaMesh::GetFloatVerticesBitangentsCount()
{
	return (AUint)_bitangentsNumber * 3;
}

float* AnimaMesh::GetFloatVerticesBitangents()
{
	float* bitangents = nullptr;
	ASizeT count = GetFloatVerticesBitangentsCount();
	ASizeT offset = 0;

	if (count > 0)
	{
		bitangents = AnimaAllocatorNamespace::AllocateArray<float>(*_allocator, count);

		for (int i = 0; i < _verticesNumber; i++)
		{
			memcpy(bitangents + offset, _bitangents[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return bitangents;
}

void AnimaMesh::SetMaterial(AnimaMaterial* material)
{
	_material = material;
}

AnimaMaterial* AnimaMesh::GetMaterial()
{
	return _material;
}

AUint AnimaMesh::GetVertexArrayObject()
{
	return _vertexArrayObject;
}

AUint AnimaMesh::GetIndexesBufferObject()
{
	return _indexesBufferObject;
}

AUint AnimaMesh::GetVerticesBufferObject()
{
	return _verticesBufferObject;
}

AUint AnimaMesh::GetNormalsBufferObject()
{
	return _normalsBufferObject;
}

//AUint AnimaMesh::GetColorsBufferObject()
//{
//	return _colorsBufferObject;
//}

AUint AnimaMesh::GetTextureCoordsBufferObject()
{
	return _textureCoordsBufferObject;
}

AUint AnimaMesh::GetTangentsBufferObject()
{
	return _tangentsBufferObject;
}

AUint AnimaMesh::GetBitangentsBufferObject()
{
	return _bitangentsBufferObject;
}

void AnimaMesh::SetParent(AnimaModel* parent)
{
	_parentModel = parent;
}

AnimaModel* AnimaMesh::GetParent() const
{
	return _parentModel;
}

void AnimaMesh::ComputeBoundingBox()
{
	if (_verticesNumber <= 0)
	{
		_boundingBoxMin.x = _boundingBoxMax.x = 0.0f;
		_boundingBoxMin.y = _boundingBoxMax.y = 0.0f;
		_boundingBoxMin.z = _boundingBoxMax.z = 0.0f;
		return;
	}

	_boundingBoxMin = _vertices[0];
	_boundingBoxMax = _vertices[0];

	for (int i = 1; i < _verticesNumber; i++)
	{
		_boundingBoxMin.x = min(_boundingBoxMin.x, _vertices[i].x);
		_boundingBoxMin.y = min(_boundingBoxMin.y, _vertices[i].y);
		_boundingBoxMin.z = min(_boundingBoxMin.z, _vertices[i].z);

		_boundingBoxMax.x = max(_boundingBoxMax.x, _vertices[i].x);
		_boundingBoxMax.y = max(_boundingBoxMax.y, _vertices[i].y);
		_boundingBoxMax.z = max(_boundingBoxMax.z, _vertices[i].z);
	}
}

AnimaVertex3f AnimaMesh::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaMesh::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

AnimaMatrix AnimaMesh::GetFinalMatrix() const
{
	AnimaModel* p = _parentModel;
	AnimaMatrix m;

	while (p != nullptr)
	{
		m = m * p->GetTransformation()->GetTransformationMatrix();
		p = p->GetParent();
	}

	return m;
}

END_ANIMA_ENGINE_NAMESPACE
