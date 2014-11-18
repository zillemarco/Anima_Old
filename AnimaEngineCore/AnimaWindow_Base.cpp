#include "AnimaWindow_Base.h"
#include "AnimaEngine.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

static bool parseVersionString(int* api, int* major, int* minor, int* rev)
{
	const char* version;
	const char* prefixes[] =
	{
		"OpenGL ES-CM ",
		"OpenGL ES-CL ",
		"OpenGL ES ",
		NULL
	};

	*api = ANIMA_ENGINE_CORE_OPENGL_API;

	version = (const char*)glGetString(GL_VERSION);
	if (!version)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Failed to retrieve context version string");
		return false;
	}

	for (int i = 0; prefixes[i]; i++)
	{
		const size_t length = strlen(prefixes[i]);

		if (strncmp(version, prefixes[i], length) == 0)
		{
			version += length;
			*api = ANIMA_ENGINE_CORE_OPENGL_ES_API;
			break;
		}
	}

	if (!sscanf(version, "%d.%d.%d", major, minor, rev))
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "No version found in context version string");
		return false;
	}

	return true;
}

bool _AnimaEngineWindowStringInExtensionString(const char* string, const GLubyte* extensions)
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

const _AnimaEngineWindowfbconfig* _AnimaEngineWindowChooseFBConfig(const _AnimaEngineWindowfbconfig* desired, const _AnimaEngineWindowfbconfig* alternatives, unsigned int count)
{
	unsigned int i;
	unsigned int missing, leastMissing = UINT_MAX;
	unsigned int colorDiff, leastColorDiff = UINT_MAX;
	unsigned int extraDiff, leastExtraDiff = UINT_MAX;
	const _AnimaEngineWindowfbconfig* current;
	const _AnimaEngineWindowfbconfig* closest = NULL;

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

			if (desired->_redBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				colorDiff += (desired->_redBits - current->_redBits) * (desired->_redBits - current->_redBits);
			}

			if (desired->_greenBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				colorDiff += (desired->_greenBits - current->_greenBits) * (desired->_greenBits - current->_greenBits);
			}

			if (desired->_blueBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				colorDiff += (desired->_blueBits - current->_blueBits) * (desired->_blueBits - current->_blueBits);
			}
		}

		// Calculate non-color channel size difference value
		{
			extraDiff = 0;

			if (desired->_alphaBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_alphaBits - current->_alphaBits) * (desired->_alphaBits - current->_alphaBits);
			}

			if (desired->_depthBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_depthBits - current->_depthBits) * (desired->_depthBits - current->_depthBits);
			}

			if (desired->_stencilBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_stencilBits - current->_stencilBits) * (desired->_stencilBits - current->_stencilBits);
			}

			if (desired->_accumRedBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumRedBits - current->_accumRedBits) * (desired->_accumRedBits - current->_accumRedBits);
			}

			if (desired->_accumGreenBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumGreenBits - current->_accumGreenBits) * (desired->_accumGreenBits - current->_accumGreenBits);
			}

			if (desired->_accumBlueBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumBlueBits - current->_accumBlueBits) * (desired->_accumBlueBits - current->_accumBlueBits);
			}

			if (desired->_accumAlphaBits != ANIMA_ENGINE_CORE_DONT_CARE)
			{
				extraDiff += (desired->_accumAlphaBits - current->_accumAlphaBits) * (desired->_accumAlphaBits - current->_accumAlphaBits);
			}

			if (desired->_samples != ANIMA_ENGINE_CORE_DONT_CARE)
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

static int compareVideoModes(const void* firstPtr, const void* secondPtr)
{
	int firstBPP, secondBPP, firstSize, secondSize;
	const AnimaEngineWindowvidmode* first = (const AnimaEngineWindowvidmode*)firstPtr;
	const AnimaEngineWindowvidmode* second = (const AnimaEngineWindowvidmode*)secondPtr;

	// First sort on color bits per pixel
	firstBPP = first->_redBits + first->_greenBits + first->_blueBits;
	secondBPP = second->_redBits + second->_greenBits + second->_blueBits;
	if (firstBPP != secondBPP)
		return firstBPP - secondBPP;

	// Then sort on screen area, in pixels
	firstSize = first->_width * first->_height;
	secondSize = second->_width * second->_height;
	if (firstSize != secondSize)
		return firstSize - secondSize;

	// Lastly sort on refresh rate
	return first->_refreshRate - second->_refreshRate;
}

