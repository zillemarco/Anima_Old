//
//  AnimaABCamera.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaABCamera__
#define __Anima__AnimaABCamera__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaCamera.h"
#include "AnimaAllocators.h"
#include "AnimaQuaternion.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaABCamera : public AnimaCamera
{
public:
	AnimaABCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager);
	AnimaABCamera(AnimaEngine* engine);
	AnimaABCamera(AnimaEngine* engine, AnimaCamerasManager* camerasManager, const AnimaVertex3f& position, const AnimaVertex3f& target, const AnimaVertex3f& up);
	AnimaABCamera(const AnimaABCamera& src);
	AnimaABCamera(AnimaABCamera&& src);
	~AnimaABCamera();

	AnimaABCamera& operator=(const AnimaABCamera& src);
	AnimaABCamera& operator=(AnimaABCamera&& src);

public:
	AnimaVertex3f GetForward() override;
	AnimaVertex3f GetLeft() override;
	AnimaVertex3f GetRight() override;
	AnimaVertex3f GetTarget();

	AnimaMatrix GetViewMatrix() override;

	AFloat GetDistance();
	void SetDistance(AFloat dist);

	void RecalculateVectors();

	void CalculateViewMatrix();

public:
	void Zoom(AFloat amount) override;
	void Move(const AnimaVertex3f& direction, AFloat amount) override;
	void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) override;
	void RotateX(AFloat angle) override;
	void RotateXDeg(AFloat angle) override;
	void RotateY(AFloat angle) override;
	void RotateYDeg(AFloat angle) override;

	void LookAt(const AnimaVertex3f& position, const AnimaVertex3f& target);

protected:
	AnimaVertex3f _target;

	AnimaMatrix _viewMatrix;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaABCamera__) */
