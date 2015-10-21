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
#include "AnimaXmlTranslators.h"

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

AnimaCamera::AnimaCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaSceneObject(name, dataGeneratorManager, allocator)
	, _active(false)
	, _projectionType(PERSPECTIVE)
	, _camerasManager(camerasManager)
{
	IMPLEMENT_ANIMA_CLASS(AnimaCamera);

	_position.x = 0.0f;
	_position.y = 0.0f;
	_position.z = 5.0f;

	_fov = 0.0f;
	_zNear = 0.0f;
	_zFar = 0.0f;
	
	INIT_AXIS;
	INIT_WORLD_AXIS;
}

AnimaCamera::AnimaCamera(const AnimaCamera& src)
	: AnimaSceneObject(src)
	, _position(src._position)
	, _xAxis(src._xAxis)
	, _yAxis(src._yAxis)
	, _zAxis(src._zAxis)
	, _worldXAxis(src._worldXAxis)
	, _worldYAxis(src._worldYAxis)
	, _worldZAxis(src._worldZAxis)
	, _fov(src._fov)
	, _zNear(src._zNear)
	, _zFar(src._zFar)
	, _active(src._active)
	, _projectionType(src._projectionType)
	, _viewMatrix(src._viewMatrix)
	, _projectionMatrix(src._projectionMatrix)
	, _camerasManager(src._camerasManager)
{
	INIT_WORLD_AXIS;
}

AnimaCamera::AnimaCamera(AnimaCamera&& src)
	: AnimaSceneObject(src)
	, _position(src._position)
	, _xAxis(src._xAxis)
	, _yAxis(src._yAxis)
	, _zAxis(src._zAxis)
	, _worldXAxis(src._worldXAxis)
	, _worldYAxis(src._worldYAxis)
	, _worldZAxis(src._worldZAxis)
	, _fov(src._fov)
	, _zNear(src._zNear)
	, _zFar(src._zFar)
	, _active(src._active)
	, _projectionType(src._projectionType)
	, _viewMatrix(src._viewMatrix)
	, _projectionMatrix(src._projectionMatrix)
	, _camerasManager(src._camerasManager)
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
		AnimaSceneObject::operator=(src);

		_position = src._position;
		_active = src._active;
		_camerasManager = src._camerasManager;

		_viewMatrix = src._viewMatrix;
		_projectionMatrix = src._projectionMatrix;
		_projectionType = src._projectionType;

		_xAxis = src._xAxis;
		_yAxis = src._yAxis;
		_zAxis = src._zAxis;

		_fov = src._fov;
		_zNear = src._zNear;
		_zFar = src._zFar;

		INIT_WORLD_AXIS;
	}

	return *this;
}

AnimaCamera& AnimaCamera::operator=(AnimaCamera&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_position = src._position;
		_active = src._active;
		_camerasManager = src._camerasManager;

		_viewMatrix = src._viewMatrix;
		_projectionMatrix = src._projectionMatrix;
		_projectionType = src._projectionType;

		_xAxis = src._xAxis;
		_yAxis = src._yAxis;
		_zAxis = src._zAxis;

		_fov = src._fov;
		_zNear = src._zNear;
		_zFar = src._zFar;
		
		INIT_WORLD_AXIS;
	}

	return *this;
}

ptree AnimaCamera::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaCamera.Name", GetName());
	
	tree.add("AnimaCamera.XAxis", _xAxis);
	tree.add("AnimaCamera.YAxis", _yAxis);
	tree.add("AnimaCamera.ZAxis", _zAxis);
	tree.add("AnimaCamera.WorldXAxis", _worldXAxis);
	tree.add("AnimaCamera.WorldYAxis", _worldYAxis);
	tree.add("AnimaCamera.WorldZAxis", _worldZAxis);
	tree.add("AnimaCamera.FOV", _fov);
	tree.add("AnimaCamera.ZNear", _zNear);
	tree.add("AnimaCamera.ZFar", _zFar);
	tree.add("AnimaCamera.ProjectionType", _projectionType);
	
	tree.add_child("AnimaCamera.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaCamera::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaCamera.Name"));
		
		_projectionType = objectTree.get<AnimaCameraProjectionType>("AnimaCamera.ProjectionType", PERSPECTIVE);
		_xAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.XAxis", AnimaVertex3f(1.0f, 0.0f, 0.0f));
		_yAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.YAxis", AnimaVertex3f(0.0f, 1.0f, 0.0f));
		_zAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.ZAxis", AnimaVertex3f(0.0f, 0.0f, 1.0f));
		_worldXAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.WorldXAxis", AnimaVertex3f(1.0f, 0.0f, 0.0f));
		_worldYAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.WorldYAxis", AnimaVertex3f(0.0f, 1.0f, 0.0f));
		_worldZAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.WorldZAxis", AnimaVertex3f(0.0f, 0.0f, 1.0f));
		_fov = objectTree.get<AFloat>("AnimaCamera.FOV", 60.0f);
		_zNear = objectTree.get<AFloat>("AnimaCamera.ZNear", 0.1f);
		_zFar = objectTree.get<AFloat>("AnimaCamera.ZFar", 1000.0f);
		
		ptree sceneObjectTree = objectTree.get_child("AnimaCamera.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetVector3f("Position"));
			_viewMatrix = GetMatrix("ViewMatrix");
			_projectionMatrix = GetMatrix("ProjectionMatrix");
			_projectionViewMatrix = GetMatrix("ProjectionViewMatrix");
			_inverseProjectionViewMatrix = GetMatrix("InverseProjectionViewMatrix");
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing camera: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing camera: %s", exception.what());
		return false;
	}
}

