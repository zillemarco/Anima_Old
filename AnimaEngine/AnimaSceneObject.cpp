//
//  AnimaSceneObject.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaSceneObject.h"
#include "AnimaMath.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaSceneObject::AnimaSceneObject(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorsManager, const AnimaString& name)
: AnimaMappedValues(allocator, dataGeneratorsManager, name)
{
}

AnimaSceneObject::AnimaSceneObject(const AnimaSceneObject& src)
: AnimaMappedValues(src)
{
}

AnimaSceneObject::AnimaSceneObject(AnimaSceneObject&& src)
	: AnimaMappedValues(src)
{
}

AnimaSceneObject::~AnimaSceneObject()
{
}

AnimaSceneObject& AnimaSceneObject::operator=(const AnimaSceneObject& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
	}

	return *this;
}

AnimaSceneObject& AnimaSceneObject::operator=(AnimaSceneObject&& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
	}

	return *this;
}

END_ANIMA_ENGINE_NAMESPACE
