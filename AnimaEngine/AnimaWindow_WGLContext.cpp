#include "AnimaWindow.h"
#include "AnimaEngine.h"

#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

static void initWGLExtensions(AnimaEngineWindow_Base* window)
{
	// This needs to include every function pointer loaded below
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_SwapIntervalEXT				= NULL;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetPixelFormatAttribivARB	= NULL;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringARB		= NULL;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringEXT		= NULL;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_CreateContextAttribsARB		= NULL;

	// This needs to include every extension used below except for
	// WGL_ARB_extensions_string and WGL_EXT_extensions_string
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_multisample					= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_framebuffer_sRGB			= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context				= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_profile		= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_EXT_create_context_es2_profile	= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_robustness	= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_EXT_swap_control				= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_pixel_format				= false;
	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_context_flush_control		= false;

	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
	if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringEXT)
	{
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
		if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringARB)
			return;
	}

	if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_multisample"))
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_multisample = true;

	if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_framebuffer_sRGB"))
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_framebuffer_sRGB = true;

	if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_create_context"))
	{
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_CreateContextAttribsARB)
			window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context = true;
	}

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context)
	{
		if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_create_context_profile"))
			window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_profile = true;
	}

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context && window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_profile)
	{
		if (_AnimaEngineWindowPlatformExtensionSupported("WGL_EXT_create_context_es2_profile"))
			window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_EXT_create_context_es2_profile = true;
	}

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context)
	{
		if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_create_context_robustness"))
			window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_robustness = true;
	}

	if (_AnimaEngineWindowPlatformExtensionSupported("WGL_EXT_swap_control"))
	{
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_SwapIntervalEXT)
			window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_EXT_swap_control = true;
	}

	if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_pixel_format"))
	{
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetPixelFormatAttribivARB)
			window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_pixel_format = true;
	}

	if (_AnimaEngineWindowPlatformExtensionSupported("WGL_ARB_context_flush_control"))
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_context_flush_control = true;
}

static bool initGlewExtensions()
{
	if (AnimaEngine::IsGlewExtensionsInitialized())
		return true;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		char str[4096];
		sprintf(str, "Error initializing GLEW: %s\n", glewGetErrorString(error));
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(error));
		return false;
	}

	AnimaEngine::SetGlewExtensionsInitialized(true);
	return true;
}

static int getPixelFormatAttrib(AnimaEngineWindow_Base* window, int pixelFormat, int attrib)
{
	int value = 0;

	if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetPixelFormatAttribivARB(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, pixelFormat, 0, 1, &attrib, &value))
	{
		// NOTE: We should probably handle this error somehow
		return 0;
	}

	return value;
}

