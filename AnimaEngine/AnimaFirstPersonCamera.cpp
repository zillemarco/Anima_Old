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

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager)
	: AnimaCamera(allocator, camerasManager)
{
	_zAxis.x = 0.0f;
	_zAxis.y = 0.0f;
	_zAxis.z = 1.0f;
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaAllocator* allocator)
	: AnimaCamera(allocator, nullptr)
{
	_zAxis.x = 0.0f;
	_zAxis.y = 0.0f;
	_zAxis.z = 1.0f;
}

AnimaFirstPersonCamera::AnimaFirstPersonCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up)
	: AnimaCamera(allocator, camerasManager, position)
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
	
	AnimaMath::RotateVector(_zAxis, angle, hAxis);
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
	
	AnimaMath::RotateVector(_zAxis, angle, _worldYAxis);
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
	_viewMatrix.LookAt(_position, _zAxis, _yAxis);
	
	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
	_InverseProjectionViewMatrix = _projectionViewMatrix.Inversed();

	//_frustum.SetCameraView(_position, _zAxis, _yAxis);
	_frustum.ComputeFrustum(_projectionViewMatrix);
}

END_ANIMA_ENGINE_NAMESPACE
