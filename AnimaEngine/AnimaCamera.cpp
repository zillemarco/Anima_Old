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

#define INIT_X_AXIS _xAxis.x = 1.0f;	\
					_xAxis.y = 0.0f;	\
					_xAxis.z = 0.0f;

#define INIT_Y_AXIS _yAxis.x = 0.0f;	\
					_yAxis.y = 1.0f;	\
					_yAxis.z = 0.0f;

#define INIT_Z_AXIS _zAxis.x = 0.0f;	\
					_zAxis.y = 0.0f;	\
					_zAxis.z = 1.0f;

#define INIT_WORLD_X_AXIS	_worldXAxis.x = 1.0f;	\
							_worldXAxis.y = 0.0f;	\
							_worldXAxis.z = 0.0f;

#define INIT_WORLD_Y_AXIS	_worldYAxis.x = 0.0f;	\
							_worldYAxis.y = 1.0f;	\
							_worldYAxis.z = 0.0f;

#define INIT_WORLD_Z_AXIS	_worldZAxis.x = 0.0f;	\
							_worldZAxis.y = 0.0f;	\
							_worldZAxis.z = 1.0f;

#define INIT_AXIS			INIT_X_AXIS			INIT_Y_AXIS			INIT_Z_AXIS
#define INIT_WORLD_AXIS		INIT_WORLD_X_AXIS	INIT_WORLD_Y_AXIS	INIT_WORLD_Z_AXIS

AnimaCamera::AnimaCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager)
	//: _position(engine)
	//, _xAxis(engine)
	//, _yAxis(engine)
	//, _zAxis(engine)
	//, _worldXAxis(engine)
	//, _worldYAxis(engine)
	//, _worldZAxis(engine)
	: _active(false)
	, _viewMatrix(engine)
	, _projectionMatrix(engine)
	, _camerasManager(camerasManager)
	, _projectionType(PERSPECTIVE)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_position.x = 0.0f;
	_position.y = 0.0f;
	_position.z = 5.0f;
	
	INIT_AXIS;
	INIT_WORLD_AXIS;
}

AnimaCamera::AnimaCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position)
	: _position(position)
	//, _xAxis(engine)
	//, _yAxis(engine)
	//, _zAxis(engine)
	//, _worldXAxis(engine)
	//, _worldYAxis(engine)
	//, _worldZAxis(engine)
	, _active(false)
	, _viewMatrix(engine)
	, _projectionMatrix(engine)
	, _camerasManager(camerasManager)
	, _projectionType(PERSPECTIVE)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	
	INIT_AXIS;
	INIT_WORLD_AXIS;
}

AnimaCamera::AnimaCamera(const AnimaCamera& src)
	: _position(src._position)
	, _xAxis(src._xAxis)
	, _yAxis(src._yAxis)
	, _zAxis(src._zAxis)
	, _worldXAxis(src._worldXAxis)
	, _worldYAxis(src._worldYAxis)
	, _worldZAxis(src._worldZAxis)
	, _active(src._active)
	, _viewMatrix(src._viewMatrix)
	, _projectionMatrix(src._projectionMatrix)
	, _camerasManager(src._camerasManager)
	, _projectionType(src._projectionType)
{
	_engine = src._engine;

	INIT_WORLD_AXIS;
}

AnimaCamera::AnimaCamera(AnimaCamera&& src)
	: _position(src._position)
	, _xAxis(src._xAxis)
	, _yAxis(src._yAxis)
	, _zAxis(src._zAxis)
	, _worldXAxis(src._worldXAxis)
	, _worldYAxis(src._worldYAxis)
	, _worldZAxis(src._worldZAxis)
	, _active(src._active)
	, _viewMatrix(src._viewMatrix)
	, _projectionMatrix(src._projectionMatrix)
	, _engine(src._engine)
	, _camerasManager(src._camerasManager)
	, _projectionType(src._projectionType)
{

	INIT_WORLD_AXIS;
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
		_active = src._active;
		_camerasManager = src._camerasManager;

		_viewMatrix = src._viewMatrix;
		_projectionMatrix = src._projectionMatrix;
		_projectionType = src._projectionType;

		_xAxis = src._xAxis;
		_yAxis = src._yAxis;
		_zAxis = src._zAxis;

		INIT_WORLD_AXIS;
	}

	return *this;
}

AnimaCamera& AnimaCamera::operator=(AnimaCamera&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_position = src._position;
		_active = src._active;
		_camerasManager = src._camerasManager;

		_viewMatrix = src._viewMatrix;
		_projectionMatrix = src._projectionMatrix;
		_projectionType = src._projectionType;

		_xAxis = src._xAxis;
		_yAxis = src._yAxis;
		_zAxis = src._zAxis;
		
		INIT_WORLD_AXIS;
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
	_position.x = x;
	_position.y = y;
	_position.z = z;
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

AnimaVertex3f AnimaCamera::GetUp()
{
	return _yAxis;
}

AnimaVertex3f AnimaCamera::GetForward()
{
	return _zAxis;
}

AnimaVertex3f AnimaCamera::GetLeft()
{
	return -_xAxis;
}

AnimaVertex3f AnimaCamera::GetRight()
{
	return _xAxis;
}

AnimaMatrix AnimaCamera::GetViewMatrix()
{
	return _viewMatrix;
}

AnimaMatrix AnimaCamera::GetProjectionMatrix()
{
	return _projectionMatrix;
}

void AnimaCamera::CalculateProjectionMatrix(float fov, float ratio, float zNear, float zFar)
{
	_projectionMatrix.SetIdentity();
	_projectionMatrix.Perspective(fov, ratio, zNear, zFar);
	_projectionType = PERSPECTIVE;
}

void AnimaCamera::CalculateProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_projectionMatrix.SetIdentity();
	_projectionMatrix.Ortho(left, right, bottom, top, zNear, zFar);
	_projectionType = ORTHO;
}

bool AnimaCamera::IsPerspectiveProjectionType()
{
	return _projectionType == PERSPECTIVE;
}

bool AnimaCamera::IsOrthoProjectionType()
{
	return _projectionType == ORTHO;
}

END_ANIMA_ENGINE_NAMESPACE
