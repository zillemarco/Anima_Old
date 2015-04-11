#include "AnimaEngine.h"
#include "AnimaScenesManager.h"
#include "AnimaShadersManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

_INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE(AnimaEngine)

bool								AnimaEngine::_animaEngineInitialized = false;
_AnimaEngineWindowmonitor**			AnimaEngine::_monitors = NULL;
int									AnimaEngine::_monitorCount = 0;
_AnimaEngineWindowMonitorCallbacks	AnimaEngine::_callbacks;
int									AnimaEngine::_animaEngineCount = 0;
_AnimaEngineWindowHints				AnimaEngine::_windowHints;
bool								AnimaEngine::_platformLibraryWindowStateInitialized = false;
bool								AnimaEngine::_platformLibraryContextStateInitialized = false;
bool								AnimaEngine::_platformLibraryTimeStateInitialized = false;
bool								AnimaEngine::_platformLibraryJoystickStateInitialized = false;
bool								AnimaEngine::_platformLibraryTLSStateInitialized = false;
bool								AnimaEngine::_usedExternal = false;
bool								AnimaEngine::_glewExtensionsInitialized = false;
AChar								AnimaEngine::_logFilePath[PATH_MAX] = "";

#define _ANIMA_LOCAL_MEMORY_SIZE			524288000 	// 500 MB
#define _ANIMA_MODELS_MEMORY_SIZE			52428800	// 50 MB
#define _ANIMA_TEXTURES_MEMORY_SIZE			419430400	// 400 MB
#define _ANIMA_ALLOCATORS_NUMBER			10
#define _ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE	((_ANIMA_LOCAL_MEMORY_SIZE - _ANIMA_MODELS_MEMORY_SIZE - _ANIMA_TEXTURES_MEMORY_SIZE) / _ANIMA_ALLOCATORS_NUMBER) - 300

AnimaEngine::AnimaEngine()
{
	_windowListHead = nullptr;
	_focusedWindow = nullptr;

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
	_scenesAllocator = nullptr;

	_scenesManager = nullptr;
	_shadersManager = nullptr;

	_animaEngineCount++;
}

AnimaEngine::~AnimaEngine()
{
	_animaEngineCount--;

	Terminate();
}

bool AnimaEngine::Initialize()
{
	if(!_platformLibraryWindowStateInitialized)
		_INIT_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE;
	
	if(!_platformLibraryTimeStateInitialized)
		_INIT_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE;
	
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

	_modelDataAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_MODELS_MEMORY_SIZE, *_localMemoryAllocator);
	_texturesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_TEXTURES_MEMORY_SIZE, *_localMemoryAllocator);
	_modelsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_genericAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_managersAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_stringAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_shadersAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_camerasAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_materialsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_lightsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_dataGeneratorsAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);
	_scenesAllocator = AnimaAllocatorNamespace::NewAnimaFreeListAllocator(_ANIMA_OTHER_ALLOCATORS_MEMORY_SIZE, *_localMemoryAllocator);

	ASizeT usedSize = _modelDataAllocator->GetSize() + _modelsAllocator->GetSize() + _genericAllocator->GetSize() + _managersAllocator->GetSize() + _stringAllocator->GetSize() + _shadersAllocator->GetSize() + _camerasAllocator->GetSize() + _texturesAllocator->GetSize() + _materialsAllocator->GetSize() + _lightsAllocator->GetSize() + _dataGeneratorsAllocator->GetSize() + _scenesAllocator->GetSize();

	ANIMA_ASSERT(usedSize < _localMemorySize - 1000);
}

bool AnimaEngine::InitializeWindowSystem()
{
	if (_animaEngineInitialized)
		return true;

	if (!_AnimaEngineWindowPlatformInit())
	{
		_AnimaEngineWindowPlatformTerminate();
		return false;
	}

	_monitors = _AnimaEngineWindowPlatformGetMonitors(&_monitorCount);
	if (_monitorCount <= 0)
	{
		_AnimaEngineWindowPlatformTerminate();
		return false;
	}

	_animaEngineInitialized = true;

	DefaultWindowHints();

	return true;
}

void AnimaEngine::InitializeManagers()
{
	_scenesManager = AnimaAllocatorNamespace::AllocateNew<AnimaScenesManager>(*_managersAllocator, this);
	_shadersManager = AnimaAllocatorNamespace::AllocateNew<AnimaShadersManager>(*_managersAllocator, this);
}

void AnimaEngine::Terminate()
{
	TerminateWindowSystem();
	TerminateManagers();
	TerminateMemorySystem();
}

void AnimaEngine::TerminateMemorySystem()
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

	if (_scenesAllocator != nullptr)
	{
		AnimaAllocatorNamespace::DeleteAnimaFreeListAllocator(*_scenesAllocator, *_localMemoryAllocator);
		_scenesAllocator = nullptr;
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
}

void AnimaEngine::TerminateWindowSystem()
{
	if (!_animaEngineInitialized)
		return;

	//memset(&_glfw.callbacks, 0, sizeof(_glfw.callbacks));

	while (_windowListHead)
		DestroyAnimaWindow((AnimaEngineWindow_Base*)_windowListHead);

	//while (_cursorListHead)
	//	DestroyAnimaCursor((AnimaEngineWindowcursor*)_cursorListHead);

	if (_animaEngineCount == 0)
	{
		for (int i = 0; i < _monitorCount; i++)
		{
			_AnimaEngineWindowmonitor* monitor = _monitors[i];
			if (monitor->_originalRamp._size)
				_AnimaEngineWindowPlatformSetGammaRamp(monitor, &monitor->_originalRamp);
		}

		_AnimaEngineWindowFreeMonitors(_monitors, _monitorCount);
		_monitors = NULL;
		_monitorCount = 0;

		_AnimaEngineWindowPlatformTerminate();

		//memset(&_glfw, 0, sizeof(_glfw));
		_animaEngineInitialized = false;
	}
}

void AnimaEngine::DestroyAnimaWindow(AnimaEngineWindow_Base* handle)
{
	AnimaEngineWindow_Base* window = (AnimaEngineWindow_Base*)handle;

	_ANIMA_ENGINE_REQUIRE_INIT();

	// Allow closing of NULL (to match the behavior of free)
	if (window == NULL)
		return;

	// Clear all callbacks to avoid exposing a half torn-down window object
	//memset(&window->_callbacks, 0, sizeof(window->callbacks));

	// The window's context must not be current on another thread when the
	// window is destroyed
	if (window == _AnimaEngineWindowPlatformGetCurrentContext())
		_AnimaEngineWindowPlatformMakeContextCurrent(NULL);

	// Clear the focused window pointer if this is the focused window
	if (window == window->GetEngine()->GetFocusedWindow())
		window->GetEngine()->SetFocusedWindow(NULL);

	_AnimaEngineWindowPlatformDestroyWindow(window);

	// Unlink window from global linked list
	AnimaEngineWindow_Base** prev = &_windowListHead;

	while (*prev != window)
		(*prev) = ((*prev)->GetNext());

	*prev = window->GetNext();

	delete window;
	window = NULL;
}

void AnimaEngine::DefaultWindowHints(void)
{
	_ANIMA_ENGINE_REQUIRE_INIT();

	memset(&_windowHints, 0, sizeof(_windowHints));

	// The default is OpenGL with minimum version 1.0
	_windowHints._api = ANIMA_ENGINE_OPENGL_API;
	_windowHints._major = 1;
	_windowHints._minor = 0;

	// The default is a focused, visible, resizable window with decorations
	_windowHints._resizable		= true;
	_windowHints._visible		= true;
	_windowHints._decorated		= true;
	_windowHints._focused		= true;
	_windowHints._autoIconify	= true;

	// The default is to select the highest available refresh rate
	_windowHints._refreshRate = ANIMA_ENGINE_DONT_CARE;

	// The default is 24 bits of color, 24 bits of depth and 8 bits of stencil,
	// double buffered
	_windowHints._redBits		= 8;
	_windowHints._greenBits		= 8;
	_windowHints._blueBits		= 8;
	_windowHints._alphaBits		= 8;
	_windowHints._depthBits		= 24;
	_windowHints._stencilBits	= 8;
	_windowHints._doublebuffer	= true;
}

