//
//  AnimaLinearAllocator.cpp
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#include "AnimaLinearAllocator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaLinearAllocator::AnimaLinearAllocator(ASizeT size, void* start)
: AnimaAllocator(size, start), _currentPos(start)
{
	ANIMA_ASSERT(size > 0);
}

AnimaLinearAllocator::~AnimaLinearAllocator()
{
	_currentPos   = nullptr;
}

void* AnimaLinearAllocator::Allocate(ASizeT size, AU8 alignment)
{
	ANIMA_ASSERT(size != 0);
	
	AU8 adjustment =  AnimaPointerMath::alignForwardAdjustment(_currentPos, alignment);
	
	if(_usedMemory + adjustment + size > _size)
		return nullptr;
	
	AUptr aligned_address = (AUptr)_currentPos + adjustment;
	
	_currentPos = (void*)(aligned_address + size);
	
	_usedMemory += size + adjustment;
	_numAllocations++;
	
	return (void*)aligned_address;
}

void AnimaLinearAllocator::Deallocate(void* p)
{
	ANIMA_ASSERT(false && "Use clear() instead");
}

void AnimaLinearAllocator::Clear()
{
	_numAllocations     = 0;
	_usedMemory         = 0;
	
	_currentPos   = _start;
}

END_ANIMA_ENGINE_NAMESPACE
