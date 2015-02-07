//
//  AnimaStage.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaStage.h"
#include "AnimaModelsManager.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaLightsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define _ANIMA_LOCAL_MEMORY_SIZE			125829120	// 120 MB
#define _ANIMA_MODELS_MEMORY_SIZE			41943040	// 40 MB
#define _ANIMA_ALLOCATORS_NUMBER			10
#define _ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE	((_ANIMA_LOCAL_MEMORY_SIZE - _ANIMA_MODELS_MEMORY_SIZE) / _ANIMA_ALLOCATORS_NUMBER) - 300

AnimaStage::AnimaStage()
{
	_localMemory = nullptr;
	_localMemorySize = 0;
	_localMemoryAllocator = nullptr;

	_modelDataAllocator = nullptr;
	_modelsAllocator = nullptr;
	_genericAllocator = nullptr;
	_stringAllocator = nullptr;
	_shadersAllocator = nullptr;
	_camerasAllocator = nullptr;
	_texturesAllocator = nullptr;
	_materialsAllocator = nullptr;
	_lightsAllocator = nullptr;
	_dataGeneratorsAllocator = nullptr;

	_modelsManager = nullptr;
	_shadersManager = nullptr;
	_camerasManager = nullptr;
	_texturesManager = nullptr;
	_dataGeneratorsManager = nullptr;
	_materialsManager = nullptr;
	_lightsManager = nullptr;
}

AnimaStage::AnimaStage(const AnimaStage& src)
{
	_localMemory = src._localMemory;
	_localMemorySize = src._localMemorySize;
	_localMemoryAllocator = src._localMemoryAllocator;

	_modelDataAllocator = src._modelDataAllocator;
	_modelsAllocator = src._modelsAllocator;
	_genericAllocator = src._genericAllocator;
	_stringAllocator = src._stringAllocator;
	_shadersAllocator = src._shadersAllocator;
	_camerasAllocator = src._camerasAllocator;
	_texturesAllocator = src._texturesAllocator;
	_materialsAllocator = src._materialsAllocator;
	_lightsAllocator = src._lightsAllocator;
	_dataGeneratorsAllocator = src._dataGeneratorsAllocator;

	_modelsManager = src._modelsManager;
	_shadersManager = src._shadersManager;
	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
	_dataGeneratorsManager = src._dataGeneratorsManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
}

AnimaStage::AnimaStage(AnimaStage&& src)
{
	_localMemory = src._localMemory;
	_localMemorySize = src._localMemorySize;
	_localMemoryAllocator = src._localMemoryAllocator;

	_modelDataAllocator = src._modelDataAllocator;
	_modelsAllocator = src._modelsAllocator;
	_genericAllocator = src._genericAllocator;
	_stringAllocator = src._stringAllocator;
	_shadersAllocator = src._shadersAllocator;
	_camerasAllocator = src._camerasAllocator;
	_texturesAllocator = src._texturesAllocator;
	_materialsAllocator = src._materialsAllocator;
	_lightsAllocator = src._lightsAllocator;
	_dataGeneratorsAllocator = src._dataGeneratorsAllocator;

	_modelsManager = src._modelsManager;
	_shadersManager = src._shadersManager;
	_camerasManager = src._camerasManager;
	_texturesManager = src._texturesManager;
	_dataGeneratorsManager = src._dataGeneratorsManager;
	_materialsManager = src._materialsManager;
	_lightsManager = src._lightsManager;
}

AnimaStage::~AnimaStage()
{
	Terminate();
}

AnimaStage& AnimaStage::operator=(const AnimaStage& src)
{
	if (this != &src)
	{
		_localMemory = src._localMemory;
		_localMemorySize = src._localMemorySize;
		_localMemoryAllocator = src._localMemoryAllocator;

		_modelDataAllocator = src._modelDataAllocator;
		_modelsAllocator = src._modelsAllocator;
		_genericAllocator = src._genericAllocator;
		_stringAllocator = src._stringAllocator;
		_shadersAllocator = src._shadersAllocator;
		_camerasAllocator = src._camerasAllocator;

		_modelsManager = src._modelsManager;
		_shadersManager = src._shadersManager;
		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
		_dataGeneratorsManager = src._dataGeneratorsManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
	}
	
	return *this;
}

AnimaStage& AnimaStage::operator=(AnimaStage&& src)
{
	if (this != &src)
	{
		_localMemory = src._localMemory;
		_localMemorySize = src._localMemorySize;
		_localMemoryAllocator = src._localMemoryAllocator;

		_modelDataAllocator = src._modelDataAllocator;
		_modelsAllocator = src._modelsAllocator;
		_genericAllocator = src._genericAllocator;
		_stringAllocator = src._stringAllocator;
		_shadersAllocator = src._shadersAllocator;
		_camerasAllocator = src._camerasAllocator;

		_modelsManager = src._modelsManager;
		_shadersManager = src._shadersManager;
		_camerasManager = src._camerasManager;
		_texturesManager = src._texturesManager;
		_dataGeneratorsManager = src._dataGeneratorsManager;
		_materialsManager = src._materialsManager;
		_lightsManager = src._lightsManager;
	}
	
	return *this;
}

