//
//  AnimaNode.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaNode.h"
#include "AnimaScene.h"
#include "AnimaMaterialsManager.h"
#include "AnimaGeometriesManager.h"
#include "AnimaAnimationsManager.h"

#ifndef min
#	define min(a, b) (a > b ? b : a)
#endif

#ifndef max
#	define max(a, b) (a < b ? b : a)
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaNode::AnimaNode(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaNode);
	_material = nullptr;
	_activeAnimation = -1;
	_topLevelNode = false;
	
	_shaderSource = ASDSO_NODE;
}

AnimaNode::AnimaNode(const AnimaNode& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _animationNodeName(src._animationNodeName)
	, _originFileName(src._originFileName)
	, _geometries(src._geometries)
	, _geometriesBonesInfo(src._geometriesBonesInfo)
	, _animations(src._animations)
{
	_material = src._material;
	_activeAnimation = -1;
	_topLevelNode = src._topLevelNode;
}

AnimaNode::AnimaNode(AnimaNode&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _animationNodeName(src._animationNodeName)
	, _originFileName(src._originFileName)
	, _geometries(src._geometries)
	, _geometriesBonesInfo(src._geometriesBonesInfo)
	, _animations(src._animations)
{
	_material = src._material;
	_activeAnimation = -1;
	_topLevelNode = src._topLevelNode;
}

AnimaNode::~AnimaNode()
{
	ClearGeometriesBonesInfo();
}

AnimaNode& AnimaNode::operator=(const AnimaNode& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		_animationNodeName = src._animationNodeName;
		_originFileName = src._originFileName;
		_topLevelNode = src._topLevelNode;

		_geometries = src._geometries;

		ClearGeometriesBonesInfo();
		SetGeometriesBonesInfo(&src._geometriesBonesInfo);

		_animations = src._animations;
		_activeAnimation = -1;
	}
	
	return *this;
}

AnimaNode& AnimaNode::operator=(AnimaNode&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);
		
		_material = src._material;
		_materialName = src._materialName;
		_originFileName = src._originFileName;
		_animationNodeName = src._animationNodeName;
		_geometries = src._geometries;
		_topLevelNode = src._topLevelNode;

		ClearGeometriesBonesInfo();
		SetGeometriesBonesInfo(&src._geometriesBonesInfo);
		
		_animations = src._animations;
		_activeAnimation = -1;
	}
	
	return *this;
}

ptree AnimaNode::GetObjectTree(bool saveName) const
{
	ptree tree;

	if (saveName)
		tree.add("AnimaNode.Name", GetName());

	AnimaString materialName = "";
	if (_material != nullptr)
		materialName = _material->GetName();
	else
		materialName = _materialName;

	tree.add("AnimaNode.TopLevelNode", IsTopLevelNode());
	tree.add("AnimaNode.Material", materialName);
	tree.add("AnimaNode.AnimationNodeName", _animationNodeName);

	ptree geometriesTree;
	AInt count = _geometries.GetSize();
	for (AInt i = 0; i < count; i++)
		geometriesTree.add("Geometry", _geometries[i]->GetName());
	tree.add_child("AnimaNode.Geometries", geometriesTree);

	ptree geometriesBonesInfoTree;
	count = _geometriesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
		geometriesBonesInfoTree.add_child("BoneInfo", _geometriesBonesInfo[i]->GetObjectTree());
	tree.add_child("AnimaNode.GeometriesBonesInfo", geometriesBonesInfoTree);

	ptree animationsTree;
	for (auto& animation : _animations)
		animationsTree.add("Animation", animation->GetName());
	tree.add_child("AnimaNode.Animations", animationsTree);

	tree.add_child("AnimaNode.SceneObject", AnimaSceneObject::GetObjectTree(false));

	return tree;
}

