//
//  AnimaABCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaABCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaABCamera::AnimaABCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	: AnimaCamera(engine, camerasManager)
	, _target(engine)
	, _viewMatrix(engine)
{
	_target[0] = 0.0f;
	_target[1] = 0.0f;
	_target[2] = 0.0f;
}

AnimaABCamera::AnimaABCamera(AnimaEngine* engine)
	: AnimaABCamera(engine, nullptr)
{
}

AnimaABCamera::AnimaABCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& target, const AnimaVertex3f& up)
	: AnimaCamera(engine, camerasManager, position, up)
	, _target(target)
	, _viewMatrix(engine)
{
	_target[0] = 0.0f;
	_target[1] = 0.0f;
	_target[2] = 0.0f;
}

AnimaABCamera::AnimaABCamera(const AnimaABCamera& src)
	: AnimaCamera(src)
	, _target(src._target)
	, _viewMatrix(src._viewMatrix)
{
}

AnimaABCamera::AnimaABCamera(AnimaABCamera&& src)
	: AnimaCamera(src)
	, _target(src._target)
	, _viewMatrix(src._viewMatrix)
{
}

AnimaABCamera::~AnimaABCamera()
{
}

AnimaABCamera& AnimaABCamera::operator=(const AnimaABCamera& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_target = src._target;
		_viewMatrix = src._viewMatrix;
	}

	return *this;
}

AnimaABCamera& AnimaABCamera::operator=(AnimaABCamera&& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_target = src._target;
		_viewMatrix = src._viewMatrix;
	}

	return *this;
}

AnimaMatrix AnimaABCamera::GetViewMatrix()
{
	return _viewMatrix;
}

AnimaVertex3f AnimaABCamera::GetForward()
{
	AnimaVertex3f forward = _target - _position;
	forward.Normalize();
	forward.DumpMemory(false);
	return forward;
}

AnimaVertex3f AnimaABCamera::GetLeft()
{
	AnimaVertex3f left = _up ^ GetForward();
	left.Normalize();
	return left;
}

AnimaVertex3f AnimaABCamera::GetRight()
{
	AnimaVertex3f right = GetForward() ^ _up;
	right.Normalize();
	return right;
}

AnimaVertex3f AnimaABCamera::GetTarget()
{
	return _target;
}

void AnimaABCamera::Zoom(AFloat amount)
{
	_position -= _target;

	if (amount < 0)
		_position += _position * 0.1f;

	if (amount > 0 && _position.Length() > 0.05f)
		_position -= _position * 0.1f;

	_position += _target;

	CalculateViewMatrix();
}

void AnimaABCamera::Move(const AnimaVertex3f& direction, AFloat amount)
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

	CalculateViewMatrix();
}

void AnimaABCamera::Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount)
{
	AnimaVertex3f dir(_engine);
	dir[0] = xDirection;
	dir[1] = yDirection;
	dir[2] = zDirection;

	Move(dir, amount);
}

void AnimaABCamera::RotateX(AFloat angle)
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

void AnimaABCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaABCamera::RotateY(AFloat angle)
{
	_position -= _target;

	AnimaMath::RotateVector(_xAxis, angle, _worldYAxis);
	AnimaMath::RotateVector(_yAxis, angle, _worldYAxis);
	AnimaMath::RotateVector(_zAxis, angle, _worldYAxis);

	float dist = _position.Length();
	_position = _target + _zAxis * dist;

	CalculateViewMatrix();
}

void AnimaABCamera::RotateYDeg(AFloat angle)
{
	RotateY(angle * (float)M_PI / 180.0f);
}

AFloat AnimaABCamera::GetDistance()
{
	AnimaVertex3f direction = _position - _target;
	return direction.Length();
}

void AnimaABCamera::SetDistance(AFloat dist)
{
	if (dist <= 0.0f)
		return;

	AnimaVertex3f dir = GetForward();
	dir.Inverse();
	dir.Normalize();
	dir *= dist;

	_position = _target + dir;
}

void AnimaABCamera::RecalculateVectors()
{
	AnimaVertex3f v = _target - _position;
	float d = v[0] * v[0] + v[2] * v[2];

	_up[0] = -(v[1] * v[0]) / d;
	_up[1] = 1.0f;
	_up[2] = -(v[1] * v[2]) / d;
	_up.Normalize();
}

void AnimaABCamera::LookAt(const AnimaVertex3f& position, const AnimaVertex3f& target)
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

void AnimaABCamera::CalculateViewMatrix()
{
	_viewMatrix[0] = _xAxis[0];								_viewMatrix[1] = _yAxis[0];								_viewMatrix[2] = _zAxis[0];								_viewMatrix[3] = 0.0f;
	_viewMatrix[4] = _xAxis[1];								_viewMatrix[5] = _yAxis[1];								_viewMatrix[6] = _zAxis[1];								_viewMatrix[7] = 0.0f;
	_viewMatrix[8] = _xAxis[2];								_viewMatrix[9] = _yAxis[2];								_viewMatrix[10] = _zAxis[2];							_viewMatrix[11] = 0.0f;
	_viewMatrix[12] = -AnimaMath::Dot(_xAxis, _position);	_viewMatrix[13] = -AnimaMath::Dot(_yAxis, _position);	_viewMatrix[14] = -AnimaMath::Dot(_zAxis, _position);	_viewMatrix[15] = 1.0f;
}

END_ANIMA_ENGINE_NAMESPACE
