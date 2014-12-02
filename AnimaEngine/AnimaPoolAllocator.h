//
//  AnimaPoolAllocator.h
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#ifndef __Anima__AnimaPoolAllocator__
#define __Anima__AnimaPoolAllocator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaPoolAllocator : public AnimaAllocator
{
public:
	AnimaPoolAllocator(ASizeT objectSize, AU8 objectAlignment, ASizeT size, void* mem);
	~AnimaPoolAllocator();
	
	void* Allocate(ASizeT size, AU8 alignment) override;
	
	void Deallocate(void* p) override;
	
private:
	AnimaPoolAllocator(const AnimaPoolAllocator&); //Prevent copies because it might cause errors
	AnimaPoolAllocator& operator=(const AnimaPoolAllocator&);
	
	ASizeT     _objectSize;
	AU8        _objectAlignment;
	
	void**     _freeList;
};

BEGIN_ANIMA_ALLOCATOR_NAMESPACE
{
	inline AnimaPoolAllocator* NewAnimaPoolAllocator(ASizeT objectSize, AU8 objectAlignment, ASizeT size, AnimaAllocator& allocator)
	{
		void* p = allocator.Allocate(size + sizeof(AnimaPoolAllocator), ANIMA_ENGINE_ALIGN_OF(AnimaPoolAllocator));
		return new (p) AnimaPoolAllocator(objectSize, objectAlignment, size, AnimaPointerMath::add(p, sizeof(AnimaPoolAllocator)));
	}
	
	inline void DeleteAnimaPoolAllocator(AnimaPoolAllocator& poolAllocator, AnimaAllocator& allocator)
	{
		poolAllocator.~AnimaPoolAllocator();
		allocator.Deallocate(&poolAllocator);
	}
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaPoolAllocator__) */
