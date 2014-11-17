//
//  AnimaFreeListAllocator.h
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#ifndef __Anima__AnimaFreeListAllocator__
#define __Anima__AnimaFreeListAllocator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocator.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaFreeListAllocator : public AnimaAllocator
{
public:
	AnimaFreeListAllocator(ASizeT size, void* start);
	~AnimaFreeListAllocator();
	
	void* Allocate(ASizeT size, AU8 alignment) override;
	
	void Deallocate(void* p) override;
	
private:
	
	struct AnimaFreeListAllocationHeader
	{
		ASizeT size;
		AU8    adjustment;
	};
	
	struct AnimaFreeListAllocatorFreeBlock
	{
		ASizeT					size;
		AnimaFreeListAllocatorFreeBlock* next;
	};
	
	AnimaFreeListAllocator(const AnimaFreeListAllocator&); //Prevent copies because it might cause errors
	AnimaFreeListAllocator& operator=(const AnimaFreeListAllocator&);
	
	AnimaFreeListAllocatorFreeBlock* _freeBlocks;
};

BEGIN_ANIMA_ALLOCATOR_NAMESPACE
{
	inline AnimaFreeListAllocator* NewAnimaFreeListAllocator(ASizeT size, AnimaAllocator& allocator)
	{
		void* p = allocator.Allocate(size + sizeof(AnimaFreeListAllocator), __alignof(AnimaFreeListAllocator));
		return new (p) AnimaFreeListAllocator(size, AnimaPointerMath::add(p, sizeof(AnimaFreeListAllocator)));
	}
	
	inline void DeleteAnimaFreeListAllocator(AnimaFreeListAllocator& freeListAllocator, AnimaAllocator& allocator)
	{
		freeListAllocator.~AnimaFreeListAllocator();
		
		allocator.Deallocate(&freeListAllocator);
	}
}

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaFreeListAllocator__) */
