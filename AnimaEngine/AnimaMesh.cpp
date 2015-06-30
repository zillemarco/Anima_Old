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
#include "AnimaMeshCreator.h"
#include "AnimaMeshInstance.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#ifndef min
#	define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#	define max(a,b) (a > b ? a : b)
#endif


AnimaMeshBoneInfo::AnimaMeshBoneInfo(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaNamedObject(name, allocator)
{
}

AnimaMeshBoneInfo::AnimaMeshBoneInfo(const AnimaMeshBoneInfo& src)
	: AnimaNamedObject(src)
	, _boneOffset(src._boneOffset)
	, _finalTransformation(src._finalTransformation)
{
}

AnimaMeshBoneInfo::AnimaMeshBoneInfo(AnimaMeshBoneInfo&& src)
	: AnimaNamedObject(src)
	, _boneOffset(src._boneOffset)
	, _finalTransformation(src._finalTransformation)
{
}


AnimaMeshBoneInfo& AnimaMeshBoneInfo::operator=(const AnimaMeshBoneInfo& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_boneOffset = src._boneOffset;
		_finalTransformation = src._finalTransformation;
	}

	return *this;
}

AnimaMeshBoneInfo& AnimaMeshBoneInfo::operator=(AnimaMeshBoneInfo&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_boneOffset = src._boneOffset;
		_finalTransformation = src._finalTransformation;
	}

	return *this;
}

AnimaMeshBoneInfo::~AnimaMeshBoneInfo()
{
}

void AnimaMeshBoneInfo::SetBoneOffset(AnimaMatrix boneOffset)
{
	_boneOffset = boneOffset;
}

AnimaMatrix AnimaMeshBoneInfo::GetBoneOffset() const
{
	return _boneOffset;
}

void AnimaMeshBoneInfo::SetFinalTransformation(AnimaMatrix finalTransformation)
{
	_finalTransformation = finalTransformation;
}

AnimaMatrix AnimaMeshBoneInfo::GetFinalTransformation() const
{
	return _finalTransformation;
}

