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
#include "AnimaArray.h"
#include "AnimaMesh.h"
#include "AnimaLight.h"
#include "AnimaCamera.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScene::AnimaScene(AnimaEngine* engine)
{
	_engine = engine;
	
	_modelsManager = nullptr;
	_meshesManager = nullptr;
	_modelInstancesManager = nullptr;
	
	_camerasManager = nullptr;
	_texturesManager = nullptr;
	_dataGeneratorsManager = nullptr;
	_materialsManager = nullptr;
	_lightsManager = nullptr;

	Initialize();
}

AnimaScene::AnimaScene(const AnimaScene& src)
{
	_engine = src._engine;

	_modelsManager = src._modelsManager;
	_meshesManager = src._meshesManager;
	_modelInstancesManager = src._modelInstancesManager;

	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
	_dataGeneratorsManager = src._dataGeneratorsManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
}

AnimaScene::AnimaScene(AnimaScene&& src)
{
	_engine = src._engine;

	_modelsManager = src._modelsManager;
	_meshesManager = src._meshesManager;
	_modelInstancesManager = src._modelInstancesManager;

	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
	_dataGeneratorsManager = src._dataGeneratorsManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
}

AnimaScene::~AnimaScene()
{
	Terminate();
}

AnimaScene& AnimaScene::operator=(const AnimaScene& src)
{
	if (this != &src)
	{
		Terminate();

		_engine = src._engine;

		_modelsManager = src._modelsManager;
		_meshesManager = src._meshesManager;
		_modelInstancesManager = src._modelInstancesManager;

		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
		_dataGeneratorsManager = src._dataGeneratorsManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
	}
	
	return *this;
}

AnimaScene& AnimaScene::operator=(AnimaScene&& src)
{
	if (this != &src)
	{
		Terminate();

		_engine = src._engine;

		_modelsManager = src._modelsManager;
		_meshesManager = src._meshesManager;
		_modelInstancesManager = src._modelInstancesManager;

		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
		_dataGeneratorsManager = src._dataGeneratorsManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
	}
	
	return *this;
}

void AnimaScene::Initialize()
{
	_camerasManager = AnimaAllocatorNamespace::AllocateNew<AnimaCamerasManager>(*_engine->GetManagersAllocator(), this);
	_texturesManager = AnimaAllocatorNamespace::AllocateNew<AnimaTexturesManager>(*_engine->GetManagersAllocator(), this);
	_dataGeneratorsManager = AnimaAllocatorNamespace::AllocateNew<AnimaDataGeneratorsManager>(*_engine->GetManagersAllocator(), this);
	_materialsManager = AnimaAllocatorNamespace::AllocateNew<AnimaMaterialsManager>(*_engine->GetManagersAllocator(), this);
	_lightsManager = AnimaAllocatorNamespace::AllocateNew<AnimaLightsManager>(*_engine->GetManagersAllocator(), this);
	
	_meshesManager = AnimaAllocatorNamespace::AllocateNew<AnimaMeshesManager>(*_engine->GetManagersAllocator(), this);
	_modelsManager = AnimaAllocatorNamespace::AllocateNew<AnimaModelsManager>(*_engine->GetManagersAllocator(), this, _meshesManager, _materialsManager);
	_modelInstancesManager = AnimaAllocatorNamespace::AllocateNew<AnimaModelInstancesManager>(*_engine->GetManagersAllocator(), this, _modelsManager, _materialsManager);
}

void AnimaScene::Terminate()
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

	if (_lightsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_engine->GetManagersAllocator(), _lightsManager);
		_lightsManager = nullptr;
	}
}

END_ANIMA_ENGINE_NAMESPACE
