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
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaMatrix.h"
#include "AnimaMaterial.h"
#include "AnimaTransformation.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#define M_2PI	2.0f * (AFloat)M_PI

class ANIMA_ENGINE_EXPORT AnimaMesh
{
public:
	AnimaMesh(AnimaAllocator* allocator);
	AnimaMesh(const AnimaMesh& src);
	AnimaMesh(AnimaMesh&& src);
	~AnimaMesh();
	
	AnimaMesh& operator=(const AnimaMesh& src);
	AnimaMesh& operator=(AnimaMesh&& src);
	
public:
	// Gestione dati struttura
	void ClearAll();
	
	void SetChildren(AnimaMesh* children, ASizeT n);
	void AddChild(const AnimaMesh& child);
	ASizeT GetChildrenNumber();
	AnimaMesh* GetChild(ASizeT index);
	AnimaMesh* GetChildren();
	void ClearChildren();

	void SetMeshes(AnimaMesh* meshes, ASizeT n);
	void AddMesh(const AnimaMesh& mesh);
	ASizeT GetMeshesNumber();
	AnimaMesh* GetMesh(ASizeT index);
	AnimaMesh* GetMeshes();
	void ClearMeshes();

	AnimaMesh* CreateMesh();

	void MakePlane();
	void MakeIcosahedralSphere(AInt recursionLevel = 0);
	void MakeCylinder(AFloat topRadius = 1.0f, AFloat bottomRadius = 1.0f, AFloat height = 1.0f, AUint radialSegments = 8, AUint heightSegments = 1, bool openEnded = false);

	void SetMeshName(const AnimaString& name);
	void SetMeshName(const char* name);
	AnimaString GetAnimaMeshName();
	const char* GetMeshName();

	void SetMeshFileName(const AnimaString& name);
	void SetMeshFileName(const char* name);
	AnimaString GetAnimaMeshFileName();
	const char* GetMeshFileName();

	void SetParent(AnimaMesh* parent);
	AnimaMesh* GetParent() const;

	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();

	AnimaMatrix GetFinalMatrix() const;

	void ComputeBoundingBox(bool updateRecursively);
	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;

	AnimaTransformation* GetTransformation();
	AnimaTransformation GetTransformationCopy();

	// Gestione dati grafici
	void SetVertices(AnimaVertex3f* v, ASizeT n);
	void AddVertex(const AnimaVertex3f& v);
	ASizeT GetVerticesNumber();
	AnimaVertex3f GetVertex(ASizeT index);
	AnimaVertex3f* GetPVertex(ASizeT index);
	AnimaVertex3f* GetVertices();
	void ClearVertices();
	
	void SetNormals(AnimaVertex3f* v, ASizeT n);
	void AddNormal(const AnimaVertex3f& v);
	ASizeT GetNormalsNumber();
	AnimaVertex3f GetNormal(ASizeT index);
	AnimaVertex3f* GetPNormal(ASizeT index);
	AnimaVertex3f* GetNormals();
	void ClearNormals();
	void ComputeSmootNormals();
	void ComputeFlatNormals();
	
	void SetTextureCoords(AnimaVertex2f* v, ASizeT n);
	void AddTextureCoord(const AnimaVertex2f& v);
	ASizeT GetTextureCoordsNumber();
	AnimaVertex2f GetTextureCoord(ASizeT index);
	AnimaVertex2f* GetPTextureCoord(ASizeT index);
	AnimaVertex2f* GetTextureCoords();
	void ClearTextureCoords();

	void SetTangents(AnimaVertex3f* v, ASizeT n);
	void AddTangent(const AnimaVertex3f& v);
	ASizeT GetTangentsNumber();
	AnimaVertex3f GetTangent(ASizeT index);
	AnimaVertex3f* GetPTangent(ASizeT index);
	AnimaVertex3f* GetTangents();
	void ClearTangents();