AnimaMesh::AnimaMesh(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
: AnimaSceneObject(name, dataGeneratorsManager, allocator)
, _materialName(allocator)
, _meshInstances(allocator)
, _vertices(allocator)
, _normals(allocator)
, _textureCoords(allocator)
, _tangents(allocator)
, _bitangents(allocator)
, _boneWeights(allocator)
, _boneIDs(allocator)
, _faces(allocator)
, _shadersNames(allocator)
, _shaderProgramName(allocator)
{
	_material = nullptr;

	_indexesBufferObject = 0;
	_verticesBufferObject = 0;
	//_colorsBufferObject = 0;
	_normalsBufferObject = 0;
	_textureCoordsBufferObject = 0;
	_tangentsBufferObject = 0;
	_bitangentsBufferObject = 0;
	_boneWeightsBufferObject = 0;
	_boneIDsBufferObject = 0;
	_vertexArrayObject = 0;
	_needsBuffersUpdate = true;
	_visible = true;

	_materialName = "";
	_shaderProgramName = "";
}

AnimaMesh::AnimaMesh(const AnimaMesh& src)
	: AnimaSceneObject(src)
	, _meshInstances(src._meshInstances)
	, _vertices(src._vertices)
	, _normals(src._normals)
	, _textureCoords(src._textureCoords)
	, _tangents(src._tangents)
	, _bitangents(src._bitangents)
	, _boneWeights(src._boneWeights)
	, _boneIDs(src._boneIDs)
	, _faces(src._faces)
	, _vertexArrayObject(src._vertexArrayObject)
	, _indexesBufferObject(src._indexesBufferObject)
	, _verticesBufferObject(src._verticesBufferObject)
	, _shadersNames(src._shadersNames)
	, _shaderProgramName(src._shaderProgramName)
	//, _colorsBufferObject(src._colorsBufferObject)
{
	_allocator = src._allocator;
	
	_indexesBufferObject = src._indexesBufferObject;
	_verticesBufferObject = src._verticesBufferObject;
	//_colorsBufferObject = src._colorsBufferObject;
	_normalsBufferObject = src._normalsBufferObject;
	_textureCoordsBufferObject = src._textureCoordsBufferObject;
	_tangentsBufferObject = src._tangentsBufferObject;
	_bitangentsBufferObject = src._bitangentsBufferObject;
	_boneWeightsBufferObject = src._boneWeightsBufferObject;
	_boneIDsBufferObject = src._boneIDsBufferObject;
	_vertexArrayObject = src._vertexArrayObject;
	_needsBuffersUpdate = src._needsBuffersUpdate;
	_material = src._material;
	_materialName = src._materialName;
	_visible = src._visible;

	_boundingBoxMin = src._boundingBoxMin;
	_boundingBoxMax = src._boundingBoxMax;
	_boundingBoxCenter = src._boundingBoxCenter;
}

AnimaMesh::AnimaMesh(AnimaMesh&& src)
	: AnimaSceneObject(src)
	, _material(src._material)
	, _materialName(src._materialName)
	, _meshInstances(src._meshInstances)
	, _boundingBoxMin(src._boundingBoxMin)
	, _boundingBoxMax(src._boundingBoxMax)
	, _boundingBoxCenter(src._boundingBoxCenter)
	, _vertices(src._vertices)
	, _normals(src._normals)
	, _textureCoords(src._textureCoords)
	, _tangents(src._tangents)
	, _bitangents(src._bitangents)
	, _boneWeights(src._boneWeights)
	, _boneIDs(src._boneIDs)
	, _faces(src._faces)
	, _vertexArrayObject(src._vertexArrayObject)
	, _indexesBufferObject(src._indexesBufferObject)
	, _verticesBufferObject(src._verticesBufferObject)
	//, _colorsBufferObject(src._colorsBufferObject)
	, _normalsBufferObject(src._normalsBufferObject)
	, _textureCoordsBufferObject(src._textureCoordsBufferObject)
	, _tangentsBufferObject(src._tangentsBufferObject)
	, _visible(src._visible)
	, _needsBuffersUpdate(src._needsBuffersUpdate)
	, _shadersNames(src._shadersNames)
	, _shaderProgramName(src._shaderProgramName)
{
}

AnimaMesh::~AnimaMesh()
{
	ClearAll();
}

AnimaMesh& AnimaMesh::operator=(const AnimaMesh& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		//_colorsBufferObject = src._colorsBufferObject;
		_normalsBufferObject = src._normalsBufferObject;
		_textureCoordsBufferObject = src._textureCoordsBufferObject;
		_tangentsBufferObject = src._tangentsBufferObject;
		_bitangentsBufferObject = src._bitangentsBufferObject;
		_boneWeightsBufferObject = src._boneWeightsBufferObject;
		_boneIDsBufferObject = src._boneIDsBufferObject;
		_vertexArrayObject = src._vertexArrayObject;
		_needsBuffersUpdate = src._needsBuffersUpdate;
		_visible = src._visible;
		_boundingBoxMin = src._boundingBoxMin;
		_boundingBoxMax = src._boundingBoxMax;
		_boundingBoxCenter = src._boundingBoxCenter;

		_material = src._material;
		_materialName = src._materialName;
		
		_vertices = src._vertices;
		_normals = src._normals;
		_textureCoords = src._textureCoords;
		_tangents = src._tangents;
		_bitangents = src._bitangents;
		_boneWeights = src._boneWeights;
		_boneIDs = src._boneIDs;
		_faces = src._faces;

		_meshInstances = src._meshInstances;

		_shadersNames.Copy(src._shadersNames);
		_shaderProgramName = src._shaderProgramName;
	}
	
	return *this;
}

