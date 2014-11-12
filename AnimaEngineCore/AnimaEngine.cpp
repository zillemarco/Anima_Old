#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

_INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE(AnimaEngine)
_INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE(AnimaEngine)

bool								AnimaEngine::_animaEngineInitialized = false;
_AnimaEngineWindowmonitor**			AnimaEngine::_monitors = NULL;
int									AnimaEngine::_monitorCount = 0;
_AnimaEngineWindowMonitorCallbacks	AnimaEngine::_callbacks;
int									AnimaEngine::_animaEngineCount = 0;
_AnimaEngineWindowHints				AnimaEngine::_windowHints;

AnimaEngine::AnimaEngine()
{
	_windowListHead = NULL;
	_focusedWindow = NULL;

	_animaEngineCount++;
}

AnimaEngine::~AnimaEngine()
{
	_animaEngineCount--;
	Terminate();
}

bool AnimaEngine::Initialize()
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

void AnimaEngine::Terminate()
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

	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

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
	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

	memset(&_windowHints, 0, sizeof(_windowHints));

	// The default is OpenGL with minimum version 1.0
	_windowHints._api = ANIMA_ENGINE_CORE_OPENGL_API;
	_windowHints._major = 1;
	_windowHints._minor = 0;

	// The default is a focused, visible, resizable window with decorations
	_windowHints._resizable		= true;
	_windowHints._visible		= true;
	_windowHints._decorated		= true;
	_windowHints._focused		= true;
	_windowHints._autoIconify	= true;

	// The default is to select the highest available refresh rate
	_windowHints._refreshRate = ANIMA_ENGINE_CORE_DONT_CARE;

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
	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

	switch (target)
	{
	case ANIMA_ENGINE_CORE_RED_BITS:
		_windowHints._redBits = hint;
		break;
	case ANIMA_ENGINE_CORE_GREEN_BITS:
		_windowHints._greenBits = hint;
		break;
	case ANIMA_ENGINE_CORE_BLUE_BITS:
		_windowHints._blueBits = hint;
		break;
	case ANIMA_ENGINE_CORE_ALPHA_BITS:
		_windowHints._alphaBits = hint;
		break;
	case ANIMA_ENGINE_CORE_DEPTH_BITS:
		_windowHints._depthBits = hint;
		break;
	case ANIMA_ENGINE_CORE_STENCIL_BITS:
		_windowHints._stencilBits = hint;
		break;
	case ANIMA_ENGINE_CORE_ACCUM_RED_BITS:
		_windowHints._accumRedBits = hint;
		break;
	case ANIMA_ENGINE_CORE_ACCUM_GREEN_BITS:
		_windowHints._accumGreenBits = hint;
		break;
	case ANIMA_ENGINE_CORE_ACCUM_BLUE_BITS:
		_windowHints._accumBlueBits = hint;
		break;
	case ANIMA_ENGINE_CORE_ACCUM_ALPHA_BITS:
		_windowHints._accumAlphaBits = hint;
		break;
	case ANIMA_ENGINE_CORE_AUX_BUFFERS:
		_windowHints._auxBuffers = hint;
		break;
	case ANIMA_ENGINE_CORE_REFRESH_RATE:
		_windowHints._refreshRate = hint;
		break;
	case ANIMA_ENGINE_CORE_SAMPLES:
		_windowHints._samples = hint;
		break;
	case ANIMA_ENGINE_CORE_CLIENT_API:
		_windowHints._api = hint;
		break;
	case ANIMA_ENGINE_CORE_CONTEXT_VERSION_MAJOR:
		_windowHints._major = hint;
		break;
	case ANIMA_ENGINE_CORE_CONTEXT_VERSION_MINOR:
		_windowHints._minor = hint;
		break;
	case ANIMA_ENGINE_CORE_CONTEXT_ROBUSTNESS:
		_windowHints._robustness = hint;
		break;
	case ANIMA_ENGINE_CORE_OPENGL_PROFILE:
		_windowHints._profile = hint;
		break;
	case ANIMA_ENGINE_CORE_CONTEXT_RELEASE_BEHAVIOR:
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
	case ANIMA_ENGINE_CORE_STEREO:
		_windowHints._stereo = hint;
		break;
	case ANIMA_ENGINE_CORE_DOUBLEBUFFER:
		_windowHints._doublebuffer = hint;
		break;
	case ANIMA_ENGINE_CORE_RESIZABLE:
		_windowHints._resizable = hint;
		break;
	case ANIMA_ENGINE_CORE_DECORATED:
		_windowHints._decorated = hint;
		break;
	case ANIMA_ENGINE_CORE_FOCUSED:
		_windowHints._focused = hint;
		break;
	case ANIMA_ENGINE_CORE_AUTO_ICONIFY:
		_windowHints._autoIconify = hint;
		break;
	case ANIMA_ENGINE_CORE_FLOATING:
		_windowHints._floating = hint;
		break;
	case ANIMA_ENGINE_CORE_VISIBLE:
		_windowHints._visible = hint;
		break;
	case ANIMA_ENGINE_CORE_SRGB_CAPABLE:
		_windowHints._sRGB = hint;
		break;
	case ANIMA_ENGINE_CORE_OPENGL_FORWARD_COMPAT:
		_windowHints._forward = hint;
		break;
	case ANIMA_ENGINE_CORE_OPENGL_DEBUG_CONTEXT:
		_windowHints._debug = hint;
		break;
	default:
		break;
	}
}

void AnimaEngine::GetVersion(int* major, int* minor, int* rev)
{
	if (major != NULL)
		*major = _ANIMA_ENGINE_CORE_VERSION_MAJOR;

	if (minor != NULL)
		*minor = _ANIMA_ENGINE_CORE_VERSION_MINOR;

	if (rev != NULL)
		*rev = _ANIMA_ENGINE_CORE_VERSION_REVISION;
}

const char* AnimaEngine::GetVersionString()
{
	return _AnimaEngineWindowPlatformGetVersionString();
}

bool AnimaEngine::ExtensionSupported(const char* extension)
{
	const GLubyte* extensions;
	AnimaEngineWindow_Base* window;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(false);

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
#if defined(_ANIMA_ENGINE_CORE_USE_OPENGL)
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

END_ANIMA_ENGINE_CORE_NAMESPACE