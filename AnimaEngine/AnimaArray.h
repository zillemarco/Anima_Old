//
//  AnimaArray.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaArray__
#define __Anima__AnimaArray__

#include "AnimaEngine.h"
#include <type_traits>
#include <typeinfo>

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class TYPE>
class ANIMA_ENGINE_EXPORT AnimaObjArray
{
public:
	AnimaObjArray(AnimaAllocator* allocator)
	{
		_data = nullptr;
		_size = 0;
		_maxSize = 0;
		_growBy = 0;
		_allocator = allocator;
	}

	template<typename... Args>
	AnimaObjArray(const AnimaObjArray& src, Args... args)
	{
		_allocator = src._allocator;
		Copy(src, args...);
	}

	virtual ~AnimaObjArray()
	{
		if (_data != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
			_data = nullptr;
		}
	}

	AnimaObjArray& operator=(const AnimaObjArray& src)
	{
		Copy(src);
		return *this;
	}

	inline TYPE operator[](AInt index) const
	{
		return GetAt(index);
	}

	inline TYPE& operator[](AInt index)
	{
		return ElementAt(index);
	}

public:
	inline AInt GetSize() const
	{
		return _size;
	}

	inline AInt GetUpperBound() const
	{
		return _size - 1;
	}

	template<typename... Args>
	void SetSize(AInt newSize, Args...args)
	{
		SetSize(newSize, -1, args...);
	}

	template<typename... Args>
	void SetSize(AInt newSize, AInt growBy, Args...args)
	{
		ANIMA_ASSERT(newSize >= 0);

		if (growBy != -1)
			_growBy = growBy;

		if (newSize == 0)
		{
			if (_data != nullptr)
			{
				AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
				_data = nullptr;
			}

			_size = 0;
			_maxSize = 0;
		}
		else if (_data == nullptr)
		{
			ANIMA_ASSERT(newSize <= SIZE_MAX / sizeof(TYPE));

			_data = AnimaAllocatorNamespace::AllocateArray<TYPE>(*_allocator, newSize, args...);

			_size = newSize;
			_maxSize = newSize;
		}
		else if (newSize <= _maxSize)
		{
			_size = newSize;
		}
		else
		{
			int growBy = _growBy;
			if (growBy == 0)
			{
				growBy = _size / 8;
				growBy = (growBy < 4) ? 4 : ((growBy > 1024) ? 1024 : growBy);
			}

			int newMax;
			if (newSize < _maxSize + growBy)
				newMax = _maxSize + growBy;
			else
				newMax = newSize;

			ANIMA_ASSERT(newMax >= _maxSize);
			ANIMA_ASSERT(newMax <= SIZE_MAX / sizeof(TYPE));

			TYPE* newData = AnimaAllocatorNamespace::AllocateArray<TYPE>(*_allocator, newMax, args...);

			memcpy(newData, _data, _maxSize * sizeof(TYPE));

			ANIMA_ASSERT(newSize > _size);
			
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
			_data = newData;
			_size = newSize;
			_maxSize = newMax;
		}
	}

	void FreeExtra()
	{
		if (_size != _maxSize)
		{
			ANIMA_ASSERT(_size <= SIZE_MAX / sizeof(TYPE));
			TYPE* newData = nullptr;

			if (_size != 0)
			{
				newData = AnimaAllocatorNamespace::AllocateArray<TYPE>(*_allocator, _size);

				memcpy(newData, _data, _size * sizeof(TYPE));
			}

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
			_data = newData;
			_maxSize = _size;
		}
	}

	template<typename... Args>
	void RemoveAll(AInt growBy, Args... args)
	{
		SetSize(0, growBy, args...);
	}

	template<typename... Args>
	void RemoveAll(Args... args)
	{
		RemoveAll(1, args...);
	}

	inline TYPE GetAt(AInt index) const
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return _data[index];
	}

	inline void SetAt(AInt index, TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		_data[index] = newElement;
	}

	inline TYPE& ElementAt(AInt index)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return _data[index];
	}

	template<typename... Args>
	void SetAtGrow(AInt index, TYPE newElement, AInt growBy, Args... args)
	{
		ANIMA_ASSERT(index >= 0);

		if (index >= _size)
			SetSize(index + 1, growBy, args...);
		_data[index] = newElement;
	}

	template<typename... Args>
	AInt Add(TYPE newElement, AInt growBy, Args... args)
	{
		AInt index = _size;
		SetAtGrow(index, newElement, growBy, args...);
		return index;
	}

	template<typename... Args>
	AInt Add(TYPE newElement, Args... args)
	{
		return Add(newElement, 1, args...);
	}

	template<typename... Args>
	AInt Append(const AnimaObjArray& src, Args... args)
	{
		ANIMA_ASSERT(this != &src);

		AInt oldSize = _size;
		InsertAt(oldSize, &src, args...);

		return oldSize;
	}

	template<typename... Args>
	void Copy(const AnimaObjArray& src, Args... args)
	{
		ANIMA_ASSERT(this != &src);

		_growBy = src._growBy;

		SetSize(1, _growBy, args...);
		InsertAt(0, &src, args...);
		RemoveAt(_size - 1, 1, args...);
	}

	template<typename... Args>
	void InsertAt(AInt index, TYPE newElement, AInt count, AInt growBy, Args... args)
	{
		ANIMA_ASSERT(index >= 0);
		ANIMA_ASSERT(count > 0);

		if (index >= _size)
			SetSize(index + count, growBy, args...);
		else
		{
			int oldSize = _size;
			SetSize(_size + count, growBy, args...);

			TYPE* newData = AnimaAllocatorNamespace::AllocateArray<TYPE>(*_allocator, count);
			memcpy(newData, &_data[oldSize], count * sizeof(TYPE));

			memmove(&_data[index + count], &_data[index], (oldSize - index) * sizeof(TYPE));

			memcpy(&_data[index], newData, count * sizeof(TYPE));

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, newData);
		}

		ANIMA_ASSERT(index + count <= _size);
		while (count--)
			_data[index++] = newElement;
	}

	template<typename... Args>
	void InsertAt(AInt index, TYPE newElement, Args... args)
	{
		InsertAt(index, newElement, 1, 1, args...);
	}

	template<typename... Args>
	void InsertAt(AInt index, TYPE newElement, AInt count, Args... args)
	{
		InsertAt(index, newElement, count, 1, args...);
	}

	template<typename... Args>
	void InsertAt(AInt index, const AnimaObjArray* newArray, Args... args)
	{
		ANIMA_ASSERT(index >= 0);

		if (newArray->GetSize() > 0)
		{
			InsertAt(index, newArray->GetAt(0), newArray->GetSize(), args...);
			for (int i = 0; i < newArray->GetSize(); i++)
				SetAt(index + i, newArray->GetAt(i));
		}
	}

	template<typename... Args>
	void RemoveAt(AInt index, Args...args)
	{
		RemoveAt(index, 1, args...);
	}

	template<typename... Args>
	void RemoveAt(AInt index, AInt count, Args...args)
	{
		ANIMA_ASSERT(index >= 0);
		ANIMA_ASSERT(count >= 1);
		ANIMA_ASSERT(index + count <= _size);

		int moveCount = _size - (index + count);

		for (int i = 0; i < count; i++)
			_data[index + i] = TYPE(args...);

		if (moveCount)
		{
			TYPE* newData = AnimaAllocatorNamespace::AllocateArray<TYPE>(*_allocator, count);
			memcpy(newData, &_data[index], count * sizeof(TYPE));

			memmove(&_data[index], &_data[index + count], moveCount * sizeof(TYPE));

			memcpy(&_data[index + moveCount], newData, count * sizeof(TYPE));

			if (newData)
			{
				AnimaAllocatorNamespace::DeallocateArray(*_allocator, newData);
				newData = nullptr;
			}
		}

		_size -= count;
	}

	AInt Contains(TYPE element)
	{
		if (_data == nullptr)
		{
			ANIMA_ASSERT(_size == 0);
			ANIMA_ASSERT(_maxSize == 0);
		}
		else
		{
			for (AInt i = 0; i < _size; i++)
			{
				if (_data[i] == element)
					return i;
			}
		}

		return -1;
	}

protected:
	TYPE*	_data;
	AInt	_size;
	AInt	_maxSize;
	AInt	_growBy;

	AnimaAllocator* _allocator;
};

template<class TYPE>
class ANIMA_ENGINE_EXPORT AnimaObjPtrArray
{
public:
	AnimaObjPtrArray(AnimaAllocator* allocator)
	{
		_data = nullptr;
		_size = 0;
		_maxSize = 0;
		_growBy = 0;
		_allocator = allocator;
	}

	AnimaObjPtrArray(const AnimaObjPtrArray& src)
	{
		_allocator = src._allocator;
		Copy(src);
	}

