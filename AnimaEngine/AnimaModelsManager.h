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
#include "AnimaModel.h"
#include "AnimaEngine.h"
#include "AnimaString.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaModelsManager
{
public:
	AnimaModelsManager(AnimaEngine* engine);
	~AnimaModelsManager();
	
public:
	bool LoadModel(const char* modelPath);
	bool LoadModel(AnimaString& modelPath);
	void AddModel(const AnimaModel& model);
	ASizeT GetModelsNumber();
	AnimaModel GetModel(ASizeT index);
	AnimaModel* GetPModel(ASizeT index);
	AnimaModel* GetModels();
	void ClearModels();
	
	
private:
	void RecursiveLoadMesh(AnimaModel* currentModel, const aiScene *scene, const aiNode* sceneNode);
	
private:
	AnimaEngine* _engine;
	
	AnimaModel* _models;
	ASizeT		_modelsNumber;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelsManager__) */
