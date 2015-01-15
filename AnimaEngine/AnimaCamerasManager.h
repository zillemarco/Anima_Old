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
#include "AnimaFPCamera.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaCamerasManager
{
	friend AnimaCamera;
	friend AnimaFPCamera;
	
public:
	AnimaCamerasManager(AnimaEngine* engine);
	~AnimaCamerasManager();
	
	AnimaFPCamera* CreateNewFPCamera();
	
	AnimaCamera* GetActiveCamera();
	
private:
	void ClearCameras(bool bDeleteObjects = true, bool bResetNumber = true);
	
	void NotifyCameraActivation(AnimaCamera* camera);
	void NotifyCameraDeactivation(AnimaCamera* camera);
	
private:
	AnimaEngine* _engine;
	
	AnimaCamera**	_cameras;
	ASizeT			_camerasNumber;
	
	AnimaCamera*	_activeCamera;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamerasManager__) */
