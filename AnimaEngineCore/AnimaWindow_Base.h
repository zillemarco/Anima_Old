#ifndef _AnimaWindow_Base_h
#define _AnimaWindow_Base_h

#include "AnimaEngineCore.h"

#include "AnimaWindow_Data.h"

#if defined(_ANIMA_ENGINE_CORE_USE_OPENGL)
#	include <GL/glext.h>
#endif

#if defined ANIMA_ENGINE_CORE_APIENTRY_DEFINED
#	undef APIENTRY
#endif

#if defined ANIMA_ENGINE_CORE_WINGDIAPI_DEFINED
#	undef WINGDIAPI
#endif

#if defined ANIMA_ENGINE_CORE_CALLBACK_DEFINED
#	undef CALLBACK
#endif

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE
typedef class AnimaEngine						AnimaEngine;
typedef class AnimaEngineWindow_Base			AnimaEngineWindow_Base;

typedef struct _AnimaEngineWindowwndconfig		_AnimaEngineWindowwndconfig;
typedef struct _AnimaEngineWindowctxconfig		_AnimaEngineWindowctxconfig;
typedef struct _AnimaEngineWindowfbconfig		_AnimaEngineWindowfbconfig;
typedef struct _AnimaEngineWindowmonitor		_AnimaEngineWindowmonitor;
typedef struct _AnimaEngineWindowcursor			_AnimaEngineWindowcursor;
END_ANIMA_ENGINE_CORE_NAMESPACE

#if defined(_ANIMA_ENGINE_CORE_COCOA)
#	include "AnimaWindow_CocoaPlatform.h"
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
	AnimaEngineWindow_Base* _share;
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
	bool _stereo;
	int _samples;
	bool _sRGB;
	bool _doublebuffer;

	_ANIMA_ENGINE_CORE_PLATFORM_FBCONFIG;
};

typedef struct AnimaEngineWindowContext
{
	int		_api;
	int		_major, _minor, _revision;
	bool	_forward, _debug;
	int		_profile;
	int		_robustness;
	int		_release;
} AnimaEngineWindowContext;

#define DECLARE_MESSAGE_MAP()				virtual void InitCallbacks();
#define BEGIN_MESSAGE_MAP(class, baseClass)	void class::InitCallbacks() { baseClass::InitCallbacks();
#define END_MESSAGE_MAP()					}

#define ANIMA_WINDOW_EVENT static

#define ANIMA_WINDOW_FOCUS_CHANGED_EVENT(callback)				SetFocusCallback(callback);
#define ANIMA_WINDOW_POSITION_CHANGED_EVENT(callback)			SetPosCallback(callback);
#define ANIMA_WINDOW_SIZE_CHANGED_EVENT(callback)				SetSizeCallback(callback);
#define ANIMA_WINDOW_ICONIFY_EVENT(callback)					SetIconifyCallback(callback);
#define ANIMA_WINDOW_VISIBILITY_CHANGED_EVENT(callback)			SetVisibilityCallback(callback);
#define ANIMA_WINDOW_PAINT_EVENT(callback)						SetDamageCallback(callback);
#define ANIMA_WINDOW_CLOSE_WINDOW_EVENT(callback)				SetCloseRequestCallback(callback);
#define ANIMA_WINDOW_KEY_EVENT(callback)						SetKeyCallback(callback);
#define ANIMA_WINDOW_CHAR_EVENT(callback)						SetCharCallback(callback);
#define ANIMA_WINDOW_MOUSE_SCROLL_EVENT(callback)				SetScrollCallback(callback);
#define ANIMA_WINDOW_MOUSE_CLICK_EVENT(callback)				SetMouseClickCallback(callback);
#define ANIMA_WINDOW_CURSOR_MOVE_EVENT(callback)				SetCursorMotionCallback(callback);
#define ANIMA_WINDOW_CURSOR_ENTER_EVENT(callback)				SetCursorEnterCallback(callback);
#define ANIMA_WINDOW_DROP_EVENT(callback)						SetDropCallback(callback);
#define ANIMA_WINDOW_FRAMEBUFFER_SIZE_CHANGED_EVENT(callback)	SetFramebufferSizeCallback(callback);

class ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindow_Base
{
public:
	AnimaEngineWindow_Base() {
		InternalInit();

		_windowId = _windowCount;
		_windowCount++;
	}

	virtual ~AnimaEngineWindow_Base() {
	}

	void SetFocusCallback(AnimaEngineWindowFocusCallback callback)						{ _windowFocusCallback = callback; }
	void SetPosCallback(AnimaEngineWindowPosCallback callback)							{ _windowPosCallback = callback; }
	void SetSizeCallback(AnimaEngineWindowSizeCallback callback)						{ _windowSizeCallback = callback; }
	void SetIconifyCallback(AnimaEngineWindowIconifyCallback callback)					{ _windowIconifyCallback = callback; }
	void SetVisibilityCallback(AnimaEngineWindowVisibilityCallback callback)			{ _windowVisibilityCallback = callback; }
	void SetDamageCallback(AnimaEngineWindowDamageCallback callback)					{ _windowDamageCallback = callback; }
	void SetCloseRequestCallback(AnimaEngineWindowCloseRequestCallback callback)		{ _windowCloseRequestCallback = callback; }
	void SetKeyCallback(AnimaEngineWindowKeyCallback callback)							{ _windowKeyCallback = callback; }
	void SetCharCallback(AnimaEngineWindowCharCallback callback)						{ _windowCharCallback = callback; }
	void SetScrollCallback(AnimaEngineWindowScrollCallback callback)					{ _windowScrollCallback = callback; }
	void SetMouseClickCallback(AnimaEngineWindowMouseClickCallback callback)			{ _windowMouseClickCallback = callback; }
	void SetCursorMotionCallback(AnimaEngineWindowCursorMotionCallback callback)		{ _windowCursorMotionCallback = callback; }
	void SetCursorEnterCallback(AnimaEngineWindowCursorEnterCallback callback)			{ _windowCursoEnterCallback = callback; }
	void SetDropCallback(AnimaEngineWindowDropCallback callback)						{ _windowDropCallback = callback; }
	void SetFramebufferSizeCallback(AnimaEngineWindowFramebufferSizeCallback callback)	{ _frameBufferSizeCallback = callback; }
	void SetGetStringiCallback(PFNGLGETSTRINGIPROC callback)							{ _GetStringiCallback = callback; }

	AnimaEngineWindowFocusCallback				GetFocusCallback()				{ return _windowFocusCallback; }
	AnimaEngineWindowPosCallback				GetPosCallback()				{ return _windowPosCallback; }
	AnimaEngineWindowSizeCallback				GetSizeCallback()				{ return _windowSizeCallback; }
	AnimaEngineWindowIconifyCallback			GetIconifyCallback()			{ return _windowIconifyCallback; }
	AnimaEngineWindowVisibilityCallback			GetVisibilityCallback()			{ return _windowVisibilityCallback; }
	AnimaEngineWindowDamageCallback				GetDamageCallback()				{ return _windowDamageCallback; }
	AnimaEngineWindowCloseRequestCallback		GetCloseRequestCallback()		{ return _windowCloseRequestCallback; }
	AnimaEngineWindowKeyCallback				GetKeyCallback()				{ return _windowKeyCallback; }
	AnimaEngineWindowCharCallback				GetCharCallback()				{ return _windowCharCallback; }
	AnimaEngineWindowScrollCallback				GetScrollCallback()				{ return _windowScrollCallback; }
	AnimaEngineWindowMouseClickCallback			GetMouseClickCallback()			{ return _windowMouseClickCallback; }
	AnimaEngineWindowCursorMotionCallback		GetCursorMotionCallback()		{ return _windowCursorMotionCallback; }
	AnimaEngineWindowCursorEnterCallback		GetCursorEnterCallback()		{ return _windowCursoEnterCallback; }
	AnimaEngineWindowDropCallback				GetDropCallback()				{ return _windowDropCallback; }
	AnimaEngineWindowFramebufferSizeCallback	GetFramebufferSizeCallback()	{ return _frameBufferSizeCallback; }
	PFNGLGETSTRINGIPROC							GetGetStringiCallback()			{ return _GetStringiCallback; }

