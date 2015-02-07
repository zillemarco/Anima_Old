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
#include "AnimaFirstPersonCamera.h"
#include "AnimaThirdPersonCamera.h"
#include "AnimaStage.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaCamerasManager
{
	friend AnimaCamera;
	friend AnimaFirstPersonCamera;
	friend AnimaThirdPersonCamera;
	
public:
	AnimaCamerasManager(AnimaStage* stage);
	~AnimaCamerasManager();
	
	AnimaFirstPersonCamera* CreateNewFirstPersonCamera(const AnimaString& name);
	AnimaFirstPersonCamera* CreateNewFirstPersonCamera(const char* name);
	AnimaThirdPersonCamera* CreateNewThirdPersonCamera(const AnimaString& name);
	AnimaThirdPersonCamera* CreateNewThirdPersonCamera(const char* name);
	
	AnimaCamera* GetActiveCamera();
	AnimaCamera* GetCamera(ASizeT index);
	AnimaCamera* GetCameraFromName(const AnimaString& name);
	AnimaCamera* GetCameraFromName(const char* name);

	void UpdatePerspectiveCameras(float fov, const AnimaVertex2f& size, float zNear, float zFar);
	void UpdateOrthoCameras(float left, float right, float bottom, float top, float zNear, float zFar);
	
private:
	void ClearCameras(bool bDeleteObjects = true, bool bResetNumber = true);
	
	void NotifyCameraActivation(AnimaCamera* camera);
	void NotifyCameraDeactivation(AnimaCamera* camera);
	
private:
	AnimaStage* _stage;
	
	AnimaCamera**	_cameras;
	ASizeT			_camerasNumber;
	
	AnimaCamera*	_activeCamera;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _camerasMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamerasManager__) */