	virtual ~AnimaObjPtrArray()
	{
		if (_data != nullptr)
		{
			for (AInt i = 0; i < _maxSize; i++)
			{
				if (_data[i] != nullptr)
				{
					AnimaAllocatorNamespace::DeallocateObject(*_allocator, _data[i]);
					_data[i] = nullptr;
				}
			}

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
			_data = nullptr;
		}
	}

	AnimaObjPtrArray& operator=(const AnimaObjPtrArray& src)
	{
		Copy(src);
		return *this;
	}

	inline TYPE operator[](AInt index) const
	{
		return GetAt(index);
	}

	inline TYPE& operator[](AInt index)
	{
		return ElementAt(index);
	}

public:
	inline AInt GetSize() const
	{
		return _size;
	}

	inline AInt GetUpperBound() const
	{
		return _size - 1;
	}
	
	void SetSize(AInt newSize)
	{
		SetSize(newSize, -1);
	}
	
	void SetSize(AInt newSize, AInt growBy)
	{
		ANIMA_ASSERT(newSize >= 0);

		if (growBy != -1)
			_growBy = growBy;

		if (newSize == 0)
		{
			if (_data != nullptr)
			{
				for (AInt i = 0; i < _maxSize; i++)
				{
					if (_data[i] != nullptr)
					{
						AnimaAllocatorNamespace::DeallocateObject(*_allocator, _data[i]);
						_data[i] = nullptr;
					}
				}

				AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
				_data = nullptr;
			}

			_size = 0;
			_maxSize = 0;
		}
		else if (_data == nullptr)
		{
			ANIMA_ASSERT(newSize <= SIZE_MAX / sizeof(TYPE));

			_data = AnimaAllocatorNamespace::AllocateArray<TYPE*>(*_allocator, newSize);

			for (AInt i = 0; i < newSize; i++)
				_data[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator);

			_size = newSize;
			_maxSize = newSize;
		}
		else if (newSize <= _maxSize)
		{
			if (newSize > _size)
			{
				for (AInt i = _size; i < newSize; i++)
				{
					if (_data[i] != nullptr)
					{
						AnimaAllocatorNamespace::DeallocateObject(*_allocator, _data[i]);
						_data[i] = nullptr;
					}

					_data[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator);
				}
			}
			else if (_size > newSize)
			{
				for (AInt i = newSize; i < _size; i++)
				{
					if (_data[i] != nullptr)
					{
						AnimaAllocatorNamespace::DeallocateObject(*_allocator, _data[i]);
						_data[i] = nullptr;
					}

					_data[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator);
				}
			}

			_size = newSize;
		}
		else
		{
			int growBy = _growBy;
			if (growBy == 0)
			{
				growBy = _size / 8;
				growBy = (growBy < 4) ? 4 : ((growBy > 1024) ? 1024 : growBy);
			}

			int newMax;
			if (newSize < _maxSize + growBy)
				newMax = _maxSize + growBy;
			else
				newMax = newSize;

			ANIMA_ASSERT(newMax >= _maxSize);
			ANIMA_ASSERT(newMax <= SIZE_MAX / sizeof(TYPE));

			TYPE** newData = AnimaAllocatorNamespace::AllocateArray<TYPE*>(*_allocator, newMax);

			memcpy(newData, _data, _maxSize * sizeof(TYPE*));

			ANIMA_ASSERT(newSize > _size);

			for (AInt i = _maxSize; i < newMax; i++)
				newData[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator);

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
			_data = newData;
			_size = newSize;
			_maxSize = newMax;
		}
	}

