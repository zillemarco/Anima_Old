#ifndef _AnimaWindow_Data_h
#define _AnimaWindow_Data_h

#include "AnimaEngineCore.h"

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__))
#define _WIN32
#endif

#ifndef APIENTRY
#	ifdef _WIN32
#		define APIENTRY __stdcall
#	else
#		define APIENTRY
#	endif
#endif

#if !defined(WINGDIAPI) && defined(_WIN32)
#	if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__POCC__)
#		define WINGDIAPI __declspec(dllimport)
#	elif defined(__LCC__)
#		define WINGDIAPI __stdcall
#	else
#	define WINGDIAPI extern
#	endif
#	define ANIMA_ENGINE_CORE_WINGDIAPI_DEFINED
#endif

#if !defined(CALLBACK) && defined(_WIN32)
#	if defined(_MSC_VER)
#		if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#			define CALLBACK __stdcall
#		else
#			define CALLBACK
#		endif
#	else
#		define CALLBACK __stdcall
#	endif
#	define ANIMA_ENGINE_CORE_CALLBACK_DEFINED
#endif

#if !defined(ANIMA_ENGINE_CORE_INCLUDE_NONE)
#	include <stddef.h>
#endif

#if defined(__APPLE_CC__)
#	if defined(ANIMA_ENGINE_CORE_INCLUDE_GLCOREARB)
#		include <OpenGL/gl3.h>
#		if defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			include <OpenGL/gl3ext.h>
#		endif
#	elif !defined(ANIMA_ENGINE_CORE_INCLUDE_NONE)
#		if !defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			define GL_GLEXT_LEGACY
#		endif
#		include <OpenGL/gl.h>
#	endif
#	if defined(ANIMA_ENGINE_CORE_INCLUDE_GLU)
#		include <OpenGL/glu.h>
#	endif
#else
#	if defined(ANIMA_ENGINE_CORE_INCLUDE_GLCOREARB)
#		include <GL/glcorearb.h>
#	elif defined(ANIMA_ENGINE_CORE_INCLUDE_ES1)
#		include <GLES/gl.h>
#		if defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			include <GLES/glext.h>
#		endif
#	elif defined(ANIMA_ENGINE_CORE_INCLUDE_ES2)
#		include <GLES2/gl2.h>
#		if defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			include <GLES2/gl2ext.h>
#		endif
#	elif defined(ANIMA_ENGINE_CORE_INCLUDE_ES3)
#		include <GLES3/gl3.h>
#		if defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			include <GLES3/gl2ext.h>
#		endif
#	elif defined(ANIMA_ENGINE_CORE_INCLUDE_ES31)
#		include <GLES3/gl31.h>
#		if defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			include <GLES3/gl2ext.h>
#		endif
#	elif !defined(ANIMA_ENGINE_CORE_INCLUDE_NONE)
#		include <GL/gl.h>
#		if defined(ANIMA_ENGINE_CORE_INCLUDE_GLEXT)
#			include <GL/glext.h>
#		endif
#	endif
#	if defined(ANIMA_ENGINE_CORE_INCLUDE_GLU)
#		include <GL/glu.h>
#	endif
#endif

