//
//  AnimaModel.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaModel::AnimaModel(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator)
	: AnimaSceneObject(name, dataGeneratorsManager, allocator)
	, _materialName(allocator)
	, _originFileName(allocator)
	, _animationNodeName(allocator)
	, _meshes(allocator)
	, _meshesBonesInfo(allocator)
	, _animations(allocator)
{
	_material = nullptr;
	_activeAnimation = -1;
}

AnimaModel::AnimaModel(const AnimaModel& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _originFileName(src._originFileName)
	, _animationNodeName(src._animationNodeName)
	, _meshes(src._meshes)
	, _meshesBonesInfo(src._meshesBonesInfo)
	, _animations(src._animations)
{
	_material = src._material;
	_activeAnimation = -1;

}

AnimaModel::AnimaModel(AnimaModel&& src)
	: AnimaSceneObject(src)
	, _materialName(src._materialName)
	, _originFileName(src._originFileName)
	, _animationNodeName(src._animationNodeName)
	, _meshes(src._meshes)
	, _meshesBonesInfo(src._meshesBonesInfo)
	, _animations(src._animations)
{
	_material = src._material;
	_activeAnimation = -1;
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

		ClearMeshesBonesInfo();
		SetMeshesBonesInfo(&src._meshesBonesInfo);
		
		_animations = src._animations;
		_activeAnimation = -1;
	}
	
	return *this;
}

void AnimaModel::SetMaterial(AnimaMaterial* material)
{
	_material = material;
	_materialName = material->GetAnimaName();
}

AnimaMaterial* AnimaModel::GetMaterial()
{
	return _material;
}

AInt AnimaModel::AddMesh(AnimaMesh* mesh)
{
	return _meshes.Add(mesh->GetAnimaName(), mesh);
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

AnimaMesh* AnimaModel::GetMeshFromName(const char* name)
{
	AnimaString str(name, _allocator);
	return _meshes[name];
}

AInt AnimaModel::GetAnimationsCount() const
{
	return _animations.GetSize();
}

void AnimaModel::SetAnimations(AnimaArray<AnimaAnimation*>* animations)
{
	_animations.RemoveAll();
	_animations = *animations;
}

AInt AnimaModel::AddAnimation(AnimaAnimation* animation)
{
	return _animations.Add(animation);
}

AnimaAnimation* AnimaModel::GetAnimation(AInt index)
{
	return _animations[index];
}

void AnimaModel::SetOriginFileName(const AnimaString& fileName)
{
	_originFileName = fileName;
}

void AnimaModel::SetOriginFileName(const char* fileName)
{
	_originFileName = fileName;
}

AnimaString AnimaModel::GetAnimaOriginFileName() const
{
	return _originFileName;
}

const char* AnimaModel::GetOriginFileName() const
{
	return _originFileName.GetConstBuffer();
}

void AnimaModel::SetAnimationNodeName(const AnimaString& animationNodeName)
{
	_animationNodeName = animationNodeName;
}

void AnimaModel::SetAnimationNodeName(const char* animationNodeName)
{
	_animationNodeName = animationNodeName;
}

AnimaString AnimaModel::GetAnimaAnimationNodeName() const
{
	return _animationNodeName;
}

const char* AnimaModel::GetAnimationNodeName() const
{
	return _animationNodeName.GetConstBuffer();
}

AnimaMappedArray<AnimaMeshBoneInfo*>* AnimaModel::GetMeshesBonesInfo()
{
	return &_meshesBonesInfo;
}

void AnimaModel::SetMeshesBonesInfo(const AnimaMappedArray<AnimaMeshBoneInfo*>* meshesBonesInfo)
{
	ClearMeshesBonesInfo();
	
	AInt count = _meshesBonesInfo.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaMeshBoneInfo* srcMeshBoneInfo = meshesBonesInfo->GetConst(i);
		AnimaMeshBoneInfo* newMeshBoneInfo = AnimaAllocatorNamespace::AllocateNew<AnimaMeshBoneInfo>(*_allocator, srcMeshBoneInfo->GetAnimaName(), _allocator);
		*newMeshBoneInfo = *srcMeshBoneInfo;
		_meshesBonesInfo.Add(srcMeshBoneInfo->GetAnimaName(), newMeshBoneInfo);
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
	ANIMA_ASSERT(animationIndex < _animations.GetSize());
	_activeAnimation = animationIndex;
}

void AnimaModel::StopAnimation()
{
	_activeAnimation = -1;
}

END_ANIMA_ENGINE_NAMESPACE