AnimaVertex3f AnimaCamera::GetPosition()
{
	return _position;
}

void AnimaCamera::SetPosition(const AnimaVertex3f& position)
{
	_position = position;

	SetVector("Position", _position);
}

void AnimaCamera::SetPosition(const AFloat& x, const AFloat& y, const AFloat& z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	SetVector("Position", _position);
}

void AnimaCamera::Activate()
{
	_active = true;

	if (_camerasManager != nullptr)
		_camerasManager->NotifyCameraActivation(this);
}

void AnimaCamera::Deactivate()
{
	_active = false;

	if (_camerasManager != nullptr)
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

void AnimaCamera::SetViewMatrix(const AnimaMatrix& matrix)
{
	_viewMatrix = matrix;

	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
	_inverseProjectionViewMatrix = _projectionViewMatrix.Inversed();

	SetMatrix("ViewMatrix", _viewMatrix);
	SetMatrix("ProjectionViewMatrix", _projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", _inverseProjectionViewMatrix);
}

AnimaMatrix AnimaCamera::GetViewMatrix()
{
	return _viewMatrix;
}

void AnimaCamera::SetProjectionMatrix(const AnimaMatrix& matrix)
{
	_projectionMatrix = matrix;

	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
	_inverseProjectionViewMatrix = _projectionViewMatrix.Inversed();

	SetMatrix("ProjectionMatrix", _projectionMatrix);
	SetMatrix("ProjectionViewMatrix", _projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", _inverseProjectionViewMatrix);
}

AnimaMatrix AnimaCamera::GetProjectionMatrix()
{
	return _projectionMatrix;
}

void AnimaCamera::CalculateProjectionMatrix(float fov, const AnimaVertex2f& size, float zNear, float zFar)
{
	if (_windowSize != size)
		_windowSize = size;

	if (fov != _fov || zNear != _zNear || zFar != _zFar)
	{
		_zNear = zNear;
		_zFar = zFar;
		_fov = fov;

		_projectionMatrix.SetIdentity();
		_projectionMatrix.Perspective(fov, size.x / size.y, zNear, zFar);
		_projectionType = PERSPECTIVE;

		_projectionViewMatrix = _projectionMatrix * _viewMatrix;
		_inverseProjectionViewMatrix = _projectionViewMatrix.Inversed();

		_frustum.ComputeFrustum(_projectionViewMatrix);

		SetMatrix("ProjectionMatrix", _projectionMatrix);
		SetMatrix("ProjectionViewMatrix", _projectionViewMatrix);
		SetMatrix("InverseProjectionViewMatrix", _inverseProjectionViewMatrix);
	}
}

void AnimaCamera::CalculateProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
	_zNear = zNear;
	_zFar = zFar;

	_projectionMatrix.SetIdentity();
	_projectionMatrix.Ortho(left, right, bottom, top, zNear, zFar);
	_projectionType = ORTHO;

	_projectionViewMatrix = _projectionMatrix * _viewMatrix;
	_inverseProjectionViewMatrix = _projectionViewMatrix.Inversed();

	//_frustum.ComputeFrustum(_projectionMatrix, _viewMatrix, zNear, zFar, fov, size.x / size.y);

	SetMatrix("ProjectionMatrix", _projectionMatrix);
	SetMatrix("ProjectionViewMatrix", _projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", _inverseProjectionViewMatrix);
}

bool AnimaCamera::IsPerspectiveProjectionType()
{
	return _projectionType == PERSPECTIVE;
}

bool AnimaCamera::IsOrthoProjectionType()
{
	return _projectionType == ORTHO;
}

AnimaVertex2f AnimaCamera::GetWindowSize()
{
	return _windowSize;
}

AnimaMatrix AnimaCamera::GetProjectionViewMatrix()
{
	return _projectionViewMatrix;
}

AnimaMatrix AnimaCamera::GetInversedProjectionViewMatrix()
{
	return _inverseProjectionViewMatrix;
}

AnimaFrustum* AnimaCamera::GetFrustum()
{
	return &_frustum;
}

END_ANIMA_ENGINE_NAMESPACE
