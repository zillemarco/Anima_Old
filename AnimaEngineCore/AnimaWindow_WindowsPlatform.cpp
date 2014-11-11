#include "AnimaWindow_Base.h"

#include <stdlib.h>
#include <malloc.h>

#ifdef __BORLANDC__
#	include <float.h>
#endif

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

static bool initLibraries(void)
{
	//_glfw.win32.winmm.instance = LoadLibraryW(L"winmm.dll");
	//if (!_glfw.win32.winmm.instance)
	//{
	//	//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to load winmm.dll");
	//	return false;
	//}

	//_glfw.win32.winmm.joyGetDevCaps = (JOYGETDEVCAPS_T)GetProcAddress(_glfw.win32.winmm.instance, "joyGetDevCapsW");
	//_glfw.win32.winmm.joyGetPos = (JOYGETPOS_T)GetProcAddress(_glfw.win32.winmm.instance, "joyGetPos");
	//_glfw.win32.winmm.joyGetPosEx = (JOYGETPOSEX_T)GetProcAddress(_glfw.win32.winmm.instance, "joyGetPosEx");
	//_glfw.win32.winmm.timeGetTime = (TIMEGETTIME_T)GetProcAddress(_glfw.win32.winmm.instance, "timeGetTime");

	//if (!_glfw.win32.winmm.joyGetDevCaps || !_glfw.win32.winmm.joyGetPos || !_glfw.win32.winmm.joyGetPosEx || !_glfw.win32.winmm.timeGetTime)
	//{
	//	//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to load winmm functions");
	//	return false;
	//}

	//_glfw.win32.user32.instance = LoadLibraryW(L"user32.dll");
	//if (_glfw.win32.user32.instance)
	//{
	//	_glfw.win32.user32.SetProcessDPIAware = (SETPROCESSDPIAWARE_T)GetProcAddress(_glfw.win32.user32.instance, "SetProcessDPIAware");
	//	_glfw.win32.user32.ChangeWindowMessageFilterEx = (CHANGEWINDOWMESSAGEFILTEREX_T)GetProcAddress(_glfw.win32.user32.instance, "ChangeWindowMessageFilterEx");
	//}

	//_glfw.win32.dwmapi.instance = LoadLibraryW(L"dwmapi.dll");
	//if (_glfw.win32.dwmapi.instance)
	//{
	//	_glfw.win32.dwmapi.DwmIsCompositionEnabled = (DWMISCOMPOSITIONENABLED_T)GetProcAddress(_glfw.win32.dwmapi.instance, "DwmIsCompositionEnabled");
	//}

	return true;
}

static void terminateLibraries(void)
{
	//if (_glfw.win32.winmm.instance)
	//	FreeLibrary(_glfw.win32.winmm.instance);

	//if (_glfw.win32.user32.instance)
	//	FreeLibrary(_glfw.win32.user32.instance);

	//if (_glfw.win32.dwmapi.instance)
	//	FreeLibrary(_glfw.win32.dwmapi.instance);
}

bool _AnimaEngineWindowIsCompositionEnabled(void)
{
	bool enabled;

	if (!_AnimaEngineWindow_DwmIsCompositionEnabled)
		return false;

	if (_AnimaEngineWindow_DwmIsCompositionEnabled(&enabled) != S_OK)
		return false;

	return enabled;
}

WCHAR* _AnimaEngineWindowCreateWideStringFromUTF8(const char* source)
{
	WCHAR* target;
	int length;

	length = MultiByteToWideChar(CP_UTF8, 0, source, -1, NULL, 0);
	if (!length)
		return NULL;

	target = (WCHAR*)calloc(length, sizeof(WCHAR));

	if (!MultiByteToWideChar(CP_UTF8, 0, source, -1, target, length))
	{
		free(target);
		return NULL;
	}

	return target;
}

char* _AnimaEngineWindowCreateUTF8FromWideString(const WCHAR* source)
{
	char* target;
	int length;

	length = WideCharToMultiByte(CP_UTF8, 0, source, -1, NULL, 0, NULL, NULL);
	if (!length)
		return NULL;

	target = (char*)calloc(length, sizeof(char));

	if (!WideCharToMultiByte(CP_UTF8, 0, source, -1, target, length, NULL, NULL))
	{
		free(target);
		return NULL;
	}

	return target;
}


bool _AnimaEngineWindowPlatformInit(void)
{
	// To make SetForegroundWindow work as we want, we need to fiddle
	// with the FOREGROUNDLOCKTIMEOUT system setting (we do this as early
	// as possible in the hope of still being the foreground process)
	SystemParametersInfoW(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &_glfw.win32.foregroundLockTimeout, 0);
	SystemParametersInfoW(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, UIntToPtr(0), SPIF_SENDCHANGE);

	if (!initLibraries())
		return false;

	if (_AnimaEngineWindow_SetProcessDPIAware)
		_AnimaEngineWindow_SetProcessDPIAware();

#ifdef __BORLANDC__
	// With the Borland C++ compiler, we want to disable FPU exceptions
	// (this is recommended for OpenGL applications under Windows)
	_control87(MCW_EM, MCW_EM);
#endif

	if (!_AnimaEngineWindowRegisterWindowClass())
		return false;

	if (!_AnimaEngineWindowInitContextAPI())
		return false;

	_AnimaEngineWindowInitTimer();
	_AnimaEngineWindowInitJoysticks();

	return true;
}

void _AnimaEngineWindowPlatformTerminate(void)
{
	_AnimaEngineWindowUnregisterWindowClass();

	// Restore previous foreground lock timeout system setting
	SystemParametersInfoW(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, UIntToPtr(_glfw.win32.foregroundLockTimeout), SPIF_SENDCHANGE);

	free(_glfw.win32.clipboardString);

	_AnimaEngineWindowTerminateJoysticks();
	_AnimaEngineWindowTerminateContextAPI();
	terminateLibraries();
}

const char* _AnimaEngineWindowPlatformGetVersionString(void)
{
	const char* version = _ANIMA_ENGINE_CORE_VERSION_NUMBER " Win32"
#if defined(_GLFW_WGL)
		" WGL"
#elif defined(_GLFW_EGL)
		" EGL"
#endif
#if defined(__MINGW32__)
		" MinGW"
#elif defined(_MSC_VER)
		" VisualC"
#elif defined(__BORLANDC__)
		" BorlandC"
#endif
#if defined(_GLFW_BUILD_DLL)
		" DLL"
#endif
		;

	return version;
}

END_ANIMA_ENGINE_CORE_NAMESPACE