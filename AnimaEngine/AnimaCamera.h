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

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaCamerasManager;

class ANIMA_ENGINE_EXPORT AnimaCamera
{
public:
	enum AnimaCameraProjectionType {
		PERSPECTIVE,
		ORTHO
	};

protected:
	AnimaCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager);
	AnimaCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position);
	
public:
	AnimaCamera(const AnimaCamera& src);
	AnimaCamera(AnimaCamera&& src);
	virtual ~AnimaCamera();

	AnimaCamera& operator=(const AnimaCamera& src);
	AnimaCamera& operator=(AnimaCamera&& src);

public:
	AnimaVertex3f GetPosition();
	virtual void SetPosition(const AnimaVertex3f& position);
	virtual void SetPosition(const AFloat& x, const AFloat& y, const AFloat& z);
	
	virtual AnimaVertex3f GetUp();
	virtual AnimaVertex3f GetForward();
	virtual AnimaVertex3f GetLeft();
	virtual AnimaVertex3f GetRight();
	
	virtual bool IsActive();

public:
	virtual void Zoom(AFloat amount) = 0;
	virtual void Move(const AnimaVertex3f& direction, AFloat amount) = 0;
	virtual void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) = 0;
	virtual void RotateX(AFloat angle) = 0;
	virtual void RotateXDeg(AFloat angle) = 0;
	virtual void RotateY(AFloat angle) = 0;
	virtual void RotateYDeg(AFloat angle) = 0;

	virtual void LookAt(const AnimaVertex3f& position, const AnimaVertex3f& value) = 0;
	virtual void LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xValue, AFloat yValue, AFloat zValue) = 0;

	virtual void CalculateViewMatrix() = 0;
	virtual void SetViewMatrix(const AnimaMatrix& matrix);
	virtual AnimaMatrix GetViewMatrix();

	virtual void CalculateProjectionMatrix(float fov, const AnimaVertex2f& size, float zNear, float zFar);
	virtual void CalculateProjectionMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
	virtual void SetProjectionMatrix(const AnimaMatrix& matrix);
	virtual AnimaMatrix GetProjectionMatrix();
	
	virtual void Activate();
	virtual void Deactivate();

	bool IsPerspectiveProjectionType();
	bool IsOrthoProjectionType();

	AnimaVertex2f GetWindowSize();

protected:
	AnimaVertex3f _position;

	AnimaVertex3f _xAxis;
	AnimaVertex3f _yAxis;
	AnimaVertex3f _zAxis;

	AnimaVertex3f _worldXAxis;
	AnimaVertex3f _worldYAxis;
	AnimaVertex3f _worldZAxis;

	AnimaVertex2f _windowSize;
	
	bool _active;
	AnimaCameraProjectionType _projectionType;

	AnimaMatrix _viewMatrix;
	AnimaMatrix _projectionMatrix;

	AnimaAllocator* _allocator;	
	AnimaCamerasManager* _camerasManager;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamera__) */
