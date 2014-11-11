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

#pragma region Dati di inizializzatione della libreria
public:
	ANIMA_ENGINE_WINDOW_HINTS _windowHints;			/*!< Contenitore di dati per inizializzare un contesto di rendering su una finestra */

	// Dovrebbero essere spostati sulla finestra
	//double _cursorPosX_cursorPosY;
	//_AnimaEngineWindowcursor*	_cursorListHead;

	_AnimaEngineWindowwindow*	_windowListHead;	/*!< Primo elemento della lista delle finestre istanziate da un'istanza di AnimaEngine */
	_AnimaEngineWindowwindow*	_focusedWindow;		/*!< Puntatore alla finestra attualmente attiva tra la lista di quelle istanziate da un'istanza di AnimaEngine */

	_AnimaEngineWindowmonitor**	_monitors;			/*!< Lista dei monitor disponibili per il sistema */
	int							_monitorCount;		/*!< Numero di monitor disponibili per il sistema */

	//struct {
	//	AnimaEngineWindowmonitorfun	_monitor;
	//} _callbacks;

	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione delle finestre */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione di OpenGL */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE;		/*!< Contenitore con dati per la gestione del timer */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE;	/*!< Contenitore con dati per la gestione del joystick */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE;		/*!< Contenitore con dati per la gestione dei thread */

	static bool _animaEngineInitialized;	/*!< Flag per indicare se AnimaEngine è stato inizializzato con successo */
#pragma endregion
};

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif