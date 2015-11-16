//
//  AnimaCamera.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaCamera__
#define __Anima__AnimaCamera__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaMatrix.h"
#include "AnimaFrustum.h"
#include "AnimaSceneObject.h"
#include "AnimaQuaternion.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaCamerasManager;

enum AnimaCameraProjectionType {
	ACPT_PERSPECTIVE,
	ACPT_ORTHOGRAPHIC
};

enum AnimaCameraType {
	ACT_FIRST_PERSON,
	ACT_SPECTATOR,
	ACT_FLIGHT,
	ACT_ORBIT
};

class ANIMA_ENGINE_EXPORT AnimaCamera : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaCamera);

public:
	AnimaCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	
public:
	AnimaCamera(const AnimaCamera& src);
	AnimaCamera(AnimaCamera&& src);
	virtual ~AnimaCamera();

	AnimaCamera& operator=(const AnimaCamera& src);
	AnimaCamera& operator=(AnimaCamera&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;

public:
	void LookAt(const AnimaVertex3f& target);
	void LookAt(const AnimaVertex3f& position, const AnimaVertex3f& target, const AnimaVertex3f& up);
	void Move(AFloat dx, AFloat dt, AFloat dz);
	void Move(const AnimaVertex3f& direction, const AnimaVertex3f& amount);
	void RotateRad(AFloat head, AFloat pitch, AFloat roll);
	void RotateDeg(AFloat head, AFloat pitch, AFloat roll);
	void SmoothRotateRad(AFloat head, AFloat pitch, AFloat roll);
	void SmoothRotateDeg(AFloat head, AFloat pitch, AFloat roll);
	void UndoRoll();
	void UpdatePosition(AFloat elapsedTime);
	void Zoom(AFloat zoom, AFloat minZomm, AFloat maxZoom);
	
	void SetUpdateData(const AnimaVertex3f& direction) { _updateDirection = direction; }
	
	void SetAcceleration(const AnimaVertex3f &acceleration);
	const AnimaVertex3f& GetAcceleration() const;
	
	void SetCameraType(AnimaCameraType type);
	AnimaCameraType GetCameraType() const { return _cameraType; }
	
	void SetCurrentVelocity(const AnimaVertex3f &currentVelocity);
	void SetCurrentVelocity(AFloat x, AFloat y, AFloat z);
	const AnimaVertex3f& GetCurrentVelocity() const { return _currentVelocity; }
	
	void SetPosition(const AnimaVertex3f& position) override;
	void SetPosition(AFloat x, AFloat y, AFloat z) override;
	
	void SetOrbitMinZoom(AFloat orbitMinZoom);
	AFloat GetOrbitMinZoom() const { return _orbitMinZoom; }
	
	void SetOrbitMaxZoom(AFloat orbitMaxZoom);
	AFloat GetOrbitMaxZoom() const { return _orbitMaxZoom; }
	
	void SetOrbitOffsetDistance(AFloat orbitOffsetDistance);
	AFloat GetOrbitOffsetDistance() const { return _orbitOffsetDistance; }
	
	void SetOrbitPitchMaxDegrees(AFloat orbitPitchMaxDegrees);
	AFloat GetOrbitPitchMaxDegrees() const;
	
	void SetOrbitPitchMinDegrees(AFloat orbitPitchMinDegrees);
	AFloat GetOrbitPitchMinDegrees() const;
	
	void SetOrientation(const AnimaQuaternion& oriantation);
	const AnimaQuaternion& GetOrientation() const { return _orientation; }
	
	void SetRotationSpeed(AFloat rotationSpeed);
	AFloat GetRotationSpeed() const { return _rotationSpeed; }
	
	void SetMaximumVelocity(const AnimaVertex3f& maximumVelocity);
	void SetMaximumVelocity(AFloat x, AFloat y, AFloat z);
	const AnimaVertex3f& GetMaximumVelocity() const { return _maximumVelocity; }
	
	const AnimaVertex3f& GetViewDirection() const { return _viewDirection; }
	
	AnimaMatrix GetProjectionMatrix() const;
	AnimaMatrix GetViewMatrix() const;
	AnimaMatrix GetProjectionViewMatrix() const;
	AnimaMatrix GetInversedProjectionViewMatrix() const;
	
	const AnimaVertex3f& GetXAxis() const { return _xAxis; }
	const AnimaVertex3f& GetYAxis() const { return _yAxis; }
	const AnimaVertex3f& GetZAxis() const { return _zAxis; }
	
	void SetPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting);
	bool PreferTargetYAxisOrbiting() const { return _preferTargetYAxisOrbiting; }
	
	bool IsActive();
	
