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
#include "AnimaMesh.h"
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
	
public:
	void SetMaterial(AnimaMaterial* material);
	AnimaMaterial* GetMaterial();

protected:
	void SetMeshes(AnimaArray<AnimaMesh*>* meshes);
	void SetGeneratorModel(AnimaModel* model);

public:
	AInt GetMeshesCount() const;
	AnimaMesh* GetMesh(AInt index);

	AnimaModel* GetGeneratorModel() const;

protected:
	const char* GetShaderPrefix() { return ""; }

protected:
	AnimaMaterial*	_material;
	AnimaString		_materialName;

	AnimaModel* _generatorModel;
	AnimaString _generatorModelName;

	AnimaArray<AnimaMesh*> _meshes;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelInstance__) */
