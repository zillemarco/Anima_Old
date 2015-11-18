#include "AnimaEngine.h"
#include "AnimaScenesManager.h"
#include "AnimaShadersManager.h"
#include "AnimaParallelProgramsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

bool								Anima::AnimaEngine::_animaEngineInitialized = false;
int									Anima::AnimaEngine::_animaEngineCount = 0;
bool								Anima::AnimaEngine::_usedExternal = false;
AChar								Anima::AnimaEngine::_logFilePath[PATH_MAX] = "";

#define _ANIMA_LOCAL_MEMORY_SIZE			524288000 	// 500 MB
#define _ANIMA_GEOMETRIES_MEMORY_SIZE			52428800	// 50 MB
#define _ANIMA_TEXTURES_MEMORY_SIZE			104857600	// 100 MB
#define _ANIMA_ALLOCATORS_NUMBER			13
#define _ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE	((_ANIMA_LOCAL_MEMORY_SIZE - _ANIMA_GEOMETRIES_MEMORY_SIZE - _ANIMA_TEXTURES_MEMORY_SIZE) / _ANIMA_ALLOCATORS_NUMBER) - 300

AnimaEngine::AnimaEngine()
{
	_localMemory = nullptr;
	_localMemorySize = 0;
	_localMemoryAllocator = nullptr;

	_geometriesAllocator = nullptr;
	_nodesAllocator = nullptr;
	_nodeInstancesAllocator = nullptr;
	_geometryInstancesAllocator = nullptr;
	_genericAllocator = nullptr;
	_shadersAllocator = nullptr;
	_camerasAllocator = nullptr;
	_texturesAllocator = nullptr;
	_materialsAllocator = nullptr;
	_lightsAllocator = nullptr;
	_dataGeneratorsAllocator = nullptr;
	_scenesAllocator = nullptr;
	_animationsAllocator = nullptr;
	_parallelProgramsAllocator = nullptr;

	_scenesManager = nullptr;
	_shadersManager = nullptr;
	_dataGeneratorsManager = nullptr;
	_parallelProgramsManager = nullptr;

	_animaEngineCount++;
}

AnimaEngine::~AnimaEngine()
{
	_animaEngineCount--;

	Terminate();
}

bool AnimaEngine::Initialize()
{
	InitializeMemorySystem();
	InitializeManagers();

	if (!InitializeWindowSystem())
		return false;

	return true;
}

void AnimaEngine::InitializeMemorySystem()
{
	ANIMA_ASSERT(_localMemory == nullptr && _localMemorySize == 0);

	_localMemorySize = _ANIMA_LOCAL_MEMORY_SIZE;
	_localMemory = malloc(_localMemorySize);

	// L'unica new di AnimaEngine.
	// Tutta la rimanente memoria utilizzata verrà 'allocata' dai custom allocators
	_localMemoryAllocator = new AnimaFreeListAllocator(_localMemorySize, _localMemory);

	_geometriesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_GEOMETRIES_MEMORY_SIZE, *_localMemoryAllocator);
	_nodesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_nodeInstancesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_geometryInstancesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);

	_texturesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_TEXTURES_MEMORY_SIZE, *_localMemoryAllocator);
	_genericAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_managersAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_shadersAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_camerasAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_materialsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_lightsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_dataGeneratorsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_scenesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_animationsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_parallelProgramsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);

	ASizeT usedSize = 0;
	usedSize += _geometriesAllocator->GetSize();
	usedSize += _nodesAllocator->GetSize();
	usedSize += _nodeInstancesAllocator->GetSize();
	usedSize += _geometryInstancesAllocator->GetSize();
	usedSize += _genericAllocator->GetSize(); 
	usedSize += _managersAllocator->GetSize(); 
	usedSize += _shadersAllocator->GetSize(); 
	usedSize += _camerasAllocator->GetSize(); 
	usedSize += _texturesAllocator->GetSize(); 
	usedSize += _materialsAllocator->GetSize(); 
	usedSize += _lightsAllocator->GetSize(); 
	usedSize += _dataGeneratorsAllocator->GetSize(); 
	usedSize += _scenesAllocator->GetSize();
	usedSize += _animationsAllocator->GetSize();
	usedSize += _parallelProgramsAllocator->GetSize();

	ANIMA_ASSERT(usedSize < _localMemorySize - 1000);
}

