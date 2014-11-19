//
//  AnimaVertex.h
//  Anima
//
//  Created by Marco Zille on 19/11/14.
//
//

#ifndef __Anima__AnimaVertex__
#define __Anima__AnimaVertex__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

template<class Type, ASizeT Size>
class ANIMA_ENGINE_CORE_EXPORT AnimaVertex
{
public:
	AnimaVertex(AnimaAllocator* allocator);
	AnimaVertex(const AnimaVertex& src);
	AnimaVertex(AnimaVertex&& src);
	~AnimaVertex();
	
	AnimaVertex& operator=(const AnimaVertex& src);
	AnimaVertex& operator=(AnimaVertex&& src);
	
	Type& operator[](ASizeT index);
	const Type& operator[](ASizeT index) const;
	
protected:
	AnimaAllocator* _allocator;
	
	Type*	_data;
	ASizeT	_size;
};

template class AnimaVertex<double, 3>;
template class AnimaVertex<double, 4>;
template class AnimaVertex<float, 3>;
template class AnimaVertex<float, 4>;
template class AnimaVertex<char, 3>;
template class AnimaVertex<char, 4>;

typedef AnimaVertex<double, 3> AnimaVertex3d;
typedef AnimaVertex<double, 4> AnimaVertex4d;

typedef AnimaVertex<float, 3> AnimaVertex3f;
typedef AnimaVertex<float, 4> AnimaVertex4f;

typedef AnimaVertex<char, 3> AnimaColor3b;
typedef AnimaVertex<char, 4> AnimaColor4b;

typedef AnimaVertex<float, 3> AnimaColor3f;
typedef AnimaVertex<float, 4> AnimaColor4f;

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaVertex__) */