void AnimaEngine::SetWindowHint(int target, int hint)
{
	_ANIMA_ENGINE_REQUIRE_INIT();

	switch (target)
	{
	case ANIMA_ENGINE_RED_BITS:
		_windowHints._redBits = hint;
		break;
	case ANIMA_ENGINE_GREEN_BITS:
		_windowHints._greenBits = hint;
		break;
	case ANIMA_ENGINE_BLUE_BITS:
		_windowHints._blueBits = hint;
		break;
	case ANIMA_ENGINE_ALPHA_BITS:
		_windowHints._alphaBits = hint;
		break;
	case ANIMA_ENGINE_DEPTH_BITS:
		_windowHints._depthBits = hint;
		break;
	case ANIMA_ENGINE_STENCIL_BITS:
		_windowHints._stencilBits = hint;
		break;
	case ANIMA_ENGINE_ACCUM_RED_BITS:
		_windowHints._accumRedBits = hint;
		break;
	case ANIMA_ENGINE_ACCUM_GREEN_BITS:
		_windowHints._accumGreenBits = hint;
		break;
	case ANIMA_ENGINE_ACCUM_BLUE_BITS:
		_windowHints._accumBlueBits = hint;
		break;
	case ANIMA_ENGINE_ACCUM_ALPHA_BITS:
		_windowHints._accumAlphaBits = hint;
		break;
	case ANIMA_ENGINE_AUX_BUFFERS:
		_windowHints._auxBuffers = hint;
		break;
	case ANIMA_ENGINE_REFRESH_RATE:
		_windowHints._refreshRate = hint;
		break;
	case ANIMA_ENGINE_SAMPLES:
		_windowHints._samples = hint;
		break;
	case ANIMA_ENGINE_CLIENT_API:
		_windowHints._api = hint;
		break;
	case ANIMA_ENGINE_CONTEXT_VERSION_MAJOR:
		_windowHints._major = hint;
		break;
	case ANIMA_ENGINE_CONTEXT_VERSION_MINOR:
		_windowHints._minor = hint;
		break;
	case ANIMA_ENGINE_CONTEXT_ROBUSTNESS:
		_windowHints._robustness = hint;
		break;
	case ANIMA_ENGINE_OPENGL_PROFILE:
		_windowHints._profile = hint;
		break;
	case ANIMA_ENGINE_CONTEXT_RELEASE_BEHAVIOR:
		_windowHints._release = hint;
		break;
	default:
		//_glfwInputError(GLFW_INVALID_ENUM, NULL);
		break;
	}
}

void AnimaEngine::SetWindowHint(int target, bool hint)
{
	switch (target)
	{
	case ANIMA_ENGINE_STEREO:
		_windowHints._stereo = hint;
		break;
	case ANIMA_ENGINE_DOUBLEBUFFER:
		_windowHints._doublebuffer = hint;
		break;
	case ANIMA_ENGINE_RESIZABLE:
		_windowHints._resizable = hint;
		break;
	case ANIMA_ENGINE_DECORATED:
		_windowHints._decorated = hint;
		break;
	case ANIMA_ENGINE_FOCUSED:
		_windowHints._focused = hint;
		break;
	case ANIMA_ENGINE_AUTO_ICONIFY:
		_windowHints._autoIconify = hint;
		break;
	case ANIMA_ENGINE_FLOATING:
		_windowHints._floating = hint;
		break;
	case ANIMA_ENGINE_VISIBLE:
		_windowHints._visible = hint;
		break;
	case ANIMA_ENGINE_SRGB_CAPABLE:
		_windowHints._sRGB = hint;
		break;
	case ANIMA_ENGINE_OPENGL_FORWARD_COMPAT:
		_windowHints._forward = hint;
		break;
	case ANIMA_ENGINE_OPENGL_DEBUG_CONTEXT:
		_windowHints._debug = hint;
		break;
	default:
		break;
	}
}