AnimaMesh& AnimaMesh::operator=(AnimaMesh&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);
		
		_vertices = src._vertices;
		_normals = src._normals;
		_textureCoords = src._textureCoords;
		_tangents = src._tangents;
		_bitangents = src._bitangents;
		_boneWeights = src._boneWeights;
		_boneIDs = src._boneIDs;
		_faces = src._faces;

		_material = src._material;
		_materialName = src._materialName;

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		//_colorsBufferObject = src._colorsBufferObject;
		_normalsBufferObject = src._normalsBufferObject;
		_textureCoordsBufferObject = src._textureCoordsBufferObject;
		_tangentsBufferObject = src._tangentsBufferObject;
		_bitangentsBufferObject = src._bitangentsBufferObject;
		_boneWeightsBufferObject = src._boneWeightsBufferObject;
		_boneIDsBufferObject = src._boneIDsBufferObject;
		_vertexArrayObject = src._vertexArrayObject;
		_needsBuffersUpdate = src._needsBuffersUpdate;
		_visible = src._visible;
		_boundingBoxMin = src._boundingBoxMin;
		_boundingBoxMax = src._boundingBoxMax;
		_boundingBoxCenter = src._boundingBoxCenter;

		_meshInstances = src._meshInstances;

		_shadersNames.Copy(src._shadersNames);
		_shaderProgramName = src._shaderProgramName;
	}
	
	return *this;
}

bool AnimaMesh::operator==(const AnimaMesh& other)
{
	return true;
}

bool AnimaMesh::operator!=(const AnimaMesh& other)
{
	return true;
}

void AnimaMesh::ClearAll()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearTangents();
	ClearBitangents();
	ClearFaces();
}

void AnimaMesh::ClearVertices()
{
	_vertices.RemoveAll();
}

void AnimaMesh::ClearNormals()
{
	_normals.RemoveAll();
}

void AnimaMesh::ClearTextureCoords()
{
	_textureCoords.RemoveAll();
}

void AnimaMesh::ClearTangents()
{
	_tangents.RemoveAll();
}

void AnimaMesh::ClearBitangents()
{
	_bitangents.RemoveAll();
}

void AnimaMesh::ClearFaces()
{
	_faces.RemoveAll();
}

void AnimaMesh::SetVertices(AnimaArray<AnimaVertex3f>* vertices)
{
	_vertices.Copy(*vertices);
}

void AnimaMesh::SetVertices(AnimaVertex3f* v, AInt n)
{
	_vertices.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_vertices.Add(v[i]);
}

void AnimaMesh::AddVertex(const AnimaVertex3f& v)
{
	_vertices.Add(v);
}

void AnimaMesh::SetNormals(AnimaArray<AnimaVertex3f>* normals)
{
	_normals.Copy(*normals);
}

void AnimaMesh::SetNormals(AnimaVertex3f* v, AInt n)
{
	_normals.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_normals.Add(v[i]);
}

void AnimaMesh::AddNormal(const AnimaVertex3f& v)
{
	_normals.Add(v);
}

void AnimaMesh::SetTextureCoords(AnimaArray<AnimaVertex2f>* textureCoords)
{
	_textureCoords.Copy(*textureCoords);
}

void AnimaMesh::SetTextureCoords(AnimaVertex2f* v, AInt n)
{
	_textureCoords.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_textureCoords.Add(v[i]);
}

void AnimaMesh::AddTextureCoord(const AnimaVertex2f& v)
{
	_textureCoords.Add(v);
}

void AnimaMesh::SetTangents(AnimaArray<AnimaVertex3f>* tangents)
{
	_tangents.Copy(*tangents);
}

void AnimaMesh::SetTangents(AnimaVertex3f* v, AInt n)
{
	_tangents.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_tangents.Add(v[i]);
}

void AnimaMesh::AddTangent(const AnimaVertex3f& v)
{
	_tangents.Add(v);
}

void AnimaMesh::SetBitangents(AnimaArray<AnimaVertex3f>* bitangents)
{
	_bitangents.Copy(*bitangents);
}

void AnimaMesh::SetBitangents(AnimaVertex3f* v, AInt n)
{
	_bitangents.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_bitangents.Add(v[i]);
}

void AnimaMesh::AddBitangent(const AnimaVertex3f& v)
{
	_bitangents.Add(v);
}

void AnimaMesh::SetBoneWeights(AnimaArray<AnimaVertex4f>* boneWeights)
{
	_boneWeights.Copy(*boneWeights);
}

void AnimaMesh::SetBoneWeights(AnimaVertex4f* v, AInt n)
{
	_boneWeights.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_boneWeights.Add(v[i]);
}

