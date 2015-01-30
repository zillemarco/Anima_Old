//
//  AnimaMaterial.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMaterial.h"
#include "AnimaDataGeneratorsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMaterial::AnimaMaterial(AnimaEngine* engine)
	: AnimaMappedValues(engine)
{
}

AnimaMaterial::AnimaMaterial(const AnimaMaterial& src)
	: AnimaMappedValues(src)
{
}

AnimaMaterial::AnimaMaterial(AnimaMaterial&& src)
	: AnimaMappedValues(src)
{
}

AnimaMaterial::~AnimaMaterial()
{
}

AnimaMaterial& AnimaMaterial::operator=(const AnimaMaterial& src)
{
	AnimaMappedValues::operator=(src);
	return *this;
}

AnimaMaterial& AnimaMaterial::operator=(AnimaMaterial&& src)
{
	AnimaMappedValues::operator=(src);
	return *this;
}

END_ANIMA_ENGINE_NAMESPACE