void AnimaEngine::GetVersion(int* major, int* minor, int* rev)
{
	if (major != NULL)
		*major = _ANIMA_ENGINE_VERSION_MAJOR;

	if (minor != NULL)
		*minor = _ANIMA_ENGINE_VERSION_MINOR;

	if (rev != NULL)
		*rev = _ANIMA_ENGINE_VERSION_REVISION;
}

const char* AnimaEngine::GetVersionString()
{
	return _AnimaEngineWindowPlatformGetVersionString();
}


AnimaEngineWindowmonitor* AnimaEngine::GetPrimaryMonitor(void)
{
	int count;
	AnimaEngineWindowmonitor** monitors = (AnimaEngineWindowmonitor**)_AnimaEngineWindowPlatformGetMonitors(&count);

	if (count > 0)
		return monitors[0];
	return nullptr;

}

void AnimaEngine::GetMonitorPos(AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos)
{
}

void AnimaEngine::GetMonitorPhysicalSize(AnimaEngineWindowmonitor* monitor, int* width, int* height)
{
}

const char* AnimaEngine::GetMonitorName(AnimaEngineWindowmonitor* monitor)
{
	return nullptr;
}

bool AnimaEngine::ExtensionSupported(const char* extension)
{
	const GLubyte* extensions;
	AnimaEngineWindow_Base* window;

	_ANIMA_ENGINE_REQUIRE_INIT_OR_RETURN(false);

	window = _AnimaEngineWindowPlatformGetCurrentContext();
	if (!window)
	{
		//_glfwInputError(GLFW_NO_CURRENT_CONTEXT, NULL);
		return false;
	}

	if (!extension || *extension == '\0')
	{
		//_glfwInputError(GLFW_INVALID_VALUE, NULL);
		return false;
	}

	if (window->GetContext()->_major < 3)
	{
		// Check if extension is in the old style OpenGL extensions string
		extensions = glGetString(GL_EXTENSIONS);
		if (!extensions)
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to retrieve extension string");
			return false;
		}

		if (_AnimaEngineWindowStringInExtensionString(extension, extensions))
			return true;
	}
#if defined(_ANIMA_ENGINE_USE_OPENGL)
	else
	{
		int i;
		GLint count;

		// Check if extension is in the modern OpenGL extensions string list

		glGetIntegerv(GL_NUM_EXTENSIONS, &count);

		if (!window->GetGetStringiCallback())
			return false;

		for (i = 0; i < count; i++)
		{
			const char* en = (const char*)window->GetGetStringiCallback()(GL_EXTENSIONS, i);
			if (!en)
			{
				//_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to retrieve extension string %i", i);
				return false;
			}

			if (strcmp(en, extension) == 0)
				return true;
		}
	}
#endif

	// Check if extension is in the platform-specific string
	return _AnimaEngineWindowPlatformExtensionSupported(extension);
}

void AnimaEngine::PollEvents()
{
	_AnimaEngineWindowPlatformPollEvents(this);
}

void AnimaEngine::WaitEvents(void)
{
	if (!_windowListHead)
		return;

	_AnimaEngineWindowPlatformWaitEvents(this);
}

void AnimaEngine::PostEmptyEvent(void)
{
	if (!_windowListHead)
		return;

	_AnimaEngineWindowPlatformPostEmptyEvent(this);
}

void AnimaEngine::SwapInterval(int interval)
{
	_AnimaEngineWindowPlatformSwapInterval(interval);
}

double AnimaEngine::GetTime(void)
{
	return _AnimaEngineWindowPlatformGetTime();
}

void AnimaEngine::SetTime(double time)
{
	_AnimaEngineWindowPlatformSetTime(time);
}

bool AnimaEngine::InitializeGlewExtensions()
{
	return _AnimaEngineWindowInitializeGlewExtensions();
}

void AnimaEngine::MakeContextCurrent(AnimaWindow* window)
{
	_AnimaEngineWindowPlatformMakeContextCurrent(window);
}

AnimaWindow* AnimaEngine::GetCurrentContext(void)
{
	return _AnimaEngineWindowPlatformGetCurrentContext();
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

END_ANIMA_ENGINE_NAMESPACE
