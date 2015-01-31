//
//  AnimaFirstPersonCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFirstPersonCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	: AnimaCamera(engine, camerasManager)
{
	_zAxis.x = 0.0f;
	_zAxis.y = 0.0f;
	_zAxis.z = 1.0f;
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaEngine* engine)
	: AnimaCamera(engine, nullptr)
{
	_zAxis.x = 0.0f;
	_zAxis.y = 0.0f;
	_zAxis.z = 1.0f;
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up)
	: AnimaCamera(engine, camerasManager, position)
{
	_yAxis = up;
	_zAxis = forward;

	_yAxis.Normalize();
	_zAxis.Normalize();
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(const AnimaFirstPersonCamera& src)
	: AnimaCamera(src)
{
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaFirstPersonCamera&& src)
	: AnimaCamera(src)
{
}

AnimaFirstPersonCamera::~AnimaFirstPersonCamera()
{
}

AnimaFirstPersonCamera& AnimaFirstPersonCamera::operator=(const AnimaFirstPersonCamera& src)
{
	AnimaCamera::operator=(src);
	return *this;
}

AnimaFirstPersonCamera& AnimaFirstPersonCamera::operator=(AnimaFirstPersonCamera&& src)
{
	AnimaCamera::operator=(src);
	return *this;
}

void AnimaFirstPersonCamera::Zoom(AFloat amount)
{
	Move(_zAxis, amount);
}

void AnimaFirstPersonCamera::Move(const AnimaVertex3f& direction, AFloat amount)
{
	_position += direction * amount;

	LookAt(_position, _zAxis);
}

void AnimaFirstPersonCamera::Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount)
{
	_position.x += xDirection * amount;
	_position.y += yDirection * amount;
	_position.z += zDirection * amount;

	LookAt(_position, _zAxis);
}

void AnimaFirstPersonCamera::RotateX(AFloat angle)
{
	AnimaVertex3f hAxis = _worldYAxis ^ _zAxis;
	hAxis.Normalize();
	
	//AnimaMath::RotateVector(_zAxis, angle, hAxis);
	_zAxis.Normalize();

	AnimaVertex3f newUp = _zAxis ^ hAxis;
	newUp.Normalize();
	_yAxis = newUp;

	CalculateViewMatrix();
}

void AnimaFirstPersonCamera::RotateXDeg(AFloat angle)
{
	RotateX(angle * (float)M_PI / 180.0f);
}

void AnimaFirstPersonCamera::RotateY(AFloat angle)
{
	AnimaVertex3f hAxis = _worldYAxis ^ _zAxis;
	hAxis.Normalize();
	
	//AnimaMath::RotateVector(_zAxis, angle, _worldYAxis);
	_zAxis.Normalize();

	AnimaVertex3f newUp = _zAxis ^ hAxis;
	newUp.Normalize();
	_yAxis = newUp;

	CalculateViewMatrix();
}

void AnimaFirstPersonCamera::RotateYDeg(AFloat angle)
{
	RotateY(angle * (float)M_PI / 180.0f);
}

void AnimaFirstPersonCamera::LookAt(const AnimaVertex3f& position, const AnimaVertex3f& forward)
{
	_position = position;
	
	_zAxis = forward;
	_zAxis.Normalize();

	_xAxis = _zAxis ^ _yAxis;
	_xAxis.Normalize();

	_yAxis = _xAxis ^ _zAxis;
	_yAxis.Normalize();

	CalculateViewMatrix();
}

void AnimaFirstPersonCamera::LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xForward, AFloat yForward, AFloat zForward)
{
	AnimaVertex3f position(xPosition, yPosition, zPosition);
	AnimaVertex3f forward(xForward, yForward, zForward);
	LookAt(position, forward);
}

void AnimaFirstPersonCamera::CalculateViewMatrix()
{
	_viewMatrix[0] = _xAxis.x;		_viewMatrix[1] = _xAxis.y;		_viewMatrix[2] = _xAxis.z;		_viewMatrix[3] = 0.0f;
	_viewMatrix[4] = _yAxis.x;		_viewMatrix[5] = _yAxis.y;		_viewMatrix[6] = _yAxis.z;		_viewMatrix[7] = 0.0f;
	_viewMatrix[8] = _zAxis.x;		_viewMatrix[9] = _zAxis.y;		_viewMatrix[10] = _zAxis.z;		_viewMatrix[11] = 0.0f;
	_viewMatrix[12] = 0.0f;			_viewMatrix[13] = 0.0f;			_viewMatrix[14] = 0.0f;			_viewMatrix[15] = 1.0f;

	_viewMatrix.Translate(-_position.x, -_position.y, -_position.z);
}

END_ANIMA_ENGINE_NAMESPACE
