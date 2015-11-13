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
#include "AnimaMath.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

const AnimaVertex3f AnimaCamera::DEFAULT_ACCELERATION(40.0f, 40.0f, 40.0f);
const AnimaVertex3f AnimaCamera::DEFAULT_MAXIMUM_VELOCITY(10.0f, 10.0f, 10.0f);

const AFloat AnimaCamera::DEFAULT_ROTATION_SPEED = 0.3f;
const AFloat AnimaCamera::DEFAULT_FOVY = 90.0f;
const AFloat AnimaCamera::DEFAULT_ZNEAR = 0.1f;
const AFloat AnimaCamera::DEFAULT_ZFAR = 1000.0f;

const AFloat AnimaCamera::DEFAULT_ORBIT_MIN_ZOOM = DEFAULT_ZNEAR + 1.0f;
const AFloat AnimaCamera::DEFAULT_ORBIT_MAX_ZOOM = DEFAULT_ZFAR * 0.5f;
const AFloat AnimaCamera::DEFAULT_ORBIT_OFFSET_DISTANCE = DEFAULT_ORBIT_MIN_ZOOM + (DEFAULT_ORBIT_MAX_ZOOM - DEFAULT_ORBIT_MIN_ZOOM) * 0.25f;

const AnimaVertex3f AnimaCamera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const AnimaVertex3f AnimaCamera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const AnimaVertex3f AnimaCamera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

AnimaCamera::AnimaCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaSceneObject(name, dataGeneratorManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaCamera);
	
	_cameraType = ACT_FIRST_PERSON;
	_projectionType = ACPT_PERSPECTIVE;
	_preferTargetYAxisOrbiting = true;
	
	_accumPitchDegrees = 0.0f;
	_savedAccumPitchDegrees = 0.0f;
	
	_rotationSpeed = DEFAULT_ROTATION_SPEED;
	_fovy = DEFAULT_FOVY;
	_aspectRatio = 1.0f;
	_zNear = DEFAULT_ZNEAR;
	_zFar = DEFAULT_ZFAR;
	_top = 1.0f;
	_left = 1.0f;
	_bottom = 1.0f;
	_right = 1.0f;
	
	_orbitMinZoom = DEFAULT_ORBIT_MIN_ZOOM;
	_orbitMaxZoom = DEFAULT_ORBIT_MAX_ZOOM;
	_orbitOffsetDistance = DEFAULT_ORBIT_OFFSET_DISTANCE;
	
	_savedPosition.Fill(0.0f, 0.0f, 0.0f);
	_target.Fill(0.0f, 0.0f, 0.0f);
	_xAxis.Fill(1.0f, 0.0f, 0.0f);
	_yAxis.Fill(0.0f, 1.0f, 0.0f);
	_targetYAxis.Fill(0.0f, 1.0f, 0.0f);
	_zAxis.Fill(0.0f, 0.0f, 1.0f);
	_viewDirection.Fill(0.0f, 0.0f, -1.0f);
	
	_acceleration = DEFAULT_ACCELERATION;
	_maximumVelocity = DEFAULT_MAXIMUM_VELOCITY;
	_currentVelocity.Fill(0.0f, 0.0f, 0.0f);
	_updateDirection.Fill(0.0f, 0.0f, 0.0f);
	
	_orientation.SetIdentity();
	_savedOrientation.SetIdentity();
	
	_active = false;
	_camerasManager = camerasManager;
}

AnimaCamera::AnimaCamera(const AnimaCamera& src)
	: AnimaSceneObject(src)
{
	IMPLEMENT_ANIMA_CLASS(AnimaCamera);
	
	_cameraType = src._cameraType;
	_projectionType = src._projectionType;
	_preferTargetYAxisOrbiting = src._preferTargetYAxisOrbiting;
	
	_accumPitchDegrees = src._accumPitchDegrees;
	_savedAccumPitchDegrees = src._savedAccumPitchDegrees;
	
	_rotationSpeed = src._rotationSpeed;
	_fovy = src._fovy;
	_aspectRatio = src._aspectRatio;
	_zNear = src._zNear;
	_zFar = src._zFar;
	_top = src._top;
	_left = src._left;
	_bottom = src._bottom;
	_right = src._right;
	
	_orbitMinZoom = src._orbitMinZoom;
	_orbitMaxZoom = src._orbitMaxZoom;
	_orbitOffsetDistance = src._orbitOffsetDistance;
	
	_savedPosition = src._savedPosition;
	_target = src._target;
	_xAxis = src._xAxis;
	_yAxis = src._yAxis;
	_targetYAxis = src._targetYAxis;
	_zAxis = src._zAxis;
	_viewDirection = src._viewDirection;
	
	_acceleration = src._acceleration;
	_currentVelocity = src._currentVelocity;
	_maximumVelocity = src._maximumVelocity;
	_updateDirection = src._updateDirection;
	
	_orientation = src._orientation;
	_savedOrientation = src._savedOrientation;
	
	_active = src._active;
	_camerasManager = src._camerasManager;
}

