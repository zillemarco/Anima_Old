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
	AnimaModelsManager(AnimaScene* scene);
	~AnimaModelsManager();
	
public:
	AnimaMesh* LoadModel(const char* modelPath, const AnimaString& name);
	AnimaMesh* LoadModel(const char* modelPath, const char* name);
	AnimaMesh* LoadModel(const AnimaString& modelPath, const AnimaString& name);
	AnimaMesh* LoadModel(const AnimaString& modelPath, const char* name);
	void AddModel(AnimaMesh& model, const AnimaString& name);
	void AddModel(AnimaMesh& model, const char* name);

	AnimaMesh* CreatePlane(const AnimaString& name);
	AnimaMesh* CreatePlane(const char* name);

	AnimaMesh* CreateModel(const AnimaString& name);
	AnimaMesh* CreateModel(const char* name);

	ASizeT GetModelsNumber();

	AnimaMesh* GetModel(ASizeT index);
	AnimaMesh* GetModelFromName(const AnimaString& name);
	AnimaMesh* GetModelFromName(const char* name);
	
	AnimaMesh* GetModels();

	void ClearModels();
	
	ASizeT GetNextModelID();
	
private:
	void RecursiveLoadMesh(AnimaMesh* currentModel, const aiScene *scene, const aiNode* sceneNode);
	void LoadMaterial(AnimaMesh* mesh, const aiMaterial* mtl);
	
private:
	AnimaScene* _scene;
	
	AnimaMesh*	_models;
	ASizeT		_modelsNumber;
	
	ASizeT		_nextModelID;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _modelsMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaModelsManager__) */
