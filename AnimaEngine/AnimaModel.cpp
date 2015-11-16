//
//  AnimaModel.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaModel.h"
#include "AnimaScene.h"
#include "AnimaMaterialsManager.h"
#include "AnimaMeshesManager.h"
#include "AnimaAnimationsManager.h"

#ifndef min
#	define min(a, b) (a > b ? b : a)
#endif

#ifndef max
#	define max(a, b) (a < b ? b : a)
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModel::AnimaModel(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaModel);
	_material = nullptr;
	_activeAnimation = -1;
	_topLevelModel = false;
	
	_shaderSource = ASDSO_NODE;
}

AnimaModel::AnimaModel(const AnimaModel& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _animationNodeName(src._animationNodeName)
	, _originFileName(src._originFileName)
	, _meshes(src._meshes)
	, _meshesBonesInfo(src._meshesBonesInfo)
	, _animations(src._animations)
{
	_material = src._material;
	_activeAnimation = -1;
	_topLevelModel = src._topLevelModel;
}

AnimaModel::AnimaModel(AnimaModel&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _animationNodeName(src._animationNodeName)
	, _originFileName(src._originFileName)
	, _meshes(src._meshes)
	, _meshesBonesInfo(src._meshesBonesInfo)
	, _animations(src._animations)
{
	_material = src._material;
	_activeAnimation = -1;
	_topLevelModel = src._topLevelModel;
}

AnimaModel::~AnimaModel()
{
	ClearMeshesBonesInfo();
}

AnimaModel& AnimaModel::operator=(const AnimaModel& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);

		_material = src._material;
		_materialName = src._materialName;
		_animationNodeName = src._animationNodeName;
		_originFileName = src._originFileName;
		_topLevelModel = src._topLevelModel;

		_meshes = src._meshes;

		ClearMeshesBonesInfo();
		SetMeshesBonesInfo(&src._meshesBonesInfo);

		_animations = src._animations;
		_activeAnimation = -1;
	}
	
	return *this;
}

AnimaModel& AnimaModel::operator=(AnimaModel&& src)
{
	if (this != &src)
	{
		AnimaSceneObject::operator=(src);
		
		_material = src._material;
		_materialName = src._materialName;
		_originFileName = src._originFileName;
		_animationNodeName = src._animationNodeName;
		_meshes = src._meshes;
		_topLevelModel = src._topLevelModel;

		ClearMeshesBonesInfo();
		SetMeshesBonesInfo(&src._meshesBonesInfo);
		
		_animations = src._animations;
		_activeAnimation = -1;
	}
	
	return *this;
}

ptree AnimaModel::GetObjectTree(bool saveName) const
{
	ptree tree;

	if (saveName)
		tree.add("AnimaModel.Name", GetName());

	AnimaString materialName = "";
	if (_material != nullptr)
		materialName = _material->GetName();
	else
		materialName = _materialName;

	tree.add("AnimaModel.TopLevelModel", IsTopLevelModel());
	tree.add("AnimaModel.Material", materialName);
	tree.add("AnimaModel.AnimationNodeName", _animationNodeName);

	ptree meshesTree;
	AInt count = _meshes.GetSize();
	for (AInt i = 0; i < count; i++)
		meshesTree.add("Mesh", _meshes[i]->GetName());
	tree.add_child("AnimaModel.Meshes", meshesTree);

	ptree meshesBonesInfoTree;
	count = _meshesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
		meshesBonesInfoTree.add_child("BoneInfo", _meshesBonesInfo[i]->GetObjectTree());
	tree.add_child("AnimaModel.MeshesBonesInfo", meshesBonesInfoTree);

	ptree animationsTree;
	for (auto& animation : _animations)
		animationsTree.add("Animation", animation->GetName());
	tree.add_child("AnimaModel.Animations", animationsTree);

	tree.add_child("AnimaModel.SceneObject", AnimaSceneObject::GetObjectTree(false));

	return tree;
}

