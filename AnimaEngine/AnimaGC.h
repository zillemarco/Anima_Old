#if !defined __Anima__AnimaGC__
#define __Anima__AnimaGC__

#include "AnimaGraphicsIncludes.h"
#include "AnimaPlatformIncludes.h"

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include <functional>
#include <mutex>

#if defined ANIMA_ENGINE_EXPORT_TO_PYTHON
#	include <boost/python/object_core.hpp>
#endif

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

/*!
 *	\brief		Struttura dati utilizzata per contenere le informazioni da utilizzare per creare il buffer di rendering per il contesto che si vuole creare
 *	\details	Struttura dati utilizzata per contenere le informazioni da utilizzare per creare il buffer di rendering per il contesto che si vuole creare
 *	\author		Zille Marco
 */
struct ANIMA_ENGINE_EXPORT AnimaGCFrameBufferConfig
{
	AInt _redBits;			/*!< Numero di bit da utilizzare nel buffer di rendering per il canale rosso. Di default vale 8 */
	AInt _greenBits;		/*!< Numero di bit da utilizzare nel buffer di rendering per il canale verde. Di default vale 8 */
	AInt _blueBits;			/*!< Numero di bit da utilizzare nel buffer di rendering per il canale blu. Di default vale 8 */
	AInt _alphaBits;		/*!< Numero di bit da utilizzare nel buffer di rendering per il canale alpha. Di default vale 8 */
	AInt _depthBits;		/*!< Numero di bit da utilizzare nel buffer di rendering per la profondita'. Di default vale 24 */
	AInt _stencilBits;		/*!< Numero di bit da utilizzare nel buffer di rendering per lo stencil. Di default vale 8 */
	AInt _accumRedBits;		/*!< Di default vale 0 */
	AInt _accumGreenBits;	/*!< Di default vale 0 */
	AInt _accumBlueBits;	/*!< Di default vale 0 */
	AInt _accumAlphaBits;	/*!< Di default vale 0 */
	AInt _auxBuffers;		/*!< Di default vale 0 */
	bool _stereo;			/*!< Indica se il buffer di rendering ha capacita' stereo. Di default vale false */
	AInt _samples;			/*!< Numero di campioni che fa il buffer di rendering. Di default vale 0 */
	bool _sRGB;				/*!< Indica se i colori nel buffer di rendering sono da rappresentare in spazio lineare. Di default vale false */
	bool _doublebuffer;		/*!< Indica se il buffer di rendering deve utilizzare il double buffering. Di default vale true */

	#if defined WIN32
		AInt _wgl;
	#endif
};

struct ANIMA_ENGINE_EXPORT AnimaGCContextConfig
{
	AInt _api;			/*!< Indica che API utilizzare per il rendering. Di default vale ANIMAGC_OPENGL_API ed e' l'unico valore accettato */
	AInt _major;		/*!< Indica la versione maggiore dell'API da utilizzare. Di default vale 4 */
	AInt _minor;		/*!< Indica la versione minore dell'API da utilizzare. Di default vale 1 */
	bool _forward;		/*!< Indica se il contesto deve venir creato con compatibilita' di poter utilizzare funzioni avanzate di OpenGL. Questo flag e' utilizzato solo se il valore di _api e' ANIMAGC_OPENGL_API. Di default vale true */
	bool _debug;		/*!< Indica se il contesto deve venir creato con funzionalità di debug avanzate. Utilizzato solo per contesti OpenGL. Di default vale false */
	AInt _profile;		/*!< Indica con che profilo deve venir creato il contesto, nel caso si voglia creare un contesto con retro-compatibilita' di OpenGL. Di default vale ANIMAGC_OPENGL_CORE_PROFILE. I valori
							 accettati sono ANIMAGC_OPENGL_ANY_PROFILE, ANIMAGC_OPENGL_CORE_PROFILE e ANIMAGC_OPENGL_COMPAT_PROFILE */
	AInt _robustness;	/*!< Di default vale ANIMAGC_NO_ROBUSTNESS. I valori accettati sono ANIMAGC_NO_ROBUSTNESS, ANIMAGC_NO_RESET_NOTIFICATION e ANIMAGC_LOSE_CONTEXT_ON_RESET */
	AInt _release;		/*!< Di default vale ANIMAGC_ANY_RELEASE_BEHAVIOR. I valori accettati sono ANIMAGC_ANY_RELEASE_BEHAVIOR, ANIMAGC_RELEASE_BEHAVIOR_FLUSH e ANIMAGC_RELEASE_BEHAVIOR_NONE */
};

