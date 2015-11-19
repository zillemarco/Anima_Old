//
//  AnimaScene.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaScene.h"
#include "AnimaNodesManager.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaLightsManager.h"
#include "AnimaNodeInstancesManager.h"
#include "AnimaGeometryInstancesManager.h"
#include "AnimaAnimationsManager.h"
#include "AnimaGeometryInstance.h"
#include "AnimaScenesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScene::AnimaScene(AnimaEngine* engine, const AnimaString& name)
	: AnimaMappedValues(nullptr, nullptr, name)
{
	// Ovviamente il motore passato non può essere null
	ANIMA_ASSERT(engine != nullptr);
	
	IMPLEMENT_ANIMA_CLASS(AnimaScene);
	
	_engine = engine;
	_allocator = _engine->GetScenesAllocator();
	
	_nodesManager = nullptr;
	_geometriesManager = nullptr;
	_nodeInstancesManager = nullptr;
	_geometryInstancesManager = nullptr;
	
	_camerasManager = nullptr;
	_texturesManager = nullptr;
//	_dataGeneratorManager = nullptr;
	_materialsManager = nullptr;
	_lightsManager = nullptr;
	_animationsManager = nullptr;
	
	_physBroadphaseInterface = nullptr;
	_physCollisionConfiguration = nullptr;
	_physCollisionDispatcher = nullptr;
	_physConstraintSolver = nullptr;
	_physWorld = nullptr;
	
	_mouseInteractor = nullptr;
	_keyboardInteractor = nullptr;
	_joystickInteractor = nullptr;
	
	_worldGravity = AnimaVertex3f(0.0f, -9.81f, 0.0f);
	_totalSceneTime = 0.0;
	_isRunning = false;
	_shaderSource = ASDSO_SCENE;

	InitializeManagers();
}

AnimaScene::AnimaScene(const AnimaScene& src)
	: AnimaMappedValues(src)
{
	_engine = src._engine;

	_nodesManager = src._nodesManager;
	_geometriesManager = src._geometriesManager;
	_nodeInstancesManager = src._nodeInstancesManager;
	_geometryInstancesManager = src._geometryInstancesManager;

	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
//	_dataGeneratorManager = src._dataGeneratorManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
	_animationsManager = src._animationsManager;
	
	_physBroadphaseInterface = src._physBroadphaseInterface;
	_physCollisionConfiguration = src._physCollisionConfiguration;
	_physCollisionDispatcher = src._physCollisionDispatcher;
	_physConstraintSolver = src._physConstraintSolver;
	_physWorld = src._physWorld;
	
	_worldGravity = src._worldGravity;
	_totalSceneTime = src._totalSceneTime;
	_isRunning = src._isRunning;
}

AnimaScene::AnimaScene(AnimaScene&& src)
	: AnimaMappedValues(src)
{
	_engine = src._engine;

	_nodesManager = src._nodesManager;
	_geometriesManager = src._geometriesManager;
	_nodeInstancesManager = src._nodeInstancesManager;
	_geometryInstancesManager = src._geometryInstancesManager;

	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
//	_dataGeneratorManager = src._dataGeneratorManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
	_animationsManager = src._animationsManager;
	
	_physBroadphaseInterface = src._physBroadphaseInterface;
	_physCollisionConfiguration = src._physCollisionConfiguration;
	_physCollisionDispatcher = src._physCollisionDispatcher;
	_physConstraintSolver = src._physConstraintSolver;
	_physWorld = src._physWorld;
	
	_worldGravity = src._worldGravity;
	_totalSceneTime = src._totalSceneTime;
	_isRunning = src._isRunning;
}

AnimaScene::~AnimaScene()
{
	TerminateManagers();
	TerminatePhysics();
}

AnimaScene& AnimaScene::operator=(const AnimaScene& src)
{
	if (this != &src)
	{
		TerminatePhysics();
		TerminateManagers();
		
		AnimaMappedValues::operator=(src);

		_engine = src._engine;

		_nodesManager = src._nodesManager;
		_geometriesManager = src._geometriesManager;
		_nodeInstancesManager = src._nodeInstancesManager;
		_geometryInstancesManager = src._geometryInstancesManager;

		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
//		_dataGeneratorManager = src._dataGeneratorManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
		_animationsManager = src._animationsManager;
		
		_physBroadphaseInterface = src._physBroadphaseInterface;
		_physCollisionConfiguration = src._physCollisionConfiguration;
		_physCollisionDispatcher = src._physCollisionDispatcher;
		_physConstraintSolver = src._physConstraintSolver;
		_physWorld = src._physWorld;
		
		_worldGravity = src._worldGravity;
		_totalSceneTime = src._totalSceneTime;
		_isRunning = src._isRunning;
	}
	
	return *this;
}

AnimaScene& AnimaScene::operator=(AnimaScene&& src)
{
	if (this != &src)
	{
		TerminatePhysics();
		TerminateManagers();
		
		AnimaMappedValues::operator=(src);

		_engine = src._engine;

		_nodesManager = src._nodesManager;
		_geometriesManager = src._geometriesManager;
		_nodeInstancesManager = src._nodeInstancesManager;
		_geometryInstancesManager = src._geometryInstancesManager;

		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
//		_dataGeneratorManager = src._dataGeneratorManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
		_animationsManager = src._animationsManager;
		
		_physBroadphaseInterface = src._physBroadphaseInterface;
		_physCollisionConfiguration = src._physCollisionConfiguration;
		_physCollisionDispatcher = src._physCollisionDispatcher;
		_physConstraintSolver = src._physConstraintSolver;
		_physWorld = src._physWorld;
		
		_worldGravity = src._worldGravity;
		_totalSceneTime = src._totalSceneTime;
		_isRunning = src._isRunning;
	}
	
	return *this;
}

ptree AnimaScene::GetObjectTree(bool saveName) const
{
	ptree tree;
	
	namespace fs = boost::filesystem;
	
	AnimaString texturesPath = (fs::path(GetName() + "-textures")).string();
	AnimaString materialsPath = (fs::path(GetName() + "-materials")).string();
	AnimaString nodesPath = (fs::path(GetName() + "-nodes")).string();
	AnimaString geometriesPath = (fs::path(GetName() + "-geometries")).string();
	AnimaString nodesInstancesPath = (fs::path(GetName() + "-nodesInstances")).string();
	AnimaString geometriesInstancesPath = (fs::path(GetName() + "-geometriesInstances")).string();
	AnimaString camerasPath = (fs::path(GetName() + "-cameras")).string();
	AnimaString lightsPath = (fs::path(GetName() + "-lights")).string();
	AnimaString animationsPath = (fs::path(GetName() + "-animations")).string();
	
	if(saveName)
		tree.add("AnimaScene.Name", GetName());
	
	tree.add("AnimaScene.TexturesPath.Value", texturesPath);
	tree.add("AnimaScene.TexturesPath.Type", "relative");
	
	tree.add("AnimaScene.MaterialsPath.Value", materialsPath);
	tree.add("AnimaScene.MaterialsPath.Type", "relative");
	
	tree.add("AnimaScene.NodesPath.Value", nodesPath);
	tree.add("AnimaScene.NodesPath.Type", "relative");
	
	tree.add("AnimaScene.GeometriesPath.Value", geometriesPath);
	tree.add("AnimaScene.GeometriesPath.Type", "relative");
	
	tree.add("AnimaScene.NodesInstancesPath.Value", nodesInstancesPath);
	tree.add("AnimaScene.NodesInstancesPath.Type", "relative");
	
	tree.add("AnimaScene.GeometriesInstancesPath.Value", geometriesInstancesPath);
	tree.add("AnimaScene.GeometriesInstancesPath.Type", "relative");
	
	tree.add("AnimaScene.CamerasPath.Value", camerasPath);
	tree.add("AnimaScene.CamerasPath.Type", "relative");
	
	tree.add("AnimaScene.LightsPath.Value", lightsPath);
	tree.add("AnimaScene.LightsPath.Type", "relative");

	tree.add("AnimaScene.AnimationsPath.Value", animationsPath);
	tree.add("AnimaScene.AnimationsPath.Type", "relative");
	
	fs::create_directory(fs::path(_savingDirectory) / fs::path(texturesPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(materialsPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(nodesPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(geometriesPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(nodesInstancesPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(geometriesInstancesPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(camerasPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(lightsPath));
	fs::create_directory(fs::path(_savingDirectory) / fs::path(animationsPath));
	
	_texturesManager->SaveTextures((fs::path(_savingDirectory) / fs::path(texturesPath)).string());
	_materialsManager->SaveMaterials((fs::path(_savingDirectory) / fs::path(materialsPath)).string());
	_geometriesManager->SaveGeometries((fs::path(_savingDirectory) / fs::path(geometriesPath)).string());
	_nodesManager->SaveNodes((fs::path(_savingDirectory) / fs::path(nodesPath)).string());
	_geometryInstancesManager->SaveGeometriesInstances((fs::path(_savingDirectory) / fs::path(geometriesInstancesPath)).string());
	_nodeInstancesManager->SaveNodesInstances((fs::path(_savingDirectory) / fs::path(nodesInstancesPath)).string());
	_camerasManager->SaveCameras((fs::path(_savingDirectory) / fs::path(camerasPath)).string());
	_lightsManager->SaveLights((fs::path(_savingDirectory) / fs::path(lightsPath)).string());
//	_animationsManager->SaveAnimations((fs::path(_savingDirectory) / fs::path(animationsPath)).string());
	
	tree.add_child("AnimaScene.MappedValues", AnimaMappedValues::GetObjectTree(false));
	
	return tree;
}

bool AnimaScene::ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName)
{
	try
	{
		namespace fs = boost::filesystem;
		using boost::algorithm::iequals;
		
		if(readName)
			SetName(objectTree.get<AnimaString>("AnimaScene.Name"));
		
		AnimaString texturesPath = objectTree.get<AnimaString>("AnimaScene.TexturesPath.Value", "");
		AnimaString texturesType = objectTree.get<AnimaString>("AnimaScene.TexturesPath.Type", "absolute");
		AnimaString materialsPath = objectTree.get<AnimaString>("AnimaScene.MaterialsPath.Value", "");
		AnimaString materialsType = objectTree.get<AnimaString>("AnimaScene.MaterialsPath.Type", "absolute");
		AnimaString nodesPath = objectTree.get<AnimaString>("AnimaScene.NodesPath.Value", "");
		AnimaString nodesType = objectTree.get<AnimaString>("AnimaScene.NodesPath.Type", "absolute");
		AnimaString geometriesPath = objectTree.get<AnimaString>("AnimaScene.GeometriesPath.Value", "");
		AnimaString geometriesType = objectTree.get<AnimaString>("AnimaScene.GeometriesPath.Type", "absolute");
		AnimaString nodesInstancesPath = objectTree.get<AnimaString>("AnimaScene.NodesInstancesPath.Value", "");
		AnimaString nodesInstancesType = objectTree.get<AnimaString>("AnimaScene.NodesInstancesPath.Type", "absolute");
		AnimaString geometriesInstancesPath = objectTree.get<AnimaString>("AnimaScene.GeometriesInstancesPath.Value", "");
		AnimaString geometriesInstancesType = objectTree.get<AnimaString>("AnimaScene.GeometriesInstancesPath.Type", "absolute");
		AnimaString camerasPath = objectTree.get<AnimaString>("AnimaScene.CamerasPath.Value", "");
		AnimaString camerasType = objectTree.get<AnimaString>("AnimaScene.CamerasPath.Type", "absolute");
		AnimaString lightsPath = objectTree.get<AnimaString>("AnimaScene.LightsPath.Value", "");
		AnimaString lightsType = objectTree.get<AnimaString>("AnimaScene.LightsPath.Type", "absolute");
		AnimaString animationsPath = objectTree.get<AnimaString>("AnimaScene.AnimationsPath.Value", "");
		AnimaString animationsType = objectTree.get<AnimaString>("AnimaScene.AnimationsPath.Type", "absolute");
		
		if(!texturesPath.empty())
		{
			if (iequals(texturesType, "relative"))
				texturesPath = (fs::path(_readingDirectory) / fs::path(texturesPath)).string();
			
			if(!scene->GetTexturesManager()->LoadTextures(texturesPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading textures: couldn't load textures at path '%s'", texturesPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: textures path not specified");
		}
		
		if(!materialsPath.empty())
		{
			if (iequals(materialsType, "relative"))
				materialsPath = (fs::path(_readingDirectory) / fs::path(materialsPath)).string();
			
			if(!scene->GetMaterialsManager()->LoadMaterials(materialsPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading materials: couldn't load materials at path '%s'", materialsPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: materials path not specified");
		}
		
		if(!geometriesPath.empty())
		{
			if (iequals(geometriesType, "relative"))
				geometriesPath = (fs::path(_readingDirectory) / fs::path(geometriesPath)).string();
			
			if(!scene->GetGeometriesManager()->LoadGeometries(geometriesPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading geometries: couldn't load geometries at path '%s'", geometriesPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: geometries path not specified");
		}
		
		if(!nodesPath.empty())
		{
			if (iequals(nodesType, "relative"))
				nodesPath = (fs::path(_readingDirectory) / fs::path(nodesPath)).string();
			
			if(!scene->GetNodesManager()->LoadNodes(nodesPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading nodes: couldn't load nodes at path '%s'", nodesPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: nodes path not specified");
		}
		
		if(!geometriesInstancesPath.empty())
		{
			if (iequals(geometriesInstancesType, "relative"))
				geometriesInstancesPath = (fs::path(_readingDirectory) / fs::path(geometriesInstancesPath)).string();
			
			if(!scene->GetGeometryInstancesManager()->LoadGeometriesInstances(geometriesInstancesPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading geometries instances: couldn't load geometries instances at path '%s'", geometriesInstancesPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: geometries instances path not specified");
		}
		
		if(!nodesInstancesPath.empty())
		{
			if (iequals(nodesInstancesType, "relative"))
				nodesInstancesPath = (fs::path(_readingDirectory) / fs::path(nodesInstancesPath)).string();
			
			if(!scene->GetNodeInstancesManager()->LoadNodesInstances(nodesInstancesPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading nodes instances: couldn't load nodes instances at path '%s'", nodesInstancesPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: nodes instances path not specified");
		}
		
		if(!camerasPath.empty())
		{
			if (iequals(camerasType, "relative"))
				camerasPath = (fs::path(_readingDirectory) / fs::path(camerasPath)).string();
			
			if(!scene->GetCamerasManager()->LoadCameras(camerasPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading cameras: couldn't load cameras at path '%s'", camerasPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: cameras path not specified");
		}
		
		if(!lightsPath.empty())
		{
			if (iequals(lightsType, "relative"))
				lightsPath = (fs::path(_readingDirectory) / fs::path(lightsPath)).string();
			
			if(!scene->GetLightsManager()->LoadLights(lightsPath))
				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading lights: couldn't load lights at path '%s'", lightsPath.c_str());
		}
		else
		{
			AnimaLogger::LogMessage("WARNING - AnimaScene loading: lights path not specified");
		}
		
//		if(!animationsPath.empty())
//		{
//			if (iequals(animationsType, "relative"))
//				animationsPath = (fs::path(_readingDirectory) / fs::path(animationsPath)).string();
//
//			if(!scene->GetAnimationsManager()->LoadAnimations(animationsPath))
//				AnimaLogger::LogMessageFormat("WARNING - AnimaScenesManager loading animations: couldn't load animations at path '%s'", animationsPath.c_str());
//		}
//		else
//		{
//			AnimaLogger::LogMessage("WARNING - AnimaScene loading: animations path not specified");
//		}
		
		_texturesManager->FinalizeObjectsAfterRead();
		_materialsManager->FinalizeObjectsAfterRead();
		_geometriesManager->FinalizeObjectsAfterRead();
		_nodesManager->FinalizeObjectsAfterRead();
		_geometryInstancesManager->FinalizeObjectsAfterRead();
		_nodeInstancesManager->FinalizeObjectsAfterRead();
		_camerasManager->FinalizeObjectsAfterRead();
		_lightsManager->FinalizeObjectsAfterRead();
//		_animationsManager->FinalizeObjectsAfterRead();
		
		// Dopo aver letto tutti i dati dei componenti della scena posso leggere i valori della scena
		ptree mappedValuesTree = objectTree.get_child("AnimaScene.MappedValues");
		if(AnimaMappedValues::ReadObject(mappedValuesTree, scene, false) == false)
			return false;
		
		// Dopo aver letto i valori della scena devo finalizzarli per 'agganciare' ciò che ancora mancava,
		// ad esempio le texture
		return FinalizeAfterRead(this);
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing scene: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing scene: %s", exception.what());
		return false;
	}
}

void AnimaScene::InitializeManagers()
{
	TerminateManagers();
	
	_camerasManager = AnimaAllocatorNamespace::AllocateNew<AnimaCamerasManager>(*_engine->GetManagersAllocator(), this);
	_texturesManager = AnimaAllocatorNamespace::AllocateNew<AnimaTexturesManager>(*_engine->GetManagersAllocator(), this);
	_dataGeneratorManager = AnimaAllocatorNamespace::AllocateNew<AnimaDataGeneratorsManager>(*_engine->GetManagersAllocator(), this);
	_lightsManager = AnimaAllocatorNamespace::AllocateNew<AnimaLightsManager>(*_engine->GetManagersAllocator(), this);

	_materialsManager = AnimaAllocatorNamespace::AllocateNew<AnimaMaterialsManager>(*_engine->GetManagersAllocator(), this, _texturesManager);
	_animationsManager = AnimaAllocatorNamespace::AllocateNew<AnimaAnimationsManager>(*_engine->GetManagersAllocator(), this);
	
	_geometriesManager = AnimaAllocatorNamespace::AllocateNew<AnimaGeometriesManager>(*_engine->GetManagersAllocator(), this, _materialsManager);
	_nodesManager = AnimaAllocatorNamespace::AllocateNew<AnimaNodesManager>(*_engine->GetManagersAllocator(), this, _geometriesManager, _materialsManager, _animationsManager);
	_geometryInstancesManager = AnimaAllocatorNamespace::AllocateNew<AnimaGeometryInstancesManager>(*_engine->GetManagersAllocator(), this, _geometriesManager);
	_nodeInstancesManager = AnimaAllocatorNamespace::AllocateNew<AnimaNodeInstancesManager>(*_engine->GetManagersAllocator(), this, _nodesManager, _geometryInstancesManager);
}

void AnimaScene::TerminateManagers()
{
	if (_nodesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _nodesManager);
		_nodesManager = nullptr;
	}

	if (_geometriesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _geometriesManager);
		_geometriesManager = nullptr;
	}

	if (_nodeInstancesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _nodeInstancesManager);
		_nodeInstancesManager = nullptr;
	}

	if (_geometryInstancesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _geometryInstancesManager);
		_geometryInstancesManager = nullptr;
	}

	if (_camerasManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _camerasManager);
		_camerasManager = nullptr;
	}

	if (_texturesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _texturesManager);
		_texturesManager = nullptr;
	}
	
	if (_lightsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _lightsManager);
		_lightsManager = nullptr;
	}

	if (_dataGeneratorManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _dataGeneratorManager);
		_dataGeneratorManager = nullptr;
	}

	if (_materialsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _materialsManager);
		_materialsManager = nullptr;
	}

	if (_animationsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _animationsManager);
		_animationsManager = nullptr;
	}
}

void AnimaScene::InitializePhysics()
{
	TerminatePhysics();
	
	_physCollisionConfiguration = new btDefaultCollisionConfiguration();
	_physCollisionDispatcher = new btCollisionDispatcher(_physCollisionConfiguration);
	_physBroadphaseInterface = (btBroadphaseInterface*)new btDbvtBroadphase();
	_physConstraintSolver = (btConstraintSolver*)new btSequentialImpulseConstraintSolver;
	_physWorld = new btDiscreteDynamicsWorld((btDispatcher*)_physCollisionDispatcher, _physBroadphaseInterface, _physConstraintSolver, _physCollisionConfiguration);
	
	_physWorld->setGravity(btVector3(_worldGravity.x, _worldGravity.y, _worldGravity.z));
}

void AnimaScene::TerminatePhysics()
{
	if(_physBroadphaseInterface != nullptr)
	{
		delete _physBroadphaseInterface;
		_physBroadphaseInterface = nullptr;
	}
	
	if(_physCollisionConfiguration != nullptr)
	{
		delete _physCollisionConfiguration;
		_physCollisionConfiguration = nullptr;
	}
	
	if(_physCollisionDispatcher != nullptr)
	{
		delete _physCollisionDispatcher;
		_physCollisionDispatcher = nullptr;
	}
	
	if(_physConstraintSolver != nullptr)
	{
		delete _physConstraintSolver;
		_physConstraintSolver = nullptr;
	}
	
	if(_physWorld != nullptr)
	{
		delete _physWorld;
		_physWorld = nullptr;
	}
}

void AnimaScene::InitializePhysicObjects()
{
	if(_physWorld != nullptr)
	{
		AInt count = _geometryInstancesManager->GetGeometryInstancesCount();
		for(AInt i = 0; i < count; i++)
		{
			AnimaGeometryInstance* instance = _geometryInstancesManager->GetGeometryInstance(i);
			instance->InitializePhysicData();
			
			if(instance->GetPhysRigidBody() != nullptr)
				_physWorld->addRigidBody(instance->GetPhysRigidBody());
		}
	}
}

void AnimaScene::Activate()
{
	_active = true;
	
	if (_engine != nullptr && _engine->GetScenesManager() != nullptr)
		_engine->GetScenesManager()->NotifySceneActivation(this);
}

void AnimaScene::Deactivate()
{
	_active = false;
	
	if (_engine != nullptr && _engine->GetScenesManager() != nullptr)
		_engine->GetScenesManager()->NotifySceneDeactivation(this);
}

void AnimaScene::StartScene()
{
	_timer.Reset();
	_totalSceneTime = 0.0f;
	_isRunning = true;
}

void AnimaScene::StepScene()
{
	// Calcolo il tempo trascorso dall'ultimo step e anche il tempo totale di animazione della scena
	AFloat elapsedTime = (AFloat)_timer.Elapsed();
	_totalSceneTime += elapsedTime;
	
	// Riavvio il timer in modo da calcolare il tempo trascorso alla prossima chiamata di StepScene
	_timer.Reset();
	
	if(_keyboardInteractor != nullptr)
		_keyboardInteractor->UpdateScene(this, elapsedTime);
	
	if(_mouseInteractor != nullptr)
		_mouseInteractor->UpdateScene(this, elapsedTime);
	
	if(_joystickInteractor != nullptr)
		_joystickInteractor->UpdateScene(this, elapsedTime);
	
	// Se è stata inizializzata la simulazione fisica allora la faccio avanzare
	if(_physWorld != nullptr)
		_physWorld->stepSimulation(btScalar(elapsedTime));
	
	// Faccio avanzare i generatori di valori
	if(_dataGeneratorManager != nullptr)
		_dataGeneratorManager->UpdateValues(elapsedTime);
	
	AInt count = _geometryInstancesManager->GetGeometryInstancesCount();
	for(AInt i = 0; i < count; i++)
		_geometryInstancesManager->GetGeometryInstance(i)->GetTransformation()->UpdateMatrix();
	
	_camerasManager->UpdateCameras(elapsedTime);
	
	_lightsManager->UpdateLightsMatrix(_camerasManager->GetActiveCamera());
}

END_ANIMA_ENGINE_NAMESPACE