AnimaCamera::AnimaCamera(AnimaCamera&& src)
	: AnimaSceneObject(src)
{
	IMPLEMENT_ANIMA_CLASS(AnimaCamera);
	
	_cameraType = src._cameraType;
	_projectionType = src._projectionType;
	_preferTargetYAxisOrbiting = src._preferTargetYAxisOrbiting;
	
	_accumPitchDegrees = src._accumPitchDegrees;
	_savedAccumPitchDegrees = src._savedAccumPitchDegrees;
	
	_rotationSpeed = src._rotationSpeed;
	_fovy = src._fovy;
	_aspectRatio = src._aspectRatio;
	_zNear = src._zNear;
	_zFar = src._zFar;
	_top = src._top;
	_left = src._left;
	_bottom = src._bottom;
	_right = src._right;
	
	_orbitMinZoom = src._orbitMinZoom;
	_orbitMaxZoom = src._orbitMaxZoom;
	_orbitOffsetDistance = src._orbitOffsetDistance;
	
	_savedPosition = src._savedPosition;
	_target = src._target;
	_xAxis = src._xAxis;
	_yAxis = src._yAxis;
	_targetYAxis = src._targetYAxis;
	_zAxis = src._zAxis;
	_viewDirection = src._viewDirection;
	
	_acceleration = src._acceleration;
	_currentVelocity = src._currentVelocity;
	_maximumVelocity = src._maximumVelocity;
	_updateDirection = src._updateDirection;
	
	_orientation = src._orientation;
	_savedOrientation = src._savedOrientation;
	
	_active = src._active;
	_camerasManager = src._camerasManager;
}

AnimaCamera::~AnimaCamera()
{
}

AnimaCamera& AnimaCamera::operator=(const AnimaCamera& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);
		
		_cameraType = src._cameraType;
		_projectionType = src._projectionType;
		_preferTargetYAxisOrbiting = src._preferTargetYAxisOrbiting;
		
		_accumPitchDegrees = src._accumPitchDegrees;
		_savedAccumPitchDegrees = src._savedAccumPitchDegrees;
		
		_rotationSpeed = src._rotationSpeed;
		_fovy = src._fovy;
		_aspectRatio = src._aspectRatio;
		_zNear = src._zNear;
		_zFar = src._zFar;
		_top = src._top;
		_left = src._left;
		_bottom = src._bottom;
		_right = src._right;
		
		_orbitMinZoom = src._orbitMinZoom;
		_orbitMaxZoom = src._orbitMaxZoom;
		_orbitOffsetDistance = src._orbitOffsetDistance;
		
		_savedPosition = src._savedPosition;
		_target = src._target;
		_xAxis = src._xAxis;
		_yAxis = src._yAxis;
		_targetYAxis = src._targetYAxis;
		_zAxis = src._zAxis;
		_viewDirection = src._viewDirection;
		
		_acceleration = src._acceleration;
		_currentVelocity = src._currentVelocity;
		_maximumVelocity = src._maximumVelocity;
		_updateDirection = src._updateDirection;
		
		_orientation = src._orientation;
		_savedOrientation = src._savedOrientation;
		
		_active = src._active;
		_camerasManager = src._camerasManager;
	}

	return *this;
}

AnimaCamera& AnimaCamera::operator=(AnimaCamera&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);
		
		_cameraType = src._cameraType;
		_projectionType = src._projectionType;
		_preferTargetYAxisOrbiting = src._preferTargetYAxisOrbiting;
		
		_accumPitchDegrees = src._accumPitchDegrees;
		_savedAccumPitchDegrees = src._savedAccumPitchDegrees;
		
		_rotationSpeed = src._rotationSpeed;
		_fovy = src._fovy;
		_aspectRatio = src._aspectRatio;
		_zNear = src._zNear;
		_zFar = src._zFar;
		_top = src._top;
		_left = src._left;
		_bottom = src._bottom;
		_right = src._right;
		
		_orbitMinZoom = src._orbitMinZoom;
		_orbitMaxZoom = src._orbitMaxZoom;
		_orbitOffsetDistance = src._orbitOffsetDistance;
		
		_savedPosition = src._savedPosition;
		_target = src._target;
		_xAxis = src._xAxis;
		_yAxis = src._yAxis;
		_targetYAxis = src._targetYAxis;
		_zAxis = src._zAxis;
		_viewDirection = src._viewDirection;
		
		_acceleration = src._acceleration;
		_currentVelocity = src._currentVelocity;
		_maximumVelocity = src._maximumVelocity;
		_updateDirection = src._updateDirection;
		
		_orientation = src._orientation;
		_savedOrientation = src._savedOrientation;
		
		_active = src._active;
		_camerasManager = src._camerasManager;
	}

	return *this;
}

