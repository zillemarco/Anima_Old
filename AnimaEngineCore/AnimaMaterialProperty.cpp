//
//  AnimaMaterialProperty.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMaterialProperty.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaMaterialProperty::AnimaMaterialProperty(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	
	_propertyName = nullptr;
	_propertyValue = nullptr;
}

AnimaMaterialProperty::AnimaMaterialProperty(const AnimaMaterialProperty& src)
{
	
}

AnimaMaterialProperty::AnimaMaterialProperty(AnimaMaterialProperty&& src)
{
	
}

AnimaMaterialProperty::~AnimaMaterialProperty()
{
	
}

AnimaMaterialProperty& AnimaMaterialProperty::operator=(const AnimaMaterialProperty& src)
{
	
}

AnimaMaterialProperty& AnimaMaterialProperty::operator=(AnimaMaterialProperty&& src)
{
	
}

END_ANIMA_ENGINE_CORE_NAMESPACE