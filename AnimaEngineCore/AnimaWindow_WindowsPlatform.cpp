#include "AnimaWindow_Base.h"
#include "AnimaEngine.h"

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <windowsx.h>
#include <Windows.h>
#include <shellapi.h>


#define _ANIMA_ENGINE_CORE_KEY_INVALID -2
#define _ANIMA_ENGINE_CORE_WNDCLASSNAME L"AnimaEngineWindow"

#ifndef EDS_ROTATEDMODE
#	define EDS_ROTATEDMODE 0x00000004
#endif

#ifndef DISPLAY_DEVICE_ACTIVE
#	define DISPLAY_DEVICE_ACTIVE 0x00000001
#endif

#ifdef __BORLANDC__
#	include <float.h>
#endif

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

static bool initLibraries(void)
{
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance = LoadLibraryW(L"winmm.dll");
	if (!AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to load winmm.dll");
		return false;
	}

	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._joyGetDevCaps = (JOYGETDEVCAPS_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance, "joyGetDevCapsW");
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._joyGetPos = (JOYGETPOS_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance, "joyGetPos");
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._joyGetPosEx = (JOYGETPOSEX_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance, "joyGetPosEx");
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._timeGetTime = (TIMEGETTIME_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance, "timeGetTime");

	if (!AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._joyGetDevCaps || !AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._joyGetPos || !AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._joyGetPosEx || !AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._timeGetTime)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to load winmm functions");
		return false;
	}

	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._instance = LoadLibraryW(L"user32.dll");
	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._instance)
	{
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._SetProcessDPIAware = (SETPROCESSDPIAWARE_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._instance, "SetProcessDPIAware");
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._ChangeWindowMessageFilterEx = (CHANGEWINDOWMESSAGEFILTEREX_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._instance, "ChangeWindowMessageFilterEx");
	}

	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_dwmapi._instance = LoadLibraryW(L"dwmapi.dll");
	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_dwmapi._instance)
	{
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_dwmapi._DwmIsCompositionEnabled = (DWMISCOMPOSITIONENABLED_T)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_dwmapi._instance, "DwmIsCompositionEnabled");
	}

	return true;
}

static void terminateLibraries(void)
{
	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance)
		FreeLibrary(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_winmm._instance);

	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._instance)
		FreeLibrary(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_user32._instance);

	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_dwmapi._instance)
		FreeLibrary(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_dwmapi._instance);
}

bool _AnimaEngineWindowIsCompositionEnabled(void)
{
	BOOL enabled;

	if (!_AnimaEngineWindow_DwmIsCompositionEnabled)
		return false;

	if (_AnimaEngineWindow_DwmIsCompositionEnabled(&enabled) != S_OK)
		return false;

	if (enabled == TRUE)
		return true;
	return false;
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
	SystemParametersInfoW(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_foregroundLockTimeout, 0);
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
	SystemParametersInfoW(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, UIntToPtr(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_foregroundLockTimeout), SPIF_SENDCHANGE);

	free(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString);

	_AnimaEngineWindowTerminateJoysticks();
	_AnimaEngineWindowTerminateContextAPI();
	terminateLibraries();
}

const char* _AnimaEngineWindowPlatformGetVersionString(void)
{
	const char* version = _ANIMA_ENGINE_CORE_VERSION_NUMBER " Win32"
#if defined(_ANIMA_ENGINE_CORE_WGL)
		" WGL"
#endif
#if defined(__MINGW32__)
		" MinGW"
#elif defined(_MSC_VER)
		" VisualC"
#elif defined(__BORLANDC__)
		" BorlandC"
#endif
#if defined(ANIMA_ENGINE_CORE_EXPORT_ENABLED)
		" DLL"
#endif
		;

	return version;
}

static void updateClipRect(AnimaEngineWindow_Base* window)
{
	RECT clipRect;
	GetClientRect(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, &clipRect);
	ClientToScreen(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, (POINT*)&clipRect.left);
	ClientToScreen(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, (POINT*)&clipRect.right);
	ClipCursor(&clipRect);
}

static void hideCursor(AnimaEngineWindow_Base* window)
{
	POINT pos;

	ReleaseCapture();
	ClipCursor(NULL);

	if (window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorHidden)
	{
		ShowCursor(TRUE);
		window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorHidden = false;
	}

	if (GetCursorPos(&pos))
	{
		if (WindowFromPoint(pos) == window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle)
			SetCursor(NULL);
	}
}

// Disable the mouse cursor
//
static void disableCursor(AnimaEngineWindow_Base* window)
{
	if (!window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorHidden)
	{
		ShowCursor(FALSE);
		window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorHidden = true;
	}

	updateClipRect(window);
	SetCapture(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle);
}

// Restores the mouse cursor
//
static void restoreCursor(AnimaEngineWindow_Base* window)
{
	POINT pos;

	ReleaseCapture();
	ClipCursor(NULL);

	if (window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorHidden)
	{
		ShowCursor(TRUE);
		window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorHidden = false;
	}

	if (GetCursorPos(&pos))
	{
		if (WindowFromPoint(pos) == window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle)
		{
			if (window->GetCursor())
				SetCursor(window->GetCursor()->win32._handle);
			else
				SetCursor(LoadCursorW(NULL, IDC_ARROW));
		}
	}
}

// Retrieves and translates modifier keys
//
static int getKeyMods(void)
{
	int mods = 0;

	if (GetKeyState(VK_SHIFT) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_SHIFT;
	if (GetKeyState(VK_CONTROL) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_CONTROL;
	if (GetKeyState(VK_MENU) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_ALT;
	if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_SUPER;

	return mods;
}

// Retrieves and translates modifier keys
//
static int getAsyncKeyMods(void)
{
	int mods = 0;

	if (GetAsyncKeyState(VK_SHIFT) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_SHIFT;
	if (GetAsyncKeyState(VK_CONTROL) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_CONTROL;
	if (GetAsyncKeyState(VK_MENU) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_ALT;
	if ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & (1 << 31))
		mods |= ANIMA_ENGINE_CORE_MOD_SUPER;

	return mods;
}

// Translates a Windows key to the corresponding ANIMA_ENGINE_CORE key
//
static int translateKey(WPARAM wParam, LPARAM lParam)
{
	// Check for numeric keypad keys
	// NOTE: This way we always force "NumLock = ON", which is intentional since
	//       the returned key code should correspond to a physical location.
	if ((HIWORD(lParam) & 0x100) == 0)
	{
		switch (MapVirtualKey(HIWORD(lParam) & 0xFF, 1))
		{
		case VK_INSERT:   return ANIMA_ENGINE_CORE_KEY_KP_0;
		case VK_END:      return ANIMA_ENGINE_CORE_KEY_KP_1;
		case VK_DOWN:     return ANIMA_ENGINE_CORE_KEY_KP_2;
		case VK_NEXT:     return ANIMA_ENGINE_CORE_KEY_KP_3;
		case VK_LEFT:     return ANIMA_ENGINE_CORE_KEY_KP_4;
		case VK_CLEAR:    return ANIMA_ENGINE_CORE_KEY_KP_5;
		case VK_RIGHT:    return ANIMA_ENGINE_CORE_KEY_KP_6;
		case VK_HOME:     return ANIMA_ENGINE_CORE_KEY_KP_7;
		case VK_UP:       return ANIMA_ENGINE_CORE_KEY_KP_8;
		case VK_PRIOR:    return ANIMA_ENGINE_CORE_KEY_KP_9;
		case VK_DIVIDE:   return ANIMA_ENGINE_CORE_KEY_KP_DIVIDE;
		case VK_MULTIPLY: return ANIMA_ENGINE_CORE_KEY_KP_MULTIPLY;
		case VK_SUBTRACT: return ANIMA_ENGINE_CORE_KEY_KP_SUBTRACT;
		case VK_ADD:      return ANIMA_ENGINE_CORE_KEY_KP_ADD;
		case VK_DELETE:   return ANIMA_ENGINE_CORE_KEY_KP_DECIMAL;
		default:          break;
		}
	}

	switch (HIWORD(lParam) & 0xFF)
	{
		// handle printable chars except space in a language independent way,
		// using scancodes rather than virtual keys
		// as virtual keys are language dependent.
		// Printable keys are mapped according to US layout.

		// Row 0:
	case 0x29:             return ANIMA_ENGINE_CORE_KEY_GRAVE_ACCENT;
	case 0x02:             return ANIMA_ENGINE_CORE_KEY_1;
	case 0x03:             return ANIMA_ENGINE_CORE_KEY_2;
	case 0x04:             return ANIMA_ENGINE_CORE_KEY_3;
	case 0x05:             return ANIMA_ENGINE_CORE_KEY_4;
	case 0x06:             return ANIMA_ENGINE_CORE_KEY_5;
	case 0x07:             return ANIMA_ENGINE_CORE_KEY_6;
	case 0x08:             return ANIMA_ENGINE_CORE_KEY_7;
	case 0x09:             return ANIMA_ENGINE_CORE_KEY_8;
	case 0x0A:             return ANIMA_ENGINE_CORE_KEY_9;
	case 0x0B:             return ANIMA_ENGINE_CORE_KEY_0;
	case 0x0C:             return ANIMA_ENGINE_CORE_KEY_MINUS;
	case 0x0D:             return ANIMA_ENGINE_CORE_KEY_EQUAL;

		// Row 1:
	case 0x10:             return ANIMA_ENGINE_CORE_KEY_Q;
	case 0x11:             return ANIMA_ENGINE_CORE_KEY_W;
	case 0x12:             return ANIMA_ENGINE_CORE_KEY_E;
	case 0x13:             return ANIMA_ENGINE_CORE_KEY_R;
	case 0x14:             return ANIMA_ENGINE_CORE_KEY_T;
	case 0x15:             return ANIMA_ENGINE_CORE_KEY_Y;
	case 0x16:             return ANIMA_ENGINE_CORE_KEY_U;
	case 0x17:             return ANIMA_ENGINE_CORE_KEY_I;
	case 0x18:             return ANIMA_ENGINE_CORE_KEY_O;
	case 0x19:             return ANIMA_ENGINE_CORE_KEY_P;
	case 0x1A:             return ANIMA_ENGINE_CORE_KEY_LEFT_BRACKET;
	case 0x1B:             return ANIMA_ENGINE_CORE_KEY_RIGHT_BRACKET;
		// We do not map 0x2B as this is only on US - use vKeys for this to prevent confusion with 0x56

		// Row 2:
	case 0x1E:             return ANIMA_ENGINE_CORE_KEY_A;
	case 0x1F:             return ANIMA_ENGINE_CORE_KEY_S;
	case 0x20:             return ANIMA_ENGINE_CORE_KEY_D;
	case 0x21:             return ANIMA_ENGINE_CORE_KEY_F;
	case 0x22:             return ANIMA_ENGINE_CORE_KEY_G;
	case 0x23:             return ANIMA_ENGINE_CORE_KEY_H;
	case 0x24:             return ANIMA_ENGINE_CORE_KEY_J;
	case 0x25:             return ANIMA_ENGINE_CORE_KEY_K;
	case 0x26:             return ANIMA_ENGINE_CORE_KEY_L;
	case 0x27:             return ANIMA_ENGINE_CORE_KEY_SEMICOLON;
	case 0x28:             return ANIMA_ENGINE_CORE_KEY_APOSTROPHE;

		// Row 3:
	case 0x2C:             return ANIMA_ENGINE_CORE_KEY_Z;
	case 0x2D:             return ANIMA_ENGINE_CORE_KEY_X;
	case 0x2E:             return ANIMA_ENGINE_CORE_KEY_C;
	case 0x2F:             return ANIMA_ENGINE_CORE_KEY_V;
	case 0x30:             return ANIMA_ENGINE_CORE_KEY_B;
	case 0x31:             return ANIMA_ENGINE_CORE_KEY_N;
	case 0x32:             return ANIMA_ENGINE_CORE_KEY_M;
	case 0x33:             return ANIMA_ENGINE_CORE_KEY_COMMA;
	case 0x34:             return ANIMA_ENGINE_CORE_KEY_PERIOD;
	case 0x35:             return ANIMA_ENGINE_CORE_KEY_SLASH;
	default:               break;
	}

	// Check which key was pressed or released
	switch (wParam)
	{
		// The SHIFT keys require special handling
	case VK_SHIFT:
	{
		// Compare scan code for this key with that of VK_RSHIFT in
		// order to determine which shift key was pressed (left or
		// right)
		const DWORD scancode = MapVirtualKey(VK_RSHIFT, 0);
		if ((DWORD)((lParam & 0x01ff0000) >> 16) == scancode)
			return ANIMA_ENGINE_CORE_KEY_RIGHT_SHIFT;

		return ANIMA_ENGINE_CORE_KEY_LEFT_SHIFT;
	}

		// The CTRL keys require special handling
	case VK_CONTROL:
	{
		MSG next;
		DWORD time;

		// Is this an extended key (i.e. right key)?
		if (lParam & 0x01000000)
			return ANIMA_ENGINE_CORE_KEY_RIGHT_CONTROL;

		// Here is a trick: "Alt Gr" sends LCTRL, then RALT. We only
		// want the RALT message, so we try to see if the next message
		// is a RALT message. In that case, this is a false LCTRL!
		time = GetMessageTime();

		if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
		{
			if (next.message == WM_KEYDOWN || next.message == WM_SYSKEYDOWN || next.message == WM_KEYUP || next.message == WM_SYSKEYUP)
			{
				if (next.wParam == VK_MENU && (next.lParam & 0x01000000) && next.time == time)
				{
					// Next message is a RALT down message, which
					// means that this is not a proper LCTRL message
					return _ANIMA_ENGINE_CORE_KEY_INVALID;
				}
			}
		}

		return ANIMA_ENGINE_CORE_KEY_LEFT_CONTROL;
	}

		// The ALT keys require special handling
	case VK_MENU:
	{
		// Is this an extended key (i.e. right key)?
		if (lParam & 0x01000000)
			return ANIMA_ENGINE_CORE_KEY_RIGHT_ALT;

		return ANIMA_ENGINE_CORE_KEY_LEFT_ALT;
	}

		// The ENTER keys require special handling
	case VK_RETURN:
	{
		// Is this an extended key (i.e. right key)?
		if (lParam & 0x01000000)
			return ANIMA_ENGINE_CORE_KEY_KP_ENTER;

		return ANIMA_ENGINE_CORE_KEY_ENTER;
	}

		// Funcion keys (non-printable keys)
	case VK_ESCAPE:        return ANIMA_ENGINE_CORE_KEY_ESCAPE;
	case VK_TAB:           return ANIMA_ENGINE_CORE_KEY_TAB;
	case VK_BACK:          return ANIMA_ENGINE_CORE_KEY_BACKSPACE;
	case VK_HOME:          return ANIMA_ENGINE_CORE_KEY_HOME;
	case VK_END:           return ANIMA_ENGINE_CORE_KEY_END;
	case VK_PRIOR:         return ANIMA_ENGINE_CORE_KEY_PAGE_UP;
	case VK_NEXT:          return ANIMA_ENGINE_CORE_KEY_PAGE_DOWN;
	case VK_INSERT:        return ANIMA_ENGINE_CORE_KEY_INSERT;
	case VK_DELETE:        return ANIMA_ENGINE_CORE_KEY_DELETE;
	case VK_LEFT:          return ANIMA_ENGINE_CORE_KEY_LEFT;
	case VK_UP:            return ANIMA_ENGINE_CORE_KEY_UP;
	case VK_RIGHT:         return ANIMA_ENGINE_CORE_KEY_RIGHT;
	case VK_DOWN:          return ANIMA_ENGINE_CORE_KEY_DOWN;
	case VK_F1:            return ANIMA_ENGINE_CORE_KEY_F1;
	case VK_F2:            return ANIMA_ENGINE_CORE_KEY_F2;
	case VK_F3:            return ANIMA_ENGINE_CORE_KEY_F3;
	case VK_F4:            return ANIMA_ENGINE_CORE_KEY_F4;
	case VK_F5:            return ANIMA_ENGINE_CORE_KEY_F5;
	case VK_F6:            return ANIMA_ENGINE_CORE_KEY_F6;
	case VK_F7:            return ANIMA_ENGINE_CORE_KEY_F7;
	case VK_F8:            return ANIMA_ENGINE_CORE_KEY_F8;
	case VK_F9:            return ANIMA_ENGINE_CORE_KEY_F9;
	case VK_F10:           return ANIMA_ENGINE_CORE_KEY_F10;
	case VK_F11:           return ANIMA_ENGINE_CORE_KEY_F11;
	case VK_F12:           return ANIMA_ENGINE_CORE_KEY_F12;
	case VK_F13:           return ANIMA_ENGINE_CORE_KEY_F13;
	case VK_F14:           return ANIMA_ENGINE_CORE_KEY_F14;
	case VK_F15:           return ANIMA_ENGINE_CORE_KEY_F15;
	case VK_F16:           return ANIMA_ENGINE_CORE_KEY_F16;
	case VK_F17:           return ANIMA_ENGINE_CORE_KEY_F17;
	case VK_F18:           return ANIMA_ENGINE_CORE_KEY_F18;
	case VK_F19:           return ANIMA_ENGINE_CORE_KEY_F19;
	case VK_F20:           return ANIMA_ENGINE_CORE_KEY_F20;
	case VK_F21:           return ANIMA_ENGINE_CORE_KEY_F21;
	case VK_F22:           return ANIMA_ENGINE_CORE_KEY_F22;
	case VK_F23:           return ANIMA_ENGINE_CORE_KEY_F23;
	case VK_F24:           return ANIMA_ENGINE_CORE_KEY_F24;
	case VK_NUMLOCK:       return ANIMA_ENGINE_CORE_KEY_NUM_LOCK;
	case VK_CAPITAL:       return ANIMA_ENGINE_CORE_KEY_CAPS_LOCK;
	case VK_SNAPSHOT:      return ANIMA_ENGINE_CORE_KEY_PRINT_SCREEN;
	case VK_SCROLL:        return ANIMA_ENGINE_CORE_KEY_SCROLL_LOCK;
	case VK_PAUSE:         return ANIMA_ENGINE_CORE_KEY_PAUSE;
	case VK_LWIN:          return ANIMA_ENGINE_CORE_KEY_LEFT_SUPER;
	case VK_RWIN:          return ANIMA_ENGINE_CORE_KEY_RIGHT_SUPER;
	case VK_APPS:          return ANIMA_ENGINE_CORE_KEY_MENU;

		// Numeric keypad
	case VK_NUMPAD0:       return ANIMA_ENGINE_CORE_KEY_KP_0;
	case VK_NUMPAD1:       return ANIMA_ENGINE_CORE_KEY_KP_1;
	case VK_NUMPAD2:       return ANIMA_ENGINE_CORE_KEY_KP_2;
	case VK_NUMPAD3:       return ANIMA_ENGINE_CORE_KEY_KP_3;
	case VK_NUMPAD4:       return ANIMA_ENGINE_CORE_KEY_KP_4;
	case VK_NUMPAD5:       return ANIMA_ENGINE_CORE_KEY_KP_5;
	case VK_NUMPAD6:       return ANIMA_ENGINE_CORE_KEY_KP_6;
	case VK_NUMPAD7:       return ANIMA_ENGINE_CORE_KEY_KP_7;
	case VK_NUMPAD8:       return ANIMA_ENGINE_CORE_KEY_KP_8;
	case VK_NUMPAD9:       return ANIMA_ENGINE_CORE_KEY_KP_9;
	case VK_DIVIDE:        return ANIMA_ENGINE_CORE_KEY_KP_DIVIDE;
	case VK_MULTIPLY:      return ANIMA_ENGINE_CORE_KEY_KP_MULTIPLY;
	case VK_SUBTRACT:      return ANIMA_ENGINE_CORE_KEY_KP_SUBTRACT;
	case VK_ADD:           return ANIMA_ENGINE_CORE_KEY_KP_ADD;
	case VK_DECIMAL:       return ANIMA_ENGINE_CORE_KEY_KP_DECIMAL;

		// Printable keys are mapped according to US layout
	case VK_SPACE:         return ANIMA_ENGINE_CORE_KEY_SPACE;
	case 0xDC:             return ANIMA_ENGINE_CORE_KEY_BACKSLASH;
	case 0xDF:             return ANIMA_ENGINE_CORE_KEY_WORLD_1;
	case 0xE2:             return ANIMA_ENGINE_CORE_KEY_WORLD_2;
	default:               break;
	}

	// No matching translation was found
	return ANIMA_ENGINE_CORE_KEY_UNKNOWN;
}

static bool enterFullscreenMode(AnimaEngineWindow_Base* window)
{
	AnimaEngineWindowvidmode mode;
	bool status;
	int xpos, ypos;

	status = _AnimaEngineWindowSetVideoMode(window->GetMonitor(), window->GetVideoMode());

	_AnimaEngineWindowPlatformGetVideoMode(window->GetMonitor(), &mode);
	_AnimaEngineWindowPlatformGetMonitorPos(window->GetMonitor(), &xpos, &ypos);

	SetWindowPos(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, HWND_TOPMOST, xpos, ypos, mode._width, mode._height, SWP_NOCOPYBITS);

	return status;
}

static void leaveFullscreenMode(AnimaEngineWindow_Base* window)
{
	_AnimaEngineWindowRestoreVideoMode(window->GetMonitor());
}

static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	AnimaEngineWindow_Base* window = (AnimaEngineWindow_Base*)GetWindowLongPtrW(hWnd, 0);

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		CREATESTRUCTW* cs = (CREATESTRUCTW*)lParam;
		SetWindowLongPtrW(hWnd, 0, (LONG_PTR)cs->lpCreateParams);
		break;
	}

	case WM_ACTIVATE:
	{
		// Window was (de)focused and/or (de)iconified
		bool focused = LOWORD(wParam) != WA_INACTIVE;
		bool iconified = HIWORD(wParam) ? true : false;

		if (focused && iconified)
		{
			if (window->IsIconified() && window->GetEngine()->GetFocusedWindow() != window)
			{
				// This is a workaround for window restoration using the
				// Win+D hot key leading to windows being told they're
				// focused and iconified and then never told they're
				// restored
				iconified = false;
			}
			else
			{
				// This is a workaround for window iconification using the
				// taskbar leading to windows being told they're focused and
				// iconified and then never told they're defocused
				focused = false;
			}
		}

		if (!focused && window->GetEngine()->GetFocusedWindow() == window)
		{
			// The window was defocused (or iconified, see above)

			if (window->GetCursorMode() != ANIMA_ENGINE_CORE_CURSOR_NORMAL)
				restoreCursor(window);

			if (window->GetMonitor() && window->IsAutoIconifiy())
			{
				if (!iconified)
				{
					// Iconify the (on top, borderless, oddly positioned)
					// window or the user will be annoyed
					_AnimaEngineWindowPlatformIconifyWindow(window);
				}

				leaveFullscreenMode(window);
			}
		}
		else if (focused && window->GetEngine()->GetFocusedWindow() != window)
		{
			// The window was focused
			if (window->GetCursorMode() != ANIMA_ENGINE_CORE_CURSOR_NORMAL)
				_AnimaEngineWindowPlatformApplyCursorMode(window);

			if (window->GetMonitor() && window->IsAutoIconifiy())
				enterFullscreenMode(window);
		}

		if (window->GetFocusCallback())
			window->GetFocusCallback()(window, focused);

		if (window->GetIconifyCallback())
			window->GetIconifyCallback()(window, iconified);

		return 0;
	}

	case WM_ACTIVATEAPP:
	{
		if (!wParam && IsIconic(hWnd))
		{
			// This is a workaround for full screen windows losing focus
			// through Alt+Tab leading to windows being told they're
			// unfocused and restored and then never told they're iconified
			if (window->GetIconifyCallback())
				window->GetIconifyCallback()(window, true);
		}

		return 0;
	}

	case WM_SHOWWINDOW:
	{
		if (window->GetVisibilityCallback())
			window->GetVisibilityCallback()(window, wParam ? true : false);
		break;
	}

	case WM_SYSCOMMAND:
	{
		switch (wParam & 0xfff0)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
		{
			if (window->GetMonitor())
			{
				// We are running in fullscreen mode, so disallow
				// screen saver and screen blanking
				return 0;
			}
			else
				break;
		}

			// User trying to access application menu using ALT?
		case SC_KEYMENU:
			return 0;
		}
		break;
	}

	case WM_CLOSE:
	{
		if (window->GetCloseRequestCallback())
			window->GetCloseRequestCallback()(window);
		return 0;
	}

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		const int scancode = (lParam >> 16) & 0x1ff;
		const int key = translateKey(wParam, lParam);
		if (key == _ANIMA_ENGINE_CORE_KEY_INVALID)
			break;

		if (window->GetKeyCallback())
			window->GetKeyCallback()(window, key, scancode, ANIMA_ENGINE_CORE_PRESS, getKeyMods());

		break;
	}

	case WM_CHAR:
	{
		if (window->GetCharCallback())
			window->GetCharCallback()(window, (unsigned int)wParam, getKeyMods(), true);
		return 0;
	}

	case WM_SYSCHAR:
	{
		if (window->GetCharCallback())
			window->GetCharCallback()(window, (unsigned int)wParam, getKeyMods(), false);
		return 0;
	}

	case WM_UNICHAR:
	{
		// This message is not sent by Windows, but is sent by some
		// third-party input method engines

		if (wParam == UNICODE_NOCHAR)
		{
			// Returning TRUE here announces support for this message
			return TRUE;
		}

		if (window->GetCharCallback())
			window->GetCharCallback()(window, (unsigned int)wParam, getKeyMods(), true);

		return FALSE;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		const int mods = getKeyMods();
		const int scancode = (lParam >> 16) & 0x1ff;
		const int key = translateKey(wParam, lParam);
		if (key == _ANIMA_ENGINE_CORE_KEY_INVALID)
			break;

		if (wParam == VK_SHIFT)
		{
			// Release both Shift keys on Shift up event, as only one event
			// is sent even if both keys are released
			if (window->GetKeyCallback())
			{
				window->GetKeyCallback()(window, ANIMA_ENGINE_CORE_KEY_LEFT_SHIFT, scancode, ANIMA_ENGINE_CORE_RELEASE, mods);
				window->GetKeyCallback()(window, ANIMA_ENGINE_CORE_KEY_RIGHT_SHIFT, scancode, ANIMA_ENGINE_CORE_RELEASE, mods);
			}
		}
		else if (wParam == VK_SNAPSHOT)
		{
			// Key down is not reported for the print screen key
			if (window->GetKeyCallback())
			{
				window->GetKeyCallback()(window, key, scancode, ANIMA_ENGINE_CORE_PRESS, mods);
				window->GetKeyCallback()(window, key, scancode, ANIMA_ENGINE_CORE_RELEASE, mods);
			}
		}
		else
		{
			if (window->GetKeyCallback())
				window->GetKeyCallback()(window, key, scancode, ANIMA_ENGINE_CORE_RELEASE, mods);
		}

		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
	{
		const int mods = getKeyMods();

		SetCapture(hWnd);

		if (uMsg == WM_LBUTTONDOWN)
		{
			if (window->GetMouseClickCallback())
				window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_LEFT, ANIMA_ENGINE_CORE_PRESS, mods);
		}
		else if (uMsg == WM_RBUTTONDOWN)
		{
			if (window->GetMouseClickCallback())
				window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_RIGHT, ANIMA_ENGINE_CORE_PRESS, mods);
		}
		else if (uMsg == WM_MBUTTONDOWN)
		{
			if (window->GetMouseClickCallback())
				window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_MIDDLE, ANIMA_ENGINE_CORE_PRESS, mods);
		}
		else
		{
			if (HIWORD(wParam) == XBUTTON1)
			{
				if (window->GetMouseClickCallback())
					window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_4, ANIMA_ENGINE_CORE_PRESS, mods);
			}
			else if (HIWORD(wParam) == XBUTTON2)
			{
				if (window->GetMouseClickCallback())
					window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_5, ANIMA_ENGINE_CORE_PRESS, mods);
			}
			
			return TRUE;
		}

		return 0;
	}

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	{
		const int mods = getKeyMods();

		ReleaseCapture();

		if (uMsg == WM_LBUTTONDOWN)
		{
			if (window->GetMouseClickCallback())
				window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_LEFT, ANIMA_ENGINE_CORE_RELEASE, mods);
		}
		else if (uMsg == WM_RBUTTONDOWN)
		{
			if (window->GetMouseClickCallback())
				window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_RIGHT, ANIMA_ENGINE_CORE_RELEASE, mods);
		}
		else if (uMsg == WM_MBUTTONDOWN)
		{
			if (window->GetMouseClickCallback())
				window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_MIDDLE, ANIMA_ENGINE_CORE_RELEASE, mods);
		}
		else
		{
			if (HIWORD(wParam) == XBUTTON1)
			{
				if (window->GetMouseClickCallback())
					window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_4, ANIMA_ENGINE_CORE_RELEASE, mods);
			}
			else if (HIWORD(wParam) == XBUTTON2)
			{
				if (window->GetMouseClickCallback())
					window->GetMouseClickCallback()(window, ANIMA_ENGINE_CORE_MOUSE_BUTTON_5, ANIMA_ENGINE_CORE_RELEASE, mods);
			}

			return TRUE;
		}
		
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		const int newCursorX = GET_X_LPARAM(lParam);
		const int newCursorY = GET_Y_LPARAM(lParam);

		if (newCursorX != window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorX || newCursorY != window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorY)
		{
			int x, y;

			if (window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_DISABLED)
			{
				if (window->GetEngine()->GetFocusedWindow() != window)
					return 0;

				x = newCursorX - window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorX;
				y = newCursorY - window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorY;
			}
			else
			{
				x = newCursorX;
				y = newCursorY;
			}

			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorX = newCursorX;
			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorY = newCursorY;
			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorCentered = false;

			if (window->GetCursorMotionCallback())
				window->GetCursorMotionCallback()(window, x, y);
		}

		if (!window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorInside)
		{
			TRACKMOUSEEVENT tme;
			ZeroMemory(&tme, sizeof(tme));
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle;
			TrackMouseEvent(&tme);

			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorInside = true;

			if (window->GetCursorEnterCallback())
				window->GetCursorEnterCallback()(window, true);
		}

		return 0;
	}

	case WM_MOUSELEAVE:
	{
		window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorInside = false;

		if (window->GetCursorEnterCallback())
			window->GetCursorEnterCallback()(window, false);
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		if (window->GetScrollCallback())
			window->GetScrollCallback()(window, 0.0, (SHORT)HIWORD(wParam) / (double)WHEEL_DELTA);
		return 0;
	}

	case WM_MOUSEHWHEEL:
	{
		// This message is only sent on Windows Vista and later
		// NOTE: The X-axis is inverted for consistency with OS X and X11.
		if (window->GetScrollCallback())
			window->GetScrollCallback()(window, -((SHORT)HIWORD(wParam) / (double)WHEEL_DELTA), 0.0);
		return 0;
	}

	case WM_SIZE:
	{
		if (window->GetEngine()->GetFocusedWindow() == window)
		{
			if (window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_DISABLED)
				updateClipRect(window);
		}

		if (window->GetFramebufferSizeCallback())
			window->GetFramebufferSizeCallback()(window, LOWORD(lParam), HIWORD(lParam));

		if (window->GetSizeCallback())
			window->GetSizeCallback()(window, LOWORD(lParam), HIWORD(lParam));

		return 0;
	}

	case WM_MOVE:
	{
		if (window->GetEngine()->GetFocusedWindow() == window)
		{
			if (window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_DISABLED)
				updateClipRect(window);
		}

		// NOTE: This cannot use LOWORD/HIWORD recommended by MSDN, as
		// those macros do not handle negative window positions correctly
		if (window->GetPosCallback())
			window->GetPosCallback()(window, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		return 0;
	}

	case WM_PAINT:
	{
		if (window->GetDamageCallback())
			window->GetDamageCallback()(window);
		break;
	}

	case WM_ERASEBKGND:
	{
		return TRUE;
	}

	case WM_SETCURSOR:
	{
		if (window->GetEngine()->GetFocusedWindow() == window && LOWORD(lParam) == HTCLIENT)
		{
			if (window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_HIDDEN || window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_DISABLED)
			{
				SetCursor(NULL);
				return TRUE;
			}
			else if (window->GetCursor())
			{
				SetCursor(window->GetCursor()->win32._handle);
				return TRUE;
			}
		}

		break;
	}

	case WM_DEVICECHANGE:
	{
		if (DBT_DEVNODES_CHANGED == wParam)
		{
			_AnimaEngineWindowInputMonitorChange(window->GetEngine());
			return TRUE;
		}
		break;
	}

	case WM_DWMCOMPOSITIONCHANGED:
	{
		if (_AnimaEngineWindowIsCompositionEnabled())
		{
			AnimaEngineWindow_Base* previous = _AnimaEngineWindowPlatformGetCurrentContext();
			_AnimaEngineWindowPlatformMakeContextCurrent(window);
			_AnimaEngineWindowPlatformSwapInterval(0);
			_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		}

		// TODO: Restore vsync if compositing was disabled
		break;
	}

	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		POINT pt;
		int i;

		const int count = DragQueryFileW(hDrop, 0xffffffff, NULL, 0);
		char** names = (char**)calloc(count, sizeof(char*));

		// Move the mouse to the position of the drop
		DragQueryPoint(hDrop, &pt);

		if (window->GetCursorMotionCallback())
			window->GetCursorMotionCallback()(window, pt.x, pt.y);

		for (i = 0; i < count; i++)
		{
			const UINT length = DragQueryFileW(hDrop, i, NULL, 0);
			WCHAR* buffer = (WCHAR*)calloc(length + 1, sizeof(WCHAR));

			DragQueryFileW(hDrop, i, buffer, length + 1);
			names[i] = _AnimaEngineWindowCreateUTF8FromWideString(buffer);

			free(buffer);
		}

		if (window->GetDropCallback())
			window->GetDropCallback()(window, count, (const char**)names);

		for (i = 0; i < count; i++)
			free(names[i]);
		free(names);

		DragFinish(hDrop);
		return 0;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static void getFullWindowSize(AnimaEngineWindow_Base* window, int clientWidth, int clientHeight, int* fullWidth, int* fullHeight)
{
	RECT rect = { 0, 0, clientWidth, clientHeight };

	AdjustWindowRectEx(&rect, window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle, FALSE, window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwExStyle);

	*fullWidth = rect.right - rect.left;
	*fullHeight = rect.bottom - rect.top;
}

static bool createWindow(AnimaEngineWindow_Base* window, const _AnimaEngineWindowwndconfig* wndconfig, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig)
{
	int xpos, ypos, fullWidth, fullHeight;
	WCHAR* wideTitle;

	window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwExStyle = WS_EX_APPWINDOW;

	if (window->GetMonitor())
	{
		window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle |= WS_POPUP;

		// NOTE: This window placement is temporary and approximate, as the
		//       correct position and size cannot be known until the monitor
		//       video mode has been set
		_AnimaEngineWindowPlatformGetMonitorPos(wndconfig->_monitor, &xpos, &ypos);
		fullWidth = wndconfig->_width;
		fullHeight = wndconfig->_height;
	}
	else
	{
		if (wndconfig->_decorated)
		{
			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			if (wndconfig->_resizable)
			{
				window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle |= WS_MAXIMIZEBOX | WS_SIZEBOX;
				window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwExStyle |= WS_EX_WINDOWEDGE;
			}
		}
		else
			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle |= WS_POPUP;

		xpos = CW_USEDEFAULT;
		ypos = CW_USEDEFAULT;

		getFullWindowSize(window, wndconfig->_width, wndconfig->_height, &fullWidth, &fullHeight);
	}

	wideTitle = _AnimaEngineWindowCreateWideStringFromUTF8(wndconfig->_title);
	if (!wideTitle)
	{
		//_glfwInputError(ANIMA_ENGINE_CORE_PLATFORM_ERROR, "Win32: Failed to convert title to wide string");
		return false;
	}

	window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle = CreateWindowExW(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwExStyle,
											_ANIMA_ENGINE_CORE_WNDCLASSNAME,
											wideTitle,
											window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle,
											xpos, ypos,
											fullWidth, fullHeight,
											NULL, // No parent window
											NULL, // No window menu
											GetModuleHandleW(NULL),
											window);

	free(wideTitle);

	if (!window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to create window");
		return false;
	}

	if (_AnimaEngineWindow_ChangeWindowMessageFilterEx)
	{
		_AnimaEngineWindow_ChangeWindowMessageFilterEx(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, WM_DROPFILES, MSGFLT_ALLOW, NULL);
		_AnimaEngineWindow_ChangeWindowMessageFilterEx(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, WM_COPYDATA, MSGFLT_ALLOW, NULL);
		_AnimaEngineWindow_ChangeWindowMessageFilterEx(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, WM_COPYGLOBALDATA, MSGFLT_ALLOW, NULL);
	}

	if (wndconfig->_floating && !wndconfig->_monitor)
	{
		SetWindowPos(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}

	DragAcceptFiles(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, TRUE);

	if (!_AnimaEngineWindowCreateContext(window, ctxconfig, fbconfig))
		return false;

	return true;
}

static void destroyWindow(AnimaEngineWindow_Base* window)
{
	_AnimaEngineWindowDestroyContext(window);

	if (window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle)
	{
		DestroyWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle);
		window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle = NULL;
	}
}

bool _AnimaEngineWindowRegisterWindowClass(void)
{
	WNDCLASSW wc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)windowProc;
	wc.cbClsExtra = 0;                           // No extra class data
	wc.cbWndExtra = sizeof(void*) + sizeof(int); // Make room for one pointer
	wc.hInstance = GetModuleHandleW(NULL);
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;                        // No background
	wc.lpszMenuName = NULL;                        // No menu
	wc.lpszClassName = _ANIMA_ENGINE_CORE_WNDCLASSNAME;

	// Load user-provided icon if available
	wc.hIcon = LoadIconW(GetModuleHandleW(NULL), L"ANIMA_ENGINE_CORE_ICON");
	if (!wc.hIcon)
	{
		// No user-provided icon found, load default icon
		wc.hIcon = LoadIconW(NULL, IDI_WINLOGO);
	}

	if (!RegisterClassW(&wc))
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to register window class");
		return false;
	}

	return true;
}

void _AnimaEngineWindowUnregisterWindowClass(void)
{
	UnregisterClassW(_ANIMA_ENGINE_CORE_WNDCLASSNAME, GetModuleHandleW(NULL));
}

bool _AnimaEngineWindowPlatformCreateWindow(AnimaEngineWindow_Base* window, const _AnimaEngineWindowwndconfig* wndconfig, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig)
{
	int status;

	if (!createWindow(window, wndconfig, ctxconfig, fbconfig))
		return false;

	status = _AnimaEngineWindowAnalyzeContext(window, ctxconfig, fbconfig);

	if (status == _ANIMA_ENGINE_CORE_RECREATION_IMPOSSIBLE)
		return false;

	if (status == _ANIMA_ENGINE_CORE_RECREATION_REQUIRED)
	{
		// Some window hints require us to re-create the context using WGL
		// extensions retrieved through the current context, as we cannot check
		// for WGL extensions or retrieve WGL entry points before we have a
		// current context (actually until we have implicitly loaded the ICD)

		// Yes, this is strange, and yes, this is the proper way on Win32

		// As Windows only allows you to set the pixel format once for a
		// window, we need to destroy the current window and create a new one
		// to be able to use the new pixel format

		// Technically, it may be possible to keep the old window around if
		// we're just creating an OpenGL 3.0+ context with the same pixel
		// format, but it's not worth the added code complexity

		// First we clear the current context (the one we just created)
		// This is usually done by glfwDestroyWindow, but as we're not doing
		// full ANIMA_ENGINE_CORE window destruction, it's duplicated here
		_AnimaEngineWindowPlatformMakeContextCurrent(NULL);

		// Next destroy the Win32 window and WGL context (without resetting or
		// destroying the ANIMA_ENGINE_CORE window object)
		destroyWindow(window);

		// ...and then create them again, this time with better APIs
		if (!createWindow(window, wndconfig, ctxconfig, fbconfig))
			return false;
	}

	if (window->GetMonitor())
	{
		_AnimaEngineWindowPlatformShowWindow(window);
		if (!enterFullscreenMode(window))
			return false;
	}

	return true;
}

