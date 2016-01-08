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
#include "AnimaString.h"
#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class TYPE>
class ANIMA_ENGINE_EXPORT AnimaObjMappedArray
{
public:
	AnimaObjMappedArray()
	{
	}

	template<typename... Args>
	AnimaObjMappedArray(const AnimaObjMappedArray& src, Args... args)
		: _array(src._array, args...)
	{
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
		_array = src._array;
		_namesMap = src._namesMap;
	}

public:
	inline AInt GetSize() const
	{
		return _array.size();
	}

	template<typename... Args>
	void RemoveAll(Args... args)
	{
		_array.clear();
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
		if (index >= 0 && index < _array.size())
			return _array[index];
		else
			return TYPE();
	}

	inline TYPE GetConst(const AnimaString& name) const
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.size())
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

		AInt newIndex = _array.size();
		_array.push_back(newElement/*, args...*/);

		ANIMA_ASSERT(newIndex >= 0);
		_namesMap[name] = newIndex;

		return newIndex;
	}

	template<typename... Args>
	bool Remove(AInt index, Args...args)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.size());
		AnimaString elementToRemoveName = GetObjectName(_array[index]);
		return Remove(elementToRemoveName, args...);
	}

	template<typename... Args>
	bool Remove(const AnimaString& name, Args...args)
	{
		auto pair = _namesMap.find(name);
		if (name == _namesMap.end())
			return false;

		_array.erase(_array.begin() + pair->second);
		_namesMap.erase(pair);

		RebuildMap();
	}
		
	inline void Set(AInt index, TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.size());
		AnimaString newElementName = GetObjectName(newElement);
		AnimaString oldElementName = GetObjectName(_array[index]);

		ANIMA_ASSERT(newElementName == oldElementName);
		_array[index] = newElement;
	}

	inline void Set(const AnimaString& name, TYPE newElement)
	{
		AInt index = Contains(name);
		Set(index, newElement);
	}

	//AInt Contains(TYPE element)
	//{
	//	return _array.Contains(element);
	//}

	AInt Contains(const AnimaString& name) const
	{
		auto pair = _namesMap.find(name);
		if (pair == _namesMap.end())
			return -1;
		return pair->second;
	}

	void RebuildMap()
	{
		_namesMap.clear();

		AInt arraySize = _array.size();
		for (AInt i = 0; i < arraySize; i++)
			_namesMap[GetObjectName(_array[i])] = i;
	}
	
protected:
	AnimaString GetObjectName(TYPE obj)
	{
		return obj.GetName();
	}

protected:
	AnimaArray<TYPE>	_array;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AInt, AnimaStringHasher> _namesMap;
#pragma warning (default: 4251) 
};


template<class TYPE>
class ANIMA_ENGINE_EXPORT AnimaObjPtrMappedArray
{
public:
	AnimaObjPtrMappedArray()
	{
	}

	AnimaObjPtrMappedArray(const AnimaObjPtrMappedArray& src)
		: _array(src._array)
	{
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
		_array = src._array;
		_namesMap = src._namesMap;
	}

public:
	inline AInt GetSize() const
	{
		return _array.size();
	}

	void RemoveAll()
	{
		_array.clear();
		_namesMap.clear();
	}

	inline TYPE Get(AInt index)
	{
		if (index >= 0 && index < _array.size())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetConst(AInt index) const
	{
		if (index >= 0 && index < _array.size())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetWithName(const AnimaString& name)
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.size())
			return _array[index];
		else
			return nullptr;
	}

	inline TYPE GetConstWithName(const AnimaString& name) const
	{
		AInt index = Contains(name);
		if (index >= 0 && index < _array.size())
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

		AInt newIndex = _array.size();
		_array.push_back(newElement);

		ANIMA_ASSERT(newIndex >= 0);
		_namesMap[name] = newIndex;

		return newIndex;
	}

	bool Remove(AInt index)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.size());
		AnimaString elementToRemoveName = GetObjectName(_array[index]);
		return Remove(elementToRemoveName);
	}

	bool Remove(const AnimaString& name)
	{
		auto pair = _namesMap.find(name);
		if (pair == _namesMap.end())
			return false;

		_array.erase(_array.begin() + pair->second);
		_namesMap.erase(pair);

		RebuildMap();
		return true;
	}

	inline void Set(AInt index, TYPE newElement)
	{
		ANIMA_ASSERT(index >= 0 && index < _array.size());
		AnimaString newElementName = GetObjectName(newElement);
		AnimaString oldElementName = GetObjectName(_array[index]);

		ANIMA_ASSERT(newElementName == oldElementName);
		_array[index] = newElement;
	}

	inline void Set(const AnimaString& name, TYPE newElement)
	{
		AInt index = Contains(name);
		Set(index, newElement);
	}

	AInt Contains(TYPE element)
	{
		AInt count = _array.size();
		for (AInt i = 0; i < count; i++)
		{
			if (_array[i] == element)
				return i;
		}

		return -1;
	}

	AInt Contains(const AnimaString& name) const
	{
		auto pair = _namesMap.find(name);
		if (pair == _namesMap.end())
			return -1;
		return pair->second;
	}

	void RebuildMap()
	{
		_namesMap.clear();

		AInt arraySize = _array.size();
		for (AInt i = 0; i < arraySize; i++)
			_namesMap[GetObjectName(_array[i])] = i;
	}

protected:
	AnimaString GetObjectName(TYPE obj)
	{
		return obj->GetName();
	}

protected:
	AnimaArray<TYPE>	_array;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AInt, AnimaStringHasher> _namesMap;
#pragma warning (default: 4251) 
};

#ifndef SWIG
template<class TYPE>
using AnimaMappedArray = typename std::conditional<std::is_pointer<TYPE>::value, AnimaObjPtrMappedArray<TYPE>, AnimaObjMappedArray<TYPE> >::type;
#endif

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMappedArray__) */