ptree AnimaCamera::GetObjectTree(bool saveName) const
{
	ptree tree;

	if (saveName)
		tree.add("AnimaCamera.Name", GetName());
	
	tree.add("AnimaCamera.CameraType", _cameraType);
	tree.add("AnimaCamera.ProjectionType", _projectionType);
	tree.add("AnimaCamera.PreferTargetYAxisOrbiting", _preferTargetYAxisOrbiting);
	tree.add("AnimaCamera.AccumPitchDegrees", _accumPitchDegrees);
	tree.add("AnimaCamera.SavedAccumPitchDegrees", _savedAccumPitchDegrees);
	tree.add("AnimaCamera.RotationSpeed", _rotationSpeed);
	tree.add("AnimaCamera.FovY", _fovy);
	tree.add("AnimaCamera.AspectRatio", _aspectRatio);
	tree.add("AnimaCamera.ZNear", _zNear);
	tree.add("AnimaCamera.ZFar", _zFar);
	tree.add("AnimaCamera.Top", _top);
	tree.add("AnimaCamera.Left", _left);
	tree.add("AnimaCamera.Bottom", _bottom);
	tree.add("AnimaCamera.Right", _right);
	tree.add("AnimaCamera.OrbitMinZoom", _orbitMinZoom);
	tree.add("AnimaCamera.OrbitMaxZoom", _orbitMaxZoom);
	tree.add("AnimaCamera.OrbitOffsetDistance", _orbitOffsetDistance);
	tree.add("AnimaCamera.SavedPosition", _savedPosition);
	tree.add("AnimaCamera.Target", _target);
	tree.add("AnimaCamera.XAxis", _xAxis);
	tree.add("AnimaCamera.YAxis", _yAxis);
	tree.add("AnimaCamera.TargetYAxis", _targetYAxis);
	tree.add("AnimaCamera.ZAxis", _zAxis);
	tree.add("AnimaCamera.ViewDirection", _viewDirection);
	tree.add("AnimaCamera.Acceleration", _acceleration);
	tree.add("AnimaCamera.MaximumVelocity", _maximumVelocity);
	tree.add("AnimaCamera.Orientation", _orientation);
	tree.add("AnimaCamera.SavedOrientation", _savedOrientation);
	tree.add("AnimaCamera.Active", _active);
	
	tree.add_child("AnimaCamera.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaCamera::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaCamera.Name"));
		
		_cameraType = objectTree.get<AnimaCameraType>("AnimaCamera.CameraType", ACT_FIRST_PERSON);
		_projectionType = objectTree.get<AnimaCameraProjectionType>("AnimaCamera.ProjectionType", ACPT_PERSPECTIVE);
		_preferTargetYAxisOrbiting = objectTree.get<bool>("AnimaCamera.PreferTargetYAxisOrbiting", true);
		_accumPitchDegrees = objectTree.get<AFloat>("AnimaCamera.AccumPitchDegrees", 0.0f);
		_savedAccumPitchDegrees = objectTree.get<AFloat>("AnimaCamera.SavedAccumPitchDegrees", 0.0f);
		_rotationSpeed = objectTree.get<AFloat>("AnimaCamera.RotationSpeed", DEFAULT_ROTATION_SPEED);
		_fovy = objectTree.get<AFloat>("AnimaCamera.FovY", DEFAULT_FOVY);
		_aspectRatio = objectTree.get<AFloat>("AnimaCamera.AspectRatio", 1.0f);
		_zNear = objectTree.get<AFloat>("AnimaCamera.ZNear", DEFAULT_ZNEAR);
		_zFar = objectTree.get<AFloat>("AnimaCamera.ZFar", DEFAULT_ZFAR);
		_top = objectTree.get<AFloat>("AnimaCamera.Top", 1.0f);
		_left = objectTree.get<AFloat>("AnimaCamera.Left", 1.0f);
		_bottom = objectTree.get<AFloat>("AnimaCamera.Bottom", 1.0f);
		_right = objectTree.get<AFloat>("AnimaCamera.Right", 1.0f);
		_orbitMinZoom = objectTree.get<AFloat>("AnimaCamera.OrbitMinZoom", DEFAULT_ORBIT_MIN_ZOOM);
		_orbitMaxZoom = objectTree.get<AFloat>("AnimaCamera.OrbitMaxZoom", DEFAULT_ORBIT_MAX_ZOOM);
		_orbitOffsetDistance = objectTree.get<AFloat>("AnimaCamera.OrbitOffsetDistance", DEFAULT_ORBIT_OFFSET_DISTANCE);
		_savedPosition = objectTree.get<AnimaVertex3f>("AnimaCamera.SavedPosition", AnimaVertex3f(0.0f, 0.0f, 0.0f));
		_target = objectTree.get<AnimaVertex3f>("AnimaCamera.Target", AnimaVertex3f(0.0f, 0.0f, 0.0f));
		_xAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.XAxis", AnimaVertex3f(1.0f, 0.0f, 0.0f));
		_yAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.YAxis", AnimaVertex3f(0.0f, 1.0f, 0.0f));
		_targetYAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.TargetYAxis", AnimaVertex3f(0.0f, 1.0f, 0.0f));
		_zAxis = objectTree.get<AnimaVertex3f>("AnimaCamera.ZAxis", AnimaVertex3f(0.0f, 0.0f, 1.0f));
		_viewDirection = objectTree.get<AnimaVertex3f>("AnimaCamera.ViewDirection", AnimaVertex3f(0.0f, 0.0f, -1.0f));
		_acceleration = objectTree.get<AnimaVertex3f>("AnimaCamera.Acceleration", DEFAULT_ACCELERATION);
		_maximumVelocity = objectTree.get<AnimaVertex3f>("AnimaCamera.MaximumVelocity", DEFAULT_MAXIMUM_VELOCITY);
		_orientation = objectTree.get<AnimaQuaternion>("AnimaCamera.Orientation", AnimaQuaternion());
		_savedOrientation = objectTree.get<AnimaQuaternion>("AnimaCamera.SavedOrientation", AnimaQuaternion());
		_active = objectTree.get<bool>("AnimaCamera.Active", false);
		
		ptree sceneObjectTree = objectTree.get_child("AnimaCamera.SceneObject");
		return AnimaSceneObject::ReadObject(sceneObjectTree, scene, false);
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

