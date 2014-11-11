#ifndef _AnimaWindow_Base_h
#define _AnimaWindow_Base_h

#include "AnimaEngineCore.h"

#include "AnimaWindow_Data.h"

#if defined(_ANIMA_ENGINE_CORE_USE_OPENGL)
#	include <GL\glext.h>
#endif

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE
typedef struct _AnimaEngineWindowwndconfig   _AnimaEngineWindowwndconfig;
typedef struct _AnimaEngineWindowctxconfig   _AnimaEngineWindowctxconfig;
typedef struct _AnimaEngineWindowfbconfig    _AnimaEngineWindowfbconfig;
typedef struct _AnimaEngineWindowwindow      _AnimaEngineWindowwindow;
typedef struct _AnimaEngineWindowlibrary     _AnimaEngineWindowlibrary;
typedef struct _AnimaEngineWindowmonitor     _AnimaEngineWindowmonitor;
typedef struct _AnimaEngineWindowcursor      _AnimaEngineWindowcursor;
END_ANIMA_ENGINE_CORE_NAMESPACE

#if defined(_ANIMA_ENGINE_CORE_COCOA)
#	include "cocoa_platform.h"
#elif defined(_ANIMA_ENGINE_CORE_WIN32)
#	include "AnimaWindow_WindowsPlatform.h"
#elif defined(_ANIMA_ENGINE_CORE_X11)
#	include "x11_platform.h"
#elif defined(_ANIMA_ENGINE_CORE_WAYLAND)
#	include "wl_platform.h"
#elif defined(_ANIMA_ENGINE_CORE_MIR)
#	include "mir_platform.h"
#else
#	error "No supported window creation API selected"
#endif

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

struct _AnimaEngineWindowwndconfig
{
	int			_width;
	int			_height;
	const char*	_title;
	bool		_resizable;
	bool		_visible;
	bool		_decorated;
	bool		_focused;
	bool		_autoIconify;
	bool		_floating;
	_AnimaEngineWindowmonitor* _monitor;
};

struct _AnimaEngineWindowctxconfig
{
	int		_api;
	int		_major;
	int		_minor;
	bool	_forward;
	bool	_debug;
	int		_profile;
	int		_robustness;
	int		_release;
	_AnimaEngineWindowwindow* _share;
};

struct _AnimaEngineWindowfbconfig
{
	int _redBits;
	int _greenBits;
	int _blueBits;
	int _alphaBits;
	int _depthBits;
	int _stencilBits;
	int _accumRedBits;
	int _accumGreenBits;
	int _accumBlueBits;
	int _accumAlphaBits;
	int _auxBuffers;
	int _stereo;
	int _samples;
	int _sRGB;
	int _doublebuffer;

	_ANIMA_ENGINE_CORE_PLATFORM_FBCONFIG;
};

struct _AnimaEngineWindowwindow
{
	struct _AnimaEngineWindowwindow* _next;

	bool	_iconified;
	bool	_resizable;
	bool	_decorated;
	bool	_autoIconify;
	bool	_floating;
	bool	_visible;
	bool	_closed;
	void*	_userPointer;

	AnimaEngineWSvidmode	_videoMode;
	_AnimaEngineWindowmonitor*	_monitor;
	_AnimaEngineWindowcursor*	_cursor;

	bool	_stickyKeys;
	bool	_stickyMouseButtons;
	double	_cursorPosX, _cursorPosY;
	int		_cursorMode;
	char	_mouseButtons[ANIMA_ENGINE_CORE_MOUSE_BUTTON_LAST + 1];
	char	_keys[ANIMA_ENGINE_CORE_KEY_LAST + 1];

	struct {
		int		_api;
		int		_major, _minor, _revision;
		bool	_forward, _debug;
		int		_profile;
		int		_robustness;
		int		_release;
	} _context;

#if defined(_ANIMA_ENGINE_CORE_USE_OPENGL)
	PFNGLGETSTRINGIPROC _GetStringi;
#endif

	//struct {
	//	AnimaEngineWSwindowposfun		_pos;
	//	AnimaEngineWSwindowsizefun		_size;
	//	AnimaEngineWSwindowclosefun		_close;
	//	AnimaEngineWSwindowrefreshfun	_refresh;
	//	AnimaEngineWSwindowfocusfun		_focus;
	//	AnimaEngineWSwindowiconifyfun	_iconify;
	//	AnimaEngineWSframebuffersizefun	_fbsize;
	//	AnimaEngineWSmousebuttonfun		_mouseButton;
	//	AnimaEngineWScursorposfun		_cursorPos;
	//	AnimaEngineWScursorenterfun		_cursorEnter;
	//	AnimaEngineWSscrollfun			_scroll;
	//	AnimaEngineWSkeyfun				_key;
	//	AnimaEngineWScharfun			_character;
	//	AnimaEngineWScharmodsfun		_charmods;
	//	AnimaEngineWSdropfun			_drop;
	//} _callbacks;

	_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
};

struct _AnimaEngineWindowmonitor
{
	char*	_name;
	int		_widthMM, _heightMM;

	AnimaEngineWSvidmode*	_modes;
	int						_modeCount;
	AnimaEngineWSvidmode	_currentMode;

	AnimaEngineWSgammaramp	_originalRamp;
	AnimaEngineWSgammaramp	_currentRamp;

	_ANIMA_ENGINE_CORE_PLATFORM_MONITOR_STATE;
};

struct _AnimaEngineWindowcursor
{
	_AnimaEngineWindowcursor* next;

	_ANIMA_ENGINE_CORE_PLATFORM_CURSOR_STATE;
};

