//
//  AnimaCamera.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaCamera.h"
#include "AnimaMath.h"
#include "AnimaCamerasManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define INIT_X_AXIS _xAxis[0] = 1.0f;	\
					_xAxis[1] = 0.0f;	\
					_xAxis[2] = 0.0f;

#define INIT_Y_AXIS _yAxis[0] = 0.0f;	\
					_yAxis[1] = 1.0f;	\
					_yAxis[2] = 0.0f;

#define INIT_Z_AXIS _zAxis[0] = 0.0f;	\
					_zAxis[1] = 0.0f;	\
					_zAxis[2] = 1.0f;

#define INIT_AXIS	INIT_X_AXIS INIT_Y_AXIS INIT_Z_AXIS

AnimaCamera::AnimaCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	: _position(engine)
	, _up(engine)
	, _xAxis(engine)
	, _yAxis(engine)
	, _zAxis(engine)
	, _active(false)
	, _camerasManager(camerasManager)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_position[0] = 0.0f;
	_position[1] = 0.0f;
	_position[2] = 5.0f;

	_up[0] = 0.0f;
	_up[1] = 1.0f;
	_up[2] = 0.0f;

	INIT_AXIS
}

AnimaCamera::AnimaCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& up)
	: _position(position)
	, _up(up)
	, _xAxis(engine)
	, _yAxis(engine)
	, _zAxis(engine)
	, _active(false)
	, _camerasManager(camerasManager)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_up.Normalize();

	INIT_AXIS
}

AnimaCamera::AnimaCamera(const AnimaCamera& src)
	: _position(src._position)
	, _up(src._up)
	, _xAxis(src._xAxis)
	, _yAxis(src._yAxis)
	, _zAxis(src._zAxis)
	, _active(src._active)
	, _camerasManager(src._camerasManager)
{
	_engine = src._engine;

	_up.Normalize();

	INIT_AXIS
}

AnimaCamera::AnimaCamera(AnimaCamera&& src)
	: _position(src._position)
	, _up(src._up)
	, _xAxis(src._xAxis)
	, _yAxis(src._yAxis)
	, _zAxis(src._zAxis)
	, _active(src._active)
	, _engine(src._engine)
	, _camerasManager(src._camerasManager)
{
	_up.Normalize();

	INIT_AXIS
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
		_active = src._active;
		_camerasManager = src._camerasManager;

		_up.Normalize();

		INIT_AXIS
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
		_active = src._active;
		_camerasManager = src._camerasManager;

		_up.Normalize();

		INIT_AXIS
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

void AnimaCamera::Activate()
{
	ANIMA_ASSERT(_camerasManager != nullptr);
	_active = true;
	
	_camerasManager->NotifyCameraActivation(this);
}

void AnimaCamera::Deactivate()
{
	ANIMA_ASSERT(_camerasManager != nullptr);
	_active = false;
	_camerasManager->NotifyCameraDeactivation(this);
}

bool AnimaCamera::IsActive()
{
	return _active;
}

END_ANIMA_ENGINE_NAMESPACE
