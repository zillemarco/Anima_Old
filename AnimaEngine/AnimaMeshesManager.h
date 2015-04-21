//
//  AnimaMeshesManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaMeshesManager__
#define __Anima__AnimaMeshesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaMesh.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMeshesManager
{
public:
	AnimaMeshesManager(AnimaScene* scene);
	~AnimaMeshesManager();
	
public:
	bool LoadMeshesFromModel(const aiScene* scene, const AnimaString& modelName);

	AnimaArray<AnimaString*>* GetLastMeshesIndexMap();
	void ClearLastMeshesIndexMap();

	AnimaMesh* GetMesh(const AnimaString& name);

protected:
	void ClearMeshes();

private:
	AnimaScene* _scene;
	AnimaMappedArray<AnimaMesh*> _meshes;

	AnimaArray<AnimaString*> _lastMeshesIndexMap;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshesManager__) */