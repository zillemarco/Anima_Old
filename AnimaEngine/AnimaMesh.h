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

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaModel;

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
	
	void SetTextureCoords(AnimaVertex2f* v, ASizeT n);
	void AddTextureCoord(const AnimaVertex2f& v);
	ASizeT GetTextureCoordsNumber();
	AnimaVertex2f GetTextureCoord(ASizeT index);
	AnimaVertex2f* GetPTextureCoord(ASizeT index);
	AnimaVertex2f* GetTextureCoords();
	void ClearTextureCoords();
	
	void SetFaces(AnimaFace* faces, ASizeT n);
	void AddFace(const AnimaFace& face);
	ASizeT GetFacesNumber();
	AnimaFace GetFace(ASizeT index);
	AnimaFace* GetPFace(ASizeT index);
	AnimaFace* GetFaces();
	void ClearFaces();

	void SetMeshName(const AnimaString& name);
	void SetMeshName(const char* name);
	AnimaString GetAnimaMeshName();
	const char* GetMeshName();

	void SetParent(AnimaModel* parent);
	AnimaModel* GetParent() const;

	bool CreateBuffers();
	void UpdateBuffers();

	void SetUpdateBuffers(bool bUpdate = true);
	bool NeedsBuffersUpdate();

	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();

	AnimaMatrix GetFinalMatrix() const;

	void ComputeBoundingBox();
	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;
	
public:
	bool AreBuffersCreated();
	
	bool IsIndicesBufferCreated();
	bool IsVerticesBufferCreated();
	//bool IsColorsBufferCreated();
	bool IsNormalsBufferCreated();
	bool IsTextureCoordsBufferCreated();
	bool IsVertexArrayObjectCreated();

	bool CreateIndicesBuffer();
	bool CreateVerticesBuffer();
	//bool CreateColorsBuffer();
	bool CreateNormalsBuffer();
	bool CreateTextureCoordsBuffer();
	bool CreateVertexArrayObject();

	AUint GetVertexArrayObject();
	AUint GetIndexesBufferObject();
	AUint GetVerticesBufferObject();
	AUint GetNormalsBufferObject();
	//AUint GetColorsBufferObject();
	AUint GetTextureCoordsBufferObject();

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

protected:	
	AnimaVertex3f*	_vertices;
	ASizeT			_verticesNumber;
	
	AnimaVertex3f*	_normals;
	ASizeT			_normalsNumber;
	
	AnimaVertex2f*	_textureCoords;
	ASizeT			_textureCoordsNumber;
	
	AnimaFace*		_faces;
	ASizeT			_facesNumber;

	AnimaMaterial*	_material;

	AnimaString		_meshName;

	AnimaModel*		_parentModel;
	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;

	AUint			_vertexArrayObject;
	AUint			_indexesBufferObject;
	AUint			_verticesBufferObject;
	AUint			_normalsBufferObject;
	//AUint			_colorsBufferObject;
	AUint			_textureCoordsBufferObject;
	
	AnimaAllocator* _allocator;

	bool _needsBuffersUpdate;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMesh__) */
