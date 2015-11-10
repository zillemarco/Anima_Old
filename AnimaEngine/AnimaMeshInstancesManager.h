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

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaMeshInstancesManager
{
public:
	AnimaMeshInstancesManager(AnimaScene* scene, AnimaMeshesManager* meshesManager);
	~AnimaMeshInstancesManager();

public:
	AnimaMeshInstance* CreateInstance(const AnimaString& instanceName, AnimaMesh* srcMesh);
	AnimaMeshInstance* CreateInstance(const AnimaString& instanceName, const AnimaString& srcMeshName);

	AnimaArray<AnimaMeshInstance*>* CreateInstances(AnimaModel* srcModel);

	AInt GetMeshInstancesCount();

	AnimaMeshInstance* GetMeshInstance(AInt index);
	AnimaMeshInstance* GetMeshInstanceFromName(const AnimaString& name);
	
	AnimaMeshInstance* LoadMeshInstanceFromFile(const AnimaString& filePath);
	AnimaMeshInstance* LoadMeshInstanceFromXml(const AnimaString& meshXmlDefinition);
	bool LoadMeshesInstances(const AnimaString& meshesInstancesPath);
	
	void SaveMeshInstanceToFile(const AnimaString& meshInstanceName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveMeshInstanceToFile(AnimaMeshInstance* meshInstance, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveMeshesInstances(const AnimaString& destinationPath);
	
	bool FinalizeObjectsAfterRead();

protected:
	AnimaMeshInstance* CreateEmptyInstance(const AnimaString& instanceName);

public:
	void ClearInstances();
	void ClearLastInstancesFromModel();
			
private:
	AnimaScene*			_scene;
	AnimaMeshesManager* _meshesManager;

	AnimaMappedArray<AnimaMeshInstance*> _meshInstances;

	AnimaArray<AnimaMeshInstance*> _lastInstancesFromModel;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshInstancesManager__) */
