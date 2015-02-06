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
	AnimaModel* LoadModel(const char* modelPath, const AnimaString& name);
	AnimaModel* LoadModel(const char* modelPath, const char* name);
	AnimaModel* LoadModel(const AnimaString& modelPath, const AnimaString& name);
	AnimaModel* LoadModel(const AnimaString& modelPath, const char* name);
	void AddModel(const AnimaModel& model, const AnimaString& name);
	void AddModel(const AnimaModel& model, const char* name);

	ASizeT GetModelsNumber();

//	AnimaModel GetModel(ASizeT index);
	AnimaModel* GetPModel(ASizeT index);
//	AnimaModel GetModelFromName(const AnimaString& name);
//	AnimaModel GetModelFromName(const char* name);
	AnimaModel* GetPModelFromName(const AnimaString& name);
	AnimaModel* GetPModelFromName(const char* name);
	
	AnimaModel* GetModels();

	void ClearModels();
	
	ASizeT GetNextModelID();
	
private:
	void RecursiveLoadMesh(AnimaModel* currentModel, const aiScene *scene, const aiNode* sceneNode);
	void LoadMaterial(AnimaMesh* mesh, const aiMaterial* mtl);
	
private:
	AnimaEngine* _engine;
	
	AnimaModel* _models;
	ASizeT		_modelsNumber;
	
	ASizeT		_nextModelID;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _modelsMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelsManager__) */