	AnimaEngine* GetEngine()			{ return _engine; }
	void SetEngine(AnimaEngine* e)		{ _engine = e; }

	AnimaEngineWindow_Base* GetNext()		{ return _next; }
	void SetNext(AnimaEngineWindow_Base* n)	{ _next = n; }

	bool IsIconified()		{ return _iconified; }
	bool IsResizable()		{ return _resizable; }
	bool IsDecorated()		{ return _decorated; }
	bool IsAutoIconifiy()	{ return _autoIconify; }
	bool IsFloating()		{ return _floating; }
	bool IsVisible()		{ return _visible; }
	bool IsClosed()			{ return _closed; }

	void SetIconified(bool b)		{ _iconified = b; }
	void SetResizable(bool b)		{ _resizable = b; }
	void SetDecorated(bool b)		{ _decorated = b; }
	void SetAutoIconifiy(bool b)	{ _autoIconify = b; }
	void SetFloating(bool b)		{ _floating = b; }
	void SetVisible(bool b)			{ _visible = b; }
	void SetClosed(bool b)			{ _closed = b; }

	bool IsStickyKeys()			{ return _stickyKeys; }
	bool IsStickyMouseButtons()	{ return _stickyMouseButtons; }

	void SetStickyKeys(bool b)			{ _stickyKeys = b; }
	void SetStickyMouseButtons(bool b)	{ _stickyMouseButtons = b; }

	double GetCursorPosX()	{ return _cursorPosX; }
	double GetCursorPosY()	{ return _cursorPosY; }

	void SetCursorPosX(double d)	{ _cursorPosX = d; }
	void SetCursorPosY(double d)	{ _cursorPosY = d; }

	int GetCursorMode()			{ return _cursorMode; }
	void SetCursorMode(int m)	{ _cursorMode = m; }

	void* GetUserPointer()			{ return _userPointer; }
	void SetUserPointer(void* p)	{ _userPointer = p; }

	AnimaEngineWindowvidmode* GetVideoMode()	{ return &_videoMode; }

	_AnimaEngineWindowmonitor* GetMonitor()		{ return _monitor; }
	_AnimaEngineWindowcursor* GetCursor()		{ return _cursor; }

	AnimaEngineWindowContext* GetContext() { return &_context; }

	void SetMonitor(_AnimaEngineWindowmonitor* m)	{ _monitor = m; }
	void SetCursor(_AnimaEngineWindowcursor* c)		{ _cursor = c;	}

	bool ShouldClose()			{ return _shouldClose;	}
	void SetShouldClose(bool b) { _shouldClose = b;		}

	char* GetMouseButtons() { return _mouseButtons; }
	char* GetKeys()			{ return _keys;			}

	int GetWindowID() { return _windowId; }

	_GETD_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE;
	_GETD_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;

	virtual void InitCallbacks();

protected:
	virtual void InternalInit() 
	{
		_engine = NULL;
		_next = NULL;

		_shouldClose = false;

		_iconified = false;
		_resizable = false;
		_decorated = false;
		_autoIconify = false;
		_floating = false;
		_visible = false;
		_closed = false;
		_userPointer = NULL;

		_monitor = NULL;
		_cursor = NULL;

		_stickyKeys = false;
		_stickyMouseButtons = false;
		_cursorPosX = 0.0;
		_cursorPosY = 0.0;
		_cursorMode = ANIMA_ENGINE_CORE_CURSOR_NORMAL;
		
		_INIT_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE;
		_INIT_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
	}
	
private:
	AnimaEngine*			_engine;
	AnimaEngineWindow_Base*	_next;

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
	bool	_shouldClose;

