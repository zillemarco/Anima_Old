//
//  AnimaWindow_Structures.h
//  Anima
//
//  Created by Marco Zille on 11/11/14.
//
//

#ifndef Anima_AnimaWindow_Data_h
#define Anima_AnimaWindow_Data_h

#include <stdio.h>
#include "AnimaEngineWS.h"

BEGIN_ANIMA_ENGINE_WS_NAMESPACE

typedef struct AnimaEngineWSmonitor AnimaEngineWSmonitor;
typedef struct AnimaEngineWSwindow	AnimaEngineWSwindow;
typedef struct AnimaEngineWScursor	AnimaEngineWScursor;

typedef struct _AnimaEngineWSwndconfig   _AnimaEngineWSwndconfig;
typedef struct _AnimaEngineWSctxconfig   _AnimaEngineWSctxconfig;
typedef struct _AnimaEngineWSfbconfig    _AnimaEngineWSfbconfig;
typedef struct _AnimaEngineWSwindow      _AnimaEngineWSwindow;
typedef struct _AnimaEngineWSlibrary     _AnimaEngineWSlibrary;
typedef struct _AnimaEngineWSmonitor     _AnimaEngineWSmonitor;
typedef struct _AnimaEngineWScursor      _AnimaEngineWScursor;

typedef struct AnimaEngineWSvidmode
{
	/*! The width, in screen coordinates, of the video mode.
	 */
	int width;
	/*! The height, in screen coordinates, of the video mode.
	 */
	int height;
	/*! The bit depth of the red channel of the video mode.
	 */
	int redBits;
	/*! The bit depth of the green channel of the video mode.
	 */
	int greenBits;
	/*! The bit depth of the blue channel of the video mode.
	 */
	int blueBits;
	/*! The refresh rate, in Hz, of the video mode.
	 */
	int refreshRate;
} AnimaEngineWSvidmode;

typedef struct AnimaEngineWSgammaramp
{
	/*! An array of value describing the response of the red channel.
	 */
	unsigned short* red;
	/*! An array of value describing the response of the green channel.
	 */
	unsigned short* green;
	/*! An array of value describing the response of the blue channel.
	 */
	unsigned short* blue;
	/*! The number of elements in each array.
	 */
	unsigned int size;
} AnimaEngineWSgammaramp;

typedef struct AnimaEngineWSimage
{
	/*! The width, in pixels, of this image.
	 */
	int width;
	/*! The height, in pixels, of this image.
	 */
	int height;
	/*! The pixel data of this image, arranged left-to-right, top-to-bottom.
	 */
	unsigned char* pixels;
} AnimaEngineWSimage;

// struttura con i dati della finestra
struct _AnimaEngineWSwndconfig
{
	int			_width;
	int			_height;
	const char* _title;
	bool		_resizable;
	bool		_visible;
	bool		_decorated;
	bool		_focused;
	bool		_autoIconify;
	bool		_floating;
	
	_AnimaEngineWSmonitor* _monitor;
};

// struttura con i dati del contesto
struct _AnimaEngineWSctxconfig
{
	int		_api;
	int		_major;
	int		_minor;
	bool	_forward;
	bool	_debug;
	int		_profile;
	int		_robustness;
	int		_release;
	_AnimaEngineWSwindow*  _share;
};

// struttura con i dati del framwbuffer
struct _AnimaEngineWSfbconfig
{
	int	_redBits;
	int	_greenBits;
	int	_blueBits;
	int	_alphaBits;
	int	_depthBits;
	int	_stencilBits;
	int	_accumRedBits;
	int	_accumGreenBits;
	int	_accumBlueBits;
	int	_accumAlphaBits;
	int	_auxBuffers;
	int	_stereo;
	int	_samples;
	int	_sRGB;
	int	_doublebuffer;
	
	//	// This is defined in the context API's context.h
	//	_GLFW_PLATFORM_FBCONFIG;
};

// struttura dati contenente i dati di un monitor
struct _AnimaEngineWSmonitor
{
	char*	_name;
	int		_widthMM, _heightMM;
	
	AnimaEngineWSvidmode*	modes;
	int						modeCount;
	AnimaEngineWSvidmode	currentMode;
	
	AnimaEngineWSgammaramp	originalRamp;
	AnimaEngineWSgammaramp	currentRamp;
	
	//	// This is defined in the window API's platform.h
	//	_GLFW_PLATFORM_MONITOR_STATE;
};

struct _AnimaEngineWScursor
{
	_AnimaEngineWScursor*	_next;
	//	// This is defined in the window API's platform.h
	//	_GLFW_PLATFORM_CURSOR_STATE;
};

END_ANIMA_ENGINE_WS_NAMESPACE

#endif
