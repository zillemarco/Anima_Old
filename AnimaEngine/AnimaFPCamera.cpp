//
//  AnimaFPCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFPCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFPCamera::AnimaFPCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	: AnimaCamera(engine, camerasManager)
	, _forward(engine)
{
	_forward[0] = 0.0f;
	_forward[1] = 0.0f;
	_forward[2] = 1.0f;
}

AnimaFPCamera::AnimaFPCamera(AnimaEngine* engine)
	: AnimaCamera(engine, nullptr)
	, _forward(engine)
{
	_forward[0] = 0.0f;
	_forward[1] = 0.0f;
	_forward[2] = 1.0f;
}

AnimaFPCamera::AnimaFPCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up)
	: AnimaCamera(engine, camerasManager, position, up)
	, _forward(forward)
{
	_forward.Normalize();
}

AnimaFPCamera::AnimaFPCamera(const AnimaFPCamera& src)
	: AnimaCamera(src)
	, _forward(src._forward)
{
	_forward.Normalize();
}

AnimaFPCamera::AnimaFPCamera(AnimaFPCamera&& src)
	: AnimaCamera(src)
	, _forward(src._forward)
{
	_forward.Normalize();
}

AnimaFPCamera::~AnimaFPCamera()
{
}

AnimaFPCamera& AnimaFPCamera::operator=(const AnimaFPCamera& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_forward = src._forward;
		_forward.Normalize();
	}

	return *this;
}

AnimaFPCamera& AnimaFPCamera::operator=(AnimaFPCamera&& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_forward = src._forward;
		_forward.Normalize();
	}

	return *this;
}

AnimaVertex3f AnimaFPCamera::GetForward()
{
	return _forward;
}

AnimaVertex3f AnimaFPCamera::GetLeft()
{
	AnimaVertex3f left = _up ^ _forward;
	left.Normalize();
	return left;
}

AnimaVertex3f AnimaFPCamera::GetRight()
{
	AnimaVertex3f right = _forward ^ _up;
	right.Normalize();
	return right;
}

void AnimaFPCamera::Zoom(AFloat amount)
{
	Move(_forward, amount);
}

void AnimaFPCamera::Move(const AnimaVertex3f& direction, AFloat amount)
{
	_position += direction * amount;
}

void AnimaFPCamera::Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount)
{
	_position[0] += xDirection * amount;
	_position[1] += yDirection * amount;
	_position[2] += zDirection * amount;
}

void AnimaFPCamera::RotateX(AFloat angle)
{
	AnimaVertex3f hAxis = _yAxis ^ _forward;
	hAxis.Normalize();
	
	AnimaMath::RotateVector(_forward, angle, hAxis);
	_forward.Normalize();

	AnimaVertex3f newUp = _forward ^ hAxis;
	newUp.Normalize();
	_up = newUp;
}

void AnimaFPCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaFPCamera::RotateY(AFloat angle)
{
	AnimaVertex3f hAxis = _yAxis ^ _forward;
	hAxis.Normalize();
	
	AnimaMath::RotateVector(_forward, angle, _yAxis);
	_forward.Normalize();

	AnimaVertex3f newUp = _forward ^ hAxis;
	newUp.Normalize();
	_up = newUp;
}

void AnimaFPCamera::RotateYDeg(AFloat angle)
{
	RotateY(angle * (float)M_PI / 180.0f);
}

AnimaMatrix AnimaFPCamera::GetViewMatrix()
{
	AnimaVertex3f side = _forward ^ _up;
	side.Normalize();
	AnimaVertex3f upVector = side ^ _forward;
	upVector.Normalize();

	AnimaMatrix m(_engine);
	m.SetData(side[0], 0);		m.SetData(side[1], 1);		m.SetData(side[2], 2);			m.SetData(0.0f, 3);
	m.SetData(upVector[0], 4);	m.SetData(upVector[1], 5);	m.SetData(upVector[2], 6);		m.SetData(0.0f, 7);
	m.SetData(-_forward[0], 8);	m.SetData(-_forward[1], 9);	m.SetData(-_forward[2], 10);	m.SetData(0.0f, 11);
	m.SetData(0.0f, 12);		m.SetData(0.0f, 13);		m.SetData(0.0f, 14);			m.SetData(1.0f, 15);

	m.Translate(-_position[0], -_position[1], -_position[2]);

	return m;
}

END_ANIMA_ENGINE_NAMESPACE
