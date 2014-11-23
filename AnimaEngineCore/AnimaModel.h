//
//  AnimaEngineModel.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaModel__
#define __Anima__AnimaModel__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaModel
{
public:
	AnimaModel(AnimaAllocator* allocator);
	AnimaModel(const AnimaModel& src);
	AnimaModel(AnimaModel&& src);
	~AnimaModel();
	
	AnimaModel& operator=(const AnimaModel& src);
	AnimaModel& operator=(AnimaModel&& src);
	
public:
	void SetVertices(AnimaVertex4f* v, ASizeT n);
	void SetVertices(AnimaVertex3f* v, ASizeT n);
	void AddVertex(const AnimaVertex4f& v);
	void AddVertex(const AnimaVertex3f& v);
	
	void SetNormals(AnimaVertex4f* v, ASizeT n);
	void SetNormals(AnimaVertex3f* v, ASizeT n);
	void AddNormal(const AnimaVertex4f& v);
	void AddNormal(const AnimaVertex3f& v);
	
	void SetTextureCoords(AnimaVertex2f* v, ASizeT n);
	void AddTextureCoord(const AnimaVertex2f& v);
	
	void SetIndexes(ASizeT* indexes, ASizeT n);
	void AddIndex(const ASizeT& index);
	
	void SetChildren(AnimaModel* children, ASizeT n);
	void AddChild(const AnimaModel& child);
	
	void ClearChildren();
	void ClearVertices();
	void ClearNormals();
	void ClearTextureCoords();
	void ClearIndexes();
	
protected:
	AnimaModel*		_modelChildren;
	ASizeT			_modelChildrenNumber;
	
	AnimaVertex4f*	_vertices;
	ASizeT			_verticesNumber;
	
	AnimaVertex4f*	_normals;
	ASizeT			_normalsNumber;
	
	AnimaVertex2f*	_textureCoords;
	ASizeT			_textureCoordsNumber;
	
	ASizeT*	_indexes;
	ASizeT	_indexesNumber;
		
	AnimaAllocator* _allocator;
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
