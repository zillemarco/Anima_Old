//
//
//  Anima
//
//  Created by Marco Zille on 06/10/15.
//
//

#ifndef __Anima__AnimaTools__
#define __Anima__AnimaTools__

#include <iostream>
#include "AnimaString.h"
#include "AnimaEngineCore.h"
#include "AnimaArray.h"
#include "AnimaTypes.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaTools
{
public:
	static AnimaString Base64Encode(const AnimaArray<AUchar>& source);
	static AnimaArray<AUchar> Base64Decode(const AnimaString& source);
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTools__) */