bool AnimaModel::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		if (readName)
			SetName(objectTree.get<AnimaString>("AnimaModel.Name"));

		_materialName = objectTree.get<AnimaString>("AnimaModel.Material", "");
		_animationNodeName = objectTree.get<AnimaString>("AnimaModel.AnimationNodeName", "");
		SetTopLevelModel(objectTree.get<bool>("AnimaModel.TopLevelModel", false));

		if (!_materialName.empty())
			_material = scene->GetMaterialsManager()->GetMaterialFromName(_materialName);
		
		for (auto& meshData : objectTree.get_child("AnimaModel.Meshes"))
		{
			if (meshData.first == "Mesh")
			{
				AnimaString meshName = meshData.second.get_value<AnimaString>("");
				if (meshName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a model called '%s': found a mesh without name", GetName().c_str());
				else
				{
					AnimaMesh* mesh = scene->GetMeshesManager()->GetMeshFromName(meshName);
					if (mesh == nullptr)
						AnimaLogger::LogMessageFormat("WARNING - Error reading a model called '%s': model has a mesh named '%s' bot the mesh doesn't exists", GetName().c_str(), meshName.c_str());
					else
						AddMesh(mesh);
				}
			}
		}

		for (auto& boneInfoData : objectTree.get_child("AnimaModel.MeshesBonesInfo"))
		{
			if (boneInfoData.first == "BoneInfo")
			{
				AnimaString meshBoneInfoName = boneInfoData.second.get<AnimaString>("AnimaMeshBoneInfo.Name", "");
				if (meshBoneInfoName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a model called '%s': found a mesh bone data without name", GetName().c_str());
				else
				{
					AnimaMeshBoneInfo* meshBoneInfo = AnimaAllocatorNamespace::AllocateNew<AnimaMeshBoneInfo>(*_allocator, meshBoneInfoName, _allocator);
					if (meshBoneInfo->ReadObject(boneInfoData.second, scene, false))
						_meshesBonesInfo.Add(meshBoneInfoName, meshBoneInfo);
				}
			}
		}

		for (auto& animationData : objectTree.get_child("AnimaModel.Animations"))
		{
			if (animationData.first == "Animation")
			{
				AnimaString animationName = animationData.second.get_value<AnimaString>("");
				if (animationName.empty())
					AnimaLogger::LogMessageFormat("WARNING - Error reading a model called '%s': found an animation without name", GetName().c_str());
				else
				{
					AnimaAnimation* animation = scene->GetAnimationsManager()->GetAnimationFromName(animationName);
					if (animation == nullptr)
						AnimaLogger::LogMessageFormat("WARNING - Error reading a model called '%s': model has an animation named '%s' bot the animation doesn't exists", GetName().c_str(), animationName.c_str());
					else
						AddAnimation(animation);
				}
			}
		}

		ptree sceneObjectTree = objectTree.get_child("AnimaModel.SceneObject");
		if (AnimaSceneObject::ReadObject(sceneObjectTree, scene, false))
		{
			SetPosition(GetPosition());
			return true;
		}

		return false;
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing model: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing mesh: %s", exception.what());
		return false;
	}
}

bool AnimaModel::FinalizeAfterRead(AnimaScene* scene)
{
	// Chiamo prima la funzione della classe base perchè si occupa di caricare i dati dei figli,
	// i quali mi sono necessari per calcolare il bounding box
	bool rv = AnimaSceneObject::FinalizeAfterRead(scene);

	if(rv)
		ComputeBoundingBox();
	
	return rv;
}

void AnimaModel::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	_materialName = material->GetName();
}

AnimaMaterial* AnimaModel::GetMaterial()
{
	return _material;
}

AInt AnimaModel::AddMesh(AnimaMesh* mesh)
{
	mesh->SetParentObject(this);
	return _meshes.Add(mesh->GetName(), mesh);
}

AInt AnimaModel::GetMeshesCount() const
{
	return _meshes.GetSize();
}

AnimaMesh* AnimaModel::GetMesh(AInt index)
{
	return _meshes[index];
}

AnimaMesh* AnimaModel::GetMeshFromName(const AnimaString& name)
{
	return _meshes[name];
}

AInt AnimaModel::GetAnimationsCount() const
{
	return _animations.size();
}

void AnimaModel::SetAnimations(AnimaArray<AnimaAnimation*>* animations)
{
	_animations.clear();
	_animations = *animations;
}

AInt AnimaModel::AddAnimation(AnimaAnimation* animation)
{
	AInt index = _animations.size();
	_animations.push_back(animation);
	return index;
}

AnimaAnimation* AnimaModel::GetAnimation(AInt index)
{
	return _animations[index];
}

void AnimaModel::SetOriginFileName(const AnimaString& fileName)
{
	_originFileName = fileName;
}

