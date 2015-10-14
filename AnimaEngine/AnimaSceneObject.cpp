//
//  AnimaSceneObject.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaSceneObject.h"
#include "AnimaMath.h"
#include "AnimaCamera.h"
#include "AnimaFirstPersonCamera.h"
#include "AnimaThirdPersonCamera.h"
#include "AnimaLight.h"
#include "AnimaMesh.h"
#include "AnimaMeshInstance.h"
#include "AnimaModel.h"
#include "AnimaModelInstance.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaSceneObject::AnimaSceneObject(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaMappedValues(allocator, dataGeneratorsManager, name)
{
	IMPLEMENT_ANIMA_CLASS(AnimaSceneObject);

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

ptree AnimaSceneObject::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if(saveName)
	{
		tree.add("AnimaSceneObject.Name", GetName());
	}
	
	tree.add_child("AnimaSceneObject.Transformation", _transformation.GetObjectTree());
	
	ptree childrenTree;
	
	AInt count = _children.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		AnimaSceneObject* child = _children[i];
		AnimaString childType = child->_GetClassName();
		
		ptree childTree;
		childTree.add("Child.Type", childType);
		childTree.add("Child.Name", child);
		childrenTree.add_child("Child.Value", child->GetObjectTree(true));
	}
	
	tree.add_child("AnimaSceneObject.Children", childrenTree);
	tree.add_child("AnimaSceneObject.MappedValues", AnimaMappedValues::GetObjectTree(false));
	
	return tree;
}

bool AnimaSceneObject::ReadObject(const ptree& objectTree, bool readName)
{
	try
	{
		if(readName)
			SetName(objectTree.get<AnimaString>("AnimaSceneObject.Name"));
		
		_transformation.ReadObject(objectTree.get_child("AnimaSceneObject.Transformaton"));
		
		for(auto& child : objectTree.get_child("AnimaSceneObject.Children"))
		{
			if(child.first == "Child")
			{
				AnimaString name = child.second.get<AnimaString>("Name");
				AnimaString type = child.second.get<AnimaString>("Type");
				
				if(type == ANIMA_CLASS_NAME(AnimaCamera))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaFirstPersonCamera))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaThirdPersonCamera))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaLight))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaDirectionalLight))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaHemisphereLight))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaPointLight))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaSpotLight))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaMesh))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaMeshInstance))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaModel))
				{
					
				}
				else if(type == ANIMA_CLASS_NAME(AnimaModelInstance))
				{
					
				}
				else
				{
					AnimaLogger::LogMessageFormat("WARNING - Unrecognized child type, named '%s', while reading a scene object named '%s'", name.c_str(), GetName().c_str());
				}
			}
		}
		
		ptree mappedValuesTree = objectTree.get_child("AnimaSceneObject.MappedValues");
		return AnimaMappedValues::ReadObject(mappedValuesTree, false);
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing scene object: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing scene object: %s", exception.what());
		return false;
	}
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
	return _children.Add(child->GetName(), child);
}

AInt AnimaSceneObject::GetChildrenCount()
{
	return _children.GetSize();
}

AnimaSceneObject* AnimaSceneObject::GetChild(const AnimaString& name)
{
	return _children.GetWithName(name);
}

AnimaSceneObject* AnimaSceneObject::GetChild(AInt index)
{
	return _children.Get(index);
}

bool AnimaSceneObject::RemoveChild(AnimaSceneObject* object)
{
	return RemoveChild(object->GetName());
}

bool AnimaSceneObject::RemoveChild(const AnimaString& name)
{
	return _children.Remove(name);
}

bool AnimaSceneObject::RemoveChild(AInt index)
{
	return _children.Remove(index);
}

AInt AnimaSceneObject::HasChild(AnimaSceneObject* object)
{
	return _children.Contains(object->GetName());
}

AInt AnimaSceneObject::HasChild(const AnimaString& name)
{
	return _children.Contains(name);
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