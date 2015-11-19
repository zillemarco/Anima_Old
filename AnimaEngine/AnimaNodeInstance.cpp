//
//  AnimaNodeInstance.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaNodeInstance.h"
#include "AnimaNodesManager.h"
#include "AnimaGeometryInstancesManager.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaNodeInstance::AnimaNodeInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaNodeInstance);
	_node = nullptr;
	_isAsset = false;
	
	_shaderSource = ASDSO_NODE;
}

AnimaNodeInstance::AnimaNodeInstance(const AnimaNodeInstance& src)
	: AnimaSceneObject(src)
	, _nodeName(src._nodeName)
	, _geometries(src._geometries)
{
	_node = src._node;
	_isAsset = src._isAsset;
}

AnimaNodeInstance::AnimaNodeInstance(AnimaNodeInstance&& src)
	: AnimaSceneObject(src)
	, _nodeName(src._nodeName)
	, _geometries(src._geometries)
{
	_node = src._node;
	_isAsset = src._isAsset;
}

AnimaNodeInstance::~AnimaNodeInstance()
{
}

AnimaNodeInstance& AnimaNodeInstance::operator=(const AnimaNodeInstance& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_geometries = src._geometries;		
		_node = src._node;
		_nodeName = src._nodeName;
		_isAsset = src._isAsset;
	}
	
	return *this;
}

AnimaNodeInstance& AnimaNodeInstance::operator=(AnimaNodeInstance&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_geometries = src._geometries;
		_node = src._node;
		_nodeName = src._nodeName;
		_isAsset = src._isAsset;
	}
	
	return *this;
}

ptree AnimaNodeInstance::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	if (saveName)
		tree.add("AnimaNodeInstance.Name", GetName());
	
	tree.add("AnimaNodeInstance.IsAsset", IsAsset());
	
	AnimaString nodeName = _nodeName;
	if(nodeName.empty() && _node != nullptr)
		nodeName = _node->GetName();
	tree.add("AnimaNodeInstance.NodeName", nodeName);
	
	ptree geometriesTree;
	AInt count = _geometries.size();
	for (AInt i = 0; i < count; i++)
		geometriesTree.add("GeometryInstance", _geometries[i]->GetName());
	tree.add_child("AnimaNodeInstance.GeometriesInstances", geometriesTree);
	
	tree.add_child("AnimaNodeInstance.SceneObject", AnimaSceneObject::GetObjectTree(false));
	
	return tree;
}

bool AnimaNodeInstance::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaNodeInstance.Name"));
		
		_nodeName = objectTree.get<AnimaString>("AnimaNodeInstance.NodeName", "");
		SetIsAsset(objectTree.get<bool>("AnimaNodeInstance.IsAsset", false));
		
		if (!_nodeName.empty())
			_node = scene->GetNodesManager()->GetNodeFromName(_nodeName);
		else
		{
			AnimaLogger::LogMessageFormat("WARNING - Error reading a node instance named '%s'. The instance source node name isn't specified", GetName().c_str());
		}
		
		for (auto& geometryData : objectTree.get_child("AnimaNodeInstance.GeometriesInstances"))
		{
			if (geometryData.first == "GeometryInstance")
			{
				AnimaString geometryInstanceName = geometryData.second.get_value<AnimaString>("");
				if (geometryInstanceName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a node instance called '%s': found a geometry instance without name", GetName().c_str());
				else
				{
					AnimaGeometryInstance* geometryInstance = scene->GetGeometryInstancesManager()->GetGeometryInstanceFromName(geometryInstanceName);
					if (geometryInstance == nullptr)
						AnimaLogger::LogMessageFormat("WARNING - Error reading a node instance called '%s': node instance has a geometry instance named '%s' bot the geometry instance doesn't exists", GetName().c_str(), geometryInstanceName.c_str());
					else
						AddGeometry(geometryInstance);
				}
			}
		}
		
		ptree sceneObjectTree = objectTree.get_child("AnimaNodeInstance.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}
		
		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing node: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing geometry: %s", exception.what());
		return false;
	}
}

void AnimaNodeInstance::SetGeometries(AnimaArray<AnimaGeometryInstance*>* geometries)
{
	_geometries.clear();

	if (geometries != nullptr)
	{
		_geometries = *geometries;

		AInt geometriesCount = _geometries.size();
		for (AInt i = 0; i < geometriesCount; i++)
			_geometries[i]->SetParentObject(this);
	}
}

void AnimaNodeInstance::AddGeometry(AnimaGeometryInstance* geometry)
{
	geometry->SetParentObject(this);
	_geometries.push_back(geometry);
}

AInt AnimaNodeInstance::GetGeometriesCount() const
{
	return _geometries.size();
}

AnimaGeometryInstance* AnimaNodeInstance::GetGeometry(AInt index)
{
	return _geometries[index];
}

void AnimaNodeInstance::SetNode(AnimaNode* node)
{
	_node = node;
	if(_node != nullptr)
		_nodeName = _node->GetName();
}

AnimaArray<AnimaGeometryInstance*>* AnimaNodeInstance::GetGeometries() const
{
	return (AnimaArray<AnimaGeometryInstance*>*)&_geometries;
}

AnimaNode* AnimaNodeInstance::GetNode()
{
	return _node;
}

void AnimaNodeInstance::UpdateChildrenTransformation()
{
	AnimaSceneObject::UpdateChildrenTransformation();
	
	AInt geometriesCount = _geometries.size();
	for (AInt i = 0; i < geometriesCount; i++)
		_geometries[i]->GetTransformation()->UpdateMatrix();
}

void AnimaNodeInstance::GetAllGeometries(AnimaArray<AnimaGeometryInstance*> *geometries)
{
	if (geometries == nullptr)
		return;

	for (AInt nm = 0; nm < _geometries.size(); nm++)
		geometries->push_back(_geometries[nm]);

	for (AInt nc = 0; nc < _children.GetSize(); nc++)
		((AnimaNodeInstance*)_children[nc])->GetAllGeometries(geometries);
}

END_ANIMA_ENGINE_NAMESPACE
