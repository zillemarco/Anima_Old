//
//  AnimaFPSCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFPSCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFPSCamera::AnimaFPSCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	: AnimaCamera(engine, camerasManager)
, _forward(engine)
{
	_forward[0] = 0.0f;
	_forward[1] = 0.0f;
	_forward[2] = 1.0f;
}

AnimaFPSCamera::AnimaFPSCamera(AnimaEngine* engine)
	: AnimaCamera(engine, nullptr)
	, _forward(engine)
{
	_forward[0] = 0.0f;
	_forward[1] = 0.0f;
	_forward[2] = 1.0f;
}

AnimaFPSCamera::AnimaFPSCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up)
	: AnimaCamera(engine, camerasManager, position, up)
	, _forward(forward)
{
	_forward.Normalize();
}

AnimaFPSCamera::AnimaFPSCamera(const AnimaFPSCamera& src)
	: AnimaCamera(src)
	, _forward(src._forward)
{
	_forward.Normalize();
}

AnimaFPSCamera::AnimaFPSCamera(AnimaFPSCamera&& src)
	: AnimaCamera(src)
	, _forward(src._forward)
{
	_forward.Normalize();
}

AnimaFPSCamera::~AnimaFPSCamera()
{
}

AnimaFPSCamera& AnimaFPSCamera::operator=(const AnimaFPSCamera& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_forward = src._forward;
		_forward.Normalize();
	}

	return *this;
}

AnimaFPSCamera& AnimaFPSCamera::operator=(AnimaFPSCamera&& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_forward = src._forward;
		_forward.Normalize();
	}

	return *this;
}

AnimaVertex3f AnimaFPSCamera::GetForward()
{
	return _forward;
}

AnimaVertex3f AnimaFPSCamera::GetLeft()
{
	AnimaVertex3f left = _up ^ _forward;
	left.Normalize();
	return left;
}

AnimaVertex3f AnimaFPSCamera::GetRight()
{
	AnimaVertex3f right = _forward ^ _up;
	right.Normalize();
	return right;
}

void AnimaFPSCamera::Move(const AnimaVertex3f& direction, AFloat amount)
{
	_position += direction * amount;
}

void AnimaFPSCamera::Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount)
{
	_position[0] += xDirection * amount;
	_position[1] += yDirection * amount;
	_position[2] += zDirection * amount;
}

void AnimaFPSCamera::RotateX(AFloat angle)
{
	AnimaVertex3f hAxis = _yAxis ^ _forward;
	hAxis.Normalize();
	
	AnimaMath::RotateVector(_forward, angle, hAxis);
	_forward.Normalize();

	AnimaVertex3f newUp = _forward ^ hAxis;
	newUp.Normalize();
	_up = newUp;
}

void AnimaFPSCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaFPSCamera::RotateY(AFloat angle)
{
	AnimaVertex3f hAxis = _yAxis ^ _forward;
	hAxis.Normalize();
	
	AnimaMath::RotateVector(_forward, angle, _yAxis);
	_forward.Normalize();

	AnimaVertex3f newUp = _forward ^ hAxis;
	newUp.Normalize();
	_up = newUp;
}

void AnimaFPSCamera::RotateYDeg(AFloat angle)
{
	RotateY(angle * (float)M_PI / 180.0f);
}

END_ANIMA_ENGINE_NAMESPACE
