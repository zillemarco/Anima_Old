//
//  AnimaDataGenerator.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaDataGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaDataGenerator::AnimaDataGenerator(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	_canUpdateValue = true;
}

AnimaDataGenerator::AnimaDataGenerator(const AnimaDataGenerator& src)
{
	_engine = src._engine;
	_canUpdateValue = src._canUpdateValue;
}

AnimaDataGenerator::AnimaDataGenerator(AnimaDataGenerator&& src)
{
	_engine = src._engine;
	_canUpdateValue = src._canUpdateValue;
}

AnimaDataGenerator::~AnimaDataGenerator()
{
}

AnimaDataGenerator& AnimaDataGenerator::operator=(const AnimaDataGenerator& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_canUpdateValue = src._canUpdateValue;
	}

	return *this;
}

AnimaDataGenerator& AnimaDataGenerator::operator=(AnimaDataGenerator&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
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
	return nullptr;
}

AnimaColor4f AnimaDataGenerator::GetColor4f()
{
	ANIMA_ASSERT(false);
	return nullptr;
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
	return nullptr;
}

AnimaVertex3f AnimaDataGenerator::GetVector3f()
{
	ANIMA_ASSERT(false);
	return nullptr;
}

AnimaVertex4f AnimaDataGenerator::GetVector4f()
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