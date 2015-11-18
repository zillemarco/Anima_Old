//
//  AnimaMaterialsManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaMaterialsManager__
#define __Anima__AnimaMaterialsManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaMaterial.h"
#include "AnimaArray.h"
#include "AnimaMappedArray.h"
#include "AnimaTypeMappedArray.h"
#include "AnimaTexturesManager.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaMaterialsManager
{
public:
	AnimaMaterialsManager(AnimaScene* scene, AnimaTexturesManager* texturesManager);
	~AnimaMaterialsManager();
	
public:
	AnimaMaterial* CreateMaterial(const AnimaString& name);

	bool LoadMaterials(const AnimaString& materialsPath);
	AnimaMaterial* LoadMaterialFromFile(const AnimaString& materialFilePath);
	AnimaMaterial* LoadMaterialFromXml(const AnimaString& materialXmlDefinition);

	AInt GetMaterialsCount();
	AnimaMaterial* GetMaterial(AInt index);
	AnimaMaterial* GetMaterialFromName(const AnimaString& name);

	AnimaMappedArray<AnimaMaterial*>* GetMaterials();

	static AnimaMaterial* GetDefaultMaterial() { ANIMA_ASSERT(_defaultMaterial != nullptr);  return _defaultMaterial; }
	
	void SaveMaterialToFile(const AnimaString& materialName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveMaterialToFile(AnimaMaterial* material, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveMaterials(const AnimaString& destinationPath);

	bool FinalizeObjectsAfterRead();

public:
	bool LoadMaterialsFromNode(const aiScene* scene, const AnimaString& nodeName);
	AnimaArray<AnimaString>* GetLastMaterialsIndexMap();
	void ClearLastMaterialsIndexMap();
	
private:
	void ClearMaterials();
	
private:
	AnimaScene* _scene;
	AnimaTexturesManager* _texturesManager;
	AnimaMappedArray<AnimaMaterial*> _materials;
	
	AnimaArray<AnimaString> _lastMaterialsIndexMap;

	static AnimaMaterial* _defaultMaterial;
	static AInt _instancesCount;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterialsManager__) */
