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
#include "AnimaMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaCamerasManager;
class AnimaTexturesManager;
class AnimaDataGeneratorsManager;
class AnimaMaterialsManager;
class AnimaLightsManager;

class AnimaModelsManager;
class AnimaMeshesManager;
class AnimaModelInstancesManager;
class AnimaMeshInstancesManager;

class ANIMA_ENGINE_EXPORT AnimaScene
{
public:
	AnimaScene(AnimaEngine* engine);
	AnimaScene(const AnimaScene& src);
	AnimaScene(AnimaScene&& src);
	~AnimaScene();

	AnimaScene& operator=(const AnimaScene& src);
	AnimaScene& operator=(AnimaScene&& src);

public:
	void Initialize();
	void Terminate();

public:
	inline AnimaEngine* GetEngine() { return _engine; }

	inline AnimaModelsManager* GetModelsManager()					{ return _modelsManager;				}
	inline AnimaMeshesManager* GetMeshesManager()					{ return _meshesManager;				}
	inline AnimaModelInstancesManager* GetModelInstancesManager()	{ return _modelInstancesManager;		}
	inline AnimaMeshInstancesManager* GetMeshInstancesManager()		{ return _meshInstancesManager;			}
	inline AnimaShadersManager* GetShadersManager()					{ return _engine->GetShadersManager();	}
	inline AnimaCamerasManager* GetCamerasManager()					{ return _camerasManager;				}
	inline AnimaTexturesManager* GetTexturesManager()				{ return _texturesManager;				}
	inline AnimaDataGeneratorsManager* GetDataGeneratorsManager()	{ return _dataGeneratorsManager;		}
	inline AnimaMaterialsManager* GetMaterialsManager()				{ return _materialsManager;				}
	inline AnimaLightsManager* GetLightsManager()					{ return _lightsManager;				}

	inline AnimaAllocator* GetMeshesAllocator()			{ return _engine->GetMeshesAllocator();			}
	inline AnimaAllocator* GetModelsAllocator()			{ return _engine->GetModelsAllocator();			}
	inline AnimaAllocator* GetModelInstancesAllocator()	{ return _engine->GetModelInstancesAllocator(); }
	inline AnimaAllocator* GetMeshInstancesAllocator()	{ return _engine->GetMeshInstancesAllocator();	}
	inline AnimaAllocator* GetGenericAllocator()		{ return _engine->GetGenericAllocator();		}
	inline AnimaAllocator* GetStringAllocator()			{ return _engine->GetStringAllocator();			}
	inline AnimaAllocator* GetShadersAllocator()		{ return _engine->GetShadersAllocator();		}
	inline AnimaAllocator* GetCamerasAllocator()		{ return _engine->GetCamerasAllocator();		}
	inline AnimaAllocator* GetTexturesAllocator()		{ return _engine->GetTexturesAllocator();		}
	inline AnimaAllocator* GetMaterialsAllocator()		{ return _engine->GetMaterialsAllocator();		}
	inline AnimaAllocator* GetLightsAllocator()			{ return _engine->GetLightsAllocator();			}
	inline AnimaAllocator* GetDataGeneratorsAllocator()	{ return _engine->GetDataGeneratorsAllocator(); }

protected:
	AnimaEngine* _engine;

	AnimaLightsManager*			_lightsManager;
	AnimaCamerasManager*		_camerasManager;				/*!< Gestore di tutte le telecamere appartenenti all'istanza corrente di AnimaEngine */
	AnimaTexturesManager*		_texturesManager;				/*!< Gestore di tutte le texture appartenenti all'istanza corrente di AnimaEngine */
	AnimaMaterialsManager*		_materialsManager;
	AnimaDataGeneratorsManager* _dataGeneratorsManager;
	AnimaModelsManager*			_modelsManager;					/*!< Gestore di tutti i modelli dell'istanza corrente di AnimaEngine */
	AnimaMeshesManager*			_meshesManager;
	AnimaModelInstancesManager*	_modelInstancesManager;
	AnimaMeshInstancesManager*	_meshInstancesManager;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScene__) */
