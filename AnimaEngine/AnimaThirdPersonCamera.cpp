//
//  AnimaThirdPersonCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaThirdPersonCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaThirdPersonCamera::AnimaThirdPersonCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	: AnimaCamera(engine, camerasManager)
	, _target(engine)
{
	LookAt(0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

AnimaThirdPersonCamera::AnimaThirdPersonCamera(AnimaEngine* engine)
	: AnimaThirdPersonCamera(engine, nullptr)
{
}

AnimaThirdPersonCamera::AnimaThirdPersonCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& target)
	: AnimaCamera(engine, camerasManager)
	, _target(engine)
{
	LookAt(position, target);
}

AnimaThirdPersonCamera::AnimaThirdPersonCamera(const AnimaThirdPersonCamera& src)
	: AnimaCamera(src)
	, _target(src._target)
{
}

AnimaThirdPersonCamera::AnimaThirdPersonCamera(AnimaThirdPersonCamera&& src)
	: AnimaCamera(src)
	, _target(src._target)
{
}

AnimaThirdPersonCamera::~AnimaThirdPersonCamera()
{
}

AnimaThirdPersonCamera& AnimaThirdPersonCamera::operator=(const AnimaThirdPersonCamera& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_target = src._target;
	}

	return *this;
}

AnimaThirdPersonCamera& AnimaThirdPersonCamera::operator=(AnimaThirdPersonCamera&& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_target = src._target;
	}

	return *this;
}

void AnimaThirdPersonCamera::Zoom(AFloat amount)
{
	_position -= _target;

	if (amount < 0)
		_position += _position * 0.1f;

	if (amount > 0 && _position.Length() > 0.05f)
		_position -= _position * 0.1f;

	_position += _target;

	LookAt(_position, _target);
}

void AnimaThirdPersonCamera::Move(const AnimaVertex3f& direction, AFloat amount)
{
	AnimaVertex3f dir = direction;
	AnimaVertex3f Up = _worldYAxis;
	AnimaVertex3f Right = _xAxis;
	AnimaVertex3f Forward = Up ^ Right;

	dir.Normalize();

	Up *= amount;
	Right *= amount;
	Forward *= amount;

	AnimaVertex3f movement(_engine);

	if (dir[2] > 0.0f) movement += Forward;
	if (dir[2] < 0.0f) movement -= Forward;
	if (dir[0] > 0.0f) movement -= Right;
	if (dir[0] < 0.0f) movement += Right;
	if (dir[1] > 0.0f) movement += Up;
	if (dir[1] < 0.0f) movement -= Up;
	
	_position += movement;
	_target += movement;

	LookAt(_position, _target);
}

void AnimaThirdPersonCamera::Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount)
{
	AnimaVertex3f dir(_engine);
	dir[0] = xDirection;
	dir[1] = yDirection;
	dir[2] = zDirection;

	Move(dir, amount);
}

void AnimaThirdPersonCamera::RotateX(AFloat angle)
{
	_position -= _target;

	AnimaMath::RotateVector(_yAxis, angle, _xAxis);
	AnimaMath::RotateVector(_zAxis, angle, _xAxis);

	if (_yAxis[1] < 0.0f)
	{
		_zAxis[0] = 0.0f;
		_zAxis[1] = _zAxis[1] > 0.0f ? 1.0f : 0.0f;
		_zAxis[2] = 0.0f;

		_yAxis = _zAxis ^ _xAxis;
	}

	if (_zAxis.Length() != 0.0f)
	{
		float dist = _position.Length();
		_position = _target + _zAxis * dist;
	}

	CalculateViewMatrix();
}

void AnimaThirdPersonCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaThirdPersonCamera::RotateY(AFloat angle)
{
	_position -= _target;

	AnimaMath::RotateVector(_xAxis, angle, _worldYAxis);
	AnimaMath::RotateVector(_yAxis, angle, _worldYAxis);
	AnimaMath::RotateVector(_zAxis, angle, _worldYAxis);

	float dist = _position.Length();
	_position = _target + _zAxis * dist;

	CalculateViewMatrix();
}

void AnimaThirdPersonCamera::RotateYDeg(AFloat angle)
{
	RotateY(angle * (float)M_PI / 180.0f);
}

AFloat AnimaThirdPersonCamera::GetDistance()
{
	AnimaVertex3f direction = _position - _target;
	return direction.Length();
}

void AnimaThirdPersonCamera::SetDistance(AFloat dist)
{
	if (dist <= 0.0f)
		return;

	AnimaVertex3f dir = GetForward();
	dir.Inverse();
	dir.Normalize();
	dir *= dist;

	_position = _target + dir;

	LookAt(_position, _target);
}

void AnimaThirdPersonCamera::LookAt(const AnimaVertex3f& position, const AnimaVertex3f& target)
{
	_position = position;
	_target = target;

	_zAxis = _position - _target;
	_zAxis.Normalize();

	_xAxis = _worldYAxis ^ _zAxis;
	_xAxis.Normalize();

	_yAxis = _zAxis ^ _xAxis;
	_yAxis.Normalize();

	CalculateViewMatrix();
}

void AnimaThirdPersonCamera::LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xTarget, AFloat yTarget, AFloat zTarget)
{
	AnimaVertex3f position(_engine);
	AnimaVertex3f target(_engine);

	position[0] = xPosition;
	position[1] = yPosition;
	position[2] = zPosition;

	target[0] = xTarget;
	target[1] = yTarget;
	target[2] = zTarget;

	LookAt(position, target);
}

void AnimaThirdPersonCamera::CalculateViewMatrix()
{
	_viewMatrix[0] = _xAxis[0];								_viewMatrix[1] = _yAxis[0];								_viewMatrix[2] = _zAxis[0];								_viewMatrix[3] = 0.0f;
	_viewMatrix[4] = _xAxis[1];								_viewMatrix[5] = _yAxis[1];								_viewMatrix[6] = _zAxis[1];								_viewMatrix[7] = 0.0f;
	_viewMatrix[8] = _xAxis[2];								_viewMatrix[9] = _yAxis[2];								_viewMatrix[10] = _zAxis[2];							_viewMatrix[11] = 0.0f;
	_viewMatrix[12] = -AnimaMath::Dot(_xAxis, _position);	_viewMatrix[13] = -AnimaMath::Dot(_yAxis, _position);	_viewMatrix[14] = -AnimaMath::Dot(_zAxis, _position);	_viewMatrix[15] = 1.0f;
}

END_ANIMA_ENGINE_NAMESPACE
