//
//  AnimaModelInstance.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaModelInstance__
#define __Anima__AnimaModelInstance__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaSceneObject.h"
#include "AnimaMaterial.h"
#include "AnimaMappedArray.h"
#include "AnimaMeshInstance.h"
#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;
class AnimaModelInstancesManager;

class ANIMA_ENGINE_EXPORT AnimaModelInstance : public AnimaSceneObject
{
	DECLARE_ANIMA_CLASS(AnimaModelInstance);

	friend class AnimaModelInstancesManager;

public:
	AnimaModelInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaModelInstance(const AnimaModelInstance& src);
	AnimaModelInstance(AnimaModelInstance&& src);
	~AnimaModelInstance();
	
	AnimaModelInstance& operator=(const AnimaModelInstance& src);
	AnimaModelInstance& operator=(AnimaModelInstance&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
protected:
	void SetMeshes(AnimaArray<AnimaMeshInstance*>* meshes);
	void AddMesh(AnimaMeshInstance* mesh);
	void SetModel(AnimaModel* model);

public:
	AInt GetMeshesCount() const;
	AnimaMeshInstance* GetMesh(AInt index);
	AnimaArray<AnimaMeshInstance*>* GetMeshes() const;

	void GetAllMeshes(AnimaArray<AnimaMeshInstance*> *meshes);

	AnimaModel* GetModel();
	
	virtual void SetTopLevelModel(bool topLevelModel) { _topLevelModel = topLevelModel; }
	virtual bool IsTopLevelModel() const { return _topLevelModel; }

protected:
	const char* GetShaderPrefix() override { return "MOD"; }

	void UpdateChildrenTransformation() override;

protected:
	AnimaModel* _model;
	AnimaString _modelName;
	
	bool _topLevelModel;

	AnimaArray<AnimaMeshInstance*> _meshes;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelInstance__) */