/*!
 *	\brief		Classe utilizzata per creare un contesto grafico su di una finestra
 *	\details	Classe utilizzata per creare un contesto grafico su di una finestra
 *	\author		Zille Marco
 */
class ANIMA_ENGINE_EXPORT AnimaGC
{
protected:
	/*!
	 *	\brief		Costruttore di AnimaGC
	 *	\details	Costruttore di AnimaGC. Messo protetto perche' non si può istanziare un contesto di rendering direttamente ma si deve
					utilizzare la funzione CreateContext()
	 *	\author		Zille Marco
	 */
	AnimaGC();

public:
	/*!
	 *	\brief		Distruttore di AnimaGC
	 *	\details	Distruttore di AnimaGC
	 *	\author		Zille Marco
	 */
	~AnimaGC();

public:
	/*!
	 *	\brief		Rende attivo il contesto grafico
	 *	\details	Rende attivo il contesto grafico in modo che le operazioni di rendering che vengono effettuare dal momento successivo
					questa chiamata vengano applicate a questo contesto, finche' non viene reso attivo un altro contesto
	 *	\author		Zille Marco
	 */
	void MakeCurrent();
	
	/*!
	 *	\brief		Fa lo swap dei buffer per aggiornare l'immagine visibile nella finestra che contiene il contesto
	 *	\details	Fa lo swap dei buffer per aggiornare l'immagine visibile nella finestra che contiene il contesto swappando il
					back-buffer, che contiene l'immagine aggiornata, con il front-buffer, che attualmente non contiene piu' un'immagine
					aggiornata
	 *	\author		Zille Marco
	 */
	void SwapBuffers();
	
	/*!
	 *	\brief		Effettua la 'pulitura' del buffer di disegno utilizzando un colore specificato dai parametri r,g,b,a
	 *	\details	Effettua la 'pulitura' del buffer di disegno utilizzando un colore specificato dai parametri r,g,b,a. I valori di
					r,g,b,a devono essere contenuti nell'intevallo [0-1]
	 *	\param[in]	r Valore del canale rosso da utilizzare per 'pulire' il buffer
	 *	\param[in]	g Valore del canale verde da utilizzare per 'pulire' il buffer
	 *	\param[in]	b Valore del canale blu da utilizzare per 'pulire' il buffer
	 *	\param[in]	a Valore del canale alpha da utilizzare per 'pulire' il buffer
	 *	\author		Zille Marco
	 */
	void ClearColor(AFloat r, AFloat g, AFloat b, AFloat a);
	
	void EnableVSync(bool enable);
	
	void SetUpdateFrameCallback(long userData, std::function<void (AnimaGC* context, long userData)> callback);
	
	#if defined ANIMA_ENGINE_EXPORT_TO_PYTHON
	void SetUpdateFrameCallback(boost::python::object userData, std::function<void (AnimaGC* context, boost::python::object userData)> callback);
	#endif

public:
	/*!
	 *	\brief		Crea un nuovo contesto grafico
	 *	\details	Crea un nuovo contesto grafico dato l'ID di una finestra e le informazioni per creare il contesto. I sistemi MacOS
					sono supportati ma solo se utilizzano Cocoa Framework
	 *	\param[in]	windowId		ID della finestra su cui si vuole creare il contesto. Per Windows questo e' il valore dell'HWND della finestra
									mentre per MacOS questo e' il valore dell'NSView* della finestra.
	 *	\param[in]	ctxconfig		Informazioni riguardo a come creare il contesto
	 *	\param[in]	fbconfig		Informazioni riguardo a come creare il buffer di rendering
	 *	\param[in]	vSyncEnabled	Indica se abilitare oppure no il V-Sync per il contesto creato
	 *	\return		Se i valori in input sono accettabili e si riesce a creare il contesto allora torna un puntatore al un nuovo
					contesto di rendering appena creato, se invece la creazione del contesto fallisce viene tornato nullptr
	 *	\author		Zille Marco
	 */
	static AnimaGC* CreateContext(long windowId, AnimaGCContextConfig ctxconfig, AnimaGCFrameBufferConfig fbconfig, bool vSyncEnabled = true);
	
	/*!
	 *	\brief		Distrugge il contesto grafico dato in input
	 *	\details	Distrugge il contesto grafico dato in input
	 *	\param[in]	context	Contesto grafico da distruggere
	 *	\author		Zille Marco
	 */
	static void DestroyContext(AnimaGC* context);
	
