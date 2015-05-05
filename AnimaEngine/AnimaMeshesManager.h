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
#include "AnimaMaterialsManager.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#pragma warning (disable: 4251)
typedef boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> BoneNameMap;
#pragma warning (default: 4251)

class ANIMA_ENGINE_EXPORT AnimaMeshesManager
{
public:
	AnimaMeshesManager(AnimaScene* scene, AnimaMaterialsManager* materialsManager);
	~AnimaMeshesManager();
	
public:
	bool LoadMeshesFromModel(const aiScene* scene, const AnimaString& modelName, AnimaArray<AnimaString*>* materialNamesMap);

	AnimaArray<AnimaString*>* GetLastMeshesIndexMap();
	AnimaArray<AnimaMeshBoneInfo>* GetLastMeshesBones();
	BoneNameMap* GetLastMeshesBonesNameMap();
	void ClearLastMeshesIndexMap();
	void ClearLastMeshesBonesData();

	AnimaMesh* GetMesh(AInt index);
	AnimaMesh* GetMeshFromName(const AnimaString& name);
	AnimaMesh* GetMeshFromName(const char* name);

	AInt GetMeshesCount() const;

protected:
	void ClearMeshes();

protected:
	AnimaMesh* CreateEmptyMesh(const AnimaString& name);
	AnimaMesh* CreateEmptyMesh(const char* name);

private:
	AnimaScene* _scene;
	AnimaMaterialsManager* _materialsManager;

	AnimaMappedArray<AnimaMesh*> _meshes;

#pragma warning (disable: 4251)
	AnimaArray<AnimaString*> _lastMeshesIndexMap;
	AnimaArray<AnimaMeshBoneInfo> _lastMeshesBones;
	BoneNameMap _lastMeshesBonesNameMap;
#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshesManager__) */