bool AnimaEngine::InitializeWindowSystem()
{
	if (_animaEngineInitialized)
		return true;

	_animaEngineInitialized = true;

	return true;
}

void AnimaEngine::InitializeManagers()
{
	_scenesManager = AnimaAllocatorNamespace::AllocateNew<AnimaScenesManager>(*_managersAllocator, this);
	_shadersManager = AnimaAllocatorNamespace::AllocateNew<AnimaShadersManager>(*_managersAllocator, this);
	_dataGeneratorsManager = AnimaAllocatorNamespace::AllocateNew<AnimaDataGeneratorsManager>(*_managersAllocator, nullptr, this);
	_parallelProgramsManager = AnimaAllocatorNamespace::AllocateNew<AnimaParallelProgramsManager>(*_managersAllocator, this);
}

void AnimaEngine::Terminate()
{
	TerminateWindowSystem();
	TerminateManagers();
	TerminateMemorySystem();
}

void AnimaEngine::TerminateMemorySystem()
{
	if (_geometriesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_geometriesAllocator, *_localMemoryAllocator);
		_geometriesAllocator = nullptr;
	}

	if (_nodesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_nodesAllocator, *_localMemoryAllocator);
		_nodesAllocator = nullptr;
	}

	if (_nodeInstancesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_nodeInstancesAllocator, *_localMemoryAllocator);
		_nodeInstancesAllocator = nullptr;
	}

	if (_geometryInstancesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_geometryInstancesAllocator, *_localMemoryAllocator);
		_geometryInstancesAllocator = nullptr;
	}

	if (_genericAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_genericAllocator, *_localMemoryAllocator);
		_genericAllocator = nullptr;
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

	if (_scenesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_scenesAllocator, *_localMemoryAllocator);
		_scenesAllocator = nullptr;
	}

	if (_animationsAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_animationsAllocator, *_localMemoryAllocator);
		_animationsAllocator = nullptr;
	}

	if (_parallelProgramsAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_parallelProgramsAllocator, *_localMemoryAllocator);
		_parallelProgramsAllocator = nullptr;
	}

	if (_managersAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_managersAllocator, *_localMemoryAllocator);
		_managersAllocator = nullptr;
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

void AnimaEngine::TerminateManagers()
{
	if (_scenesManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _scenesManager);
		_scenesManager = nullptr;
	}

	if (_shadersManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _shadersManager);
		_shadersManager = nullptr;
	}

	if (_dataGeneratorsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _dataGeneratorsManager);
		_dataGeneratorsManager = nullptr;
	}

	if (_parallelProgramsManager != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_managersAllocator, _parallelProgramsManager);
		_parallelProgramsManager = nullptr;
	}
}

void AnimaEngine::TerminateWindowSystem()
{
	if (!_animaEngineInitialized)
		return;
}

void AnimaEngine::DumpMemory(const char* fileName, bool bLogToFile)
{
	bool bCanClose = true;
	
	if(bLogToFile)
	{
		if(!freopen(fileName, "a", stdout))
		{
			fclose(stdout);
			bCanClose = false;
		}
	}
	else
		bCanClose = false;
		
	if (bCanClose)
		fclose(stdout);
}

void AnimaEngine::SetLogFilePath(const AChar* path)
{
	strcpy(_logFilePath, path);
}

AChar* AnimaEngine::GetLogFilePath()
{
	return _logFilePath;
}

void AnimaEngine::SetUsedExternal(bool bUsedExternal)
{
	_usedExternal = bUsedExternal; 
}

bool AnimaEngine::IsUsedExteral() 
{ 
	return _usedExternal; 
}

bool AnimaEngine::IsInitialized()
{ 
	return _animaEngineInitialized; 
}

END_ANIMA_ENGINE_NAMESPACE
