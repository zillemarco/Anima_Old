//
//  AnimaGeometry.cpp
//  Anima
//
//  Created by Marco Zille on 25/11/14.
//
//

#include <stdio.h>
#include <string.h>
#include "AnimaGeometry.h"
#include "AnimaMeshCreator.h"
#include "AnimaMeshInstance.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaMaterialsManager.h"
#include "AnimaScene.h"
#include "AnimaXmlTranslators.h"

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
	IMPLEMENT_ANIMA_CLASS(AnimaMeshBoneInfo);
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

AnimaGeometry::AnimaGeometry(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaGeometry);
	_material = nullptr;

	_indexesBufferObject = 0;
	_verticesBufferObject = 0;
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
	
	_shaderSource = ASDSO_GEOMETRY;
}

AnimaGeometry::AnimaGeometry(const AnimaGeometry& src)
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
{
	_allocator = src._allocator;
	
	_indexesBufferObject = src._indexesBufferObject;
	_verticesBufferObject = src._verticesBufferObject;
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

AnimaGeometry::AnimaGeometry(AnimaGeometry&& src)
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
	, _normalsBufferObject(src._normalsBufferObject)
	, _textureCoordsBufferObject(src._textureCoordsBufferObject)
	, _tangentsBufferObject(src._tangentsBufferObject)
	, _visible(src._visible)
	, _needsBuffersUpdate(src._needsBuffersUpdate)
	, _shadersNames(src._shadersNames)
	, _shaderProgramName(src._shaderProgramName)
{
}

AnimaGeometry::~AnimaGeometry()
{
	ClearAll();
}

AnimaGeometry& AnimaGeometry::operator=(const AnimaGeometry& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
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

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
	}
	
	return *this;
}

AnimaGeometry& AnimaGeometry::operator=(AnimaGeometry&& src)
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

		_shadersNames = src._shadersNames;
		_shaderProgramName = src._shaderProgramName;
	}
	
	return *this;
}

bool AnimaGeometry::operator==(const AnimaGeometry& other)
{
	return true;
}

bool AnimaGeometry::operator!=(const AnimaGeometry& other)
{
	return true;
}

ptree AnimaGeometry::GetObjectTree(bool saveName) const
{
	ptree tree;

	if (saveName)
		tree.add("AnimaGeometry.Name", GetName());

	AnimaString materialName = "";
	if (_material != nullptr)
		materialName = _material->GetName();

	tree.put("AnimaGeometry.Material", materialName);
	tree.add("AnimaGeometry.Vertices", _vertices);
	tree.add("AnimaGeometry.Normals", _normals);
	tree.add("AnimaGeometry.TextureCoords", _textureCoords);
	tree.add("AnimaGeometry.Tangents", _tangents);
	tree.add("AnimaGeometry.Bitangents", _bitangents);
	tree.add("AnimaGeometry.BoneWeights", _boneWeights);
	tree.add("AnimaGeometry.BoneIDs", _boneIDs);

	AnimaArray<AUint> indexes;
	GetFacesIndicesArray(&indexes);
	tree.add("AnimaGeometry.Indexes", indexes);

	AnimaArray<AnimaVertex3f> facesNormals;
	GetFacesNormalsArray(&facesNormals);
	tree.add("AnimaGeometry.FacesNormals", facesNormals);

	tree.add_child("AnimaGeometry.SceneObject", AnimaSceneObject::GetObjectTree(false));

	return tree;
}

bool AnimaGeometry::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaGeometry.Name"));
		
		AnimaString materialName = objectTree.get<AnimaString>("AnimaGeometry.Material", "");
		_vertices = objectTree.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Vertices", AnimaArray<AnimaVertex3f>());
		_normals = objectTree.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Normals", AnimaArray<AnimaVertex3f>());
		_textureCoords = objectTree.get<AnimaArray<AnimaVertex2f> >("AnimaGeometry.TextureCoords", AnimaArray<AnimaVertex2f>());
		_tangents = objectTree.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Tangents", AnimaArray<AnimaVertex3f>());
		_bitangents = objectTree.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.Bitangents", AnimaArray<AnimaVertex3f>());
		_boneWeights = objectTree.get<AnimaArray<AnimaVertex4f> >("AnimaGeometry.BoneWeights", AnimaArray<AnimaVertex4f>());
		_boneIDs = objectTree.get<AnimaArray<AnimaVertex4f> >("AnimaGeometry.BoneIDs", AnimaArray<AnimaVertex4f>());

		AnimaArray<AUint> indexes = objectTree.get<AnimaArray<AUint> >("AnimaGeometry.Indexes", AnimaArray<AUint>());
		AnimaArray<AnimaVertex3f> facesNormals = objectTree.get<AnimaArray<AnimaVertex3f> >("AnimaGeometry.FacesNormals", AnimaArray<AnimaVertex3f>());

		AInt indexesCount = indexes.size();
		AInt facesNormalsCount = facesNormals.size();

		bool hasFacesNormal = (facesNormalsCount == indexesCount % 3);

		for (AInt i = 0; i < indexesCount; i += 3)
		{
			AnimaFace face;
			face.SetIndexes(indexes[i], indexes[i + 1], indexes[i + 2]);

			if (hasFacesNormal)
				face.SetNormal(facesNormals[i % 3]);

			AddFace(face);
		}

		if (!materialName.empty())
			_material = scene->GetMaterialsManager()->GetMaterialFromName(materialName);

		ptree sceneObjectTree = objectTree.get_child("AnimaGeometry.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh: %s", exception.what());
		return false;
	}
}

void AnimaGeometry::ClearAll()
{
	ANIMA_ASSERT(_allocator != nullptr);
	
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearTangents();
	ClearBitangents();
	ClearFaces();
}

void AnimaGeometry::ClearVertices()
{
	_vertices.clear();
}

void AnimaGeometry::ClearNormals()
{
	_normals.clear();
}

void AnimaGeometry::ClearTextureCoords()
{
	_textureCoords.clear();
}

void AnimaGeometry::ClearTangents()
{
	_tangents.clear();
}

void AnimaGeometry::ClearBitangents()
{
	_bitangents.clear();
}

void AnimaGeometry::ClearFaces()
{
	_faces.clear();
}

void AnimaGeometry::SetVertices(const AnimaArray<AnimaVertex3f>& vertices)
{
	_vertices = vertices;
}

void AnimaGeometry::SetVertices(AnimaVertex3f* v, AInt n)
{
	_vertices.clear();

	_vertices.reserve(n);
	for (AInt i = 0; i < n; i++)
		_vertices.push_back(v[i]);
}

void AnimaGeometry::AddVertex(const AnimaVertex3f& v)
{
	_vertices.push_back(v);
}

void AnimaGeometry::SetNormals(const AnimaArray<AnimaVertex3f>& normals)
{
	_normals = normals;
}

void AnimaGeometry::SetNormals(AnimaVertex3f* v, AInt n)
{
	_normals.clear();
	
	_normals.reserve(n);
	for (AInt i = 0; i < n; i++)
		_normals.push_back(v[i]);
}

void AnimaGeometry::AddNormal(const AnimaVertex3f& v)
{
	_normals.push_back(v);
}

void AnimaGeometry::SetTextureCoords(const AnimaArray<AnimaVertex2f>& textureCoords)
{
	_textureCoords = textureCoords;
}

void AnimaGeometry::SetTextureCoords(AnimaVertex2f* v, AInt n)
{
	_textureCoords.clear();

	_textureCoords.reserve(n);
	for (AInt i = 0; i < n; i++)
		_textureCoords.push_back(v[i]);
}

void AnimaGeometry::AddTextureCoord(const AnimaVertex2f& v)
{
	_textureCoords.push_back(v);
}

void AnimaGeometry::SetTangents(const AnimaArray<AnimaVertex3f>& tangents)
{
	_tangents = tangents;
}

void AnimaGeometry::SetTangents(AnimaVertex3f* v, AInt n)
{
	_tangents.clear();

	_tangents.reserve(n);
	for (AInt i = 0; i < n; i++)
		_tangents.push_back(v[i]);
}

void AnimaGeometry::AddTangent(const AnimaVertex3f& v)
{
	_tangents.push_back(v);
}

void AnimaGeometry::SetBitangents(const AnimaArray<AnimaVertex3f>& bitangents)
{
	_bitangents = bitangents;
}

void AnimaGeometry::SetBitangents(AnimaVertex3f* v, AInt n)
{
	_bitangents.clear();

	_bitangents.reserve(n);
	for (AInt i = 0; i < n; i++)
		_bitangents.push_back(v[i]);
}

void AnimaGeometry::AddBitangent(const AnimaVertex3f& v)
{
	_bitangents.push_back(v);
}

void AnimaGeometry::SetBoneWeights(const AnimaArray<AnimaVertex4f>& boneWeights)
{
	_boneWeights = boneWeights;
}

void AnimaGeometry::SetBoneWeights(AnimaVertex4f* v, AInt n)
{
	_boneWeights.clear();

	_boneWeights.reserve(n);
	for (AInt i = 0; i < n; i++)
		_boneWeights.push_back(v[i]);
}

void AnimaGeometry::AddBoneWeight(const AnimaVertex4f& v)
{
	_boneWeights.push_back(v);
}

AInt AnimaGeometry::GetBoneWeightsCount()
{
	return _boneWeights.size();
}

AnimaVertex4f AnimaGeometry::GetBoneWeight(AInt index)
{
	return _boneWeights[index];
}

AnimaVertex4f* AnimaGeometry::GetPBoneWeight(AInt index)
{
	return &_boneWeights[index];
}

AnimaArray<AnimaVertex4f>* AnimaGeometry::GetBoneWeights()
{
	return &_boneWeights;
}

void AnimaGeometry::ClearBoneWeights()
{
	_boneWeights.clear();
}

void AnimaGeometry::SetBoneIDs(const AnimaArray<AnimaVertex4f>& boneIDs)
{
	_boneIDs = boneIDs;
}

void AnimaGeometry::SetBoneIDs(AnimaVertex4f* v, AInt n)
{
	_boneIDs.clear();

	_boneIDs.reserve(n);
	for (AInt i = 0; i < n; i++)
		_boneIDs.push_back(v[i]);
}

void AnimaGeometry::AddBoneID(const AnimaVertex4f& v)
{
	_boneIDs.push_back(v);
}

AInt AnimaGeometry::GetBoneIDsCount()
{
	return _boneIDs.size();
}

AnimaVertex4f AnimaGeometry::GetBoneID(AInt index)
{
	return _boneIDs[index];
}

AnimaVertex4f* AnimaGeometry::GetPBoneID(AInt index)
{
	return &_boneIDs[index];
}

AnimaArray<AnimaVertex4f>* AnimaGeometry::GetBoneIDs()
{
	return &_boneIDs;
}

void AnimaGeometry::ClearBoneIDs()
{
	_boneIDs.clear();;
}

void AnimaGeometry::SetFaces(const AnimaArray<AnimaFace>& faces)
{
	_faces = faces;
}

void AnimaGeometry::SetFaces(AnimaFace* faces, AInt n)
{
	_faces.clear();

	_faces.reserve(n);
	for (AInt i = 0; i < n; i++)
		_faces.push_back(faces[i]);
}

void AnimaGeometry::AddFace(const AnimaFace& face)
{
	_faces.push_back(face);
}

AInt AnimaGeometry::GetVerticesCount()
{
	return _vertices.size();
}

AnimaVertex3f AnimaGeometry::GetVertex(AInt index)
{
	return _vertices[index];
}

AnimaVertex3f* AnimaGeometry::GetPVertex(AInt index)
{
	return &_vertices[index];
}

AnimaArray<AnimaVertex3f>* AnimaGeometry::GetVertices()
{
	return &_vertices;
}

AInt AnimaGeometry::GetNormalsCount()
{
	return _normals.size();
}

AnimaVertex3f AnimaGeometry::GetNormal(AInt index)
{
	return _normals[index];
}

AnimaVertex3f* AnimaGeometry::GetPNormal(AInt index)
{
	return &_normals[index];
}

AnimaArray<AnimaVertex3f>* AnimaGeometry::GetNormals()
{
	return &_normals;
}

AInt AnimaGeometry::GetTextureCoordsCount()
{
	return _textureCoords.size();
}

AnimaVertex2f AnimaGeometry::GetTextureCoord(AInt index)
{
	return _textureCoords[index];
}

AnimaVertex2f* AnimaGeometry::GetPTextureCoord(AInt index)
{
	return &_textureCoords[index];
}

AnimaArray<AnimaVertex2f>* AnimaGeometry::GetTextureCoords()
{
	return &_textureCoords;
}

AInt AnimaGeometry::GetTangentsCount()
{
	return _tangents.size();
}

AnimaVertex3f AnimaGeometry::GetTangent(AInt index)
{
	return _tangents[index];
}

AnimaVertex3f* AnimaGeometry::GetPTangent(AInt index)
{
	return &_tangents[index];
}

AnimaArray<AnimaVertex3f>* AnimaGeometry::GetTangents()
{
	return &_tangents;
}

AInt AnimaGeometry::GetBitangentsCount()
{
	return _bitangents.size();
}

AnimaVertex3f AnimaGeometry::GetBitangent(AInt index)
{
	return _bitangents[index];
}

AnimaVertex3f* AnimaGeometry::GetPBitangent(AInt index)
{
	return &_bitangents[index];
}

AnimaArray<AnimaVertex3f>* AnimaGeometry::GetBitangents()
{
	return &_bitangents;
}

AInt AnimaGeometry::GetFacesCount()
{
	return _faces.size();
}

AnimaFace AnimaGeometry::GetFace(AInt index)
{
	return _faces[index];
}

AnimaFace* AnimaGeometry::GetPFace(AInt index)
{
	return &_faces[index];
}

AnimaArray<AnimaFace>* AnimaGeometry::GetFaces()
{
	return &_faces;
}

bool AnimaGeometry::CanCreateBuffers()
{
	if (_vertices.size() <= 0)
		return false;
	return true;
}

bool AnimaGeometry::CreateBuffers()
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

void AnimaGeometry::UpdateBuffers()
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
		if (_verticesBufferObject > 0)
			glInvalidateBufferData(_verticesBufferObject);

		if (_indexesBufferObject > 0)
			glInvalidateBufferData(_indexesBufferObject);

		if (_normalsBufferObject > 0)
			glInvalidateBufferData(_normalsBufferObject);
		
		if (_textureCoordsBufferObject > 0)
			glInvalidateBufferData(_textureCoordsBufferObject);
		
		if (_tangentsBufferObject > 0)
			glInvalidateBufferData(_tangentsBufferObject);
		
		if (_bitangentsBufferObject > 0)
			glInvalidateBufferData(_bitangentsBufferObject);
		
		if (_boneWeightsBufferObject > 0)
			glInvalidateBufferData(_boneWeightsBufferObject);
		
		if (_boneIDsBufferObject > 0)
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

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, vertices);
		vertices = nullptr;
	}

	if (GetFloatVerticesNormalCount() > 0)
	{
		AFloat* normals = GetFloatVerticesNormal();
		glBindBuffer(GL_ARRAY_BUFFER, _normalsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesNormalCount(), normals, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, normals);
		normals = nullptr;
	}

	if (GetFloatVerticesTextureCoordCount() > 0)
	{
		AFloat* textureCoords = GetFloatVerticesTextureCoord();
		glBindBuffer(GL_ARRAY_BUFFER, _textureCoordsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesTextureCoordCount(), textureCoords, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, textureCoords);
		textureCoords = nullptr;
	}

	if (GetFloatVerticesTangentsCount() > 0)
	{
		AFloat* tangents = GetFloatVerticesTangents();
		glBindBuffer(GL_ARRAY_BUFFER, _tangentsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesTangentsCount(), tangents, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(3);
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tangents);
		tangents = nullptr;
	}

	if (GetFloatVerticesBitangentsCount() > 0)
	{
		AFloat* bitangents = GetFloatVerticesBitangents();
		glBindBuffer(GL_ARRAY_BUFFER, _bitangentsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatVerticesBitangentsCount(), bitangents, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, bitangents);
		bitangents = nullptr;
	}

	if (GetFloatBoneWeightsCount() > 0)
	{
		AFloat* boneWeights = GetFloatBoneWeights();
		glBindBuffer(GL_ARRAY_BUFFER, _boneWeightsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatBoneWeightsCount(), boneWeights, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, boneWeights);
		boneWeights = nullptr;
	}

	if (GetFloatBoneIDsCount() > 0)
	{
		AFloat* boneIDs = GetFloatBoneIDs();
		glBindBuffer(GL_ARRAY_BUFFER, _boneIDsBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AFloat) * GetFloatBoneIDsCount(), boneIDs, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(6);
		//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, 0);

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, boneIDs);
		boneIDs = nullptr;
	}

	glBindVertexArray(0);

	_needsBuffersUpdate = false;
}

bool AnimaGeometry::AreBuffersCreated()
{
	return IsIndicesBufferCreated() && IsVerticesBufferCreated() && IsVertexArrayObjectCreated() && IsTextureCoordsBufferCreated() && IsNormalsBufferCreated() && IsTangentsBufferCreated() && IsBitangentsBufferCreated() && IsBoneWeightsBufferCreated() && IsBoneIDsBufferCreated();// && IsColorsBufferCreated();
}

bool AnimaGeometry::IsIndicesBufferCreated()
{
	return _indexesBufferObject > 0;
}

bool AnimaGeometry::IsVerticesBufferCreated()
{
	return _verticesBufferObject > 0;
}

bool AnimaGeometry::IsNormalsBufferCreated()
{
	return _normalsBufferObject > 0;
}

bool AnimaGeometry::IsTextureCoordsBufferCreated()
{
	return _textureCoordsBufferObject > 0;
}

bool AnimaGeometry::IsTangentsBufferCreated()
{
	return _tangentsBufferObject > 0;
}

bool AnimaGeometry::IsBitangentsBufferCreated()
{
	return _bitangentsBufferObject > 0;
}

bool AnimaGeometry::IsBoneWeightsBufferCreated()
{
	return _boneWeightsBufferObject > 0;
}

bool AnimaGeometry::IsBoneIDsBufferCreated()
{
	return _boneIDsBufferObject > 0;
}

bool AnimaGeometry::IsVertexArrayObjectCreated()
{
	return _vertexArrayObject > 0;
}

bool AnimaGeometry::CreateIndicesBuffer()
{
	if (IsIndicesBufferCreated())
		return true;

	glGenBuffers(1, &_indexesBufferObject);
	if (_indexesBufferObject <= 0)
		return false;

	return true;
}

bool AnimaGeometry::CreateNormalsBuffer()
{
	if (IsNormalsBufferCreated())
		return true;

	glGenBuffers(1, &_normalsBufferObject);
	if (_normalsBufferObject <= 0)
		return false;

	return true;
}

bool AnimaGeometry::CreateTextureCoordsBuffer()
{
	if (IsTextureCoordsBufferCreated())
		return true;

	if (GetFloatVerticesTextureCoordCount() > 0)
	{
		glGenBuffers(1, &_textureCoordsBufferObject);
		if (_textureCoordsBufferObject <= 0)
			return false;
	}

	return true;
}

bool AnimaGeometry::CreateTangentsBuffer()
{
	if (IsTangentsBufferCreated())
		return true;

	if (GetFloatVerticesTangentsCount() > 0)
	{
		glGenBuffers(1, &_tangentsBufferObject);
		if (_tangentsBufferObject <= 0)
			return false;
	}

	return true;
}

bool AnimaGeometry::CreateBitangentsBuffer()
{
	if (IsBitangentsBufferCreated())
		return true;

	if (GetFloatVerticesBitangentsCount() > 0)
	{
		glGenBuffers(1, &_bitangentsBufferObject);
		if (_bitangentsBufferObject <= 0)
			return false;
	}

	return true;
}

bool AnimaGeometry::CreateBoneWeightsBuffer()
{
	if (IsBoneWeightsBufferCreated())
		return true;

	if (GetFloatBoneWeightsCount() > 0)
	{
		glGenBuffers(1, &_boneWeightsBufferObject);
		if (_boneWeightsBufferObject <= 0)
			return false;
	}

	return true;
}

bool AnimaGeometry::CreateBoneIDsBuffer()
{
	if (IsBoneIDsBufferCreated())
		return true;

	if (GetFloatBoneIDsCount() > 0)
	{
		glGenBuffers(1, &_boneIDsBufferObject);
		if (_boneIDsBufferObject <= 0)
			return false;
	}

	return true;
}

bool AnimaGeometry::CreateVerticesBuffer()
{
	if (IsVerticesBufferCreated())
		return true;
	
	glGenBuffers(1, &_verticesBufferObject);
	if (_verticesBufferObject <= 0)
		return false;
	
	return true;
}

bool AnimaGeometry::CreateVertexArrayObject()
{
	if (IsVertexArrayObjectCreated())
		return true;

	glGenVertexArrays(1, &_vertexArrayObject);
	if (_vertexArrayObject <= 0)
		return false;

	return true;
}

void AnimaGeometry::SetUpdateBuffers(bool bUpdate)
{
	_needsBuffersUpdate = bUpdate;
}

bool AnimaGeometry::NeedsBuffersUpdate()
{
	return _needsBuffersUpdate;
}

AUint AnimaGeometry::GetFacesIndicesCount()
{
	return (AUint)_faces.size() * 3;
}

AUint* AnimaGeometry::GetFacesIndices()
{
	AUint* indexes = nullptr;
	AInt count = GetFacesIndicesCount();
	AInt copied = 0;
	AInt offset = 0;

	if (count > 0)
	{
		indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, count);

		AInt facesCount = _faces.size();
		for (AInt i = 0; i < facesCount; i++)
		{
			copied = _faces[i].GetIndexesCount();
			_faces[i].GetConstIndexes(indexes + offset, copied);
			offset += copied;
		}
	}

	return indexes;
}

AUint AnimaGeometry::GetFloatVerticesCount()
{
	return (AUint)_vertices.size() * 3;
}

AFloat* AnimaGeometry::GetFloatVertices()
{
	AFloat* vertices = nullptr;
	AInt count = GetFloatVerticesCount();
	AInt offset = 0;

	if (count > 0)
	{
		vertices = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt verticesCount = _vertices.size();
		for (int i = 0; i < verticesCount; i++)
		{
			memcpy(vertices + offset, _vertices[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return vertices;
}

AUint AnimaGeometry::GetFloatVerticesNormalCount()
{
	return (AUint)_normals.size() * 3;
}

AFloat* AnimaGeometry::GetFloatVerticesNormal()
{
	AFloat* normals = nullptr;
	AInt count = GetFloatVerticesNormalCount();

	AInt offset = 0;

	if (count > 0)
	{
		normals = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt normalsCount = _normals.size();
		for (int i = 0; i < normalsCount; i++)
		{
			memcpy(normals + offset, _normals[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return normals;
}

AUint AnimaGeometry::GetFloatVerticesTextureCoordCount()
{
	return (AUint)_textureCoords.size() * 2;
}

AFloat* AnimaGeometry::GetFloatVerticesTextureCoord()
{
	AFloat* textureCoords = nullptr;
	AInt count = GetFloatVerticesTextureCoordCount();
	AInt offset = 0;

	if (count > 0)
	{
		textureCoords = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt textureCoordsCount = _textureCoords.size();
		for (int i = 0; i < textureCoordsCount; i++)
		{
			memcpy(textureCoords + offset, _textureCoords[i].vec, sizeof(AFloat) * 2);
			offset += 2;
		}
	}

	return textureCoords;
}

AUint AnimaGeometry::GetFloatVerticesTangentsCount()
{
	return (AUint)_tangents.size() * 3;
}

AFloat* AnimaGeometry::GetFloatVerticesTangents()
{
	AFloat* tangents = nullptr;
	AInt count = GetFloatVerticesTangentsCount();
	AInt offset = 0;

	if (count > 0)
	{
		tangents = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt tangentsCount = _tangents.size();
		for (int i = 0; i < tangentsCount; i++)
		{
			memcpy(tangents + offset, _tangents[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return tangents;
}

AUint AnimaGeometry::GetFloatVerticesBitangentsCount()
{
	return (AUint)_bitangents.size() * 3;
}

AFloat* AnimaGeometry::GetFloatVerticesBitangents()
{
	AFloat* bitangents = nullptr;
	AInt count = GetFloatVerticesBitangentsCount();
	AInt offset = 0;

	if (count > 0)
	{
		bitangents = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt bitangentsCount = _bitangents.size();
		for (int i = 0; i < bitangentsCount; i++)
		{
			memcpy(bitangents + offset, _bitangents[i].vec, sizeof(AFloat) * 3);
			offset += 3;
		}
	}

	return bitangents;
}

AUint AnimaGeometry::GetFloatBoneWeightsCount()
{
	return (AUint)_boneWeights.size() * 4;
}

float* AnimaGeometry::GetFloatBoneWeights()
{
	AFloat* boneWeights = nullptr;
	AInt count = GetFloatBoneWeightsCount();
	AInt offset = 0;

	if (count > 0)
	{
		boneWeights = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt weightsCount = _boneWeights.size();
		for (int i = 0; i < weightsCount; i++)
		{
			memcpy(boneWeights + offset, _boneWeights[i].vec, sizeof(AFloat) * 4);
			offset += 4;
		}
	}

	return boneWeights;
}

AUint AnimaGeometry::GetFloatBoneIDsCount()
{
	return (AUint)_boneIDs.size() * 4;
}

float* AnimaGeometry::GetFloatBoneIDs()
{
	AFloat* boneIDs = nullptr;
	AInt count = GetFloatBoneIDsCount();
	AInt offset = 0;

	if (count > 0)
	{
		boneIDs = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, count);

		AInt idsCount = _boneIDs.size();
		for (int i = 0; i < idsCount; i++)
		{
			memcpy(boneIDs + offset, _boneIDs[i].vec, sizeof(AFloat) * 4);
			offset += 4;
		}
	}

	return boneIDs;
}

void AnimaGeometry::SetIsVisible(bool visible)
{
	_visible = visible;
}

bool AnimaGeometry::IsVisible()
{
	return _visible;
}

void AnimaGeometry::SetMaterial(AnimaMaterial* material)
{
	_material = material;

	if (_material)
		_materialName = _material->GetName();
	else
		_materialName = "";
}

AnimaMaterial* AnimaGeometry::GetMaterial()
{
	return _material;
}

AUint AnimaGeometry::GetVertexArrayObject()
{
	return _vertexArrayObject;
}

AUint AnimaGeometry::GetIndexesBufferObject()
{
	return _indexesBufferObject;
}

AUint AnimaGeometry::GetVerticesBufferObject()
{
	return _verticesBufferObject;
}

AUint AnimaGeometry::GetNormalsBufferObject()
{
	return _normalsBufferObject;
}

AUint AnimaGeometry::GetTextureCoordsBufferObject()
{
	return _textureCoordsBufferObject;
}

AUint AnimaGeometry::GetTangentsBufferObject()
{
	return _tangentsBufferObject;
}

AUint AnimaGeometry::GetBitangentsBufferObject()
{
	return _bitangentsBufferObject;
}

AUint AnimaGeometry::GetBoneWeightsBufferObject()
{
	return _boneWeightsBufferObject;
}

AUint AnimaGeometry::GetBoneIDsBufferObject()
{
	return _boneIDsBufferObject;
}

void AnimaGeometry::ComputeBoundingBox()
{
	AInt verticesCount = _vertices.size();
	if (verticesCount <= 0)
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
			
		for (AInt i = 1; i < verticesCount; i++)
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

AnimaVertex3f AnimaGeometry::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaGeometry::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

AnimaVertex3f AnimaGeometry::GetBoundingBoxCenter() const
{
	return _boundingBoxCenter;
}

void AnimaGeometry::AddInstance(AnimaMeshInstance* instance)
{
	if (instance != nullptr)
		_meshInstances.Add(instance->GetName(), instance);
}

AInt AnimaGeometry::GetInstancesCount() const
{
	return _meshInstances.GetSize();
}

AnimaMeshInstance* AnimaGeometry::GetInstance(AInt index) const
{
	return _meshInstances[index];
}

AnimaMeshInstance* AnimaGeometry::GetInstanceFromName(const AnimaString& name) const
{
	return _meshInstances[name];
}

void AnimaGeometry::ComputeSmootNormals()
{
	AInt facesCount = _faces.size();
	AInt verticesCount = _vertices.size();

	if (facesCount > 0 && verticesCount > 0)
	{
		ClearNormals();

		for (AInt i = 0; i < verticesCount; i++)
		{
			AInt pos = GetNextFaceContainingVertex(0, i);

			if (pos >= 0)
			{
				AnimaVertex3f normal = _faces[pos].GetNormal();
				while ((pos = GetNextFaceContainingVertex(pos + 1, i)) != -1)
					normal += _faces[pos].GetNormal();

				normal.Normalize();
				_normals.push_back(normal);
			}
		}
	}
}

void AnimaGeometry::ComputeFlatNormals()
{
	AInt facesCount = _faces.size();

	if (facesCount > 0)
	{
		ClearNormals();

		for (AInt i = 0; i < facesCount; i++)
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

void AnimaGeometry::FlipNormals()
{
	for (AInt i = 0; i < _normals.size(); i++)
		_normals[i] = -_normals[i];
}

void AnimaGeometry::FlipFaces()
{
	for (AInt i = 0; i < _faces.size(); i++)
	{
		AnimaFace* face = &_faces[i];
		AUint a = face->GetIndex(0);
		AUint b = face->GetIndex(1);
		AUint c = face->GetIndex(2);

		face->SetIndexes(c, b, a);
	}
}

AInt AnimaGeometry::GetNextFaceContainingVertex(AInt start, AInt vertexIndex) const
{
	AInt facesCount = _faces.size();
	for (AInt i = start; i < facesCount; i++)
	{
		if (_faces[i].HasIndex((AUint)vertexIndex))
			return (AInt)i;
	}

	return -1;
}

void AnimaGeometry::AddShader(AnimaShader* shader)
{
	_shadersNames.push_back(shader->GetName());
}

void AnimaGeometry::AddShader(const AnimaString& shaderName)
{
	_shadersNames.push_back(shaderName);
}

AInt AnimaGeometry::GetShadersCount() const
{
	return _shadersNames.size();
}

AnimaString AnimaGeometry::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

void AnimaGeometry::SetShaderProgram(const AnimaString& shaderProgramName)
{
	_shaderProgramName = shaderProgramName;
}

AnimaString AnimaGeometry::GetShaderProgramName() const
{
	return _shaderProgramName;
}

void AnimaGeometry::MakePlane()
{
	AnimaMeshCreator::MakePlane(this, _allocator);
}

void AnimaGeometry::MakeCube()
{
	AnimaMeshCreator::MakeCube(this, _allocator);
}

void AnimaGeometry::MakeIcosahedralSphere(AInt recursionLevel)
{
	AnimaMeshCreator::MakeIcosahedralSphere(this, recursionLevel, _allocator);
}

void AnimaGeometry::MakeCylinder(AFloat topRadius, AFloat bottomRadius, AFloat height, AUint radialSegments, AUint heightSegments, bool openEnded)
{
	AnimaMeshCreator::MakeCylinder(this, topRadius, bottomRadius, height, radialSegments, heightSegments, openEnded, _allocator);
}

void AnimaGeometry::Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial)
{
	program->UpdateSceneObjectProperties(this, renderer);

	if (updateMaterial)
	{
		AnimaMaterial* material = _material;
		if (material == nullptr)
			material = AnimaMaterialsManager::GetDefaultMaterial();

		program->UpdateMappedValuesObjectProperties(material, renderer);
	}

#ifdef _WIN32
	if (start)
	{
		//glBindVertexArray(GetVertexArrayObject());
		program->EnableInputs(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexesBufferObject());
	}

	glDrawElements(GL_TRIANGLES, GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	if (end)
	{
		//glBindVertexArray(0);
		program->DisableInputs();
	}
#else
	program->EnableInputs(this);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaGeometry::GetFacesIndicesArray(AnimaArray<AUint>* dst) const
{
	if (dst == nullptr)
		return;

	for (auto& face : _faces)
	{
		dst->push_back(face.GetIndex(0));
		dst->push_back(face.GetIndex(1));
		dst->push_back(face.GetIndex(2));
	}
}

void AnimaGeometry::GetFacesNormalsArray(AnimaArray<AnimaVertex3f>* dst) const
{
	if (dst == nullptr)
		return;

	for (auto& face : _faces)
		dst->push_back(face.GetNormal());
}

END_ANIMA_ENGINE_NAMESPACE