	/*!
	 *	\brief		Torna un'istanza di AnimaGCContextConfig inizializzata con i valori di default del contesto per creare un contesto grafico
	 *	\details	Torna un'istanza di AnimaGCContextConfig inizializzata con i valori di default del contesto per creare un contesto grafico
	 *	\return		Istanza di AnimaGCContextConfig inizializzata con dei valori di default del contesto per creare un contesto grafico
	 *	\author		Zille Marco
	 */
	static AnimaGCContextConfig GetDefaultContextConfig();
	
	/*!
	 *	\brief		Torna un'istanza di AnimaGCFrameBufferConfig inizializzata con i valori di default del buffer di rendering per creare un contesto grafico
	 *	\details	Torna un'istanza di AnimaGCFrameBufferConfig inizializzata con i valori di default del buffer di rendering per creare un contesto grafico
	 *	\return		Istanza di AnimaGCFrameBufferConfig inizializzata con dei valori di default del buffer di rendering per creare un contesto grafico
	 *	\author		Zille Marco
	 */
	static AnimaGCFrameBufferConfig GetDefaultFrameBufferConfig();
	
	/*!
	 *	\brief		Carica la libreria GLEW utilizzata come interfaccia per le funzioni di OpenGL
	 *	\details	Carica la libreria GLEW utilizzata come interfaccia per le funzioni di OpenGL
	 *	\return		True se il caricamento della librerira va a buon fine, false altrimenti
	 *	\author		Zille Marco
	 */
	static bool InitializeGLEWExtensions();

protected:
	/*!
	 *	\brief		Controlla se l'estensione cercata e' contenuta nella lista di estensioni attualmente supportate
	 *	\details	Controlla se l'estensione cercata e' contenuta nella lista di estensioni attualmente supportate
	 *	\param[in]	string		Nome dell'estensione che si vuole controllare
	 *	\param[in]	extension	Array di caratteri contenente i valori delle estensioni attualmente supportate
	 *	\return		True se l'estensione cercata e' contenuta nella lista di estensioni attualmente supportate, false altrimenti
	 *	\author		Zille Marco
	 */
	static bool StringInExtensionString(const char* string, const GLubyte* extensions);
	
	/*!
	 *	\brief		Cerca la configurazione del buffer di rendering piu' somigliante ad una lista di configurazioni di buffer di rendering passate
	 *	\details	Cerca la configurazione del buffer di rendering piu' somigliante ad una lista di configurazioni di buffer di rendering passate
	 *	\param[in]	desired			Configurazione del buffer di rendering originale, quella che si vorrebbe ottenere
	 *	\param[in]	alternatives	Array di configurazioni del buffer di rendering possibili con cui viene confrontata la configurazione del buffer di rendering originale
	 *	\param[in]	count			Numero di elementi contenuti in alternatives
	 *	\return		Valore della configurazione del buffer di rendering piu' simile a desired
	 *	\author		Zille Marco
	 */
	static const AnimaGCFrameBufferConfig* ChooseFrameBufferConfig(const AnimaGCFrameBufferConfig* desired, const AnimaGCFrameBufferConfig* alternatives, unsigned int count);
	
protected:
	static bool _GLEWExtensionsLoaded;		/*!< Indica se la libreria GLEW e' stata caricata correttamente */
	static bool _contextAPIsInitialized;	/*!< Indica se le API del contesto sono state inizializzate correttamente */
	long _updateFrameUserData;
	bool _vSyncEnabled;
	std::function<void (AnimaGC* context, long userData)> _updateFrameCallback;
	
	#if defined ANIMA_ENGINE_EXPORT_TO_PYTHON
	boost::python::object _pythonUpdateFrameUserData;
	std::function<void (AnimaGC* context, boost::python::object userData)> _pythonUpdateFrameCallback;
	#endif
	
#ifdef _WIN32
	public:
		bool CheckIntegrity(HWND hWnd);
		HGLRC GetContext();
				
	public:
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
#else
	public:
		bool CheckIntegrity(NSView* view);
		NSOpenGLContext* GetContext();
		NSOpenGLPixelFormat* GetPixelFormat();
	
	protected:
		static bool InitializeContextAPIs();
		static void TerminateContextAPIs();
	
		static void SurfaceNeedsUpdateHandler(CFNotificationCenterRef center, void* observer, CFStringRef name, const void* object, CFDictionaryRef userInfo);
		static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext);
	
	protected:
		NSView* _view;
		NSOpenGLContext* _context;
		NSOpenGLPixelFormat* _pixelFormat;
		CVDisplayLinkRef _displayLink;
	
		static std::mutex _updateFrameCallbackMutex;
	
	protected:
		static void* _framework;
	
#endif
};

END_ANIMA_ENGINE_NAMESPACE


#endif //__Anima__AnimaGC__