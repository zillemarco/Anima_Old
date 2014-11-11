#include "AnimaWindow_Base.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

bool _AnimaEngineWindowInitTLS(void)
{
	AnimaEngine::win32_tls._context = TlsAlloc();
	if (AnimaEngine::win32_tls._context == TLS_OUT_OF_INDEXES)
	{
		//_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to allocate TLS index");
		return false;
	}

	AnimaEngine::win32_tls._allocated = true;
	return true;
}

void _AnimaEngineWindowTerminateTLS(void)
{
	if (AnimaEngine::win32_tls._allocated)
		TlsFree(AnimaEngine::win32_tls._context);
}

void _AnimaEngineWindowSetCurrentContext(_AnimaEngineWindowwindow* context)
{
	TlsSetValue(AnimaEngine::win32_tls._context, context);
}

_AnimaEngineWindowwindow* _AnimaEngineWindowPlatformGetCurrentContext(void)
{
	return (_AnimaEngineWindowwindow*)TlsGetValue(AnimaEngine::win32_tls._context);
}

END_ANIMA_ENGINE_CORE_NAMESPACE