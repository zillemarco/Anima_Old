//
//  AnimaMappedArray.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMappedArray__
#define __Anima__AnimaMappedArray__

#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class TYPE, class ARG_TYPE>
class ANIMA_ENGINE_EXPORT AnimaMappedArray
{
public:
	AnimaMappedArray(AnimaAllocator* allocator)
	{
		_allocator = allocator;
	}

	AnimaMappedArray(const AnimaMappedArray& src)
	{
		_allocator = src._allocator;
	}

	virtual ~AnimaMappedArray()
	{
	}

	AnimaMappedArray& operator=(const AnimaMappedArray& src)
	{
		Copy(src);
		return *this;
	}

public:
protected:
	AnimaAllocator* _allocator;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMappedArray__) */
