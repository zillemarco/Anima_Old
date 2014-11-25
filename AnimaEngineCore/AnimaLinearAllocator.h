//
//  AnimaLinearAllocator.h
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#ifndef __Anima__AnimaLinearAllocator__
#define __Anima__AnimaLinearAllocator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocator.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaLinearAllocator : public AnimaAllocator
{
public:
	AnimaLinearAllocator(ASizeT size, void* start);
	~AnimaLinearAllocator();
	
	void* Allocate(ASizeT size, AU8 alignment) override;
	
	void Deallocate(void* p) override;
	
	void Clear();
	
private:
	AnimaLinearAllocator(const AnimaLinearAllocator&);
	AnimaLinearAllocator& operator=(const AnimaLinearAllocator&);
	
	void* _currentPos;
};

BEGIN_ANIMA_ALLOCATOR_NAMESPACE
{
	inline AnimaLinearAllocator* NewAnimaLinearAllocator(size_t size, AnimaAllocator& allocator)
	{
		void* p = allocator.Allocate(size + sizeof(AnimaLinearAllocator), ANIMA_ENGINE_CORE_ALIGN_OF(AnimaLinearAllocator));
		return new (p) AnimaLinearAllocator(size, AnimaPointerMath::add(p, sizeof(AnimaLinearAllocator)));
	}
	
	inline void DeleteAnimaLinearAllocator(AnimaLinearAllocator& linearAllocator, AnimaAllocator& allocator)
	{
		linearAllocator.~AnimaLinearAllocator();
		
		allocator.Deallocate(&linearAllocator);
	}
}

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaLinearAllocator__) */
