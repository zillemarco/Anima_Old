#pragma once

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaQuaternion : public AnimaVertex4f
{
public:
	AnimaQuaternion(AnimaEngine* engine);
	AnimaQuaternion(AnimaEngine* engine, AFloat data[4]);
	AnimaQuaternion(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w);
	AnimaQuaternion(const AnimaQuaternion& src);
	AnimaQuaternion(AnimaQuaternion&& src);
	~AnimaQuaternion();

public:
	AnimaQuaternion Conjugate();
};

END_ANIMA_ENGINE_NAMESPACE