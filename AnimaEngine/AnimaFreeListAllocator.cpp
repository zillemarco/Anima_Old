//
//  FreeListAllocator.cpp
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#include "AnimaFreeListAllocator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFreeListAllocator::AnimaFreeListAllocator(ASizeT size, void* start)
: AnimaAllocator(size, start), _freeBlocks((AnimaFreeListAllocatorFreeBlock*)start)
{
	ANIMA_ASSERT(size > sizeof(AnimaFreeListAllocatorFreeBlock));
	
	_freeBlocks->size  = size;
	_freeBlocks->next = nullptr;
}

AnimaFreeListAllocator::~AnimaFreeListAllocator()
{
	_freeBlocks        = nullptr;
}

void* AnimaFreeListAllocator::Allocate(ASizeT size, AU8 alignment)
{
	ANIMA_ASSERT(size != 0 && alignment != 0);
	
	AnimaFreeListAllocatorFreeBlock* prev_free_block = nullptr;
	AnimaFreeListAllocatorFreeBlock* free_block     = _freeBlocks;
	
	while(free_block != nullptr)
	{
		//Calculate adjustment needed to keep object correctly aligned
		AU8 adjustment = AnimaPointerMath::alignForwardAdjustmentWithHeader(free_block, alignment, sizeof(AnimaFreeListAllocationHeader));
		
		size_t total_size = size + adjustment;
		
		//If allocation doesn't fit in this FreeBlock, try the next
		if(free_block->size < total_size)
		{
			prev_free_block = free_block;
			free_block = free_block->next;
			continue;
		}
		
		static_assert(sizeof(AnimaFreeListAllocationHeader) >= sizeof(AnimaFreeListAllocatorFreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");
		
		//If allocations in the remaining memory will be impossible
		if(free_block->size - total_size <= sizeof(AnimaFreeListAllocationHeader))
		{
			//Increase allocation size instead of creating a new FreeBlock
			total_size = free_block->size;
			
			if(prev_free_block != nullptr)
				prev_free_block->next = free_block->next;
			else
				_freeBlocks = free_block->next;
		}
		else
		{
			//Else create a new FreeBlock containing remaining memory
			AnimaFreeListAllocatorFreeBlock* next_block = (AnimaFreeListAllocatorFreeBlock*)(AnimaPointerMath::add(free_block, total_size));
			next_block->size = free_block->size - total_size;
			next_block->next = free_block->next;
			
			if(prev_free_block != nullptr)
				prev_free_block->next = next_block;
			else
				_freeBlocks = next_block;
		}
		
		AUptr aligned_address = (AUptr)free_block + adjustment;
		
		AnimaFreeListAllocationHeader* header = (AnimaFreeListAllocationHeader*)(aligned_address - sizeof(AnimaFreeListAllocationHeader));
		header->size             = total_size;
		header->adjustment       = adjustment;
		
		_usedMemory += total_size;
		_numAllocations++;
		
		ANIMA_ASSERT(AnimaPointerMath::alignForwardAdjustment((void*)aligned_address, alignment) == 0);
		
		return (void*)aligned_address;
	}
	
	//ASSERT(false && "Couldn't find free block large enough!");
	
	return nullptr;
}

void AnimaFreeListAllocator::Deallocate(void* p)
{
	ANIMA_ASSERT(p != nullptr);
	
	AnimaFreeListAllocationHeader* header = (AnimaFreeListAllocationHeader*)AnimaPointerMath::subtract(p, sizeof(AnimaFreeListAllocationHeader));
	
	AUptr  block_start = reinterpret_cast<AUptr>(p) - header->adjustment;
	ASizeT block_size  = header->size;
	AUptr  block_end   = block_start + block_size;
	
	AnimaFreeListAllocatorFreeBlock* prev_free_block = nullptr;
	AnimaFreeListAllocatorFreeBlock* free_block = _freeBlocks;
	
	while(free_block != nullptr)
	{
		if( (AUptr) free_block >= block_end )
			break;
		
		prev_free_block = free_block;
		free_block = free_block->next;
	}
	
	if(prev_free_block == nullptr)
	{
		prev_free_block = (AnimaFreeListAllocatorFreeBlock*) block_start;
		prev_free_block->size = block_size;
		prev_free_block->next = _freeBlocks;
		
		_freeBlocks = prev_free_block;
	}
	else if((AUptr) prev_free_block + prev_free_block->size == block_start)
	{
		prev_free_block->size += block_size;
	}
	else
	{
		AnimaFreeListAllocatorFreeBlock* temp = (AnimaFreeListAllocatorFreeBlock*) block_start;
		temp->size = block_size;
		temp->next = prev_free_block->next;
		prev_free_block->next = temp;
		
		prev_free_block = temp;
	}
	
	if(free_block != nullptr && (AUptr) free_block == block_end)
	{
		prev_free_block->size += free_block->size;
		prev_free_block->next = free_block->next;
	}
	
	_numAllocations--;
	_usedMemory -= block_size;
}

END_ANIMA_ENGINE_NAMESPACE
