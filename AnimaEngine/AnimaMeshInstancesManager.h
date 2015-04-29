//
//  AnimaEngineModelsManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaMeshInstancesManager__
#define __Anima__AnimaMeshInstancesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaMesh.h"
#include "AnimaMesh.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaMeshesManager.h"
#include "AnimaMeshInstance.h"
#include "AnimaModel.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMeshInstancesManager
{
public:
	AnimaMeshInstancesManager(AnimaScene* scene, AnimaMeshesManager* meshesManager);
	~AnimaMeshInstancesManager();

public:
	AnimaMeshInstance* CreateInstance(const AnimaString& instanceName, AnimaMesh* srcMesh);
	AnimaMeshInstance* CreateInstance(const char* instanceName, AnimaMesh* srcMesh);
	AnimaMeshInstance* CreateInstance(const AnimaString& instanceName, const AnimaString& srcMeshName);
	AnimaMeshInstance* CreateInstance(const char* instanceName, const AnimaString& srcMeshName);
	AnimaMeshInstance* CreateInstance(const AnimaString& instanceName, const char* srcMeshName);
	AnimaMeshInstance* CreateInstance(const char* instanceName, const char* srcMeshName);

	AnimaArray<AnimaMeshInstance*>* CreateInstances(AnimaModel* srcModel);

	AInt GetMeshInstancesNumber();

	AnimaMeshInstance* GetMeshInstance(AInt index);
	AnimaMeshInstance* GetMeshInstanceFromName(const AnimaString& name);
	AnimaMeshInstance* GetMeshInstanceFromName(const char* name);

protected:
	AnimaMeshInstance* CreateEmptyInstance(const AnimaString& instanceName);
	AnimaMeshInstance* CreateEmptyInstance(const char* instanceName);

public:
	void ClearInstances();
	void ClearLastInstancesFromModel();
			
private:
	AnimaScene*			_scene;
	AnimaMeshesManager* _meshesManager;

	AnimaMappedArray<AnimaMeshInstance*> _meshIntances;

	AnimaArray<AnimaMeshInstance*> _lastInstancesFromModel;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshInstancesManager__) */