protected:
	void RotateFirstPerson(AFloat headingDegrees, AFloat pitchDegrees);
	void RotateFlight(AFloat headingDegrees, AFloat pitchDegrees, AFloat rollDegrees);
	void RotateOrbit(AFloat headingDegrees, AFloat pitchDegrees, AFloat rollDegrees);
	void UpdateVelocity(AFloat elapsedTime);
	void UpdateViewMatrix();

public:
	void CalculateProjectionMatrix(AFloat fov, const AnimaVertex2f& size, AFloat zNear, AFloat zFar);
	void CalculateProjectionMatrix(AFloat left, AFloat right, AFloat bottom, AFloat top, AFloat zNear, AFloat zFar);

	void Activate();
	void Deactivate();

	bool IsPerspectiveProjectionType();
	bool IsOrthographicProjectionType();

	AnimaVertex2f GetWindowSize();

	AnimaFrustum* GetFrustum();
	
	AnimaVertex2f WorldPointToScreenPoint(const AnimaVertex3f& worldPoint) const;
	AnimaVertex3f ScreenPointToWorldPoint(const AnimaVertex2f& screenPoint) const;
		
protected:
	AnimaCameraType _cameraType;
	AnimaCameraProjectionType _projectionType;
	
	bool _preferTargetYAxisOrbiting;
	AFloat _accumPitchDegrees;
	AFloat _savedAccumPitchDegrees;
	AFloat _rotationSpeed;
	AFloat _fovy;
	AFloat _aspectRatio;
	AFloat _zNear;
	AFloat _zFar;
	AFloat _orbitMinZoom;
	AFloat _orbitMaxZoom;
	AFloat _orbitOffsetDistance;
	AFloat _firstPersonYOffset;
	AnimaVertex3f _savedPosition;
	AnimaVertex3f _target;
	AnimaVertex3f _targetYAxis;
	AnimaVertex3f _xAxis;
	AnimaVertex3f _yAxis;
	AnimaVertex3f _zAxis;
	AnimaVertex3f _viewDirection;
	AnimaVertex3f _acceleration;
	AnimaVertex3f _currentVelocity;
	AnimaVertex3f _maximumVelocity;
	AnimaVertex3f _updateDirection;
	AnimaQuaternion _orientation;
	AnimaQuaternion _savedOrientation;
	AnimaMatrix _viewMatrix;
	AnimaMatrix _projMatrix;
	AnimaMatrix _viewProjMatrix;

	AFloat _top;
	AFloat _bottom;
	AFloat _left;
	AFloat _right;
	
	bool _active;
	AnimaFrustum _frustum;
	AnimaVertex2f _windowSize;
	AnimaCamerasManager* _camerasManager;
	
	static const AnimaVertex3f DEFAULT_ACCELERATION;
	static const AnimaVertex3f DEFAULT_MAXIMUM_VELOCITY;
	static const AFloat DEFAULT_ROTATION_SPEED;
	static const AFloat DEFAULT_FOVY;
	static const AFloat DEFAULT_ZNEAR;
	static const AFloat DEFAULT_ZFAR;
	static const AFloat DEFAULT_ORBIT_MIN_ZOOM;
	static const AFloat DEFAULT_ORBIT_MAX_ZOOM;
	static const AFloat DEFAULT_ORBIT_OFFSET_DISTANCE;
	static const AnimaVertex3f WORLD_XAXIS;
	static const AnimaVertex3f WORLD_YAXIS;
	static const AnimaVertex3f WORLD_ZAXIS;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamera__) */
