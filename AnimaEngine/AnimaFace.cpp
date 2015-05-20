//
//  AnimaFace.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFace.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFace::AnimaFace()
{
	SetIndexes(0, 0, 0);
	_normal = AnimaVertex3f(0.0f, 1.0f, 0.0f);
}

AnimaFace::AnimaFace(AUint indexes[3])
{
	SetIndexes(indexes);
	_normal = AnimaVertex3f(0.0f, 1.0f, 0.0f);
}

AnimaFace::AnimaFace(AUint a, AUint b, AUint c)
{
	SetIndexes(a, b, c);
	_normal = AnimaVertex3f(0.0f, 1.0f, 0.0f);
}

AnimaFace::AnimaFace(const AnimaFace& src)
{
	SetIndexes(src._indexes[0], src._indexes[1], src._indexes[2]);
	SetNormal(src._normal);
}

AnimaFace::AnimaFace(AnimaFace&& src)
{
	SetIndexes(src._indexes[0], src._indexes[1], src._indexes[2]);
	SetNormal(src._normal);
}

AnimaFace::~AnimaFace()
{
	SetIndexes(0, 0, 0);
	_normal = AnimaVertex3f(0.0f, 1.0f, 0.0f);
}

AnimaFace& AnimaFace::operator=(const AnimaFace& src)
{
	if (this != &src)
	{
		SetIndexes(src._indexes[0], src._indexes[1], src._indexes[2]);
		SetNormal(src._normal);
	}
	
	return *this;
}

AnimaFace& AnimaFace::operator=(AnimaFace&& src)
{
	if (this != &src)
	{
		SetIndexes(src._indexes);
		SetNormal(src._normal);
	}
	
	return *this;
}

void AnimaFace::SetIndexes(AUint a, AUint b, AUint c)
{
	_indexes[0] = a;
	_indexes[1] = b;
	_indexes[2] = c;
}

void AnimaFace::SetIndexes(AUint indexes[3])
{
	SetIndexes(indexes[0], indexes[1], indexes[2]);
}

AInt AnimaFace::GetIndexesCount()
{
	return 3;
}

AUint* AnimaFace::GetIndexes()
{
	return _indexes;
}

AUint AnimaFace::GetIndex(AInt index) const
{
	ANIMA_ASSERT(index >= 0 && index < 3);
	return _indexes[index];
}

const AUint* AnimaFace::GetConstIndexes()
{
	return const_cast<AUint*>(_indexes);
}

bool AnimaFace::GetIndexes(AUint* outIndexes, AInt& outIndexesSize)
{
	if (outIndexesSize < 3)
		return false;
	outIndexes = _indexes;
	return true;
}

bool AnimaFace::GetConstIndexes(AUint* outIndexes, AInt& outIndexesSize)
{
	if (outIndexesSize < 3)
		return false;

	memcpy(outIndexes, _indexes, sizeof(AUint) * 3);
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
	for (ASizeT i = 0; i < 3; i++)
	{
		if (_indexes[i] == index)
			return true;
	}

	return false;
}

END_ANIMA_ENGINE_NAMESPACE
