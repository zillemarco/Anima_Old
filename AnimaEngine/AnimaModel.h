//
//  AnimaModel.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaModel__
#define __Anima__AnimaModel__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaSceneObject.h"
#include "AnimaMaterial.h"
#include "AnimaMappedArray.h"
#include "AnimaMesh.h"
#include "AnimaAnimation.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#pragma warning (disable: 4251)
typedef boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> BoneNameMap;
#pragma warning (default: 4251)

class ANIMA_ENGINE_EXPORT AnimaModel : public AnimaSceneObject
{
	friend class AnimaModelInstancesManager;
	friend class AnimaMeshInstancesManager;

public:
	AnimaModel(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaModel(const AnimaModel& src);
	AnimaModel(AnimaModel&& src);
	~AnimaModel();
	
	AnimaModel& operator=(const AnimaModel& src);
	AnimaModel& operator=(AnimaModel&& src);

public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();

	AInt GetMeshesCount() const;
	AInt AddMesh(AnimaMesh* mesh);
	AnimaMesh* GetMesh(AInt index);
	AnimaMesh* GetMeshFromName(const AnimaString& name);
	AnimaMesh* GetMeshFromName(const char* name);

	AInt GetAnimationsCount() const;
	void SetAnimations(AnimaArray<AnimaAnimation*>* animations);
	AInt AddAnimation(AnimaAnimation* animation);
	AnimaAnimation* GetAnimation(AInt index);

	void SetOriginFileName(const AnimaString& fileName);
	void SetOriginFileName(const char* fileName);
	AnimaString GetAnimaOriginFileName() const;
	const char* GetOriginFileName() const;

	void SetAnimationNodeName(const AnimaString& animationNodeName);
	void SetAnimationNodeName(const char* animationNodeName);
	AnimaString GetAnimaAnimationNodeName() const;
	const char* GetAnimationNodeName() const;

	AnimaMappedArray<AnimaMeshBoneInfo*>* GetMeshesBonesInfo();
	void SetMeshesBonesInfo(const AnimaMappedArray<AnimaMeshBoneInfo*>* meshesBonesInfo);
	void ClearMeshesBonesInfo();

	void UpdateAnimation(AFloat animationTime);
	void SetActiveAnimation(AInt animationIndex);
	void StopAnimation();

protected:
	const char* GetShaderPrefix() { return "MOD"; }

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;
	AnimaString		_animationNodeName;
	AnimaString		_originFileName;

	AnimaMappedArray<AnimaMesh*> _meshes;
	AnimaMappedArray<AnimaMeshBoneInfo*> _meshesBonesInfo;
	
	AnimaArray<AnimaAnimation*> _animations;
	AInt _activeAnimation;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
