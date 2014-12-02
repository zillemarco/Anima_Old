//
//  AnimaProxyAllocator.h
//  Anima
//
//  Created by Marco Zille on 16/11/14.
//
//

#ifndef __Anima__AnimaProxyAllocator__
#define __Anima__AnimaProxyAllocator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaProxyAllocator : public AnimaAllocator
{
public:
	AnimaProxyAllocator(AnimaAllocator& allocator);
	~AnimaProxyAllocator();
	
	void* Allocate(ASizeT size, AU8 alignment) override;
	
	void Deallocate(void* p) override;
	
private:
	AnimaProxyAllocator(const AnimaProxyAllocator&); //Prevent copies because it might cause errors
	AnimaProxyAllocator& operator=(const AnimaProxyAllocator&);
	
	AnimaAllocator& _allocator;
};

BEGIN_ANIMA_ALLOCATOR_NAMESPACE
{
	inline AnimaProxyAllocator* NewAnimaProxyAllocator(AnimaAllocator& allocator)
	{
		void* p = allocator.Allocate(sizeof(AnimaProxyAllocator), ANIMA_ENGINE_ALIGN_OF(AnimaProxyAllocator));
		return new (p) AnimaProxyAllocator(allocator);
	}
	
	inline void DeleteAnimaProxyAllocator(AnimaProxyAllocator& proxyAllocator, AnimaAllocator& allocator)
	{
		proxyAllocator.~AnimaProxyAllocator();
		
		allocator.Deallocate(&proxyAllocator);
	}
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaProxyAllocator__) */
