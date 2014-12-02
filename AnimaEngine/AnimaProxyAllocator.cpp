//
//  AnimaProxyAllocator.cpp
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#include "AnimaProxyAllocator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaProxyAllocator::AnimaProxyAllocator(AnimaAllocator& allocator)
: AnimaAllocator(allocator.GetSize(), allocator.GetStart()), _allocator(allocator)
{
	
}

AnimaProxyAllocator::~AnimaProxyAllocator()
{
	
}

void* AnimaProxyAllocator::Allocate(ASizeT size, AU8 alignment)
{
	ANIMA_ASSERT(size != 0);
	_numAllocations++;
	
	ASizeT mem = _allocator.GetUsedMemory();
	
	void* p = _allocator.Allocate(size, alignment);
	
	_usedMemory += _allocator.GetUsedMemory() - mem;
	
	return p;
}

void AnimaProxyAllocator::Deallocate(void* p)
{
	_numAllocations--;
	
	ASizeT mem = _allocator.GetUsedMemory();
	
	_allocator.Deallocate(p);
	
	_usedMemory -= mem - _allocator.GetUsedMemory();
}

END_ANIMA_ENGINE_NAMESPACE
