#include "AnimaWindow_Base.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

bool _AnimaEngineWindowInitContextAPI(void)
{
	if (!_AnimaEngineWindowInitTLS())
		return false;
	
	_AnimaEngineWindowlibraryNSGL* platformLibraryState = AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;
	platformLibraryState->_framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
	if (platformLibraryState->_framework == NULL)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "NSGL: Failed to locate OpenGL framework");
		return false;
	}
	
	return true;
}

void _AnimaEngineWindowTerminateContextAPI(void)
{
	_AnimaEngineWindowTerminateTLS();
}

bool _AnimaEngineWindowCreateContext(AnimaEngineWindow_Base* window, const _AnimaEngineWindowctxconfig* ctxconfig, const _AnimaEngineWindowfbconfig* fbconfig)
{
	unsigned int attributeCount = 0;
	
	if (ctxconfig->_api == ANIMA_ENGINE_CORE_OPENGL_ES_API)
	{
//		_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: This API does not support OpenGL ES");
		return false;
	}
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (ctxconfig->_major == 3 && ctxconfig->_minor < 2)
	{
//		_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X does not " "support OpenGL 3.0 or 3.1");
		return false;
	}
	
	if (ctxconfig->_major > 2)
	{
		if (!ctxconfig->_forward)
		{
//			_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X only " "supports OpenGL 3.2 and later versions if they " "are forward-compatible");
			return false;
		}
		
		if (ctxconfig->_profile != ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE)
		{
//			_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X only " "supports OpenGL 3.2 and later versions if they " "use the core profile");
			return false;
		}
	}
#else
	if (ctxconfig->_major > 2)
	{
//		_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X does not " "support OpenGL version 3.0 or above");
		return false;
	}
#endif
	
	// Context robustness modes (GL_KHR_robustness) are not yet supported on
	// OS X but are not a hard constraint, so ignore and continue
	
	// Context release behaviors (GL_KHR_context_flush_control) are not yet
	// supported on OS X but are not a hard constraint, so ignore and continue
	
#define ADD_ATTR(x) { attributes[attributeCount++] = x; }
#define ADD_ATTR2(x, y) { ADD_ATTR(x); ADD_ATTR(y); }
	
	// Arbitrary array size here
	NSOpenGLPixelFormatAttribute attributes[40];
	
	ADD_ATTR(NSOpenGLPFAClosestPolicy);
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (ctxconfig->_major > 2)
	{
		ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
	}
	else
#endif
	{
		if (fbconfig->_auxBuffers != ANIMA_ENGINE_CORE_DONT_CARE)
			ADD_ATTR2(NSOpenGLPFAAuxBuffers, fbconfig->_auxBuffers);
		
		if (fbconfig->_accumRedBits != ANIMA_ENGINE_CORE_DONT_CARE && fbconfig->_accumGreenBits != ANIMA_ENGINE_CORE_DONT_CARE &&
			fbconfig->_accumBlueBits != ANIMA_ENGINE_CORE_DONT_CARE && fbconfig->_accumAlphaBits != ANIMA_ENGINE_CORE_DONT_CARE)
		{
			const int accumBits = fbconfig->_accumRedBits + fbconfig->_accumGreenBits + fbconfig->_accumBlueBits + fbconfig->_accumAlphaBits;
			
			ADD_ATTR2(NSOpenGLPFAAccumSize, accumBits);
		}
	}
	
	if (fbconfig->_redBits != ANIMA_ENGINE_CORE_DONT_CARE && fbconfig->_greenBits != ANIMA_ENGINE_CORE_DONT_CARE && fbconfig->_blueBits != ANIMA_ENGINE_CORE_DONT_CARE)
	{
		int colorBits = fbconfig->_redBits + fbconfig->_greenBits + fbconfig->_blueBits;
		
		// OS X needs non-zero color size, so set resonable values
		if (colorBits == 0)
			colorBits = 24;
		else if (colorBits < 15)
			colorBits = 15;
		
		ADD_ATTR2(NSOpenGLPFAColorSize, colorBits);
	}
	
	if (fbconfig->_alphaBits != ANIMA_ENGINE_CORE_DONT_CARE)
		ADD_ATTR2(NSOpenGLPFAAlphaSize, fbconfig->_alphaBits);
	
	if (fbconfig->_depthBits != ANIMA_ENGINE_CORE_DONT_CARE)
		ADD_ATTR2(NSOpenGLPFADepthSize, fbconfig->_depthBits);
	
	if (fbconfig->_stencilBits != ANIMA_ENGINE_CORE_DONT_CARE)
		ADD_ATTR2(NSOpenGLPFAStencilSize, fbconfig->_stencilBits);
	
	if (fbconfig->_stereo)
		ADD_ATTR(NSOpenGLPFAStereo);
	
	if (fbconfig->_doublebuffer)
		ADD_ATTR(NSOpenGLPFADoubleBuffer);
	
	if (fbconfig->_samples != ANIMA_ENGINE_CORE_DONT_CARE)
	{
		if (fbconfig->_samples == 0)
		{
			ADD_ATTR2(NSOpenGLPFASampleBuffers, 0);
		}
		else
		{
			ADD_ATTR2(NSOpenGLPFASampleBuffers, 1);
			ADD_ATTR2(NSOpenGLPFASamples, fbconfig->_samples);
		}
	}
	
	// NOTE: All NSOpenGLPixelFormats on the relevant cards support sRGB
	//       frambuffer, so there's no need (and no way) to request it
	
	ADD_ATTR(0);
	
#undef ADD_ATTR
#undef ADD_ATTR2
	
	_AnimaEngineWindowcontextNSGL* windowContext = window->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
	windowContext->_pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
	if (windowContext->_pixelFormat == nil)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "NSGL: Failed to create OpenGL pixel format");
		return false;
	}
	
	NSOpenGLContext* share = NULL;
	
	if (ctxconfig->_share)
		share = ctxconfig->_share->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE->_context;
	
	windowContext->_context = [[NSOpenGLContext alloc] initWithFormat:windowContext->_pixelFormat shareContext:share];
	if (windowContext->_context == nil)
	{
//		_glfwInputError(GLFW_PLATFORM_ERROR, "NSGL: Failed to create OpenGL context");
		return false;
	}
	
	return true;
}

