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

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMesh
{
public:
	AnimaMesh(AnimaEngine* engine);
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

	void Draw(AnimaMatrix transformMatrix);

	bool CreateBuffers();
	void UpdateBuffers();

	void SetUpdateBuffers(bool bUpdate = true);
	bool NeedsBuffersUpdate();

private:
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

	AnimaString		_meshName;

	AUint			_vertexArrayObject;
	AUint			_indexesBufferObject;
	AUint			_verticesBufferObject;
	AUint			_normalsBufferObject;
	//AUint			_colorsBufferObject;
	AUint			_textureCoordsBufferObject;
	
	AnimaEngine* _engine;

	bool _needsBuffersUpdate;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMesh__) */
