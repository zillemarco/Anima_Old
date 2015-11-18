//
//  AnimaEngineNodesManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaNodesManager__
#define __Anima__AnimaNodesManager__

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
#include "AnimaAnimationsManager.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <boost/property_tree/ptree.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaNodesManager
{
public:
	AnimaNodesManager(AnimaScene* scene, AnimaGeometriesManager* geometriesManager, AnimaMaterialsManager* materialsManager, AnimaAnimationsManager* animationsManager);
	~AnimaNodesManager();
	
public:
	AnimaNode* CreateNode(const AnimaString& name, bool topLevelNode = true);

	AnimaNode* LoadNodeFromFile(const AnimaString& filePath);
	AnimaNode* LoadNodeFromXml(const AnimaString& nodeXmlDefinition);
	AnimaNode* LoadNodeFromExternalFile(const AnimaString& nodePath, const AnimaString& name);
	bool LoadNodes(const AnimaString& nodesPath);
	
	AInt GetNodesCount(bool topLevelNodes = true);

	AnimaNode* GetNode(AInt index, bool topLevelNode = true);
	AnimaNode* GetNodeFromName(const AnimaString& name, bool topLevelNode = true);
	
	void ClearNodes();
	
	void SaveNodeToFile(const AnimaString& nodeName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveNodeToFile(AnimaNode* node, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveNodes(const AnimaString& destinationPath);

	bool FinalizeObjectsAfterRead();

private:
	AnimaNode* LoadNodeFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString>* geometriesMap, const AnimaString& nodeName);

	boost::property_tree::ptree GetNodeTree(AnimaNode* node);
	AnimaNode* LoadNodeFromTree(boost::property_tree::ptree* tree);

private:
	AnimaScene* _scene;

	AnimaGeometriesManager*		_geometriesManager;
	AnimaMaterialsManager*	_materialsManager;
	AnimaAnimationsManager* _animationsManager;

	AnimaMappedArray<AnimaNode*>	_nodes;
	AnimaMappedArray<AnimaNode*>	_topLevelNodes;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaNodesManager__) */
