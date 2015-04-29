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

class AnimaModelInstancesManager;

class ANIMA_ENGINE_EXPORT AnimaModelInstance : public AnimaSceneObject
{
	friend class AnimaModelInstancesManager;

public:
	AnimaModelInstance(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	AnimaModelInstance(const AnimaModelInstance& src);
	AnimaModelInstance(AnimaModelInstance&& src);
	~AnimaModelInstance();
	
	AnimaModelInstance& operator=(const AnimaModelInstance& src);
	AnimaModelInstance& operator=(AnimaModelInstance&& src);
	
protected:
	void SetMeshes(AnimaArray<AnimaMeshInstance*>* meshes);
	void SetModel(AnimaModel* model);

public:
	AInt GetMeshesCount() const;
	AnimaMeshInstance* GetMesh(AInt index);
	AnimaArray<AnimaMeshInstance*>* GetMeshes() const;

	AnimaModel* GetModel() const;

protected:
	const char* GetShaderPrefix() { return ""; }

protected:
	AnimaModel* _model;
	AnimaString _modelName;

	AnimaArray<AnimaMeshInstance*> _meshes;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelInstance__) */
