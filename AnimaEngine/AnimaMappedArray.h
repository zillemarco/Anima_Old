//
//  AnimaMappedArray.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMappedArray__
#define __Anima__AnimaMappedArray__

#include "AnimaEngine.h"
#include "AnimaTypes.h"
#include "AnimaArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class TYPE>
class ANIMA_ENGINE_EXPORT AnimaObjMappedArray
{
public:
	AnimaObjMappedArray(AnimaAllocator* allocator)
		: _array(allocator)
	{
		_allocator = allocator;
	}

	template<typename... Args>
	AnimaObjMappedArray(const AnimaObjMappedArray& src, Args... args)
		: _array(src._array, args...)
	{
		_allocator = src._allocator;
	}

	virtual ~AnimaObjMappedArray()
	{
	}

	AnimaObjMappedArray& operator=(const AnimaObjMappedArray& src)
	{
		Copy(src);
		return *this;
	}
	
	inline TYPE operator[](AInt index) const
	{
		return GetConst(index);
	}

	inline TYPE& operator[](AInt index)
	{
		return Get(index);
	}

	inline TYPE operator[](const char* name) const
	{
		return GetConst(Contains(name));
	}

	inline TYPE& operator[](const char* name)
	{
		return Get(Contains(name));
	}

	inline TYPE operator[](const AnimaString& name) const
	{
		return GetConst(Contains(name));
	}

	inline TYPE& operator[](const AnimaString& name)
	{
		return Get(Contains(name));
	}

	template<typename... Args>
	void Copy(const AnimaObjMappedArray& src, Args... args)
	{
		_array.Copy(src._array, args...);
		_namesMap = src._namesMap;
	}

public:
	inline AInt GetSize() const
	{
		return _array.GetSize();
	}

	template<typename... Args>
	void RemoveAll(Args... args)
	{
		_array.RemoveAll(1, args...);
	}

	inline TYPE& Get(AInt index)
	{
		return _array[index];
	}

	inline TYPE GetConst(AInt index) const
	{
		return _array[index];
	}
	
	inline TYPE& Get(const AnimaString& name)
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return TYPE();
	}

	inline TYPE GetConst(const AnimaString& name) const
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return TYPE();
	}
	
	inline TYPE& Get(const char* name)
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return TYPE();
	}

	inline TYPE GetConst(const char* name) const
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return TYPE();
	}

	inline AnimaArray<TYPE>* GetArray()
	{
		return &_array;
	}

	template<typename... Args>
	AInt Add(const AnimaString& name, TYPE newElement, Args... args)
	{
		return Add(name, newElement, false, args...);
	}

	template<typename... Args>
	AInt Add(const AnimaString& name, TYPE newElement, bool returnExisting, Args... args)
	{
		AInt existingIndex = Contains(name);
		if (existingIndex > -1)
		{
			if (returnExisting)
				return existingIndex;
			return -1;
		}

		AInt newIndex = _array.Add(newElement, args...);

		ANIMA_ASSERT(newIndex >= 0);
		_namesMap[name] = newIndex;

		return newIndex;
	}

	template<typename... Args>
	AInt Add(const char* name, TYPE newElement, bool returnExisting, Args... args)
	{
		AnimaString str(name, _allocator);
		return Add(str, newElement, returnExisting, args...);
	}

	template<typename... Args>
	AInt Add(const char* name, TYPE newElement, Args... args)
	{
		AnimaString str(name, _allocator);
		return Add(str, newElement, false, args...);
	}

	template<typename... Args>
	bool Remove(AInt index, Args...args)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.GetSize());
		AnimaString elementToRemoveName = GetName(_array[index]);
		return Remove(elementToRemoveName, args...);
	}

	template<typename... Args>
	bool Remove(const AnimaString& name, Args...args)
	{
		auto pair = _namesMap.find(name);
		if (name == _namesMap.end())
			return false;

		_array.RemoveAt(pair->second, args...);
		_namesMap.erase(pair);

		RebuildMap();
	}

	template<typename... Args>
	bool Remove(const char* name, Args...args)
	{
		AnimaString str(name, _allocator);
		return Remove(str, args...);
	}
	
	inline void Set(AInt index, TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.GetSize());
		AnimaString newElementName = GetName(newElement);
		AnimaString oldElementName = GetName(_array[index]);

		ANIMA_ASSERT(newElementName == oldElementName);
		_array[index] = newElement;
	}

	inline void Set(const AnimaString& name, TYPE newElement)
	{
		AInt index = Contains(name);
		Set(index, newElement);
	}

	inline void Set(const char* name, TYPE newElement)
	{
		AnimaString str(name, _allocator);
		Set(str, newElement);
	}

	AInt Contains(TYPE element)
	{
		return _array.Contains(element);
	}

	AInt Contains(const AnimaString& name) const
	{
		auto pair = _namesMap.find(name);
		if (pair == _namesMap.end())
			return -1;
		return pair->second;
	}

	AInt Contains(const char* name) const
	{
		AnimaString str(name, _allocator);
		return Contains(str);
	}

	void RebuildMap()
	{
		_namesMap.clear();

		AInt arraySize = _array.GetSize();
		for (AInt i = 0; i < arraySize; i++)
			_namesMap[GetName(_array[i])] = i;
	}
	
protected:
	AnimaString GetName(TYPE obj)
	{
		return obj.GetAnimaName();
	}

