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
	, _shadersNames(allocator)
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
	, _shadersNames(src._shadersNames)
{
}

AnimaMaterial::AnimaMaterial(AnimaMaterial&& src)
	: AnimaMappedValues(src)
	, _shadersNames(src._shadersNames)
{
}

AnimaMaterial::~AnimaMaterial()
{
}

AnimaMaterial& AnimaMaterial::operator=(const AnimaMaterial& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
		_shadersNames.Copy(src._shadersNames);
	}
	return *this;
}

AnimaMaterial& AnimaMaterial::operator=(AnimaMaterial&& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
		_shadersNames.Copy(src._shadersNames);
	}
	return *this;
}

void AnimaMaterial::AddShader(AnimaShader* shader)
{
	_shadersNames.Add(shader->GetAnimaName());
}

void AnimaMaterial::AddShader(const AnimaString& shaderName)
{
	_shadersNames.Add(shaderName);
}

void AnimaMaterial::AddShader(const char* shaderName)
{
	_shadersNames.Add(AnimaString(shaderName, nullptr));
}

AInt AnimaMaterial::GetShadersCount() const
{
	return _shadersNames.GetSize();
}

AnimaString AnimaMaterial::GetShaderName(AInt index) const
{
	return _shadersNames[index];
}

END_ANIMA_ENGINE_NAMESPACE
