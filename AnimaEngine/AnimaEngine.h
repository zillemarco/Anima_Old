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
#include "AnimaAllocators.h"
#include "AnimaWindow.h"
#include "AnimaString.h"
#include "AnimaEngineConfig.h"

#if defined _MSC_VER
#define PATH_MAX MAX_PATH
#endif

#define _ANIMA_ENGINE_REQUIRE_INIT()					\
    if (!Anima::AnimaEngine::IsInitialized())				\
		{													\
		/*_glfwInputError(GLFW_NOT_INITIALIZED, NULL);*/	\
		return;												\
		}

#define _ANIMA_ENGINE_REQUIRE_INIT_OR_RETURN(x)		\
    if (!Anima::AnimaEngine::IsInitialized())				\
		{													\
        /*_glfwInputError(GLFW_NOT_INITIALIZED, NULL);*/	\
        return x;											\
		}

#define _ANIMA_ENGINE_SWAP_POINTERS(x, y, type)	\
		{												\
        void* t;										\
        t = (void*)x;									\
        x = y;											\
        y = (type)t;									\
		}

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef AnimaEngineWindow_Base AnimaWindow;

class AnimaScenesManager;
class AnimaShadersManager;
class AnimaDataGeneratorsManager;

/*!
 *	\brief		Classe principale di AnimaEngine
 *	\details	Questa classe è il cuore di tutto AnimaEngine. Contiene tutti i componenti necessari per
 *				poter inizializzare e cominciare ad utilizzare AnimaEngine
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaEngine
{
public:
	/*!
	 *	\brief		Costruttore della classe AnimaEngine
	 *	\author		Zille Marco
	 */
	AnimaEngine();
	
	/*!
	 *	\brief		Distruttore della classe AnimaEngine
	 *	\details	Distruttore della classe AnimaEngine. Viene anche chiamato il metodo Terminate() in modo da 
					termiare tutti i componenti ancora attivi di AnimaEngine
	 *	\author		Zille Marco
	 *	\sa			Terminate()
	 */
	~AnimaEngine();

