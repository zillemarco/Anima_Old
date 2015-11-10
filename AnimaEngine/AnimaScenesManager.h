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
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaScenesManager
{
	friend AnimaScene;
	
public:
	AnimaScenesManager(AnimaEngine* engine);
	~AnimaScenesManager();
	
	AnimaScene* CreateScene(const AnimaString& name);

	AnimaScene* GetScene(AUint index);
	AnimaScene* GetSceneFromName(const AnimaString& name);
	
	bool LoadScenes(const AnimaString& scenesPath);
	AnimaScene* LoadSceneFromFile(const AnimaString& sceneFilePath);
	AnimaScene* LoadSceneFromXml(const AnimaString& sceneXmlDefinition, const AnimaString& sceneFoldersPath);
	
	void SaveSceneToFile(const AnimaString& sceneName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveSceneToFile(AnimaScene* scene, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveScenes(const AnimaString& destinationPath);
	
	AnimaScene* GetActiveScene();
	
private:
	void ClearScenes();
	
	void NotifySceneActivation(AnimaScene* scene);
	void NotifySceneDeactivation(AnimaScene* scene);
	
private:
	AnimaEngine* _engine;
	AnimaMappedArray<AnimaScene*> _scenes;
	
	AnimaScene* _activeScene;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScenesManager__) */
