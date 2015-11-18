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
#include "AnimaGC.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"
#include "AnimaEngineConfig.h"

#if defined _MSC_VER
#include <Windows.h>
#define PATH_MAX MAX_PATH
#endif

//#define SAVE_SCENE

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaScenesManager;
class AnimaShadersManager;
class AnimaDataGeneratorsManager;
class AnimaParallelProgramsManager;

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
	static void SetUsedExternal(bool bUsedExternal = true);
		
	/*!
	 *	\brief		Ritorna la modalità di utilizzo impostata di AnimaEngine
	 *	\details	Metodo che ritorna la modalità di utilizzo impostata di AnimaEngine
	 *	\return		bool	valore del membro _usedExternal
	 *	\sa			_usedExternal
	 *	\author		Zille Marco
	 */
	static bool IsUsedExteral();
	
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
	static bool IsInitialized();

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

private:
	static bool _animaEngineInitialized;			/*!< Flag per indicare se AnimaEngine Ë stato inizializzato con successo */
	static int	_animaEngineCount;					/*!< Contatore di istanze di AnimaEngine */

	static bool _usedExternal;						/*!< Flag che indica se AnimaEngine è utilizzato all'interno di un altro programma */

private:
	static AChar _logFilePath[PATH_MAX];			/*!< Path del file di log di AnimaEngine */