public:
	/*!
	 *	\brief		Inizializzazione di AnimaEngine
	 *	\details	Questo metodo deve essere chiamato prima di poter effettuare qualsiasi operazione
					attraverso l'uso di AnimaEngine. Si occupa di inizializzare la memoria utilizzata poi dalle
					varie classi manager di AnimaEngine
	 *	\author		Zille Marco
	 */
	bool Initialize();
	
	/*!
	 *	\brief		Deinizializzazione di AnimaEngine
	 *	\details	Questo metodo si occupa di deinizializzare e ripulire tutta la memoria utilizzata da AnimaEngine
					e da tutti i suoi manager in generale.
					Non si deve necessariamente richiamare sempre perchè richiamato dal distrutture della classe ~AnimaEngine()
	 *	\sa			~AnimaEngine()
	 *	\author		Zille Marco
	 */
	void Terminate();

	AnimaWindow* GetWindowListHead() { return _windowListHead; }

	AnimaWindow* GetFocusedWindow()			{ return _focusedWindow; }
	void SetFocusedWindow(AnimaWindow* w)	{ _focusedWindow = w; }

	template<class T>
	T* CreateAnimaWindow(int width, int height, const char* title, AnimaEngineWindowmonitor* monitor = NULL, AnimaWindow* share = NULL);

	void DestroyAnimaWindow(AnimaEngineWindow_Base* window);

	void PollEvents(void);
	void WaitEvents(void);
	void PostEmptyEvent(void);

	double GetTime(void);
	void SetTime(double time);
	double GetTimeResolution() { return _GET_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE->_resolution; }

	void MakeContextCurrent(AnimaWindow* window);
	AnimaWindow* GetCurrentContext(void);

	static void SwapInterval(int interval);

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
	
	/*!
	 *	\brief		Imposta la modalità di utilizzo di AnimaEngine
	 *	\details	Questo metodo imposta la modalità di utilizzo di AnimaEngine. Se AnimaEngine 
					viene utilizzato come estensione per un'altro programma, deve essere chiamata questa funzione
					con parametro true.
					Questo valore vale per tutte le istanze della classe AnimaEngine utilizzate all'interno dello stesso programma
	 *	\param[in]	bUsedExternal	valore che verrà assegnato al membro _usedExternal
	 *	\sa			_usedExternal
	 *	\author		Zille Marco
	 */
	static void SetUsedExternal(bool bUsedExternal = true) { _usedExternal = bUsedExternal; }
	
	/*!
	 *	\brief		Ritorna la modalità di utilizzo impostata di AnimaEngine
	 *	\details	Metodo che ritorna la modalità di utilizzo impostata di AnimaEngine
	 *	\return		bool	valore del membro _usedExternal
	 *	\sa			_usedExternal
	 *	\author		Zille Marco
	 */
	static bool IsUsedExteral() { return _usedExternal; }
	
	/*!
	 *	\brief		Inizializza le estensioni di GLEW per l'utilizzo di OpenGL
	 *	\details	Metodo per inizializzare le estensioni di GLEW per l'utilizzo di OpenGL.
					Se sono già state inizializzate in precedenza torna il metodo termina senza reinizializzarle.
	 *	\return		Torna true se le estensioni vengono inizializzate correttamente o erano già state
					inizializzate correttamente, altrimenti false
	 *	\sa			IsGlewExtensionsInitialized(), SetGlewExtensionsInitialized()
	 *	\author		Zille Marco
	 */
	static bool InitializeGlewExtensions();
	
	/*!
	 *	\brief		Metodo che serve per sapere se le estensioni GLEW sono state inizializzate
	 *	\details	Metodo che serve per sapere se le estensioni GLEW sono state inizializzate
	 *	\return		Torna il valore di _glewExtensionsInitialized
	 *	\sa			InitializeGlewExtensions(), SetGlewExtensionsInitialized(), _glewExtensionsInitialized
	 *	\author		Zille Marco
	 */
	static bool IsGlewExtensionsInitialized()			{ return _glewExtensionsInitialized; }
	
	/*!
	 *	\brief		Imposta il valore di _glewExtensionsInitialized
	 *	\details	Imposta il valore di _glewExtensionsInitialized.
					Può servire nel caso in cui si volesse reinizializzare le estensioni GLEW per qualche motivo
	 *	\param[in]	bSet	Valore da assegnare a _glewExtensionsInitialized
	 *	\sa			InitializeGlewExtensions(), SetGlewExtensionsInitialized()
	 *	\author		Zille Marco
	 */
	static void SetGlewExtensionsInitialized(bool bSet) { _glewExtensionsInitialized = bSet; }
	
	/*!
	 *	\brief		Imposta il percorso del file di log di AnimaEngine
	 *	\details	Imposta il percorso del file di log di AnimaEngine in cui verranno scritti tutti i log
					degli eventi di errore o di warning
	 *	\param[in]	bSet	Valore da assegnare a _glewExtensionsInitialized
	 *	\sa			GetLogFilePath()
	 *	\author		Zille Marco
	 */
	static void SetLogFilePath(const AChar* path);
	
	/*!
	 *	\brief		Torna il percorso del file di log di AnimaEngine
	 *	\details	Torna il percorso del file di log di AnimaEngine
	 *	\return		Torna il valore di _logFilePath
	 *	\sa			SetLogFilePath()
	 *	\author		Zille Marco
	 */
	static AChar* GetLogFilePath();

public:
	/*!
	 *	\brief		Metodo per sapere se l'inizializzazione di AnimaEngine è stata eseguita in modo corretto
	 *	\details	Metodo per sapere se l'inizializzazione di AnimaEngine è stata eseguita in modo corretto
	 *	\return		Torna il valore di _animaEngineInitialized
	 *	\sa			Initialize()
	 *	\author		Zille Marco
	 */
	static bool IsInitialized() { return _animaEngineInitialized; }

	static _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE;
	static _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE;
	static _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE;
	static _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE;
	static _GETD_ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE;

public:
	/*!
	 *	\brief		Metodo che fa un dump di tutta la memoria utilizzata da AnimaEngine
	 *	\details	Metodo che fa un dump di tutta la memoria utilizzata da AnimaEngine indicando tutti i vari allocators,
					la quantità di memoria a loro assegnata e la quantità di quella occupata
	 *	\param[in]	fileName	Percorso di un file in cui verrà scritto il risultato del dump. Questo parametro viene utilizzato solo se bLogToFile viene passato a true
	 *	\param[in]	bLogToFile	Indica se il dump deve essere salvato su di un file (il cui percorso viene indicato dal parametro fileName) oppure deve essere solo stampato a console
	 *	\author		Zille Marco
	 */
	void DumpMemory(const char* fileName, bool bLogToFile = true);

