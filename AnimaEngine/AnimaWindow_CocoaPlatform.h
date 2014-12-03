#ifndef _AnimaWindow_WindowsPlatform_h
#define _AnimaWindow_WindowsPlatform_h

#include "AnimaEngineCore.h"

#include <stdint.h>

#if defined __OBJC__
#	import <Cocoa/Cocoa.h>
#else
#	include <ApplicationServices/ApplicationServices.h>
	typedef void* id;
#endif

#include "AnimaWindow_CocoaTLS.h"

#if defined _ANIMA_ENGINE_NSGL
#	include "AnimaWindow_NSGLContext.h"
#else
#	error Nessun client selezionato disponibile per la piattaforma
#endif

#include "AnimaWindow_CocoaJoystick.h"

#define _ANIMA_ENGINE_PLATFORM_WINDOW_STATE         _AnimaEngineWindowwindowNS  ns
#define _ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE _AnimaEngineWindowlibraryNS ns
#define _ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE   _AnimaEngineWindowtimeNS    ns_time
#define _ANIMA_ENGINE_PLATFORM_MONITOR_STATE        _AnimaEngineWindowmonitorNS ns
#define _ANIMA_ENGINE_PLATFORM_CURSOR_STATE         _AnimaEngineWindowcursorNS  ns

#define _GETD_ANIMA_ENGINE_PLATFORM_WINDOW_STATE	_AnimaEngineWindowwindowNS* GetPlatformWindowState() { return &ns; }
#define _GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE	GetPlatformWindowState()

#define _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE	_AnimaEngineWindowlibraryNS* GetPlatformLibraryWindowState() { return &ns; }
#define _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE	GetPlatformLibraryWindowState()

#define _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE	_AnimaEngineWindowtimeNS* GetPlatformTimeState() { return &ns_time; }
#define _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE	GetPlatformTimeState()

#define _GETD_ANIMA_ENGINE_PLATFORM_MONITOR_STATE	_AnimaEngineWindowmonitorNS* GetPlatformMonitorState() { return &ns; }
#define _GET_ANIMA_ENGINE_PLATFORM_MONITOR_STATE	GetPlatformMonitorState()

#define _GETD_ANIMA_ENGINE_PLATFORM_CURSOR_STATE	_AnimaEngineWindowcursorNS* GetPlatformCursorState() { return &ns; }
#define _GET_ANIMA_ENGINE_PLATFORM_CURSOR_STATE	GetPlatformCursorState()

#define _INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE(class) _AnimaEngineWindowlibraryNS class::ns;
#define _INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE(class) _AnimaEngineWindowtimeNS class::ns_time;

#define _INIT_ANIMA_ENGINE_PLATFORM_WINDOW_STATE	ns._object			= NULL;		\
														ns._delegate		= NULL;		\
														ns._view			= NULL;		\
														ns._modifierFlags	= 0;		\
														ns._cursorInside	= false;

#define _INIT_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE	ns._eventSource		= NULL;	\
																ns._delegate		= NULL;	\
																ns._autoreleasePool	= NULL;	\
																ns._cursor			= NULL;	\
																ns._clipboardString	= NULL;

#define _INIT_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE		ns_time._resolution	= 0.0001;	\
																ns_time._base		= 0;

#define _INIT_ANIMA_ENGINE_PLATFORM_MONITOR_STATE			ns._displayID		= NULL	\
																ns._previousMode	= NULL	\
																ns._screen			= NULL

#define _INIT_ANIMA_ENGINE_PLATFORM_CURSOR_STATE			ns._object = NULL;

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef struct _AnimaEngineWindowwindowNS
{
	id              _object;
	id	            _delegate;
	id              _view;
	unsigned int    _modifierFlags;
	bool            _cursorInside;
	
} _AnimaEngineWindowwindowNS;

typedef struct _AnimaEngineWindowlibraryNS
{
	CGEventSourceRef	_eventSource;
	id					_delegate;
	id					_autoreleasePool;
	id					_cursor;
	
	char* _clipboardString;
	
} _AnimaEngineWindowlibraryNS;

typedef struct _AnimaEngineWindowmonitorNS
{
	CGDirectDisplayID   _displayID;
	CGDisplayModeRef    _previousMode;
	id                  _screen;
	
} _AnimaEngineWindowmonitorNS;

typedef struct _AnimaEngineWindowcursorNS
{
	id _object;
	
} _AnimaEngineWindowcursorNS;

typedef struct _AnimaEngineWindowtimeNS
{
	double _base;
	double _resolution;
	
} _AnimaEngineWindowtimeNS;


void _AnimaEngineWindowInitTimer(void);

bool _AnimaEngineWindowSetVideoMode(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowvidmode* desired);
void _AnimaEngineWindowRestoreVideoMode(_AnimaEngineWindowmonitor* monitor);

END_ANIMA_ENGINE_NAMESPACE

#endif