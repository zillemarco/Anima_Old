//
//  AnimaScene.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaScene__
#define __Anima__AnimaScene__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaModelsManager;
class AnimaShadersManager;
class AnimaCamerasManager;
class AnimaTexturesManager;
class AnimaDataGeneratorsManager;
class AnimaMaterialsManager;
class AnimaLightsManager;
class AnimaEngine;

class ANIMA_ENGINE_EXPORT AnimaScene
{
	friend class AnimaEngine;

public:
	AnimaScene();
	AnimaScene(const AnimaScene& src);
	AnimaScene(AnimaScene&& src);
	~AnimaScene();
	
	AnimaScene& operator=(const AnimaScene& src);
	AnimaScene& operator=(AnimaScene&& src);

public:
	void Initialize();
	void Terminate();
	
public:
	inline AnimaAllocator* GetModelsAllocator() {
		ANIMA_ASSERT(_modelsAllocator != nullptr);
		return _modelsAllocator;
	}

	inline AnimaAllocator* GetGenericAllocator() {
		ANIMA_ASSERT(_genericAllocator != nullptr);
		return _genericAllocator;
	}

	inline AnimaAllocator* GetModelDataAllocator() {
		ANIMA_ASSERT(_modelDataAllocator != nullptr);
		return _modelDataAllocator;
	}

	inline AnimaAllocator* GetStringAllocator() {
		ANIMA_ASSERT(_stringAllocator != nullptr);
		return _stringAllocator;
	}

	inline AnimaAllocator* GetShadersAllocator() {
		ANIMA_ASSERT(_shadersAllocator != nullptr);
		return _shadersAllocator;
	}

	inline AnimaAllocator* GetCamerasAllocator() {
		ANIMA_ASSERT(_camerasAllocator != nullptr);
		return _camerasAllocator;
	}

	inline AnimaAllocator* GetTexturesAllocator() {
		ANIMA_ASSERT(_texturesAllocator != nullptr);
		return _texturesAllocator;
	}

	inline AnimaAllocator* GetMaterialsAllocator() {
		ANIMA_ASSERT(_materialsAllocator != nullptr);
		return _materialsAllocator;
	}

	inline AnimaAllocator* GetLightsAllocator() {
		ANIMA_ASSERT(_lightsAllocator != nullptr);
		return _lightsAllocator;
	}

	inline AnimaAllocator* GetDataGeneratorsAllocator() {
		ANIMA_ASSERT(_dataGeneratorsAllocator != nullptr)
		return _dataGeneratorsAllocator;
	}

	inline AnimaModelsManager* GetModelsManager() {
		ANIMA_ASSERT(_modelsManager != nullptr);
		return _modelsManager;
	}

	inline AnimaShadersManager* GetShadersManager() {
		ANIMA_ASSERT(_shadersManager != nullptr);
		return _shadersManager;
	}

	inline AnimaCamerasManager* GetCamerasManager() {
		ANIMA_ASSERT(_camerasManager != nullptr);
		return _camerasManager;
	}

	inline AnimaTexturesManager* GetTexturesManager() {
		ANIMA_ASSERT(_texturesManager != nullptr);
		return _texturesManager;
	}

	inline AnimaDataGeneratorsManager* GetDataGeneratorsManager() {
		ANIMA_ASSERT(_dataGeneratorsManager != nullptr);
		return _dataGeneratorsManager;
	}

	inline AnimaMaterialsManager* GetMaterialsManager() {
		ANIMA_ASSERT(_materialsManager != nullptr);
		return _materialsManager;
	}

	inline AnimaLightsManager* GetLightsManager() {
		ANIMA_ASSERT(_lightsManager != nullptr);
		return _lightsManager;
	}

private:
	void InitializeMemorySystem();
	void InitializeManagers();

	void TerminateMemorySystem();
	void TerminateManagers();
	
protected:
	void*	_localMemory;							/*!< Buffer della memoria principale di AnimaEngine a cui puntano i custom allocator */
	ASizeT	_localMemorySize;						/*!< Dimensione del buffer della memoria principale di AnimaEngine a cui puntano i custom allocator */
	AnimaFreeListAllocator*	_localMemoryAllocator;	/*!< Allocator usato per creare tutti gli altri allocator */

	AnimaFreeListAllocator* _genericAllocator;		/*!< Allocator usato genericamente */
	AnimaFreeListAllocator* _modelDataAllocator;	/*!< Allocator usato dalla classe AnimaVertex, suoi derivati e utilizzatori */
	AnimaFreeListAllocator* _modelsAllocator;		/*!< Allocator usato dalla classe AnimaModel e suoi derivati per gestire modelli e mesh */
	AnimaFreeListAllocator* _managersAllocator;		/*!< Allocator usato all'interno di AnimaEngine per costruire i vari manager */
	AnimaFreeListAllocator* _stringAllocator;		/*!< Allocator usato dalla classe AnimaString */
	AnimaFreeListAllocator* _shadersAllocator;		/*!< Allocator usato dalla classe AnimaShaderProgram e AnimaShadersManager */
	AnimaFreeListAllocator* _camerasAllocator;		/*!< Allocator usato dalla classe AnimaCamerasManager */
	AnimaFreeListAllocator* _texturesAllocator;		/*!< Allocator usato dalla classe AnimaCamerasManager */
	AnimaFreeListAllocator* _materialsAllocator;	/*!< Allocator usato dalla classe AnimaCamerasManager */
	AnimaFreeListAllocator* _lightsAllocator;
	AnimaFreeListAllocator* _dataGeneratorsAllocator;

	AnimaModelsManager* _modelsManager;					/*!< Gestore di tutti i modelli dell'istanza corrente di AnimaEngine */
	AnimaShadersManager* _shadersManager;				/*!< Gestore di tutti gli shader dell'istanza corrente di AnimaEngine */
	AnimaCamerasManager* _camerasManager;				/*!< Gestore di tutte le telecamere appartenenti all'istanza corrente di AnimaEngine */
	AnimaTexturesManager* _texturesManager;				/*!< Gestore di tutte le texture appartenenti all'istanza corrente di AnimaEngine */
	AnimaDataGeneratorsManager* _dataGeneratorsManager;
	AnimaMaterialsManager* _materialsManager;
	AnimaLightsManager* _lightsManager;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScene__) */