bool AnimaNode::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaNode.Name"));

		_materialName = objectTree.get<AnimaString>("AnimaNode.Material", "");
		_animationNodeName = objectTree.get<AnimaString>("AnimaNode.AnimationNodeName", "");
		SetTopLevelNode(objectTree.get<bool>("AnimaNode.TopLevelNode", false));

		if (!_materialName.empty())
			_material = scene->GetMaterialsManager()->GetMaterialFromName(_materialName);
		
		for (auto& geometryData : objectTree.get_child("AnimaNode.Geometries"))
		{
			if (geometryData.first == "Geometry")
			{
				AnimaString geometryName = geometryData.second.get_value<AnimaString>("");
				if (geometryName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a node called '%s': found a geometry without name", GetName().c_str());
				else
				{
					AnimaGeometry* geometry = scene->GetGeometriesManager()->GetGeometryFromName(geometryName);
					if (geometry == nullptr)
						AnimaLogger::LogMessageFormat("WARNING - Error reading a node called '%s': node has a geometry named '%s' bot the geometry doesn't exists", GetName().c_str(), geometryName.c_str());
					else
						AddGeometry(geometry);
				}
			}
		}

		for (auto& boneInfoData : objectTree.get_child("AnimaNode.GeometriesBonesInfo"))
		{
			if (boneInfoData.first == "BoneInfo")
			{
				AnimaString geometryBoneInfoName = boneInfoData.second.get<AnimaString>("AnimaGeometryBoneInfo.Name", "");
				if (geometryBoneInfoName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a node called '%s': found a geometry bone data without name", GetName().c_str());
				else
				{
					AnimaGeometryBoneInfo* geometryBoneInfo = AnimaAllocatorNamespace::AllocateNew<AnimaGeometryBoneInfo>(*_allocator, geometryBoneInfoName, _allocator);
					if (geometryBoneInfo->ReadObject(boneInfoData.second, scene, false))
						_geometriesBonesInfo.Add(geometryBoneInfoName, geometryBoneInfo);
				}
			}
		}

		for (auto& animationData : objectTree.get_child("AnimaNode.Animations"))
		{
			if (animationData.first == "Animation")
			{
				AnimaString animationName = animationData.second.get_value<AnimaString>("");
				if (animationName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a node called '%s': found an animation without name", GetName().c_str());
				else
				{
					AnimaAnimation* animation = scene->GetAnimationsManager()->GetAnimationFromName(animationName);
					if (animation == nullptr)
						AnimaLogger::LogMessageFormat("WARNING - Error reading a node called '%s': node has an animation named '%s' bot the animation doesn't exists", GetName().c_str(), animationName.c_str());
					else
						AddAnimation(animation);
				}
			}
		}

		ptree sceneObjectTree = objectTree.get_child("AnimaNode.SceneObject");
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

bool AnimaNode::FinalizeAfterRead(AnimaScene* scene)
{
	// Chiamo prima la funzione della classe base perchè si occupa di caricare i dati dei figli,
	// i quali mi sono necessari per calcolare il bounding box
	bool rv = AnimaSceneObject::FinalizeAfterRead(scene);

	if(rv)
		ComputeBoundingBox();
	
	return rv;
}

void AnimaNode::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	_materialName = material->GetName();
}

AnimaMaterial* AnimaNode::GetMaterial()
{
	return _material;
}

AInt AnimaNode::AddGeometry(AnimaGeometry* geometry)
{
	geometry->SetParentObject(this);
	return _geometries.Add(geometry->GetName(), geometry);
}

AInt AnimaNode::GetGeometriesCount() const
{
	return _geometries.GetSize();
}

AnimaGeometry* AnimaNode::GetGeometry(AInt index)
{
	return _geometries[index];
}

AnimaGeometry* AnimaNode::GetGeometryFromName(const AnimaString& name)
{
	return _geometries[name];
}

AInt AnimaNode::GetAnimationsCount() const
{
	return _animations.size();
}

void AnimaNode::SetAnimations(AnimaArray<AnimaAnimation*>* animations)
{
	_animations.clear();
	_animations = *animations;
}

AInt AnimaNode::AddAnimation(AnimaAnimation* animation)
{
	AInt index = _animations.size();
	_animations.push_back(animation);
	return index;
}

AnimaAnimation* AnimaNode::GetAnimation(AInt index)
{
	return _animations[index];
}

void AnimaNode::SetOriginFileName(const AnimaString& fileName)
{
	_originFileName = fileName;
}

AnimaString AnimaNode::GetAnimaOriginFileName() const
{
	return _originFileName;
}

void AnimaNode::SetAnimationNodeName(const AnimaString& animationNodeName)
{
	_animationNodeName = animationNodeName;
}

AnimaString AnimaNode::GetAnimaAnimationNodeName() const
{
	return _animationNodeName;
}

AnimaMappedArray<AnimaGeometryBoneInfo*>* AnimaNode::GetGeometriesBonesInfo()
{
	return &_geometriesBonesInfo;
}

void AnimaNode::SetGeometriesBonesInfo(const AnimaMappedArray<AnimaGeometryBoneInfo*>* geometriesBonesInfo)
{
	ClearGeometriesBonesInfo();
	
	AInt count = geometriesBonesInfo->GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaGeometryBoneInfo* srcGeometryBoneInfo = geometriesBonesInfo->GetConst(i);
		AnimaGeometryBoneInfo* newGeometryBoneInfo = AnimaAllocatorNamespace::AllocateNew<AnimaGeometryBoneInfo>(*_allocator, srcGeometryBoneInfo->GetName(), _allocator);
		*newGeometryBoneInfo = *srcGeometryBoneInfo;
		_geometriesBonesInfo.Add(srcGeometryBoneInfo->GetName(), newGeometryBoneInfo);
	}
}

void AnimaNode::ClearGeometriesBonesInfo()
{
	AInt count = _geometriesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaGeometryBoneInfo* geometryBoneInfo = _geometriesBonesInfo[i];
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, geometryBoneInfo);
		geometryBoneInfo = nullptr;
	}

	_geometriesBonesInfo.RemoveAll();
}

