//
//  AnimaMaterial.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMaterial__
#define __Anima__AnimaMaterial__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMappedValues.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterial : public AnimaMappedValues
{
public:
	AnimaMaterial(AnimaEngine* engine, const AnimaString& name);
	AnimaMaterial(const AnimaMaterial& src);
	AnimaMaterial(AnimaMaterial&& src);
	~AnimaMaterial();
	
	AnimaMaterial& operator=(const AnimaMaterial& src);
	AnimaMaterial& operator=(AnimaMaterial&& src);	
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterial__) */
