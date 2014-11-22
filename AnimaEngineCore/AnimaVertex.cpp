#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

template class AnimaVertex<double, 2>;
template class AnimaVertex<double, 3>;
template class AnimaVertex<double, 4>;

template class AnimaVertex<float, 2>;
template class AnimaVertex<float, 3>;
template class AnimaVertex<float, 4>;

template class AnimaVertex<char, 3>;
template class AnimaVertex<char, 4>;

template<class Type, ASizeT Size>
AnimaVertex<Type, Size>::AnimaVertex(AnimaAllocator* allocator)
{
	ANIMA_ASSERT(allocator != nullptr);

	_allocator = allocator;
	_size = Size;

	_data = (Type*)_allocator->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_CORE_ALIGN_OF(Type));
}

template<class Type, ASizeT Size>
AnimaVertex<Type, Size>::AnimaVertex(const AnimaVertex& src)
{
	_data = nullptr;
	_size = src._size;
	_allocator = src._allocator;

	_data = (Type*)_allocator->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_CORE_ALIGN_OF(Type));

	memcpy(_data, src._data, sizeof(Type) * _size);
}

template<class Type, ASizeT Size>
AnimaVertex<Type, Size>::AnimaVertex(AnimaVertex&& src)
	: _data(src._data)
	, _size(src._size)
{
	src._data = nullptr;
	src._size = 0;
}

template<class Type, ASizeT Size>
AnimaVertex<Type, Size>::~AnimaVertex()
{
	ANIMA_ASSERT(_allocator != nullptr);

	_allocator->Deallocate(_data);
	_data = nullptr;
}

template<class Type, ASizeT Size>
AnimaVertex<Type, Size>& AnimaVertex<Type, Size>::operator=(const AnimaVertex& src)
{
	if (this != &src)
	{
		if (_data != nullptr)
		{
			ANIMA_ASSERT(_allocator != nullptr);
			_allocator->Deallocate(_data);
			_data = nullptr;
		}

		_size = src._size;
		_data = (Type*)_allocator->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_CORE_ALIGN_OF(Type));

		memcpy(_data, src._data, sizeof(Type) * _size);
	}

	return *this;
}

template<class Type, ASizeT Size>
AnimaVertex<Type, Size>& AnimaVertex<Type, Size>::operator=(AnimaVertex&& src)
{
	if (this != &src)
	{
		_data = src._data;
		_size = src._size;

		src._data = nullptr;
		src._size = 0;
	}

	return *this;
}

template<class Type, ASizeT Size>
Type& AnimaVertex<Type, Size>::operator[](ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _size);
	return _data[index];
}

template<class Type, ASizeT Size>
const Type& AnimaVertex<Type, Size>::operator[](ASizeT index) const
{
	ANIMA_ASSERT(index >= 0 && index < _size);
	return const_cast<Type&>(_data[index]);
}

END_ANIMA_ENGINE_CORE_NAMESPACE