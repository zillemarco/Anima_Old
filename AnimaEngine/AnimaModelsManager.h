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

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaModelsManager
{
public:
	AnimaModelsManager(AnimaScene* scene, AnimaMeshesManager* meshesManager, AnimaMaterialsManager* materialsManager, AnimaAnimationsManager* animationsManager);
	~AnimaModelsManager();
	
public:
	AnimaModel* LoadModel(const char* modelPath, const AnimaString& name);
	AnimaModel* LoadModel(const char* modelPath, const char* name);
	AnimaModel* LoadModel(const AnimaString& modelPath, const AnimaString& name);
	AnimaModel* LoadModel(const AnimaString& modelPath, const char* name);

	AnimaModel* CreateModel(const AnimaString& name);
	AnimaModel* CreateModel(const char* name);

	AInt GetModelsNumber();

	AnimaModel* GetModel(AInt index);
	AnimaModel* GetModelFromName(const AnimaString& name);
	AnimaModel* GetModelFromName(const char* name);
	
	void ClearModels();
	
private:
	AnimaModel* LoadModelFromScene(const aiScene* scene, const aiNode* sceneNode, AnimaArray<AnimaString*>* meshesMap, const AnimaString& modelName);

private:
	AnimaScene* _scene;

	AnimaMeshesManager*		_meshesManager;
	AnimaMaterialsManager*	_materialsManager;
	AnimaAnimationsManager* _animationsManager;

	AnimaArray<AnimaModel*>			_models;
	AnimaMappedArray<AnimaModel*>	_topLevelModels;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelsManager__) */
