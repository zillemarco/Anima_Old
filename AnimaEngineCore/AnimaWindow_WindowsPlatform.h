#ifndef _AnimaWindow_WindowsPlatform_h
#define _AnimaWindow_WindowsPlatform_h

#include "AnimaEngineCore.h"

#ifndef NOMINMAX
#	define NOMINMAX
#endif

#ifndef VC_EXTRALEAN
#	define VC_EXTRALEAN
#endif

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif

#undef APIENTRY

#ifndef UNICODE
#	define UNICODE
#endif

#if WINVER < 0x0501
#	undef WINVER
#	define WINVER 0x0501
#endif

#if _WIN32_WINNT < 0x0501
#	undef _WIN32_WINNT
#	define _WIN32_WINNT 0x0501
#endif

#include <windows.h>
#include <mmsystem.h>
#include <dbt.h>

#if defined(_MSC_VER)
#	include <malloc.h>
#	define strdup _strdup
#endif

#ifndef WM_MOUSEHWHEEL
#	define WM_MOUSEHWHEEL 0x020E
#endif

#ifndef WM_DWMCOMPOSITIONCHANGED
#	define WM_DWMCOMPOSITIONCHANGED 0x031E
#endif

#ifndef WM_COPYGLOBALDATA
#	define WM_COPYGLOBALDATA 0x0049
#endif

#ifndef WM_UNICHAR
#	define WM_UNICHAR 0x0109
#endif

#ifndef UNICODE_NOCHAR
#	define UNICODE_NOCHAR 0xFFFF
#endif

#if WINVER < 0x0601
	typedef struct tagCHANGEFILTERSTRUCT {
		DWORD cbSize;
		DWORD ExtStatus;	
	} CHANGEFILTERSTRUCT, *PCHANGEFILTERSTRUCT;

#	ifndef MSGFLT_ALLOW
#		define MSGFLT_ALLOW 1
#	endif
#endif
	
typedef MMRESULT(WINAPI * JOYGETDEVCAPS_T)(UINT, LPJOYCAPS, UINT);
typedef MMRESULT(WINAPI * JOYGETPOS_T)(UINT, LPJOYINFO);
typedef MMRESULT(WINAPI * JOYGETPOSEX_T)(UINT, LPJOYINFOEX);
typedef DWORD(WINAPI * TIMEGETTIME_T)(void);
typedef BOOL(WINAPI * SETPROCESSDPIAWARE_T)(void);
typedef BOOL(WINAPI * CHANGEWINDOWMESSAGEFILTEREX_T)(HWND, UINT, DWORD, PCHANGEFILTERSTRUCT);
typedef HRESULT(WINAPI * DWMISCOMPOSITIONENABLED_T)(BOOL*);

#define _AnimaEngineWindow_joyGetDevCaps AnimaEngine::win32._winmm._joyGetDevCaps
#define _AnimaEngineWindow_joyGetPos AnimaEngine::win32._winmm._joyGetPos
#define _AnimaEngineWindow_joyGetPosEx AnimaEngine::win32._winmm._joyGetPosEx
#define _AnimaEngineWindow_timeGetTime AnimaEngine::win32._winmm._timeGetTime
#define _AnimaEngineWindow_SetProcessDPIAware AnimaEngine::win32._user32._SetProcessDPIAware
#define _AnimaEngineWindow_ChangeWindowMessageFilterEx AnimaEngine::win32._user32._ChangeWindowMessageFilterEx
#define _AnimaEngineWindow_DwmIsCompositionEnabled AnimaEngine::win32._dwmapi._DwmIsCompositionEnabled

#define _ANIMA_ENGINE_CORE_RECREATION_NOT_NEEDED 0
#define _ANIMA_ENGINE_CORE_RECREATION_REQUIRED   1
#define _ANIMA_ENGINE_CORE_RECREATION_IMPOSSIBLE 2

#include "AnimaWindow_WindowsTLS.h"

#if defined(_ANIMA_ENGINE_CORE_WGL)
#	include "AnimaWindow_WGLContext.h"
#elif defined(_GLFW_EGL)
#	define _GLFW_EGL_NATIVE_WINDOW  window->win32.handle
#	define _GLFW_EGL_NATIVE_DISPLAY EGL_DEFAULT_DISPLAY
#	include "egl_context.h"
#else
#	error "No supported context creation API selected"
#endif

#include "AnimaWindow_WindowsJoystick.h"

#define _ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE         _AnimaEngineWindowwindowWin32  win32
#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE _AnimaEngineWindowlibraryWin32 win32
#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE   _AnimaEngineWindowtimeWin32    win32_time
#define _ANIMA_ENGINE_CORE_PLATFORM_MONITOR_STATE        _AnimaEngineWindowmonitorWin32 win32
#define _ANIMA_ENGINE_CORE_PLATFORM_CURSOR_STATE         _AnimaEngineWindowcursorWin32  win32

#define _INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE(class) _AnimaEngineWindowlibraryWin32 class::win32;
#define _INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE(class) _AnimaEngineWindowtimeWin32 class::win32_time;

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowwindowWin32
{
	HWND	_handle;
	DWORD	_dwStyle;
	DWORD	_dwExStyle;
	bool	_cursorCentered;
	bool	_cursorInside;
	bool	_cursorHidden;
	int		_oldCursorX, _oldCursorY;

} _AnimaEngineWindowwindowWin32;

typedef struct _AnimaEngineWindowlibraryWin32
{
	DWORD	_foregroundLockTimeout;
	char*	_clipboardString;

	struct {
		HINSTANCE		_instance;
		JOYGETDEVCAPS_T	_joyGetDevCaps;
		JOYGETPOS_T		_joyGetPos;
		JOYGETPOSEX_T	_joyGetPosEx;
		TIMEGETTIME_T	_timeGetTime;
	} _winmm;

	struct {
		HINSTANCE						_instance;
		SETPROCESSDPIAWARE_T			_SetProcessDPIAware;
		CHANGEWINDOWMESSAGEFILTEREX_T	_ChangeWindowMessageFilterEx;
	} _user32;

	struct {
		HINSTANCE					_instance;
		DWMISCOMPOSITIONENABLED_T	_DwmIsCompositionEnabled;
	} _dwmapi;

} _AnimaEngineWindowlibraryWin32;

typedef struct _AnimaEngineWindowmonitorWin32
{
	WCHAR	_adapterName[32];
	WCHAR	_displayName[32];
	char	_publicAdapterName[64];
	char	_publicDisplayName[64];
	bool	_modeChanged;

} _AnimaEngineWindowmonitorWin32;

typedef struct _AnimaEngineWindowcursorWin32
{
	HCURSOR _handle;

} _AnimaEngineWindowcursorWin32;

typedef struct _AnimaEngineWindowtimeWin32
{
	bool				_hasPC;
	double				_resolution;
	unsigned __int64	_base;

} _AnimaEngineWindowtimeWin32;


extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowRegisterWindowClass(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowUnregisterWindowClass(void);

extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowIsCompositionEnabled(void);

extern "C" ANIMA_ENGINE_CORE_EXPORT WCHAR* _AnimaEngineWindowCreateWideStringFromUTF8(const char* source);
extern "C" ANIMA_ENGINE_CORE_EXPORT char* _AnimaEngineWindowCreateUTF8FromWideString(const WCHAR* source);

extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowInitTimer(void);

extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowSetVideoMode(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowvidmode* desired);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowRestoreVideoMode(_AnimaEngineWindowmonitor* monitor);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif