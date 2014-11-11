//
//  AnimaWindow_CocoaPlatform.h
//  Anima
//
//  Created by Marco Zille on 11/11/14.
//
//

#ifndef Anima_AnimaWindow_CocoaPlatform_h
#define Anima_AnimaWindow_CocoaPlatform_h

#include <stdint>

#if defined(_OBJC_)
#import <Cocoa/Cocoa.h>
#else
#include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#endif


#define _ANIMA_ENGINE_WS_PLATFORM_WINDOW_STATE         _AnimaEngineWSwindowNS  ns
#define _ANIMA_ENGINE_WS_PLATFORM_LIBRARY_WINDOW_STATE _AnimaEngineWSlibraryNS ns
#define _ANIMA_ENGINE_WS_PLATFORM_LIBRARY_TIME_STATE   _AnimaEngineWStimeNS    ns_time
#define _ANIMA_ENGINE_WS_PLATFORM_MONITOR_STATE        _AnimaEngineWSmonitorNS ns
#define _ANIMA_ENGINE_WS_PLATFORM_CURSOR_STATE         _AnimaEngineWScursorNS  ns

// Dati specifici delle finestre per Cocoa
//
typedef struct _AnimaEngineWSwindowNS
{
	id              object;
	id	            delegate;
	id              view;
	unsigned int    modifierFlags;
	int             cursorInside;
	
} _AnimaEngineWSwindowNS;


// Dati globali specifici per Cocoa
//
typedef struct _AnimaEngineWSlibraryNS
{
	CGEventSourceRef eventSource;
	id              delegate;
	id              autoreleasePool;
	id              cursor;
	
	char*           clipboardString;
	
} _AnimaEngineWSlibraryNS;


// Dati specifici dei monitor per Cocoa
//
typedef struct _AnimaEngineWSmonitorNS
{
	CGDirectDisplayID   displayID;
	CGDisplayModeRef    previousMode;
	id                  screen;
	
} _AnimaEngineWSmonitorNS;


// Dati specifici dei cursori per Cocoa
//
typedef struct _AnimaEngineWScursorNS
{
	id              object;
	
} _AnimaEngineWScursorNS;


// Dati specifici dei timer per Cocoa
//
typedef struct _AnimaEngineWStimeNS
{
	double          base;
	double          resolution;
	
} _AnimaEngineWStimeNS;


void _AnimaEngineWSInitTimer(void);

//GLboolean _AnimaEngineWSSetVideoMode(_AnimaEngineWSmonitor* monitor, const AnimaEngineWSvidmode* desired);
//void _AnimeEngineWSRestoreVideoMode(_AnimaEngineWSmonitor* monitor);

#endif
