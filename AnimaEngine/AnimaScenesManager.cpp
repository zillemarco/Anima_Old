//
//  AnimaScenesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaScenesManager.h"
#include "AnimaLogger.h"

#include "AnimaGeometriesManager.h"
#include "AnimaNodesManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaGeometryInstancesManager.h"
#include "AnimaNodeInstancesManager.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScenesManager::AnimaScenesManager(AnimaEngine* engine)
{
	_engine = engine;
	_activeScene = nullptr;
}

AnimaScenesManager::~AnimaScenesManager()
{
	ClearScenes();
}

AnimaScene* AnimaScenesManager::CreateScene(const AnimaString& name)
{
	AInt index = _scenes.Contains(name);
	if(index >= 0)
		return nullptr;
	
	AnimaScene* scene = AnimaAllocatorNamespace::AllocateNew<AnimaScene>(*(_engine->GetScenesAllocator()), _engine, name);
	_scenes.Add(name, scene);
	return scene;
}

void AnimaScenesManager::ClearScenes()
{
	AInt count = _scenes.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaScene* scene = _scenes[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_engine->GetScenesAllocator()), scene);
		scene = nullptr;
	}
	
	_scenes.RemoveAll();
}

void AnimaScenesManager::NotifySceneActivation(AnimaScene* scene)
{
	if(_activeScene != nullptr)
		_activeScene->Deactivate();
	_activeScene = scene;
}

void AnimaScenesManager::NotifySceneDeactivation(AnimaScene* scene)
{
	_activeScene = nullptr;
}

AnimaScene* AnimaScenesManager::GetActiveScene()
{
	// Ciclo per trovare una scena attiva
	// _activeScene non dovrebbe mai essere null quindi il più delle volte il ciclo non viene nemmeno eseguito
	AnimaScene* firstScene = nullptr;
	if(_activeScene == nullptr)
	{
		AInt count = _scenes.GetSize();
		for (AInt i = 0; i < count && _activeScene == nullptr; i++)
		{
			AnimaScene* scene = _scenes[i];
			
			if(firstScene == nullptr)
				firstScene = scene;
			
			if(scene->IsActive())
				firstScene = scene;
		}
	}
	
	// Se ho trovato la scena attiva, torno il suo puntatore
	if (_activeScene != nullptr)
		return _activeScene;
	
	// Se anche dopo il ciclo non si è trovata una scena attiva ed esiste almeno una scena
	// attivo quella scena e torno _activeScene che verrà aggiornato da NotifySceneActivation
	if(_activeScene == nullptr && firstScene != nullptr)
	{
		firstScene->Activate();
		return _activeScene;
	}
	
	// Se non esistono nemmeno camere, ovviamente, torno null
	return nullptr;
}

AnimaScene* AnimaScenesManager::GetScene(AUint index)
{
	return _scenes[index];
}

AnimaScene* AnimaScenesManager::GetSceneFromName(const AnimaString& name)
{
	return _scenes[name];
}

bool AnimaScenesManager::LoadScenes(const AnimaString& scenesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(scenesPath);
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".ascene")
			{
				if(LoadSceneFromFile(directoryIterator->path().string()) == nullptr)
					return false;
			}
		}
	}
	
	return true;
}

AnimaScene* AnimaScenesManager::LoadSceneFromFile(const AnimaString& sceneFilePath)
{
	std::ifstream fileStream(sceneFilePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadSceneFromXml(xml, boost::filesystem::path(sceneFilePath).parent_path().string());
}

AnimaScene* AnimaScenesManager::LoadSceneFromXml(const AnimaString& sceneXmlDefinition, const AnimaString& sceneFoldersPath)
{
	AnimaScene* scene = nullptr;
	
	namespace fs = boost::filesystem;
	using boost::algorithm::iequals;
	using boost::property_tree::ptree;
	ptree pt;
	
	std::stringstream ss(sceneXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	AnimaString name = pt.get<AnimaString>("AnimaScene.Name", "");
	if(name.empty())
		return nullptr;
	
	scene = CreateScene(name);
	
	if (scene)
	{
		scene->SetReadingDirectory(sceneFoldersPath);
		scene->ReadObject(pt, scene, false);
	}
	
	return scene;
}

void AnimaScenesManager::SaveSceneToFile(const AnimaString& sceneName, const AnimaString& destinationPath, bool createFinalPath)
{
	SaveSceneToFile(GetSceneFromName(sceneName), destinationPath, createFinalPath);
}

void AnimaScenesManager::SaveSceneToFile(AnimaScene* scene, const AnimaString& destinationPath, bool createFinalPath)
{
	if(scene == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	
	if(createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(scene->GetName() + ".ascene");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	scene->SaveObject(saveFileName);
}

void AnimaScenesManager::SaveScenes(const AnimaString& destinationPath)
{
	AInt count = _scenes.GetSize();
	for (AInt i = 0; i < count; i++)
		SaveSceneToFile(_scenes[i], destinationPath, true);
}

END_ANIMA_ENGINE_NAMESPACE