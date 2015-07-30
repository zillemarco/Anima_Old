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
typedef boost::unordered_map<AnimaString, AUint, AnimaStringHasher> BoneNameMap;
#pragma warning (default: 4251)

class ANIMA_ENGINE_EXPORT AnimaMeshesManager
{
public:
	AnimaMeshesManager(AnimaScene* scene, AnimaMaterialsManager* materialsManager);
	~AnimaMeshesManager();
	
public:
	bool LoadMeshesFromModel(const aiScene* scene, const AnimaString& modelName/*, AnimaArray<AnimaString>* materialNamesMap*/);

	AnimaArray<AnimaString>* GetLastMeshesIndexMap();
	AnimaMappedArray<AnimaMeshBoneInfo*>* GetLastMeshesBonesInfo();
	void ClearLastMeshesIndexMap();
	void ClearLastMeshesBonesData();

	AnimaMesh* GetMesh(AInt index);
	AnimaMesh* GetMeshFromName(const AnimaString& name);

	AInt GetMeshesCount() const;

	AnimaMesh* LoadMeshFromFile(const AnimaString& meshFilePath);
	AnimaMesh* LoadMeshFromXml(const AnimaString& meshXmlDefinition);
	void LoadMeshes(const AnimaString& meshesPath);

	void SaveMeshToFile(const AnimaString& meshName, const AnimaString& filePath);
	void SaveMeshToFile(AnimaMesh* mesh, const AnimaString& filePath);

protected:
	void ClearMeshes();

protected:
	AnimaMesh* CreateEmptyMesh(const AnimaString& name);

private:
	AnimaScene* _scene;
	AnimaMaterialsManager* _materialsManager;

	AnimaMappedArray<AnimaMesh*> _meshes;
	AnimaArray<AnimaString> _lastMeshesIndexMap;

	AnimaMappedArray<AnimaMeshBoneInfo*> _lastMeshesBonesInfo;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshesManager__) */
