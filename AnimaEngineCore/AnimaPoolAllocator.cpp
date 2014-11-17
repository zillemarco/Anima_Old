//
//  AnimaPoolAllocator.cpp
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#include "AnimaPoolAllocator.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaPoolAllocator::AnimaPoolAllocator(ASizeT objectSize, AU8 objectAlignment, ASizeT size, void* mem)
: AnimaAllocator(size, mem), _objectSize(objectSize), _objectAlignment(objectAlignment)
{
	ANIMA_ASSERT(objectSize >= sizeof(void*));
	
	//Calculate adjustment needed to keep object correctly aligned
	AU8 adjustment = AnimaPointerMath::alignForwardAdjustment(mem, objectAlignment);
	
	_freeList = (void**)AnimaPointerMath::add(mem, adjustment);
	
	ASizeT numObjects = (size - adjustment) / objectSize;
	
	void** p = _freeList;
	
	//Initialize free blocks list
	for(ASizeT i = 0; i < numObjects-1; i++)
	{
		*p = AnimaPointerMath::add(p, objectSize );
		p = (void**) *p;
	}
	
	*p = nullptr;
}

AnimaPoolAllocator::~AnimaPoolAllocator()
{
	_freeList = nullptr;
}

void* AnimaPoolAllocator::Allocate(ASizeT size, AU8 alignment)
{
	ANIMA_ASSERT(size == _objectSize && alignment == _objectAlignment);
	
	if(_freeList == nullptr)
		return nullptr;
	
	void* p = _freeList;
	
	_freeList = (void**)(*_freeList);
	
	_usedMemory += size;
	_numAllocations++;
	
	return p;
}

void AnimaPoolAllocator::Deallocate(void* p)
{
	*((void**)p) = _freeList;
	
	_freeList = (void**)p;
	
	_usedMemory -= _objectSize;
	_numAllocations--;
}

END_ANIMA_ENGINE_CORE_NAMESPACE