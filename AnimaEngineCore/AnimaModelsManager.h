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

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaModelsManager
{
public:
	AnimaModelsManager(AnimaAllocator* allocator);
	~AnimaModelsManager();
	
public:
	bool LoadModel(const char* modelPath);
	
private:
	void RecursiveLoadMesh(AnimaModel* currentModel, const aiScene *scene, const aiNode* sceneNode);
	
private:
	AnimaAllocator* _allocator;
	
	AnimaModel* _models;
	ASizeT		_modelsNumber;
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif /* defined(__Anima__AnimaModelsManager__) */
