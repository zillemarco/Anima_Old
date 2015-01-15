//
//  AnimaFPCamera.h
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

class ANIMA_ENGINE_EXPORT AnimaFPCamera : public AnimaCamera
{
public:
	AnimaFPCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager);
	AnimaFPCamera(AnimaEngine* engine);
	AnimaFPCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& forward, const AnimaVertex3f& up);
	AnimaFPCamera(const AnimaFPCamera& src);
	AnimaFPCamera(AnimaFPCamera&& src);
	~AnimaFPCamera();

	AnimaFPCamera& operator=(const AnimaFPCamera& src);
	AnimaFPCamera& operator=(AnimaFPCamera&& src);

public:
	AnimaVertex3f GetForward() override;
	AnimaVertex3f GetLeft() override;
	AnimaVertex3f GetRight() override;

public:
	void Zoom(AFloat amount) override;
	void Move(const AnimaVertex3f& direction, AFloat amount) override;
	void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) override;
	void RotateX(AFloat angle) override;
	void RotateXDeg(AFloat angle) override;
	void RotateY(AFloat angle) override;
	void RotateYDeg(AFloat angle) override;
	AnimaMatrix GetViewMatrix() override;

protected:
	AnimaVertex3f _forward;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFPCamera__) */