void _AnimaEngineWindowPlatformDestroyWindow(AnimaEngineWindow_Base* window)
{
	if (window->GetMonitor())
		leaveFullscreenMode(window);

	destroyWindow(window);
}

void _AnimaEngineWindowPlatformSetWindowTitle(AnimaEngineWindow_Base* window, const char* title)
{
	WCHAR* wideTitle = _AnimaEngineWindowCreateWideStringFromUTF8(title);
	if (!wideTitle)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to convert title to wide string");
		return;
	}

	SetWindowTextW(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, wideTitle);
	free(wideTitle);
}

void _AnimaEngineWindowPlatformGetWindowPos(AnimaEngineWindow_Base* window, int* xpos, int* ypos)
{
	POINT pos = { 0, 0 };
	ClientToScreen(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, &pos);

	if (xpos)
		*xpos = pos.x;
	if (ypos)
		*ypos = pos.y;
}

void _AnimaEngineWindowPlatformSetWindowPos(AnimaEngineWindow_Base* window, int xpos, int ypos)
{
	RECT rect = { xpos, ypos, xpos, ypos };

	AdjustWindowRectEx(&rect, window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle, FALSE, window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwExStyle);
	SetWindowPos(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, NULL, rect.left, rect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

void _AnimaEngineWindowPlatformGetWindowSize(AnimaEngineWindow_Base* window, int* width, int* height)
{
	RECT area;
	GetClientRect(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, &area);

	if (width)
		*width = area.right;
	if (height)
		*height = area.bottom;
}

void _AnimaEngineWindowPlatformSetWindowSize(AnimaEngineWindow_Base* window, int width, int height)
{
	if (window->GetMonitor())
		enterFullscreenMode(window);
	else
	{
		int fullWidth, fullHeight;
		getFullWindowSize(window, width, height, &fullWidth, &fullHeight);

		SetWindowPos(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, HWND_TOP, 0, 0, fullWidth, fullHeight, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
	}
}

void _AnimaEngineWindowPlatformGetFramebufferSize(AnimaEngineWindow_Base* window, int* width, int* height)
{
	_AnimaEngineWindowPlatformGetWindowSize(window, width, height);
}

void _AnimaEngineWindowPlatformGetWindowFrameSize(AnimaEngineWindow_Base* window, int* left, int* top, int* right, int* bottom)
{
	RECT rect;
	int width, height;

	_AnimaEngineWindowPlatformGetWindowSize(window, &width, &height);
	SetRect(&rect, 0, 0, width, height);

	AdjustWindowRectEx(&rect, window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwStyle, FALSE, window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_dwExStyle);

	if (left)
		*left = -rect.left;
	if (top)
		*top = -rect.top;
	if (right)
		*right = rect.right - width;
	if (bottom)
		*bottom = rect.bottom - height;
}

void _AnimaEngineWindowPlatformIconifyWindow(AnimaEngineWindow_Base* window)
{
	ShowWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, SW_MINIMIZE);
}

void _AnimaEngineWindowPlatformRestoreWindow(AnimaEngineWindow_Base* window)
{
	ShowWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, SW_RESTORE);
}

void _AnimaEngineWindowPlatformShowWindow(AnimaEngineWindow_Base* window)
{
	ShowWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, SW_SHOW);
	BringWindowToTop(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle);
	SetForegroundWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle);
	SetFocus(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle);
}