	void FreeExtra()
	{
		if (_size != _maxSize)
		{
			ANIMA_ASSERT(_size <= SIZE_MAX / sizeof(TYPE));
			TYPE** newData = nullptr;

			if (_size != 0)
			{
				newData = AnimaAllocatorNamespace::AllocateArray<TYPE*>(*_allocator, _size);

				memcpy(newData, _data, _size * sizeof(TYPE*));

				for (AInt i = _size; i < _maxSize; i++)
				{
					if (_data[i] != nullptr)
					{
						delete _data[i];
						_data[i] = nullptr;
					}
				}
			}

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _data);
			_data = newData;
			_maxSize = _size;
		}
	}

	void RemoveAll(AInt growBy)
	{
		SetSize(0, growBy);
	}
	
	void RemoveAll()
	{
		RemoveAll(1);
	}

	inline TYPE GetAt(AInt index) const
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return *_data[index];
	}

	inline void SetAt(AInt index, TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		(*_data[index]) = newElement;
	}

	inline TYPE& ElementAt(AInt index)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return *_data[index];
	}

	void SetAtGrow(AInt index, TYPE newElement, AInt growBy)
	{
		ANIMA_ASSERT(index >= 0);

		if (index >= _size)
			SetSize(index + 1, growBy);
		*(_data[index]) = newElement;
	}

	AInt Add(TYPE newElement, AInt growBy)
	{
		AInt index = _size;
		SetAtGrow(index, newElement, growBy);
		return index;
	}
	
	AInt Add(TYPE newElement)
	{
		return Add(newElement, 1);
	}

	AInt Append(const AnimaObjPtrArray& src)
	{
		ANIMA_ASSERT(this != &src);

		AInt oldSize = _size;
		InsertAt(oldSize, &src);

		return oldSize;
	}

	void Copy(const AnimaObjPtrArray& src)
	{
		ANIMA_ASSERT(this != &src);

		_growBy = src._growBy;

		SetSize(1, _growBy);
		InsertAt(0, &src);
		RemoveAt(_size - 1, 1);
	}

	void InsertAt(AInt index, TYPE newElement, AInt count, AInt growBy)
	{
		ANIMA_ASSERT(index >= 0);
		ANIMA_ASSERT(count > 0);

		if (index >= _size)
			SetSize(index + count, growBy);
		else
		{
			int oldSize = _size;
			SetSize(_size + count, growBy);

			TYPE** newData = AnimaAllocatorNamespace::AllocateArray<TYPE*>(*_allocator, count);
			memcpy(newData, &_data[oldSize], count * sizeof(TYPE*));

			memmove(&_data[index + count], &_data[index], (oldSize - index) * sizeof(TYPE*));

			memcpy(&_data[index], newData, count * sizeof(TYPE*));

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, newData);
		}

		ANIMA_ASSERT(index + count <= _size);
		while (count--)
			*(_data[index++]) = newElement;
	}
	
	void InsertAt(AInt index, TYPE newElement)
	{
		InsertAt(index, newElement, 1, 1);
	}
	
	void InsertAt(AInt index, TYPE newElement, AInt count)
	{
		InsertAt(index, newElement, count, 1);
	}
	
	void InsertAt(AInt index, const AnimaObjPtrArray* newArray)
	{
		ANIMA_ASSERT(index >= 0);

		if (newArray->GetSize() > 0)
		{
			InsertAt(index, newArray->GetAt(0), newArray->GetSize());
			for (int i = 0; i < newArray->GetSize(); i++)
				SetAt(index + i, newArray->GetAt(i));
		}
	}
	
	void RemoveAt(AInt index)
	{
		RemoveAt(index, 1);
	}
	
	void RemoveAt(AInt index, AInt count)
	{
		ANIMA_ASSERT(index >= 0);
		ANIMA_ASSERT(count >= 1);
		ANIMA_ASSERT(index + count <= _size);

		int moveCount = _size - (index + count);

		for (int i = 0; i < count; i++)
		{
			if (_data[index + i] != nullptr)
			{
				AnimaAllocatorNamespace::DeallocateObject(*_allocator, _data[index + i]);
				_data[index + i] = nullptr;
			}

			_data[index + i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator);
		}

		if (moveCount)
		{
			TYPE** newData = AnimaAllocatorNamespace::AllocateArray<TYPE*>(*_allocator, count);
			memcpy(newData, &_data[index], count * sizeof(TYPE*));

			memmove(&_data[index], &_data[index + count], moveCount * sizeof(TYPE*));

			memcpy(&_data[index + moveCount], newData, count * sizeof(TYPE*));

			if (newData)
			{
				AnimaAllocatorNamespace::DeallocateArray(*_allocator, newData);
				newData = nullptr;
			}
		}

		_size -= count;
	}

	bool Contains(TYPE element)
	{
		if (_data == nullptr)
		{
			ANIMA_ASSERT(_size == 0);
			ANIMA_ASSERT(_maxSize == 0);
		}
		else
		{
			for (int i = 0; i < _size; i++)
			{
				if (*(_data[i]) == element)
					return true;
			}
		}

		return false;
	}

protected:
	TYPE**	_data;
	AInt	_size;
	AInt	_maxSize;
	AInt	_growBy;

	AnimaAllocator* _allocator;
};

#ifndef SWIG
template<class TYPE>
using AnimaArray = typename std::conditional<std::is_pointer<TYPE>::value, AnimaObjPtrArray<TYPE>, AnimaObjArray<TYPE> >::type;
#endif

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaArray__) */
