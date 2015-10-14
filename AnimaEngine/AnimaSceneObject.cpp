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

#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaMeshesManager.h"
#include "AnimaMeshInstancesManager.h"
#include "AnimaModelsManager.h"
#include "AnimaModelInstancesManager.h"

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
	}
	
	tree.add_child("AnimaSceneObject.Children", childrenTree);
	tree.add_child("AnimaSceneObject.MappedValues", AnimaMappedValues::GetObjectTree(false));
	
	return tree;
}

bool AnimaSceneObject::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
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
				AnimaSceneObjectChildrenReadData readData;
				readData._name = child.second.get<AnimaString>("Name");
				readData._type = child.second.get<AnimaString>("Type");

				_childrenReadData.push_back(readData);
			}
		}
		
		ptree mappedValuesTree = objectTree.get_child("AnimaSceneObject.MappedValues");
		return AnimaMappedValues::ReadObject(mappedValuesTree, scene, false);
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

bool AnimaSceneObject::TranslateChildrenData(AnimaScene* scene)
{
	if (scene == nullptr)
		return false;

	for (auto& childData : _childrenReadData)
	{
		AnimaSceneObject* child = nullptr;
		if (childData._type == ANIMA_CLASS_NAME(AnimaCamera))
		{
			child = scene->GetCamerasManager()->GetCameraFromName(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaFirstPersonCamera))
		{
			child = scene->GetCamerasManager()->GetCameraOfTypeFromName<AnimaFirstPersonCamera>(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaThirdPersonCamera))
		{
			child = scene->GetCamerasManager()->GetCameraOfTypeFromName<AnimaThirdPersonCamera>(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaLight))
		{
			child = scene->GetLightsManager()->GetLightFromName(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaDirectionalLight))
		{
			child = scene->GetLightsManager()->GetLightOfTypeFromName<AnimaDirectionalLight>(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaHemisphereLight))
		{
			child = scene->GetLightsManager()->GetLightOfTypeFromName<AnimaHemisphereLight>(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaPointLight))
		{
			child = scene->GetLightsManager()->GetLightOfTypeFromName<AnimaPointLight>(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaSpotLight))
		{
			child = scene->GetLightsManager()->GetLightOfTypeFromName<AnimaSpotLight>(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaMesh))
		{
			child = scene->GetMeshesManager()->GetMeshFromName(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaMeshInstance))
		{
			child = scene->GetMeshInstancesManager()->GetMeshInstanceFromName(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaModel))
		{
			child = scene->GetModelsManager()->GetModelFromName(childData._name);
		}
		else if (childData._type == ANIMA_CLASS_NAME(AnimaModelInstance))
		{
			child = scene->GetModelInstancesManager()->GetModelInstanceFromName(childData._name);
		}

		if (child != nullptr)
		{
			this->AddChild(child);
		}
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Unrecognized child type, named '%s', while reading a scene object named '%s'. Child type is '%s'", childData._name.c_str(), GetName().c_str(), childData._type.c_str());
		}
	}

	_childrenReadData.clear();

	return true;
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