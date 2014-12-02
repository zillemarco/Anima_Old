//
//  AnimaMaterialProperty.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMaterialProperty__
#define __Anima__AnimaMaterialProperty__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMaterialProperty
{
public:
	AnimaMaterialProperty(AnimaEngine* engine);
	AnimaMaterialProperty(const AnimaMaterialProperty& src);
	AnimaMaterialProperty(AnimaMaterialProperty&& src);
	~AnimaMaterialProperty();
	
	AnimaMaterialProperty& operator=(const AnimaMaterialProperty& src);
	AnimaMaterialProperty& operator=(AnimaMaterialProperty&& src);
	
private:
	char* _propertyName;
	void* _propertyValue;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterialProperty__) */