void _AnimaEngineWindowPlatformUnhideWindow(AnimaEngineWindow_Base* window)
{
	ShowWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, SW_SHOW);
}

void _AnimaEngineWindowPlatformHideWindow(AnimaEngineWindow_Base* window)
{
	ShowWindow(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, SW_HIDE);
}

void _AnimaEngineWindowPlatformPollEvents(AnimaEngine* engine)
{
	MSG msg;
	AnimaEngineWindow_Base* window;

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			// Treat WM_QUIT as a close on all windows
			window = engine->GetWindowListHead();
			while (window)
			{
				if (window->GetCloseRequestCallback())
					window->GetCloseRequestCallback()(window);

				window = window->GetNext();
			}
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	window = engine->GetFocusedWindow();
	if (window)
	{
		// LSHIFT/RSHIFT fixup (keys tend to "stick" without this fix)
		// This is the only async event handling in GLFW, but it solves some
		// nasty problems
		{
			const int mods = getAsyncKeyMods();

			// Get current state of left and right shift keys
			const int lshiftDown = (GetAsyncKeyState(VK_LSHIFT) >> 15) & 1;
			const int rshiftDown = (GetAsyncKeyState(VK_RSHIFT) >> 15) & 1;

			// See if this differs from our belief of what has happened
			// (we only have to check for lost key up events)
			if (!lshiftDown && window->GetKeys()[ANIMA_ENGINE_CORE_KEY_LEFT_SHIFT] == 1)
			{
				if(window->GetKeyCallback())
					window->GetKeyCallback()(window, ANIMA_ENGINE_CORE_KEY_LEFT_SHIFT, 0, ANIMA_ENGINE_CORE_RELEASE, mods);
			}

			if (!rshiftDown && window->GetKeys()[ANIMA_ENGINE_CORE_KEY_RIGHT_SHIFT] == 1)
			{
				if (window->GetKeyCallback())
					window->GetKeyCallback()(window, ANIMA_ENGINE_CORE_KEY_RIGHT_SHIFT, 0, ANIMA_ENGINE_CORE_RELEASE, mods);
			}
		}

		// Did the cursor move in an focused window that has disabled the cursor
		if (window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_DISABLED && !window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorCentered)
		{
			int width, height;
			_AnimaEngineWindowPlatformGetWindowSize(window, &width, &height);
			_AnimaEngineWindowPlatformSetCursorPos(window, width / 2.0, height / 2.0);
			window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorCentered = true;
		}
	}
}

