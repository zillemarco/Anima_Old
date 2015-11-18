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
	TerminatePhysics();
	TerminateManagers();
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
