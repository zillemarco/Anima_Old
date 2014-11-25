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
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

template<class Type, ASizeT Size>
class ANIMA_ENGINE_CORE_EXPORT AnimaVertex
{
public:
	AnimaVertex(AnimaEngine* engine)
	{
		ANIMA_ASSERT(engine != nullptr);
		
		_engine = engine;
		_size = Size;
		
		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_CORE_ALIGN_OF(Type));
	}
	
	AnimaVertex(const AnimaVertex& src)
	{
		_data = nullptr;
		_size = src._size;
		_engine = src._engine;
		
		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_CORE_ALIGN_OF(Type));
		
		memcpy(_data, src._data, sizeof(Type) * _size);
	}
	
	AnimaVertex(AnimaVertex&& src)
	: _engine(src._engine)
	, _data(src._data)
	, _size(src._size)
	{
		src._data = nullptr;
		src._size = 0;
	}
	
	~AnimaVertex()
	{
		ANIMA_ASSERT(_engine != nullptr);
		
		(_engine->GetGenericAllocator())->Deallocate(_data);
		_data = nullptr;
	}
	
	AnimaVertex& operator=(const AnimaVertex& src)
	{
		if (this != &src)
		{
			if (_data != nullptr)
			{
				(_engine->GetGenericAllocator())->Deallocate(_data);
				_data = nullptr;
			}
			
			_size = src._size;
			_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_CORE_ALIGN_OF(Type));
			
			memcpy(_data, src._data, sizeof(Type) * _size);
		}
		
		return *this;
	}
	
	AnimaVertex& operator=(AnimaVertex&& src)
	{
		if (this != &src)
		{
			_engine = src._engine;
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
		return const_cast<Type&>(_data[index]);
	}
	
protected:
	AnimaEngine* _engine;
	
	Type*	_data;
	ASizeT	_size;
};

typedef AnimaVertex<double, 2> AnimaVertex2d;
typedef AnimaVertex<double, 3> AnimaVertex3d;
typedef AnimaVertex<double, 4> AnimaVertex4d;

typedef AnimaVertex<float, 2> AnimaVertex2f;
typedef AnimaVertex<float, 3> AnimaVertex3f;
typedef AnimaVertex<float, 4> AnimaVertex4f;

typedef AnimaVertex<char, 3> AnimaColor3b;
typedef AnimaVertex<char, 4> AnimaColor4b;

typedef AnimaVertex<float, 3> AnimaColor3f;
typedef AnimaVertex<float, 4> AnimaColor4f;

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaVertex__) */
