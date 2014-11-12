//
//  AnimaEngine.h
//  Anima
//
//  Created by Marco Zille on 08/11/14.
//
//

#ifndef __Anima__AnimaEngine__
#define __Anima__AnimaEngine__

#include "AnimaEngineCore.h"
#include "AnimaAllocator.h"
#include "AnimaStackAllocator.h"
#include "AnimaWindow_Base.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaEngine
{
public:
	AnimaEngine();
	~AnimaEngine();

public:
	bool Initialize();
	void Terminate();

	AnimaEngineWindow_Base* GetWindowListHead() { return _windowListHead;	}

	AnimaEngineWindow_Base* GetFocusedWindow()			{ return _focusedWindow;	}
	void SetFocusedWindow(AnimaEngineWindow_Base* w)	{ _focusedWindow = w;		}

	AnimaEngineWindow_Base* CreateAnimaWindow(int width, int height, const char* title, AnimaEngineWindowmonitor* monitor = NULL, AnimaEngineWindow_Base* share = NULL);
	void DestroyAnimaWindow(AnimaEngineWindow_Base* window);

	void PollEvents(void);
	void WaitEvents(void);
	void PostEmptyEvent(void);

	double GetTime(void);
	void SetTime(double time);
	void MakeContextCurrent(AnimaEngineWindow_Base* window);
	AnimaEngineWindow_Base* GetCurrentContext(void);
	void SwapInterval(int interval);
	
	static void GetVersion(int* major, int* minor, int* rev);
	static const char* GetVersionString(void);

	static void DefaultWindowHints(void);
	static void SetWindowHint(int target, int hint);
	static void SetWindowHint(int target, bool hint);

	static AnimaEngineWindowmonitor* GetPrimaryMonitor(void);
	static void GetMonitorPos(AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos);
	static void GetMonitorPhysicalSize(AnimaEngineWindowmonitor* monitor, int* width, int* height);
	static const char* GetMonitorName(AnimaEngineWindowmonitor* monitor);
	
	static AnimaEngineWindowmonitorfun SetMonitorCallback(AnimaEngineWindowmonitorfun cbfun);
	
	static const AnimaEngineWindowvidmode* GetVideoModes(AnimaEngineWindowmonitor* monitor, int* count);
	static const AnimaEngineWindowvidmode* GetVideoMode(AnimaEngineWindowmonitor* monitor);
	
	static void SetGamma(AnimaEngineWindowmonitor* monitor, float gamma);
	static const AnimaEngineWindowgammaramp* GetGammaRamp(AnimaEngineWindowmonitor* monitor);
	static void SetGammaRamp(AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowgammaramp* ramp);
	
	static AnimaEngineWindowcursor* CreateCursor(const AnimaEngineWindowimage* image, int xhot, int yhot);
	static void DestroyCursor(AnimaEngineWindowcursor* cursor);

	static bool JoystickPresent(int joy);
	
	static const float* GetJoystickAxes(int joy, int* count);
	static const unsigned char* GetJoystickButtons(int joy, int* count);
	static const char* GetJoystickName(int joy);
	static bool ExtensionSupported(const char* extension);
	
public:
	static bool IsInitialized() { return _animaEngineInitialized; }

	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE;

public:
	static _AnimaEngineWindowmonitor**	_monitors;				/*!< Lista dei monitor disponibili per il sistema */
	static int							_monitorCount;			/*!< Numero di monitor disponibili per il sistema */

	static _AnimaEngineWindowMonitorCallbacks _callbacks;

private:	
	AnimaEngineWindow_Base*	_windowListHead;					/*!< Primo elemento della lista delle finestre istanziate da un'istanza di AnimaEngine */
	AnimaEngineWindow_Base*	_focusedWindow;						/*!< Puntatore alla finestra attualmente attiva tra la lista di quelle istanziate da un'istanza di AnimaEngine */

	static _AnimaEngineWindowHints _windowHints;				/*!< Contenitore di dati per inizializzare un contesto di rendering su una finestra */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione delle finestre */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione di OpenGL */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE;		/*!< Contenitore con dati per la gestione del timer */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE;	/*!< Contenitore con dati per la gestione del joystick */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE;		/*!< Contenitore con dati per la gestione dei thread */
	static bool _animaEngineInitialized;						/*!< Flag per indicare se AnimaEngine è stato inizializzato con successo */
	static int	_animaEngineCount;								/*!< Contatore di istanze di AnimaEngine */
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#define _ANIMA_ENGINE_CORE_REQUIRE_INIT()					\
    if (!Anima::AnimaEngine::IsInitialized())				\
	{														\
		/*_glfwInputError(GLFW_NOT_INITIALIZED, NULL);*/	\
		return;												\
	}

#define _ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(x)		\
    if (!Anima::AnimaEngine::IsInitialized())				\
	{														\
        /*_glfwInputError(GLFW_NOT_INITIALIZED, NULL);*/	\
        return x;											\
	}

#define _ANIMA_ENGINE_CORE_SWAP_POINTERS(x, y, type)	\
	{													\
        void* t;										\
        t = (void*)x;									\
        x = y;											\
        y = (type)t;									\
	}

#endif