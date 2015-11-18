//
//  AnimaEngineNodesManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaNodeInstancesManager__
#define __Anima__AnimaNodeInstancesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaGeometry.h"
#include "AnimaNode.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaGeometriesManager.h"
#include "AnimaNodesManager.h"
#include "AnimaNodeInstance.h"
#include "AnimaGeometryInstancesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaNodeInstancesManager
{
public:
	AnimaNodeInstancesManager(AnimaScene* scene, AnimaNodesManager* nodesManager, AnimaGeometryInstancesManager* geometryInstancesManager);
	~AnimaNodeInstancesManager();

public:
	AnimaNodeInstance* CreateInstance(const AnimaString& instanceName, AnimaNode* srcNode);
	AnimaNodeInstance* CreateInstance(const AnimaString& instanceName, const AnimaString& srcNodeName, bool topLevelNode = true);

	AInt GetNodeInstancesCount(bool topLevelNodes = true);

	AnimaNodeInstance* GetNodeInstance(AInt index, bool topLevelNode = true);
	AnimaNodeInstance* GetNodeInstanceFromName(const AnimaString& name, bool topLevelNode = true);
	
	AnimaNodeInstance* LoadNodeInstanceFromFile(const AnimaString& filePath);
	AnimaNodeInstance* LoadNodeInstanceFromXml(const AnimaString& nodeXmlDefinition);
	bool LoadNodesInstances(const AnimaString& nodesInstancesPath);
	
	void SaveNodeInstanceToFile(const AnimaString& nodeInstanceName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveNodeInstanceToFile(AnimaNodeInstance* nodeInstance, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveNodesInstances(const AnimaString& destinationPath);
	
	bool FinalizeObjectsAfterRead();

public:
	void ClearInstances();

protected:
	AnimaNodeInstance* CreateInstanceFromNode(const AnimaString& instanceName, AnimaNode* srcNode, bool useSrcNodeName = false);
	AnimaNodeInstance* CreateEmptyInstance(const AnimaString& instanceName, bool topLevelNode = false);
		
private:
	AnimaScene*			_scene;
	AnimaNodesManager*	_nodesManager;
	AnimaGeometryInstancesManager* _geometryInstancesManager;

	AnimaMappedArray<AnimaNodeInstance*>	_nodeInstances;
	AnimaMappedArray<AnimaNodeInstance*>	_topLevelNodeInstances;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaNodeInstancesManager__) */
