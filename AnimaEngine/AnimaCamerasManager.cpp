//
//  AnimaCamerasManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaCamerasManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaCamerasManager::AnimaCamerasManager(AnimaStage* stage)
{
	_stage = stage;
	
	_cameras = nullptr;
	_camerasNumber = 0;
	
	_activeCamera = nullptr;
}

AnimaCamerasManager::~AnimaCamerasManager()
{
	ClearCameras();
}

AnimaFirstPersonCamera* AnimaCamerasManager::CreateNewFirstPersonCamera(const AnimaString& name)
{
	if (_camerasMap.find(name) != _camerasMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_camerasNumber > 0)
	{
		AnimaCamera** tmpOldCameras = AnimaAllocatorNamespace::AllocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _camerasNumber);
		
		for (int i = 0; i < _camerasNumber; i++)
			tmpOldCameras[i] = _cameras[i];
		
		ClearCameras(false, false);
		
		_camerasNumber++;
		_cameras = AnimaAllocatorNamespace::AllocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _camerasNumber);
		
		for (int i = 0; i < _camerasNumber - 1; i++)
			_cameras[i] = tmpOldCameras[i];
		
		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetCamerasAllocator()), tmpOldCameras);
		tmpOldCameras = nullptr;
	}
	else
	{
		_camerasNumber++;
		_cameras = AnimaAllocatorNamespace::AllocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _camerasNumber);
	}
	
	_cameras[_camerasNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*(_stage->GetCamerasAllocator()), _stage->GetCamerasAllocator(), this);

	_camerasMap[name] = (AUint)(_camerasNumber - 1);

	return (AnimaFirstPersonCamera*)_cameras[_camerasNumber - 1];
}

AnimaFirstPersonCamera* AnimaCamerasManager::CreateNewFirstPersonCamera(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreateNewFirstPersonCamera(str);
}

AnimaThirdPersonCamera* AnimaCamerasManager::CreateNewThirdPersonCamera(const AnimaString& name)
{
	if (_camerasMap.find(name) != _camerasMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_camerasNumber > 0)
	{
		AnimaCamera** tmpOldCameras = AnimaAllocatorNamespace::AllocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _camerasNumber);

		for (int i = 0; i < _camerasNumber; i++)
			tmpOldCameras[i] = _cameras[i];

		ClearCameras(false, false);

		_camerasNumber++;
		_cameras = AnimaAllocatorNamespace::AllocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _camerasNumber);

		for (int i = 0; i < _camerasNumber - 1; i++)
			_cameras[i] = tmpOldCameras[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetCamerasAllocator()), tmpOldCameras);
		tmpOldCameras = nullptr;
	}
	else
	{
		_camerasNumber++;
		_cameras = AnimaAllocatorNamespace::AllocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _camerasNumber);
	}

	_cameras[_camerasNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaThirdPersonCamera>(*(_stage->GetCamerasAllocator()), _stage->GetCamerasAllocator(), this);

	_camerasMap[name] = (AUint)(_camerasNumber - 1);

	return (AnimaThirdPersonCamera*)_cameras[_camerasNumber - 1];
}

AnimaThirdPersonCamera* AnimaCamerasManager::CreateNewThirdPersonCamera(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return CreateNewThirdPersonCamera(str);
}

void AnimaCamerasManager::ClearCameras(bool bDeleteObjects, bool bResetNumber)
{
	if (_cameras != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_camerasNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_stage->GetCamerasAllocator()), _cameras[i]);
				_cameras[i] = nullptr;
			}
		}
		
		AnimaAllocatorNamespace::DeallocateArray<AnimaCamera*>(*(_stage->GetCamerasAllocator()), _cameras);
		_cameras = nullptr;
	}
	
	if (bResetNumber)
		_camerasNumber = 0;
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
	for(ASizeT i = 0; _activeCamera == nullptr && i < _camerasNumber; i++)
	{
		if(_cameras[i]->IsActive())
		{
			_activeCamera = _cameras[i];
		}
	}
	
	// Se ho trovato la camera attiva, torno il suo puntatore
	if (_activeCamera != nullptr)
		return _activeCamera;

	// Se anche dopo il ciclo non si è trovata una camera attiva ed esiste almeno una camera
	// attivo quella camera e torno _activeCamera che verrà aggiornato da NotifyCameraActivation
	if(_activeCamera == nullptr && _camerasNumber > 0)
	{
		_cameras[0]->Activate();
		return _activeCamera;
	}
	
	// Se non esistono nemmeno camere, ovviamente, torno null
	return nullptr;
}

void AnimaCamerasManager::UpdatePerspectiveCameras(float fov, const AnimaVertex2f& size, float zNear, float zFar)
{
	for (ASizeT i = 0; i < _camerasNumber; i++)
	{
		if (_cameras[i]->IsPerspectiveProjectionType())
			_cameras[i]->CalculateProjectionMatrix(fov, size, zNear, zFar);
	}
}

void AnimaCamerasManager::UpdateOrthoCameras(float left, float right, float bottom, float top, float zNear, float zFar)
{
	for (ASizeT i = 0; i < _camerasNumber; i++)
	{
		if (_cameras[i]->IsOrthoProjectionType())
			_cameras[i]->CalculateProjectionMatrix(left, right, bottom, top, zNear, zFar);
	}
}

AnimaCamera* AnimaCamerasManager::GetCamera(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _camerasNumber);
	return _cameras[index];
}

AnimaCamera* AnimaCamerasManager::GetCameraFromName(const AnimaString& name)
{
	if (_camerasMap.find(name) == _camerasMap.end())
		return nullptr;
	return GetCamera(_camerasMap[name]);
}

AnimaCamera* AnimaCamerasManager::GetCameraFromName(const char* name)
{
	AnimaString str(name, _stage->GetStringAllocator());
	return GetCameraFromName(str);
}

END_ANIMA_ENGINE_NAMESPACE