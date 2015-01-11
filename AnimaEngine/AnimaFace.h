//
//  AnimaFace.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFace__
#define __Anima__AnimaFace__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFace
{
public:
	AnimaFace(AnimaEngine* engine);
	AnimaFace(const AnimaFace& src);
	AnimaFace(AnimaFace&& src);
	~AnimaFace();
	
	AnimaFace& operator=(const AnimaFace& src);
	AnimaFace& operator=(AnimaFace&& src);
	
public:
	void SetIndexes(ASizeT* indexes, ASizeT n);
	void AddIndex(const ASizeT& index);
	
	void ClearIndexes();

	ASizeT GetIndexesCount();

	ASizeT* GetIndexes();
	const ASizeT* GetConstIndexes();
	bool GetIndexes(ASizeT* outIndexes, ASizeT& inOutSize);
	bool GetConstIndexes(ASizeT* outIndexes, ASizeT& inOutSize);
	
protected:
	ASizeT* _indexes;
	ASizeT	_indexesNumber;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFace__) */
