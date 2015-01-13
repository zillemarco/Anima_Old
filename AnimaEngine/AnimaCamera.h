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

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaCamera
{
protected:
	AnimaCamera(AnimaEngine* engine);
	AnimaCamera(AnimaEngine* engine, const AnimaVertex3f& position, const AnimaVertex3f& up);
	AnimaCamera(const AnimaCamera& src);
	AnimaCamera(AnimaCamera&& src);

public:
	~AnimaCamera();

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

public:
	virtual void Move(const AnimaVertex3f& direction, AFloat amount) = 0;
	virtual void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) = 0;
	virtual void RotateX(AFloat angle) = 0;
	virtual void RotateXDeg(AFloat angle) = 0;
	virtual void RotateY(AFloat angle) = 0;
	virtual void RotateYDeg(AFloat angle) = 0;

protected:
	AnimaVertex3f _position;
	AnimaVertex3f _up;

	AnimaVertex3f _yAxis;

	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamera__) */
