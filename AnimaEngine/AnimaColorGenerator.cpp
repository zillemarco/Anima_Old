//
//  AnimaColorGenerator.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaColorGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaColorGenerator::AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaDataGenerator(name, allocator)
	, _color(0.0f, 0.0f, 0.0f, 1.0f)
{
}

AnimaColorGenerator::AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaColor3f& color)
	: AnimaDataGenerator(name, allocator)
	, _color(color.r, color.g, color.b, 1.0f)
{
}

AnimaColorGenerator::AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat r, AFloat g, AFloat b)
	: AnimaDataGenerator(name, allocator)
	, _color(r, g, b, 1.0f)
{
}

AnimaColorGenerator::AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaColor4f& color)
	: AnimaDataGenerator(name, allocator)
	, _color(color)
{
}

AnimaColorGenerator::AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat r, AFloat g, AFloat b, AFloat a)
	: AnimaDataGenerator(name, allocator)
	, _color(r, g, b, a)
{
}

AnimaColorGenerator::AnimaColorGenerator(const AnimaColorGenerator& src)
	: AnimaDataGenerator(src)
	, _color(src._color)
{
}

AnimaColorGenerator::AnimaColorGenerator(AnimaColorGenerator&& src)
	: AnimaDataGenerator(src)
	, _color(src._color)
{
}

AnimaColorGenerator::~AnimaColorGenerator()
{
}

AnimaColorGenerator& AnimaColorGenerator::operator=(const AnimaColorGenerator& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_color = src._color;
	}

	return *this;
}

AnimaColorGenerator& AnimaColorGenerator::operator=(AnimaColorGenerator&& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_color = src._color;
	}

	return *this;
}

void AnimaColorGenerator::UpdateValue()
{
}

void AnimaColorGenerator::SetColor(const AnimaColor3f& color)
{
	_color.r = color.r;
	_color.g = color.g;
	_color.b = color.b;
	_color.a = 1.0f;
}

void AnimaColorGenerator::SetColor(AFloat r, AFloat g, AFloat b)
{
	_color.r = r;
	_color.g = g;
	_color.b = b;
	_color.a = 1.0f;
}

void AnimaColorGenerator::SetColor(const AnimaColor4f& color)
{
	_color = color;
}

void AnimaColorGenerator::SetColor(AFloat r, AFloat g, AFloat b, AFloat a)
{
	_color.r = r;
	_color.g = g;
	_color.b = b;
	_color.a = a;
}

AnimaColor3f AnimaColorGenerator::GetColor3f()
{
	AnimaColor3f c(_color.r, _color.g, _color.b);
	return c;
}

AnimaColor4f AnimaColorGenerator::GetColor4f()
{
	return _color;
}

END_ANIMA_ENGINE_NAMESPACE