void _AnimaEngineWindowPlatformWaitEvents(AnimaEngine* engine)
{
	WaitMessage();

	_AnimaEngineWindowPlatformPollEvents(engine);
}

void _AnimaEngineWindowPlatformPostEmptyEvent(AnimaEngine* engine)
{
	AnimaEngineWindow_Base* window = engine->GetWindowListHead();
	PostMessage(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, WM_NULL, 0, 0);
}

void _AnimaEngineWindowPlatformSetCursorPos(AnimaEngineWindow_Base* window, double xpos, double ypos)
{
	POINT pos = { (int)xpos, (int)ypos };
	ClientToScreen(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle, &pos);
	SetCursorPos(pos.x, pos.y);

	window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorX = (int)xpos;
	window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_oldCursorY = (int)ypos;
}

void _AnimaEngineWindowPlatformApplyCursorMode(AnimaEngineWindow_Base* window)
{
	switch (window->GetCursorMode())
	{
	case ANIMA_ENGINE_CORE_CURSOR_NORMAL:
		restoreCursor(window);
		break;
	case ANIMA_ENGINE_CORE_CURSOR_HIDDEN:
		hideCursor(window);
		break;
	case ANIMA_ENGINE_CORE_CURSOR_DISABLED:
		disableCursor(window);
		break;
	}
}

