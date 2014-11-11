//
//  AnimaStackAllocator.cpp
//  Anima
//
//  Created by Marco Zille on 08/11/14.
//
//

#include "AnimaStackAllocator.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaStackAllocator::AnimaStackAllocator(ASizeT size, void* start)
: AnimaAllocator(size, start), _currentPosition(start)
{
	ANIMA_ASSERT(size > 0);
	
	#if defined ANIMA_ALLOCATOR_DEBUG
	_prevPosition = nullptr;
	#endif
}

AnimaStackAllocator::~AnimaStackAllocator()
{
	#if defined ANIMA_ALLOCATOR_DEBUG
	_prevPosition = nullptr;
	#endif
	
	_currentPosition = nullptr;
}

void* AnimaStackAllocator::Allocate(ASizeT size, AU8 alignment)
{
	ANIMA_ASSERT(size > 0);
	
	AU8 adjustment = AnimaPointerMath::alignForwardAdjustmentWithHeader(_currentPosition, alignment, sizeof(AnimaStackAllocationHeader));
	
	// Controllo di non tentare di allocare più memoria di quella che era stata stanziata per questo allocator
	if(_usedMemory + adjustment + size > _size)
		return nullptr;
	
	void* alignedAddress = AnimaPointerMath::add(_currentPosition, adjustment);
	
	AnimaStackAllocationHeader* header = (AnimaStackAllocationHeader*)(AnimaPointerMath::subtract(alignedAddress, sizeof(AnimaStackAllocationHeader)));
	
	header->_adjustment = adjustment;
	
	#if defined ANIMA_ALLOCATOR_DEBUG
	header->_prevAddress = _prevPosition;
	_prevPosition = alignedAddress;
	#endif
	
	_currentPosition = AnimaPointerMath::add(alignedAddress, size);
	_usedMemory += size + adjustment;
	_numAllocations++;
	
	return alignedAddress;
}

void AnimaStackAllocator::Deallocate(void* p)
{
	#if defined ANIMA_ALLOCATOR_DEBUG
	ANIMA_ASSERT(_prevPosition == p);
	#endif
	
	AnimaStackAllocationHeader* header = (AnimaStackAllocationHeader*)(AnimaPointerMath::subtract(p, sizeof(AnimaStackAllocationHeader)));
	
	_usedMemory -= (AUptr)_currentPosition - (AUptr)p + header->_adjustment;
	_currentPosition = AnimaPointerMath::subtract(p, header->_adjustment);
	
	
	#if defined ANIMA_ALLOCATOR_DEBUG
	_prevPosition = header->_prevAddress;
	#endif
	
	_numAllocations--;
}

END_ANIMA_ENGINE_CORE_NAMESPACE