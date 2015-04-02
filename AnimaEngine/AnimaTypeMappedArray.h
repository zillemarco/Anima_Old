//
//  AnimaTypeMappedArray.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTypeMappedArray__
#define __Anima__AnimaTypeMappedArray__

#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class TYPE, class ARG_TYPE>
class ANIMA_ENGINE_EXPORT AnimaTypeMappedArray
{
public:
	AnimaTypeMappedArray(AnimaAllocator* allocator)
	{
		_allocator = allocator;
	}

	AnimaTypeMappedArray(const AnimaTypeMappedArray& src)
	{
		_allocator = src._allocator;
	}

	virtual ~AnimaTypeMappedArray()
	{
	}

	AnimaTypeMappedArray& operator=(const AnimaTypeMappedArray& src)
	{
		Copy(src);
		return *this;
	}

public:
protected:
	AnimaAllocator* _allocator;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTypeMappedArray__) */
