//
//  AnimaEngineModelsManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaModelInstancesManager__
#define __Anima__AnimaModelInstancesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaMesh.h"
#include "AnimaModel.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaMeshesManager.h"
#include "AnimaModelsManager.h"
#include "AnimaModelInstance.h"
#include "AnimaMeshInstancesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaModelInstancesManager
{
public:
	AnimaModelInstancesManager(AnimaScene* scene, AnimaModelsManager* modelsManager, AnimaMeshInstancesManager* meshInstancesManager);
	~AnimaModelInstancesManager();

public:
	AnimaModelInstance* CreateInstance(const AnimaString& instanceName, AnimaModel* srcModel);
	AnimaModelInstance* CreateInstance(const AnimaString& instanceName, const AnimaString& srcModelName);

	AInt GetModelInstancesCount();

	AnimaModelInstance* GetModelInstance(AInt index);
	AnimaModelInstance* GetModelInstanceFromName(const AnimaString& name);

public:
	void ClearInstances();

protected:
	AnimaModelInstance* CreateInstanceFromModel(const AnimaString& instanceName, AnimaModel* srcModel, bool useSrcModelName = false);
	AnimaModelInstance* CreateEmptyInstance(const AnimaString& instanceName);
		
private:
	AnimaScene*			_scene;
	AnimaModelsManager*	_modelsManager;
	AnimaMeshInstancesManager* _meshInstancesManager;

	AnimaMappedArray<AnimaModelInstance*>	_modelIntances;
	AnimaMappedArray<AnimaModelInstance*>	_topLevelModelInstances;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelInstancesManager__) */
