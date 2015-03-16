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

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class TYPE, class ARG_TYPE>
class ANIMA_ENGINE_EXPORT AnimaArray
{
public:
	AnimaArray(AnimaAllocator* allocator)
	{
		_data = nullptr;
		_size = 0;
		_maxSize = 0;
		_growBy = 0;
		_allocator = allocator;
	}

	AnimaArray(const AnimaArray& src)
	{
		Copy(src);
	}

	virtual ~AnimaArray()
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

	AnimaArray& operator=(const AnimaArray& src)
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
	void SetSize(AInt newSize, AInt growBy = -1, Args&&... args)
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
				_data[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator, std::forward<Args>(args)...);

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

					_data[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator, std::forward<Args>(args)...);
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

					_data[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator, std::forward<Args>(args)...);
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
				newData[i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator, std::forward<Args>(args)...);

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

	template<typename... Args>
	void RemoveAll(Args&&... args)
	{
		SetSize(0, -1, std::forward<Args>(args)...);
	}

	inline TYPE GetAt(AInt index) const
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return *_data[index];
	}

	inline void SetAt(AInt index, ARG_TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		(*_data[index]) = newElement;
	}

	inline TYPE& ElementAt(AInt index)
	{
		ANIMA_ASSERT(index >= 0 && index < _size);
		return *_data[index];
	}

	template<typename... Args>
	void SetAtGrow(AInt index, ARG_TYPE newElement, Args&&... args)
	{
		ANIMA_ASSERT(index >= 0);

		if (index >= _size)
			SetSize(index + 1, -1, std::forward<Args>(args)...);
		*(_data[index]) = newElement;
	}

	template<typename... Args>
	AInt Add(ARG_TYPE newElement, Args&&... args)
	{
		AInt index = _size;
		SetAtGrow(index, newElement, std::forward<Args>(args)...);
		return index;
	}

	template<typename... Args>
	AInt Append(const AnimaArray& src, Args&&... args)
	{
		ANIMA_ASSERT(this != &src);

		AInt oldSize = _size;
		InsertAt(oldSize, &src, std::forward<Args>(args)...);

		return oldSize;
	}

	template<typename... Args>
	void Copy(const AnimaArray& src, Args&&... args)
	{
		ANIMA_ASSERT(this != &src);
		SetSize(1, -1, std::forward<Args>(args)...);
		InsertAt(0, &src, std::forward<Args>(args)...);
		RemoveAt(_size - 1, std::forward<Args>(args)...);
	}

	template<typename... Args>
	void InsertAt(AInt index, ARG_TYPE newElement, AInt count = 1, Args&&... args)
	{
		ANIMA_ASSERT(index >= 0);
		ANIMA_ASSERT(count > 0);

		if (index >= _size)
			SetSize(index + count, -1, std::forward<Args>(args)...);
		else
		{
			int oldSize = _size;
			SetSize(_size + count, -1, std::forward<Args>(args)...);

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

	template<typename... Args>
	void InserAt(AInt index, AnimaArray* newArray, Args&&... args)
	{
		ANIMA_ASSERT(idex >= 0);

		if (newArray->GetSize() > 0)
		{
			InsertAt(idnex, newArray->GetAt(0), newArray->GetSize());
			for (int i = 0; i < newArray->GetSize(); i++)
				SetAt(index + i, newArray->GetAt(i));
		}
	}

	template<typename... Args>
	void RemoveAt(AInt index, AInt count = 1, Args&&... args)
	{
		ASSERT(index >= 0);
		ASSERT(count >= 0);
		ASSERT(index + count <= _size);

		int moveCount = _size - (index + count);

		for (int i = 0; i < count; i++)
		{
			if (_data[index + i] != nullptr)
			{
				AnimaAllocatorNamespace::DeallocateObject(*_allocator, m_pData[nIndex + i]);
				m_pData[nIndex + i] = nullptr;
			}

			_data[nIndex + i] = AnimaAllocatorNamespace::AllocateNew<TYPE>(*_allocator, std::forward<Args>(args)...);
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

	bool Contains(ARG_TYPE element)
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

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaArray__) */