void AnimaCamera::LookAt(const AnimaVertex3f& target)
{
	LookAt(GetPosition(), target, _yAxis);
}

void AnimaCamera::LookAt(const AnimaVertex3f& position, const AnimaVertex3f& target, const AnimaVertex3f& up)
{
	AnimaSceneObject::SetPosition(position);
	_target = target;
	
	_zAxis = position - target;
	_zAxis.Normalize();
	
	_viewDirection = -_zAxis;
	
	_xAxis = up ^ _zAxis;
	_xAxis.Normalize();
	
	_yAxis = _zAxis ^ _xAxis;
	_yAxis.Normalize();
	
	AnimaMatrix m;
	
	m.vecM[0][0] = _xAxis.x;
	m.vecM[1][0] = _xAxis.y;
	m.vecM[2][0] = _xAxis.z;
	m.vecM[3][0] = -(_xAxis * position);
	
	m.vecM[0][1] = _yAxis.x;
	m.vecM[1][1] = _yAxis.y;
	m.vecM[2][1] = _yAxis.z;
	m.vecM[3][1] = -(_yAxis * position);
	
	m.vecM[0][2] = _zAxis.x;
	m.vecM[1][2] = _zAxis.y;
	m.vecM[2][2] = _zAxis.z;
	m.vecM[3][2] = -(_zAxis * position);
	
	// Extract the pitch angle from the view matrix.
	_accumPitchDegrees = AnimaMath::RadToDeg(asinf(m.vecM[1][2]));
	
	_orientation.FromMatrix(m);
	UpdateViewMatrix();
}

void AnimaCamera::Move(AFloat dx, AFloat dy, AFloat dz)
{
	// Moves the camera by dx world units to the left or right; dy
	// world units upwards or downwards; and dz world units forwards
	// or backwards.
	
	if (_cameraType == ACT_ORBIT)
	{
		// Orbiting camera is always positioned relative to the
		// target position. See updateViewMatrix().
		return;
	}
	
	AnimaVertex3f position = GetPosition();
	AnimaVertex3f forwards;
	
	if (_cameraType == ACT_FIRST_PERSON)
	{
		// Calculate the forwards direction. Can't just use the camera's local
		// z axis as doing so will cause the camera to move more slowly as the
		// camera's view approaches 90 degrees straight up and down.
		
		forwards = WORLD_YAXIS ^ _xAxis;
		forwards.Normalize();
	}
	else
	{
		forwards = _viewDirection;
	}
	
	position += _xAxis * dx;
	position += WORLD_YAXIS * dy;
	position += forwards * dz;
	
	SetPosition(position);
}

void AnimaCamera::Move(const AnimaVertex3f& direction, const AnimaVertex3f& amount)
{
	// Moves the camera by the specified amount of world units in the specified
	// direction in world space.
	
	if (_cameraType == ACT_ORBIT)
	{
		// Orbiting camera is always positioned relative to the
		// target position. See updateViewMatrix().
		return;
	}
	
	AnimaVertex3f position = GetPosition();
	
	position.x += direction.x * amount.x;
	position.y += direction.y * amount.y;
	position.z += direction.z * amount.z;
	
	AnimaSceneObject::SetPosition(position);
	
	UpdateViewMatrix();
}

void AnimaCamera::RotateRad(AFloat head, AFloat pitch, AFloat roll)
{
	RotateDeg(AnimaMath::RadToDeg(head), AnimaMath::RadToDeg(pitch), AnimaMath::RadToDeg(roll));
}

void AnimaCamera::RotateDeg(AFloat head, AFloat pitch, AFloat roll)
{
	// Rotates the camera based on its current behavior.
	// Note that not all behaviors support rolling.
	pitch = -pitch;
	head = -head;
	roll = -roll;
	
	switch (_cameraType)
	{
		default:
			break;
			
		case ACT_FIRST_PERSON:
		case ACT_SPECTATOR:
			RotateFirstPerson(head, pitch);
			break;
			
		case ACT_FLIGHT:
			RotateFlight(head, pitch, roll);
			break;
			
		case ACT_ORBIT:
			RotateOrbit(head, pitch, roll);
			break;
	}
	
	UpdateViewMatrix();
}

void AnimaCamera::SmoothRotateRad(AFloat head, AFloat pitch, AFloat roll)
{
	SmoothRotateDeg(AnimaMath::RadToDeg(head), AnimaMath::RadToDeg(pitch), AnimaMath::RadToDeg(roll));
}

void AnimaCamera::SmoothRotateDeg(AFloat head, AFloat pitch, AFloat roll)
{
	// This method applies a scaling factor to the rotation angles prior to
	// using these rotation angles to rotate the camera. This method is usually
	// called when the camera is being rotated using an input device (such as a
	// mouse or a joystick).
	
	head *= _rotationSpeed;
	pitch *= _rotationSpeed;
	roll *= _rotationSpeed;
	
	RotateDeg(head, pitch, roll);
}

void AnimaCamera::UndoRoll()
{
	// Undo any camera rolling by leveling the camera. When the camera is
	// orbiting this method will cause the camera to become level with the
	// orbit target.
	
	if (_cameraType == ACT_ORBIT)
		LookAt(GetPosition(), _target, _targetYAxis);
	else
		LookAt(GetPosition(), GetPosition() + _viewDirection, WORLD_YAXIS);
}

