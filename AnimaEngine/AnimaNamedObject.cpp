//
//  AnimaNamedObject.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaNamedObject.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaNamedObject::AnimaNamedObject(const AnimaString& name, AnimaAllocator* allocator)
	: _allocator(allocator)
{
	_name = name;
}

AnimaNamedObject::AnimaNamedObject(const AnimaNamedObject& src)
	: _allocator(src._allocator)
	, _name(src._name)
{
}

AnimaNamedObject::AnimaNamedObject(AnimaNamedObject&& src)
	: _allocator(src._allocator)
	, _name(src._name)
{
}

AnimaNamedObject::~AnimaNamedObject()
{
}

AnimaNamedObject& AnimaNamedObject::operator=(const AnimaNamedObject& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
	}

	return *this;
}

AnimaNamedObject& AnimaNamedObject::operator=(AnimaNamedObject&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
	}

	return *this;
}

void AnimaNamedObject::SetName(const AnimaString& name)
{
	_name = name;
}

AnimaString AnimaNamedObject::GetName() const
{
	return _name;
}

END_ANIMA_ENGINE_NAMESPACE