void AnimaNode::UpdateAnimation(AFloat animationTime)
{
	if (_activeAnimation >= 0)
		_animations[_activeAnimation]->UpdateAnimation(this, animationTime);
}

void AnimaNode::SetActiveAnimation(AInt animationIndex)
{
	ANIMA_ASSERT(animationIndex < _animations.size());
	_activeAnimation = animationIndex;
}

void AnimaNode::StopAnimation()
{
	_activeAnimation = -1;
}

void AnimaNode::ComputeBoundingBox()
{
	_boundingBoxMin.x = _boundingBoxMax.x = 0.0f;
	_boundingBoxMin.y = _boundingBoxMax.y = 0.0f;
	_boundingBoxMin.z = _boundingBoxMax.z = 0.0f;

	AInt geometriesCount = _geometries.GetSize();
	if (geometriesCount > 0)
	{
		AnimaGeometry* geometry = _geometries[0];
		
		geometry->ComputeBoundingBox();
		_boundingBoxMin = geometry->GetBoundingBoxMin();
		_boundingBoxMax = geometry->GetBoundingBoxMax();

		for (AInt nm = 1; nm < geometriesCount; nm++)
		{
			geometry = _geometries[1];
			geometry->ComputeBoundingBox();
			
			AnimaVertex3f bbmin = geometry->GetBoundingBoxMin();
			AnimaVertex3f bbmax = geometry->GetBoundingBoxMax();
			
			_boundingBoxMin.x = min(_boundingBoxMin.x, bbmin.x);
			_boundingBoxMin.y = min(_boundingBoxMin.y, bbmin.y);
			_boundingBoxMin.z = min(_boundingBoxMin.z, bbmin.z);

			_boundingBoxMax.x = max(_boundingBoxMax.x, bbmax.x);
			_boundingBoxMax.y = max(_boundingBoxMax.y, bbmax.y);
			_boundingBoxMax.z = max(_boundingBoxMax.z, bbmax.z);
		}
	}

	for (AInt nc = 0; nc < _children.GetSize(); nc++)
	{
		AnimaSceneObject* child = _children[nc];
		
		if(child->IsOfClass(ANIMA_CLASS_NAME(AnimaNode)))
		{
			AnimaNode* childNode = (AnimaNode*)child;
			childNode->ComputeBoundingBox();
			
			AnimaVertex3f bbmin = childNode->GetBoundingBoxMin();
			AnimaVertex3f bbmax = childNode->GetBoundingBoxMax();

			_boundingBoxMin.x = min(_boundingBoxMin.x, bbmin.x);
			_boundingBoxMin.y = min(_boundingBoxMin.y, bbmin.y);
			_boundingBoxMin.z = min(_boundingBoxMin.z, bbmin.z);

			_boundingBoxMax.x = max(_boundingBoxMax.x, bbmax.x);
			_boundingBoxMax.y = max(_boundingBoxMax.y, bbmax.y);
			_boundingBoxMax.z = max(_boundingBoxMax.z, bbmax.z);
		}
	}

	_boundingBoxCenter = AnimaVertex3f((_boundingBoxMin.x + _boundingBoxMax.x) / 2.0f, (_boundingBoxMin.y + _boundingBoxMax.y) / 2.0f, (_boundingBoxMin.z + _boundingBoxMax.z) / 2.0f);
}

AnimaVertex3f AnimaNode::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaNode::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

AnimaVertex3f AnimaNode::GetBoundingBoxCenter() const
{
	return _boundingBoxCenter;
}

void AnimaNode::UpdateChildrenTransformation()
{
	AnimaSceneObject::UpdateChildrenTransformation();

	AInt geometriesCount = _geometries.GetSize();
	for (AInt i = 0; i < geometriesCount; i++)
		_geometries[i]->GetTransformation()->UpdateMatrix();
}

void AnimaNode::GetAllGeometries(AnimaArray<AnimaGeometry*> *geometries)
{
	if (geometries == nullptr)
		return;

	for (AInt nm = 0; nm < _geometries.GetSize(); nm++)
		geometries->push_back(_geometries[nm]);
	
	for (AInt nc = 0; nc < _children.GetSize(); nc++)
		((AnimaNode*)_children[nc])->GetAllGeometries(geometries);
}

END_ANIMA_ENGINE_NAMESPACE
