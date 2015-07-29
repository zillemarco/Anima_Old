//
//  AnimaEngineModelsManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaModelsManager__
#define __Anima__AnimaModelsManager__

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
#include "AnimaAnimationsManager.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <boost/property_tree/ptree.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaModelsManager
{
public:
	AnimaModelsManager(AnimaScene* scene, AnimaMeshesManager* meshesManager, AnimaMaterialsManager* materialsManager, AnimaAnimationsManager* animationsManager);
	~AnimaModelsManager();
	
public:
	AnimaModel* CreateModel(const AnimaString& name);

	AnimaModel* LoadModelFromFile(const AnimaString& filePath);
	AnimaModel* LoadModelFromXml(const AnimaString& modelXmlDefinition);
	AnimaModel* LoadModelFromExternalFile(const AnimaString& modelPath, const AnimaString& name);
	
	AInt GetModelsCount();

	AnimaModel* GetModel(AInt index);
	AnimaModel* GetModelFromName(const AnimaString& name);
	
	void ClearModels();
	
	void SaveModelToFile(const AnimaString& modelName, const AnimaString& filePath);
	void SaveModelToFile(AnimaModel* model, const AnimaString& filePath);

private:
	AnimaModel* LoadModelFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString>* meshesMap, const AnimaString& modelName);

	boost::property_tree::ptree GetModelTree(AnimaModel* model);
	AnimaModel* LoadModelFromTree(boost::property_tree::ptree* tree);

private:
	AnimaScene* _scene;

	AnimaMeshesManager*		_meshesManager;
	AnimaMaterialsManager*	_materialsManager;
	AnimaAnimationsManager* _animationsManager;

	AnimaMappedArray<AnimaModel*>	_models;
	AnimaMappedArray<AnimaModel*>	_topLevelModels;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelsManager__) */
