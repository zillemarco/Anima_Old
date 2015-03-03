//
//  AnimaFace.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFace.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFace::AnimaFace(AnimaAllocator* allocator)
{
	ANIMA_ASSERT(allocator != nullptr);
	_allocator = allocator;
	
	_indexes = nullptr;
	_indexesNumber = 0;
	_normal = AnimaVertex3f(0.0f, 1.0f, 0.0f);
}

AnimaFace::AnimaFace(const AnimaFace& src)
{
	_allocator = src._allocator;
	_indexes = nullptr;
	_indexesNumber = 0;
	
	SetIndexes(src._indexes, src._indexesNumber);
	SetNormal(src._normal);
}

AnimaFace::AnimaFace(AnimaFace&& src)
: _indexes(src._indexes)
, _indexesNumber(src._indexesNumber)
, _allocator(src._allocator)
, _normal(src._normal)
{
	src._indexes = nullptr;
	src._indexesNumber = 0;
}

AnimaFace::~AnimaFace()
{
	ClearIndexes();
}

AnimaFace& AnimaFace::operator=(const AnimaFace& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		SetIndexes(src._indexes, src._indexesNumber);
		SetNormal(src._normal);
	}
	
	return *this;
}

AnimaFace& AnimaFace::operator=(AnimaFace&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		
		_indexes = src._indexes;
		_indexesNumber = src._indexesNumber;
		_normal = src._normal;

		src._indexes = nullptr;
		src._indexesNumber = 0;
	}
	
	return *this;
}

void AnimaFace::SetIndexes(AUint* indexes, ASizeT n)
{
	ANIMA_ASSERT(_allocator != nullptr);
	ClearIndexes();
	
	if(indexes != nullptr && n > 0)
	{
		_indexesNumber = n;
		_indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _indexesNumber);
		
		for (int i = 0; i < _indexesNumber; i++)
			_indexes[i] = indexes[i];
	}
}

void AnimaFace::AddIndex(const AUint& index)
{
	ANIMA_ASSERT(_allocator != nullptr);
	if(_indexesNumber > 0)
	{
		AUint* tmpOldIndexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _indexesNumber);
		
		for (int i = 0; i < _indexesNumber; i++)
			tmpOldIndexes[i] = _indexes[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _indexes);
		
		_indexesNumber++;
		_indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _indexesNumber);
		
		for (int i = 0; i < _indexesNumber - 1; i++)
			_indexes[i] = tmpOldIndexes[i];
		
		_indexes[_indexesNumber - 1] = index;
		
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, tmpOldIndexes);
	}
	else
	{
		_indexesNumber++;
		_indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _indexesNumber);
		
		_indexes[_indexesNumber - 1] = index;
	}
}

void AnimaFace::ClearIndexes()
{
	if(_indexes != nullptr && _indexesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _indexes);
		_indexes = nullptr;
		_indexesNumber = 0;
	}
}

ASizeT AnimaFace::GetIndexesCount()
{
	return _indexesNumber;
}

AUint* AnimaFace::GetIndexes()
{
	return _indexes;
}

AUint AnimaFace::GetIndex(ASizeT index) const
{
	ANIMA_ASSERT(index >= 0 && index < _indexesNumber);
	return _indexes[index];
}

const AUint* AnimaFace::GetConstIndexes()
{
	return const_cast<AUint*>(_indexes);
}

bool AnimaFace::GetIndexes(AUint* outIndexes, ASizeT& outIndexesSize)
{
	if (outIndexesSize < _indexesNumber)
		return false;
	outIndexes = _indexes;
	return true;
}

bool AnimaFace::GetConstIndexes(AUint* outIndexes, ASizeT& outIndexesSize)
{
	if (outIndexesSize < _indexesNumber)
		return false;

	memcpy(outIndexes, _indexes, sizeof(AUint) * _indexesNumber);
	return true;
}

void AnimaFace::SetNormal(const AnimaVertex3f& n)
{
	_normal = n;
}

void AnimaFace::SetNormal(const AFloat& x, const AFloat& y, const AFloat& z)
{
	_normal.x = x;
	_normal.y = y;
	_normal.z = z;
}

AnimaVertex3f AnimaFace::GetNormal() const
{
	return _normal;
}

bool AnimaFace::HasIndex(AUint index) const
{
	for (ASizeT i = 0; i < _indexesNumber; i++)
	{
		if (_indexes[i] == index)
			return true;
	}

	return false;
}

END_ANIMA_ENGINE_NAMESPACE
