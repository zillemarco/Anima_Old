//
//  AnimaTypeMappedArray.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTypeMappedArray__
#define __Anima__AnimaTypeMappedArray__

#include "AnimaEngine.h"
#include "AnimaMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

template<class BASE_TYPE>
class ANIMA_ENGINE_EXPORT AnimaTypeMappedArray
{
public:
	AnimaTypeMappedArray(AnimaAllocator* allocator)
	{
		_allocator = allocator;
	}

	AnimaTypeMappedArray(const AnimaTypeMappedArray& src)
	{
		_mappedArrays = src._mappedArrays;
		_mappedNames = src._mappedNames;
		_allocator = src._allocator;
	}

	virtual ~AnimaTypeMappedArray()
	{
	}

	AnimaTypeMappedArray& operator=(const AnimaTypeMappedArray& src)
	{
		_allocator = src._allocator;
		_mappedArrays = src._mappedArrays;
		_mappedNames = src._mappedNames;

		return *this;
	}

public:
	template<class TYPE> void Add(const AnimaString& name, TYPE newElement)
	{
		if (_mappedNames.find(name) != _mappedNames.end())
			return;

		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		AnimaMappedArray<BASE_TYPE>* mappedArray = nullptr;

		if (pair != _mappedArrays.end())
			mappedArray = pair->second;
		else
		{
			mappedArray = AnimaAllocatorNamespace::AllocateNew<AnimaMappedArray<BASE_TYPE> >(*_allocator, _allocator);
			_mappedArrays[type] = mappedArray;
		}

		int index = mappedArray->Contains(name);
		if (index >= 0)
			return;

		mappedArray->Add(name, newElement);
		_mappedNames[name] = mappedArray;
	}

	template<class TYPE> void Add(const char* name, TYPE newElement)
	{
		AnimaString str(name, _allocator);
		Add(str, newElement);
	}

	BASE_TYPE GetWithIndex(AInt index) const
	{
		AInt base = 0;
		for (auto mappedArray : _mappedArrays)
		{
			AInt top = base + mappedArray.second->GetSize();

			if (index >= base && index < top)
				return mappedArray.second->Get(index - base);

			base = top;
		}
		return nullptr;
	}

	BASE_TYPE GetWithName(const AnimaString& name) const
	{
		auto namesPair = _mappedNames.find(name);
		if (namesPair != _mappedNames.end())
		{
			AnimaMappedArray<BASE_TYPE>* mappedArray = namesPair->second;
			return mappedArray->Get(name);
		}

		return nullptr;
	}

	BASE_TYPE GetWithName(const char* name) const
	{
		AnimaString str(name, _allocator);
		return GetWithName(str);
	}

	template<class TYPE> TYPE GetWithNameAndType(AInt index) const
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			return pair->Get(index);
		return nullptr;
	}

	template<class TYPE> TYPE GetWithNameAndType(const AnimaString& name) const
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			return pair->second->Get(name);
		return nullptr;
	}

	template<class TYPE> TYPE GetWithNameAndType(const char* name) const
	{
		AnimaString str(name, _allocator);
		return GetWithNameAndType(str);
	}

	template<class TYPE> void SetElementOfType(AInt index, TYPE newElement)
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			pair->second->Set(index, newElement);
	}

	template<class TYPE> void SetElementOfType(const AnimaString& name, TYPE newElement)
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			pair->second->Set(name, newElement);
	}
	
	template<class TYPE> void SetElementOfType(const char* name, TYPE newElement)
	{
		AnimaString str(name, _allocator);
		return SetElementOfType(str, newElement);
	}

	template<class TYPE> void RemoveFromType(AInt index)
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			pair->second->Remove(index);
	}

	template<class TYPE> void RemoveFromType(const AnimaString& name)
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			pair->second->Remove(name);
	}

	template<class TYPE> void RemoveFromType(const char* name)
	{
		AnimaString str(name, _allocator);
		return RemoveFromType(str);
	}

	void RemoveAll()
	{
		for (auto pair : _mappedArrays)
		{
			pair.second->RemoveAll();
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
			pair.second = nullptr;
		}

		for (auto pair : _mappedNames)
			pair.second = nullptr;

		_mappedArrays.clear();
		_mappedNames.clear();
	}

	template<class TYPE> AnimaMappedArray<TYPE>* GetMappedArrayOfType() const
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			return pair->second;
		return nullptr;
	}

	template<class TYPE> AnimaArray<BASE_TYPE>* GetMappedArrayArrayOfType() const
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			return pair->second->GetArray();
		return nullptr;
	}

	boost::unordered_map<AnimaString, AnimaMappedArray<BASE_TYPE>*, AnimaString::Hasher>* GetArraysMap()
	{
		return &_mappedArrays;
	}

	BASE_TYPE Contains(const AnimaString& name)
	{
		auto pair = _mappedNames.find(name);
		if (pair != _mappedNames.end())
			return pair->second->Get(name);
		return nullptr;
	}

	BASE_TYPE Contains(const char* name)
	{
		AnimaString str(name, _allocator);
		return Contains(str);
	}

	AInt GetTotalSize() const
	{
		return (AInt)_mappedNames.size();
	}

	template<class TYPE> AInt GetSizeOfType() const
	{
		AnimaString type(typeid(TYPE).name(), _allocator);
		auto pair = _mappedArrays.find(type);

		if (pair != _mappedArrays.end())
			return pair->second->GetSize();
		return 0;
	}

protected:
	AnimaAllocator* _allocator;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaMappedArray<BASE_TYPE>*, AnimaString::Hasher> _mappedArrays;
	boost::unordered_map<AnimaString, AnimaMappedArray<BASE_TYPE>*, AnimaString::Hasher> _mappedNames;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTypeMappedArray__) */
