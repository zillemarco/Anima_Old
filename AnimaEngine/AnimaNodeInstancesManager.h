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
	AnimaNodeInstance* CreateNodeInstance(const AnimaString& instanceName, AnimaNode* srcNode);
	AnimaNodeInstance* CreateNodeInstance(const AnimaString& instanceName, const AnimaString& srcNodeName);
	
	AnimaNodeInstance* CreateAssetInstance(const AnimaString& instanceName, AnimaNode* srcAsset);
	AnimaNodeInstance* CreateAssetInstance(const AnimaString& instanceName, const AnimaString& srcAssetName);

	AInt GetNodeInstancesCount() const;
	AInt GetAssetInstancesCount() const;

	AnimaNodeInstance* GetNodeInstance(AInt index);
	AnimaNodeInstance* GetAssetInstance(AInt index);
	
	AnimaNodeInstance* GetNodeInstanceFromName(const AnimaString& name);
	AnimaNodeInstance* GetAssetInstanceFromName(const AnimaString& name);
	
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
	AnimaNodeInstance* CreateEmptyInstance(const AnimaString& instanceName, bool asset = false);
		
private:
	AnimaScene*			_scene;
	AnimaNodesManager*	_nodesManager;
	AnimaGeometryInstancesManager* _geometryInstancesManager;

	AnimaMappedArray<AnimaNodeInstance*>	_nodeInstances;
	AnimaMappedArray<AnimaNodeInstance*>	_assetInstances;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaNodeInstancesManager__) */
