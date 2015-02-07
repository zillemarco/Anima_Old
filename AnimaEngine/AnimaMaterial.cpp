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

AnimaMaterial::AnimaMaterial(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
	: AnimaMappedValues(allocator, dataGeneratorManager, name)
{
	SetInteger("frontFace", GL_CCW);
	SetInteger("cullFace", GL_BACK);
	SetBoolean("wireframe", false);
	SetColor("wireframeColor", 0.0f, 0.0f, 0.0f);
	SetFloat("maxTessellationLevel", 1.0f);
	SetFloat("tessellationAlpha", 0.0f);
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
