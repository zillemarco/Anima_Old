//
//  AnimaCamerasManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaCamerasManager__
#define __Anima__AnimaCamerasManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaCamera.h"
#include "AnimaMappedArray.h"
#include "AnimaArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaCamerasManager
{
	friend AnimaCamera;
	
public:
	AnimaCamerasManager(AnimaScene* scene);
	~AnimaCamerasManager();
	
	AnimaCamera* CreateCamera(const AnimaString& name);
	
	AnimaCamera* GetActiveCamera();
	AnimaCamera* GetCameraFromName(const AnimaString& name);
	
	AInt GetCamerasCount();

	void UpdatePerspectiveCameras(float fov, const AnimaVertex2f& size, float zNear, float zFar);
	void UpdateOrthoCameras(float left, float right, float bottom, float top, float zNear, float zFar);
	
	AnimaCamera* LoadCameraFromFile(const AnimaString& filePath);
	AnimaCamera* LoadCameraFromXml(const AnimaString& cameraXmlDefinition);
	bool LoadCameras(const AnimaString& camerasPath);
	
	void SaveCameraToFile(const AnimaString& cameraName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveCameraToFile(AnimaCamera* camera, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveCameras(const AnimaString& destinationPath);
	
	bool FinalizeObjectsAfterRead();
	
	void UpdateCameras(AFloat elapsedTime);
	
private:
	void ClearCameras();
	
	void NotifyCameraActivation(AnimaCamera* camera);
	void NotifyCameraDeactivation(AnimaCamera* camera);
	
private:
	AnimaScene* _scene;
	AnimaMappedArray<AnimaCamera*> _cameras;
	
	AnimaCamera*	_activeCamera;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamerasManager__) */
