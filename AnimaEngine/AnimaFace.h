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
	AnimaFace(AnimaAllocator* allocator);
	AnimaFace(const AnimaFace& src);
	AnimaFace(AnimaFace&& src);
	~AnimaFace();
	
	AnimaFace& operator=(const AnimaFace& src);
	AnimaFace& operator=(AnimaFace&& src);
	
public:
	void SetIndexes(AUint* indexes, ASizeT n);
	void AddIndex(const AUint& index);
	AUint GetIndex(ASizeT index) const;
	AUint* GetIndexes();
	const AUint* GetConstIndexes();
	bool GetIndexes(AUint* outIndexes, ASizeT& inOutSize);
	bool GetConstIndexes(AUint* outIndexes, ASizeT& inOutSize);
	void ClearIndexes();
	ASizeT GetIndexesCount();
	bool HasIndex(AUint index) const;

	void SetNormal(const AnimaVertex3f& n);
	void SetNormal(const AFloat& x, const AFloat& y, const AFloat& z);
	AnimaVertex3f GetNormal() const;
				
protected:
	AUint*	_indexes;
	ASizeT	_indexesNumber;

	AnimaVertex3f _normal;
	
	AnimaAllocator* _allocator;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFace__) */
