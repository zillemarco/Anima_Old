//
//  AnimaAllocator.h
//  Anima
//
//  Created by Marco Zille on 08/11/14.
//
//

#ifndef __Anima__AnimaAllocator__
#define __Anima__AnimaAllocator__

#include <new>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAssert.h"

#define BEGIN_ANIMA_ALLOCATOR_NAMESPACE namespace AnimaAllocatorNamespace
#define BEGIN_ANIMA_POINTER_MATH_NAMESPACE namespace AnimaPointerMath

BEGIN_ANIMA_ENGINE_NAMESPACE

BEGIN_ANIMA_POINTER_MATH_NAMESPACE
{
	void*		alignForward(void* address, AU8 alignment);
	const void* alignForward(const void* address, AU8 alignment);
	
	void*		alignBackward(void* address, AU8 alignment);
	const void* alignBackward(const void* address, AU8 alignment);
	
	AU8			alignForwardAdjustment(const void* address, AU8 alignment);
	AU8			alignForwardAdjustmentWithHeader(const void* address, AU8 alignment, AU8 headerSize);
	
	AU8			alignBackwardAdjustment(const void* address, AU8 alignment);
	
	void*		add(void* p, ASizeT x);
	const void*	add(const void* p, ASizeT x);
	
	void*		subtract(void* p, ASizeT x);
	const void*	subtract(const void* p, ASizeT x);
};

class ANIMA_ENGINE_EXPORT AnimaAllocator
{
public:
	AnimaAllocator(ASizeT size, void* start)
	{
		_start	= start;
		_size	= size;
		
		_usedMemory		= 0;
		_numAllocations = 0;
		_numArrayAllocations = 0;
	}
	
	virtual ~AnimaAllocator()
	{
		ANIMA_ASSERT(_numAllocations == 0 && _usedMemory == 0 && _numArrayAllocations == 0);
		
		_start	= nullptr;
		_size	= 0;
	}
	
	virtual void* Allocate(ASizeT size, AU8 alignment = 4) = 0;
	virtual void Deallocate(void* p) = 0;
	
	void* GetStart() const
	{
		return _start;
	}
	
	ASizeT GetSize() const
	{
		return _size;
	}
	
	ASizeT GetUsedMemory() const
	{
		return _usedMemory;
	}
	
	ASizeT GetNumAllocations() const
	{
		return _numAllocations;
	}
	
	ASizeT GetNumArrayAllocations() const
	{
		return _numArrayAllocations;
	}
	
	void AddArrayAllocation() { _numArrayAllocations++; }
	void SubArrayAllocation() { _numArrayAllocations--; }

	void Dump() const
	{
		printf("\t- Size:\t\t%f MB\n", _size / (1024.0f * 1024.0f));
		printf("\t- Used memory:\t%f MB\n", _usedMemory / (1024.0f * 1024.0f));
		printf("\t- Allocations:\t%lu\n", _numAllocations);
	}
	
protected:
	void*	_start;
	ASizeT	_size;
	
	ASizeT	_usedMemory;
	ASizeT	_numAllocations;
	ASizeT	_numArrayAllocations;
};

