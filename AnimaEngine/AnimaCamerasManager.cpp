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
: _cameras(scene->GetEngine()->GetCamerasAllocator())
{
	_scene = scene;
	_activeCamera = nullptr;
}

AnimaCamerasManager::~AnimaCamerasManager()
{
	ClearCameras();
}

AnimaFirstPersonCamera* AnimaCamerasManager::CreateFirstPersonCamera(const AnimaString& name)
{
	AnimaCamera* camera = _cameras.Contains(name);
	if (camera != nullptr)
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	AnimaFirstPersonCamera* newCamera = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*(_scene->GetCamerasAllocator()), _scene->GetCamerasAllocator(), this, _scene->GetDataGeneratorsManager(), name);
	_cameras.Add<AnimaFirstPersonCamera*>(name, newCamera);
	
	return newCamera;
}

AnimaThirdPersonCamera* AnimaCamerasManager::CreateThirdPersonCamera(const AnimaString& name)
{
	AnimaCamera* camera = _cameras.Contains(name);
	if (camera != nullptr)
		return nullptr;
	
	ANIMA_ASSERT(_scene != nullptr);
	AnimaThirdPersonCamera* newCamera = AnimaAllocatorNamespace::AllocateNew<AnimaThirdPersonCamera>(*(_scene->GetCamerasAllocator()), _scene->GetCamerasAllocator(), this, _scene->GetDataGeneratorsManager(), name);
	_cameras.Add<AnimaThirdPersonCamera*>(name, newCamera);
	
	return newCamera;
}

void AnimaCamerasManager::ClearCameras()
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaStringHasher>* camerasMap = _cameras.GetArraysMap();
	for (auto camerasPair : (*camerasMap))
	{
		AnimaMappedArray<AnimaCamera*>* camerasArray = camerasPair.second;
		AInt count = camerasArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaCamera* camera = (*camerasArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetCamerasAllocator()), camera);
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
		boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaStringHasher>* camerasMap = _cameras.GetArraysMap();
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
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaStringHasher>* camerasMap = _cameras.GetArraysMap();
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
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaStringHasher>* camerasMap = _cameras.GetArraysMap();
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

AInt AnimaCamerasManager::GetTotalCamerasCount()
{
	AInt count = 0;
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaCamera*>*, AnimaStringHasher>* camerasMap = _cameras.GetArraysMap();
	for (auto camerasPair : (*camerasMap))
		count += camerasPair.second->GetSize();
	
	return count;
}

AnimaTypeMappedArray<AnimaCamera*>* AnimaCamerasManager::GetCameras()
{
	return &_cameras;
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
	
	AnimaString cameraTypeName = "";
	
	if(pt.get_optional<AnimaString>(ANIMA_CLASS_NAME(AnimaFirstPersonCamera) + ".Name"))
		cameraTypeName = ANIMA_CLASS_NAME(AnimaFirstPersonCamera);
	else if(pt.get_optional<AnimaString>(ANIMA_CLASS_NAME(AnimaThirdPersonCamera) + ".Name"))
		cameraTypeName = ANIMA_CLASS_NAME(AnimaThirdPersonCamera);
	
	if(cameraTypeName.empty())
	{
		AnimaLogger::LogMessageFormat("ERROR - Error reading a camera. Engine was unable to recognize the camera type.");
		return nullptr;
	}
	
	AnimaString name = pt.get<AnimaString>(cameraTypeName + ".Name");
	
	// Controllo che il nome del modello non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_cameras.Contains(name) != nullptr)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a camera. A camera named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());

	if(cameraTypeName == ANIMA_CLASS_NAME(AnimaFirstPersonCamera))
		camera = CreateFirstPersonCamera(name);
	else if(cameraTypeName == ANIMA_CLASS_NAME(AnimaFirstPersonCamera))
		camera = CreateThirdPersonCamera(name);
	
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
	AInt count = _cameras.GetTotalSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveCameraToFile(_cameras.GetWithIndex(i), destinationPath, true);
	}
}

bool AnimaCamerasManager::FinalizeObjectsAfterRead()
{
	AInt count = _cameras.GetTotalSize();
	for(AInt i = 0; i < count; i++)
		_cameras.GetWithIndex(i)->FinalizeAfterRead(_scene);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE