//
//  AnimaVectorGenerator.cpp
//  Anima
//
//  Created zy Marco Zille on 26/11/14.
//
//

#include "AnimaVectorGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = 0.0f;
	_vector[1] = 0.0f;
	_vector[2] = 0.0f;
	_vector[3] = 0.0f;
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine, const AnimaVertex2f& vector)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = vector[0];
	_vector[1] = vector[1];
	_vector[2] = 0.0f;
	_vector[3] = 0.0f;
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine, AFloat x, AFloat y)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = 0.0f;
	_vector[3] = 0.0f;
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine, const AnimaVertex3f& vector)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = vector[0];
	_vector[1] = vector[1];
	_vector[2] = vector[2];
	_vector[3] = 0.0f;
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine, AFloat x, AFloat y, AFloat z)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = z;
	_vector[3] = 0.0f;
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine, const AnimaVertex4f& vector)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = vector[0];
	_vector[1] = vector[1];
	_vector[2] = vector[2];
	_vector[3] = vector[3];
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w)
	: AnimaDataGenerator(engine)
	, _vector(engine)
{
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = z;
	_vector[3] = w;
}

AnimaVectorGenerator::AnimaVectorGenerator(const AnimaVectorGenerator& src)
	: AnimaDataGenerator(src._engine)
	, _vector(src._vector)
{
}

AnimaVectorGenerator::AnimaVectorGenerator(AnimaVectorGenerator&& src)
	: AnimaDataGenerator(src._engine)
	, _vector(src._vector)
{
}

AnimaVectorGenerator::~AnimaVectorGenerator()
{
}

AnimaVectorGenerator& AnimaVectorGenerator::operator=(const AnimaVectorGenerator& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_vector = src._vector;
	}

	return *this;
}

AnimaVectorGenerator& AnimaVectorGenerator::operator=(AnimaVectorGenerator&& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_vector = src._vector;
	}

	return *this;
}

void AnimaVectorGenerator::UpdateValue()
{
}

void AnimaVectorGenerator::SetVector(const AnimaVertex2f& vector)
{
	_vector[0] = vector[0];
	_vector[1] = vector[1];
	_vector[2] = 0.0f;
	_vector[3] = 0.0f;
}

void AnimaVectorGenerator::SetVector(AFloat x, AFloat y)
{
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = 0.0f;
	_vector[3] = 0.0f;
}

void AnimaVectorGenerator::SetVector(const AnimaVertex3f& vector)
{
	_vector[0] = vector[0];
	_vector[1] = vector[1];
	_vector[2] = vector[2];
	_vector[3] = 0.0f;
}

void AnimaVectorGenerator::SetVector(AFloat x, AFloat y, AFloat z)
{
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = z;
	_vector[3] = 0.0f;
}

void AnimaVectorGenerator::SetVector(const AnimaVertex4f& vector)
{
	_vector[0] = vector[0];
	_vector[1] = vector[1];
	_vector[2] = vector[2];
	_vector[3] = vector[3];
}

void AnimaVectorGenerator::SetVector(AFloat x, AFloat y, AFloat z, AFloat w)
{
	_vector[0] = x;
	_vector[1] = y;
	_vector[2] = z;
	_vector[3] = w;
}

AnimaVertex2f AnimaVectorGenerator::GetVector2f()
{
	AnimaVertex2f c(_engine);
	c[0] = _vector[0];
	c[1] = _vector[1];

	return c;
}

AnimaVertex3f AnimaVectorGenerator::GetVector3f()
{
	AnimaVertex3f c(_engine);
	c[0] = _vector[0];
	c[1] = _vector[1];
	c[2] = _vector[2];

	return c;
}

AnimaVertex4f AnimaVectorGenerator::GetVector4f()
{
	return _vector;
}

END_ANIMA_ENGINE_NAMESPACE
