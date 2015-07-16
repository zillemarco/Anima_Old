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
#include "AnimaScene.h"

#include <assimp/scene.h>
#include <assimp/vector3.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

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

	inline AnimaMaterial* GetDefaultMaterial() { return _defaultMaterial; }

	AnimaMappedArray<AnimaMaterial*>* GetMaterials();

public:
	bool LoadMaterialsFromModel(const aiScene* scene, const AnimaString& modelName);
	AnimaArray<AnimaString>* GetLastMaterialsIndexMap();
	void ClearLastMaterialsIndexMap();
	
private:
	void ClearMaterials();
	
private:
	AnimaScene* _scene;
	AnimaTexturesManager* _texturesManager;
	AnimaMappedArray<AnimaMaterial*> _materials;

	AnimaMaterial* _defaultMaterial;

	AnimaArray<AnimaString> _lastMaterialsIndexMap;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMaterialsManager__) */
