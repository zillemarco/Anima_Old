#if !defined __Anima__AnimaGC__
#define __Anima__AnimaGC__

#if !defined ANIMA_ENGINE_INCLUDED_GLEW
#	include <GL/glew.h>
#	define ANIMA_ENGINE_INCLUDED_GLEW
#endif

#if defined _WIN32
#	include <GL/wglew.h>
#	include <windows.h>
#endif

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define ANIMAGC_OPENGL_API				0x00030001
#define ANIMAGC_OPENGL_ES_API			0x00030002

#define ANIMAGC_ANY_RELEASE_BEHAVIOR	0
#define ANIMAGC_RELEASE_BEHAVIOR_FLUSH	0x00035001
#define ANIMAGC_RELEASE_BEHAVIOR_NONE	0x00035002

#define ANIMAGC_NO_ROBUSTNESS			0
#define ANIMAGC_NO_RESET_NOTIFICATION	0x00031001
#define ANIMAGC_LOSE_CONTEXT_ON_RESET	0x00031002

#define ANIMAGC_OPENGL_ANY_PROFILE		0
#define ANIMAGC_OPENGL_CORE_PROFILE		0x00032001
#define ANIMAGC_OPENGL_COMPAT_PROFILE	0x00032002

#define ANIMAGC_RECREATION_NOT_NEEDED	0
#define ANIMAGC_RECREATION_REQUIRED		1
#define ANIMAGC_RECREATION_IMPOSSIBLE	2

typedef void(*AnimaGCGLproc)(void);

struct ANIMA_ENGINE_EXPORT AnimaGCFrameBufferConfig
{
	AInt _redBits;
	AInt _greenBits;
	AInt _blueBits;
	AInt _alphaBits;
	AInt _depthBits;
	AInt _stencilBits;
	AInt _accumRedBits;
	AInt _accumGreenBits;
	AInt _accumBlueBits;
	AInt _accumAlphaBits;
	AInt _auxBuffers;
	bool _stereo;
	AInt _samples;
	bool _sRGB;
	bool _doublebuffer;

	#if defined _WIN32
		AInt _wgl;
	#endif
};

struct ANIMA_ENGINE_EXPORT AnimaGCContextConfig
{
	AInt _api;
	AInt _major;
	AInt _minor;
	bool _forward;
	bool _debug;
	AInt _profile;
	AInt _robustness;
	AInt _release;
};

/*!
 *	\brief		Classe utilizzata per creare un contesto grafico su di una finestra
 *	\details	Classe utilizzata per creare un contesto grafico su di una finestra
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaGC
{
protected:
	AnimaGC();

public:
	~AnimaGC();

public:
	void MakeCurrent();
	void SwapBuffers();

	static void DestroyContext(AnimaGC* context);
	static void SetSwapInterval(AInt interval);

protected:
	static bool StringInExtensionString(const char* string, const GLubyte* extensions);
	static const AnimaGCFrameBufferConfig* ChooseFrameBufferConfig(const AnimaGCFrameBufferConfig* desired, const AnimaGCFrameBufferConfig* alternatives, unsigned int count);

#if defined _WIN32
	public:
		bool CheckIntegrity(HWND hWnd);
		HGLRC GetContext();

	public:
		static AnimaGC* CreateContext(HWND hWnd, const AnimaGCContextConfig* ctxconfig, const AnimaGCFrameBufferConfig* fbconfig);
		
	public:
		static bool InitializeGLEWExtensions();
		static bool IsExtensionSupported(const char* extension, HDC hDC);

	protected:
		static bool InitializeWGLExtensions();
		static AInt GetPixelFormatAttrib(AInt pixelFormat, AInt attrib, AnimaGC* gc);
		static bool ChoosePixelFormat(const AnimaGCFrameBufferConfig* desired, AInt* result, AnimaGC* gc);
		static bool InitializeContextAPIs();
		static void TerminateContextAPIs();
		static AInt	AnalyzeContext(const AnimaGCContextConfig* ctxconfig, const AnimaGCFrameBufferConfig* fbconfig);
		static AnimaGCGLproc GetProcAddress(const char* procname);

	protected:
		HWND _hWnd;
		HDC _hDC;
		HGLRC _hContext;

	protected:
		static HINSTANCE _OpenGL32Instance;
		static PFNWGLSWAPINTERVALEXTPROC _SwapIntervalEXT;
		static PFNWGLGETPIXELFORMATATTRIBIVARBPROC _GetPixelFormatAttribivARB;
		static PFNWGLGETEXTENSIONSSTRINGEXTPROC _GetExtensionsStringEXT;
		static PFNWGLGETEXTENSIONSSTRINGARBPROC _GetExtensionsStringARB;
		static PFNWGLCREATECONTEXTATTRIBSARBPROC _CreateContextAttribsARB;
		static bool _EXT_swap_control;
		static bool _ARB_multisample;
		static bool _ARB_framebuffer_sRGB;
		static bool _ARB_pixel_format;
		static bool _ARB_create_context;
		static bool _ARB_create_context_profile;
		static bool _EXT_create_context_es2_profile;
		static bool _ARB_create_context_robustness;
		static bool _ARB_context_flush_control;

		static bool _GLWExtensionsLoaded;
		static bool _GLEWExtensionsLoaded;
		static bool _contextAPIsInitialized;
#else
#endif
};

END_ANIMA_ENGINE_NAMESPACE


#endif //__Anima__AnimaGC__