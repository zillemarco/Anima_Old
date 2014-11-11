//
//  AnimaWindow_Base.h
//  Anima
//
//  Created by Marco Zille on 11/11/14.
//
//

#ifndef Anima_AnimaWindow_Base_h
#define Anima_AnimaWindow_Base_h

#include <stdio.h>
#include "AnimaEngineWS.h"
#include "AnimaWindow_Data.h"

BEGIN_ANIMA_ENGINE_WS_NAMESPACE

// struttura dati contenente i dati di una finestra
struct _AnimaEngineWSwindow
{
	struct _AnimaEngineWSwindow* _next;
	
	// Window settings and state
	bool	_iconified;
	bool	_resizable;
	bool	_decorated;
	bool	_autoIconify;
	bool	_floating;
	bool	_visible;
	bool	_closed;
	void*	_userPointer;
	
	AnimaEngineWSvidmode	_videoMode;
	_AnimaEngineWSmonitor*	_monitor;
	_AnimaEngineWScursor*	_cursor;
	
	// Window input state
	bool	_stickyKeys;
	bool	_stickyMouseButtons;
	double	_cursorPosX, _cursorPosY;
	int		_cursorMode;
	char	_mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
	char	_keys[GLFW_KEY_LAST + 1];
	
	// OpenGL extensions and context attributes
	struct {
		int		_api;
		int		_major, _minor, _revision;
		bool	_forward, _debug;
		int		_profile;
		int		_robustness;
		int		_release;
	} _context;
	
#if defined(_GLFW_USE_OPENGL)
	PFNGLGETSTRINGIPROC GetStringi;
#endif
	
	struct {
		AnimaEngineWSwindowposfun        _pos;
		AnimaEngineWSwindowsizefun       _size;
		AnimaEngineWSwindowclosefun      _close;
		AnimaEngineWSwindowrefreshfun    _refresh;
		AnimaEngineWSwindowfocusfun      _focus;
		AnimaEngineWSwindowiconifyfun    _iconify;
		AnimaEngineWSframebuffersizefun  _fbsize;
		AnimaEngineWSmousebuttonfun      _mouseButton;
		AnimaEngineWScursorposfun        _cursorPos;
		AnimaEngineWScursorenterfun      _cursorEnter;
		AnimaEngineWSscrollfun           _scroll;
		AnimaEngineWSkeyfun              _key;
		AnimaEngineWScharfun             _character;
		AnimaEngineWScharmodsfun         _charmods;
		AnimaEngineWSdropfun             _drop;
	} _callbacks;
	
//	// This is defined in the window API's platform.h
//	_GLFW_PLATFORM_WINDOW_STATE;
//	// This is defined in the context API's context.h
//	_GLFW_PLATFORM_CONTEXT_STATE;
};

struct _AnimaEngineWSlibrary
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
	
	double			_cursorPosX, _cursorPosY;
	
	_AnimaEngineWScursor*	_cursorListHead;
	
	_AnimaEngineWSwindow*	_windowListHead;
	_AnimaEngineWSwindow*	_focusedWindow;
	
	_AnimaEngineWSmonitor**	_monitors;
	int				_monitorCount;
	
	struct {
		AnimaEngineWSmonitorfun	_monitor;
	} _callbacks;
	
//	// This is defined in the window API's platform.h
//	_GLFW_PLATFORM_LIBRARY_WINDOW_STATE;
//	// This is defined in the context API's context.h
//	_GLFW_PLATFORM_LIBRARY_CONTEXT_STATE;
//	// This is defined in the platform's time.h
//	_GLFW_PLATFORM_LIBRARY_TIME_STATE;
//	// This is defined in the platform's joystick.h
//	_GLFW_PLATFORM_LIBRARY_JOYSTICK_STATE;
//	// This is defined in the platform's tls.h
//	_GLFW_PLATFORM_LIBRARY_TLS_STATE;
};

END_ANIMA_ENGINE_WS_NAMESPACE

#endif