public:
	/*!
	 *	\brief		Torna l'allocator dei nodeli 3D (classe AnimaNode)
	 *	\details	Torna l'allocator dei nodeli 3D (classe AnimaNode) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _nodesAllocator
	 *	\sa			_nodesAllocator, AnimaNode, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetNodesAllocator() {
		ANIMA_ASSERT(_nodesAllocator != nullptr);
		return _nodesAllocator;
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
	 *	\brief		Torna l'allocator delle geometry dei nodeli 3D (classe AnimaGeometry)
	 *	\details	Torna l'allocator delle geometry dei nodeli 3D (classe AnimaGeometry) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _geometriesAllocator
	 *	\sa			_geometriesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetGeometriesAllocator() {
		ANIMA_ASSERT(_geometriesAllocator != nullptr);
		return _geometriesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle istanze dei nodeli 3D (classe AnimaNodeInstance)
	 *	\details	Torna l'allocator delle istanze dei nodeli 3D (classe AnimaNodeInstance) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _nodeInstancesAllocator
	 *	\sa			_nodeInstancesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetNodeInstancesAllocator() {
		ANIMA_ASSERT(_nodeInstancesAllocator != nullptr);
		return _nodeInstancesAllocator;
	}
	
	/*!
	 *	\brief		Torna l'allocator delle istanze delle geometry dei nodeli 3D (classe AnimaGeometry)
	 *	\details	Torna l'allocator delle istanze delle geometry dei nodeli 3D (classe AnimaGeometry) effettuando anche un controllo che sia stato inizializzato
	 *	\return		Il valore di _geometryInstancesAllocator
	 *	\sa			_geometryInstancesAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetGeometryInstancesAllocator() {
		ANIMA_ASSERT(_geometryInstancesAllocator != nullptr);
		return _geometryInstancesAllocator;
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
	 *	\brief		Torna l'allocator dei programmi GPGPU. Usato dalle classi AnimaParallelProgramsManager e AnimaParallelProgram
	 *	\details	Torna l'allocator dei programmi GPGPU. Usato dalle classi AnimaParallelProgramsManager e AnimaParallelProgram
	 *	\return		Il valore di _dataGeneratorsAllocator
	 *	\sa			_dataGeneratorsAllocator, AnimaAllocator
	 *	\author		Zille Marco
	 */
	inline AnimaAllocator* GetParallelProgramsAllocator() {
		ANIMA_ASSERT(_parallelProgramsAllocator != nullptr);
		return _parallelProgramsAllocator;
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
	 *	\details	Torna il manager dei generatori di dati di AnimaEngine. In genere un generatore di dati è legato ad una scena ma, in alcuni casi
					come ad esempio per i generatori di dati usati nella classe AnimaRenderer e derivate, che non sono legati ad una scena, può essere necessatio
					avere a disposizione un manager dei generatori di dati generico.
	 *	\return		Il valore di _dataGeneratorsManager
	 *	\sa			_dataGeneratorsManager
	 *	\author		Zille Marco
	 */
	inline AnimaDataGeneratorsManager* GetDataGeneratorsManager() {
		ANIMA_ASSERT(_dataGeneratorsManager != nullptr);
		return _dataGeneratorsManager;
	}
	
	/*!
	 *	\brief		Torna il manager dei programmi GPGPU utilizzati da AnimaEngine
	 *	\details	Torna il manager dei programmi GPGPU utilizzati da AnimaEngine
	 *	\return		Il valore di _parallelProgramsManager
	 *	\sa			_parallelProgramsManager
	 *	\author		Zille Marco
	 */
	inline AnimaParallelProgramsManager* GetParallelProgramsManager() {
		ANIMA_ASSERT(_parallelProgramsManager != nullptr);
		return _parallelProgramsManager;
	}

protected:
	void*	_localMemory;								/*!< Buffer della memoria principale di AnimaEngine a cui puntano gli allocator */
	ASizeT	_localMemorySize;							/*!< Dimensione del buffer della memoria principale di AnimaEngine a cui puntano gli allocator */
	AnimaFreeListAllocator*	_localMemoryAllocator;		/*!< Allocator usato per creare tutti gli altri allocator */

	AnimaFreeListAllocator* _genericAllocator;			/*!< Allocator usato genericamente, senza un utilizzo in particolare */
	AnimaFreeListAllocator* _managersAllocator;			/*!< Allocator usato all'interno di AnimaEngine per costruire i vari manager */
	AnimaFreeListAllocator* _shadersAllocator;			/*!< Allocator usato dalla classe AnimaShader, AnimaShaderProgram e AnimaShadersManager */
	AnimaFreeListAllocator* _camerasAllocator;			/*!< Allocator usato dalla classe AnimaCamerasManager, AnimaCamera e derivate */
	AnimaFreeListAllocator* _texturesAllocator;			/*!< Allocator usato dalla classe AnimaTexturesManager, AnimaTexture e derivate */
	AnimaFreeListAllocator* _materialsAllocator;		/*!< Allocator usato dalla classe AnimaMaterialsManager, AnimaMaterial e derivate */
	AnimaFreeListAllocator* _lightsAllocator;			/*!< Allocator usato dalla classe AnimaLightsManager, AnimaLight e derivate */
	AnimaFreeListAllocator* _dataGeneratorsAllocator;	/*!< Allocator usato dalla classe AnimaDataGeneratorsManager, AnimaDataGenerator e derivate */
	AnimaFreeListAllocator* _scenesAllocator;			/*!< Allocator usato dalla classe AnimaScenesManager e AnimaScene */
	AnimaFreeListAllocator* _animationsAllocator;		/*!< Allocator usato dalla classe AnimaAnimationsManager e AnimaAnimation */
	AnimaFreeListAllocator* _geometriesAllocator;			/*!< Allocator usato dalla classe AnimaGeometriesManager e AnimaGeometry */
	AnimaFreeListAllocator* _nodesAllocator;			/*!< Allocator usato dalla classe AnimaNodesManager e AnimaNode */
	AnimaFreeListAllocator* _nodeInstancesAllocator;	/*!< Allocator usato dalla classe AnimaNodeInstancesManager e AnimaNodeInstance */
	AnimaFreeListAllocator* _geometryInstancesAllocator;	/*!< Allocator usato dalla classe AnimaGeometryInstancesManager e AnimaGeometryInstance */
	AnimaFreeListAllocator* _parallelProgramsAllocator;	/*!< Allocator usato dalla classe AnimaParallelProgramsManager e AnimaParallelProgram */

private:
	AnimaScenesManager*			_scenesManager;				/*!< Manager delle scene create da AnimaEngine */
	AnimaShadersManager*		_shadersManager;			/*!< Manager di tutti gli shader dell'istanza corrente di AnimaEngine */
	AnimaDataGeneratorsManager* _dataGeneratorsManager;		/*!< Manager di tutti i data generator che non sono legati ad una scena specifica (vedi esempio in AnimaRenderer) */
	AnimaParallelProgramsManager* _parallelProgramsManager;	/*!< Manager di tutti i programi con esecuzione in parallelo (GPGPU) */
};

END_ANIMA_ENGINE_NAMESPACE

#endif