	AnimaEngineWindowContext _context;

	int			_windowId;
	static int	_windowCount;

#if defined(_ANIMA_ENGINE_CORE_USE_OPENGL)
	PFNGLGETSTRINGIPROC _GetStringiCallback;
#endif

	AnimaEngineWindowFocusCallback				_windowFocusCallback;
	AnimaEngineWindowPosCallback				_windowPosCallback;
	AnimaEngineWindowSizeCallback				_windowSizeCallback;
	AnimaEngineWindowIconifyCallback			_windowIconifyCallback;
	AnimaEngineWindowVisibilityCallback			_windowVisibilityCallback;
	AnimaEngineWindowDamageCallback				_windowDamageCallback;
	AnimaEngineWindowCloseRequestCallback		_windowCloseRequestCallback;
	AnimaEngineWindowKeyCallback				_windowKeyCallback;
	AnimaEngineWindowCharCallback				_windowCharCallback;
	AnimaEngineWindowScrollCallback				_windowScrollCallback;
	AnimaEngineWindowMouseClickCallback			_windowMouseClickCallback;
	AnimaEngineWindowCursorMotionCallback		_windowCursorMotionCallback;
	AnimaEngineWindowCursorEnterCallback		_windowCursoEnterCallback;
	AnimaEngineWindowDropCallback				_windowDropCallback;
	AnimaEngineWindowFramebufferSizeCallback	_frameBufferSizeCallback;

	_ANIMA_ENGINE_CORE_PLATFORM_WINDOW_STATE;
	_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;

private:
	static void AnimaEngineWindow_Base_Focus(AnimaEngineWindow_Base* window, bool focused);
	static void AnimaEngineWindow_Base_Pos(AnimaEngineWindow_Base* window, int xpos, int ypos);
	static void AnimaEngineWindow_Base_Size(AnimaEngineWindow_Base* window, int width, int height);
	static void AnimaEngineWindow_Base_Iconify(AnimaEngineWindow_Base* window, bool iconified);
	static void AnimaEngineWindow_Base_Visibility(AnimaEngineWindow_Base* window, bool visible);
	static void AnimaEngineWindow_Base_Damage(AnimaEngineWindow_Base* window);
	static void AnimaEngineWindow_Base_CloseRequest(AnimaEngineWindow_Base* window);
	static void AnimaEngineWindow_Base_Key(AnimaEngineWindow_Base* window, int key, int scancode, int action, int mods);
	static void AnimaEngineWindow_Base_Char(AnimaEngineWindow_Base* window, unsigned int codepoint, int mods, bool plain);
	static void AnimaEngineWindow_Base_Scroll(AnimaEngineWindow_Base* window, double x, double y);
	static void AnimaEngineWindow_Base_MouseClick(AnimaEngineWindow_Base* window, int button, int action, int mods);
	static void AnimaEngineWindow_Base_CursorMotion(AnimaEngineWindow_Base* window, double x, double y);
	static void AnimaEngineWindow_Base_CursorEnter(AnimaEngineWindow_Base* window, bool entered);
	static void AnimaEngineWindow_Base_Drop(AnimaEngineWindow_Base* window, int count, const char** names);
	static void AnimaEngineWindow_Base_FrameBufferSize(AnimaEngineWindow_Base* window, int width, int height);
};

struct _AnimaEngineWindowmonitor
{
	char*	_name;
	int		_widthMM, _heightMM;

	AnimaEngineWindowvidmode*	_modes;
	int							_modeCount;
	AnimaEngineWindowvidmode	_currentMode;

	AnimaEngineWindowgammaramp	_originalRamp;
	AnimaEngineWindowgammaramp	_currentRamp;

