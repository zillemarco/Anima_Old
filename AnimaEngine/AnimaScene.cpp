//
//  AnimaScene.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaScene.h"
#include "AnimaModelsManager.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaLightsManager.h"
#include "AnimaModelInstancesManager.h"
#include "AnimaMeshInstancesManager.h"
#include "AnimaAnimationsManager.h"
#include "AnimaMeshInstance.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScene::AnimaScene(AnimaEngine* engine, const AnimaString& name)
	: AnimaNamedObject(name, nullptr)
{
	IMPLEMENT_ANIMA_CLASS(AnimaScene);
	
	_engine = engine;
	
	_modelsManager = nullptr;
	_meshesManager = nullptr;
	_modelInstancesManager = nullptr;
	_meshInstancesManager = nullptr;
	
	_camerasManager = nullptr;
	_texturesManager = nullptr;
	_dataGeneratorsManager = nullptr;
	_materialsManager = nullptr;
	_lightsManager = nullptr;
	_animationsManager = nullptr;
	
	_physBroadphaseInterface = nullptr;
	_physCollisionConfiguration = nullptr;
	_physCollisionDispatcher = nullptr;
	_physConstraintSolver = nullptr;
	_physWorld = nullptr;
	
	_worldGravity = AnimaVertex3f(0.0f, -9.81f, 0.0f);

	InitializeManagers();
}

AnimaScene::AnimaScene(const AnimaScene& src)
	: AnimaNamedObject(src)
{
	_engine = src._engine;

	_modelsManager = src._modelsManager;
	_meshesManager = src._meshesManager;
	_modelInstancesManager = src._modelInstancesManager;
	_meshInstancesManager = src._meshInstancesManager;

	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
	_dataGeneratorsManager = src._dataGeneratorsManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
	_animationsManager = src._animationsManager;
	
	_physBroadphaseInterface = src._physBroadphaseInterface;
	_physCollisionConfiguration = src._physCollisionConfiguration;
	_physCollisionDispatcher = src._physCollisionDispatcher;
	_physConstraintSolver = src._physConstraintSolver;
	_physWorld = src._physWorld;
	
	_worldGravity = src._worldGravity;
}

AnimaScene::AnimaScene(AnimaScene&& src)
	: AnimaNamedObject(src)
{
	_engine = src._engine;

	_modelsManager = src._modelsManager;
	_meshesManager = src._meshesManager;
	_modelInstancesManager = src._modelInstancesManager;
	_meshInstancesManager = src._meshInstancesManager;

	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
	_dataGeneratorsManager = src._dataGeneratorsManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
	_animationsManager = src._animationsManager;
	
	_physBroadphaseInterface = src._physBroadphaseInterface;
	_physCollisionConfiguration = src._physCollisionConfiguration;
	_physCollisionDispatcher = src._physCollisionDispatcher;
	_physConstraintSolver = src._physConstraintSolver;
	_physWorld = src._physWorld;
	
	_worldGravity = src._worldGravity;
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
		
		AnimaNamedObject::operator=(src);

		_engine = src._engine;

		_modelsManager = src._modelsManager;
		_meshesManager = src._meshesManager;
		_modelInstancesManager = src._modelInstancesManager;
		_meshInstancesManager = src._meshInstancesManager;

		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
		_dataGeneratorsManager = src._dataGeneratorsManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
		_animationsManager = src._animationsManager;
		
		_physBroadphaseInterface = src._physBroadphaseInterface;
		_physCollisionConfiguration = src._physCollisionConfiguration;
		_physCollisionDispatcher = src._physCollisionDispatcher;
		_physConstraintSolver = src._physConstraintSolver;
		_physWorld = src._physWorld;
		
		_worldGravity = src._worldGravity;
	}
	
	return *this;
}

AnimaScene& AnimaScene::operator=(AnimaScene&& src)
{
	if (this != &src)
	{
		TerminatePhysics();
		TerminateManagers();
		
		AnimaNamedObject::operator=(src);

		_engine = src._engine;

		_modelsManager = src._modelsManager;
		_meshesManager = src._meshesManager;
		_modelInstancesManager = src._modelInstancesManager;
		_meshInstancesManager = src._meshInstancesManager;

		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
		_dataGeneratorsManager = src._dataGeneratorsManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
		_animationsManager = src._animationsManager;
		
		_physBroadphaseInterface = src._physBroadphaseInterface;
		_physCollisionConfiguration = src._physCollisionConfiguration;
		_physCollisionDispatcher = src._physCollisionDispatcher;
		_physConstraintSolver = src._physConstraintSolver;
		_physWorld = src._physWorld;
		
		_worldGravity = src._worldGravity;
	}
	
	return *this;
}

void AnimaScene::InitializeManagers()
{
	TerminateManagers();
	
	_camerasManager = AnimaAllocatorNamespace::AllocateNew<AnimaCamerasManager>(*_engine->GetManagersAllocator(), this);
	_texturesManager = AnimaAllocatorNamespace::AllocateNew<AnimaTexturesManager>(*_engine->GetManagersAllocator(), this);
	_dataGeneratorsManager = AnimaAllocatorNamespace::AllocateNew<AnimaDataGeneratorsManager>(*_engine->GetManagersAllocator(), this);
	_lightsManager = AnimaAllocatorNamespace::AllocateNew<AnimaLightsManager>(*_engine->GetManagersAllocator(), this);

	_materialsManager = AnimaAllocatorNamespace::AllocateNew<AnimaMaterialsManager>(*_engine->GetManagersAllocator(), this, _texturesManager);
	_animationsManager = AnimaAllocatorNamespace::AllocateNew<AnimaAnimationsManager>(*_engine->GetManagersAllocator(), this);
	
	_meshesManager = AnimaAllocatorNamespace::AllocateNew<AnimaMeshesManager>(*_engine->GetManagersAllocator(), this, _materialsManager);
	_modelsManager = AnimaAllocatorNamespace::AllocateNew<AnimaModelsManager>(*_engine->GetManagersAllocator(), this, _meshesManager, _materialsManager, _animationsManager);
	_meshInstancesManager = AnimaAllocatorNamespace::AllocateNew<AnimaMeshInstancesManager>(*_engine->GetManagersAllocator(), this, _meshesManager);
	_modelInstancesManager = AnimaAllocatorNamespace::AllocateNew<AnimaModelInstancesManager>(*_engine->GetManagersAllocator(), this, _modelsManager, _meshInstancesManager);
}

void AnimaScene::TerminateManagers()
{
	if (_modelsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _modelsManager);
		_modelsManager = nullptr;
	}

	if (_meshesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _meshesManager);
		_meshesManager = nullptr;
	}

	if (_modelInstancesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _modelInstancesManager);
		_modelInstancesManager = nullptr;
	}

	if (_meshInstancesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _meshInstancesManager);
		_meshInstancesManager = nullptr;
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

	if (_dataGeneratorsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _dataGeneratorsManager);
		_dataGeneratorsManager = nullptr;
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
		AInt count = _meshInstancesManager->GetMeshInstancesCount();
		for(AInt i = 0; i < count; i++)
		{
			AnimaMeshInstance* instance = _meshInstancesManager->GetMeshInstance(i);
			instance->InitializePhysicData();
			
			if(instance->GetPhysRigidBody() != nullptr)
				_physWorld->addRigidBody(instance->GetPhysRigidBody());
		}
	}
}

END_ANIMA_ENGINE_NAMESPACE
