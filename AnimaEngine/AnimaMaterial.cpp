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
	SetInteger("FrontFace", GL_CCW);
	SetInteger("CullFace", GL_BACK);
	SetBoolean("Wireframe", false);
	SetColor("WireframeColor", 0.0f, 0.0f, 0.0f);
	SetFloat("MaxTessellationLevel", 1.0f);
	SetFloat("TessellationAlpha", 0.0f);
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