	_ANIMA_ENGINE_CORE_PLATFORM_MONITOR_STATE;
};

struct _AnimaEngineWindowMonitorCallbacks
{ 
	AnimaEngineWindowmonitorfun _monitor; 
};

struct _AnimaEngineWindowcursor
{
	_AnimaEngineWindowcursor* next;

	_ANIMA_ENGINE_CORE_PLATFORM_CURSOR_STATE;
};

struct _AnimaEngineWindowHints {
	int		_redBits;		
	int		_greenBits;		
	int		_blueBits;		
	int		_alphaBits;		
	int		_depthBits;		
	int		_stencilBits;	
	int		_accumRedBits;	
	int		_accumGreenBits;
	int		_accumBlueBits;	
	int		_accumAlphaBits;
	int		_auxBuffers;	
	bool	_stereo;		
	bool	_resizable;		
	bool	_visible;		
	bool	_decorated;		
	bool	_focused;		
	bool	_autoIconify;	
	bool	_floating;		
	int		_samples;		
	bool	_sRGB;			
	int		_refreshRate;	
	bool	_doublebuffer;	
	int		_api;			
	int		_major;			
	int		_minor;			
	bool	_forward;		
	bool	_debug;			
	int		_profile;		
	int		_robustness;	
	int		_release;		
};

// Elenco dichiarazioni funzioni di utilizzo specifiche per piattaforma
extern "C" ANIMA_ENGINE_CORE_EXPORT bool						_AnimaEngineWindowPlatformInit(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformTerminate(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT const char*					_AnimaEngineWindowPlatformGetVersionString(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetCursorPos(AnimaEngineWindow_Base* window, double xpos, double ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformApplyCursorMode(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT _AnimaEngineWindowmonitor**	_AnimaEngineWindowPlatformGetMonitors(int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool						_AnimaEngineWindowPlatformIsSameMonitor(_AnimaEngineWindowmonitor* first, _AnimaEngineWindowmonitor* second);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetMonitorPos(_AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindowvidmode*	_AnimaEngineWindowPlatformGetVideoModes(_AnimaEngineWindowmonitor* monitor, int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetVideoMode(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowvidmode* mode);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetGammaRamp(_AnimaEngineWindowmonitor* monitor, AnimaEngineWindowgammaramp* ramp);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetGammaRamp(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowgammaramp* ramp);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetClipboardString(AnimaEngineWindow_Base* window, const char* string);
extern "C" ANIMA_ENGINE_CORE_EXPORT const char*					_AnimaEngineWindowPlatformGetClipboardString(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool						_AnimaEngineWindowPlatformJoystickPresent(int joy);
extern "C" ANIMA_ENGINE_CORE_EXPORT const float*				_AnimaEngineWindowPlatformGetJoystickAxes(int joy, int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT const unsigned char*		_AnimaEngineWindowPlatformGetJoystickButtons(int joy, int* count);
extern "C" ANIMA_ENGINE_CORE_EXPORT const char*					_AnimaEngineWindowPlatformGetJoystickName(int joy);
extern "C" ANIMA_ENGINE_CORE_EXPORT double						_AnimaEngineWindowPlatformGetTime(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetTime(double time);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool						_AnimaEngineWindowPlatformCreateWindow(AnimaEngineWindow_Base* window, const _AnimaEngineWindowwndconfig* wndconfig, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformDestroyWindow(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetWindowTitle(AnimaEngineWindow_Base* window, const char* title);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetWindowPos(AnimaEngineWindow_Base* window, int* xpos, int* ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetWindowPos(AnimaEngineWindow_Base* window, int xpos, int ypos);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetWindowSize(AnimaEngineWindow_Base* window, int* width, int* height);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetWindowSize(AnimaEngineWindow_Base* window, int width, int height);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetFramebufferSize(AnimaEngineWindow_Base* window, int* width, int* height);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformGetWindowFrameSize(AnimaEngineWindow_Base* window, int* left, int* top, int* right, int* bottom);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformIconifyWindow(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformRestoreWindow(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformShowWindow(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformUnhideWindow(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformHideWindow(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformPollEvents(AnimaEngine* engine);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformWaitEvents(AnimaEngine* engine);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformPostEmptyEvent(AnimaEngine* engine);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformMakeContextCurrent(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindow_Base*		_AnimaEngineWindowPlatformGetCurrentContext(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSwapBuffers(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSwapInterval(int interval);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool						_AnimaEngineWindowPlatformExtensionSupported(const char* extension);
extern "C" ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindowglproc		_AnimaEngineWindowPlatformGetProcAddress(const char* procname);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool						_AnimaEngineWindowPlatformCreateCursor(_AnimaEngineWindowcursor* cursor, const AnimaEngineWindowimage* image, int xhot, int yhot);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformDestroyCursor(_AnimaEngineWindowcursor* cursor);
extern "C" ANIMA_ENGINE_CORE_EXPORT void						_AnimaEngineWindowPlatformSetCursor(AnimaEngine* engine, AnimaEngineWindow_Base* window, _AnimaEngineWindowcursor* cursor);

extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowInputMonitorChange(AnimaEngine* engine);

// Elenco funzioni di utilità
extern "C" ANIMA_ENGINE_CORE_EXPORT const AnimaEngineWindowvidmode*		_AnimaEngineWindowChooseVideoMode(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowvidmode* desired);
extern "C" ANIMA_ENGINE_CORE_EXPORT int									_AnimaEngineWindowCompareVideoModes(const AnimaEngineWindowvidmode* first, const AnimaEngineWindowvidmode* second);
extern "C" ANIMA_ENGINE_CORE_EXPORT void								_AnimaEngineWindowSplitBPP(int bpp, int* red, int* green, int* blue);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool								_AnimaEngineWindowStringInExtensionString(const char* string, const GLubyte* extensions);
extern "C" ANIMA_ENGINE_CORE_EXPORT const _AnimaEngineWindowfbconfig*	_AnimaEngineWindowChooseFBConfig(const _AnimaEngineWindowfbconfig* desired, const _AnimaEngineWindowfbconfig* alternatives, unsigned int count);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool								_AnimaEngineWindowRefreshContextAttribs(const _AnimaEngineWindowctxconfig* ctxconfig);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool								_AnimaEngineWindowIsValidContextConfig(const _AnimaEngineWindowctxconfig* ctxconfig);
extern "C" ANIMA_ENGINE_CORE_EXPORT bool								_AnimaEngineWindowIsValidContext(const _AnimaEngineWindowctxconfig* ctxconfig);
extern "C" ANIMA_ENGINE_CORE_EXPORT void								_AnimaEngineWindowAllocGammaArrays(AnimaEngineWindowgammaramp* ramp, unsigned int size);
extern "C" ANIMA_ENGINE_CORE_EXPORT void								_AnimaEngineWindowFreeGammaArrays(AnimaEngineWindowgammaramp* ramp);
extern "C" ANIMA_ENGINE_CORE_EXPORT _AnimaEngineWindowmonitor*			_AnimaEngineWindowAllocMonitor(const char* name, int widthMM, int heightMM);
extern "C" ANIMA_ENGINE_CORE_EXPORT void								_AnimaEngineWindowFreeMonitor(_AnimaEngineWindowmonitor* monitor);
extern "C" ANIMA_ENGINE_CORE_EXPORT void								_AnimaEngineWindowFreeMonitors(_AnimaEngineWindowmonitor** monitors, int count);
extern "C" ANIMA_ENGINE_CORE_EXPORT AnimaEngineWindowglproc				_AnimaEngineWindowGetProcAddress(const char* procname);


END_ANIMA_ENGINE_CORE_NAMESPACE

#endif