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

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class Type, ASizeT Size>
class ANIMA_ENGINE_EXPORT AnimaVertex
{
public:
	AnimaVertex(AnimaEngine* engine)
	{
		ANIMA_ASSERT(engine != nullptr);

		_engine = engine;
		_size = Size;

		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
		memset(_data, 0, sizeof(Type) * Size);
	}

	AnimaVertex(AnimaEngine* engine, Type data[Size])
	{
		ANIMA_ASSERT(engine != nullptr);

		_engine = engine;
		_size = Size;

		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
		memcpy(_data, data, sizeof(Type) * _size);
	}

	AnimaVertex(const AnimaVertex& src)
	{
		if (&src != this)
		{
			_data = nullptr;
			_size = src._size;
			_engine = src._engine;

			_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
			memcpy(_data, src._data, sizeof(Type) * _size);
		}
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

		if (_data != nullptr)
		{
			(_engine->GetGenericAllocator())->Deallocate(_data);
			_data = nullptr;
		}
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
			_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));

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
	
	inline operator Type*()
	{
		return _data;
	}

	inline operator const Type*()
	{
		return const_cast<Type*>(_data);
	}

	inline void SetData(Type* data[Size])
	{
		memcpy(_data, data, sizeof(Type) * _size);
	}

	inline Type* GetData() const
	{
		return _data;
	}

	inline void CopyData(Type* d) const
	{
		memcpy(d, _data, sizeof(Type) * _size);
	}

	inline const Type* GetConstData() const
	{
		return const_cast<Type*>(_data);
	}

	inline void Reverse()
	{
		for (int i = 0; i < _size / 2; i++)
		{
			Type tmp = _data[i];
			_data[i] = _data[_size - i - 1];
			_data[_size - i - 1] = tmp;
		}
	}

	void DumpMemory()
	{
		for (int i = 0; i < _size; i++)
			printf("%f\t", _data[i]);
		printf("\n");
	}

	AnimaEngine* GetEngine() const
	{
		return _engine;
	}

	Type Length()
	{
		Type rv = 0;
		for (int i = 0; i < _size; i++)
			rv += _data[i] * _data[i];

		return (Type)sqrt(rv);
	}

	void Normalize()
	{
		Type len = Length();
		for (int i = 0; i < _size; i++)
			_data[i] /= len;
	}

	ASizeT GetSize() const
	{
		return _size;
	}
		
protected:
	AnimaEngine* _engine;
	
	Type*	_data;
	ASizeT	_size;
};

typedef AnimaVertex<float, 2> AnimaVertex2f;
typedef AnimaVertex<float, 3> AnimaVertex3f;
typedef AnimaVertex<float, 4> AnimaVertex4f;

typedef AnimaVertex<char, 3> AnimaColor3b;
typedef AnimaVertex<char, 4> AnimaColor4b;

typedef AnimaVertex<float, 3> AnimaColor3f;
typedef AnimaVertex<float, 4> AnimaColor4f;

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaVertex__) */