static bool refreshVideoModes(_AnimaEngineWindowmonitor* monitor)
{
	int modeCount;
	AnimaEngineWindowvidmode* modes;

	if (monitor->_modes)
		return true;

	modes = _AnimaEngineWindowPlatformGetVideoModes(monitor, &modeCount);
	if (!modes)
		return false;

	qsort(modes, modeCount, sizeof(AnimaEngineWindowvidmode), compareVideoModes);

	free(monitor->_modes);
	monitor->_modes = modes;
	monitor->_modeCount = modeCount;

	return true;
}

void _AnimaEngineWindowInputMonitorChange(AnimaEngine* engine)
{
	int i, j, monitorCount = AnimaEngine::_monitorCount;
	_AnimaEngineWindowmonitor** monitors = AnimaEngine::_monitors;

	AnimaEngine::_monitors = _AnimaEngineWindowPlatformGetMonitors(&AnimaEngine::_monitorCount);

	// Re-use still connected monitor objects
	for (i = 0; i < AnimaEngine::_monitorCount; i++)
	{
		for (j = 0; j < monitorCount; j++)
		{
			if (_AnimaEngineWindowPlatformIsSameMonitor(AnimaEngine::_monitors[i], monitors[j]))
			{
				_AnimaEngineWindowFreeMonitor(AnimaEngine::_monitors[i]);
				AnimaEngine::_monitors[i] = monitors[j];
				break;
			}
		}
	}

	// Find and report disconnected monitors (not in the new list)
	for (i = 0; i < monitorCount; i++)
	{
		AnimaEngineWindow_Base* window;

		for (j = 0; j < AnimaEngine::_monitorCount; j++)
		{
			if (monitors[i] == AnimaEngine::_monitors[j])
				break;
		}

		if (j < AnimaEngine::_monitorCount)
			continue;

		for (window = engine->GetWindowListHead(); window; window = window->GetNext())
		{
			if (window->GetMonitor() == monitors[i])
				window->SetMonitor(NULL);
		}

		if (AnimaEngine::_callbacks._monitor)
			AnimaEngine::_callbacks._monitor((AnimaEngineWindowmonitor*)monitors[i], ANIMA_ENGINE_CORE_DISCONNECTED);
	}

	// Find and report newly connected monitors (not in the old list)
	// Re-used monitor objects are then removed from the old list to avoid
	// having them destroyed at the end of this function
	for (i = 0; i < AnimaEngine::_monitorCount; i++)
	{
		for (j = 0; j < monitorCount; j++)
		{
			if (AnimaEngine::_monitors[i] == monitors[j])
			{
				monitors[j] = NULL;
				break;
			}
		}

		if (j < monitorCount)
			continue;

		if (AnimaEngine::_callbacks._monitor)
			AnimaEngine::_callbacks._monitor((AnimaEngineWindowmonitor*)AnimaEngine::_monitors[i], ANIMA_ENGINE_CORE_CONNECTED);
	}

	_AnimaEngineWindowFreeMonitors(monitors, monitorCount);
}

_AnimaEngineWindowmonitor* _AnimaEngineWindowAllocMonitor(const char* name, int widthMM, int heightMM)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)calloc(1, sizeof(_AnimaEngineWindowmonitor));
	monitor->_name = strdup(name);
	monitor->_widthMM = widthMM;
	monitor->_heightMM = heightMM;

	return monitor;
}

void _AnimaEngineWindowFreeMonitor(_AnimaEngineWindowmonitor* monitor)
{
	if (monitor == NULL)
		return;

	_AnimaEngineWindowFreeGammaArrays(&monitor->_originalRamp);
	_AnimaEngineWindowFreeGammaArrays(&monitor->_currentRamp);

	free(monitor->_modes);
	free(monitor->_name);
	free(monitor);
}