bool _AnimaEngineWindowPlatformCreateCursor(_AnimaEngineWindowcursor* cursor, const AnimaEngineWindowimage* image, int xhot, int yhot)
{
	HDC dc;
	HBITMAP bitmap, mask;
	BITMAPV5HEADER bi;
	ICONINFO ii;
	DWORD* target = 0;
	BYTE* source = (BYTE*)image->_pixels;
	int i;

	ZeroMemory(&bi, sizeof(bi));
	bi.bV5Size = sizeof(BITMAPV5HEADER);
	bi.bV5Width = image->_width;
	bi.bV5Height = -image->_height;
	bi.bV5Planes = 1;
	bi.bV5BitCount = 32;
	bi.bV5Compression = BI_BITFIELDS;
	bi.bV5RedMask = 0x00ff0000;
	bi.bV5GreenMask = 0x0000ff00;
	bi.bV5BlueMask = 0x000000ff;
	bi.bV5AlphaMask = 0xff000000;

	dc = GetDC(NULL);
	bitmap = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&target, NULL, (DWORD)0);
	ReleaseDC(NULL, dc);

	if (!bitmap)
		return GL_FALSE;

	mask = CreateBitmap(image->_width, image->_height, 1, 1, NULL);
	if (!mask)
	{
		DeleteObject(bitmap);
		return GL_FALSE;
	}

	for (i = 0; i < image->_width * image->_height; i++, target++, source += 4)
	{
		*target = (source[3] << 24) | (source[0] << 16) | (source[1] << 8) | source[2];
	}

	ZeroMemory(&ii, sizeof(ii));
	ii.fIcon = FALSE;
	ii.xHotspot = xhot;
	ii.yHotspot = yhot;
	ii.hbmMask = mask;
	ii.hbmColor = bitmap;

	cursor->win32._handle = (HCURSOR)CreateIconIndirect(&ii);

	DeleteObject(bitmap);
	DeleteObject(mask);

	if (!cursor->win32._handle)
		return false;

	return true;
}

void _AnimaEngineWindowPlatformDestroyCursor(_AnimaEngineWindowcursor* cursor)
{
	if (cursor->win32._handle)
		DestroyIcon((HICON)cursor->win32._handle);
}

void _AnimaEngineWindowPlatformSetCursor(AnimaEngine* engine, AnimaEngineWindow_Base* window, _AnimaEngineWindowcursor* cursor)
{
	// It should be guaranteed that the cursor is not being used by this window if
	// the following condition is not met. That way it should be safe to destroy the
	// cursor after calling glfwSetCursor(window, NULL) on all windows using the cursor.
	if (window->GetCursorMode() == ANIMA_ENGINE_CORE_CURSOR_NORMAL && engine->GetFocusedWindow() == window && window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_cursorInside)
	{
		if (cursor)
			SetCursor(cursor->win32._handle);
		else
			SetCursor(LoadCursorW(NULL, IDC_ARROW));
	}
}

void _AnimaEngineWindowPlatformSetClipboardString(AnimaEngineWindow_Base* window, const char* string)
{
	WCHAR* wideString;
	HANDLE stringHandle;
	size_t wideSize;

	wideString = _AnimaEngineWindowCreateWideStringFromUTF8(string);
	if (!wideString)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to convert clipboard string to " "wide string");
		return;
	}

	wideSize = (wcslen(wideString) + 1) * sizeof(WCHAR);

	stringHandle = GlobalAlloc(GMEM_MOVEABLE, wideSize);
	if (!stringHandle)
	{
		free(wideString);

		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to allocate global handle for clipboard");
		return;
	}

	memcpy(GlobalLock(stringHandle), wideString, wideSize);
	GlobalUnlock(stringHandle);

	if (!OpenClipboard(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle))
	{
		GlobalFree(stringHandle);
		free(wideString);

		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to open clipboard");
		return;
	}

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, stringHandle);
	CloseClipboard();

	free(wideString);
}

const char* _AnimaEngineWindowPlatformGetClipboardString(AnimaEngineWindow_Base* window)
{
	HANDLE stringHandle;

	if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
	{
		//_glfwInputError(ANIMA_ENGINE_CORE_FORMAT_UNAVAILABLE, NULL);
		return NULL;
	}

	if (!OpenClipboard(window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle))
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to open clipboard");
		return NULL;
	}

	stringHandle = GetClipboardData(CF_UNICODETEXT);
	if (!stringHandle)
	{
		CloseClipboard();

		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to retrieve clipboard data");
		return NULL;
	}

	free(AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString);
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString = _AnimaEngineWindowCreateUTF8FromWideString((const WCHAR*)GlobalLock(stringHandle));

	GlobalUnlock(stringHandle);
	CloseClipboard();

	if (!AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to convert wide string to UTF-8");
		return NULL;
	}

	return AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE->_clipboardString;
}

HWND AnimaEngineWindowGetWin32Window(AnimaEngineWindow_Base* handle)
{
	AnimaEngineWindow_Base* window = (AnimaEngineWindow_Base*)handle;
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);
	return window->_GET_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE->_handle;
}

