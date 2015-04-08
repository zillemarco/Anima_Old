//
//  AnimaCamerasManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaCamerasManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaCamerasManager::AnimaCamerasManager(AnimaEngine* engine)
: _cameras(engine->GetCamerasAllocator())
{
	_engine = engine;
	_activeCamera = nullptr;
}

AnimaCamerasManager::~AnimaCamerasManager()
{
	ClearCameras();
}

AnimaFirstPersonCamera* AnimaCamerasManager::CreateFirstPersonCamera(const AnimaString& name)
{
	return CreateCamera<AnimaFirstPersonCamera>(name);
}

AnimaFirstPersonCamera* AnimaCamerasManager::CreateFirstPersonCamera(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return CreateFirstPersonCamera(str);
}

AnimaThirdPersonCamera* AnimaCamerasManager::CreateThirdPersonCamera(const AnimaString& name)
{
	return CreateCamera<AnimaThirdPersonCamera>(name);
}

AnimaThirdPersonCamera* AnimaCamerasManager::CreateThirdPersonCamera(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return CreateThirdPersonCamera(str);
}

void AnimaCamerasManager::ClearCameras()
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaString::Hasher>* camerasMap = _cameras.GetArraysMap();
	for (auto camerasPair : (*camerasMap))
	{
		AnimaMappedArray<AnimaCamera*>* camerasArray = camerasPair.second;
		AInt count = camerasArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaCamera* camera = (*camerasArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetCamerasAllocator()), camera);
			camera = nullptr;
		}
	}
	
	_cameras.RemoveAll();
}

void AnimaCamerasManager::NotifyCameraActivation(AnimaCamera* camera)
{
	if(_activeCamera != nullptr)
		_activeCamera->Deactivate();
	_activeCamera = camera;
}

void AnimaCamerasManager::NotifyCameraDeactivation(AnimaCamera* camera)
{
	_activeCamera = nullptr;
}

AnimaCamera* AnimaCamerasManager::GetActiveCamera()
{
	// Ciclo per trovare una camera attiva
	// _activeCamera non dovrebbe mai essere null quindi il più delle volte il ciclo non viene nemmeno eseguito
	AnimaCamera* firstCamera = nullptr;
	if(_activeCamera == nullptr)
	{
		boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaString::Hasher>* camerasMap = _cameras.GetArraysMap();
		for (auto camerasPair : (*camerasMap))
		{
			AnimaMappedArray<AnimaCamera*>* camerasArray = camerasPair.second;
			AInt count = camerasArray->GetSize();
			for (AInt i = 0; i < count && _activeCamera == nullptr; i++)
			{
				AnimaCamera* camera = (*camerasArray)[i];
				
				if(firstCamera == nullptr)
					firstCamera = camera;
				
				if(camera->IsActive())
					_activeCamera = camera;
			}
			
			if(_activeCamera != nullptr)
				break;
		}
	}
	
	// Se ho trovato la camera attiva, torno il suo puntatore
	if (_activeCamera != nullptr)
		return _activeCamera;

	// Se anche dopo il ciclo non si è trovata una camera attiva ed esiste almeno una camera
	// attivo quella camera e torno _activeCamera che verrà aggiornato da NotifyCameraActivation
	if(_activeCamera == nullptr && firstCamera != nullptr)
	{
		firstCamera->Activate();
		return _activeCamera;
	}
	
	// Se non esistono nemmeno camere, ovviamente, torno null
	return nullptr;
}

void AnimaCamerasManager::UpdatePerspectiveCameras(float fov, const AnimaVertex2f& size, float zNear, float zFar)
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaString::Hasher>* camerasMap = _cameras.GetArraysMap();
	for (auto camerasPair : (*camerasMap))
	{
		AnimaMappedArray<AnimaCamera*>* camerasArray = camerasPair.second;
		AInt count = camerasArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaCamera* camera = (*camerasArray)[i];
			
			if(camera->IsPerspectiveProjectionType())
				camera->CalculateProjectionMatrix(fov, size, zNear, zFar);
		}
	}
}

void AnimaCamerasManager::UpdateOrthoCameras(float left, float right, float bottom, float top, float zNear, float zFar)
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaString::Hasher>* camerasMap = _cameras.GetArraysMap();
	for (auto camerasPair : (*camerasMap))
	{
		AnimaMappedArray<AnimaCamera*>* camerasArray = camerasPair.second;
		AInt count = camerasArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaCamera* camera = (*camerasArray)[i];
			
			if(camera->IsOrthoProjectionType())
				camera->CalculateProjectionMatrix(left, right, bottom, top, zNear, zFar);
		}
	}
}

AnimaCamera* AnimaCamerasManager::GetCameraFromName(const AnimaString& name)
{
	return _cameras.GetWithName(name);
}

AnimaCamera* AnimaCamerasManager::GetCameraFromName(const char* name)
{
	AnimaString str(name, _engine->GetStringAllocator());
	return GetCameraFromName(str);
}

AInt AnimaCamerasManager::GetTotalCamerasCount()
{
	AInt count = 0;
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaString::Hasher>* camerasMap = _cameras.GetArraysMap();
	for (auto camerasPair : (*camerasMap))
		count += camerasPair.second->GetSize();
	
	return count;
}

AnimaTypeMappedArray<AnimaCamera*>* AnimaCamerasManager::GetCameras()
{
	return &_cameras;
}

END_ANIMA_ENGINE_NAMESPACE