void AnimaMesh::AddBoneWeight(const AnimaVertex4f& v)
{
	_boneWeights.Add(v);
}

AInt AnimaMesh::GetBoneWeightsNumber()
{
	return _boneWeights.GetSize();
}

AnimaVertex4f AnimaMesh::GetBoneWeight(AInt index)
{
	return _boneWeights[index];
}

AnimaVertex4f* AnimaMesh::GetPBoneWeight(AInt index)
{
	return &_boneWeights[index];
}

AnimaArray<AnimaVertex4f>* AnimaMesh::GetBoneWeights()
{
	return &_boneWeights;
}

void AnimaMesh::ClearBoneWeights()
{
	_boneWeights.RemoveAll();
}

void AnimaMesh::SetBoneIDs(AnimaArray<AnimaVertex4f>* boneIDs)
{
	_boneIDs.Copy(*boneIDs);
}

void AnimaMesh::SetBoneIDs(AnimaVertex4f* v, AInt n)
{
	_boneIDs.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_boneIDs.Add(v[i]);
}

void AnimaMesh::AddBoneID(const AnimaVertex4f& v)
{
	_boneIDs.Add(v);
}

AInt AnimaMesh::GetBoneIDsNumber()
{
	return _boneIDs.GetSize();
}

AnimaVertex4f AnimaMesh::GetBoneID(AInt index)
{
	return _boneIDs[index];
}

AnimaVertex4f* AnimaMesh::GetPBoneID(AInt index)
{
	return &_boneIDs[index];
}

AnimaArray<AnimaVertex4f>* AnimaMesh::GetBoneIDs()
{
	return &_boneIDs;
}

void AnimaMesh::ClearBoneIDs()
{
	_boneIDs.RemoveAll();;
}

void AnimaMesh::SetFaces(AnimaArray<AnimaFace>* faces)
{
	_faces.Copy(*faces);
}

void AnimaMesh::SetFaces(AnimaFace* faces, AInt n)
{
	_faces.RemoveAll();

	for (AInt i = 0; i < n; i++)
		_faces.Add(faces[i]);
}

void AnimaMesh::AddFace(const AnimaFace& face)
{
	_faces.Add(face);
}

AInt AnimaMesh::GetVerticesNumber()
{
	return _vertices.GetSize();
}

AnimaVertex3f AnimaMesh::GetVertex(AInt index)
{
	return _vertices[index];
}

AnimaVertex3f* AnimaMesh::GetPVertex(AInt index)
{
	return &_vertices[index];
}

AnimaArray<AnimaVertex3f>* AnimaMesh::GetVertices()
{
	return &_vertices;
}

AInt AnimaMesh::GetNormalsNumber()
{
	return _normals.GetSize();
}

AnimaVertex3f AnimaMesh::GetNormal(AInt index)
{
	return _normals[index];
}

AnimaVertex3f* AnimaMesh::GetPNormal(AInt index)
{
	return &_normals[index];
}

AnimaArray<AnimaVertex3f>* AnimaMesh::GetNormals()
{
	return &_normals;
}

AInt AnimaMesh::GetTextureCoordsNumber()
{
	return _textureCoords.GetSize();
}

AnimaVertex2f AnimaMesh::GetTextureCoord(AInt index)
{
	return _textureCoords[index];
}

AnimaVertex2f* AnimaMesh::GetPTextureCoord(AInt index)
{
	return &_textureCoords[index];
}

AnimaArray<AnimaVertex2f>* AnimaMesh::GetTextureCoords()
{
	return &_textureCoords;
}

AInt AnimaMesh::GetTangentsNumber()
{
	return _tangents.GetSize();
}

AnimaVertex3f AnimaMesh::GetTangent(AInt index)
{
	return _tangents[index];
}

AnimaVertex3f* AnimaMesh::GetPTangent(AInt index)
{
	return &_tangents[index];
}

AnimaArray<AnimaVertex3f>* AnimaMesh::GetTangents()
{
	return &_tangents;
}

AInt AnimaMesh::GetBitangentsNumber()
{
	return _bitangents.GetSize();
}

