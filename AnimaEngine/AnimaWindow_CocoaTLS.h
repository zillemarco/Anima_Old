#ifndef _AnimaWindow_WindowsTLS_h
#define _AnimaWindow_WindowsTLS_h

#include "AnimaEngineCore.h"
#include <pthread.h>

#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE _AnimaEngineWindowtlsNS ns_tls

#define _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE	_AnimaEngineWindowtlsNS* GetPlatformLibraryTLSState() { return &ns_tls; }
#define _GET_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE	GetPlatformLibraryTLSState()

#define _INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE(class) _AnimaEngineWindowtlsNS class::ns_tls;

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowtlsNS
{
	pthread_key_t _context;
	
} _AnimaEngineWindowtlsNS;


bool _AnimaEngineWindowInitTLS(void);
void _AnimaEngineWindowTerminateTLS(void);
void _AnimaEngineWindowSetCurrentContext(AnimaEngineWindow_Base* context);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif