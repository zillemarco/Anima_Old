//
//  AnimaStackAllocator.h
//  Anima
//
//  Created by Marco Zille on 08/11/14.
//
//

#ifndef __Anima__AnimaStackAllocator__
#define __Anima__AnimaStackAllocator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocator.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaStackAllocator : public AnimaAllocator
{
public:
	AnimaStackAllocator(ASizeT size, void* start);
	~AnimaStackAllocator();
	
	void* Allocate(ASizeT size, AU8 alignment) override;
	void Deallocate(void* p) override;
	
private:
	// Rendo privato il costruttore di copia perchè la copia potrebbe dare degli errori
	AnimaStackAllocator(const AnimaStackAllocator& src);
	
	// Rendo privato l'operatore di copia perchè la copia potrebbe dare degli errori
	AnimaStackAllocator& operator=(const AnimaStackAllocator& src);
	
	struct AnimaStackAllocationHeader
	{
		#if defined ANIMA_ALLOCATOR_DEBUG
		void* _prevAddress;
		#endif
		
		AU8 _adjustment;
	};
	
	#if defined ANIMA_ALLOCATOR_DEBUG
	void* _prevPosition;
	#endif
	
	void* _currentPosition;
};

BEGIN_ANIMA_ALLOCATOR_NAMESPACE
{
	inline AnimaStackAllocator* NewAnimaStackAllocator(ASizeT size, AnimaAllocator& allocator)
	{
		void* p = allocator.Allocate(size + sizeof(AnimaStackAllocator), __alignof(AnimaStackAllocator));
		return new (p) AnimaStackAllocator(size, AnimaPointerMath::add(p, sizeof(AnimaStackAllocator)));
	}
	
	inline void DeleteAnimaStackAllocator(AnimaStackAllocator& stackAllocator, AnimaAllocator& allocator)
	{
		stackAllocator.~AnimaStackAllocator();
		allocator.Deallocate(&stackAllocator);
	}
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaStackAllocator__) */
