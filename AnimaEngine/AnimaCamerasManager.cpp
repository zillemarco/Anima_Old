//
//  AnimaCamerasManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaCamerasManager.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaCamerasManager::AnimaCamerasManager(AnimaScene* scene)
{
	_scene = scene;
	_activeCamera = nullptr;
}

AnimaCamerasManager::~AnimaCamerasManager()
{
	ClearCameras();
}

AnimaCamera* AnimaCamerasManager::CreateCamera(const AnimaString& name)
{
	if(_cameras.Contains(name) >= 0)
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	AnimaCamera* newCamera = AnimaAllocatorNamespace::AllocateNew<AnimaCamera>(*(_scene->GetCamerasAllocator()), _scene->GetCamerasAllocator(), this, _scene->GetDataGeneratorsManager(), name);
	_cameras.Add(name, newCamera);
	
	return newCamera;
}

void AnimaCamerasManager::ClearCameras()
{
	AInt count = _cameras.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaCamera* camera = _cameras[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetCamerasAllocator()), camera);
		camera = nullptr;
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
		AInt count = _cameras.GetSize();
		for (AInt i = 0; i < count && _activeCamera == nullptr; i++)
		{
			AnimaCamera* camera = _cameras[i];
				
			if(firstCamera == nullptr)
				firstCamera = camera;
				
			if(camera->IsActive())
				_activeCamera = camera;
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
	AInt count = _cameras.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaCamera* camera = _cameras[i];
			
		if(camera->IsPerspectiveProjectionType())
			camera->CalculateProjectionMatrix(fov, size, zNear, zFar);
	}
}

void AnimaCamerasManager::UpdateOrthoCameras(float left, float right, float bottom, float top, float zNear, float zFar)
{
	AInt count = _cameras.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaCamera* camera = _cameras[i];
		
		if(camera->IsOrthographicProjectionType())
			camera->CalculateProjectionMatrix(left, right, bottom, top, zNear, zFar);
	}
}

AnimaCamera* AnimaCamerasManager::GetCameraFromName(const AnimaString& name)
{
	return _cameras[name];
}

AInt AnimaCamerasManager::GetCamerasCount()
{
	return _cameras.GetSize();
}

AnimaCamera* AnimaCamerasManager::LoadCameraFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadCameraFromXml(xml);
}

AnimaCamera* AnimaCamerasManager::LoadCameraFromXml(const AnimaString& cameraXmlDefinition)
{
	AnimaCamera* camera = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(cameraXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString name = pt.get<AnimaString>("AnimaCamera.Name");
	
	// Controllo che il nome del modello non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_cameras.Contains(name) >= 0)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a camera. A camera named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());

	camera = CreateCamera(name);
	
	if (camera)
	{
		camera->ReadObject(pt, _scene, false);
	}
	
	return camera;
}

bool AnimaCamerasManager::LoadCameras(const AnimaString& camerasPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(camerasPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".acamera")
			{
				if(LoadCameraFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaCamerasManager::SaveCameraToFile(const AnimaString& cameraName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaCamera* camera = _cameras.GetWithName(cameraName);
	SaveCameraToFile(camera, destinationPath, createFinalPath);
}

void AnimaCamerasManager::SaveCameraToFile(AnimaCamera* camera, const AnimaString& destinationPath, bool createFinalPath)
{
	if (camera == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(camera->GetName() + ".acamera");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	camera->SaveObject(saveFileName);
}

void AnimaCamerasManager::SaveCameras(const AnimaString& destinationPath)
{
	AInt count = _cameras.GetSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveCameraToFile(_cameras[i], destinationPath, true);
	}
}

bool AnimaCamerasManager::FinalizeObjectsAfterRead()
{
	AInt count = _cameras.GetSize();
	for(AInt i = 0; i < count; i++)
		_cameras[i]->FinalizeAfterRead(_scene);
	return true;
}

void AnimaCamerasManager::UpdateCameras(AFloat elapsedTime)
{
	AInt count = _cameras.GetSize();
	for(AInt i = 0; i < count; i++)
		_cameras[i]->UpdatePosition(elapsedTime);
}

END_ANIMA_ENGINE_NAMESPACE