void _AnimaEngineWindowAllocGammaArrays(AnimaEngineWindowgammaramp* ramp, unsigned int size)
{
	ramp->_red = (unsigned short*)calloc(size, sizeof(unsigned short));
	ramp->_green = (unsigned short*)calloc(size, sizeof(unsigned short));
	ramp->_blue = (unsigned short*)calloc(size, sizeof(unsigned short));
	ramp->_size = size;
}

void _AnimaEngineWindowFreeGammaArrays(AnimaEngineWindowgammaramp* ramp)
{
	free(ramp->_red);
	free(ramp->_green);
	free(ramp->_blue);

	memset(ramp, 0, sizeof(AnimaEngineWindowgammaramp));
}

void _AnimaEngineWindowFreeMonitors(_AnimaEngineWindowmonitor** monitors, int count)
{
	int i;

	for (i = 0; i < count; i++)
		_AnimaEngineWindowFreeMonitor(monitors[i]);

	free(monitors);
}

const AnimaEngineWindowvidmode* _AnimaEngineWindowChooseVideoMode(_AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowvidmode* desired)
{
	int i;
	unsigned int sizeDiff, leastSizeDiff = UINT_MAX;
	unsigned int rateDiff, leastRateDiff = UINT_MAX;
	unsigned int colorDiff, leastColorDiff = UINT_MAX;
	const AnimaEngineWindowvidmode* current;
	const AnimaEngineWindowvidmode* closest = NULL;

	if (!refreshVideoModes(monitor))
		return NULL;

	for (i = 0; i < monitor->_modeCount; i++)
	{
		current = monitor->_modes + i;

		colorDiff = 0;

		if (desired->_redBits != ANIMA_ENGINE_CORE_DONT_CARE)
			colorDiff += abs(current->_redBits - desired->_redBits);
		if (desired->_greenBits != ANIMA_ENGINE_CORE_DONT_CARE)
			colorDiff += abs(current->_greenBits - desired->_greenBits);
		if (desired->_blueBits != ANIMA_ENGINE_CORE_DONT_CARE)
			colorDiff += abs(current->_blueBits - desired->_blueBits);

		sizeDiff = abs((current->_width - desired->_width) * (current->_width - desired->_width) + (current->_height - desired->_height) * (current->_height - desired->_height));

		if (desired->_refreshRate != ANIMA_ENGINE_CORE_DONT_CARE)
			rateDiff = abs(current->_refreshRate - desired->_refreshRate);
		else
			rateDiff = UINT_MAX - current->_refreshRate;

		if ((colorDiff < leastColorDiff) || (colorDiff == leastColorDiff && sizeDiff < leastSizeDiff) || (colorDiff == leastColorDiff && sizeDiff == leastSizeDiff && rateDiff < leastRateDiff))
		{
			closest = current;
			leastSizeDiff = sizeDiff;
			leastRateDiff = rateDiff;
			leastColorDiff = colorDiff;
		}
	}

	return closest;
}

int _AnimaEngineWindowCompareVideoModes(const AnimaEngineWindowvidmode* first, const AnimaEngineWindowvidmode* second)
{
	return compareVideoModes(first, second);
}

void _AnimaEngineWindowSplitBPP(int bpp, int* red, int* green, int* blue)
{
	int delta;

	// We assume that by 32 the user really meant 24
	if (bpp == 32)
		bpp = 24;

	// Convert "bits per pixel" to red, green & blue sizes

	*red = *green = *blue = bpp / 3;
	delta = bpp - (*red * 3);
	if (delta >= 1)
		*green = *green + 1;

	if (delta == 2)
		*red = *red + 1;
}

AnimaEngineWindowmonitor** AnimaEngineWindowGetMonitors(int* count)
{
	*count = 0;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);

	*count = AnimaEngine::_monitorCount;
	return (AnimaEngineWindowmonitor**)AnimaEngine::_monitors;
}

AnimaEngineWindowmonitor* AnimaEngineWindowGetPrimaryMonitor(void)
{
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);
	return (AnimaEngineWindowmonitor*)AnimaEngine::_monitors[0];
}

void AnimaEngineWindowGetMonitorPos(AnimaEngineWindowmonitor* handle, int* xpos, int* ypos)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;

	if (xpos)
		*xpos = 0;
	if (ypos)
		*ypos = 0;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

	_AnimaEngineWindowPlatformGetMonitorPos(monitor, xpos, ypos);
}

void AnimaEngineWindowGetMonitorPhysicalSize(AnimaEngineWindowmonitor* handle, int* width, int* height)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;

	if (width)
		*width = 0;
	if (height)
		*height = 0;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

	if (width)
		*width = monitor->_widthMM;
	if (height)
		*height = monitor->_heightMM;
}

const char* AnimaEngineWindowGetMonitorName(AnimaEngineWindowmonitor* handle)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);
	return monitor->_name;
}

AnimaEngineWindowmonitorfun AnimaEngineWindowSetMonitorCallback(AnimaEngineWindowmonitorfun cbfun)
{
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);
	_ANIMA_ENGINE_CORE_SWAP_POINTERS(AnimaEngine::_callbacks._monitor, cbfun, AnimaEngineWindowmonitorfun);
	return cbfun;
}

const AnimaEngineWindowvidmode* AnimaEngineWindowGetVideoModes(AnimaEngineWindowmonitor* handle, int* count)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;

	*count = 0;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);

	if (!refreshVideoModes(monitor))
		return NULL;

	*count = monitor->_modeCount;
	return monitor->_modes;
}

const AnimaEngineWindowvidmode* AnimaEngineWindowGetVideoMode(AnimaEngineWindowmonitor* handle)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);

	_AnimaEngineWindowPlatformGetVideoMode(monitor, &monitor->_currentMode);
	return &monitor->_currentMode;
}

const AnimaEngineWindowgammaramp* AnimaEngineWindowGetGammaRamp(AnimaEngineWindowmonitor* handle)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);

	_AnimaEngineWindowFreeGammaArrays(&monitor->_currentRamp);
	_AnimaEngineWindowPlatformGetGammaRamp(monitor, &monitor->_currentRamp);

	return &monitor->_currentRamp;
}

void AnimaEngineWindowSetGammaRamp(AnimaEngineWindowmonitor* handle, const AnimaEngineWindowgammaramp* ramp)
{
	_AnimaEngineWindowmonitor* monitor = (_AnimaEngineWindowmonitor*)handle;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

	if (!monitor->_originalRamp._size)
		_AnimaEngineWindowPlatformGetGammaRamp(monitor, &monitor->_originalRamp);

	_AnimaEngineWindowPlatformSetGammaRamp(monitor, ramp);
}

void AnimaEngineWindowSetGamma(AnimaEngineWindowmonitor* handle, float gamma)
{
	int i;
	unsigned short values[256];
	AnimaEngineWindowgammaramp ramp;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT();

	if (gamma <= 0.f)
	{
		//_glfwInputError(GLFW_INVALID_VALUE, "Gamma value must be greater than zero");
		return;
	}

	for (i = 0; i < 256; i++)
	{
		double value;

		// Calculate intensity
		value = i / 255.0;

		// Apply gamma curve
		value = pow(value, 1.0 / gamma) * 65535.0 + 0.5;

		// Clamp to value range
		if (value > 65535.0)
			value = 65535.0;

		values[i] = (unsigned short)value;
	}

	ramp._red = values;
	ramp._green = values;
	ramp._blue = values;
	ramp._size = 256;

	AnimaEngineWindowSetGammaRamp(handle, &ramp);
}

