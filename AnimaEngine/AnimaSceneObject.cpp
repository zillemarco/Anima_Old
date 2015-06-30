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
	, _children(allocator)
{
	_parentObject = nullptr;
	_transformation.SetParentObject(this);
}

AnimaSceneObject::AnimaSceneObject(const AnimaSceneObject& src)
	: AnimaMappedValues(src)
	, _children(src._children)
{
	_parentObject = src._parentObject;
}

AnimaSceneObject::AnimaSceneObject(AnimaSceneObject&& src)
	: AnimaMappedValues(src)
	, _children(src._children)
{
	_parentObject = src._parentObject;
}

AnimaSceneObject::~AnimaSceneObject()
{
	_children.RemoveAll();
}

AnimaSceneObject& AnimaSceneObject::operator=(const AnimaSceneObject& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
		_children = src._children;
		_parentObject = src._parentObject;
	}

	return *this;
}

AnimaSceneObject& AnimaSceneObject::operator=(AnimaSceneObject&& src)
{
	if (this != &src)
	{
		AnimaMappedValues::operator=(src);
		_children = src._children;
		_parentObject = src._parentObject;
	}

	return *this;
}

void AnimaSceneObject::SetPosition(const AnimaVertex3f& position)
{
	AnimaMappedValues::SetVector("Position", position);
	_transformation.Translate(position);
}

void AnimaSceneObject::SetPosition(AFloat x, AFloat y, AFloat z)
{
	AnimaMappedValues::SetVector("Position", x, y, z);
	_transformation.Translate(x, y, z);
}

void AnimaSceneObject::SetParentObject(AnimaSceneObject* parentObject)
{
	_parentObject = parentObject;
}

AnimaSceneObject* AnimaSceneObject::GetParentObject() const
{
	return _parentObject;
}

AnimaSceneObject* AnimaSceneObject::GetAncestorObject()
{
	AnimaSceneObject* ancestor = this;
	while (ancestor->GetParentObject() != nullptr)
		ancestor = ancestor->GetParentObject();

	return ancestor;
}

AnimaVertex3f AnimaSceneObject::GetPosition()
{
	return AnimaMappedValues::GetVector3f("Position");
}

AInt AnimaSceneObject::AddChild(AnimaSceneObject* child)
{
	child->SetParentObject(this);
	return _children.Add(child->GetAnimaName(), child);
}

AInt AnimaSceneObject::GetChildrenNumber()
{
	return _children.GetSize();
}

AnimaSceneObject* AnimaSceneObject::GetChild(const AnimaString& name)
{
	return _children.GetWithName(name);
}

AnimaSceneObject* AnimaSceneObject::GetChild(const char* name)
{
	return _children.GetWithName(name);
}

AnimaSceneObject* AnimaSceneObject::GetChild(AInt index)
{
	return _children.Get(index);
}

bool AnimaSceneObject::RemoveChild(AnimaSceneObject* object)
{
	return RemoveChild(object->GetAnimaName());
}

bool AnimaSceneObject::RemoveChild(const AnimaString& name)
{
	return _children.Remove(name);
}

bool AnimaSceneObject::RemoveChild(const char* name)
{
	AnimaString str = name;
	return RemoveChild(str);
}

bool AnimaSceneObject::RemoveChild(AInt index)
{
	return _children.Remove(index);
}

AInt AnimaSceneObject::HasChild(AnimaSceneObject* object)
{
	return _children.Contains(object->GetAnimaName());
}

AInt AnimaSceneObject::HasChild(const AnimaString& name)
{
	return _children.Contains(name);
}

AInt AnimaSceneObject::HasChild(const char* name)
{
	AnimaString str = name;
	return HasChild(str);
}

AnimaTransformation* AnimaSceneObject::GetTransformation()
{
	return &_transformation;
}

AnimaTransformation AnimaSceneObject::GetTransformationCopy()
{
	return _transformation;
}

void AnimaSceneObject::UpdateChildrenTransformation()
{
	AInt childrenCount = _children.GetSize();
	for (AInt i = 0; i < childrenCount; i++)
		_children[i]->GetTransformation()->UpdateMatrix();
}

END_ANIMA_ENGINE_NAMESPACE