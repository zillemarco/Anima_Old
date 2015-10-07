//
//  AnimaScenesManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaScenesManager__
#define __Anima__AnimaScenesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaMappedArray.h"
#include "AnimaArray.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaScenesManager
{
public:
	AnimaScenesManager(AnimaEngine* engine);
	~AnimaScenesManager();
	
	AnimaScene* CreateScene(const AnimaString& name);

	AnimaScene* GetScene(AUint index);
	AnimaScene* GetSceneFromName(const AnimaString& name);
	
	bool LoadScenes(const AnimaString& scenesPath);
	AnimaScene* LoadSceneFromFile(const AnimaString& sceneFilePath);
	AnimaScene* LoadSceneFromXml(const AnimaString& sceneXmlDefinition);
	
	void SaveSceneToFile(const AnimaString& sceneName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveSceneToFile(AnimaScene* scene, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveScenes(const AnimaString& destinationPath);
	
private:
	void ClearScenes();
	
private:
	AnimaEngine* _engine;
	AnimaMappedArray<AnimaScene*> _scenes;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScenesManager__) */