// Definizione delle chiavi per la tastiera
#define ANIMA_ENGINE_CORE_KEY_UNKNOWN            -1
#define ANIMA_ENGINE_CORE_KEY_SPACE              32
#define ANIMA_ENGINE_CORE_KEY_APOSTROPHE         39  /* ' */
#define ANIMA_ENGINE_CORE_KEY_COMMA              44  /* , */
#define ANIMA_ENGINE_CORE_KEY_MINUS              45  /* - */
#define ANIMA_ENGINE_CORE_KEY_PERIOD             46  /* . */
#define ANIMA_ENGINE_CORE_KEY_SLASH              47  /* / */
#define ANIMA_ENGINE_CORE_KEY_0                  48
#define ANIMA_ENGINE_CORE_KEY_1                  49
#define ANIMA_ENGINE_CORE_KEY_2                  50
#define ANIMA_ENGINE_CORE_KEY_3                  51
#define ANIMA_ENGINE_CORE_KEY_4                  52
#define ANIMA_ENGINE_CORE_KEY_5                  53
#define ANIMA_ENGINE_CORE_KEY_6                  54
#define ANIMA_ENGINE_CORE_KEY_7                  55
#define ANIMA_ENGINE_CORE_KEY_8                  56
#define ANIMA_ENGINE_CORE_KEY_9                  57
#define ANIMA_ENGINE_CORE_KEY_SEMICOLON          59  /* ; */
#define ANIMA_ENGINE_CORE_KEY_EQUAL              61  /* = */
#define ANIMA_ENGINE_CORE_KEY_A                  65
#define ANIMA_ENGINE_CORE_KEY_B                  66
#define ANIMA_ENGINE_CORE_KEY_C                  67
#define ANIMA_ENGINE_CORE_KEY_D                  68
#define ANIMA_ENGINE_CORE_KEY_E                  69
#define ANIMA_ENGINE_CORE_KEY_F                  70
#define ANIMA_ENGINE_CORE_KEY_G                  71
#define ANIMA_ENGINE_CORE_KEY_H                  72
#define ANIMA_ENGINE_CORE_KEY_I                  73
#define ANIMA_ENGINE_CORE_KEY_J                  74
#define ANIMA_ENGINE_CORE_KEY_K                  75
#define ANIMA_ENGINE_CORE_KEY_L                  76
#define ANIMA_ENGINE_CORE_KEY_M                  77
#define ANIMA_ENGINE_CORE_KEY_N                  78
#define ANIMA_ENGINE_CORE_KEY_O                  79
#define ANIMA_ENGINE_CORE_KEY_P                  80
#define ANIMA_ENGINE_CORE_KEY_Q                  81
#define ANIMA_ENGINE_CORE_KEY_R                  82
#define ANIMA_ENGINE_CORE_KEY_S                  83
#define ANIMA_ENGINE_CORE_KEY_T                  84
#define ANIMA_ENGINE_CORE_KEY_U                  85
#define ANIMA_ENGINE_CORE_KEY_V                  86
#define ANIMA_ENGINE_CORE_KEY_W                  87
#define ANIMA_ENGINE_CORE_KEY_X                  88
#define ANIMA_ENGINE_CORE_KEY_Y                  89
#define ANIMA_ENGINE_CORE_KEY_Z                  90
#define ANIMA_ENGINE_CORE_KEY_LEFT_BRACKET       91  /* [ */
#define ANIMA_ENGINE_CORE_KEY_BACKSLASH          92  /* \ */
#define ANIMA_ENGINE_CORE_KEY_RIGHT_BRACKET      93  /* ] */
#define ANIMA_ENGINE_CORE_KEY_GRAVE_ACCENT       96  /* ` */
#define ANIMA_ENGINE_CORE_KEY_WORLD_1            161 /* non-US #1 */
#define ANIMA_ENGINE_CORE_KEY_WORLD_2            162 /* non-US #2 */
#define ANIMA_ENGINE_CORE_KEY_ESCAPE             256
#define ANIMA_ENGINE_CORE_KEY_ENTER              257
#define ANIMA_ENGINE_CORE_KEY_TAB                258
#define ANIMA_ENGINE_CORE_KEY_BACKSPACE          259
#define ANIMA_ENGINE_CORE_KEY_INSERT             260
#define ANIMA_ENGINE_CORE_KEY_DELETE             261
#define ANIMA_ENGINE_CORE_KEY_RIGHT              262
#define ANIMA_ENGINE_CORE_KEY_LEFT               263
#define ANIMA_ENGINE_CORE_KEY_DOWN               264
#define ANIMA_ENGINE_CORE_KEY_UP                 265
#define ANIMA_ENGINE_CORE_KEY_PAGE_UP            266
#define ANIMA_ENGINE_CORE_KEY_PAGE_DOWN          267
#define ANIMA_ENGINE_CORE_KEY_HOME               268
#define ANIMA_ENGINE_CORE_KEY_END                269
#define ANIMA_ENGINE_CORE_KEY_CAPS_LOCK          280
#define ANIMA_ENGINE_CORE_KEY_SCROLL_LOCK        281
#define ANIMA_ENGINE_CORE_KEY_NUM_LOCK           282
#define ANIMA_ENGINE_CORE_KEY_PRINT_SCREEN       283
#define ANIMA_ENGINE_CORE_KEY_PAUSE              284
#define ANIMA_ENGINE_CORE_KEY_F1                 290
#define ANIMA_ENGINE_CORE_KEY_F2                 291
#define ANIMA_ENGINE_CORE_KEY_F3                 292
#define ANIMA_ENGINE_CORE_KEY_F4                 293
#define ANIMA_ENGINE_CORE_KEY_F5                 294
#define ANIMA_ENGINE_CORE_KEY_F6                 295
#define ANIMA_ENGINE_CORE_KEY_F7                 296
#define ANIMA_ENGINE_CORE_KEY_F8                 297
#define ANIMA_ENGINE_CORE_KEY_F9                 298
#define ANIMA_ENGINE_CORE_KEY_F10                299
#define ANIMA_ENGINE_CORE_KEY_F11                300
#define ANIMA_ENGINE_CORE_KEY_F12                301
#define ANIMA_ENGINE_CORE_KEY_F13                302
#define ANIMA_ENGINE_CORE_KEY_F14                303
#define ANIMA_ENGINE_CORE_KEY_F15                304
#define ANIMA_ENGINE_CORE_KEY_F16                305
#define ANIMA_ENGINE_CORE_KEY_F17                306
#define ANIMA_ENGINE_CORE_KEY_F18                307
#define ANIMA_ENGINE_CORE_KEY_F19                308
#define ANIMA_ENGINE_CORE_KEY_F20                309
#define ANIMA_ENGINE_CORE_KEY_F21                310
#define ANIMA_ENGINE_CORE_KEY_F22                311
#define ANIMA_ENGINE_CORE_KEY_F23                312
#define ANIMA_ENGINE_CORE_KEY_F24                313
#define ANIMA_ENGINE_CORE_KEY_F25                314
#define ANIMA_ENGINE_CORE_KEY_KP_0               320
#define ANIMA_ENGINE_CORE_KEY_KP_1               321
#define ANIMA_ENGINE_CORE_KEY_KP_2               322
#define ANIMA_ENGINE_CORE_KEY_KP_3               323
#define ANIMA_ENGINE_CORE_KEY_KP_4               324
#define ANIMA_ENGINE_CORE_KEY_KP_5               325
#define ANIMA_ENGINE_CORE_KEY_KP_6               326
#define ANIMA_ENGINE_CORE_KEY_KP_7               327
#define ANIMA_ENGINE_CORE_KEY_KP_8               328
#define ANIMA_ENGINE_CORE_KEY_KP_9               329
#define ANIMA_ENGINE_CORE_KEY_KP_DECIMAL         330
#define ANIMA_ENGINE_CORE_KEY_KP_DIVIDE          331
#define ANIMA_ENGINE_CORE_KEY_KP_MULTIPLY        332
#define ANIMA_ENGINE_CORE_KEY_KP_SUBTRACT        333
#define ANIMA_ENGINE_CORE_KEY_KP_ADD             334
#define ANIMA_ENGINE_CORE_KEY_KP_ENTER           335
#define ANIMA_ENGINE_CORE_KEY_KP_EQUAL           336
#define ANIMA_ENGINE_CORE_KEY_LEFT_SHIFT         340
#define ANIMA_ENGINE_CORE_KEY_LEFT_CONTROL       341
#define ANIMA_ENGINE_CORE_KEY_LEFT_ALT           342
#define ANIMA_ENGINE_CORE_KEY_LEFT_SUPER         343
#define ANIMA_ENGINE_CORE_KEY_RIGHT_SHIFT        344
#define ANIMA_ENGINE_CORE_KEY_RIGHT_CONTROL      345
#define ANIMA_ENGINE_CORE_KEY_RIGHT_ALT          346
#define ANIMA_ENGINE_CORE_KEY_RIGHT_SUPER        347
#define ANIMA_ENGINE_CORE_KEY_MENU               348
#define ANIMA_ENGINE_CORE_KEY_LAST               ANIMA_ENGINE_CORE_KEY_MENU

