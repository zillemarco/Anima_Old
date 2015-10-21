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
#include "AnimaTypeMappedArray.h"
#include "AnimaMappedArray.h"
#include "AnimaArray.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaCamerasManager
{
	friend AnimaCamera;
	friend AnimaFirstPersonCamera;
	friend AnimaThirdPersonCamera;
	
public:
	AnimaCamerasManager(AnimaScene* scene);
	~AnimaCamerasManager();
	
	template<class T> T* CreateCamera(const AnimaString& name);
	
	template<class T> AnimaArray<AnimaCamera*>* GetCamerasArrayOfType();
	template<class T> AnimaCamera* GetCameraOfTypeFromName(const AnimaString& name);
	
	AnimaFirstPersonCamera* CreateFirstPersonCamera(const AnimaString& name);
	AnimaThirdPersonCamera* CreateThirdPersonCamera(const AnimaString& name);
	
	AnimaCamera* GetActiveCamera();
	AnimaCamera* GetCameraFromName(const AnimaString& name);
	
	AInt GetTotalCamerasCount();
	AnimaTypeMappedArray<AnimaCamera*>* GetCameras();

	void UpdatePerspectiveCameras(float fov, const AnimaVertex2f& size, float zNear, float zFar);
	void UpdateOrthoCameras(float left, float right, float bottom, float top, float zNear, float zFar);
	
	AnimaCamera* LoadCameraFromFile(const AnimaString& filePath);
	AnimaCamera* LoadCameraFromXml(const AnimaString& cameraXmlDefinition);
	bool LoadCameras(const AnimaString& camerasPath);
	
//	AnimaFirstPersonCamera* LoadFirstPersonCameraFromFile(const AnimaString& filePath);
//	AnimaFirstPersonCamera* LoadFirstPersonCameraFromXml(const AnimaString& firstPersonCameraXmlDefinition);
//	bool LoadFirstPersonCameras(const AnimaString& firstPersonCamerasPath);
//	
//	AnimaThirdPersonCamera* LoadThirdPersonCameraFromFile(const AnimaString& filePath);
//	AnimaThirdPersonCamera* LoadThirdPersonCameraFromXml(const AnimaString& thirdPersonCameraXmlDefinition);
//	bool LoadThirdPersonCameras(const AnimaString& thirdPersonCamerasPath);
	
	void SaveCameraToFile(const AnimaString& cameraName, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveCameraToFile(AnimaCamera* camera, const AnimaString& destinationPath, bool createFinalPath = false);
	void SaveCameras(const AnimaString& destinationPath);
	
	bool FinalizeObjectsAfterRead();
	
private:
	void ClearCameras();
	
	void NotifyCameraActivation(AnimaCamera* camera);
	void NotifyCameraDeactivation(AnimaCamera* camera);
	
private:
	AnimaScene* _scene;
	AnimaTypeMappedArray<AnimaCamera*> _cameras;
	
	AnimaCamera*	_activeCamera;
};

template<class T>
T* AnimaCamerasManager::CreateCamera(const AnimaString& name)
{
	AnimaCamera* camera = _cameras.Contains(name);
	if (camera != nullptr)
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	T* newCamera = AnimaAllocatorNamespace::AllocateNew<T>(*(_scene->GetCamerasAllocator()), _scene->GetCamerasAllocator(), this, _scene->GetDataGeneratorsManager(), name);
	_cameras.Add<T*>(name, newCamera);
	
	return newCamera;
}

template<class T>
AnimaArray<AnimaCamera*>* AnimaCamerasManager::GetCamerasArrayOfType()
{
	return _cameras.GetMappedArrayArrayOfType<T*>();
}

template<class T>
AnimaCamera* AnimaCamerasManager::GetCameraOfTypeFromName(const AnimaString& name)
{
	return _cameras.GetWithNameAndType<T*>(name);
}

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaCamerasManager__) */
