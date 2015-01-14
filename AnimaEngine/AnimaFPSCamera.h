//
//  AnimaFPSCamera.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFPSCamera__
#define __Anima__AnimaFPSCamera__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaCamera.h"
#include "AnimaAllocators.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFPSCamera : public AnimaCamera
{
public:
	AnimaFPSCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager);
	AnimaFPSCamera(AnimaEngine* engine);
	AnimaFPSCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up);
	AnimaFPSCamera(const AnimaFPSCamera& src);
	AnimaFPSCamera(AnimaFPSCamera&& src);
	~AnimaFPSCamera();

	AnimaFPSCamera& operator=(const AnimaFPSCamera& src);
	AnimaFPSCamera& operator=(AnimaFPSCamera&& src);

public:
	AnimaVertex3f GetForward() override;
	AnimaVertex3f GetLeft() override;
	AnimaVertex3f GetRight() override;

public:
	void Move(const AnimaVertex3f& direction, AFloat amount) override;
	void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) override;
	void RotateX(AFloat angle) override;
	void RotateXDeg(AFloat angle) override;
	void RotateY(AFloat angle) override;
	void RotateYDeg(AFloat angle) override;

protected:
	AnimaVertex3f _forward;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFPSCamera__) */
