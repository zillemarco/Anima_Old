#ifndef _AnimaWindow_WGLContext_h
#define _AnimaWindow_WGLContext_h

#include "AnimaEngineCore.h"
#include <GL\wglext.h>

#define _ANIMA_ENGINE_CORE_PLATFORM_FBCONFIG                 int						wgl
#define _ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE            _AnimaEngineWindowcontextWGL wgl
#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE    _AnimaEngineWindowlibraryWGL wgl

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowcontextWGL
{
	HDC		_dc;
	HGLRC	_context;

	PFNWGLSWAPINTERVALEXTPROC			_SwapIntervalEXT;
	PFNWGLGETPIXELFORMATATTRIBIVARBPROC	_GetPixelFormatAttribivARB;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC	_GetExtensionsStringEXT;
	PFNWGLGETEXTENSIONSSTRINGARBPROC	_GetExtensionsStringARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC	_CreateContextAttribsARB;
	GLboolean							_EXT_swap_control;
	GLboolean							_ARB_multisample;
	GLboolean							_ARB_framebuffer_sRGB;
	GLboolean							_ARB_pixel_format;
	GLboolean							_ARB_create_context;
	GLboolean							_ARB_create_context_profile;
	GLboolean							_EXT_create_context_es2_profile;
	GLboolean							_ARB_create_context_robustness;
	GLboolean							_ARB_context_flush_control;

} _AnimaEngineWindowcontextWGL;

typedef struct _AnimaEngineWindowlibraryWGL
{
	struct {
		HINSTANCE _instance;
	} _opengl32;

} _AnimaEngineWindowlibraryWGL;

bool _AnimaEngineWindowInitContextAPI(void);
void _AnimaEngineWindowTerminateContextAPI(void);
int _AnimaEngineWindowCreateContext(_AnimaEngineWindowwindow* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);

void _AnimaEngineWindowDestroyContext(_AnimaEngineWindowwindow* window);
int _AnimaEngineWindowAnalyzeContext(const _AnimaEngineWindowwindow* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif