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
	IMPLEMENT_ANIMA_CLASS(AnimaNamedObject);

	_name = name;
}

AnimaNamedObject::AnimaNamedObject(const AnimaNamedObject& src)
	: _allocator(src._allocator)
	, _name(src._name)
	, _className(src._className)
	, _derivedClassNames(src._derivedClassNames)
{
}

AnimaNamedObject::AnimaNamedObject(AnimaNamedObject&& src)
	: _allocator(src._allocator)
	, _name(src._name)
	, _className(src._className)
	, _derivedClassNames(src._derivedClassNames)
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
		_className = src._className;
		_derivedClassNames = src._derivedClassNames;
	}

	return *this;
}

AnimaNamedObject& AnimaNamedObject::operator=(AnimaNamedObject&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
		_className = src._className;
		_derivedClassNames = src._derivedClassNames;
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

void AnimaNamedObject::_AddDerivedClassName(AnimaString derivedClassName)
{
	_derivedClassNames.push_back(derivedClassName);
}

void AnimaNamedObject::_SetClassName(AnimaString className)
{
	_className = className;
}

bool AnimaNamedObject::IsOfClass(AnimaString className)
{
	if (_className == className)
		return true;

	for (auto val : _derivedClassNames)
	{
		if (val == className)
			return true;
	}

	return false;
}

END_ANIMA_ENGINE_NAMESPACE