AnimaVertex3f AnimaMesh::GetBitangent(AInt index)
{
	return _bitangents[index];
}

AnimaVertex3f* AnimaMesh::GetPBitangent(AInt index)
{
	return &_bitangents[index];
}

AnimaArray<AnimaVertex3f>* AnimaMesh::GetBitangents()
{
	return &_bitangents;
}

AInt AnimaMesh::GetFacesNumber()
{
	return _faces.GetSize();
}

AnimaFace AnimaMesh::GetFace(AInt index)
{
	return _faces[index];
}

AnimaFace* AnimaMesh::GetPFace(AInt index)
{
	return &_faces[index];
}

AnimaArray<AnimaFace>* AnimaMesh::GetFaces()
{
	return &_faces;
}

bool AnimaMesh::CanCreateBuffers()
{
	if (_vertices.GetSize() <= 0)
		return false;
	return true;
}

bool AnimaMesh::CreateBuffers()
{
	if (!CanCreateBuffers())
		return true;

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

	if (!CreateBoneWeightsBuffer())
		return false;

	if (!CreateBoneIDsBuffer())
		return false;
	
	//if (!CreateColorsBuffer())
	//	return false;

	return CreateTextureCoordsBuffer();
}

void AnimaMesh::UpdateBuffers()
{
	if (!CanCreateBuffers())
	{
		_needsBuffersUpdate = false;
		return;
	}

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
		glInvalidateBufferData(_boneWeightsBufferObject);
		glInvalidateBufferData(_boneIDsBufferObject);
	}
	
	glBindVertexArray(_vertexArrayObject);
	
	if (GetFacesIndicesCount() > 0)
	{
		AUint* indexes = GetFacesIndices();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AUint) * GetFacesIndicesCount(), indexes, GL_STATIC_DRAW);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, indexes);
		indexes = nullptr;
	}

	if (GetFloatVerticesCount() > 0)
	{
		AFloat* vertices = GetFloatVertices();
		glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesCount(), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, vertices);
		vertices = nullptr;
	}

	if (GetFloatVerticesNormalCount() > 0)
	{
		AFloat* normals = GetFloatVerticesNormal();
		glBindBuffer(GL_ARRAY_BUFFER, _normalsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesNormalCount(), normals, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, normals);
		normals = nullptr;
	}

	if (GetFloatVerticesTextureCoordCount() > 0)
	{
		AFloat* textureCoords = GetFloatVerticesTextureCoord();
		glBindBuffer(GL_ARRAY_BUFFER, _textureCoordsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesTextureCoordCount(), textureCoords, GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, textureCoords);
		textureCoords = nullptr;
	}

	if (GetFloatVerticesTangentsCount() > 0)
	{
		AFloat* tangents = GetFloatVerticesTangents();
		glBindBuffer(GL_ARRAY_BUFFER, _tangentsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesTangentsCount(), tangents, GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tangents);
		tangents = nullptr;
	}

	if (GetFloatVerticesBitangentsCount() > 0)
	{
		AFloat* bitangents = GetFloatVerticesBitangents();
		glBindBuffer(GL_ARRAY_BUFFER, _bitangentsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesBitangentsCount(), bitangents, GL_STATIC_DRAW);

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, bitangents);
		bitangents = nullptr;
	}

	if (GetFloatBoneWeightsCount() > 0)
	{
		AFloat* boneWeights = GetFloatBoneWeights();
		glBindBuffer(GL_ARRAY_BUFFER, _boneWeightsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatBoneWeightsCount(), boneWeights, GL_STATIC_DRAW);

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, boneWeights);
		boneWeights = nullptr;
	}

	if (GetFloatBoneIDsCount() > 0)
	{
		AFloat* boneIDs = GetFloatBoneIDs();
		glBindBuffer(GL_ARRAY_BUFFER, _boneIDsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatBoneIDsCount(), boneIDs, GL_STATIC_DRAW);

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, boneIDs);
		boneIDs = nullptr;
	}

	glBindVertexArray(0);

	_needsBuffersUpdate = false;
}

