//
//  AnimaMath.h
//  Anima
//
//  Created by Marco Zille on 17/12/14.
//
//

#ifndef __Anima__AnimaMath__
#define __Anima__AnimaMath__

#include "AnimaEngineCore.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"
#include "AnimaQuaternion.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMath
{
public:
	static AnimaQuaternion QuaternionMulQuaternion(const AnimaQuaternion& q1, const AnimaQuaternion& q2);
	static AnimaQuaternion QuaternionMulVector(const AnimaQuaternion& q, const AnimaVertex3f& v);

	static AnimaVertex3f RotateVector(const AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis);
	static void RotateVector(AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis);

	static bool PointInsideSphere(const AnimaVertex3f& point, const AnimaVertex3f& sphereCenter, const AFloat& sphereRadius);
	static bool PointInsideCone(const AnimaVertex3f& point, const AnimaVertex3f& coneTip, const AnimaVertex3f& coneDirection, float coneHeight, float coneAngle);
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMath__) */