	void SetBitangents(AnimaVertex3f* v, ASizeT n);
	void AddBitangent(const AnimaVertex3f& v);
	ASizeT GetBitangentsNumber();
	AnimaVertex3f GetBitangent(ASizeT index);
	AnimaVertex3f* GetPBitangent(ASizeT index);
	AnimaVertex3f* GetBitangents();
	void ClearBitangents();
	
	void SetFaces(AnimaFace* faces, ASizeT n);
	void AddFace(const AnimaFace& face);
	ASizeT GetFacesNumber();
	AnimaFace GetFace(ASizeT index);
	AnimaFace* GetPFace(ASizeT index);
	AnimaFace* GetFaces();
	void ClearFaces();
	
	bool CreateBuffers();
	void UpdateBuffers();

	void SetUpdateBuffers(bool bUpdate = true);
	bool NeedsBuffersUpdate();
		
public:
	bool AreBuffersCreated();
	bool CanCreateBuffers();
	
	bool IsIndicesBufferCreated();
	bool IsVerticesBufferCreated();
	//bool IsColorsBufferCreated();
	bool IsNormalsBufferCreated();
	bool IsTextureCoordsBufferCreated();
	bool IsTangentsBufferCreated();
	bool IsBitangentsBufferCreated();
	bool IsVertexArrayObjectCreated();

	bool CreateIndicesBuffer();
	bool CreateVerticesBuffer();
	//bool CreateColorsBuffer();
	bool CreateNormalsBuffer();
	bool CreateTextureCoordsBuffer();
	bool CreateTangentsBuffer();
	bool CreateBitangentsBuffer();
	bool CreateVertexArrayObject();

	AUint GetVertexArrayObject();
	AUint GetIndexesBufferObject();
	AUint GetVerticesBufferObject();
	AUint GetNormalsBufferObject();
	//AUint GetColorsBufferObject();
	AUint GetTextureCoordsBufferObject();
	AUint GetTangentsBufferObject();
	AUint GetBitangentsBufferObject();

	AUint GetFacesIndicesCount();
	AUint* GetFacesIndices();

	AUint GetFloatVerticesCount();
	float* GetFloatVertices();

	AUint GetFloatVerticesNormalCount();
	float* GetFloatVerticesNormal();

	//AUint GetFloatVerticesColorCount();
	//float* GetFloatVerticesColor();

	AUint GetFloatVerticesTextureCoordCount();
	float* GetFloatVerticesTextureCoord();

	AUint GetFloatVerticesTangentsCount();
	float* GetFloatVerticesTangents();

	AUint GetFloatVerticesBitangentsCount();
	float* GetFloatVerticesBitangents();

protected:
	AInt GetNextFaceContainingVertex(ASizeT start, ASizeT vertexIndex) const;

protected:
	AnimaAllocator* _allocator;

	// Dati struttura
	AnimaMaterial*	_material;
	AnimaString		_meshName;
	AnimaString		_meshFileName;
	
	AnimaMesh*		_parentMesh;
	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;

	AnimaMesh*		_meshes;
	ASizeT			_meshesNumber;

	AnimaMesh*		_meshChildren;
	ASizeT			_meshChildrenNumber;

	AnimaTransformation _transformation;

	// Dati grafica
	AnimaVertex3f*	_vertices;
	ASizeT			_verticesNumber;
	
	AnimaVertex3f*	_normals;
	ASizeT			_normalsNumber;
	
	AnimaVertex2f*	_textureCoords;
	ASizeT			_textureCoordsNumber;

	AnimaVertex3f*	_tangents;
	ASizeT			_tangentsNumber;

	AnimaVertex3f*	_bitangents;
	ASizeT			_bitangentsNumber;
	
	AnimaFace*		_faces;
	ASizeT			_facesNumber;
	
	AUint			_vertexArrayObject;
	AUint			_indexesBufferObject;
	AUint			_verticesBufferObject;
	AUint			_normalsBufferObject;
	//AUint			_colorsBufferObject;
	AUint			_textureCoordsBufferObject;
	AUint			_tangentsBufferObject;
	AUint			_bitangentsBufferObject;
	
	bool _needsBuffersUpdate;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMesh__) */