void AnimaCamera::UpdatePosition(AFloat elapsedTime)
{
	// Moves the camera using Newton's second law of motion. Unit mass is
	// assumed here to somewhat simplify the calculations. The direction vector
	// is in the range [-1,1].
	
	if (_currentVelocity.Length2() != 0.0f)
	{
		// Only move the camera if the velocity vector is not of zero length.
		// Doing this guards against the camera slowly creeping around due to
		// floating point rounding errors.
		
		AnimaVertex3f displacement = (_currentVelocity * elapsedTime) + (0.5f * _acceleration * elapsedTime * elapsedTime);
		
		// Floating point rounding errors will slowly accumulate and cause the
		// camera to move along each axis. To prevent any unintended movement
		// the displacement vector is clamped to zero for each direction that
		// the camera isn't moving in. Note that the updateVelocity() method
		// will slowly decelerate the camera's velocity back to a stationary
		// state when the camera is no longer moving along that direction. To
		// account for this the camera's current velocity is also checked.
		
		if (_updateDirection.x == 0.0f && AnimaMath::CloseEnough(_currentVelocity.x, 0.0f))
			displacement.x = 0.0f;
		
		if (_updateDirection.y == 0.0f && AnimaMath::CloseEnough(_currentVelocity.y, 0.0f))
			displacement.y = 0.0f;
		
		if (_updateDirection.z == 0.0f && AnimaMath::CloseEnough(_currentVelocity.z, 0.0f))
			displacement.z = 0.0f;
		
		AnimaLogger::LogMessageFormat("Velocity: %.3f:%.3f:%.3f", _currentVelocity.x, _currentVelocity.y, _currentVelocity.z);
		AnimaLogger::LogMessageFormat("Displacemente: %.3f:%.3f:%.3f", displacement.x, displacement.y, displacement.z);
		
		Move(displacement.x, displacement.y, displacement.z);
	}
	
	// Continuously update the camera's velocity vector even if the camera
	// hasn't moved during this call. When the camera is no longer being moved
	// the camera is decelerating back to its stationary state.
	
	UpdateVelocity(elapsedTime);
	
	// Dopo aver aggiornato i dati della camera devo riazzerare la direzione
	_updateDirection.Fill(0.0f, 0.0f, 0.0f);
}

void AnimaCamera::Zoom(AFloat zoom, AFloat minZoom, AFloat maxZoom)
{
	if (_cameraType == ACT_ORBIT)
	{
		// Moves the camera closer to or further away from the orbit
		// target. The zoom amounts are in world units.
		
		_orbitMaxZoom = maxZoom;
		_orbitMinZoom = minZoom;
		
		AnimaVertex3f offset = GetPosition() - _target;
		
		_orbitOffsetDistance = offset.Length();
		offset.Normalize();
		_orbitOffsetDistance += zoom;
		_orbitOffsetDistance = std::min(std::max(_orbitOffsetDistance, minZoom), maxZoom);
		
		offset *= _orbitOffsetDistance;
		AnimaSceneObject::SetPosition(offset + _target);
		
		UpdateViewMatrix();
	}
	else
	{
		// For the other behaviors zoom is interpreted as changing the
		// horizontal field of view. The zoom amounts refer to the horizontal
		// field of view in degrees.
		
		zoom = std::min(std::max(zoom, minZoom), maxZoom);
		CalculateProjectionMatrix(zoom, _windowSize, _zNear, _zFar);
	}
}

void AnimaCamera::SetAcceleration(const AnimaVertex3f& acceleration)
{
	_acceleration = acceleration;
}

void AnimaCamera::SetCameraType(AnimaCameraType cameraType)
{
	// Switch to a new camera mode (i.e., behavior).
	// This method is complicated by the fact that it tries to save the current
	// behavior's state prior to making the switch to the new camera behavior.
	// Doing this allows seamless switching between camera behaviors.
	
	AnimaCameraType prevType = _cameraType;
	
	if (prevType == cameraType)
		return;
	
	_cameraType = cameraType;
	
	switch (cameraType)
	{
		case ACT_FIRST_PERSON:
		{
			switch (prevType)
			{
				default:
					break;
				
				case ACT_FLIGHT:
				case ACT_SPECTATOR:
				{
					AnimaVertex3f position = GetPosition();
					position.y = _firstPersonYOffset;
					AnimaSceneObject::SetPosition(position);
					
					UpdateViewMatrix();
					break;
				}
				case ACT_ORBIT:
				{
					AnimaVertex3f position = GetPosition();
					position.x = _savedPosition.x;
					position.z = _savedPosition.z;
					position.y = _firstPersonYOffset;
					AnimaSceneObject::SetPosition(position);
					
					_orientation = _savedOrientation;
					_accumPitchDegrees = _savedAccumPitchDegrees;
					
					UpdateViewMatrix();
					break;
				}
			}
			
			UndoRoll();
			break;
		}
		case ACT_SPECTATOR:
		{
			switch (prevType)
			{
				default:
					break;
				
				case ACT_FLIGHT:
					UpdateViewMatrix();
					break;
				
				case ACT_ORBIT:
					AnimaSceneObject::SetPosition(_savedPosition);
					_orientation = _savedOrientation;
					_accumPitchDegrees = _savedAccumPitchDegrees;
					
					UpdateViewMatrix();
					break;
			}
			
			UndoRoll();
			break;
		}
		case ACT_FLIGHT:
		{
			if (prevType == ACT_ORBIT)
			{
				AnimaSceneObject::SetPosition(_savedPosition);
				_orientation = _savedOrientation;
				_accumPitchDegrees = _savedAccumPitchDegrees;

				UpdateViewMatrix();
			}
			else
			{
				_savedPosition = GetPosition();
				
				UpdateViewMatrix();
			}
			break;
		}
		case ACT_ORBIT:
		{
			if (prevType == ACT_FIRST_PERSON)
				_firstPersonYOffset = GetPosition().y;
			
			_savedPosition = GetPosition();
			_savedOrientation = _orientation;
			_savedAccumPitchDegrees = _accumPitchDegrees;
			
			_targetYAxis = _yAxis;
			
			AnimaVertex3f newPosition = GetPosition() + _zAxis * _orbitOffsetDistance;
			AnimaVertex3f newTarget = GetPosition();
			
			LookAt(newPosition, newTarget, _targetYAxis);
			break;
		}
	}
}

void AnimaCamera::SetCurrentVelocity(const AnimaVertex3f& currentVelocity)
{
	_currentVelocity = currentVelocity;
}

void AnimaCamera::SetCurrentVelocity(AFloat x, AFloat y, AFloat z)
{
	_currentVelocity.Fill(x, y, z);
}

void AnimaCamera::SetOrbitMaxZoom(AFloat orbitMaxZoom)
{
	_orbitMaxZoom = orbitMaxZoom;
}

void AnimaCamera::SetOrbitMinZoom(AFloat orbitMinZoom)
{
	_orbitMinZoom = orbitMinZoom;
}

void AnimaCamera::SetOrbitOffsetDistance(AFloat orbitOffsetDistance)
{
	_orbitOffsetDistance = orbitOffsetDistance;
}

void AnimaCamera::SetOrientation(const AnimaQuaternion& orientation)
{
	AnimaMatrix m = orientation.GetMatrix();
	
	// Store the pitch for this new orientation.
	// First person and spectator behaviors limit pitching to
	// 90 degrees straight up and down.
	
	_accumPitchDegrees = AnimaMath::RadToDeg(asinf(m.vecM[1][2]));
	
	// First person and spectator behaviors don't allow rolling.
	// Negate any rolling that might be encoded in the new orientation.
	
	_orientation = orientation;
	
	if (_cameraType == ACT_FIRST_PERSON || _cameraType == ACT_SPECTATOR)
		LookAt(GetPosition(), GetPosition() + _viewDirection, WORLD_YAXIS);
	
	UpdateViewMatrix();
}

void AnimaCamera::SetPosition(const AnimaVertex3f& position)
{
	AnimaSceneObject::SetPosition(position);
	UpdateViewMatrix();
}

void AnimaCamera::SetPosition(AFloat x, AFloat y, AFloat z)
{
	SetPosition(AnimaVertex3f(x, y, z));
}

void AnimaCamera::SetPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting)
{
	// Determines the behavior of Y axis rotations when the camera is
	// orbiting a target. When preferTargetYAxisOrbiting is true all
	// Y axis rotations are about the orbit target's local Y axis.
	// When preferTargetYAxisOrbiting is false then the camera's
	// local Y axis is used instead.
	
	_preferTargetYAxisOrbiting = preferTargetYAxisOrbiting;
	
	if (_preferTargetYAxisOrbiting)
		UndoRoll();
}

void AnimaCamera::SetRotationSpeed(float rotationSpeed)
{
	// This is just an arbitrary value used to scale rotations
	// when rotateSmoothly() is called.
	_rotationSpeed = rotationSpeed;
}

void AnimaCamera::SetMaximumVelocity(const AnimaVertex3f& maximumVelocity)
{
	_maximumVelocity = maximumVelocity;
}

void AnimaCamera::SetMaximumVelocity(AFloat x, AFloat y, AFloat z)
{
	_maximumVelocity.Fill(x, y, z);
}

void AnimaCamera::RotateFirstPerson(AFloat headingDegrees, AFloat pitchDegrees)
{
	// Implements the rotation logic for the first person style and
	// spectator style camera behaviors. Roll is ignored.
	
	_accumPitchDegrees += pitchDegrees;
	
	if (_accumPitchDegrees > 90.0f)
	{
		pitchDegrees = 90.0f - (_accumPitchDegrees - pitchDegrees);
		_accumPitchDegrees = 90.0f;
	}
	
	if (_accumPitchDegrees < -90.0f)
	{
		pitchDegrees = -90.0f - (_accumPitchDegrees - pitchDegrees);
		_accumPitchDegrees = -90.0f;
	}
	
	AnimaQuaternion rot;
	
	// Rotate camera about the world y axis.
	// Note the order the quaternions are multiplied. That is important!
	if (headingDegrees != 0.0f)
	{
		rot.FromAxisAndAngleDeg(WORLD_YAXIS, headingDegrees);
		_orientation = rot * _orientation;
	}
	
	// Rotate camera about its local x axis.
	// Note the order the quaternions are multiplied. That is important!
	if (pitchDegrees != 0.0f)
	{
		rot.FromAxisAndAngleDeg(WORLD_XAXIS, pitchDegrees);
		_orientation = _orientation * rot;
	}
}