static bool choosePixelFormat(AnimaEngineWindow_Base* window, const _AnimaEngineWindowfbconfig* desired, int* result)
{
	_AnimaEngineWindowfbconfig* usableConfigs;
	const _AnimaEngineWindowfbconfig* closest;
	int i, nativeCount, usableCount;

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_pixel_format)
		nativeCount = getPixelFormatAttrib(window, 1, WGL_NUMBER_PIXEL_FORMATS_ARB);
	else
		nativeCount = DescribePixelFormat(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, 1, sizeof(PIXELFORMATDESCRIPTOR), NULL);

	if (!nativeCount)
	{
		//_glfwInputError(GLFW_API_UNAVAILABLE, "WGL: No pixel formats found");
		return false;
	}

	usableConfigs = (_AnimaEngineWindowfbconfig*)calloc(nativeCount, sizeof(_AnimaEngineWindowfbconfig));
	usableCount = 0;

	for (i = 0; i < nativeCount; i++)
	{
		const int n = i + 1;
		_AnimaEngineWindowfbconfig* u = usableConfigs + usableCount;

		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_pixel_format)
		{
			// Get pixel format attributes through WGL_ARB_pixel_format
			if (!getPixelFormatAttrib(window, n, WGL_SUPPORT_OPENGL_ARB) || !getPixelFormatAttrib(window, n, WGL_DRAW_TO_WINDOW_ARB))
				continue;

			if (getPixelFormatAttrib(window, n, WGL_PIXEL_TYPE_ARB) != WGL_TYPE_RGBA_ARB)
				continue;

			if (getPixelFormatAttrib(window, n, WGL_ACCELERATION_ARB) == WGL_NO_ACCELERATION_ARB)
				continue;

			u->_redBits = getPixelFormatAttrib(window, n, WGL_RED_BITS_ARB);
			u->_greenBits = getPixelFormatAttrib(window, n, WGL_GREEN_BITS_ARB);
			u->_blueBits = getPixelFormatAttrib(window, n, WGL_BLUE_BITS_ARB);
			u->_alphaBits = getPixelFormatAttrib(window, n, WGL_ALPHA_BITS_ARB);

			u->_depthBits = getPixelFormatAttrib(window, n, WGL_DEPTH_BITS_ARB);
			u->_stencilBits = getPixelFormatAttrib(window, n, WGL_STENCIL_BITS_ARB);

			u->_accumRedBits = getPixelFormatAttrib(window, n, WGL_ACCUM_RED_BITS_ARB);
			u->_accumGreenBits = getPixelFormatAttrib(window, n, WGL_ACCUM_GREEN_BITS_ARB);
			u->_accumBlueBits = getPixelFormatAttrib(window, n, WGL_ACCUM_BLUE_BITS_ARB);
			u->_accumAlphaBits = getPixelFormatAttrib(window, n, WGL_ACCUM_ALPHA_BITS_ARB);

			u->_auxBuffers = getPixelFormatAttrib(window, n, WGL_AUX_BUFFERS_ARB);

			if (getPixelFormatAttrib(window, n, WGL_STEREO_ARB))
				u->_stereo = true;
			if (getPixelFormatAttrib(window, n, WGL_DOUBLE_BUFFER_ARB))
				u->_doublebuffer = true;

			if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_multisample)
				u->_samples = getPixelFormatAttrib(window, n, WGL_SAMPLES_ARB);

			if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_framebuffer_sRGB)
			{
				if (getPixelFormatAttrib(window, n, WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB))
					u->_sRGB = true;
			}
		}
		else
		{
			PIXELFORMATDESCRIPTOR pfd;

			// Get pixel format attributes through old-fashioned PFDs

			if (!DescribePixelFormat(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
				continue;

			if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW) || !(pfd.dwFlags & PFD_SUPPORT_OPENGL))
				continue;

			if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED) && (pfd.dwFlags & PFD_GENERIC_FORMAT))
				continue;

			if (pfd.iPixelType != PFD_TYPE_RGBA)
				continue;

			u->_redBits = pfd.cRedBits;
			u->_greenBits = pfd.cGreenBits;
			u->_blueBits = pfd.cBlueBits;
			u->_alphaBits = pfd.cAlphaBits;

			u->_depthBits = pfd.cDepthBits;
			u->_stencilBits = pfd.cStencilBits;

			u->_accumRedBits = pfd.cAccumRedBits;
			u->_accumGreenBits = pfd.cAccumGreenBits;
			u->_accumBlueBits = pfd.cAccumBlueBits;
			u->_accumAlphaBits = pfd.cAccumAlphaBits;

			u->_auxBuffers = pfd.cAuxBuffers;

			if (pfd.dwFlags & PFD_STEREO)
				u->_stereo = true;
			if (pfd.dwFlags & PFD_DOUBLEBUFFER)
				u->_doublebuffer = true;
		}

		u->wgl = n;
		usableCount++;
	}

	if (!usableCount)
	{
		//_glfwInputError(GLFW_API_UNAVAILABLE, "WGL: The driver does not appear to support OpenGL");

		free(usableConfigs);
		return false;
	}

	closest = _AnimaEngineWindowChooseFBConfig(desired, usableConfigs, usableCount);
	if (!closest)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "WGL: Failed to find a suitable pixel format");

		free(usableConfigs);
		return false;
	}

	*result = closest->wgl;
	free(usableConfigs);

	return true;
}

bool _AnimaEngineWindowInitializeGlewExtensions()
{
	return initGlewExtensions();
}

bool _AnimaEngineWindowInitContextAPI(void)
{
	if (!_AnimaEngineWindowInitTLS())
		return false;

	AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE->_opengl32._instance = LoadLibraryW(L"opengl32.dll");
	if (!AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE->_opengl32._instance)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to load opengl32.dll");
		return false;
	}
	
	return true;
}

void _AnimaEngineWindowTerminateContextAPI(void)
{
	if (AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE->_opengl32._instance)
		FreeLibrary(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE->_opengl32._instance);

	_AnimaEngineWindowTerminateTLS();
}

#define setWGLattrib(attribName, attribValue)						\
{																	\
    attribs[index++] = attribName;									\
    attribs[index++] = attribValue;									\
    assert((size_t) index < sizeof(attribs) / sizeof(attribs[0]));	\
}

bool _AnimaEngineWindowCreateContext(AnimaEngineWindow_Base* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig)
{
	int attribs[40];
	int pixelFormat = 0;
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC share = NULL;

	if (ctxconfig->_share)
		share = ctxconfig->_share->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context;

	window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc = GetDC(window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_handle);
	if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to retrieve DC for window");
		return false;
	}

	if (!choosePixelFormat(window, fbconfig, &pixelFormat))
		return false;

	if (!DescribePixelFormat(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, pixelFormat, sizeof(pfd), &pfd))
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to retrieve PFD for selected pixel " "format");
		return false;
	}

	if (!SetPixelFormat(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, pixelFormat, &pfd))
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to set selected pixel format");
		return false;
	}

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context)
	{
		int index = 0, mask = 0, flags = 0, strategy = 0;

		if (ctxconfig->_api == ANIMA_ENGINE_OPENGL_API)
		{
			if (ctxconfig->_forward)
				flags |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

			if (ctxconfig->_debug)
				flags |= WGL_CONTEXT_DEBUG_BIT_ARB;

			if (ctxconfig->_profile)
			{
				if (ctxconfig->_profile == ANIMA_ENGINE_OPENGL_CORE_PROFILE)
					mask |= WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
				else if (ctxconfig->_profile == ANIMA_ENGINE_OPENGL_COMPAT_PROFILE)
					mask |= WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			}
		}
		else
			mask |= WGL_CONTEXT_ES2_PROFILE_BIT_EXT;

		if (ctxconfig->_robustness)
		{
			if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_robustness)
			{
				if (ctxconfig->_robustness == ANIMA_ENGINE_NO_RESET_NOTIFICATION)
					strategy = WGL_NO_RESET_NOTIFICATION_ARB;
				else if (ctxconfig->_robustness == ANIMA_ENGINE_LOSE_CONTEXT_ON_RESET)
					strategy = WGL_LOSE_CONTEXT_ON_RESET_ARB;

				flags |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;
			}
		}

		if (ctxconfig->_release)
		{
			if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_context_flush_control)
			{
				if (ctxconfig->_release == ANIMA_ENGINE_RELEASE_BEHAVIOR_NONE)
				{
					setWGLattrib(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB);
				}
				else if (ctxconfig->_release == ANIMA_ENGINE_RELEASE_BEHAVIOR_FLUSH)
				{
					setWGLattrib(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB);
				}
			}
		}

		if (ctxconfig->_major != 1 || ctxconfig->_minor != 0)
		{
			// NOTE: Only request an explicitly versioned context when
			//       necessary, as explicitly requesting version 1.0 does not
			//       always return the highest available version

			setWGLattrib(WGL_CONTEXT_MAJOR_VERSION_ARB, ctxconfig->_major);
			setWGLattrib(WGL_CONTEXT_MINOR_VERSION_ARB, ctxconfig->_minor);
		}

		if (flags)
			setWGLattrib(WGL_CONTEXT_FLAGS_ARB, flags);

		if (mask)
			setWGLattrib(WGL_CONTEXT_PROFILE_MASK_ARB, mask);

		if (strategy)
			setWGLattrib(WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB, strategy);

		setWGLattrib(0, 0);

		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context = window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_CreateContextAttribsARB(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, share, attribs);
		if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context)
		{
			//_glfwInputError(GLFW_VERSION_UNAVAILABLE, "WGL: Failed to create OpenGL context");
			return false;
		}
	}
	else
	{
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context = wglCreateContext(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc);
		if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context)
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "WGL: Failed to create OpenGL context");
			return false;
		}

		if (share)
		{
			if (!wglShareLists(share, window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context))
			{
				//_glfwInputError(GLFW_PLATFORM_ERROR, "WGL: Failed to enable sharing with specified " "OpenGL context");
				return false;
			}
		}
	}

	_AnimaEngineWindowPlatformMakeContextCurrent(window);
	initWGLExtensions(window);

	return true;
}

void _AnimaEngineWindowDestroyContext(AnimaEngineWindow_Base* window)
{
	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context)
	{
		wglDeleteContext(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context);
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context = NULL;
	}

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc)
	{
		ReleaseDC(window->_GET_ANIMA_ENGINE_PLATFORM_WINDOW_STATE->_handle, window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc);
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc = NULL;
	}
}

int _AnimaEngineWindowAnalyzeContext(AnimaEngineWindow_Base* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig)
{
	bool required = false;

	if (ctxconfig->_api == ANIMA_ENGINE_OPENGL_API)
	{
		if (ctxconfig->_forward)
		{
			if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context)
			{
				//_glfwInputError(GLFW_VERSION_UNAVAILABLE, "WGL: A forward compatible OpenGL context " "requested but WGL_ARB_create_context is " "unavailable");
				return _ANIMA_ENGINE_RECREATION_IMPOSSIBLE;
			}

			required = true;
		}

		if (ctxconfig->_profile)
		{
			if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_profile)
			{
				//_glfwInputError(GLFW_VERSION_UNAVAILABLE, "WGL: OpenGL profile requested but " "WGL_ARB_create_context_profile is unavailable");
				return _ANIMA_ENGINE_RECREATION_IMPOSSIBLE;
			}

			required = true;
		}

		if (ctxconfig->_release)
		{
			if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_context_flush_control)
				required = true;
		}
	}
	else
	{
		if (!window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context || !window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context_profile || !window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_EXT_create_context_es2_profile)
		{
			//_glfwInputError(GLFW_API_UNAVAILABLE, "WGL: OpenGL ES requested but " "WGL_ARB_create_context_es2_profile is unavailable");
			return _ANIMA_ENGINE_RECREATION_IMPOSSIBLE;
		}

		required = true;
	}

	if (ctxconfig->_major != 1 || ctxconfig->_minor != 0)
	{
		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context)
			required = true;
	}

	if (ctxconfig->_debug)
	{
		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_create_context)
			required = true;
	}

	if (fbconfig->_samples > 0)
	{
		// We want FSAA, but can we get it?
		// FSAA is not a hard constraint, so otherwise we just don't care

		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_multisample && window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_pixel_format)
		{
			// We appear to have both the extension and the means to ask for it
			required = true;
		}
	}

	if (fbconfig->_sRGB)
	{
		// We want sRGB, but can we get it?
		// sRGB is not a hard constraint, so otherwise we just don't care

		if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_framebuffer_sRGB && window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_ARB_pixel_format)
		{
			// We appear to have both the extension and the means to ask for it
			required = true;
		}
	}

	if (required)
		return _ANIMA_ENGINE_RECREATION_REQUIRED;

	return _ANIMA_ENGINE_RECREATION_NOT_NEEDED;
}


void _AnimaEngineWindowPlatformMakeContextCurrent(AnimaEngineWindow_Base* window)
{
	if (window)
		wglMakeCurrent(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc, window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context);
	else
		wglMakeCurrent(NULL, NULL);

	_AnimaEngineWindowSetCurrentContext(window);
}

void _AnimaEngineWindowPlatformSwapBuffers(AnimaEngineWindow_Base* window)
{
	SwapBuffers(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc);
}

void _AnimaEngineWindowPlatformSwapInterval(int interval)
{
	AnimaEngineWindow_Base* window = _AnimaEngineWindowPlatformGetCurrentContext();

#if !defined(_ANIMA_ENGINE_USE_DWM_SWAP_INTERVAL)
	if (_AnimaEngineWindowIsCompositionEnabled() && interval)
	{
		// Don't enabled vsync when desktop compositing is enabled, as it leads
		// to frame jitter
		return;
	}
#endif

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_EXT_swap_control)
		window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_SwapIntervalEXT(interval);
}

bool _AnimaEngineWindowPlatformExtensionSupported(const char* extension)
{
	const GLubyte* extensions;

	AnimaEngineWindow_Base* window = _AnimaEngineWindowPlatformGetCurrentContext();

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringEXT != NULL)
	{
		extensions = (GLubyte*)window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringEXT();
		if (extensions != NULL)
		{
			if (_AnimaEngineWindowStringInExtensionString(extension, extensions))
				return true;
		}
	}

	if (window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringARB != NULL)
	{
		extensions = (GLubyte*)window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_GetExtensionsStringARB(window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_dc);
		if (extensions != NULL)
		{
			if (_AnimaEngineWindowStringInExtensionString(extension, extensions))
				return true;
		}
	}

	return false;
}

AnimaEngineWindowglproc _AnimaEngineWindowPlatformGetProcAddress(const char* procname)
{
	const AnimaEngineWindowglproc proc = (AnimaEngineWindowglproc)wglGetProcAddress(procname);
	if (proc)
		return proc;

	return (AnimaEngineWindowglproc)GetProcAddress(AnimaEngine::_GET_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE->_opengl32._instance, procname);
}

HGLRC AnimaEngineWindowGetWGLContext(AnimaEngineWindow_Base* handle)
{
	AnimaEngineWindow_Base* window = (AnimaEngineWindow_Base*)handle;
	_ANIMA_ENGINE_REQUIRE_INIT_OR_RETURN(NULL);

	return window->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context;
}

END_ANIMA_ENGINE_NAMESPACE
