//
//  AnimaColorGenerator.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaColorGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaColorGenerator::AnimaColorGenerator(AnimaEngine* engine)
	: AnimaDataGenerator(engine)
	, _color(engine)
{
	_color[0] = 0.0f;
	_color[1] = 0.0f;
	_color[2] = 0.0f;
}

AnimaColorGenerator::AnimaColorGenerator(AnimaEngine* engine, const AnimaColor3f& color)
	: AnimaDataGenerator(engine)
	, _color(engine)
{
	_color[0] = color[0];
	_color[1] = color[1];
	_color[2] = color[2];
}

AnimaColorGenerator::AnimaColorGenerator(AnimaEngine* engine, AFloat r, AFloat g, AFloat b)
	: AnimaDataGenerator(engine)
	, _color(engine)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
}

AnimaColorGenerator::AnimaColorGenerator(const AnimaColorGenerator& src)
	: AnimaDataGenerator(src._engine)
	, _color(src._color)
{
}

AnimaColorGenerator::AnimaColorGenerator(AnimaColorGenerator&& src)
	: AnimaDataGenerator(src._engine)
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

void AnimaColorGenerator::SetColor3f(const AnimaColor3f& color)
{
	_color[0] = color[0];
	_color[1] = color[1];
	_color[2] = color[2];
}

void AnimaColorGenerator::SetColor3f(AFloat r, AFloat g, AFloat b)
{
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
}

AnimaColor3f AnimaColorGenerator::GetColor3f()
{
	return _color;
}

END_ANIMA_ENGINE_NAMESPACE