bool AnimaMesh::AreBuffersCreated()
{
	return IsIndicesBufferCreated() && IsVerticesBufferCreated() && IsVertexArrayObjectCreated() && IsTextureCoordsBufferCreated() && IsNormalsBufferCreated() && IsTangentsBufferCreated() && IsBitangentsBufferCreated() && IsBoneWeightsBufferCreated() && IsBoneIDsBufferCreated();// && IsColorsBufferCreated();
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

bool AnimaMesh::IsBoneWeightsBufferCreated()
{
	return _boneWeightsBufferObject > 0;
}

bool AnimaMesh::IsBoneIDsBufferCreated()
{
	return _boneIDsBufferObject > 0;
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

bool AnimaMesh::CreateBoneWeightsBuffer()
{
	if (IsBoneWeightsBufferCreated())
		return true;

	glGenBuffers(1, &_boneWeightsBufferObject);
	if (_boneWeightsBufferObject <= 0)
		return false;

	return true;
}

bool AnimaMesh::CreateBoneIDsBuffer()
{
	if (IsBoneIDsBufferCreated())
		return true;

	glGenBuffers(1, &_boneIDsBufferObject);
	if (_boneIDsBufferObject <= 0)
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
	return (AUint)_faces.GetSize() * 3;
}

AUint* AnimaMesh::GetFacesIndices()
{
	AUint* indexes = nullptr;
	AInt count = GetFacesIndicesCount();
	AInt copied = 0;
	AInt offset = 0;

	if (count > 0)
	{
		indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, count);

		AInt facesNumber = _faces.GetSize();
		for (AInt i = 0; i < facesNumber; i++)
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
	return (AUint)_vertices.GetSize() * 3;
}

AFloat* AnimaMesh::GetFloatVertices()
{
	AFloat* vertices = nullptr;
	AInt count = GetFloatVerticesCount();
	AInt offset = 0;

	if (count > 0)
	{
		vertices = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt verticesNumber = _vertices.GetSize();
		for (int i = 0; i < verticesNumber; i++)
		{
			memcpy(vertices + offset, _vertices[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return vertices;
}

AUint AnimaMesh::GetFloatVerticesNormalCount()
{
	return (AUint)_normals.GetSize() * 3;
}

AFloat* AnimaMesh::GetFloatVerticesNormal()
{
	AFloat* normals = nullptr;
	AInt count = GetFloatVerticesNormalCount();

	AInt offset = 0;

	if (count > 0)
	{
		normals = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt normalsNumber = _normals.GetSize();
		for (int i = 0; i < normalsNumber; i++)
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
//AFloat* AnimaMesh::GetFloatVerticesColor()
//{
//	return nullptr;
//}

AUint AnimaMesh::GetFloatVerticesTextureCoordCount()
{
	return (AUint)_textureCoords.GetSize() * 2;
}

AFloat* AnimaMesh::GetFloatVerticesTextureCoord()
{
	AFloat* textureCoords = nullptr;
	AInt count = GetFloatVerticesTextureCoordCount();
	AInt offset = 0;

	if (count > 0)
	{
		textureCoords = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt textureCoordsNumber = _textureCoords.GetSize();
		for (int i = 0; i < textureCoordsNumber; i++)
		{
			memcpy(textureCoords + offset, _textureCoords[i].vec, sizeof(AFloat) * 2);
			offset += 2;
		}
	}

	return textureCoords;
}

AUint AnimaMesh::GetFloatVerticesTangentsCount()
{
	return (AUint)_tangents.GetSize() * 3;
}

AFloat* AnimaMesh::GetFloatVerticesTangents()
{
	AFloat* tangents = nullptr;
	AInt count = GetFloatVerticesTangentsCount();
	AInt offset = 0;

	if (count > 0)
	{
		tangents = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt tangentsNumber = _tangents.GetSize();
		for (int i = 0; i < tangentsNumber; i++)
		{
			memcpy(tangents + offset, _tangents[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return tangents;
}

AUint AnimaMesh::GetFloatVerticesBitangentsCount()
{
	return (AUint)_bitangents.GetSize() * 3;
}

AFloat* AnimaMesh::GetFloatVerticesBitangents()
{
	AFloat* bitangents = nullptr;
	AInt count = GetFloatVerticesBitangentsCount();
	AInt offset = 0;

	if (count > 0)
	{
		bitangents = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt bitangentsNumber = _bitangents.GetSize();
		for (int i = 0; i < bitangentsNumber; i++)
		{
			memcpy(bitangents + offset, _bitangents[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return bitangents;
}

AUint AnimaMesh::GetFloatBoneWeightsCount()
{
	return (AUint)_boneWeights.GetSize() * 4;
}

float* AnimaMesh::GetFloatBoneWeights()
{
	AFloat* boneWeights = nullptr;
	AInt count = GetFloatBoneWeightsCount();
	AInt offset = 0;

	if (count > 0)
	{
		boneWeights = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt weightsNumber = _boneWeights.GetSize();
		for (int i = 0; i < weightsNumber; i++)
		{
			memcpy(boneWeights + offset, _boneWeights[i].vec, sizeof(AFloat) * 4);
			offset += 4;
		}
	}

	return boneWeights;
}

AUint AnimaMesh::GetFloatBoneIDsCount()
{
	return (AUint)_boneIDs.GetSize() * 4;
}

float* AnimaMesh::GetFloatBoneIDs()
{
	AFloat* boneIDs = nullptr;
	AInt count = GetFloatBoneIDsCount();
	AInt offset = 0;

	if (count > 0)
	{
		boneIDs = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt idsNumber = _boneIDs.GetSize();
		for (int i = 0; i < idsNumber; i++)
		{
			memcpy(boneIDs + offset, _boneIDs[i].vec, sizeof(AFloat) * 4);
			offset += 4;
		}
	}

	return boneIDs;
}

void AnimaMesh::SetIsVisible(bool visible)
{
	_visible = visible;
}

bool AnimaMesh::IsVisible()
{
	return _visible;
}

void AnimaMesh::SetMaterial(AnimaMaterial* material)
{
	_material = material;

	if (_material)
		_materialName = _material->GetAnimaName();
	else
		_materialName = "";
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

AUint AnimaMesh::GetBoneWeightsBufferObject()
{
	return _boneWeightsBufferObject;
}

AUint AnimaMesh::GetBoneIDsBufferObject()
{
	return _boneIDsBufferObject;
}

void AnimaMesh::ComputeBoundingBox(bool updateRecursively)
{
	AInt verticesNumber = _vertices.GetSize();
	if (verticesNumber <= 0)
	{
		_boundingBoxMin.x = _boundingBoxMax.x = 0.0f;
		_boundingBoxMin.y = _boundingBoxMax.y = 0.0f;
		_boundingBoxMin.z = _boundingBoxMax.z = 0.0f;
	}
	else
	{
		_boundingBoxMin.x = _boundingBoxMax.x = _vertices[0].x;
		_boundingBoxMin.y = _boundingBoxMax.y = _vertices[0].y;
		_boundingBoxMin.z = _boundingBoxMax.z = _vertices[0].z;
			
		for (AInt i = 1; i < verticesNumber; i++)
		{
			_boundingBoxMin.x = min(_boundingBoxMin.x, _vertices[i].x);
			_boundingBoxMin.y = min(_boundingBoxMin.y, _vertices[i].y);
			_boundingBoxMin.z = min(_boundingBoxMin.z, _vertices[i].z);
				
			_boundingBoxMax.x = max(_boundingBoxMax.x, _vertices[i].x);
			_boundingBoxMax.y = max(_boundingBoxMax.y, _vertices[i].y);
			_boundingBoxMax.z = max(_boundingBoxMax.z, _vertices[i].z);
		}
	}
		
	_boundingBoxCenter = AnimaVertex3f((_boundingBoxMin.x + _boundingBoxMax.x) / 2.0f, (_boundingBoxMin.y + _boundingBoxMax.y) / 2.0f, (_boundingBoxMin.z + _boundingBoxMax.z) / 2.0f);
}

AnimaVertex3f AnimaMesh::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaMesh::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

AnimaVertex3f AnimaMesh::GetBoundingBoxCenter() const
{
	return _boundingBoxCenter;
}

void AnimaMesh::AddInstance(AnimaMeshInstance* instance)
{
	if (instance != nullptr)
		_meshInstances.Add(instance->GetAnimaName(), instance);
}

AInt AnimaMesh::GetInstancesCount() const
{
	return _meshInstances.GetSize();
}

AnimaMeshInstance* AnimaMesh::GetInstance(AInt index) const
{
	return _meshInstances[index];
}

AnimaMeshInstance* AnimaMesh::GetInstanceFromName(const AnimaString& name) const
{
	return _meshInstances[name];
}

AnimaMeshInstance* AnimaMesh::GetInstanceFromName(const char* name) const
{
	return _meshInstances[name];
}

void AnimaMesh::ComputeSmootNormals()
{
	AInt facesNumber = _faces.GetSize();
	AInt verticesNumber = _vertices.GetSize();

	if (facesNumber > 0 && verticesNumber > 0)
	{
		ClearNormals();

		for (AInt i = 0; i < verticesNumber; i++)
		{
			AInt pos = GetNextFaceContainingVertex(0, i);

			if (pos >= 0)
			{
				AnimaVertex3f normal = _faces[pos].GetNormal();
				while ((pos = GetNextFaceContainingVertex(pos + 1, i)) != -1)
					normal += _faces[pos].GetNormal();

				normal.Normalize();
				_normals.Add(normal);
			}
		}
	}
}

void AnimaMesh::ComputeFlatNormals()
{
	AInt facesNumber = _faces.GetSize();

	if (facesNumber > 0)
	{
		ClearNormals();

		for (AInt i = 0; i < facesNumber; i++)
		{
			AnimaFace* face = &_faces[i];

			AnimaVertex3f p1, p2, p3, u, v;
			p1 = _vertices[face->GetIndex(0)];
			p2 = _vertices[face->GetIndex(1)];
			p3 = _vertices[face->GetIndex(2)];
			u = p2 - p1;
			v = p3 - p1;

			face->SetNormal((u ^ v).Normalized());
		}
	}
}

AInt AnimaMesh::GetNextFaceContainingVertex(AInt start, AInt vertexIndex) const
{
	AInt facesNumber = _faces.GetSize();
	for (AInt i = start; i < facesNumber; i++)
	{
		if (_faces[i].HasIndex((AUint)vertexIndex))
			return (AInt)i;
	}

	return -1;
}

void AnimaMesh::AddShader(AnimaShader* shader)
{
	_shadersNames.Add(shader->GetAnimaName());
}

void AnimaMesh::AddShader(const AnimaString& shaderName)
{
	_shadersNames.Add(shaderName);
}

void AnimaMesh::AddShader(const char* shaderName)
{
	_shadersNames.Add(AnimaString(shaderName, nullptr));
}

AInt AnimaMesh::GetShadersCount() const
{
	return _shadersNames.GetSize();
}

AnimaString AnimaMesh::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

void AnimaMesh::SetShaderProgram(const AnimaString& shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

void AnimaMesh::SetShaderProgram(const char* shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

AnimaString AnimaMesh::GetShaderProgramName() const
{
	return _shaderProgramName;
}

void AnimaMesh::MakePlane()
{
	AnimaMeshCreator::MakePlane(this, _allocator);
}

void AnimaMesh::MakeIcosahedralSphere(AInt recursionLevel)
{
	AnimaMeshCreator::MakeIcosahedralSphere(this, recursionLevel, _allocator);
}

void AnimaMesh::MakeCylinder(AFloat topRadius, AFloat bottomRadius, AFloat height, AUint radialSegments, AUint heightSegments, bool openEnded)
{
	AnimaMeshCreator::MakeCylinder(this, topRadius, bottomRadius, height, radialSegments, heightSegments, openEnded, _allocator);
}

void AnimaMesh::Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool updateMaterial)
{
	program->UpdateSceneObjectProperties(this, renderer);

	if (updateMaterial)
		program->UpdateMappedValuesObjectProperties(_material, renderer);

#ifdef _WIN32
	glBindVertexArray(GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	program->EnableInputs(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

END_ANIMA_ENGINE_NAMESPACE