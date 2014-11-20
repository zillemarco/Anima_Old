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
	~AnimaModel();
	
public:
	void SetVertices(AnimaVertex4f* v, ASizeT n);
	void SetVertices(AnimaVertex3f* v, ASizeT n);
	
	void SetNormals(AnimaVertex4f* v, ASizeT n);
	void SetNormals(AnimaVertex3f* v, ASizeT n);
	
	void SetTextureCoords(AnimaVertex2f* v, ASizeT n);
	
	void SetIndices(ASizeT* indices, ASizeT n);
	
	void ClearChildren();
	void ClearVertices();
	void ClearNormals();
	void ClearTextureCoords();
	void ClearIndices();
	
protected:
	AnimaModel*		_modelChildren;
	
	AnimaVertex4f*	_vertices;
	AnimaVertex4f*	_normals;
	AnimaVertex2f*	_textureCoords;
	ASizeT*			_indices;
	
	ASizeT _modelChildrenNumber;
	
	ASizeT _verticesNumber;
	ASizeT _normalsNumber;
	ASizeT _textureCoordsNumber;
	ASizeT _indicesNumber;
	

	
	AnimaAllocator* _allocator;
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