AnimaString AnimaModel::GetAnimaOriginFileName() const
{
	return _originFileName;
}

void AnimaModel::SetAnimationNodeName(const AnimaString& animationNodeName)
{
	_animationNodeName = animationNodeName;
}

AnimaString AnimaModel::GetAnimaAnimationNodeName() const
{
	return _animationNodeName;
}

AnimaMappedArray<AnimaMeshBoneInfo*>* AnimaModel::GetMeshesBonesInfo()
{
	return &_meshesBonesInfo;
}

void AnimaModel::SetMeshesBonesInfo(const AnimaMappedArray<AnimaMeshBoneInfo*>* meshesBonesInfo)
{
	ClearMeshesBonesInfo();
	
	AInt count = meshesBonesInfo->GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaMeshBoneInfo* srcMeshBoneInfo = meshesBonesInfo->GetConst(i);
		AnimaMeshBoneInfo* newMeshBoneInfo = AnimaAllocatorNamespace::AllocateNew<AnimaMeshBoneInfo>(*_allocator, srcMeshBoneInfo->GetName(), _allocator);
		*newMeshBoneInfo = *srcMeshBoneInfo;
		_meshesBonesInfo.Add(srcMeshBoneInfo->GetName(), newMeshBoneInfo);
	}
}

void AnimaModel::ClearMeshesBonesInfo()
{
	AInt count = _meshesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaMeshBoneInfo* meshBoneInfo = _meshesBonesInfo[i];
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, meshBoneInfo);
		meshBoneInfo = nullptr;
	}

	_meshesBonesInfo.RemoveAll();
}

void AnimaModel::UpdateAnimation(AFloat animationTime)
{
	if (_activeAnimation >= 0)
		_animations[_activeAnimation]->UpdateAnimation(this, animationTime);
}

void AnimaModel::SetActiveAnimation(AInt animationIndex)
{
	ANIMA_ASSERT(animationIndex < _animations.size());
	_activeAnimation = animationIndex;
}

void AnimaModel::StopAnimation()
{
	_activeAnimation = -1;
}

void AnimaModel::ComputeBoundingBox()
{
	_boundingBoxMin.x = _boundingBoxMax.x = 0.0f;
	_boundingBoxMin.y = _boundingBoxMax.y = 0.0f;
	_boundingBoxMin.z = _boundingBoxMax.z = 0.0f;

	AInt meshesCount = _meshes.GetSize();
	if (meshesCount > 0)
	{
		AnimaMesh* mesh = _meshes[0];
		
		mesh->ComputeBoundingBox();
		_boundingBoxMin = mesh->GetBoundingBoxMin();
		_boundingBoxMax = mesh->GetBoundingBoxMax();

		for (AInt nm = 1; nm < meshesCount; nm++)
		{
			mesh = _meshes[1];
			mesh->ComputeBoundingBox();
			
			AnimaVertex3f bbmin = mesh->GetBoundingBoxMin();
			AnimaVertex3f bbmax = mesh->GetBoundingBoxMax();
			
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
		
		if(child->IsOfClass(ANIMA_CLASS_NAME(AnimaModel)))
		{
			AnimaModel* childModel = (AnimaModel*)child;
			childModel->ComputeBoundingBox();
			
			AnimaVertex3f bbmin = childModel->GetBoundingBoxMin();
			AnimaVertex3f bbmax = childModel->GetBoundingBoxMax();

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

AnimaVertex3f AnimaModel::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaModel::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

AnimaVertex3f AnimaModel::GetBoundingBoxCenter() const
{
	return _boundingBoxCenter;
}

void AnimaModel::UpdateChildrenTransformation()
{
	AnimaSceneObject::UpdateChildrenTransformation();

	AInt meshesCount = _meshes.GetSize();
	for (AInt i = 0; i < meshesCount; i++)
		_meshes[i]->GetTransformation()->UpdateMatrix();
}

void AnimaModel::GetAllMeshes(AnimaArray<AnimaMesh*> *meshes)
{
	if (meshes == nullptr)
		return;

	for (AInt nm = 0; nm < _meshes.GetSize(); nm++)
		meshes->push_back(_meshes[nm]);
	
	for (AInt nc = 0; nc < _children.GetSize(); nc++)
		((AnimaModel*)_children[nc])->GetAllMeshes(meshes);
}

END_ANIMA_ENGINE_NAMESPACE