// Definizione delle chiavi per il mouse
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_1         0
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_2         1
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_3         2
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_4         3
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_5         4
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_6         5
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_7         6
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_8         7
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_LAST      ANIMA_ENGINE_CORE_MOUSE_BUTTON_8
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_LEFT      ANIMA_ENGINE_CORE_MOUSE_BUTTON_1
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_RIGHT     ANIMA_ENGINE_CORE_MOUSE_BUTTON_2
#define ANIMA_ENGINE_CORE_MOUSE_BUTTON_MIDDLE    ANIMA_ENGINE_CORE_MOUSE_BUTTON_3

// Definizione delle chiavi per il joystick
#define ANIMA_ENGINE_CORE_JOYSTICK_1             0
#define ANIMA_ENGINE_CORE_JOYSTICK_2             1
#define ANIMA_ENGINE_CORE_JOYSTICK_3             2
#define ANIMA_ENGINE_CORE_JOYSTICK_4             3
#define ANIMA_ENGINE_CORE_JOYSTICK_5             4
#define ANIMA_ENGINE_CORE_JOYSTICK_6             5
#define ANIMA_ENGINE_CORE_JOYSTICK_7             6
#define ANIMA_ENGINE_CORE_JOYSTICK_8             7
#define ANIMA_ENGINE_CORE_JOYSTICK_9             8
#define ANIMA_ENGINE_CORE_JOYSTICK_10            9
#define ANIMA_ENGINE_CORE_JOYSTICK_11            10
#define ANIMA_ENGINE_CORE_JOYSTICK_12            11
#define ANIMA_ENGINE_CORE_JOYSTICK_13            12
#define ANIMA_ENGINE_CORE_JOYSTICK_14            13
#define ANIMA_ENGINE_CORE_JOYSTICK_15            14
#define ANIMA_ENGINE_CORE_JOYSTICK_16            15
#define ANIMA_ENGINE_CORE_JOYSTICK_LAST          ANIMA_ENGINE_CORE_JOYSTICK_16