bool _AnimaEngineWindowRefreshContextAttribs(const _AnimaEngineWindowctxconfig* ctxconfig)
{
	AnimaEngineWindow_Base* window = _AnimaEngineWindowPlatformGetCurrentContext();
	
	AnimaEngineWindowContext* windowContext = window->GetContext();
	
	if (!parseVersionString(&windowContext->_api, &windowContext->_major, &windowContext->_minor, &windowContext->_revision))
		return false;

#if defined(_ANIMA_ENGINE_CORE_USE_OPENGL)
	if (windowContext->_major > 2)
	{
		// OpenGL 3.0+ uses a different function for extension string retrieval
		// We cache it here instead of in glfwExtensionSupported mostly to alert
		// users as early as possible that their build may be broken
		window->SetGetStringiCallback((PFNGLGETSTRINGIPROC)_AnimaEngineWindowGetProcAddress("glGetStringi"));
		if (!window->GetGetStringiCallback())
		{
			//_glfwInputError(GLFW_PLATFORM_ERROR, "Entry point retrieval is broken");
			return false;
		}
	}

	if (windowContext->_api == ANIMA_ENGINE_CORE_OPENGL_API)
	{
		// Read back context flags (OpenGL 3.0 and above)
		if (windowContext->_major >= 3)
		{
			GLint flags;
			glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

			if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
				windowContext->_forward = true;
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
				windowContext->_debug = true;
			else if (AnimaEngine::ExtensionSupported("GL_ARB_debug_output") && ctxconfig->_debug)
			{
				// HACK: This is a workaround for older drivers (pre KHR_debug)
				//       not setting the debug bit in the context flags for
				//       debug contexts
				windowContext->_debug = true;
			}
		}

		// Read back OpenGL context profile (OpenGL 3.2 and above)
		if (windowContext->_major > 3 || (windowContext->_major == 3 && windowContext->_minor >= 2))
		{
			GLint mask;
			glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);

			if (mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
				windowContext->_profile = ANIMA_ENGINE_CORE_OPENGL_COMPAT_PROFILE;
			else if (mask & GL_CONTEXT_CORE_PROFILE_BIT)
				windowContext->_profile = ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE;
			else if (AnimaEngine::ExtensionSupported("GL_ARB_compatibility"))
			{
				// HACK: This is a workaround for the compatibility profile bit
				//       not being set in the context flags if an OpenGL 3.2+
				//       context was created without having requested a specific
				//       version
				windowContext->_profile = ANIMA_ENGINE_CORE_OPENGL_COMPAT_PROFILE;
			}
		}

		// Read back robustness strategy
		if (AnimaEngine::ExtensionSupported("GL_ARB_robustness"))
		{
			// NOTE: We avoid using the context flags for detection, as they are
			//       only present from 3.0 while the extension applies from 1.1

			GLint strategy;
			glGetIntegerv(GL_RESET_NOTIFICATION_STRATEGY_ARB, &strategy);

			if (strategy == GL_LOSE_CONTEXT_ON_RESET_ARB)
				windowContext->_robustness = ANIMA_ENGINE_CORE_LOSE_CONTEXT_ON_RESET;
			else if (strategy == GL_NO_RESET_NOTIFICATION_ARB)
				windowContext->_robustness = ANIMA_ENGINE_CORE_NO_RESET_NOTIFICATION;
		}
	}
	else
	{
		// Read back robustness strategy
		if (AnimaEngine::ExtensionSupported("GL_EXT_robustness"))
		{
			// NOTE: The values of these constants match those of the OpenGL ARB
			//       one, so we can reuse them here

			GLint strategy;
			glGetIntegerv(GL_RESET_NOTIFICATION_STRATEGY_ARB, &strategy);

			if (strategy == GL_LOSE_CONTEXT_ON_RESET_ARB)
				windowContext->_robustness = ANIMA_ENGINE_CORE_LOSE_CONTEXT_ON_RESET;
			else if (strategy == GL_NO_RESET_NOTIFICATION_ARB)
				windowContext->_robustness = ANIMA_ENGINE_CORE_NO_RESET_NOTIFICATION;
		}
	}

	if (AnimaEngine::ExtensionSupported("GL_KHR_context_flush_control"))
	{
		GLint behavior;
		glGetIntegerv(GL_CONTEXT_RELEASE_BEHAVIOR, &behavior);

		if (behavior == GL_NONE)
			windowContext->_release = ANIMA_ENGINE_CORE_RELEASE_BEHAVIOR_NONE;
		else if (behavior == GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH)
			windowContext->_release = ANIMA_ENGINE_CORE_RELEASE_BEHAVIOR_FLUSH;
	}
#endif

	return true;
}

AnimaEngineWindowglproc	_AnimaEngineWindowGetProcAddress(const char* procname)
{
	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);

	if (!_AnimaEngineWindowPlatformGetCurrentContext())
	{
		//_glfwInputError(GLFW_NO_CURRENT_CONTEXT, NULL);
		return NULL;
	}

	return _AnimaEngineWindowPlatformGetProcAddress(procname);
}

bool _AnimaEngineWindowIsValidContextConfig(const _AnimaEngineWindowctxconfig* ctxconfig)
{
	if (ctxconfig->_api != ANIMA_ENGINE_CORE_OPENGL_API && ctxconfig->_api != ANIMA_ENGINE_CORE_OPENGL_ES_API)
	{
		//_glfwInputError(GLFW_INVALID_ENUM, "Invalid client API requested");
		return false;
	}

	if (ctxconfig->_api == ANIMA_ENGINE_CORE_OPENGL_API)
	{
		if ((ctxconfig->_major < 1 ||	ctxconfig->_minor < 0) ||
			(ctxconfig->_major == 1 &&	ctxconfig->_minor > 5) ||
			(ctxconfig->_major == 2 &&	ctxconfig->_minor > 1) ||
			(ctxconfig->_major == 3 &&	ctxconfig->_minor > 3))
		{
			// OpenGL 1.0 is the smallest valid version
			// OpenGL 1.x series ended with version 1.5
			// OpenGL 2.x series ended with version 2.1
			// OpenGL 3.x series ended with version 3.3

			//_glfwInputError(GLFW_INVALID_VALUE, "Invalid OpenGL version %i.%i requested", ctxconfig->major, ctxconfig->minor);
			return false;
		}

		if (ctxconfig->_profile)
		{
			if (ctxconfig->_profile != ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE && ctxconfig->_profile != ANIMA_ENGINE_CORE_OPENGL_COMPAT_PROFILE)
			{
				//_glfwInputError(GLFW_INVALID_ENUM, "Invalid OpenGL profile requested");
				return false;
			}

			if (ctxconfig->_major < 3 || (ctxconfig->_major == 3 && ctxconfig->_minor < 2))
			{
				// Desktop OpenGL context profiles are only defined for version 3.2
				// and above
				//_glfwInputError(GLFW_INVALID_VALUE, "Context profiles only exist for " "OpenGL version 3.2 and above"); 
				return false;
			}
		}

		if (ctxconfig->_forward && ctxconfig->_major < 3)
		{
			// Forward-compatible contexts are only defined for OpenGL version 3.0 and above
			//_glfwInputError(GLFW_INVALID_VALUE, "Forward compatibility only exist for OpenGL " "version 3.0 and above");
			return false;
		}
	}
	else if (ctxconfig->_api == ANIMA_ENGINE_CORE_OPENGL_ES_API)
	{
		if ( ctxconfig->_major < 1 ||  ctxconfig->_minor < 0	||
			(ctxconfig->_major == 1 && ctxconfig->_minor > 1)	||
			(ctxconfig->_major == 2 && ctxconfig->_minor > 0))
		{
			// OpenGL ES 1.0 is the smallest valid version
			// OpenGL ES 1.x series ended with version 1.1
			// OpenGL ES 2.x series ended with version 2.0

			//_glfwInputError(GLFW_INVALID_VALUE, "Invalid OpenGL ES version %i.%i requested", ctxconfig->major, ctxconfig->minor);
			return false;
		}

		if (ctxconfig->_profile)
		{
			// OpenGL ES does not support profiles
			//_glfwInputError(GLFW_INVALID_VALUE, "Context profiles are not supported by OpenGL ES");
			return false;
		}

		if (ctxconfig->_forward)
		{
			// OpenGL ES does not support forward-compatibility
			//_glfwInputError(GLFW_INVALID_VALUE, "Forward compatibility is not supported by OpenGL ES");
			return false;
		}
	}

	if (ctxconfig->_robustness)
	{
		if (ctxconfig->_robustness != ANIMA_ENGINE_CORE_NO_RESET_NOTIFICATION && ctxconfig->_robustness != ANIMA_ENGINE_CORE_LOSE_CONTEXT_ON_RESET)
		{
			//_glfwInputError(GLFW_INVALID_VALUE, "Invalid context robustness mode requested");
			return false;
		}
	}

	if (ctxconfig->_release)
	{
		if (ctxconfig->_release != ANIMA_ENGINE_CORE_RELEASE_BEHAVIOR_NONE && ctxconfig->_release != ANIMA_ENGINE_CORE_RELEASE_BEHAVIOR_FLUSH)
		{
			//_glfwInputError(GLFW_INVALID_VALUE, "Invalid context release behavior requested");
			return false;
		}
	}

	return true;
}

bool _AnimaEngineWindowIsValidContext(const _AnimaEngineWindowctxconfig* ctxconfig)
{
	AnimaEngineWindow_Base* window = _AnimaEngineWindowPlatformGetCurrentContext();
	AnimaEngineWindowContext* windowContext = window->GetContext();

	if (windowContext->_major < ctxconfig->_major || (windowContext->_major == ctxconfig->_major && windowContext->_minor < ctxconfig->_minor))
	{
		// The desired OpenGL version is greater than the actual version
		// This only happens if the machine lacks {GLX|WGL}_ARB_create_context
		// /and/ the user has requested an OpenGL version greater than 1.0
		// For API consistency, we emulate the behavior of the
		// {GLX|WGL}_ARB_create_context extension and fail here

		//_glfwInputError(GLFW_VERSION_UNAVAILABLE, NULL);
		return false;
	}

	return true;
}

int AnimaEngineWindow_Base::_windowCount = 0;

void AnimaEngineWindow_Base::InitCallbacks()
{
	_windowFocusCallback = AnimaEngineWindow_Base_Focus;
	_windowPosCallback = AnimaEngineWindow_Base_Pos;
	_windowSizeCallback = AnimaEngineWindow_Base_Size;
	_windowIconifyCallback = AnimaEngineWindow_Base_Iconify;
	_windowVisibilityCallback = AnimaEngineWindow_Base_Visibility;
	_windowDamageCallback = AnimaEngineWindow_Base_Damage;
	_windowCloseRequestCallback = AnimaEngineWindow_Base_CloseRequest;
	_windowKeyCallback = AnimaEngineWindow_Base_Key;
	_windowCharCallback = AnimaEngineWindow_Base_Char;
	_windowScrollCallback = AnimaEngineWindow_Base_Scroll;
	_windowMouseClickCallback = AnimaEngineWindow_Base_MouseClick;
	_windowCursorMotionCallback = AnimaEngineWindow_Base_CursorMotion;
	_windowCursoEnterCallback = AnimaEngineWindow_Base_CursorEnter;
	_windowDropCallback = AnimaEngineWindow_Base_Drop;
	_frameBufferSizeCallback = AnimaEngineWindow_Base_FrameBufferSize;
	_GetStringiCallback = NULL;
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Focus(AnimaEngineWindow_Base* window, bool focused)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Pos(AnimaEngineWindow_Base* window, int xpos, int ypos)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Size(AnimaEngineWindow_Base* window, int width, int height)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Iconify(AnimaEngineWindow_Base* window, bool iconified)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Visibility(AnimaEngineWindow_Base* window, bool visible)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Damage(AnimaEngineWindow_Base* window)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_CloseRequest(AnimaEngineWindow_Base* window)
{
	window->SetShouldClose(true);
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Key(AnimaEngineWindow_Base* window, int key, int scancode, int action, int mods)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Char(AnimaEngineWindow_Base* window, unsigned int codepoint, int mods, bool plain)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Scroll(AnimaEngineWindow_Base* window, double x, double y)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_MouseClick(AnimaEngineWindow_Base* window, int button, int action, int mods)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_CursorMotion(AnimaEngineWindow_Base* window, double x, double y)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_CursorEnter(AnimaEngineWindow_Base* window, bool entered)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_Drop(AnimaEngineWindow_Base* window, int count, const char** names)
{
}

void AnimaEngineWindow_Base::AnimaEngineWindow_Base_FrameBufferSize(AnimaEngineWindow_Base* window, int width, int height)
{
}

END_ANIMA_ENGINE_CORE_NAMESPACE