void _AnimaEngineWindowDestroyContext(AnimaEngineWindow_Base* window)
{
	_AnimaEngineWindowcontextNSGL* windowContext = window->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
	[windowContext->_pixelFormat release];
	windowContext->_pixelFormat = nil;
	
	[windowContext->_context release];
	windowContext->_context = nil;
}

void _AnimaEngineWindowPlatformMakeContextCurrent(AnimaEngineWindow_Base* window)
{
	if (window)
		[window->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE->_context makeCurrentContext];
	else
		[NSOpenGLContext clearCurrentContext];
	
	_AnimaEngineWindowSetCurrentContext(window);
}

void _AnimaEngineWindowPlatformSwapBuffers(AnimaEngineWindow_Base* window)
{
	// ARP appears to be unnecessary, but this is future-proof
	[window->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE->_context flushBuffer];
}

void _AnimaEngineWindowPlatformSwapInterval(int interval)
{
	AnimaEngineWindow_Base* window = _AnimaEngineWindowPlatformGetCurrentContext();
	_AnimaEngineWindowcontextNSGL* windowContext = window->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
	
	GLint sync = interval;
	[windowContext->_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
}

bool _AnimaEngineWindowPlatformExtensionSupported(const char* extension)
{
	// There are no NSGL extensions
	return false;
}

AnimaEngineWindowglproc _AnimaEngineWindowPlatformGetProcAddress(const char* procname)
{
	CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault, procname, kCFStringEncodingASCII);

	_AnimaEngineWindowlibraryNSGL* platformLibraryContext = AnimaEngine::_GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;
	
	AnimaEngineWindowglproc symbol = (AnimaEngineWindowglproc)CFBundleGetFunctionPointerForName((CFBundleRef)platformLibraryContext->_framework, symbolName);
	
	CFRelease(symbolName);
	
	return symbol;
}

id AnimaEngineWindowGetNSGLContext(AnimaEngineWindow_Base* handle)
{
	AnimaEngineWindow_Base* window = (AnimaEngineWindow_Base*) handle;
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(nil);
	return (id)window->_GET_ANIMA_ENGINE_CORE_PLATFORM_CONTEXT_STATE;
}

END_ANIMA_ENGINE_CORE_NAMESPACE