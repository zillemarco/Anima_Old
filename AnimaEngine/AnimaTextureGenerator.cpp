//
//  AnimaTextureGenerator.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaTextureGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaTextureGenerator::AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaDataGenerator(name, allocator)
	, _texture(nullptr)
{
	IMPLEMENT_ANIMA_CLASS(AnimaTextureGenerator);
}

AnimaTextureGenerator::AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator, AnimaTexture* texture)
	: AnimaDataGenerator(name, allocator)
	, _texture(texture)
{
	IMPLEMENT_ANIMA_CLASS(AnimaTextureGenerator);
}

AnimaTextureGenerator::AnimaTextureGenerator(const AnimaTextureGenerator& src)
	: AnimaDataGenerator(src)
	, _texture(src._texture)
{
}

AnimaTextureGenerator::AnimaTextureGenerator(AnimaTextureGenerator&& src)
	: AnimaDataGenerator(src)
	, _texture(src._texture)
{
}

AnimaTextureGenerator::~AnimaTextureGenerator()
{
}

AnimaTextureGenerator& AnimaTextureGenerator::operator=(const AnimaTextureGenerator& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_texture = src._texture;
	}

	return *this;
}

AnimaTextureGenerator& AnimaTextureGenerator::operator=(AnimaTextureGenerator&& src)
{
	if (this != &src)
	{
		AnimaDataGenerator::operator=(src);

		_texture = src._texture;
	}

	return *this;
}

void AnimaTextureGenerator::UpdateValue()
{
}

void AnimaTextureGenerator::SetTexture(AnimaTexture* texture)
{
	_texture = texture;
}

AnimaTexture* AnimaTextureGenerator::GetTexture()
{
	return _texture;
}

END_ANIMA_ENGINE_NAMESPACE