struct _AnimaEngineWindowlibrary
{
	struct {
		int	_redBits;
		int	_greenBits;
		int	_blueBits;
		int	_alphaBits;
		int	_depthBits;
		int	_stencilBits;
		int	_accumRedBits;
		int	_accumGreenBits;
		int	_accumBlueBits;
		int	_accumAlphaBits;
		int	_auxBuffers;
		int	_stereo;
		int	_resizable;
		int	_visible;
		int	_decorated;
		int	_focused;
		int	_autoIconify;
		int	_floating;
		int	_samples;
		int	_sRGB;
		int	_refreshRate;
		int	_doublebuffer;
		int	_api;
		int	_major;
		int	_minor;
		int	_forward;
		int	_debug;
		int	_profile;
		int	_robustness;
		int	_release;
	} _hints;

	double _cursorPosX, _cursorPosY;

	_AnimaEngineWindowcursor*	_cursorListHead;

	_AnimaEngineWindowwindow*	_windowListHead;
	_AnimaEngineWindowwindow*	_focusedWindow;

	_AnimaEngineWindowmonitor**	_monitors;
	int						_monitorCount;

	//struct {
	//	AnimaEngineWSmonitorfun	_monitor;
	//} _callbacks;

	_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE;
};

bool _AnimaEngineWindowPlatformInit(void);
void _AnimaEngineWindowPlatformTerminate(void);
const char* _AnimaEngineWindowPlatformGetVersionString(void);
void _AnimaEngineWindowPlatformSetCursorPos(_AnimaEngineWindowwindow* window, double xpos, double ypos);
void _AnimaEngineWindowPlatformApplyCursorMode(_AnimaEngineWindowwindow* window);
_AnimaEngineWindowmonitor** _AnimaEngineWindowPlatformGetMonitors(int* count);
GLboolean _AnimaEngineWindowPlatformIsSameMonitor(_AnimaEngineWindowmonitor* first, _AnimaEngineWindowmonitor* second);
void _AnimaEngineWindowPlatformGetMonitorPos(_AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos);
AnimaEngineWSvidmode* _AnimaEngineWindowPlatformGetVideoModes(_AnimaEngineWindowmonitor* monitor, int* count);
void _AnimaEngineWindowPlatformGetVideoMode(_AnimaEngineWindowmonitor* monitor, AnimaEngineWSvidmode* mode);
void _AnimaEngineWindowPlatformGetGammaRamp(_AnimaEngineWindowmonitor* monitor, AnimaEngineWSgammaramp* ramp);
void _AnimaEngineWindowPlatformSetGammaRamp(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWSgammaramp* ramp);
void _AnimaEngineWindowPlatformSetClipboardString(_AnimaEngineWindowwindow* window, const char* string);
const char* _AnimaEngineWindowPlatformGetClipboardString(_AnimaEngineWindowwindow* window);
int _AnimaEngineWindowPlatformJoystickPresent(int joy);
const float* _AnimaEngineWindowPlatformGetJoystickAxes(int joy, int* count);
const unsigned char* _AnimaEngineWindowPlatformGetJoystickButtons(int joy, int* count);
const char* _AnimaEngineWindowPlatformGetJoystickName(int joy);
double _AnimaEngineWindowPlatformGetTime(void);
void _AnimaEngineWindowPlatformSetTime(double time);
int _AnimaEngineWindowPlatformCreateWindow(_AnimaEngineWindowwindow* window, const _AnimaEngineWindowwndconfig* wndconfig, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);
void _AnimaEngineWindowPlatformDestroyWindow(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformSetWindowTitle(_AnimaEngineWindowwindow* window, const char* title);
void _AnimaEngineWindowPlatformGetWindowPos(_AnimaEngineWindowwindow* window, int* xpos, int* ypos);
void _AnimaEngineWindowPlatformSetWindowPos(_AnimaEngineWindowwindow* window, int xpos, int ypos);
void _AnimaEngineWindowPlatformGetWindowSize(_AnimaEngineWindowwindow* window, int* width, int* height);
void _AnimaEngineWindowPlatformSetWindowSize(_AnimaEngineWindowwindow* window, int width, int height);
void _AnimaEngineWindowPlatformGetFramebufferSize(_AnimaEngineWindowwindow* window, int* width, int* height);
void _AnimaEngineWindowPlatformGetWindowFrameSize(_AnimaEngineWindowwindow* window, int* left, int* top, int* right, int* bottom);
void _AnimaEngineWindowPlatformIconifyWindow(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformRestoreWindow(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformShowWindow(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformUnhideWindow(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformHideWindow(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformPollEvents(void);
void _AnimaEngineWindowPlatformWaitEvents(void);
void _AnimaEngineWindowPlatformPostEmptyEvent(void);
void _AnimaEngineWindowPlatformMakeContextCurrent(_AnimaEngineWindowwindow* window);
_AnimaEngineWindowwindow* _AnimaEngineWindowPlatformGetCurrentContext(void);
void _AnimaEngineWindowPlatformSwapBuffers(_AnimaEngineWindowwindow* window);
void _AnimaEngineWindowPlatformSwapInterval(int interval);
int _AnimaEngineWindowPlatformExtensionSupported(const char* extension);
AnimaEngineWSglproc _AnimaEngineWindowPlatformGetProcAddress(const char* procname);
int _AnimaEngineWindowPlatformCreateCursor(_AnimaEngineWindowcursor* cursor, const AnimaEngineWSimage* image, int xhot, int yhot);
void _AnimaEngineWindowPlatformDestroyCursor(_AnimaEngineWindowcursor* cursor);
void _AnimaEngineWindowPlatformSetCursor(_AnimaEngineWindowwindow* window, _AnimaEngineWindowcursor* cursor);


END_ANIMA_ENGINE_CORE_NAMESPACE

#endif