private:
	/*!
	 *	\brief		Metodo che inizializza la memoria di AnimaEngine
	 *	\details	Metodo che alloca la memoria utilizzata da AnimaEngine e la redistribuisce ai vari allocators
	  *	\sa			TerminateManagers(), TerminateMemorySystem(), InitializeManagers()
	 *	\author		Zille Marco
	 */
	void InitializeMemorySystem();
	
	bool InitializeWindowSystem();
	
	/*!
	 *	\brief		Inizializza i manager di AnimaEngine che sono in comune tra le varie scene
	 *	\details	Metodo che inizializza i manager di AnimaEngine che sono in comune tra le varie scene
	 *	\author		Zille Marco
	 */
	void InitializeManagers();
	
	/*!
	 *	\brief		Deinizializza la memoria di AnimaEngine
	 *	\details	Metodo che deinizializza e dealloca tutta la memoria che era stata utilizzata da AnimaEngine facendo anche un controllo che tutti gli 
					oggetti siano stati distrutti corretamente durante l'esecuzione di AnimaEngine
 	 *	\sa			TerminateManagers(), InitializeMemorySystem(), InitializeManagers()
	 *	\author		Zille Marco
	 */
	void TerminateMemorySystem();
	void TerminateWindowSystem();
	
	/*!
	 *	\brief		Deinizializza i manager che sono in comune tra le varie scene
	 *	\details	Metodo che deinizializza i manager che sono in comune tra le varie scene eliminando tutti i loro dati e rilasciando la memoria da loro utilizzata
	 *	\sa			TerminateMemorySystem(), InitializeMemorySystem(), InitializeManagers()
	 *	\author		Zille Marco
	 */
	void TerminateManagers();

public:
	static _AnimaEngineWindowmonitor**	_monitors;				/*!< Lista dei monitor disponibili per il sistema */
	static int							_monitorCount;			/*!< Numero di monitor disponibili per il sistema */

	static _AnimaEngineWindowMonitorCallbacks _callbacks;

private:
	AnimaWindow*	_windowListHead;					/*!< Primo elemento della lista delle finestre istanziate da un'istanza di AnimaEngine */
	AnimaWindow*	_focusedWindow;						/*!< Puntatore alla finestra attualmente attiva tra la lista di quelle istanziate da un'istanza di AnimaEngine */

	static _AnimaEngineWindowHints _windowHints;			/*!< Contenitore di dati per inizializzare un contesto di rendering su una finestra */
	static _ANIMA_ENGINE_PLATFORM_LIBRARY_WINDOW_STATE;		/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione delle finestre */
	static _ANIMA_ENGINE_PLATFORM_LIBRARY_CONTEXT_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione di OpenGL */
	static _ANIMA_ENGINE_PLATFORM_LIBRARY_TIME_STATE;		/*!< Contenitore con dati per la gestione del timer */
	static _ANIMA_ENGINE_PLATFORM_LIBRARY_JOYSTICK_STATE;	/*!< Contenitore con dati per la gestione del joystick */
	static _ANIMA_ENGINE_PLATFORM_LIBRARY_TLS_STATE;		/*!< Contenitore con dati per la gestione dei thread */
	static bool _animaEngineInitialized;					/*!< Flag per indicare se AnimaEngine Ë stato inizializzato con successo */
	static int	_animaEngineCount;							/*!< Contatore di istanze di AnimaEngine */
	
	static bool _platformLibraryWindowStateInitialized;
	static bool _platformLibraryContextStateInitialized;
	static bool _platformLibraryTimeStateInitialized;
	static bool _platformLibraryJoystickStateInitialized;
	static bool _platformLibraryTLSStateInitialized;
	static bool _glewExtensionsInitialized;

	static bool _usedExternal;						/*!< Flag che indica se AnimaEngine è utilizzato all'interno di un altro programma */

private:
	static AChar _logFilePath[PATH_MAX];			/*!< Path del file di log di AnimaEngine */

public:
	/*!
	 *	\brief		Torna l'allocator dei modelli 3D (classe AnimaModel)
	 *	\details	Torna l'allocator dei modelli 3D (classe AnimaModel) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _modelsAllocator
	 *	\sa			_modelsAllocator, AnimaModel, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetModelsAllocator() {
		ANIMA_ASSERT(_modelsAllocator != nullptr);
		return _modelsAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator generico
	 *	\details	Torna l'allocator generico che può essere utilizzato da qualsiasi classe di AnimaEngine
	 *	\return		Il valore di _genericAllocator
	 *	\sa			_genericAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetGenericAllocator() {
		ANIMA_ASSERT(_genericAllocator != nullptr);
		return _genericAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle mesh dei modelli 3D (classe AnimaMesh)
	 *	\details	Torna l'allocator delle mesh dei modelli 3D (classe AnimaMesh) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _meshesAllocator
	 *	\sa			_meshesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetMeshesAllocator() {
		ANIMA_ASSERT(_meshesAllocator != nullptr);
		return _meshesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle istanze dei modelli 3D (classe AnimaModelInstance)
	 *	\details	Torna l'allocator delle istanze dei modelli 3D (classe AnimaModelInstance) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _modelInstancesAllocator
	 *	\sa			_modelInstancesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetModelInstancesAllocator() {
		ANIMA_ASSERT(_modelInstancesAllocator != nullptr);
		return _modelInstancesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle istanze delle mesh dei modelli 3D (classe AnimaMesh)
	 *	\details	Torna l'allocator delle istanze delle mesh dei modelli 3D (classe AnimaMesh) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _meshInstancesAllocator
	 *	\sa			_meshInstancesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetMeshInstancesAllocator() {
		ANIMA_ASSERT(_meshInstancesAllocator != nullptr);
		return _meshInstancesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle stringe usate dalla classe AnimaString ed in generate da tutte le classi
	 *	\details	Torna l'allocator delle stringe usate dalla classe AnimaString ed in generate da tutte le classi
	 *	\return		Il valore di _stringAllocator
	 *	\sa			_stringAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetStringAllocator() {
		ANIMA_ASSERT(_stringAllocator != nullptr);
		return _stringAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator degli shader. Usato dalle classi AnimaShadersManager, AnimaShader e AnimaShaderProgram
	 *	\details	Torna l'allocator degli shader. Usato dalle classi AnimaShadersManager, AnimaShader e AnimaShaderProgram
	 *	\return		Il valore di _shadersAllocator
	 *	\sa			_shadersAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetShadersAllocator() {
		ANIMA_ASSERT(_shadersAllocator != nullptr);
		return _shadersAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle camere (telecamere). Usato dalle classi AnimaCamerasManager, AnimaCamera e derivate
	 *	\details	Torna l'allocator delle camere (telecamere). Usato dalle classi AnimaCamerasManager, AnimaCamera e derivate
	 *	\return		Il valore di _camerasAllocator
	 *	\sa			_cameraAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetCamerasAllocator() {
		ANIMA_ASSERT(_camerasAllocator != nullptr);
		return _camerasAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle texture. Usato dalle classi AnimaTexturesManager, AnimaTexture e derivate
	 *	\details	Torna l'allocator delle texture. Usato dalle classi AnimaTexturesManager, AnimaTexture e derivate
	 *	\return		Il valore di _texturesAllocator
	 *	\sa			_texturesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetTexturesAllocator() {
		ANIMA_ASSERT(_texturesAllocator != nullptr);
		return _texturesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator dei materiali. Usato dalle classi AnimaMaterialsManager, AnimaMaterial e derivate
	 *	\details	Torna l'allocator dei materiali. Usato dalle classi AnimaMaterialsManager, AnimaMaterial e derivate
	 *	\return		Il valore di _materialsAllocator
	 *	\sa			_materialsAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetMaterialsAllocator() {
		ANIMA_ASSERT(_materialsAllocator != nullptr);
		return _materialsAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle luci. Usato dalle classi AnimaLigtsManager, AnimaLight e derivate
	 *	\details	Torna l'allocator delle luci. Usato dalle classi AnimaLigtsManager, AnimaLight e derivate
	 *	\return		Il valore di _lightsAllocator
	 *	\sa			_lightsAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetLightsAllocator() {
		ANIMA_ASSERT(_lightsAllocator != nullptr);
		return _lightsAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator dei generatori di dati. Usato dalle classi AnimaDataGeneratorsManager, AnimaDataGenerator e derivate
	 *	\details	Torna l'allocator dei generatori di dati. Usato dalle classi AnimaDataGeneratorsManager, AnimaDataGenerator e derivate
	 *	\return		Il valore di _dataGeneratorsAllocator
	 *	\sa			_dataGeneratorsAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetDataGeneratorsAllocator() {
		ANIMA_ASSERT(_dataGeneratorsAllocator != nullptr);
		return _dataGeneratorsAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle scene. Usato dalle classi AnimaScenesManager e AnimaScene
	 *	\details	Torna l'allocator delle scene. Usato dalle classi AnimaScenesManager e AnimaScene
	 *	\return		Il valore di _scenesAllocator
	 *	\sa			_scenesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetScenesAllocator() {
		ANIMA_ASSERT(_scenesAllocator != nullptr);
		return _scenesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle animazioni. Usato dalle classi AnimaAnimationsManager e AnimaAnimation
	 *	\details	Torna l'allocator delle animazioni. Usato dalle classi AnimaAnimationsManager e AnimaAnimation
	 *	\return		Il valore di _animationsManager
	 *	\sa			_animationsManager, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetAnimationsAllocator() {
		ANIMA_ASSERT(_animationsAllocator != nullptr);
		return _animationsAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator dei vari manager usati da AnimaEngine
	 *	\details	Torna l'allocator dei vari manager usati da AnimaEngine
	 *	\return		Il valore di _managersAllocator
	 *	\sa			_managersAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetManagersAllocator() {
		ANIMA_ASSERT(_managersAllocator != nullptr);
		return _managersAllocator;
	}
	
	/*!
	 *	\brief		Torna il manager degli shader utilizzati da AnimaEngine
	 *	\details	Torna il manager degli shader utilizzati da AnimaEngine
	 *	\return		Il valore di _shadersManager
	 *	\sa			_shadersManager
	 *	\author		Zille Marco
	 */
	inline AnimaShadersManager* GetShadersManager() {
		ANIMA_ASSERT(_shadersManager != nullptr);
		return _shadersManager;
	}
	
	/*!
	 *	\brief		Torna il manager delle scene di AnimaEngine
	 *	\details	Torna il manager delle scene di AnimaEngine
	 *	\return		Il valore di _scenesManager
	 *	\sa			_scenesManager
	 *	\author		Zille Marco
	 */
	inline AnimaScenesManager* GetScenesManager() {
		ANIMA_ASSERT(_scenesManager != nullptr);
		return _scenesManager;
	}
	
	/*!
	 *	\brief		Torna il manager dei generatori di dati di AnimaEngine
	 *	\details	Torna il manager dei generatori di dati di AnimaEngine. In genere un generatore di dati è legato ad una scene ma, in alcuni casi
					(come ad esempio per i generatori di dati usati nella classe AnimaRenderer e derivate che non sono legati ad una scena), può essere necessatio
					avere a disposizione un manager dei generatori di dati generico.
	 *	\return		Il valore di _dataGeneratorsManager
	 *	\sa			_dataGeneratorsManager
	 *	\author		Zille Marco
	 */
	inline AnimaDataGeneratorsManager* GetDataGeneratorsManager() {
		ANIMA_ASSERT(_dataGeneratorsManager != nullptr);
		return _dataGeneratorsManager;
	}

