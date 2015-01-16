//
//  AnimaFirstPersonCamera.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFPCamera__
#define __Anima__AnimaFPCamera__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaCamera.h"
#include "AnimaAllocators.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFirstPersonCamera : public AnimaCamera
{
public:
	AnimaFirstPersonCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager);
	AnimaFirstPersonCamera(AnimaEngine* engine);
	AnimaFirstPersonCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up);
	AnimaFirstPersonCamera(const AnimaFirstPersonCamera& src);
	AnimaFirstPersonCamera(AnimaFirstPersonCamera&& src);
	~AnimaFirstPersonCamera();

	AnimaFirstPersonCamera& operator=(const AnimaFirstPersonCamera& src);
	AnimaFirstPersonCamera& operator=(AnimaFirstPersonCamera&& src);
	
public:
	void Zoom(AFloat amount) override;
	void Move(const AnimaVertex3f& direction, AFloat amount) override;
	void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) override;
	void RotateX(AFloat angle) override;
	void RotateXDeg(AFloat angle) override;
	void RotateY(AFloat angle) override;
	void RotateYDeg(AFloat angle) override;
	
	void CalculateViewMatrix() override;

	void LookAt(const AnimaVertex3f& position, const AnimaVertex3f& forward) override;
	void LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xForward, AFloat yForward, AFloat zForward) override;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFPCamera__) */
