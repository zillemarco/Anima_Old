//
//  AnimaScenesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaScenesManager.h"
#include "AnimaLogger.h"

#include "AnimaMeshesManager.h"
#include "AnimaModelsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaTexturesManager.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScenesManager::AnimaScenesManager(AnimaEngine* engine)
{
	_engine = engine;
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
	
	return LoadSceneFromXml(xml);
}

AnimaScene* AnimaScenesManager::LoadSceneFromXml(const AnimaString& sceneXmlDefinition)
{
	AnimaScene* scene = nullptr;
	
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
			AnimaString texturesPath = pt.get<AnimaString>("AnimaScene.TexturesPath", "");
			AnimaString materialsPath = pt.get<AnimaString>("AnimaScene.MaterialsPath", "");
			AnimaString modelsPath = pt.get<AnimaString>("AnimaScene.ModelsPath", "");
			AnimaString meshesPath = pt.get<AnimaString>("AnimaScene.MeshesPath", "");
			AnimaString camerasPath = pt.get<AnimaString>("AnimaScene.CamerasPath", "");
			AnimaString lightsPath = pt.get<AnimaString>("AnimaScene.LightsPath", "");
			AnimaString animationsPath = pt.get<AnimaString>("AnimaScene.AnimationsPath", "");
			
			if(!texturesPath.empty())
			{
				if(!scene->GetTexturesManager()->LoadTextures(texturesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading textures: couldn't load textures at path '%s'", texturesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: textures path not specified");
			}
			
			if(!materialsPath.empty())
			{
				if(!scene->GetMaterialsManager()->LoadMaterials(materialsPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading materials: couldn't load materials at path '%s'", materialsPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: materials path not specified");
			}
			
			if(!meshesPath.empty())
			{
				if(!scene->GetMeshesManager()->LoadMeshes(meshesPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading meshes: couldn't load meshes at path '%s'", meshesPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: meshes path not specified");
			}
			
			if(!modelsPath.empty())
			{
				if(!scene->GetModelsManager()->LoadModels(modelsPath))
					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading models: couldn't load models at path '%s'", modelsPath.c_str());
			}
			else
			{
				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: models path not specified");
			}
			
//			if(!camerasPath.empty())
//			{
//				if(!scene->GetCamerasManager()->LoadCameras(camerasPath))
//					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading cameras: couldn't load cameras at path '%s'", camerasPath.c_str());
//			}
//			else
//			{
//				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: cameras path not specified");
//			}
//			
//			if(!lightsPath.empty())
//			{
//				if(!scene->GetLightsManager()->LoadLights(lightsPath))
//					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading lights: couldn't load lights at path '%s'", lightsPath.c_str());
//			}
//			else
//			{
//				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: lights path not specified");
//			}
//			
//			if(!animationsPath.empty())
//			{
//				if(!scene->GetAnimationsManager()->LoadAnimations(animationsPath))
//					AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading animations: couldn't load animations at path '%s'", animationsPath.c_str());
//			}
//			else
//			{
//				AnimaLogger::LogMessage("WARNING - AnimaScenesManager loading scene: animations path not specified");
//			}
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
	
	AnimaString texturesPath = (fs::path(saveDirectory) / fs::path("textures")).string();
	AnimaString materialsPath = (fs::path(saveDirectory) / fs::path("materials")).string();
	AnimaString modelsPath = (fs::path(saveDirectory) / fs::path("models")).string();
	AnimaString meshesPath = (fs::path(saveDirectory) / fs::path("meshes")).string();
	AnimaString camerasPath = (fs::path(saveDirectory) / fs::path("cameras")).string();
	AnimaString lightsPath = (fs::path(saveDirectory) / fs::path("lights")).string();
	AnimaString animationsPath = (fs::path(saveDirectory) / fs::path("animations")).string();
	
	using boost::property_tree::ptree;
	ptree sceneTree;
	
	sceneTree.add("AnimaScene.Name", scene->GetName());
	sceneTree.add("AnimaScene.TexturesPath", texturesPath);
	sceneTree.add("AnimaScene.MaterialsPath", materialsPath);
	sceneTree.add("AnimaScene.ModelsPath", modelsPath);
	sceneTree.add("AnimaScene.MeshesPath", meshesPath);
	sceneTree.add("AnimaScene.CamerasPath", camerasPath);
	sceneTree.add("AnimaScene.LightsPath", lightsPath);
	sceneTree.add("AnimaScene.AnimationsPath", animationsPath);
	
	boost::property_tree::write_xml(saveFileName, sceneTree, std::locale(), boost::property_tree::xml_writer_make_settings<ptree::key_type>('\t', 1));
	
	fs::create_directory(fs::path(texturesPath));
	fs::create_directory(fs::path(materialsPath));
	fs::create_directory(fs::path(modelsPath));
	fs::create_directory(fs::path(meshesPath));
	fs::create_directory(fs::path(camerasPath));
	fs::create_directory(fs::path(lightsPath));
	fs::create_directory(fs::path(animationsPath));
	
	scene->GetTexturesManager()->SaveTextures(texturesPath);
	scene->GetMaterialsManager()->SaveMaterials(materialsPath);
	scene->GetMeshesManager()->SaveMeshes(meshesPath);
	scene->GetModelsManager()->SaveModels(modelsPath);
//	scene->GetCamerasManager()->SaveCameras(camerasPath);
//	scene->GetLightsManager()->SaveLights(lightsPath);
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