protected:
	void*	_localMemory;								/*!< Buffer della memoria principale di AnimaEngine a cui puntano gli allocator */
	ASizeT	_localMemorySize;							/*!< Dimensione del buffer della memoria principale di AnimaEngine a cui puntano gli allocator */
	AnimaFreeListAllocator*	_localMemoryAllocator;		/*!< Allocator usato per creare tutti gli altri allocator */

	AnimaFreeListAllocator* _genericAllocator;			/*!< Allocator usato genericamente, senza un utilizzo in particolare */
	AnimaFreeListAllocator* _managersAllocator;			/*!< Allocator usato all'interno di AnimaEngine per costruire i vari manager */
	AnimaFreeListAllocator* _stringAllocator;			/*!< Allocator usato dalla classe AnimaString e per le stringhe in generale */
	AnimaFreeListAllocator* _shadersAllocator;			/*!< Allocator usato dalla classe AnimaShader, AnimaShaderProgram e AnimaShadersManager */
	AnimaFreeListAllocator* _camerasAllocator;			/*!< Allocator usato dalla classe AnimaCamerasManager, AnimaCamera e derivate */
	AnimaFreeListAllocator* _texturesAllocator;			/*!< Allocator usato dalla classe AnimaTexturesManager, AnimaTexture e derivate */
	AnimaFreeListAllocator* _materialsAllocator;		/*!< Allocator usato dalla classe AnimaMaterialsManager, AnimaMaterial e derivate */
	AnimaFreeListAllocator* _lightsAllocator;			/*!< Allocator usato dalla classe AnimaLightsManager, AnimaLight e derivate */
	AnimaFreeListAllocator* _dataGeneratorsAllocator;	/*!< Allocator usato dalla classe AnimaDataGeneratorsManager, AnimaDataGenerator e derivate */
	AnimaFreeListAllocator* _scenesAllocator;			/*!< Allocator usato dalla classe AnimaScenesManager e AnimaScene */
	AnimaFreeListAllocator* _animationsAllocator;		/*!< Allocator usato dalla classe AnimaAnimationsManager e AnimaAnimation */
	AnimaFreeListAllocator* _meshesAllocator;			/*!< Allocator usato dalla classe AnimaMeshesManager e AnimaMesh */
	AnimaFreeListAllocator* _modelsAllocator;			/*!< Allocator usato dalla classe AnimaModelsManager e AnimaModel */
	AnimaFreeListAllocator* _modelInstancesAllocator;	/*!< Allocator usato dalla classe AnimaModelInstancesManager e AnimaModelInstance */
	AnimaFreeListAllocator* _meshInstancesAllocator;	/*!< Allocator usato dalla classe AnimaMeshInstancesManager e AnimaMeshInstance */

private:
	AnimaScenesManager*			_scenesManager;			/*!< Manager delle scene create da AnimaEngine */
	AnimaShadersManager*		_shadersManager;		/*!< Manager di tutti gli shader dell'istanza corrente di AnimaEngine */
	AnimaDataGeneratorsManager* _dataGeneratorsManager;	/*!< Manager di tutti i data generator che non sono legati ad una scena specifica (vedi esempio in AnimaRenderer) */
};

template<class T>
T* AnimaEngine::CreateAnimaWindow(int width, int height, const char* title, AnimaEngineWindowmonitor* monitor, AnimaEngineWindow_Base* share)
{
	_AnimaEngineWindowfbconfig fbconfig;
	_AnimaEngineWindowctxconfig ctxconfig;
	_AnimaEngineWindowwndconfig wndconfig;
	AnimaEngineWindow_Base* previous;

	T* window;

	_ANIMA_ENGINE_REQUIRE_INIT_OR_RETURN(NULL);

	if (width <= 0 || height <= 0)
	{
		//_glfwInputError(GLFW_INVALID_VALUE, "Invalid window size");
		return NULL;
	}

	// Set up desired framebuffer config
	fbconfig._redBits = _windowHints._redBits;
	fbconfig._greenBits = _windowHints._greenBits;
	fbconfig._blueBits = _windowHints._blueBits;
	fbconfig._alphaBits = _windowHints._alphaBits;
	fbconfig._depthBits = _windowHints._depthBits;
	fbconfig._stencilBits = _windowHints._stencilBits;
	fbconfig._accumRedBits = _windowHints._accumRedBits;
	fbconfig._accumGreenBits = _windowHints._accumGreenBits;
	fbconfig._accumBlueBits = _windowHints._accumBlueBits;
	fbconfig._accumAlphaBits = _windowHints._accumAlphaBits;
	fbconfig._auxBuffers = _windowHints._auxBuffers;
	fbconfig._stereo = _windowHints._stereo;
	fbconfig._samples = _windowHints._samples;
	fbconfig._sRGB = _windowHints._sRGB;
	fbconfig._doublebuffer = _windowHints._doublebuffer;

	// Set up desired window config
	wndconfig._width = width;
	wndconfig._height = height;
	wndconfig._title = title;
	wndconfig._resizable = _windowHints._resizable;
	wndconfig._visible = _windowHints._visible;
	wndconfig._decorated = _windowHints._decorated;
	wndconfig._focused = _windowHints._focused;
	wndconfig._autoIconify = _windowHints._autoIconify;
	wndconfig._floating = _windowHints._floating;
	wndconfig._monitor = (_AnimaEngineWindowmonitor*)monitor;

	// Set up desired context config
	ctxconfig._api = _windowHints._api;
	ctxconfig._major = _windowHints._major;
	ctxconfig._minor = _windowHints._minor;
	ctxconfig._forward = _windowHints._forward;
	ctxconfig._debug = _windowHints._debug;
	ctxconfig._profile = _windowHints._profile;
	ctxconfig._robustness = _windowHints._robustness;
	ctxconfig._release = _windowHints._release;
	ctxconfig._share = (AnimaEngineWindow_Base*)share;

	// Check the OpenGL bits of the window config
	if (!_AnimaEngineWindowIsValidContextConfig(&ctxconfig))
		return NULL;

	window = new T();
	window->InitCallbacks();
	window->SetEngine(this);
	window->SetNext(_windowListHead);
	_windowListHead = window;

	if (wndconfig._monitor)
	{
		wndconfig._resizable = true;
		wndconfig._visible = true;
		wndconfig._focused = true;

		// Set up desired video mode
		window->GetVideoMode()->_width = width;
		window->GetVideoMode()->_height = height;
		window->GetVideoMode()->_redBits = _windowHints._redBits;
		window->GetVideoMode()->_greenBits = _windowHints._greenBits;
		window->GetVideoMode()->_blueBits = _windowHints._blueBits;
		window->GetVideoMode()->_refreshRate = _windowHints._refreshRate;
	}

	// Transfer window hints that are persistent settings and not
	// just initial states
	window->SetMonitor(wndconfig._monitor);
	window->SetResizable(wndconfig._resizable);
	window->SetDecorated(wndconfig._decorated);
	window->SetAutoIconifiy(wndconfig._autoIconify);
	window->SetFloating(wndconfig._floating);
	window->SetCursorMode(ANIMA_ENGINE_CURSOR_NORMAL);

	// Save the currently current context so it can be restored later
	previous = _AnimaEngineWindowPlatformGetCurrentContext();

	// Open the actual window and create its context
	if (!_AnimaEngineWindowPlatformCreateWindow(window, &wndconfig, &ctxconfig, &fbconfig))
	{
		DestroyAnimaWindow((AnimaEngineWindow_Base*)window);
		_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		return NULL;
	}

	_AnimaEngineWindowPlatformMakeContextCurrent(window);

	// Retrieve the actual (as opposed to requested) context attributes
	if (!_AnimaEngineWindowRefreshContextAttribs(&ctxconfig))
	{
		DestroyAnimaWindow((AnimaEngineWindow_Base*)window);
		_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		return NULL;
	}

	// Verify the context against the requested parameters
	if (!_AnimaEngineWindowIsValidContext(&ctxconfig))
	{
		DestroyAnimaWindow((AnimaEngineWindow_Base*)window);
		_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		return NULL;
	}

	// Clearing the front buffer to black to avoid garbage pixels left over
	// from previous uses of our bit of VRAM
	glClear(GL_COLOR_BUFFER_BIT);
	_AnimaEngineWindowPlatformSwapBuffers(window);

	// Restore the previously current context (or NULL)
	_AnimaEngineWindowPlatformMakeContextCurrent(previous);

	if (wndconfig._monitor)
	{
		int width, height;
		_AnimaEngineWindowPlatformGetWindowSize(window, &width, &height);

		window->SetCursorPosX(width / 2);
		window->SetCursorPosY(height / 2);

		_AnimaEngineWindowPlatformSetCursorPos(window, window->GetCursorPosX(), window->GetCursorPosY());
	}
	else
	{
		if (wndconfig._visible)
		{
			if (wndconfig._focused)
				_AnimaEngineWindowPlatformShowWindow(window);
			else
				_AnimaEngineWindowPlatformUnhideWindow(window);
		}
	}

	return (T*)window;
}

END_ANIMA_ENGINE_NAMESPACE

#endif