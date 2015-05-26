#include "AnimaGC.h"

#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#define ANIMA_GC_DONT_CARE	-1

#if defined _WIN32
	HINSTANCE AnimaGC::_OpenGL32Instance = NULL;
	PFNWGLSWAPINTERVALEXTPROC AnimaGC::_SwapIntervalEXT = NULL;
	PFNWGLGETPIXELFORMATATTRIBIVARBPROC AnimaGC::_GetPixelFormatAttribivARB = NULL;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC AnimaGC::_GetExtensionsStringEXT = NULL;
	PFNWGLGETEXTENSIONSSTRINGARBPROC AnimaGC::_GetExtensionsStringARB = NULL;
	PFNWGLCREATECONTEXTATTRIBSARBPROC AnimaGC::_CreateContextAttribsARB = NULL;
	bool AnimaGC::_EXT_swap_control = false;
	bool AnimaGC::_ARB_multisample = false;
	bool AnimaGC::_ARB_framebuffer_sRGB = false;
	bool AnimaGC::_ARB_pixel_format = false;
	bool AnimaGC::_ARB_create_context = false;
	bool AnimaGC::_ARB_create_context_profile = false;
	bool AnimaGC::_EXT_create_context_es2_profile = false;
	bool AnimaGC::_ARB_create_context_robustness = false;
	bool AnimaGC::_ARB_context_flush_control = false;
	bool AnimaGC::_GLWExtensionsLoaded = false;
	bool AnimaGC::_GLEWExtensionsLoaded = false;
	bool AnimaGC::_contextAPIsInitialized = false;


#define setWGLattrib(attribName, attribValue)								\
		{																	\
			attribs[index++] = attribName;									\
			attribs[index++] = attribValue;									\
			assert((size_t) index < sizeof(attribs) / sizeof(attribs[0]));	\
		}
#endif

AnimaGC::AnimaGC()
{
	#if defined _WIN32
		_hWnd = NULL;
		_hDC = NULL;	
		_hContext = NULL;
	#else
	#endif
}

AnimaGC::~AnimaGC()
{
	DestroyContext(this);
}

void AnimaGC::MakeCurrent()
{
	#if defined _WIN32
		wglMakeCurrent(_hDC, _hContext);
	#else
	#endif
	//_AnimaEngineWindowSetCurrentContext(window);
}

void AnimaGC::SwapBuffers()
{
	#if defined _WIN32
		::SwapBuffers(_hDC);
	#else
	#endif
}

void AnimaGC::ClearColor(AFloat r, AFloat g, AFloat b, AFloat a)
{
	if (!_GLEWExtensionsLoaded)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(r, g, b, a);
}

void AnimaGC::SetSwapInterval(AInt interval)
{
	if (_EXT_swap_control)
		_SwapIntervalEXT(interval);
}

const AnimaGCFrameBufferConfig* AnimaGC::ChooseFrameBufferConfig(const AnimaGCFrameBufferConfig* desired, const AnimaGCFrameBufferConfig* alternatives, unsigned int count)
{
	unsigned int i;
	unsigned int missing, leastMissing = UINT_MAX;
	unsigned int colorDiff, leastColorDiff = UINT_MAX;
	unsigned int extraDiff, leastExtraDiff = UINT_MAX;
	const AnimaGCFrameBufferConfig* current;
	const AnimaGCFrameBufferConfig* closest = NULL;

	for (i = 0; i < count; i++)
	{
		current = alternatives + i;

		if (desired->_stereo && current->_stereo)
		{
			// Stereo is a hard constraint
			continue;
		}

		if (desired->_doublebuffer != current->_doublebuffer)
		{
			// Double buffering is a hard constraint
			continue;
		}

		// Count number of missing buffers
		{
			missing = 0;

			if (desired->_alphaBits > 0 && current->_alphaBits == 0)
				missing++;

			if (desired->_depthBits > 0 && current->_depthBits == 0)
				missing++;

			if (desired->_stencilBits > 0 && current->_stencilBits == 0)
				missing++;

			if (desired->_auxBuffers > 0 && current->_auxBuffers < desired->_auxBuffers)
			{
				missing += desired->_auxBuffers - current->_auxBuffers;
			}

			if (desired->_samples > 0 && current->_samples == 0)
			{
				// Technically, several multisampling buffers could be
				// involved, but that's a lower level implementation detail and
				// not important to us here, so we count them as one
				missing++;
			}
		}

		// These polynomials make many small channel size differences matter
		// less than one large channel size difference

		// Calculate color channel size difference value
		{
			colorDiff = 0;

			if (desired->_redBits != ANIMA_GC_DONT_CARE)
			{
				colorDiff += (desired->_redBits - current->_redBits) * (desired->_redBits - current->_redBits);
			}

			if (desired->_greenBits != ANIMA_GC_DONT_CARE)
			{
				colorDiff += (desired->_greenBits - current->_greenBits) * (desired->_greenBits - current->_greenBits);
			}

			if (desired->_blueBits != ANIMA_GC_DONT_CARE)
			{
				colorDiff += (desired->_blueBits - current->_blueBits) * (desired->_blueBits - current->_blueBits);
			}
		}

		// Calculate non-color channel size difference value
		{
			extraDiff = 0;

			if (desired->_alphaBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_alphaBits - current->_alphaBits) * (desired->_alphaBits - current->_alphaBits);
			}

			if (desired->_depthBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_depthBits - current->_depthBits) * (desired->_depthBits - current->_depthBits);
			}

			if (desired->_stencilBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_stencilBits - current->_stencilBits) * (desired->_stencilBits - current->_stencilBits);
			}

			if (desired->_accumRedBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_accumRedBits - current->_accumRedBits) * (desired->_accumRedBits - current->_accumRedBits);
			}

			if (desired->_accumGreenBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_accumGreenBits - current->_accumGreenBits) * (desired->_accumGreenBits - current->_accumGreenBits);
			}

			if (desired->_accumBlueBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_accumBlueBits - current->_accumBlueBits) * (desired->_accumBlueBits - current->_accumBlueBits);
			}

			if (desired->_accumAlphaBits != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_accumAlphaBits - current->_accumAlphaBits) * (desired->_accumAlphaBits - current->_accumAlphaBits);
			}

			if (desired->_samples != ANIMA_GC_DONT_CARE)
			{
				extraDiff += (desired->_samples - current->_samples) * (desired->_samples - current->_samples);
			}

			if (desired->_sRGB && !current->_sRGB)
				extraDiff++;
		}

		// Figure out if the current one is better than the best one found so far
		// Least number of missing buffers is the most important heuristic,
		// then color buffer size match and lastly size match for other buffers

		if (missing < leastMissing)
			closest = current;
		else if (missing == leastMissing)
		{
			if ((colorDiff < leastColorDiff) || (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
			{
				closest = current;
			}
		}

		if (current == closest)
		{
			leastMissing = missing;
			leastColorDiff = colorDiff;
			leastExtraDiff = extraDiff;
		}
	}

	return closest;
}

bool AnimaGC::StringInExtensionString(const char* string, const GLubyte* extensions)
{
	const GLubyte* start;
	GLubyte* where;
	GLubyte* terminator;

	// It takes a bit of care to be fool-proof about parsing the
	// OpenGL extensions string. Don't be fooled by sub-strings,
	// etc.
	start = extensions;
	for (;;)
	{
		where = (GLubyte*)strstr((const char*)start, string);
		if (!where)
			return false;

		terminator = where + strlen(string);
		if (where == start || *(where - 1) == ' ')
		{
			if (*terminator == ' ' || *terminator == '\0')
				break;
		}

		start = terminator;
	}

	return true;
}

AnimaGCContextConfig AnimaGC::GetDefaultContextConfig()
{
	AnimaGCContextConfig ccfg;

	ccfg._api = ANIMAGC_OPENGL_API;
	ccfg._major = 1;
	ccfg._minor = 0;
	ccfg._forward = false;
	ccfg._debug = false;
	ccfg._profile = ANIMAGC_OPENGL_ANY_PROFILE;
	ccfg._robustness = ANIMAGC_NO_ROBUSTNESS;
	ccfg._release = ANIMAGC_ANY_RELEASE_BEHAVIOR;

	return ccfg;
}

AnimaGCFrameBufferConfig AnimaGC::GetDefaultFrameBufferConfig()
{
	AnimaGCFrameBufferConfig fbcfg;

	fbcfg._redBits = 8;
	fbcfg._greenBits = 8;
	fbcfg._blueBits = 8;
	fbcfg._alphaBits = 8;
	fbcfg._depthBits = 24;
	fbcfg._stencilBits = 8;
	fbcfg._accumRedBits = 0;
	fbcfg._accumGreenBits = 0;
	fbcfg._accumBlueBits = 0;
	fbcfg._accumAlphaBits = 0;
	fbcfg._auxBuffers = 0;
	fbcfg._stereo = false;
	fbcfg._samples = 0;
	fbcfg._sRGB = false;
	fbcfg._doublebuffer = true;

	#if defined _WIN32
		fbcfg._wgl = 0;
	#endif

	return fbcfg;
}

#if defined _WIN32
	WCHAR* WideStringFromUTF8(const char* source)
	{
		WCHAR* target;
		int length;

		length = MultiByteToWideChar(CP_UTF8, 0, source, -1, NULL, 0);
		if (!length)
			return NULL;

		target = (WCHAR*)calloc(length, sizeof(WCHAR));

		if (!MultiByteToWideChar(CP_UTF8, 0, source, -1, target, length))
		{
			free(target);
			return NULL;
		}

		return target;
	}

	static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	bool AnimaGC::InitializeWGLExtensions()
	{
		if (_GLWExtensionsLoaded == true)
			return true;

		// This needs to include every function pointer loaded below
		_SwapIntervalEXT = NULL;
		_GetPixelFormatAttribivARB = NULL;
		_GetExtensionsStringARB = NULL;
		_GetExtensionsStringEXT = NULL;
		_CreateContextAttribsARB = NULL;

		// This needs to include every extension used below except for
		// WGL_ARB_extensions_string and WGL_EXT_extensions_string
		_ARB_multisample = false;
		_ARB_framebuffer_sRGB = false;
		_ARB_create_context = false;
		_ARB_create_context_profile = false;
		_EXT_create_context_es2_profile = false;
		_ARB_create_context_robustness = false;
		_EXT_swap_control = false;
		_ARB_pixel_format = false;
		_ARB_context_flush_control = false;

		DWORD dwStyle = 0;// WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
		DWORD dwExStyle = 0;// WS_EX_APPWINDOW;
		HWND hWnd = NULL;
		HDC hDC = NULL;
		HGLRC hGLRC = NULL;
		PIXELFORMATDESCRIPTOR pixelFormat;

		WNDCLASSW wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)windowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(void*) + sizeof(int);
		wc.hInstance = GetModuleHandleW(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"AnimaGCTempWindow";
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);

		if (!RegisterClassW(&wc))
			return false;

		WCHAR* wideTitle = WideStringFromUTF8("AnimaGCTempWindow");
		hWnd = CreateWindowExW(dwExStyle, L"AnimaGCTempWindow", wideTitle, dwStyle, 0, 0, 0, 0, NULL, NULL, GetModuleHandleW(NULL), NULL);
		free(wideTitle);

		if (!hWnd)
		{
			UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));
			return false;
		}

		hDC = GetDC(hWnd);

		if (!hDC)
		{
			DestroyWindow(hWnd);
			UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));
			return false;
		}

		if (SetPixelFormat(hDC, 1, &pixelFormat) != 1)
		{
			ReleaseDC(hWnd, hDC);
			DestroyWindow(hWnd);
			UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));
			return false;
		}

		hGLRC = wglCreateContext(hDC);
		if (!hDC)
		{
			ReleaseDC(hWnd, hDC);
			DestroyWindow(hWnd);
			UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));
			return false;
		}

		if (wglMakeCurrent(hDC, hGLRC) != 1)
		{
			wglDeleteContext(hGLRC);
			ReleaseDC(hWnd, hDC);
			DestroyWindow(hWnd);
			UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));
			return false;
		}

		_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
		if (!_GetExtensionsStringEXT)
		{
			_GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
			if (!_GetExtensionsStringARB)
			{
				wglDeleteContext(hGLRC);
				ReleaseDC(hWnd, hDC);
				DestroyWindow(hWnd);
				UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));
				return false;
			}
		}

		if (IsExtensionSupported("WGL_ARB_multisample", hDC))
			_ARB_multisample = true;

		if (IsExtensionSupported("WGL_ARB_framebuffer_sRGB", hDC))
			_ARB_framebuffer_sRGB = true;

		if (IsExtensionSupported("WGL_ARB_create_context", hDC))
		{
			_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			if (_CreateContextAttribsARB)
				_ARB_create_context = true;
		}

		if (_ARB_create_context)
		{
			if (IsExtensionSupported("WGL_ARB_create_context_profile", hDC))
				_ARB_create_context_profile = true;
		}

		if (_ARB_create_context && _ARB_create_context_profile)
		{
			if (IsExtensionSupported("WGL_EXT_create_context_es2_profile", hDC))
				_EXT_create_context_es2_profile = true;
		}

		if (_ARB_create_context)
		{
			if (IsExtensionSupported("WGL_ARB_create_context_robustness", hDC))
				_ARB_create_context_robustness = true;
		}

		if (IsExtensionSupported("WGL_EXT_swap_control", hDC))
		{
			_SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			if (_SwapIntervalEXT)
				_EXT_swap_control = true;
		}

		if (IsExtensionSupported("WGL_ARB_pixel_format", hDC))
		{
			_GetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
			if (_GetPixelFormatAttribivARB)
				_ARB_pixel_format = true;
		}

		if (IsExtensionSupported("WGL_ARB_context_flush_control", hDC))
			_ARB_context_flush_control = true;


		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hGLRC);
		ReleaseDC(hWnd, hDC);
		DestroyWindow(hWnd);
		UnregisterClassW(L"AnimaGCTempWindow", GetModuleHandleW(NULL));

		_GLWExtensionsLoaded = true;
		return true;
	}

	bool AnimaGC::InitializeGLEWExtensions()
	{
		if (_GLEWExtensionsLoaded)
			return true;

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			return false;

		glewExperimental = GL_TRUE;
		error = glewInit();

		if (error != GLEW_OK)
		{
			char str[4096];
			sprintf(str, "Error initializing GLEW: %s\n", glewGetErrorString(error));
			fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(error));
			return false;
		}


		_GLEWExtensionsLoaded = true;

		error = glGetError();
		if (error != GL_NO_ERROR)
			return false;
		
		return true;
	}

	bool AnimaGC::IsExtensionSupported(const char* extension, HDC hDC)
	{
		const GLubyte* extensions;

		if (_GetExtensionsStringEXT != NULL)
		{
			extensions = (GLubyte*)_GetExtensionsStringEXT();
			if (extensions != NULL)
			{
				if (StringInExtensionString(extension, extensions))
					return true;
			}
		}

		if (_GetExtensionsStringARB != NULL)
		{
			extensions = (GLubyte*)_GetExtensionsStringARB(hDC);
			if (extensions != NULL)
			{
				if (StringInExtensionString(extension, extensions))
					return true;
			}
		}

		return false;
	}
	
	AInt AnimaGC::GetPixelFormatAttrib(AInt pixelFormat, AInt attrib, AnimaGC* gc)
	{
		int value = 0;

		if (!_GetPixelFormatAttribivARB(gc->_hDC, pixelFormat, 0, 1, &attrib, &value))
		{
			// NOTE: We should probably handle this error somehow
			return 0;
		}

		return value;
	}


	bool AnimaGC::ChoosePixelFormat(const AnimaGCFrameBufferConfig* desired, AInt* result, AnimaGC* gc)
	{
		AnimaGCFrameBufferConfig* usableConfigs;
		const AnimaGCFrameBufferConfig* closest;
		int i, nativeCount, usableCount;

		if (_ARB_pixel_format)
			nativeCount = GetPixelFormatAttrib(1, WGL_NUMBER_PIXEL_FORMATS_ARB, gc);
		else
			nativeCount = DescribePixelFormat(gc->_hDC, 1, sizeof(PIXELFORMATDESCRIPTOR), NULL);

		if (!nativeCount)
		{
			//_glfwInputError(GLFW_API_UNAVAILABLE, "WGL: No pixel formats found");
			return false;
		}

		usableConfigs = (AnimaGCFrameBufferConfig*)calloc(nativeCount, sizeof(AnimaGCFrameBufferConfig));
		usableCount = 0;

		for (i = 0; i < nativeCount; i++)
		{
			const int n = i + 1;
			AnimaGCFrameBufferConfig* u = usableConfigs + usableCount;

			if (_ARB_pixel_format)
			{
				// Get pixel format attributes through WGL_ARB_pixel_format
				if (!GetPixelFormatAttrib(n, WGL_SUPPORT_OPENGL_ARB, gc) || !GetPixelFormatAttrib(n, WGL_DRAW_TO_WINDOW_ARB, gc))
					continue;

				if (GetPixelFormatAttrib(n, WGL_PIXEL_TYPE_ARB, gc) != WGL_TYPE_RGBA_ARB)
					continue;

				if (GetPixelFormatAttrib(n, WGL_ACCELERATION_ARB, gc) == WGL_NO_ACCELERATION_ARB)
					continue;

				u->_redBits = GetPixelFormatAttrib(n, WGL_RED_BITS_ARB, gc);
				u->_greenBits = GetPixelFormatAttrib(n, WGL_GREEN_BITS_ARB, gc);
				u->_blueBits = GetPixelFormatAttrib(n, WGL_BLUE_BITS_ARB, gc);
				u->_alphaBits = GetPixelFormatAttrib(n, WGL_ALPHA_BITS_ARB, gc);

				u->_depthBits = GetPixelFormatAttrib(n, WGL_DEPTH_BITS_ARB, gc);
				u->_stencilBits = GetPixelFormatAttrib(n, WGL_STENCIL_BITS_ARB, gc);

				u->_accumRedBits = GetPixelFormatAttrib(n, WGL_ACCUM_RED_BITS_ARB, gc);
				u->_accumGreenBits = GetPixelFormatAttrib(n, WGL_ACCUM_GREEN_BITS_ARB, gc);
				u->_accumBlueBits = GetPixelFormatAttrib(n, WGL_ACCUM_BLUE_BITS_ARB, gc);
				u->_accumAlphaBits = GetPixelFormatAttrib(n, WGL_ACCUM_ALPHA_BITS_ARB, gc);

				u->_auxBuffers = GetPixelFormatAttrib(n, WGL_AUX_BUFFERS_ARB, gc);

				if (GetPixelFormatAttrib(n, WGL_STEREO_ARB, gc))
					u->_stereo = true;
				if (GetPixelFormatAttrib(n, WGL_DOUBLE_BUFFER_ARB, gc))
					u->_doublebuffer = true;

				if (_ARB_multisample)
					u->_samples = GetPixelFormatAttrib(n, WGL_SAMPLES_ARB, gc);

				if (_ARB_framebuffer_sRGB)
				{
					if (GetPixelFormatAttrib(n, WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, gc))
						u->_sRGB = true;
				}
			}
			else
			{
				PIXELFORMATDESCRIPTOR pfd;

				// Get pixel format attributes through old-fashioned PFDs

				if (!DescribePixelFormat(gc->_hDC, n, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
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

			u->_wgl = n;
			usableCount++;
		}

		if (!usableCount)
		{
			//_glfwInputError(GLFW_API_UNAVAILABLE, "WGL: The driver does not appear to support OpenGL");

			free(usableConfigs);
			return false;
		}

		closest = ChooseFrameBufferConfig(desired, usableConfigs, usableCount);
		if (!closest)
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "WGL: Failed to find a suitable pixel format");

			free(usableConfigs);
			return false;
		}

		*result = closest->_wgl;
		free(usableConfigs);

		return true;
	}

	bool AnimaGC::InitializeContextAPIs()
	{
		if (_contextAPIsInitialized)
			return true;

		_OpenGL32Instance = LoadLibraryW(L"opengl32.dll");
		if (!_OpenGL32Instance)
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to load opengl32.dll");
			return false;
		}

		_contextAPIsInitialized = true;
		return true;
	}

	void AnimaGC::TerminateContextAPIs(void)
	{
		if (!_contextAPIsInitialized)
			return;

		if (_OpenGL32Instance)
			FreeLibrary(_OpenGL32Instance);
	}

	AnimaGC* AnimaGC::CreateContext(void* windowId, const AnimaGCContextConfig* ctxconfig, const AnimaGCFrameBufferConfig* fbconfig)
	{
		HWND hWnd = *((HWND*)windowId);

		HDC hDC = ::GetDC(hWnd);

		if (!InitializeContextAPIs())
		{
			MessageBox(NULL, "No context api", "", MB_OK);
			return nullptr;
		}

		if (!InitializeWGLExtensions())
		{
			MessageBox(NULL, "No GLW", "", MB_OK);
			return nullptr;
		}
		
		AnimaGC* newGC = new AnimaGC();
		newGC->_hWnd = hWnd;

		int attribs[40];
		int pixelFormat = 0;
		PIXELFORMATDESCRIPTOR pfd;

		newGC->_hDC = GetDC(hWnd);
		if (!newGC->_hDC)
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to retrieve DC for window");
			delete newGC;
			return nullptr;
		}

		if (!ChoosePixelFormat(fbconfig, &pixelFormat, newGC))
		{
			delete newGC;
			return nullptr;
		}

		if (!DescribePixelFormat(newGC->_hDC, pixelFormat, sizeof(pfd), &pfd))
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to retrieve PFD for selected pixel " "format");
			delete newGC;
			return nullptr;
		}

		if (!SetPixelFormat(newGC->_hDC, pixelFormat, &pfd))
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to set selected pixel format");
			delete newGC;
			return nullptr;
		}

		if (_ARB_create_context)
		{
			int index = 0, mask = 0, flags = 0, strategy = 0;

			if (ctxconfig->_api == ANIMAGC_OPENGL_API)
			{
				if (ctxconfig->_forward)
					flags |= WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

				if (ctxconfig->_debug)
					flags |= WGL_CONTEXT_DEBUG_BIT_ARB;

				if (ctxconfig->_profile)
				{
					if (ctxconfig->_profile == ANIMAGC_OPENGL_CORE_PROFILE)
						mask |= WGL_CONTEXT_CORE_PROFILE_BIT_ARB;
					else if (ctxconfig->_profile == ANIMAGC_OPENGL_COMPAT_PROFILE)
						mask |= WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
				}
			}
			else
				mask |= WGL_CONTEXT_ES2_PROFILE_BIT_EXT;

			if (ctxconfig->_robustness)
			{
				if (_ARB_create_context_robustness)
				{
					if (ctxconfig->_robustness == ANIMAGC_NO_RESET_NOTIFICATION)
						strategy = WGL_NO_RESET_NOTIFICATION_ARB;
					else if (ctxconfig->_robustness == ANIMAGC_LOSE_CONTEXT_ON_RESET)
						strategy = WGL_LOSE_CONTEXT_ON_RESET_ARB;

					flags |= WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB;
				}
			}

			if (ctxconfig->_release)
			{
				if (_ARB_context_flush_control)
				{
					if (ctxconfig->_release == ANIMAGC_RELEASE_BEHAVIOR_NONE)
					{
						setWGLattrib(WGL_CONTEXT_RELEASE_BEHAVIOR_ARB, WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB);
					}
					else if (ctxconfig->_release == ANIMAGC_RELEASE_BEHAVIOR_FLUSH)
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

			newGC->_hContext = _CreateContextAttribsARB(newGC->_hDC, nullptr, attribs);
			if (!newGC->_hContext)
			{
				//_glfwInputError(GLFW_VERSION_UNAVAILABLE, "WGL: Failed to create OpenGL context");
				delete newGC;
				return nullptr;
			}
		}
		else
		{
			newGC->_hContext = wglCreateContext(newGC->_hDC);
			if (!newGC->_hContext)
			{
				//_glfwInputError(GLFW_PLATFORM_ERROR, "WGL: Failed to create OpenGL context");
				delete newGC;
				return nullptr;
			}
		}

		newGC->MakeCurrent();
		InitializeGLEWExtensions();

		return newGC;
	}

	void AnimaGC::DestroyContext(AnimaGC* context)
	{
		if (context->_hContext)
		{
			wglDeleteContext(context->_hContext);
			context->_hContext = NULL;
		}

		if (context->_hDC)
		{
			ReleaseDC(context->_hWnd, context->_hDC);
			context->_hDC = NULL;
		}
	}

	AInt AnimaGC::AnalyzeContext(const AnimaGCContextConfig* ctxconfig, const AnimaGCFrameBufferConfig* fbconfig)
	{
		bool required = false;

		if (ctxconfig->_api == ANIMAGC_OPENGL_API)
		{
			if (ctxconfig->_forward)
			{
				if (!_ARB_create_context)
				{
					//_glfwInputError(GLFW_VERSION_UNAVAILABLE, "WGL: A forward compatible OpenGL context " "requested but WGL_ARB_create_context is " "unavailable");
					return ANIMAGC_RECREATION_IMPOSSIBLE;
				}

				required = true;
			}

			if (ctxconfig->_profile)
			{
				if (!_ARB_create_context_profile)
				{
					//_glfwInputError(GLFW_VERSION_UNAVAILABLE, "WGL: OpenGL profile requested but " "WGL_ARB_create_context_profile is unavailable");
					return ANIMAGC_RECREATION_IMPOSSIBLE;
				}

				required = true;
			}

			if (ctxconfig->_release)
			{
				if (_ARB_context_flush_control)
					required = true;
			}
		}
		else
		{
			if (!_ARB_create_context || !_ARB_create_context_profile || !_EXT_create_context_es2_profile)
			{
				//_glfwInputError(GLFW_API_UNAVAILABLE, "WGL: OpenGL ES requested but " "WGL_ARB_create_context_es2_profile is unavailable");
				return ANIMAGC_RECREATION_IMPOSSIBLE;
			}

			required = true;
		}

		if (ctxconfig->_major != 1 || ctxconfig->_minor != 0)
		{
			if (_ARB_create_context)
				required = true;
		}

		if (ctxconfig->_debug)
		{
			if (_ARB_create_context)
				required = true;
		}

		if (fbconfig->_samples > 0)
		{
			// We want FSAA, but can we get it?
			// FSAA is not a hard constraint, so otherwise we just don't care

			if (_ARB_multisample && _ARB_pixel_format)
			{
				// We appear to have both the extension and the means to ask for it
				required = true;
			}
		}

		if (fbconfig->_sRGB)
		{
			// We want sRGB, but can we get it?
			// sRGB is not a hard constraint, so otherwise we just don't care

			if (_ARB_framebuffer_sRGB && _ARB_pixel_format)
			{
				// We appear to have both the extension and the means to ask for it
				required = true;
			}
		}

		if (required)
			return ANIMAGC_RECREATION_REQUIRED;

		return ANIMAGC_RECREATION_NOT_NEEDED;
	}

	AnimaGCGLproc AnimaGC::GetProcAddress(const char* procname)
	{
		const AnimaGCGLproc proc = (AnimaGCGLproc)wglGetProcAddress(procname);
		if (proc)
			return proc;

		return (AnimaGCGLproc)::GetProcAddress(_OpenGL32Instance, procname);
	}

	HGLRC AnimaGC::GetContext()
	{
		return _hContext;
	}

	bool AnimaGC::CheckIntegrity(HWND hWnd)
	{
		return true;
	}
	
#else
#endif

END_ANIMA_ENGINE_NAMESPACE