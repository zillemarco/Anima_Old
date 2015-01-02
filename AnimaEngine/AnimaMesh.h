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
	void SetVertices(AnimaVertex4f* v, ASizeT n);
	void SetVertices(AnimaVertex3f* v, ASizeT n);
	void AddVertex(const AnimaVertex4f& v);
	void AddVertex(const AnimaVertex3f& v);
	ASizeT GetVerticesNumber();
	AnimaVertex4f GetVertex(ASizeT index);
	AnimaVertex4f* GetPVertex(ASizeT index);
	AnimaVertex4f* GetVertices();
	void ClearVertices();
	
	void SetNormals(AnimaVertex4f* v, ASizeT n);
	void SetNormals(AnimaVertex3f* v, ASizeT n);
	void AddNormal(const AnimaVertex4f& v);
	void AddNormal(const AnimaVertex3f& v);
	ASizeT GetNormalsNumber();
	AnimaVertex4f GetNormal(ASizeT index);
	AnimaVertex4f* GetPNormal(ASizeT index);
	AnimaVertex4f* GetNormals();
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
		
protected:	
	AnimaVertex4f*	_vertices;
	ASizeT			_verticesNumber;
	
	AnimaVertex4f*	_normals;
	ASizeT			_normalsNumber;
	
	AnimaVertex2f*	_textureCoords;
	ASizeT			_textureCoordsNumber;
	
	AnimaFace*		_faces;
	ASizeT			_facesNumber;

	AnimaString		_meshName;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMesh__) */
