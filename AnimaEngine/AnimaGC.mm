#include "AnimaGC.h"

#include <stdlib.h>
#include <assert.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#define ANIMA_GC_DONT_CARE	-1

void* AnimaGC::_framework = NULL;
bool AnimaGC::_contextAPIsInitialized = false;
bool AnimaGC::_GLEWExtensionsLoaded = false;

AnimaGC::AnimaGC()
{
	_view = nil;
	_context = nil;
	_pixelFormat = nil;
}

AnimaGC::~AnimaGC()
{
	DestroyContext(this);
}

void AnimaGC::MakeCurrent()
{
	if (_context)
		[_context makeCurrentContext];
	else
		[NSOpenGLContext clearCurrentContext];
}

void AnimaGC::SwapBuffers()
{
	if (_context)
		[_context flushBuffer];
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
	//if(_context)
	//{
	//	GLint sync = interval;
	//	[_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
	//}
}

bool AnimaGC::InitializeGLEWExtensions()
{
	if (_GLEWExtensionsLoaded)
		return true;
	
	char szBuff[1024];
	sprintf(szBuff, "OpenGL version string: %s\n", glGetString(GL_VERSION));

	puts(szBuff);
	
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
	ccfg._major = 4;
	ccfg._minor = 1;
	ccfg._forward = true;
	ccfg._debug = false;
	ccfg._profile = ANIMAGC_OPENGL_CORE_PROFILE;
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

	return fbcfg;
}

bool AnimaGC::InitializeContextAPIs()
{
	if (_contextAPIsInitialized)
		return true;
	
	_framework = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
	if(_framework == NULL)
		return false;
	
	_contextAPIsInitialized = true;
	return true;
}

void AnimaGC::TerminateContextAPIs(void)
{
	if (!_contextAPIsInitialized)
		return;
}

void AnimaGC::SurfaceNeedsUpdateHandler(CFNotificationCenterRef center, void* observer, CFStringRef name, const void* object, CFDictionaryRef userInfo)
{
	AnimaGC* gc = (AnimaGC*)observer;
	
	if(gc->_context)
		[gc->_context update];
}

AnimaGC* AnimaGC::CreateContext(long windowId, AnimaGCContextConfig ctxconfig, AnimaGCFrameBufferConfig fbconfig, bool vSyncEnabled)
{
	if(!InitializeContextAPIs())
		return nullptr;
	
	AnimaGC* newGC = new AnimaGC();
	newGC->_view = (NSView*)windowId;
	
	CFNotificationCenterAddObserver(CFNotificationCenterGetLocalCenter(), newGC, &SurfaceNeedsUpdateHandler, (__bridge CFStringRef)NSViewGlobalFrameDidChangeNotification, newGC->_view, CFNotificationSuspensionBehaviorDeliverImmediately);
	
	unsigned int attributeCount = 0;
	
	if (ctxconfig._api == ANIMAGC_OPENGL_ES_API)
	{
		//		_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: This API does not support OpenGL ES");
		return nullptr;
	}
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
	if (ctxconfig._major == 3 && ctxconfig._minor < 2)
	{
		//		_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X does not " "support OpenGL 3.0 or 3.1");
		return nullptr;
	}
	
	if (ctxconfig._major > 2)
	{
		if (!ctxconfig._forward)
		{
			//			_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X only " "supports OpenGL 3.2 and later versions if they " "are forward-compatible");
			return nullptr;
		}
		
		if (ctxconfig._profile != ANIMAGC_OPENGL_CORE_PROFILE)
		{
			//			_glfwInputError(GLFW_VERSION_UNAVAILABLE, "NSGL: The targeted version of OS X only " "supports OpenGL 3.2 and later versions if they " "use the core profile");
			return nullptr;
		}
	}
#else
	if (ctxconfig._major > 2)
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
	if (ctxconfig._major > 2)
	{
		ADD_ATTR2(NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core);
	}
	else
#endif
	{
		if (fbconfig._auxBuffers != ANIMA_GC_DONT_CARE)
			ADD_ATTR2(NSOpenGLPFAAuxBuffers, fbconfig._auxBuffers);
		
		if (fbconfig._accumRedBits != ANIMA_GC_DONT_CARE && fbconfig._accumGreenBits != ANIMA_GC_DONT_CARE &&
			fbconfig._accumBlueBits != ANIMA_GC_DONT_CARE && fbconfig._accumAlphaBits != ANIMA_GC_DONT_CARE)
		{
			const int accumBits = fbconfig._accumRedBits + fbconfig._accumGreenBits + fbconfig._accumBlueBits + fbconfig._accumAlphaBits;
			
			ADD_ATTR2(NSOpenGLPFAAccumSize, accumBits);
		}
	}
	
	if (fbconfig._redBits != ANIMA_GC_DONT_CARE && fbconfig._greenBits != ANIMA_GC_DONT_CARE && fbconfig._blueBits != ANIMA_GC_DONT_CARE)
	{
		int colorBits = fbconfig._redBits + fbconfig._greenBits + fbconfig._blueBits;
		
		// OS X needs non-zero color size, so set resonable values
		if (colorBits == 0)
			colorBits = 24;
		else if (colorBits < 15)
			colorBits = 15;
		
		ADD_ATTR2(NSOpenGLPFAColorSize, colorBits);
	}
	
	if (fbconfig._alphaBits != ANIMA_GC_DONT_CARE)
		ADD_ATTR2(NSOpenGLPFAAlphaSize, fbconfig._alphaBits);
	
	if (fbconfig._depthBits != ANIMA_GC_DONT_CARE)
		ADD_ATTR2(NSOpenGLPFADepthSize, fbconfig._depthBits);
	
	if (fbconfig._stencilBits != ANIMA_GC_DONT_CARE)
		ADD_ATTR2(NSOpenGLPFAStencilSize, fbconfig._stencilBits);
	
	if (fbconfig._stereo)
		ADD_ATTR(NSOpenGLPFAStereo);
	
	if (fbconfig._doublebuffer)
		ADD_ATTR(NSOpenGLPFADoubleBuffer);
	
	if (fbconfig._samples != ANIMA_GC_DONT_CARE)
	{
		if (fbconfig._samples == 0)
		{
			ADD_ATTR2(NSOpenGLPFASampleBuffers, 0);
		}
		else
		{
			ADD_ATTR2(NSOpenGLPFASampleBuffers, 1);
			ADD_ATTR2(NSOpenGLPFASamples, fbconfig._samples);
		}
	}
	
	// NOTE: All NSOpenGLPixelFormats on the relevant cards support sRGB
	//       frambuffer, so there's no need (and no way) to request it
	
	ADD_ATTR(0);
	
#undef ADD_ATTR
#undef ADD_ATTR2
	
	newGC->_pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
	if (newGC->_pixelFormat == nil)
	{
		//		_glfwInputError(GLFW_PLATFORM_ERROR, "NSGL: Failed to create OpenGL pixel format");
		return nullptr;
	}
	
	NSOpenGLContext* share = NULL;
	//if (ctxconfig._share)
	//	share = ctxconfig._share->_GET_ANIMA_ENGINE_PLATFORM_CONTEXT_STATE->_context;
	
	newGC->_context = [[NSOpenGLContext alloc] initWithFormat:newGC->_pixelFormat shareContext:share];
	if (newGC->_context == nil)
	{
		//		_glfwInputError(GLFW_PLATFORM_ERROR, "NSGL: Failed to create OpenGL context");
		return nullptr;
	}
	
	[newGC->_context setView:newGC->_view];
	
	newGC->MakeCurrent();
	InitializeGLEWExtensions();

	return newGC;
}

void AnimaGC::DestroyContext(AnimaGC* context)
{
	CFNotificationCenterRemoveEveryObserver(CFNotificationCenterGetLocalCenter(), context);
	
	[context->_pixelFormat release];
	context->_pixelFormat = nil;
	
	[context->_context release];
	context->_context = nil;
}

NSOpenGLContext* AnimaGC::GetContext()
{
	return _context;
}

NSOpenGLPixelFormat* AnimaGC::GetPixelFormat()
{
	return _pixelFormat;
}

bool AnimaGC::CheckIntegrity(NSView* view)
{
	return true;
}

END_ANIMA_ENGINE_NAMESPACE