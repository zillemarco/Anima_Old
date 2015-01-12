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
	void SetIndexes(AUint* indexes, ASizeT n);
	void AddIndex(const AUint& index);
	
	void ClearIndexes();

	ASizeT GetIndexesCount();

	AUint* GetIndexes();
	const AUint* GetConstIndexes();
	bool GetIndexes(AUint* outIndexes, ASizeT& inOutSize);
	bool GetConstIndexes(AUint* outIndexes, ASizeT& inOutSize);
	
protected:
	AUint*	_indexes;
	ASizeT	_indexesNumber;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFace__) */
