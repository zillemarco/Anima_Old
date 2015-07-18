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
{
	_material = nullptr;
	_activeAnimation = -1;
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
	_materialName = material->GetName();
}

AnimaMaterial* AnimaModel::GetMaterial()
{
	return _material;
}

AInt AnimaModel::AddMesh(AnimaMesh* mesh)
{
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
		_boundingBoxMin = _meshes[0]->GetBoundingBoxMin();
		_boundingBoxMax = _meshes[0]->GetBoundingBoxMax();

		for (AInt nm = 1; nm < meshesCount; nm++)
		{
			AnimaVertex3f bbmin = _meshes[nm]->GetBoundingBoxMin();
			AnimaVertex3f bbmax = _meshes[nm]->GetBoundingBoxMax();
			
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
		AnimaVertex3f bbmin = ((AnimaModel*)_children[nc])->GetBoundingBoxMin();
		AnimaVertex3f bbmax = ((AnimaModel*)_children[nc])->GetBoundingBoxMax();

		_boundingBoxMin.x = min(_boundingBoxMin.x, bbmin.x);
		_boundingBoxMin.y = min(_boundingBoxMin.y, bbmin.y);
		_boundingBoxMin.z = min(_boundingBoxMin.z, bbmin.z);

		_boundingBoxMax.x = max(_boundingBoxMax.x, bbmax.x);
		_boundingBoxMax.y = max(_boundingBoxMax.y, bbmax.y);
		_boundingBoxMax.z = max(_boundingBoxMax.z, bbmax.z);
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
