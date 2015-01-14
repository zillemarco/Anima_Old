#pragma once

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaQuaternion : public AnimaVertex4f
{
public:
	AnimaQuaternion(AnimaEngine* engine);
	AnimaQuaternion(AnimaEngine* engine, AFloat data[4]);
	AnimaQuaternion(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w);
	AnimaQuaternion(AnimaEngine* engine, const AnimaVertex3f& axis, AFloat angle);
	AnimaQuaternion(const AnimaQuaternion& src);
	AnimaQuaternion(AnimaQuaternion&& src);
	~AnimaQuaternion();

public:
	AnimaQuaternion Conjugate();
	
	AnimaMatrix GetMatrix();
};

END_ANIMA_ENGINE_NAMESPACE