#define ANIMA_ENGINE_CORE_RELEASE	0
#define ANIMA_ENGINE_CORE_PRESS	1
#define ANIMA_ENGINE_CORE_REPEAT	2

#define ANIMA_ENGINE_CORE_OPENGL_API             0x00030001
#define ANIMA_ENGINE_CORE_OPENGL_ES_API          0x00030002

#define ANIMA_ENGINE_CORE_NO_ROBUSTNESS                   0
#define ANIMA_ENGINE_CORE_NO_RESET_NOTIFICATION  0x00031001
#define ANIMA_ENGINE_CORE_LOSE_CONTEXT_ON_RESET  0x00031002

#define ANIMA_ENGINE_CORE_OPENGL_ANY_PROFILE              0
#define ANIMA_ENGINE_CORE_OPENGL_CORE_PROFILE    0x00032001
#define ANIMA_ENGINE_CORE_OPENGL_COMPAT_PROFILE  0x00032002

#define ANIMA_ENGINE_CORE_ANY_RELEASE_BEHAVIOR            0
#define ANIMA_ENGINE_CORE_RELEASE_BEHAVIOR_FLUSH 0x00035001
#define ANIMA_ENGINE_CORE_RELEASE_BEHAVIOR_NONE  0x00035002

typedef void(*AnimaEngineWSglproc)(void);

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct AnimaEngineWSmonitor AnimaEngineWSmonitor;
typedef struct AnimaEngineWSwindow	AnimaEngineWSwindow;
typedef struct AnimaEngineWScursor	AnimaEngineWScursor;

typedef struct AnimaEngineWSvidmode
{
	int _width;
	int _height;
	int _redBits;
	int _greenBits;
	int _blueBits;
	int _refreshRate;
} AnimaEngineWSvidmode;

typedef struct AnimaEngineWSgammaramp
{
	unsigned short* _red;
	unsigned short* _green;
	unsigned short* _blue;
	unsigned int	_size;
} AnimaEngineWSgammaramp;

typedef struct AnimaEngineWSimage
{
	int _width;
	int _height;
	unsigned char* _pixels;
} AnimaEngineWSimage;

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif