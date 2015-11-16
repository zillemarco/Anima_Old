//
//  AnimaMesh.h
//  Anima
//
//  Created by Marco Zille on 25/11/14.
//
//

#ifndef __Anima__AnimaMesh__
#define __Anima__AnimaMesh__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaVertex.h"
#include "AnimaFace.h"
#include "AnimaString.h"
#include "AnimaMatrix.h"
#include "AnimaMaterial.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaSceneObject.h"
#include "AnimaNamedObject.h"
#include "AnimaShader.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaMeshInstance;
class AnimaRenderer;
class AnimaShaderProgram;

class ANIMA_ENGINE_EXPORT AnimaMeshBoneInfo : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaMeshBoneInfo);

public:
	AnimaMeshBoneInfo(const AnimaString& name, AnimaAllocator* allocator);
	AnimaMeshBoneInfo(const AnimaMeshBoneInfo& src);
	AnimaMeshBoneInfo(AnimaMeshBoneInfo&& src);
	~AnimaMeshBoneInfo();

	AnimaMeshBoneInfo& operator=(const AnimaMeshBoneInfo& src);
	AnimaMeshBoneInfo& operator=(AnimaMeshBoneInfo&& src);

public:
	void SetBoneOffset(AnimaMatrix boneOffset);
	AnimaMatrix GetBoneOffset() const;

	void SetFinalTransformation(AnimaMatrix finalTransformation);
	AnimaMatrix GetFinalTransformation() const;

protected:
	AnimaMatrix _boneOffset;
	AnimaMatrix _finalTransformation;
};

class ANIMA_ENGINE_EXPORT AnimaMesh : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaMesh);

