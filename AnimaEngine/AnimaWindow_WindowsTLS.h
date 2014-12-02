#ifndef _AnimaWindow_WindowsTLS_h
#define _AnimaWindow_WindowsTLS_h

#include "AnimaEngineCore.h"

#define _ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE _AnimaEngineWindowtlsWin32 win32_tls

#define _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE	_AnimaEngineWindowtlsWin32* GetPlatformLibraryTLSState() { return &win32_tls; }
#define _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE	GetPlatformLibraryTLSState()

#define _INIT_STATIC_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE(class) _AnimaEngineWindowtlsWin32 class::win32_tls;

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef struct _AnimaEngineWindowtlsWin32
{
	bool	_allocated;
	DWORD	_context;

} _AnimaEngineWindowtlsWin32;

extern "C" ANIMA_ENGINE_EXPORT bool _AnimaEngineWindowInitTLS(void);
extern "C" ANIMA_ENGINE_EXPORT void _AnimaEngineWindowTerminateTLS(void);
extern "C" ANIMA_ENGINE_EXPORT void _AnimaEngineWindowSetCurrentContext(AnimaEngineWindow_Base* context);

END_ANIMA_ENGINE_NAMESPACE

#endif