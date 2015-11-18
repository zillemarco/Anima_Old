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
		try
		{
			AnimaString texturesPath = pt.get<AnimaString>("AnimaScene.TexturesPath.Value", "");
			AnimaString texturesType = pt.get<AnimaString>("AnimaScene.TexturesPath.Type", "absolute");
			AnimaString materialsPath = pt.get<AnimaString>("AnimaScene.MaterialsPath.Value", "");
			AnimaString materialsType = pt.get<AnimaString>("AnimaScene.MaterialsPath.Type", "absolute");
			AnimaString nodesPath = pt.get<AnimaString>("AnimaScene.NodesPath.Value", "");
			AnimaString nodesType = pt.get<AnimaString>("AnimaScene.NodesPath.Type", "absolute");
			AnimaString geometriesPath = pt.get<AnimaString>("AnimaScene.GeometriesPath.Value", "");
			AnimaString geometriesType = pt.get<AnimaString>("AnimaScene.GeometriesPath.Type", "absolute");
			AnimaString nodesInstancesPath = pt.get<AnimaString>("AnimaScene.NodesInstancesPath.Value", "");
			AnimaString nodesInstancesType = pt.get<AnimaString>("AnimaScene.NodesInstancesPath.Type", "absolute");
			AnimaString geometriesInstancesPath = pt.get<AnimaString>("AnimaScene.GeometriesInstancesPath.Value", "");
			AnimaString geometriesInstancesType = pt.get<AnimaString>("AnimaScene.GeometriesInstancesPath.Type", "absolute");
			AnimaString camerasPath = pt.get<AnimaString>("AnimaScene.CamerasPath.Value", "");
			AnimaString camerasType = pt.get<AnimaString>("AnimaScene.CamerasPath.Type", "absolute");
			AnimaString lightsPath = pt.get<AnimaString>("AnimaScene.LightsPath.Value", "");
			AnimaString lightsType = pt.get<AnimaString>("AnimaScene.LightsPath.Type", "absolute");
			AnimaString animationsPath = pt.get<AnimaString>("AnimaScene.AnimationsPath.Value", "");
			AnimaString animationsType = pt.get<AnimaString>("AnimaScene.AnimationsPath.Type", "absolute");
			
			if(!texturesPath.empty())
			{
				if (iequals(texturesType, "relative"))
					texturesPath = (fs::path(sceneFoldersPath) / fs::path(texturesPath)).string();

				if(!scene->GetTexturesManager()->LoadTextures(texturesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading textures: couldn't load textures at path '%s'", texturesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: textures path not specified");
			}
			
			if(!materialsPath.empty())
			{
				if (iequals(materialsType, "relative"))
					materialsPath = (fs::path(sceneFoldersPath) / fs::path(materialsPath)).string();

				if(!scene->GetMaterialsManager()->LoadMaterials(materialsPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading materials: couldn't load materials at path '%s'", materialsPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: materials path not specified");
			}
			
			if(!geometriesPath.empty())
			{
				if (iequals(geometriesType, "relative"))
					geometriesPath = (fs::path(sceneFoldersPath) / fs::path(geometriesPath)).string();

				if(!scene->GetGeometriesManager()->LoadGeometries(geometriesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading geometries: couldn't load geometries at path '%s'", geometriesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: geometries path not specified");
			}
			
			if(!nodesPath.empty())
			{
				if (iequals(nodesType, "relative"))
					nodesPath = (fs::path(sceneFoldersPath) / fs::path(nodesPath)).string();

				if(!scene->GetNodesManager()->LoadNodes(nodesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading nodes: couldn't load nodes at path '%s'", nodesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: nodes path not specified");
			}
			
			if(!geometriesInstancesPath.empty())
			{
				if (iequals(geometriesInstancesType, "relative"))
					geometriesInstancesPath = (fs::path(sceneFoldersPath) / fs::path(geometriesInstancesPath)).string();

				if(!scene->GetGeometryInstancesManager()->LoadGeometriesInstances(geometriesInstancesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading geometries instances: couldn't load geometries instances at path '%s'", geometriesInstancesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: geometries instances path not specified");
			}
			
			if(!nodesInstancesPath.empty())
			{
				if (iequals(nodesInstancesType, "relative"))
					nodesInstancesPath = (fs::path(sceneFoldersPath) / fs::path(nodesInstancesPath)).string();

				if(!scene->GetNodeInstancesManager()->LoadNodesInstances(nodesInstancesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading nodes instances: couldn't load nodes instances at path '%s'", nodesInstancesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: nodes instances path not specified");
			}
			
			if(!camerasPath.empty())
			{
				if (iequals(camerasType, "relative"))
					camerasPath = (fs::path(sceneFoldersPath) / fs::path(camerasPath)).string();

				if(!scene->GetCamerasManager()->LoadCameras(camerasPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading cameras: couldn't load cameras at path '%s'", camerasPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: cameras path not specified");
			}
			
			if(!lightsPath.empty())
			{
				if (iequals(lightsType, "relative"))
					lightsPath = (fs::path(sceneFoldersPath) / fs::path(lightsPath)).string();

				if(!scene->GetLightsManager()->LoadLights(lightsPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading lights: couldn't load lights at path '%s'", lightsPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: lights path not specified");
			}
			
//			if(!animationsPath.empty())
//			{
//				if (iequals(animationsType, "relative"))
//					animationsPath = (fs::path(sceneFoldersPath) / fs::path(animationsPath)).string();
//
//				if(!scene->GetAnimationsManager()->LoadAnimations(animationsPath))
//					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading animations: couldn't load animations at path '%s'", animationsPath.c_str());
//			}
//			else
//			{
//				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: animations path not specified");
//			}

			scene->GetTexturesManager()->FinalizeObjectsAfterRead();
			scene->GetMaterialsManager()->FinalizeObjectsAfterRead();
			scene->GetGeometriesManager()->FinalizeObjectsAfterRead();
			scene->GetNodesManager()->FinalizeObjectsAfterRead();
			scene->GetGeometryInstancesManager()->FinalizeObjectsAfterRead();
			scene->GetNodeInstancesManager()->FinalizeObjectsAfterRead();
			scene->GetCamerasManager()->FinalizeObjectsAfterRead();
			scene->GetLightsManager()->FinalizeObjectsAfterRead();
			//	scene->GetAnimationsManager()->FinalizeObjectsAfterRead();
		}
		catch (boost::property_tree::ptree_bad_path& exception)
		{
			AnimaLogger::LogMessageFormat("ERROR - AnimaScenesManager loading scene: %s", exception.what());
		}
		catch (boost::property_tree::ptree_bad_data& exception)
		{
			AnimaLogger::LogMessageFormat("ERROR - AnimaScenesManager loading scene: %s", exception.what());
		}
		catch (boost::property_tree::ptree_error& exception)
		{
			AnimaLogger::LogMessageFormat("ERROR - AnimaScenesManager loading scene: %s", exception.what());
		}
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
	
	AnimaString saveDirectory = fs::path(saveFileName).parent_path().string();
	
	AnimaString texturesPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-textures")).string();
	AnimaString materialsPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-materials")).string();
	AnimaString nodesPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-nodes")).string();
	AnimaString geometriesPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-geometries")).string();
	AnimaString nodesInstancesPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-nodesInstances")).string();
	AnimaString geometriesInstancesPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-geometriesInstances")).string();
	AnimaString camerasPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-cameras")).string();
	AnimaString lightsPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-lights")).string();
	AnimaString animationsPath = (/*fs::path(saveDirectory) / */fs::path(scene->GetName() + "-animations")).string();
	
	using boost::property_tree::ptree;
	ptree sceneTree;
	
	sceneTree.add("AnimaScene.Name", scene->GetName());
	sceneTree.add("AnimaScene.TexturesPath", texturesPath);
	sceneTree.add("AnimaScene.MaterialsPath", materialsPath);
	sceneTree.add("AnimaScene.NodesPath", nodesPath);
	sceneTree.add("AnimaScene.GeometriesPath", geometriesPath);
	sceneTree.add("AnimaScene.NodesInstancesPath", nodesInstancesPath);
	sceneTree.add("AnimaScene.GeometriesInstancesPath", geometriesInstancesPath);
	sceneTree.add("AnimaScene.CamerasPath", camerasPath);
	sceneTree.add("AnimaScene.LightsPath", lightsPath);
	sceneTree.add("AnimaScene.AnimationsPath", animationsPath);
	
	boost::property_tree::write_xml(saveFileName, sceneTree, std::locale(), boost::property_tree::xml_writer_make_settings<ptree::key_type>('\t', 1));
	
	fs::create_directory(fs::path(saveDirectory) / fs::path(texturesPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(materialsPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(nodesPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(geometriesPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(nodesInstancesPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(geometriesInstancesPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(camerasPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(lightsPath));
	fs::create_directory(fs::path(saveDirectory) / fs::path(animationsPath));
	
	scene->GetTexturesManager()->SaveTextures(texturesPath);
	scene->GetMaterialsManager()->SaveMaterials(materialsPath);
	scene->GetGeometriesManager()->SaveGeometries(geometriesPath);
	scene->GetNodesManager()->SaveNodes(nodesPath);
	scene->GetGeometryInstancesManager()->SaveGeometriesInstances(geometriesInstancesPath);
	scene->GetNodeInstancesManager()->SaveNodesInstances(nodesInstancesPath);
	scene->GetCamerasManager()->SaveCameras(camerasPath);
	scene->GetLightsManager()->SaveLights(lightsPath);
//	scene->GetAnimationsManager()->SaveAnimations(animationsPath);
}

void AnimaScenesManager::SaveScenes(const AnimaString& destinationPath)
{
	AInt count = _scenes.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		SaveSceneToFile(_scenes[i], destinationPath, true);
	}
}

END_ANIMA_ENGINE_NAMESPACE