void AnimaStage::Initialize()
{
	InitializeMemorySystem();
	InitializeManagers();
}

void AnimaStage::InitializeMemorySystem()
{
	ANIMA_ASSERT(_localMemory == nullptr && _localMemorySize == 0);

	_localMemorySize = _ANIMA_LOCAL_MEMORY_SIZE;
	_localMemory = malloc(_localMemorySize);

	// L'unica new di AnimaEngine.
	// Tutta la rimanente memoria utilizzata verrà 'allocata' dai custom allocators
	_localMemoryAllocator = new AnimaFreeListAllocator(_localMemorySize, _localMemory);

	_modelDataAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_MODELS_MEMORY_SIZE, *_localMemoryAllocator);
	_modelsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_genericAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_managersAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_stringAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_shadersAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_camerasAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_texturesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_materialsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_lightsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_dataGeneratorsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);

	ANIMA_ASSERT((_modelDataAllocator->GetSize() + _modelsAllocator->GetSize() + _genericAllocator->GetSize() + _managersAllocator->GetSize() + _stringAllocator->GetSize() + _shadersAllocator->GetSize() + _camerasAllocator->GetSize() + _texturesAllocator->GetSize() + _materialsAllocator->GetSize() + _lightsAllocator->GetSize() + _dataGeneratorsAllocator->GetSize()) < _localMemorySize - 1000);
}

void AnimaStage::InitializeManagers()
{
	_modelsManager = AnimaAllocatorNamespace::AllocateNew<AnimaModelsManager>(*_managersAllocator, this);
	_shadersManager = AnimaAllocatorNamespace::AllocateNew<AnimaShadersManager>(*_managersAllocator, this);
	_camerasManager = AnimaAllocatorNamespace::AllocateNew<AnimaCamerasManager>(*_managersAllocator, this);
	_texturesManager = AnimaAllocatorNamespace::AllocateNew<AnimaTexturesManager>(*_managersAllocator, this);
	_dataGeneratorsManager = AnimaAllocatorNamespace::AllocateNew<AnimaDataGeneratorsManager>(*_managersAllocator, this);
	_materialsManager = AnimaAllocatorNamespace::AllocateNew<AnimaMaterialsManager>(*_managersAllocator, this);
	_lightsManager = AnimaAllocatorNamespace::AllocateNew<AnimaLightsManager>(*_managersAllocator, this);
}

void AnimaStage::Terminate()
{
	TerminateManagers();
	TerminateMemorySystem();
}

void AnimaStage::TerminateMemorySystem()
{
	if (_modelDataAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_modelDataAllocator, *_localMemoryAllocator);
		_modelDataAllocator = nullptr;
	}

	if (_modelsAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_modelsAllocator, *_localMemoryAllocator);
		_modelsAllocator = nullptr;
	}

	if (_genericAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_genericAllocator, *_localMemoryAllocator);
		_genericAllocator = nullptr;
	}

	if (_managersAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_managersAllocator, *_localMemoryAllocator);
		_managersAllocator = nullptr;
	}

	if (_stringAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_stringAllocator, *_localMemoryAllocator);
		_stringAllocator = nullptr;
	}

	if (_shadersAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_shadersAllocator, *_localMemoryAllocator);
		_shadersAllocator = nullptr;
	}

	if (_camerasAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_camerasAllocator, *_localMemoryAllocator);
		_camerasAllocator = nullptr;
	}

	if (_texturesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_texturesAllocator, *_localMemoryAllocator);
		_texturesAllocator = nullptr;
	}

	if (_materialsAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_materialsAllocator, *_localMemoryAllocator);
		_materialsAllocator = nullptr;
	}

	if (_lightsAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_lightsAllocator, *_localMemoryAllocator);
		_lightsAllocator = nullptr;
	}
	
	if (_dataGeneratorsAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_dataGeneratorsAllocator, *_localMemoryAllocator);
		_dataGeneratorsAllocator = nullptr;
	}

	if (_localMemoryAllocator != nullptr)
	{
		delete _localMemoryAllocator;
		_localMemoryAllocator = nullptr;
	}

	if (_localMemory != nullptr && _localMemorySize > 0)
	{
		free(_localMemory);

		_localMemory = nullptr;
		_localMemorySize = 0;
	}
}

void AnimaStage::TerminateManagers()
{
	if (_modelsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _modelsManager);
		_modelsManager = nullptr;
	}

	if (_shadersManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _shadersManager);
		_shadersManager = nullptr;
	}

	if (_camerasManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _camerasManager);
		_camerasManager = nullptr;
	}

	if (_texturesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _texturesManager);
		_texturesManager = nullptr;
	}

	if (_dataGeneratorsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _dataGeneratorsManager);
		_dataGeneratorsManager = nullptr;
	}

	if (_materialsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _materialsManager);
		_materialsManager = nullptr;
	}

	if (_lightsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _lightsManager);
		_lightsManager = nullptr;
	}
}

END_ANIMA_ENGINE_NAMESPACE
