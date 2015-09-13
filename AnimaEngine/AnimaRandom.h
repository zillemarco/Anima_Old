//
//  AnimaRandom.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaRandom__
#define __Anima__AnimaRandom__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include <random>

BEGIN_ANIMA_ENGINE_NAMESPACE

namespace AnimaRandom
{
	ANIMA_ENGINE_EXPORT AInt GetRandomInt(const AInt& from, const AInt& to);
	ANIMA_ENGINE_EXPORT AFloat GetRandomFloat(const AFloat& from, const AFloat& to);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaRandom__) */
