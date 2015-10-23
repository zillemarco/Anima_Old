//
//  AnimaLightsManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaLightsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaLightsManager::AnimaLightsManager(AnimaScene* scene)
	: _lights(scene->GetEngine()->GetLightsAllocator())
{
	_scene = scene;
}

AnimaLightsManager::~AnimaLightsManager()
{
	ClearLights();
}

AnimaDirectionalLight* AnimaLightsManager::CreateDirectionalLight(const AnimaString& name)
{
	return CreateLight<AnimaDirectionalLight>(name);
}

AnimaPointLight* AnimaLightsManager::CreatePointLight(const AnimaString& name)
{
	return CreateLight<AnimaPointLight>(name);
}

AnimaSpotLight* AnimaLightsManager::CreateSpotLight(const AnimaString& name)
{
	return CreateLight<AnimaSpotLight>(name);
}

AnimaHemisphereLight* AnimaLightsManager::CreateHemisphereLight(const AnimaString& name)
{
	return CreateLight<AnimaHemisphereLight>(name);
}

void AnimaLightsManager::ClearLights()
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaStringHasher>* lightsMap = _lights.GetArraysMap();
	for (auto lightsPair : (*lightsMap))
	{
		AnimaMappedArray<AnimaLight*>* lightsArray = lightsPair.second;
		AInt count = lightsArray->GetSize();
		for (AInt i = 0; i < count; i++)
		{
			AnimaLight* light = (*lightsArray)[i];
			AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetLightsAllocator()), light);
			light = nullptr;
		}
	}

	_lights.RemoveAll();
}

AnimaLight* AnimaLightsManager::GetLightFromName(const AnimaString& name)
{
	return _lights.GetWithName(name);
}

AInt AnimaLightsManager::GetTotalLightsCount()
{
	AInt count = 0;
	
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaStringHasher>* lightsMap = _lights.GetArraysMap();
	for (auto lightsPair : (*lightsMap))
		count += lightsPair.second->GetSize();

	return count;
}

AnimaTypeMappedArray<AnimaLight*>* AnimaLightsManager::GetLights()
{
	return &_lights;
}

void AnimaLightsManager::UpdateLightsMatrix(AnimaCamera* activeCamera)
{
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaStringHasher>* lightsMap = _lights.GetArraysMap();
	for (auto lightsPair : (*lightsMap))
	{
		AnimaMappedArray<AnimaLight*>* lightsArray = lightsPair.second;
		AInt count = lightsArray->GetSize();
		for (AInt i = 0; i < count; i++)
			lightsArray->Get(i)->ComputeLightMatrix(activeCamera);
	}
}

AnimaLight* AnimaLightsManager::LoadLightFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadLightFromXml(xml);
}

AnimaLight* AnimaLightsManager::LoadLightFromXml(const AnimaString& lightXmlDefinition)
{
	AnimaLight* light = nullptr;
	
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(lightXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString lightTypeName = "";
	
	if(pt.get_optional<AnimaString>(ANIMA_CLASS_NAME(AnimaDirectionalLight) + ".Name"))
		lightTypeName = ANIMA_CLASS_NAME(AnimaDirectionalLight);
	else if(pt.get_optional<AnimaString>(ANIMA_CLASS_NAME(AnimaPointLight) + ".Name"))
		lightTypeName = ANIMA_CLASS_NAME(AnimaPointLight);
	else if(pt.get_optional<AnimaString>(ANIMA_CLASS_NAME(AnimaSpotLight) + ".Name"))
		lightTypeName = ANIMA_CLASS_NAME(AnimaSpotLight);
	else if(pt.get_optional<AnimaString>(ANIMA_CLASS_NAME(AnimaHemisphereLight) + ".Name"))
		lightTypeName = ANIMA_CLASS_NAME(AnimaHemisphereLight);
	
	if(lightTypeName.empty())
	{
		AnimaLogger::LogMessageFormat("ERROR - Error reading a light. Engine was unable to recognize the light type.");
		return nullptr;
	}
	
	AnimaString name = pt.get<AnimaString>(lightTypeName + ".Name");
	
	// Controllo che il nome del modello non esista già e se esiste gli aggiungo un indice
	AnimaString originalName = name;
	AInt index = 0;
	while (_lights.Contains(name) != nullptr)
		name = FormatString("%s_%d", originalName.c_str(), index);
	
	if(name != originalName)
		AnimaLogger::LogMessageFormat("WARNING - Error reading a light. A light named '%s' already existed so it's been renamed to '%s'", originalName.c_str(), name.c_str());
	
	if(lightTypeName == ANIMA_CLASS_NAME(AnimaDirectionalLight))
		light = CreateDirectionalLight(name);
	else if(lightTypeName == ANIMA_CLASS_NAME(AnimaPointLight))
		light = CreatePointLight(name);
	else if(lightTypeName == ANIMA_CLASS_NAME(AnimaSpotLight))
		light = CreateSpotLight(name);
	else if(lightTypeName == ANIMA_CLASS_NAME(AnimaHemisphereLight))
		light = CreateHemisphereLight(name);
	
	if (light)
	{
		light->ReadObject(pt, _scene, false);
	}
	
	return light;
}

bool AnimaLightsManager::LoadLights(const AnimaString& lightsPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(lightsPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".alight")
			{
				if(LoadLightFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	return true;
}

void AnimaLightsManager::SaveLightToFile(const AnimaString& lightName, const AnimaString& destinationPath, bool createFinalPath)
{
	AnimaLight* light = _lights.GetWithName(lightName);
	SaveLightToFile(light, destinationPath, createFinalPath);
}

void AnimaLightsManager::SaveLightToFile(AnimaLight* light, const AnimaString& destinationPath, bool createFinalPath)
{
	if (light == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if (createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(light->GetName() + ".alight");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	light->SaveObject(saveFileName);
}

void AnimaLightsManager::SaveLights(const AnimaString& destinationPath)
{
	AInt count = _lights.GetTotalSize();
	for(AInt i = 0; i < count; i++)
	{
		SaveLightToFile(_lights.GetWithIndex(i), destinationPath, true);
	}
}

bool AnimaLightsManager::FinalizeObjectsAfterRead()
{
	AInt count = _lights.GetTotalSize();
	for(AInt i = 0; i < count; i++)
		_lights.GetWithIndex(i)->FinalizeAfterRead(_scene);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE