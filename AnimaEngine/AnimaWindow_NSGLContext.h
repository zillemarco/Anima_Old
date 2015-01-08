#ifndef _AnimaWindow_WGLContext_h
#define _AnimaWindow_WGLContext_h

#include "AnimaEngineCore.h"

#define _ANIMA_ENGINE_PLATFORM_FBCONFIG
#define _ANIMA_ENGINE_PLATFORM_CONTEXT_STATE            _AnimaEngineWindowcontextNSGL	nsgl
#define _ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE    _AnimaEngineWindowlibraryNSGL	nsgl

#define _GETD_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE	_AnimaEngineWindowcontextNSGL* GetPlatformContextState() { return &nsgl; }
#define _GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE	GetPlatformContextState()

#define _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE	_AnimaEngineWindowlibraryNSGL* GetPlatformLibraryContextState() { return &nsgl; }
#define _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE	GetPlatformLibraryContextState()

#define _INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE(class) _AnimaEngineWindowlibraryNSGL class::nsgl;

#define _INIT_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE	nsgl._pixelFormat	= NULL;		\
														nsgl._context		= NULL;

#define _INIT_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE	nsgl._framework = NULL;

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef struct _AnimaEngineWindowcontextNSGL
{
	id _pixelFormat;
	id _context;
	
} _AnimaEngineWindowcontextNSGL;

typedef struct _AnimaEngineWindowlibraryNSGL
{
	void* _framework;
	
} _AnimaEngineWindowlibraryNSGL;

bool _AnimaEngineWindowInitContextAPI(void);
void _AnimaEngineWindowTerminateContextAPI(void);
bool _AnimaEngineWindowCreateContext(AnimaEngineWindow_Base* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);
void _AnimaEngineWindowDestroyContext(AnimaEngineWindow_Base* window);

extern "C" ANIMA_ENGINE_EXPORT bool	_AnimaEngineWindowInitializeGlewExtensions();

END_ANIMA_ENGINE_NAMESPACE

#endif