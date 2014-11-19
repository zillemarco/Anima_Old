//
//  AnimaVertex.h
//  Anima
//
//  Created by Marco Zille on 19/11/14.
//
//

#ifndef __Anima__AnimaVertex__
#define __Anima__AnimaVertex__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

template<class Type, ASizeT Size>
class ANIMA_ENGINE_CORE_EXPORT AnimaVertex
{
public:
	AnimaVertex(AnimaAllocator* allocator)
	{
		ANIMA_ASSERT(allocator != nullptr);
		
		_allocator = allocator;
		_size = Size;
		
		_data = (Type*)_allocator->Allocate(sizeof(Type) * _size, __alignof__(Type));
	}
	
	AnimaVertex(const AnimaVertex& src)
	{
		_data = nullptr;
		_size = src._size;
		_allocator = src._allocator;
		
		_data = _allocator->Allocate(sizeof(Type) * _size, __alignof__(Type));
		
		memcpy(_data, src._data, sizeof(Type) * _size);
	}
	
	AnimaVertex(const AnimaVertex&& src)
	: _data(src._data)
	, _size(src._size)
	{
		src._data = nullptr;
		src._size = 0;
	}
	
	~AnimaVertex()
	{
		ANIMA_ASSERT(_allocator != nullptr);
		
		_allocator->Deallocate(_data);
		_data = nullptr;
	}
	
	AnimaVertex& operator=(const AnimaVertex& src)
	{
		if(this != &src)
		{
			if(_data != nullptr)
			{
				ANIMA_ASSERT(_allocator != nullptr);
				_allocator->Deallocate(_data);
				_data = nullptr;
			}
			
			_size = src._size;
			_data = (Type*)_allocator->Allocate(sizeof(Type) * _size, __alignof__(Type));
			
			memcpy(_data, src._data, sizeof(Type) * _size);
		}
		
		return *this;
	}
	
	AnimaVertex& operator=(const AnimaVertex&& src)
	{
		if(this != &src)
		{
			_data = src._data;
			_size = src._size;
			
			src._data = nullptr;
			src._size = 0;
		}
		
		return *this;
	}
	
	Type& operator[](ASizeT index)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return _data[index];
	}
	
	const Type& operator[](ASizeT index) const
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return const_cast<Type&>(*this)[index];
	}
	
protected:
	AnimaAllocator* _allocator;
	
	Type*	_data;
	ASizeT	_size;
};

typedef AnimaVertex<double, 3> AnimaVertex3d;
typedef AnimaVertex<double, 4> AnimaVertex4d;

typedef AnimaVertex<float, 3> AnimaVertex3f;
typedef AnimaVertex<float, 4> AnimaVertex4f;

typedef AnimaVertex<char, 3> AnimaColor3b;
typedef AnimaVertex<char, 4> AnimaColor4b;

typedef AnimaVertex<float, 3> AnimaColor3f;
typedef AnimaVertex<float, 4> AnimaColor4f;


END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaVertex__) */
