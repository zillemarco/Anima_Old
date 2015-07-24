//
//  AnimaDataGenerator.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaDataGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaDataGenerator::AnimaDataGenerator(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaDataGenerator);
	_canUpdateValue = true;
}

AnimaDataGenerator::AnimaDataGenerator(const AnimaDataGenerator& src)
	: AnimaNamedObject(src)
{
	_canUpdateValue = src._canUpdateValue;
}

AnimaDataGenerator::AnimaDataGenerator(AnimaDataGenerator&& src)
	: AnimaNamedObject(src)
{
	_canUpdateValue = src._canUpdateValue;
}

AnimaDataGenerator::~AnimaDataGenerator()
{
}

AnimaDataGenerator& AnimaDataGenerator::operator=(const AnimaDataGenerator& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_canUpdateValue = src._canUpdateValue;
	}

	return *this;
}

AnimaDataGenerator& AnimaDataGenerator::operator=(AnimaDataGenerator&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_canUpdateValue = src._canUpdateValue;
	}

	return *this;
}

void AnimaDataGenerator::SetColor(const AnimaColor4f& color)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetColor(AFloat r, AFloat g, AFloat b, AFloat a)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetColor(const AnimaColor3f& color)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetColor(AFloat r, AFloat g, AFloat b)
{
	ANIMA_ASSERT(false);
}

AnimaColor3f AnimaDataGenerator::GetColor3f()
{
	ANIMA_ASSERT(false);
	return AnimaColor3f();
}

AnimaColor4f AnimaDataGenerator::GetColor4f()
{
	ANIMA_ASSERT(false);
	return AnimaColor4f();
}

void AnimaDataGenerator::SetVector(const AnimaVertex2f& color)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetVector(AFloat x, AFloat y)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetVector(const AnimaVertex3f& color)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetVector(AFloat x, AFloat y, AFloat z)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetVector(const AnimaVertex4f& color)
{
	ANIMA_ASSERT(false);
}

void AnimaDataGenerator::SetVector(AFloat x, AFloat y, AFloat z, AFloat w)
{
	ANIMA_ASSERT(false);
}

AnimaVertex2f AnimaDataGenerator::GetVector2f()
{
	ANIMA_ASSERT(false);
	return AnimaVertex2f();
}

AnimaVertex3f AnimaDataGenerator::GetVector3f()
{
	ANIMA_ASSERT(false);
	return AnimaVertex3f();
}

AnimaVertex4f AnimaDataGenerator::GetVector4f()
{
	ANIMA_ASSERT(false);
	return AnimaVertex4f();
}

void AnimaDataGenerator::SetTexture(AnimaTexture* texture)
{
	ANIMA_ASSERT(false);
}

AnimaTexture* AnimaDataGenerator::GetTexture()
{
	ANIMA_ASSERT(false);
	return nullptr;
}

void AnimaDataGenerator::StopValueUpdate()
{
	_canUpdateValue = false;
}

void AnimaDataGenerator::StartValueUpdate()
{
	_canUpdateValue = false;
}

bool AnimaDataGenerator::CanUpdateValue()
{
	return _canUpdateValue;
}

END_ANIMA_ENGINE_NAMESPACE
