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

class ANIMA_ENGINE_EXPORT AnimaModel : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaModel);

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
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;

public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();

	AInt GetMeshesCount() const;
	AInt AddMesh(AnimaMesh* mesh);
	AnimaMesh* GetMesh(AInt index);
	AnimaMesh* GetMeshFromName(const AnimaString& name);

	AInt GetAnimationsCount() const;
	void SetAnimations(AnimaArray<AnimaAnimation*>* animations);
	AInt AddAnimation(AnimaAnimation* animation);
	AnimaAnimation* GetAnimation(AInt index);

	void SetOriginFileName(const AnimaString& fileName);
	AnimaString GetAnimaOriginFileName() const;

	void SetAnimationNodeName(const AnimaString& animationNodeName);
	AnimaString GetAnimaAnimationNodeName() const;

	AnimaMappedArray<AnimaMeshBoneInfo*>* GetMeshesBonesInfo();
	void SetMeshesBonesInfo(const AnimaMappedArray<AnimaMeshBoneInfo*>* meshesBonesInfo);
	void ClearMeshesBonesInfo();

	void UpdateAnimation(AFloat animationTime);
	void SetActiveAnimation(AInt animationIndex);
	void StopAnimation();

	void ComputeBoundingBox();
	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;
	AnimaVertex3f GetBoundingBoxCenter() const;

	void GetAllMeshes(AnimaArray<AnimaMesh*> *meshes);
	
	virtual void SetTopLevelModel(bool topLevelModel) { _topLevelModel = topLevelModel; }
	virtual bool IsTopLevelModel() const { return _topLevelModel; }

protected:
	const char* GetShaderPrefix() { return "MOD"; }
	
	virtual void UpdateChildrenTransformation();

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;
	AnimaString		_animationNodeName;
	AnimaString		_originFileName;

	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;
	AnimaVertex3f	_boundingBoxCenter;
	
	bool _topLevelModel;

	AnimaMappedArray<AnimaMesh*> _meshes;
	AnimaMappedArray<AnimaMeshBoneInfo*> _meshesBonesInfo;
	
	AnimaArray<AnimaAnimation*> _animations;
	AInt _activeAnimation;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModel__) */
