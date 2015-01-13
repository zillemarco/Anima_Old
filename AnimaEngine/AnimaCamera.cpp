//
//  AnimaCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaCamera.h"
#include "AnimaMath.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define INIT_Y_AXIS _yAxis[0] = 0.0f;	\
					_yAxis[1] = 1.0f;	\
					_yAxis[2] = 0.0f;

AnimaCamera::AnimaCamera(AnimaEngine* engine)
	: _position(engine)
	, _up(engine)
	, _yAxis(engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_position[0] = 0.0f;
	_position[1] = 0.0f;
	_position[2] = 5.0f;

	_up[0] = 0.0f;
	_up[1] = 1.0f;
	_up[2] = 0.0f;

	INIT_Y_AXIS
}

AnimaCamera::AnimaCamera(AnimaEngine* engine, const AnimaVertex3f& position, const AnimaVertex3f& up)
	: _position(position)
	, _up(up)
	, _yAxis(engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_position.Normalize();
	_up.Normalize();

	INIT_Y_AXIS
}

AnimaCamera::AnimaCamera(const AnimaCamera& src)
	: _position(src._position)
	, _up(src._up)
	, _yAxis(src._engine)
{
	_engine = src._engine;

	_position.Normalize();
	_up.Normalize();

	INIT_Y_AXIS
}

AnimaCamera::AnimaCamera(AnimaCamera&& src)
	: _position(src._position)
	, _up(src._up)
	, _engine(src._engine)
	, _yAxis(src._engine)
{
	_position.Normalize();
	_up.Normalize();

	INIT_Y_AXIS
}

AnimaCamera::~AnimaCamera()
{
}

AnimaCamera& AnimaCamera::operator=(const AnimaCamera& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_position = src._position;
		_up = src._up;

		_position.Normalize();
		_up.Normalize();

		INIT_Y_AXIS
	}

	return *this;
}

AnimaCamera& AnimaCamera::operator=(AnimaCamera&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_position = src._position;
		_up = src._up;

		_position.Normalize();
		_up.Normalize();

		INIT_Y_AXIS
	}

	return *this;
}

AnimaVertex3f AnimaCamera::GetPosition()
{
	return _position;
}

void AnimaCamera::SetPosition(const AnimaVertex3f& position)
{
	_position = position;
}

void AnimaCamera::SetPosition(const AFloat& x, const AFloat& y, const AFloat& z)
{
	_position[0] = x;
	_position[1] = y;
	_position[2] = z;
}

AnimaVertex3f AnimaCamera::GetUp()
{
	return _up;
}

END_ANIMA_ENGINE_NAMESPACE
