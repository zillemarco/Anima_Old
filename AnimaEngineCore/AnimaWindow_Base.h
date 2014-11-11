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

	AnimaEngineWindowvidmode	_videoMode;
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
	//	AnimaEngineWindowwindowposfun		_pos;
	//	AnimaEngineWindowwindowsizefun		_size;
	//	AnimaEngineWindowwindowclosefun		_close;
	//	AnimaEngineWindowwindowrefreshfun	_refresh;
	//	AnimaEngineWindowwindowfocusfun		_focus;
	//	AnimaEngineWindowwindowiconifyfun	_iconify;
	//	AnimaEngineWindowframebuffersizefun	_fbsize;
	//	AnimaEngineWindowmousebuttonfun		_mouseButton;
	//	AnimaEngineWindowcursorposfun		_cursorPos;
	//	AnimaEngineWindowcursorenterfun		_cursorEnter;
	//	AnimaEngineWindowscrollfun			_scroll;
	//	AnimaEngineWindowkeyfun				_key;
	//	AnimaEngineWindowcharfun			_character;
	//	AnimaEngineWindowcharmodsfun		_charmods;
	//	AnimaEngineWindowdropfun			_drop;
	//} _callbacks;

	_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
};

struct _AnimaEngineWindowmonitor
{
	char*	_name;
	int		_widthMM, _heightMM;

	AnimaEngineWindowvidmode*	_modes;
	int						_modeCount;
	AnimaEngineWindowvidmode	_currentMode;

	AnimaEngineWindowgammaramp	_originalRamp;
	AnimaEngineWindowgammaramp	_currentRamp;

	_ANIMA_ENGINE_CORE_PLATFORM_MONITOR_STATE;
};

struct _AnimaEngineWindowcursor
{
	_AnimaEngineWindowcursor* next;

	_ANIMA_ENGINE_CORE_PLATFORM_CURSOR_STATE;
};

#define ANIMA_ENGINE_WINDOW_HINTS	struct {					\
										int	_redBits;			\
										int	_greenBits;			\
										int	_blueBits;			\
										int	_alphaBits;			\
										int	_depthBits;			\
										int	_stencilBits;		\
										int	_accumRedBits;		\
										int	_accumGreenBits;	\
										int	_accumBlueBits;		\
										int	_accumAlphaBits;	\
										int	_auxBuffers;		\
										int	_stereo;			\
										int	_resizable;			\
										int	_visible;			\
										int	_decorated;			\
										int	_focused;			\
										int	_autoIconify;		\
										int	_floating;			\
										int	_samples;			\
										int	_sRGB;				\
										int	_refreshRate;		\
										int	_doublebuffer;		\
										int	_api;				\
										int	_major;				\
										int	_minor;				\
										int	_forward;			\
										int	_debug;				\
										int	_profile;			\
										int	_robustness;		\
										int	_release;			\
									}

extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowPlatformInit(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformTerminate(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT const char* _AnimaEngineWindowPlatformGetVersionString(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetCursorPos(_AnimaEngineWindowwindow* window, double xpos, double ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformApplyCursorMode(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT _AnimaEngineWindowmonitor** _AnimaEngineWindowPlatformGetMonitors(int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT GLboolean _AnimaEngineWindowPlatformIsSameMonitor(_AnimaEngineWindowmonitor* first, _AnimaEngineWindowmonitor* second);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetMonitorPos(_AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindowvidmode* _AnimaEngineWindowPlatformGetVideoModes(_AnimaEngineWindowmonitor* monitor, int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetVideoMode(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowvidmode* mode);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetGammaRamp(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowgammaramp* ramp);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetGammaRamp(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowgammaramp* ramp);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetClipboardString(_AnimaEngineWindowwindow* window, const char* string);
extern "C" ANIMA_ENGINE_CORE_EXPORT const char* _AnimaEngineWindowPlatformGetClipboardString(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowPlatformJoystickPresent(int joy);
extern "C" ANIMA_ENGINE_CORE_EXPORT const float* _AnimaEngineWindowPlatformGetJoystickAxes(int joy, int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT const unsigned char* _AnimaEngineWindowPlatformGetJoystickButtons(int joy, int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT const char* _AnimaEngineWindowPlatformGetJoystickName(int joy);
extern "C" ANIMA_ENGINE_CORE_EXPORT double _AnimaEngineWindowPlatformGetTime(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetTime(double time);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowPlatformCreateWindow(_AnimaEngineWindowwindow* window, const _AnimaEngineWindowwndconfig* wndconfig, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformDestroyWindow(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetWindowTitle(_AnimaEngineWindowwindow* window, const char* title);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetWindowPos(_AnimaEngineWindowwindow* window, int* xpos, int* ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetWindowPos(_AnimaEngineWindowwindow* window, int xpos, int ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetWindowSize(_AnimaEngineWindowwindow* window, int* width, int* height);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetWindowSize(_AnimaEngineWindowwindow* window, int width, int height);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetFramebufferSize(_AnimaEngineWindowwindow* window, int* width, int* height);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformGetWindowFrameSize(_AnimaEngineWindowwindow* window, int* left, int* top, int* right, int* bottom);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformIconifyWindow(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformRestoreWindow(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformShowWindow(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformUnhideWindow(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformHideWindow(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformPollEvents(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformWaitEvents(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformPostEmptyEvent(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformMakeContextCurrent(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT _AnimaEngineWindowwindow* _AnimaEngineWindowPlatformGetCurrentContext(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSwapBuffers(_AnimaEngineWindowwindow* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSwapInterval(int interval);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowPlatformExtensionSupported(const char* extension);
extern "C" ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindowglproc _AnimaEngineWindowPlatformGetProcAddress(const char* procname);
extern "C" ANIMA_ENGINE_CORE_EXPORT int _AnimaEngineWindowPlatformCreateCursor(_AnimaEngineWindowcursor* cursor, const AnimaEngineWindowimage* image, int xhot, int yhot);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformDestroyCursor(_AnimaEngineWindowcursor* cursor);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowPlatformSetCursor(_AnimaEngineWindowwindow* window, _AnimaEngineWindowcursor* cursor);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowStringInExtensionString(const char* string, const GLubyte* extensions);
extern "C" ANIMA_ENGINE_CORE_EXPORT const _AnimaEngineWindowfbconfig* _AnimaEngineWindowChooseFBConfig(const _AnimaEngineWindowfbconfig* desired, const _AnimaEngineWindowfbconfig* alternatives, unsigned int count);


END_ANIMA_ENGINE_CORE_NAMESPACE

#endif