void AnimaCamera::RotateFlight(AFloat headingDegrees, AFloat pitchDegrees, AFloat rollDegrees)
{
	// Implements the rotation logic for the flight style camera behavior.
	
	_accumPitchDegrees += pitchDegrees;
	
	if (_accumPitchDegrees > 360.0f)
		_accumPitchDegrees -= 360.0f;
	
	if (_accumPitchDegrees < -360.0f)
		_accumPitchDegrees += 360.0f;
	
	AnimaQuaternion rot;
	
	rot.FromHeadPitchRollDeg(headingDegrees, pitchDegrees, rollDegrees);
	_orientation *= rot;
}

void AnimaCamera::RotateOrbit(AFloat headingDegrees, AFloat pitchDegrees, AFloat rollDegrees)
{
	// Implements the rotation logic for the orbit style camera behavior.
	// Roll is ignored for target Y axis orbiting.
	//
	// Briefly here's how this orbit camera implementation works. Switching to
	// the orbit camera behavior via the setBehavior() method will set the
	// camera's orientation to match the orbit target's orientation. Calls to
	// rotateOrbit() will rotate this orientation. To turn this into a third
	// person style view the updateViewMatrix() method will move the camera
	// position back 'm_orbitOffsetDistance' world units along the camera's
	// local z axis from the orbit target's world position.
	
	AnimaQuaternion rot;
	
	if (_preferTargetYAxisOrbiting)
	{
		if (headingDegrees != 0.0f)
		{
			rot.FromAxisAndAngleDeg(_targetYAxis, headingDegrees);
			_orientation = rot * _orientation;
		}
		
		if (pitchDegrees != 0.0f)
		{
			rot.FromAxisAndAngleDeg(WORLD_XAXIS, pitchDegrees);
			_orientation = _orientation * rot;
		}
	}
	else
	{
		rot.FromHeadPitchRollDeg(headingDegrees, pitchDegrees, rollDegrees);
		_orientation *= rot;
	}
}

void AnimaCamera::UpdateVelocity(AFloat elapsedTime)
{
	// Updates the camera's velocity based on the supplied movement direction
	// and the elapsed time (since this method was last called). The movement
	// direction is in the range [-1,1].
	
	if (_updateDirection.x != 0.0f)
	{
		// Camera is moving along the x axis.
		// Linearly accelerate up to the camera's max speed.
		
		_currentVelocity.x += _updateDirection.x * _acceleration.x * elapsedTime;
		
		if (_currentVelocity.x > _maximumVelocity.x)
			_currentVelocity.x = _maximumVelocity.x;
		else if (_currentVelocity.x < -_maximumVelocity.x)
			_currentVelocity.x = -_maximumVelocity.x;
	}
	else
	{
		// Camera is no longer moving along the x axis.
		// Linearly decelerate back to stationary state.
		
		if (_currentVelocity.x > 0.0f)
		{
			if ((_currentVelocity.x -= _acceleration.x * elapsedTime) < 0.0f)
				_currentVelocity.x = 0.0f;
		}
		else
		{
			if ((_currentVelocity.x += _acceleration.x * elapsedTime) > 0.0f)
				_currentVelocity.x = 0.0f;
		}
	}
	
	if (_updateDirection.y != 0.0f)
	{
		// Camera is moving along the y axis.
		// Linearly accelerate up to the camera's max speed.
		
		_currentVelocity.y += _updateDirection.y * _acceleration.y * elapsedTime;
		
		if (_currentVelocity.y > _maximumVelocity.y)
			_currentVelocity.y = _maximumVelocity.y;
		else if (_currentVelocity.y < -_maximumVelocity.y)
			_currentVelocity.y = -_maximumVelocity.y;
	}
	else
	{
		// Camera is no longer moving along the y axis.
		// Linearly decelerate back to stationary state.
		
		if (_currentVelocity.y > 0.0f)
		{
			if ((_currentVelocity.y -= _acceleration.y * elapsedTime) < 0.0f)
				_currentVelocity.y = 0.0f;
		}
		else
		{
			if ((_currentVelocity.y += _acceleration.y * elapsedTime) > 0.0f)
				_currentVelocity.y = 0.0f;
		}
	}
	
	if (_updateDirection.z != 0.0f)
	{
		// Camera is moving along the z axis.
		// Linearly accelerate up to the camera's max speed.
		
		_currentVelocity.z += _updateDirection.z * _acceleration.z * elapsedTime;
		
		if (_currentVelocity.z > _maximumVelocity.z)
			_currentVelocity.z = _maximumVelocity.z;
		else if (_currentVelocity.z < -_maximumVelocity.z)
			_currentVelocity.z = -_maximumVelocity.z;
	}
	else
	{
		// Camera is no longer moving along the z axis.
		// Linearly decelerate back to stationary state.
		
		if (_currentVelocity.z > 0.0f)
		{
			if ((_currentVelocity.z -= _acceleration.z * elapsedTime) < 0.0f)
				_currentVelocity.z = 0.0f;
		}
		else
		{
			if ((_currentVelocity.z += _acceleration.z * elapsedTime) > 0.0f)
				_currentVelocity.z = 0.0f;
		}
	}
}