BEGIN_ANIMA_ALLOCATOR_NAMESPACE
{
	template<class T> T* AllocateNew(AnimaAllocator& allocator)
	{
		return new (allocator.Allocate(sizeof(T), ANIMA_ENGINE_ALIGN_OF(T))) T;
	}
	
	template<class T> T* AllocateNew(AnimaAllocator& allocator, const T& t)
	{
		return new (allocator.Allocate(sizeof(T), ANIMA_ENGINE_ALIGN_OF(T))) T(t);
	}

	template<class T> T* AllocateNew(AnimaAllocator& allocator, T&& t)
	{
		return new (allocator.Allocate(sizeof(T), ANIMA_ENGINE_ALIGN_OF(T))) T(t);
	}

	template<class T, typename... Args> T* AllocateNew(AnimaAllocator& allocator, Args... args)
	{
		return new (allocator.Allocate(sizeof(T), ANIMA_ENGINE_ALIGN_OF(T))) T(args...);
	}
	
	template<class T> void DeallocateObject(AnimaAllocator& allocator, T& object)
	{
		object.~T();
		allocator.Deallocate(&object);
	}
	
	template<class T> void DeallocateObject(AnimaAllocator& allocator, T* object)
	{
		object->~T();
		allocator.Deallocate(object);
	}
	
	template<class T> T* AllocateArray(AnimaAllocator& allocator, ASizeT length)
	{
		ANIMA_ASSERT(length > 0);
		AU8 headerSize = sizeof(ASizeT) / sizeof(T);
		
		if(sizeof(ASizeT) % sizeof(T) > 0)
			headerSize += 1;
		
		// Alloco spazio extra all'inizio per memorizzare la lunghezza dell'array prima dell'array
		T* p = ((T*)allocator.Allocate(sizeof(T) * (length + headerSize), ANIMA_ENGINE_ALIGN_OF(T))) + headerSize;
		
		allocator.AddArrayAllocation();
		
		// Memorizzo la lunghezza dell'array in memoria prima dell'array vero e proprio
		*(((ASizeT*)p) - 1) = length;
		
		for(ASizeT i = 0; i < length; i++)
			new (&p[i]) T;
		
		return p;
	}

	template<class T> T* AllocateArray(AnimaAllocator& allocator, ASizeT length, const T& t)
	{
		ANIMA_ASSERT(length > 0);
		AU8 headerSize = sizeof(ASizeT) / sizeof(T);

		if (sizeof(ASizeT) % sizeof(T) > 0)
			headerSize += 1;

		// Alloco spazio extra all'inizio per memorizzare la lunghezza dell'array prima dell'array
		T* p = ((T*)allocator.Allocate(sizeof(T) * (length + headerSize), ANIMA_ENGINE_ALIGN_OF(T))) + headerSize;
		
		allocator.AddArrayAllocation();

		// Memorizzo la lunghezza dell'array in memoria prima dell'array vero e proprio
		*(((ASizeT*)p) - 1) = length;

		for (ASizeT i = 0; i < length; i++)
			new (&p[i]) T(t);

		return p;
	}

	template<class T> T* AllocateArray(AnimaAllocator& allocator, ASizeT length, T&& t)
	{
		ANIMA_ASSERT(length > 0);
		AU8 headerSize = sizeof(ASizeT) / sizeof(T);

		if (sizeof(ASizeT) % sizeof(T) > 0)
			headerSize += 1;

		// Alloco spazio extra all'inizio per memorizzare la lunghezza dell'array prima dell'array
		T* p = ((T*)allocator.Allocate(sizeof(T) * (length + headerSize), ANIMA_ENGINE_ALIGN_OF(T))) + headerSize;
		
		allocator.AddArrayAllocation();

		// Memorizzo la lunghezza dell'array in memoria prima dell'array vero e proprio
		*(((ASizeT*)p) - 1) = length;

		for (ASizeT i = 0; i < length; i++)
			new (&p[i]) T(t);

		return p;
	}

	template<class T, typename... Args> T* AllocateArray(AnimaAllocator& allocator, ASizeT length, Args... args)
	{
		ANIMA_ASSERT(length > 0);
		AU8 headerSize = sizeof(ASizeT) / sizeof(T);

		if (sizeof(ASizeT) % sizeof(T) > 0)
			headerSize += 1;

		// Alloco spazio extra all'inizio per memorizzare la lunghezza dell'array prima dell'array
		T* p = ((T*)allocator.Allocate(sizeof(T) * (length + headerSize), ANIMA_ENGINE_ALIGN_OF(T))) + headerSize;
		
		allocator.AddArrayAllocation();

		// Memorizzo la lunghezza dell'array in memoria prima dell'array vero e proprio
		*(((ASizeT*)p) - 1) = length;

		for (ASizeT i = 0; i < length; i++)
			new (&p[i]) T(args...);

		return p;
	}
	
	template<class T> void DeallocateArray(AnimaAllocator& allocator, T* array)
	{
		ANIMA_ASSERT(array != nullptr);
		
		ASizeT length = *(((ASizeT*)array) - 1);
		
		for(ASizeT i = 0; i < length; i++)
			array[i].~T();
		
		allocator.SubArrayAllocation();
		
		// Calcolo quanto spazio avevo aggiunto all'array per memorizzare la lunghezza
		AU8 headerSize = sizeof(ASizeT) / sizeof(T);
		
		if(sizeof(ASizeT) % sizeof(T) > 0)
			headerSize += 1;
		
		allocator.Deallocate(array - headerSize);
	}
};

BEGIN_ANIMA_POINTER_MATH_NAMESPACE
{
	inline void* alignForward(void* address, AU8 alignment)
	{
		return (void*)((reinterpret_cast<AUptr>(address) + static_cast<AUptr>(alignment - 1) ) & static_cast<AUptr>(~(alignment - 1)));
	}
	
	inline const void* alignForward(const void* address, AU8 alignment)
	{
		return (void*)((reinterpret_cast<AUptr>(address) + static_cast<AUptr>(alignment - 1) ) & static_cast<AUptr>(~(alignment - 1)));
	}
	
	inline void* alignBackward(void* address, AU8 alignment)
	{
		return (void*)(reinterpret_cast<AUptr>(address) & static_cast<AUptr>(~(alignment - 1)));
	}
	
	inline const void* alignBackward(const void* address, AU8 alignment)
	{
		return (void*)(reinterpret_cast<AUptr>(address) & static_cast<AUptr>(~(alignment - 1)));
	}
	
	inline AU8 alignForwardAdjustment(const void* address, AU8 alignment)
	{
		AU8 adjustment =  alignment - (reinterpret_cast<AUptr>(address) & static_cast<AUptr>(alignment - 1));
		
		if(adjustment == alignment)
			return 0; // ritorno 0 come allineamento perchè è già allineato
		
		return adjustment;
	}
	
	inline AU8 alignForwardAdjustmentWithHeader(const void* address, AU8 alignment, AU8 headerSize)
	{
		AU8 adjustment =  alignForwardAdjustment(address, alignment);
		
		AU8 neededSpace = headerSize;
		
		if(adjustment < neededSpace)
		{
			neededSpace -= adjustment;
			
			// Aumento l'aggiustamento per farci stare l'header
			adjustment += alignment * (neededSpace / alignment);
			
			if(neededSpace % alignment > 0)
				adjustment += alignment;
		}
		
		return adjustment;
	}
	
	inline AU8 alignBackwardAdjustment(const void* address, AU8 alignment)
	{
		AU8 adjustment =  reinterpret_cast<AUptr>(address) & static_cast<AUptr>(alignment - 1);
		
		if(adjustment == alignment)
			return 0; // ritorno 0 come allineamento perchè è già allineato
		
		return adjustment;
	}
	
	inline void* add(void* p, ASizeT x)
	{
		return (void*)(reinterpret_cast<AUptr>(p) + x);
	}
	
	inline const void* add(const void* p, ASizeT x)
	{
		return (const void*)(reinterpret_cast<AUptr>(p) + x);
	}
	
	inline void* subtract(void* p, ASizeT x)
	{
		return (void*)(reinterpret_cast<AUptr>(p) - x);
	}
	
	inline const void* subtract(const void* p, ASizeT x)
	{
		return (const void*)(reinterpret_cast<AUptr>(p) - x);
	}
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaAllocator__) */
