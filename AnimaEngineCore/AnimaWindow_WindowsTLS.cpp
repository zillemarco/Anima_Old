#include "AnimaWindow_Base.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

bool _AnimaEngineWindowInitTLS(void)
{
	//_glfw.win32_tls.context = TlsAlloc();
	//if (_glfw.win32_tls.context == TLS_OUT_OF_INDEXES)
	//{
	//	_glfwInputError(GLFW_PLATFORM_ERROR,
	//		"Win32: Failed to allocate TLS index");
	//	return GL_FALSE;
	//}

	//_glfw.win32_tls.allocated = GL_TRUE;
	return true;
}

void _AnimaEngineWindowTerminateTLS(void)
{
	//if (_glfw.win32_tls.allocated)
	//	TlsFree(_glfw.win32_tls.context);
}

void _AnimaEngineWindowSetCurrentContext(_AnimaEngineWindowwindow* context)
{
	//TlsSetValue(_glfw.win32_tls.context, context);
}

_AnimaEngineWindowwindow* _AnimaEngineWindowPlatformGetCurrentContext(void)
{
	//return TlsGetValue(_glfw.win32_tls.context);
	return NULL;
}

END_ANIMA_ENGINE_CORE_NAMESPACE