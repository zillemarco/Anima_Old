//
//  AnimaThirdPersonCamera.h
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
#include "AnimaCamera.h"
#include "AnimaAllocators.h"
#include "AnimaQuaternion.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaThirdPersonCamera : public AnimaCamera
{
	DECLARE_ANIMA_CLASS(AnimaThirdPersonCamera);

public:
	AnimaThirdPersonCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaThirdPersonCamera(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name);
	AnimaThirdPersonCamera(AnimaAllocator* allocator, AnimaCamerasManager* camerasManager, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name, const AnimaVertex3f& position, const AnimaVertex3f& target);
	AnimaThirdPersonCamera(const AnimaThirdPersonCamera& src);
	AnimaThirdPersonCamera(AnimaThirdPersonCamera&& src);
	~AnimaThirdPersonCamera();

	AnimaThirdPersonCamera& operator=(const AnimaThirdPersonCamera& src);
	AnimaThirdPersonCamera& operator=(AnimaThirdPersonCamera&& src);

public:
	AnimaVertex3f GetTarget();

	void CalculateViewMatrix() override;

	AFloat GetDistance();
	void SetDistance(AFloat dist);
	
public:
	void Zoom(AFloat amount) override;
	void Move(const AnimaVertex3f& direction, AFloat amount) override;
	void Move(const AFloat& xDirection, const AFloat& yDirection, const AFloat& zDirection, AFloat amount) override;
	void RotateX(AFloat angle) override;
	void RotateXDeg(AFloat angle) override;
	void RotateY(AFloat angle) override;
	void RotateYDeg(AFloat angle) override;

	void LookAt(const AnimaVertex3f& position, const AnimaVertex3f& target) override;
	void LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xTarget, AFloat yTarget, AFloat zTarget) override;

protected:
	AnimaVertex3f _target;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaABCamera__) */