public:
	AnimaMesh(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaMesh(const AnimaMesh& src);
	AnimaMesh(AnimaMesh&& src);
	~AnimaMesh();
	
	AnimaMesh& operator=(const AnimaMesh& src);
	AnimaMesh& operator=(AnimaMesh&& src);

	bool operator==(const AnimaMesh& other);
	bool operator!=(const AnimaMesh& other);

public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;

public:
	void Draw(AnimaRenderer* renderer, AnimaShaderProgram* program, bool start, bool end, bool updateMaterial = true);

public:
	// Gestione dati struttura
	void ClearAll();

	void MakePlane();
	void MakeCube();
	void MakeIcosahedralSphere(AInt recursionLevel = 0);
	void MakeCylinder(AFloat topRadius = 1.0f, AFloat bottomRadius = 1.0f, AFloat height = 1.0f, AUint radialSegments = 8, AUint heightSegments = 1, bool openEnded = false);
	
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();
	
	void ComputeBoundingBox();
	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;
	AnimaVertex3f GetBoundingBoxCenter() const;

	void AddInstance(AnimaMeshInstance* instance);
	AInt GetInstancesCount() const;
	AnimaMeshInstance* GetInstance(AInt index) const;
	AnimaMeshInstance* GetInstanceFromName(const AnimaString& name) const;
	
	// Gestione dati grafici
	void SetVertices(const AnimaArray<AnimaVertex3f>& vertices);
	void SetVertices(AnimaVertex3f* v, AInt n);
	void AddVertex(const AnimaVertex3f& v);
	AInt GetVerticesCount();
	AnimaVertex3f GetVertex(AInt index);
	AnimaVertex3f* GetPVertex(AInt index);
	AnimaArray<AnimaVertex3f>* GetVertices();
	void ClearVertices();

	void SetNormals(const AnimaArray<AnimaVertex3f>& normals);
	void SetNormals(AnimaVertex3f* v, AInt n);
	void AddNormal(const AnimaVertex3f& v);
	AInt GetNormalsCount();
	AnimaVertex3f GetNormal(AInt index);
	AnimaVertex3f* GetPNormal(AInt index);
	AnimaArray<AnimaVertex3f>* GetNormals();
	void ClearNormals();
	void ComputeSmootNormals();
	void ComputeFlatNormals();
	void FlipNormals();

	void SetTextureCoords(const AnimaArray<AnimaVertex2f>& textureCoords);
	void SetTextureCoords(AnimaVertex2f* v, AInt n);
	void AddTextureCoord(const AnimaVertex2f& v);
	AInt GetTextureCoordsCount();
	AnimaVertex2f GetTextureCoord(AInt index);
	AnimaVertex2f* GetPTextureCoord(AInt index);
	AnimaArray<AnimaVertex2f>* GetTextureCoords();
	void ClearTextureCoords();

	void SetTangents(const AnimaArray<AnimaVertex3f>& tangents);
	void SetTangents(AnimaVertex3f* v, AInt n);
	void AddTangent(const AnimaVertex3f& v);
	AInt GetTangentsCount();
	AnimaVertex3f GetTangent(AInt index);
	AnimaVertex3f* GetPTangent(AInt index);
	AnimaArray<AnimaVertex3f>* GetTangents();
	void ClearTangents();

	void SetBitangents(const AnimaArray<AnimaVertex3f>& bitangents);
	void SetBitangents(AnimaVertex3f* v, AInt n);
	void AddBitangent(const AnimaVertex3f& v);
	AInt GetBitangentsCount();
	AnimaVertex3f GetBitangent(AInt index);
	AnimaVertex3f* GetPBitangent(AInt index);
	AnimaArray<AnimaVertex3f>* GetBitangents();
	void ClearBitangents();

	void SetBoneWeights(const AnimaArray<AnimaVertex4f>& boneWeights);
	void SetBoneWeights(AnimaVertex4f* v, AInt n);
	void AddBoneWeight(const AnimaVertex4f& v);
	AInt GetBoneWeightsCount();
	AnimaVertex4f GetBoneWeight(AInt index);
	AnimaVertex4f* GetPBoneWeight(AInt index);
	AnimaArray<AnimaVertex4f>* GetBoneWeights();
	void ClearBoneWeights();

	void SetBoneIDs(const AnimaArray<AnimaVertex4f>& boneIDs);
	void SetBoneIDs(AnimaVertex4f* v, AInt n);
	void AddBoneID(const AnimaVertex4f& v);
	AInt GetBoneIDsCount();
	AnimaVertex4f GetBoneID(AInt index);
	AnimaVertex4f* GetPBoneID(AInt index);
	AnimaArray<AnimaVertex4f>* GetBoneIDs();
	void ClearBoneIDs();

	void SetFaces(const AnimaArray<AnimaFace>& faces);
	void SetFaces(AnimaFace* faces, AInt n);
	void AddFace(const AnimaFace& face);
	AInt GetFacesCount();
	AnimaFace GetFace(AInt index);
	AnimaFace* GetPFace(AInt index);
	AnimaArray<AnimaFace>* GetFaces();
	void ClearFaces();
	void FlipFaces();
	
	bool CreateBuffers();
	void UpdateBuffers();

	void SetUpdateBuffers(bool bUpdate = true);
	bool NeedsBuffersUpdate();

	// Gestione dati necessari al disegno
	void AddShader(AnimaShader* shader);
	void AddShader(const AnimaString& shaderName);
	AInt GetShadersCount() const;
	AnimaString GetShaderName(AInt index) const;

	void SetShaderProgram(const AnimaString& shaderProgramName);
	AnimaString GetShaderProgramName() const;
			
public:
	bool AreBuffersCreated();
	bool CanCreateBuffers();
	
	bool IsIndicesBufferCreated();
	bool IsVerticesBufferCreated();
	bool IsNormalsBufferCreated();
	bool IsTextureCoordsBufferCreated();
	bool IsTangentsBufferCreated();
	bool IsBitangentsBufferCreated();
	bool IsBoneWeightsBufferCreated();
	bool IsBoneIDsBufferCreated();
	bool IsVertexArrayObjectCreated();

	bool CreateIndicesBuffer();
	bool CreateVerticesBuffer();
	bool CreateNormalsBuffer();
	bool CreateTextureCoordsBuffer();
	bool CreateTangentsBuffer();
	bool CreateBitangentsBuffer();
	bool CreateBoneWeightsBuffer();
	bool CreateBoneIDsBuffer();
	bool CreateVertexArrayObject();

	AUint GetVertexArrayObject();
	AUint GetIndexesBufferObject();
	AUint GetVerticesBufferObject();
	AUint GetNormalsBufferObject();
	AUint GetTextureCoordsBufferObject();
	AUint GetTangentsBufferObject();
	AUint GetBitangentsBufferObject();
	AUint GetBoneWeightsBufferObject();
	AUint GetBoneIDsBufferObject();

	AUint GetFacesIndicesCount();
	AUint* GetFacesIndices();
	void GetFacesIndicesArray(AnimaArray<AUint>* dst) const;
	void GetFacesNormalsArray(AnimaArray<AnimaVertex3f>* dst) const;

	AUint GetFloatVerticesCount();
	float* GetFloatVertices();

	AUint GetFloatVerticesNormalCount();
	float* GetFloatVerticesNormal();

	AUint GetFloatVerticesTextureCoordCount();
	float* GetFloatVerticesTextureCoord();

	AUint GetFloatVerticesTangentsCount();
	float* GetFloatVerticesTangents();

	AUint GetFloatVerticesBitangentsCount();
	float* GetFloatVerticesBitangents();

	AUint GetFloatBoneWeightsCount();
	float* GetFloatBoneWeights();

	AUint GetFloatBoneIDsCount();
	float* GetFloatBoneIDs();

	void SetIsVisible(bool visible);
	bool IsVisible();

protected:
	AInt GetNextFaceContainingVertex(AInt start, AInt vertexIndex) const;

protected:
	// Dati struttura
	AnimaMaterial*	_material;
	AnimaString		_materialName;

	AnimaMappedArray<AnimaMeshInstance*> _meshInstances;
	
	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;
	AnimaVertex3f	_boundingBoxCenter;

	// Dati grafica
	AnimaArray<AnimaVertex3f>	_vertices;
	AnimaArray<AnimaVertex3f>	_normals;
	AnimaArray<AnimaVertex2f>	_textureCoords;
	AnimaArray<AnimaVertex3f>	_tangents;
	AnimaArray<AnimaVertex3f>	_bitangents;
	AnimaArray<AnimaVertex4f>	_boneWeights;
	AnimaArray<AnimaVertex4f>	_boneIDs;
	AnimaArray<AnimaFace>		_faces;

	AUint _vertexArrayObject;
	AUint _indexesBufferObject;
	AUint _verticesBufferObject;
	AUint _normalsBufferObject;
	AUint _textureCoordsBufferObject;
	AUint _tangentsBufferObject;
	AUint _bitangentsBufferObject;
	AUint _boneWeightsBufferObject;
	AUint _boneIDsBufferObject;

	bool _visible;	
	bool _needsBuffersUpdate;

	// Dati necessari al disegno
	AnimaArray<AnimaString> _shadersNames;
	AnimaString _shaderProgramName;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMesh__) */
