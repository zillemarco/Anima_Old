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
	, _rotation(engine)
{
	_target[0] = 0.0f;
	_target[1] = 0.0f;
	_target[2] = 0.0f;

	_rotation[0] = 0.0f;
	_rotation[1] = 0.0f;
	_rotation[2] = 0.0f;
}

AnimaABCamera::AnimaABCamera(AnimaEngine* engine)
	: AnimaCamera(engine, nullptr)
	, _target(engine)
	, _rotation(engine)
{
	_target[0] = 0.0f;
	_target[1] = 0.0f;
	_target[2] = 0.0f;

	_rotation[0] = 0.0f;
	_rotation[1] = 0.0f;
	_rotation[2] = 0.0f;
}

AnimaABCamera::AnimaABCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& target, const AnimaVertex3f& up)
	: AnimaCamera(engine, camerasManager, position, up)
	, _target(target)
	, _rotation(engine)
{
	_rotation[0] = 0.0f;
	_rotation[1] = 0.0f;
	_rotation[2] = 0.0f;
}

AnimaABCamera::AnimaABCamera(const AnimaABCamera& src)
	: AnimaCamera(src)
	, _target(src._target)
	, _rotation(src._rotation)
{
}

AnimaABCamera::AnimaABCamera(AnimaABCamera&& src)
	: AnimaCamera(src)
	, _target(src._target)
	, _rotation(src._rotation)
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
		_rotation = src._rotation;
	}

	return *this;
}

AnimaABCamera& AnimaABCamera::operator=(AnimaABCamera&& src)
{
	if (this != &src)
	{
		AnimaCamera::operator=(src);

		_target = src._target;
		_rotation = src._rotation;
	}

	return *this;
}

AnimaVertex3f AnimaABCamera::GetForward()
{
	AnimaVertex3f forward = _target - _position;
	//forward[0] *= -1.0f;
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

AnimaVertex3f AnimaABCamera::GetCenter()
{
	return _target;
}

void AnimaABCamera::Zoom(AFloat amount)
{
	if (amount >= 0.0f && GetDistance() <= 0.001f)
		return;

	AnimaVertex3f dir = GetForward();
	dir.Normalize();
	dir *= amount;

	AnimaVertex3f newPos = _position + dir;
	AFloat dist = (_target - newPos).Length();

	if (fabs(dist) <= 0.00001f)
		return;

	_position += dir;
}

void AnimaABCamera::Move(const AnimaVertex3f& direction, AFloat amount)
{
	AnimaVertex3f dir = direction;
	dir.Normalize();
	dir *= amount;

	//_position += direction * amount;
	//_target += direction * amount;

	AnimaVertex3f t = GetForward();
	t.Normalize();

	_up.Normalize();
	_up = _up - (t * AnimaMath::Dot(t, _up));
	_up.Normalize();

	AnimaVertex3f b = _up ^ t;
	b.Normalize();

	_position += b * dir[0];
	_position += _up * dir[1];
	_position -= t * dir[2];

	_target += b * dir[0];
	_target += _up * dir[1];
	_target -= t * dir[2];
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
	//AnimaVertex3f hAxis = _yAxis ^ GetForward();
	//hAxis.Normalize();

	//AnimaMath::RotateVector(_position, angle, hAxis);
	//_position += _target;

	AnimaMatrix matrix(_engine);

	matrix.RotateX(-_rotation[0]);
	matrix.RotateY(-_rotation[1]);
	matrix.RotateZ(-_rotation[2]);
	matrix.Translate(-_target[0], -_target[1], -_target[2]);
	_position = AnimaMath::MatrixMulVector(matrix, _position);

	matrix.SetIdentity();
	matrix.RotateX(angle);
	_position = AnimaMath::MatrixMulVector(matrix, _position);
	
	matrix.SetIdentity();
	matrix.Translate(_target[0], _target[1], _target[2]);
	matrix.RotateZ(_rotation[2]);
	matrix.RotateY(_rotation[1]);
	matrix.RotateX(_rotation[0]);
	_position = AnimaMath::MatrixMulVector(matrix, _position);

	_rotation[0] += angle;

	RecalculateVectors();
}

void AnimaABCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaABCamera::RotateY(AFloat angle)
{
	//AnimaVertex3f rotAxis = GetRight() ^ GetForward();
	//rotAxis.Normalize();

	//AnimaMath::RotateVector(_position, angle, rotAxis);
	//_position += _target;

	AnimaMatrix matrix(_engine);

	matrix.RotateX(-_rotation[0]);
	matrix.RotateY(-_rotation[1]);
	matrix.RotateZ(-_rotation[2]);
	matrix.Translate(-_target[0], -_target[1], -_target[2]);
	_position = AnimaMath::MatrixMulVector(matrix, _position);

	matrix.SetIdentity();
	matrix.RotateY(angle);
	_position = AnimaMath::MatrixMulVector(matrix, _position);

	matrix.SetIdentity();
	matrix.Translate(_target[0], _target[1], _target[2]);
	matrix.RotateZ(_rotation[2]);
	matrix.RotateY(_rotation[1]);
	matrix.RotateX(_rotation[0]);
	_position = AnimaMath::MatrixMulVector(matrix, _position);

	_rotation[1] += angle;
	
	RecalculateVectors();
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
	dir.Reverse();
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

END_ANIMA_ENGINE_NAMESPACE
