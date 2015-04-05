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

AnimaSceneObject::AnimaSceneObject(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
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

void AnimaSceneObject::SetPosition(const AnimaVertex3f& position)
{
	AnimaMappedValues::SetVector("Position", position);
	_transformation.SetInitialTransformationMatrix(AnimaMatrix::MakeTranslation(position));
}

void AnimaSceneObject::SetPosition(AFloat x, AFloat y, AFloat z)
{
	AnimaMappedValues::SetVector("Position", x, y, z);
	_transformation.SetInitialTransformationMatrix(AnimaMatrix::MakeTranslation(x, y, z));
}

AnimaVertex3f AnimaSceneObject::GetPosition()
{
	return AnimaMappedValues::GetVector3f("Position");
}

END_ANIMA_ENGINE_NAMESPACE
