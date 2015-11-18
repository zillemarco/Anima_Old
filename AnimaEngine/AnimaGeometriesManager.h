//
//  AnimaGeometriesManager.h
//  Anima
//
//  Created by Marco Zille on 20/11/14.
//
//

#ifndef __Anima__AnimaGeometriesManager__
#define __Anima__AnimaGeometriesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaGeometry.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaMaterialsManager.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

#pragma warning (disable: 4251)
typedef boost::unordered_map<AnimaString, AUint, AnimaStringHasher> BoneNameMap;
#pragma warning (default: 4251)

class ANIMA_ENGINE_EXPORT AnimaGeometriesManager
{
public:
	AnimaGeometriesManager(AnimaScene* scene, AnimaMaterialsManager* materialsManager);
	~AnimaGeometriesManager();
	
public:
	bool LoadGeometriesFromNode(const aiScene* scene, const AnimaString& nodeName/*, AnimaArray<AnimaString>* materialNamesMap*/);

	AnimaGeometry* CreateGeometry(const AnimaString& name);

	AnimaArray<AnimaString>* GetLastGeometriesIndexMap();
	AnimaMappedArray<AnimaGeometryBoneInfo*>* GetLastGeometriesBonesInfo();
	void ClearLastGeometriesIndexMap();
	void ClearLastGeometriesBonesData();

	AnimaGeometry* GetGeometry(AInt index);
	AnimaGeometry* GetGeometryFromName(const AnimaString& name);

	AInt GetGeometriesCount() const;

	AnimaGeometry* LoadGeometryFromFile(const AnimaString& geometryFilePath);
	AnimaGeometry* LoadGeometryFromXml(const AnimaString& geometryXmlDefinition);
	bool LoadGeometries(const AnimaString& geometriesPath);

	void SaveGeometryToFile(const AnimaString& geometryName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveGeometryToFile(AnimaGeometry* geometry, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveGeometries(const AnimaString& destinationPath);

	bool FinalizeObjectsAfterRead();

protected:
	void ClearGeometries();

private:
	AnimaScene* _scene;
	AnimaMaterialsManager* _materialsManager;

	AnimaMappedArray<AnimaGeometry*> _geometries;
	AnimaArray<AnimaString> _lastGeometriesIndexMap;

	AnimaMappedArray<AnimaGeometryBoneInfo*> _lastGeometriesBonesInfo;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaGeometriesManager__) */