void AnimaCamera::UpdateViewMatrix()
{
	// Reconstruct the view matrix.
	AnimaMatrix viewMatrix = _orientation.GetMatrix();
	
	_xAxis.Fill(viewMatrix.vecM[0][0], viewMatrix.vecM[1][0], viewMatrix.vecM[2][0]);
	_yAxis.Fill(viewMatrix.vecM[0][1], viewMatrix.vecM[1][1], viewMatrix.vecM[2][1]);
	_zAxis.Fill(viewMatrix.vecM[0][2], viewMatrix.vecM[1][2], viewMatrix.vecM[2][2]);
	_viewDirection = -_zAxis;
	
	if (_cameraType == ACT_ORBIT)
	{
		// Calculate the new camera position based on the current
		// orientation. The camera must always maintain the same
		// distance from the target. Use the current offset vector
		// to determine the correct distance from the target.
		
		AnimaSceneObject::SetPosition(_target + _zAxis * _orbitOffsetDistance);
	}
	
	AnimaVertex3f position = GetPosition();
	
	viewMatrix.vecM[3][0] = -(_xAxis * position);
	viewMatrix.vecM[3][1] = -(_yAxis * position);
	viewMatrix.vecM[3][2] = -(_zAxis * position);
	
	AnimaMatrix projectionViewMatrix = GetProjectionMatrix() * viewMatrix;
	_frustum.ComputeFrustum(projectionViewMatrix);
	
	SetMatrix("ViewMatrix", viewMatrix);
	SetMatrix("ProjectionViewMatrix", projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", projectionViewMatrix.Inversed());
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

void AnimaCamera::CalculateProjectionMatrix(AFloat fov, const AnimaVertex2f& size, AFloat zNear, AFloat zFar)
{
	_projectionType = ACPT_PERSPECTIVE;
	
	if (_windowSize != size)
		_windowSize = size;

	_zNear = zNear;
	_zFar = zFar;
	_fovy = fov;
	_aspectRatio = size.x / size.y;

	AnimaMatrix projectionMatrix = AnimaMatrix::MakePerspective(_fovy, _aspectRatio, zNear, zFar);
	AnimaMatrix projectionViewMatrix = projectionMatrix * GetViewMatrix();

	_frustum.ComputeFrustum(projectionViewMatrix);

	SetMatrix("ProjectionMatrix", projectionMatrix);
	SetMatrix("ProjectionViewMatrix", projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", projectionViewMatrix.Inversed());
}

void AnimaCamera::CalculateProjectionMatrix(AFloat left, AFloat right, AFloat bottom, AFloat top, AFloat zNear, AFloat zFar)
{
	_projectionType = ACPT_ORTHOGRAPHIC;
	
	_zNear = zNear;
	_zFar = zFar;
	_top = top;
	_bottom = bottom;
	_left = left;
	_right = right;
	_aspectRatio = 1.0f;
	
	AnimaMatrix projectionMatrix = AnimaMatrix::MakeOrtho(_left, _right, _bottom, _top, _zNear, _zFar);
	AnimaMatrix projectionViewMatrix = projectionMatrix * GetViewMatrix();
	
	_frustum.ComputeFrustum(projectionViewMatrix);
	
	SetMatrix("ProjectionMatrix", projectionMatrix);
	SetMatrix("ProjectionViewMatrix", projectionViewMatrix);
	SetMatrix("InverseProjectionViewMatrix", projectionViewMatrix.Inversed());
}

AnimaMatrix AnimaCamera::GetProjectionMatrix() const
{
	return GetMatrix("ProjectionMatrix");
}

AnimaMatrix AnimaCamera::GetViewMatrix() const
{
	return GetMatrix("ViewMatrix");
}

AnimaMatrix AnimaCamera::GetProjectionViewMatrix() const
{
	return GetMatrix("ProjectionViewMatrix");
}

AnimaMatrix AnimaCamera::GetInversedProjectionViewMatrix() const
{
	return GetMatrix("InverseProjectionViewMatrix");
}

bool AnimaCamera::IsPerspectiveProjectionType()
{
	return _projectionType == ACPT_PERSPECTIVE;
}

bool AnimaCamera::IsOrthographicProjectionType()
{
	return _projectionType == ACPT_ORTHOGRAPHIC;
}

AnimaVertex2f AnimaCamera::GetWindowSize()
{
	return _windowSize;
}

AnimaFrustum* AnimaCamera::GetFrustum()
{
	return &_frustum;
}

AnimaVertex2f AnimaCamera::WorldPointToScreenPoint(const AnimaVertex3f& worldPoint) const
{
	AnimaVertex3f point = GetProjectionViewMatrix() * worldPoint;
	AFloat screenX = (roundf(point.x + 1.0f) / 2.0f) * _windowSize.x;
	AFloat screenY = (roundf(1.0f - point.y) / 2.0f) * _windowSize.y;
	
	return AnimaVertex2f(screenX, screenY);
}

AnimaVertex3f AnimaCamera::ScreenPointToWorldPoint(const AnimaVertex2f& screenPoint) const
{
	AnimaVertex4f viewport(0.0f, 0.0f, _windowSize.x, _windowSize.y);

	AnimaVertex4f tmp(screenPoint, 1.0f, 1.0f);
	tmp.x = (tmp.x - viewport.x) / viewport.z;
	tmp.y = (tmp.y - viewport.y) / viewport.w;
	tmp = (tmp * 2.0f) - 1.0f;
	
	AnimaVertex4f obj = GetInversedProjectionViewMatrix() * tmp;
	obj /= obj.w;
	
	return AnimaVertex3f(obj);
}

END_ANIMA_ENGINE_NAMESPACE