protected:
	AnimaAllocator*		_allocator;
	AnimaArray<TYPE>	_array;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AInt, AnimaString::Hasher> _namesMap;
#pragma warning (default: 4251) 
};


template<class TYPE>
class ANIMA_ENGINE_EXPORT AnimaObjPtrMappedArray
{
public:
	AnimaObjPtrMappedArray(AnimaAllocator* allocator)
		: _array(allocator)
	{
		_allocator = allocator;
	}

	AnimaObjPtrMappedArray(const AnimaObjPtrMappedArray& src)
		: _array(src._array)
	{
		_allocator = src._allocator;
	}

	virtual ~AnimaObjPtrMappedArray()
	{
	}

	AnimaObjPtrMappedArray& operator=(const AnimaObjPtrMappedArray& src)
	{
		Copy(src);
		return *this;
	}

	inline TYPE operator[](AInt index) const
	{
		return GetConst(index);
	}

	inline TYPE operator[](AInt index)
	{
		return Get(index);
	}

	inline TYPE operator[](const char* name) const
	{
		AInt index = Contains(name);
		if(index >= 0)
			return GetConst(index);
		return nullptr;
	}

	inline TYPE operator[](const char* name)
	{
		AInt index = Contains(name);
		if(index >= 0)
			return Get(index);
		return nullptr;
	}

	inline TYPE operator[](const AnimaString& name) const
	{
		AInt index = Contains(name);
		if(index >= 0)
			return GetConst(index);
		return nullptr;
	}

	inline TYPE operator[](const AnimaString& name)
	{
		AInt index = Contains(name);
		if(index >= 0)
			return Get(index);
		return nullptr;
	}

	void Copy(const AnimaObjPtrMappedArray& src)
	{
		_array.Copy(src._array);
		_namesMap = src._namesMap;
	}

public:
	inline AInt GetSize() const
	{
		return _array.GetSize();
	}

	void RemoveAll()
	{
		_array.RemoveAll(1);
		_namesMap.clear();
	}

	inline TYPE Get(AInt index)
	{
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetConst(AInt index) const
	{
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetWithName(const AnimaString& name)
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetConstWithName(const AnimaString& name) const
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetWithName(const char* name)
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetConstWithName(const char* name) const
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.GetSize())
			return _array[index];
		else
			return nullptr;
	}

	inline AnimaArray<TYPE>* GetArray()
	{
		return &_array;
	}

	AInt Add(const AnimaString& name, TYPE newElement)
	{
		return Add(name, newElement, false);
	}

	AInt Add(const AnimaString& name, TYPE newElement, bool returnExisting)
	{
		AInt existingIndex = Contains(name);
		if (existingIndex > -1)
		{
			if (returnExisting)
				return existingIndex;
			return -1;
		}

		AInt newIndex = _array.Add(newElement);

		ANIMA_ASSERT(newIndex >= 0);
		_namesMap[name] = newIndex;

		return newIndex;
	}

	AInt Add(const char* name, TYPE newElement)
	{
		AnimaString str(name, _allocator);
		return Add(str, newElement, false);
	}

	AInt Add(const char* name, TYPE newElement, bool returnExisting)
	{
		AnimaString str(name, _allocator);
		return Add(str, newElement, returnExisting);
	}

	bool Remove(AInt index)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.GetSize());
		AnimaString elementToRemoveName = GetName(_array[index]);
		return Remove(elementToRemoveName);
	}

	bool Remove(const AnimaString& name)
	{
		auto pair = _namesMap.find(name);
		if (pair == _namesMap.end())
			return false;

		_array.RemoveAt(pair->second);
		_namesMap.erase(pair);

		RebuildMap();
		return true;
	}

	bool Remove(const char* name)
	{
		AnimaString str(name, _allocator);
		return Remove(str);
	}

	inline void Set(AInt index, TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.GetSize());
		AnimaString newElementName = GetName(newElement);
		AnimaString oldElementName = GetName(_array[index]);

		ANIMA_ASSERT(newElementName == oldElementName);
		_array[index] = newElement;
	}

	inline void Set(const AnimaString& name, TYPE newElement)
	{
		AInt index = Contains(name);
		Set(index, newElement);
	}

	inline void Set(const char* name, TYPE newElement)
	{
		AnimaString str(name, _allocator);
		Set(str, newElement);
	}

	AInt Contains(TYPE element)
	{
		return _array.Contains(element);
	}

	AInt Contains(const AnimaString& name) const
	{
		auto pair = _namesMap.find(name);
		if (pair == _namesMap.end())
			return -1;
		return pair->second;
	}

	AInt Contains(const char* name) const
	{
		AnimaString str(name, _allocator);
		return Contains(str);
	}

	void RebuildMap()
	{
		_namesMap.clear();

		AInt arraySize = _array.GetSize();
		for (AInt i = 0; i < arraySize; i++)
			_namesMap[GetName(_array[i])] = i;
	}

protected:
	AnimaString GetName(TYPE obj)
	{
		return obj->GetAnimaName();
	}

protected:
	AnimaAllocator*		_allocator;
	AnimaArray<TYPE>	_array;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AInt, AnimaString::Hasher> _namesMap;
#pragma warning (default: 4251) 
};

#ifndef SWIG
template<class TYPE>
using AnimaMappedArray = typename std::conditional<std::is_pointer<TYPE>::value, AnimaObjPtrMappedArray<TYPE>, AnimaObjMappedArray<TYPE> >::type;
#endif

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMappedArray__) */
