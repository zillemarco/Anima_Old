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

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMesh : public AnimaSceneObject
{
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
	// Gestione dati struttura
	void ClearAll();

	void MakePlane();
	void MakeIcosahedralSphere(AInt recursionLevel = 0);
	void MakeCylinder(AFloat topRadius = 1.0f, AFloat bottomRadius = 1.0f, AFloat height = 1.0f, AUint radialSegments = 8, AUint heightSegments = 1, bool openEnded = false);
	
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();
	
	void ComputeBoundingBox(bool updateRecursively);
	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;
	AnimaVertex3f GetBoundingBoxCenter() const;
	
	// Gestione dati grafici
	void SetVertices(AnimaArray<AnimaVertex3f>* vertices);
	void SetVertices(AnimaVertex3f* v, AInt n);
	void AddVertex(const AnimaVertex3f& v);
	AInt GetVerticesNumber();
	AnimaVertex3f GetVertex(AInt index);
	AnimaVertex3f* GetPVertex(AInt index);
	AnimaArray<AnimaVertex3f>* GetVertices();
	void ClearVertices();

	void SetNormals(AnimaArray<AnimaVertex3f>* normals);
	void SetNormals(AnimaVertex3f* v, AInt n);
	void AddNormal(const AnimaVertex3f& v);
	AInt GetNormalsNumber();
	AnimaVertex3f GetNormal(AInt index);
	AnimaVertex3f* GetPNormal(AInt index);
	AnimaArray<AnimaVertex3f>* GetNormals();
	void ClearNormals();
	void ComputeSmootNormals();
	void ComputeFlatNormals();

	void SetTextureCoords(AnimaArray<AnimaVertex2f>* textureCoords);
	void SetTextureCoords(AnimaVertex2f* v, AInt n);
	void AddTextureCoord(const AnimaVertex2f& v);
	AInt GetTextureCoordsNumber();
	AnimaVertex2f GetTextureCoord(AInt index);
	AnimaVertex2f* GetPTextureCoord(AInt index);
	AnimaArray<AnimaVertex2f>* GetTextureCoords();
	void ClearTextureCoords();

	void SetTangents(AnimaArray<AnimaVertex3f>* tangents);
	void SetTangents(AnimaVertex3f* v, AInt n);
	void AddTangent(const AnimaVertex3f& v);
	AInt GetTangentsNumber();
	AnimaVertex3f GetTangent(AInt index);
	AnimaVertex3f* GetPTangent(AInt index);
	AnimaArray<AnimaVertex3f>* GetTangents();
	void ClearTangents();

	void SetBitangents(AnimaArray<AnimaVertex3f>* bitangents);
	void SetBitangents(AnimaVertex3f* v, AInt n);
	void AddBitangent(const AnimaVertex3f& v);
	AInt GetBitangentsNumber();
	AnimaVertex3f GetBitangent(AInt index);
	AnimaVertex3f* GetPBitangent(AInt index);
	AnimaArray<AnimaVertex3f>* GetBitangents();
	void ClearBitangents();

	void SetFaces(AnimaArray<AnimaFace>* faces);
	void SetFaces(AnimaFace* faces, AInt n);
	void AddFace(const AnimaFace& face);
	AInt GetFacesNumber();
	AnimaFace GetFace(AInt index);
	AnimaFace* GetPFace(AInt index);
	AnimaArray<AnimaFace>* GetFaces();
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

	void SetIsVisible(bool visible);
	bool IsVisible();

protected:
	AInt GetNextFaceContainingVertex(AInt start, AInt vertexIndex) const;

	const char* GetShaderPrefix() { return ""; }

protected:
	// Dati struttura
	AnimaMaterial*	_material;
	AnimaString		_materialName;
	
	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;
	AnimaVertex3f	_boundingBoxCenter;

	// Dati grafica
	AnimaArray<AnimaVertex3f>	_vertices;
	AnimaArray<AnimaVertex3f>	_normals;
	AnimaArray<AnimaVertex2f>	_textureCoords;
	AnimaArray<AnimaVertex3f>	_tangents;
	AnimaArray<AnimaVertex3f>	_bitangents;
	AnimaArray<AnimaFace>		_faces;
	
	AUint			_vertexArrayObject;
	AUint			_indexesBufferObject;
	AUint			_verticesBufferObject;
	AUint			_normalsBufferObject;
	//AUint			_colorsBufferObject;
	AUint			_textureCoordsBufferObject;
	AUint			_tangentsBufferObject;
	AUint			_bitangentsBufferObject;

	bool _visible;	
	bool _needsBuffersUpdate;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMesh__) */
