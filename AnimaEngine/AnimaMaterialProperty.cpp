//
//  AnimaMaterialProperty.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMaterialProperty.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

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
	return *this;
}

AnimaMaterialProperty& AnimaMaterialProperty::operator=(AnimaMaterialProperty&& src)
{
	return *this;
}

END_ANIMA_ENGINE_NAMESPACE