bool _AnimaEngineWindowSetVideoMode(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowvidmode* desired)
{
	AnimaEngineWindowvidmode current;
	const AnimaEngineWindowvidmode* best;
	DEVMODEW dm;

	best = _AnimaEngineWindowChooseVideoMode(monitor, desired);
	_AnimaEngineWindowPlatformGetVideoMode(monitor, &current);
	if (_AnimaEngineWindowCompareVideoModes(&current, best) == 0)
		return true;

	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(DEVMODEW);
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
	dm.dmPelsWidth = best->_width;
	dm.dmPelsHeight = best->_height;
	dm.dmBitsPerPel = best->_redBits + best->_greenBits + best->_blueBits;
	dm.dmDisplayFrequency = best->_refreshRate;

	if (dm.dmBitsPerPel < 15 || dm.dmBitsPerPel >= 24)
		dm.dmBitsPerPel = 32;

	if (ChangeDisplaySettingsExW(monitor->win32._adapterName, &dm, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to set video mode");
		return false;
	}

	monitor->win32._modeChanged = true;
	return true;
}

void _AnimaEngineWindowRestoreVideoMode(_AnimaEngineWindowmonitor* monitor)
{
	if (monitor->win32._modeChanged)
	{
		ChangeDisplaySettingsExW(monitor->win32._adapterName, NULL, NULL, CDS_FULLSCREEN, NULL);
		monitor->win32._modeChanged = false;
	}
}

_AnimaEngineWindowmonitor** _AnimaEngineWindowPlatformGetMonitors(int* count)
{
	int size = 0, found = 0;
	_AnimaEngineWindowmonitor** monitors = NULL;
	DWORD adapterIndex, displayIndex;

	*count = 0;

	for (adapterIndex = 0;; adapterIndex++)
	{
		DISPLAY_DEVICEW adapter;

		ZeroMemory(&adapter, sizeof(DISPLAY_DEVICEW));
		adapter.cb = sizeof(DISPLAY_DEVICEW);

		if (!EnumDisplayDevicesW(NULL, adapterIndex, &adapter, 0))
			break;

		if (!(adapter.StateFlags & DISPLAY_DEVICE_ACTIVE))
			continue;

		for (displayIndex = 0;; displayIndex++)
		{
			DISPLAY_DEVICEW display;
			char* name;
			HDC dc;

			ZeroMemory(&display, sizeof(DISPLAY_DEVICEW));
			display.cb = sizeof(DISPLAY_DEVICEW);

			if (!EnumDisplayDevicesW(adapter.DeviceName, displayIndex, &display, 0))
				break;

			if (found == size)
			{
				size += 4;
				monitors = (_AnimaEngineWindowmonitor**)realloc(monitors, sizeof(_AnimaEngineWindowmonitor*) * size);
			}

			name = _AnimaEngineWindowCreateUTF8FromWideString(display.DeviceString);
			if (!name)
			{
				//_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to convert string to UTF-8");
				continue;
			}

			dc = CreateDCW(L"DISPLAY", adapter.DeviceName, NULL, NULL);

			monitors[found] = _AnimaEngineWindowAllocMonitor(name, GetDeviceCaps(dc, HORZSIZE), GetDeviceCaps(dc, VERTSIZE));

			DeleteDC(dc);
			free(name);

			wcscpy(monitors[found]->win32._adapterName, adapter.DeviceName);
			wcscpy(monitors[found]->win32._displayName, display.DeviceName);

			WideCharToMultiByte(CP_UTF8, 0, adapter.DeviceName, -1, monitors[found]->win32._publicAdapterName, sizeof(monitors[found]->win32._publicAdapterName), NULL, NULL);

			WideCharToMultiByte(CP_UTF8, 0, display.DeviceName, -1, monitors[found]->win32._publicDisplayName, sizeof(monitors[found]->win32._publicDisplayName), NULL, NULL);

			if (adapter.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE &&
				displayIndex == 0)
			{
				_ANIMA_ENGINE_CORE_SWAP_POINTERS(monitors[0], monitors[found], _AnimaEngineWindowmonitor*);
			}

			found++;
		}
	}

	*count = found;
	return monitors;
}

bool _AnimaEngineWindowPlatformIsSameMonitor(_AnimaEngineWindowmonitor* first, _AnimaEngineWindowmonitor* second)
{
	return wcscmp(first->win32._displayName, second->win32._displayName) == 0;
}

void _AnimaEngineWindowPlatformGetMonitorPos(_AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos)
{
	DEVMODEW settings;
	ZeroMemory(&settings, sizeof(DEVMODEW));
	settings.dmSize = sizeof(DEVMODEW);

	EnumDisplaySettingsExW(monitor->win32._adapterName, ENUM_CURRENT_SETTINGS, &settings, EDS_ROTATEDMODE);

	if (xpos)
		*xpos = settings.dmPosition.x;
	if (ypos)
		*ypos = settings.dmPosition.y;
}

AnimaEngineWindowvidmode* _AnimaEngineWindowPlatformGetVideoModes(_AnimaEngineWindowmonitor* monitor, int* found)
{
	int modeIndex = 0, count = 0;
	AnimaEngineWindowvidmode* result = NULL;

	*found = 0;

	for (;;)
	{
		int i;
		AnimaEngineWindowvidmode mode;
		DEVMODEW dm;

		ZeroMemory(&dm, sizeof(DEVMODEW));
		dm.dmSize = sizeof(DEVMODEW);

		if (!EnumDisplaySettingsW(monitor->win32._adapterName, modeIndex, &dm))
			break;

		modeIndex++;

		if (dm.dmBitsPerPel < 15)
		{
			// Skip modes with less than 15 BPP
			continue;
		}

		mode._width = dm.dmPelsWidth;
		mode._height = dm.dmPelsHeight;
		mode._refreshRate = dm.dmDisplayFrequency;
		_AnimaEngineWindowSplitBPP(dm.dmBitsPerPel, &mode._redBits, &mode._greenBits, &mode._blueBits);

		for (i = 0; i < *found; i++)
		{
			if (_AnimaEngineWindowCompareVideoModes(result + i, &mode) == 0)
				break;
		}

		if (i < *found)
		{
			// This is a duplicate, so skip it
			continue;
		}

		if (*found == count)
		{
			if (count)
				count *= 2;
			else
				count = 128;

			result = (AnimaEngineWindowvidmode*)realloc(result, count * sizeof(AnimaEngineWindowvidmode));
		}

		result[*found] = mode;
		(*found)++;
	}

	return result;
}

void _AnimaEngineWindowPlatformGetVideoMode(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowvidmode* mode)
{
	DEVMODEW dm;

	ZeroMemory(&dm, sizeof(DEVMODEW));
	dm.dmSize = sizeof(DEVMODEW);

	EnumDisplaySettingsW(monitor->win32._adapterName, ENUM_CURRENT_SETTINGS, &dm);

	mode->_width = dm.dmPelsWidth;
	mode->_height = dm.dmPelsHeight;
	mode->_refreshRate = dm.dmDisplayFrequency;
	_AnimaEngineWindowSplitBPP(dm.dmBitsPerPel, &mode->_redBits, &mode->_greenBits, &mode->_blueBits);
}

void _AnimaEngineWindowPlatformGetGammaRamp(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowgammaramp* ramp)
{
	HDC dc;
	WORD values[768];

	dc = CreateDCW(L"DISPLAY", monitor->win32._adapterName, NULL, NULL);
	GetDeviceGammaRamp(dc, values);
	DeleteDC(dc);

	_AnimaEngineWindowAllocGammaArrays(ramp, 256);

	memcpy(ramp->_red, values + 0, 256 * sizeof(unsigned short));
	memcpy(ramp->_green, values + 256, 256 * sizeof(unsigned short));
	memcpy(ramp->_blue, values + 512, 256 * sizeof(unsigned short));
}

void _AnimaEngineWindowPlatformSetGammaRamp(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowgammaramp* ramp)
{
	HDC dc;
	WORD values[768];

	if (ramp->_size != 256)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Gamma ramp size must be 256");
		return;
	}

	memcpy(values + 0, ramp->_red, 256 * sizeof(unsigned short));
	memcpy(values + 256, ramp->_green, 256 * sizeof(unsigned short));
	memcpy(values + 512, ramp->_blue, 256 * sizeof(unsigned short));

	dc = CreateDCW(L"DISPLAY", monitor->win32._adapterName, NULL, NULL);
	SetDeviceGammaRamp(dc, values);
	DeleteDC(dc);
}

const char* AnimaEngineWindowGetWin32Adapter(AnimaEngineWindowmonitor* handle)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);
	return monitor->win32._publicAdapterName;
}

const char* AnimaEngineWindowGetWin32Monitor(AnimaEngineWindowmonitor* handle)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);
	return monitor->win32._publicDisplayName;
}

static unsigned __int64 getRawTime(void)
{
	if (AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_hasPC)
	{
		unsigned __int64 time;
		QueryPerformanceCounter((LARGE_INTEGER*)&time);
		return time;
	}
	else
		return (unsigned __int64)_AnimaEngineWindow_timeGetTime();
}

void _AnimaEngineWindowInitTimer(void)
{
	unsigned __int64 frequency;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
	{
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_hasPC = true;
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_resolution = 1.0 / (double)frequency;
	}
	else
	{
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_hasPC = false;
		AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_resolution = 0.001;
	}

	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_base = getRawTime();
}

double _AnimaEngineWindowPlatformGetTime(void)
{
	return (double)(getRawTime() - AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_base) * AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_resolution;
}

void _AnimaEngineWindowPlatformSetTime(double time)
{
	AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_base = getRawTime() - (unsigned __int64)(time / AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE->_resolution);
}


END_ANIMA_ENGINE_CORE_NAMESPACE