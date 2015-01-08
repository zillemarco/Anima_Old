#ifndef _AnimaWindow_WGLContext_h
#define _AnimaWindow_WGLContext_h

#include "AnimaEngineCore.h"
#include <GL/wglew.h>

#define _ANIMA_ENGINE_PLATFORM_FBCONFIG                 int							wgl
#define _ANIMA_ENGINE_PLATFORM_CONTEXT_STATE            _AnimaEngineWindowcontextWGL	wgl
#define _ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE    _AnimaEngineWindowlibraryWGL	wgl

#define _GETD_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE	_AnimaEngineWindowcontextWGL* GetPlatformContextState() { return &wgl; }
#define _GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE	GetPlatformContextState()

#define _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE	_AnimaEngineWindowlibraryWGL* GetPlatformLibraryContextState() { return &wgl; }
#define _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE	GetPlatformLibraryContextState()

#define _INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE(class) _AnimaEngineWindowlibraryWGL class::wgl;

#define _INIT_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE			wgl._dc								= NULL;		\
																wgl._context						= NULL;		\
																wgl._SwapIntervalEXT				= NULL;		\
																wgl._GetPixelFormatAttribivARB		= NULL;		\
																wgl._GetExtensionsStringEXT			= NULL;		\
																wgl._GetExtensionsStringARB			= NULL;		\
																wgl._CreateContextAttribsARB		= NULL;		\
																wgl._EXT_swap_control				= false;	\
																wgl._ARB_multisample				= false;	\
																wgl._ARB_framebuffer_sRGB			= false;	\
																wgl._ARB_pixel_format				= false;	\
																wgl._ARB_create_context				= false;	\
																wgl._ARB_create_context_profile		= false;	\
																wgl._EXT_create_context_es2_profile = false;	\
																wgl._ARB_create_context_robustness	= false;	\
																wgl._ARB_context_flush_control		= false;

#define _INIT_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE	wlg._opengl32._instance = NULL;

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef struct _AnimaEngineWindowcontextWGL
{
	HDC		_dc;
	HGLRC	_context;

	PFNWGLSWAPINTERVALEXTPROC			_SwapIntervalEXT;
	PFNWGLGETPIXELFORMATATTRIBIVARBPROC	_GetPixelFormatAttribivARB;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC	_GetExtensionsStringEXT;
	PFNWGLGETEXTENSIONSSTRINGARBPROC	_GetExtensionsStringARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC	_CreateContextAttribsARB;
	bool								_EXT_swap_control;
	bool								_ARB_multisample;
	bool								_ARB_framebuffer_sRGB;
	bool								_ARB_pixel_format;
	bool								_ARB_create_context;
	bool								_ARB_create_context_profile;
	bool								_EXT_create_context_es2_profile;
	bool								_ARB_create_context_robustness;
	bool								_ARB_context_flush_control;

} _AnimaEngineWindowcontextWGL;

typedef struct _AnimaEngineWindowlibraryWGL
{
	struct {
		HINSTANCE _instance;
	} _opengl32;

} _AnimaEngineWindowlibraryWGL;

extern "C" ANIMA_ENGINE_EXPORT bool _AnimaEngineWindowInitContextAPI(void);
extern "C" ANIMA_ENGINE_EXPORT void _AnimaEngineWindowTerminateContextAPI(void);
extern "C" ANIMA_ENGINE_EXPORT bool _AnimaEngineWindowCreateContext(AnimaEngineWindow_Base* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);

extern "C" ANIMA_ENGINE_EXPORT void _AnimaEngineWindowDestroyContext(AnimaEngineWindow_Base* window);
extern "C" ANIMA_ENGINE_EXPORT int _AnimaEngineWindowAnalyzeContext(AnimaEngineWindow_Base* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);

extern "C" ANIMA_ENGINE_EXPORT bool	_AnimaEngineWindowInitializeGlewExtensions();

END_ANIMA_ENGINE_NAMESPACE

#endif