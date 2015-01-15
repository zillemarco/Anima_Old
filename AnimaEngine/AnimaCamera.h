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
#include "AnimaEngine.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaCamerasManager;

class ANIMA_ENGINE_EXPORT AnimaCamera
{
protected:
	AnimaCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager);
	AnimaCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& up);
	
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
	
	AnimaVertex3f GetUp();

	virtual AnimaVertex3f GetForward() = 0;
	virtual AnimaVertex3f GetLeft() = 0;
	virtual AnimaVertex3f GetRight() = 0;
	
	virtual bool IsActive();

public:
	virtual void Zoom(AFloat amount) = 0;
	virtual void Move(const AnimaVertex3f& direction, AFloat amount) = 0;
	virtual void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) = 0;
	virtual void RotateX(AFloat angle) = 0;
	virtual void RotateXDeg(AFloat angle) = 0;
	virtual void RotateY(AFloat angle) = 0;
	virtual void RotateYDeg(AFloat angle) = 0;
	virtual AnimaMatrix GetViewMatrix() = 0;
	
	virtual void Activate();
	virtual void Deactivate();

protected:
	AnimaVertex3f _position;
	AnimaVertex3f _up;

	AnimaVertex3f _xAxis;
	AnimaVertex3f _yAxis;
	AnimaVertex3f _zAxis;
	
	bool _active;

	AnimaEngine* _engine;	
	AnimaCamerasManager* _camerasManager;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamera__) */
