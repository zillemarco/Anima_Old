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

class AnimaModelsManager;
class AnimaCamerasManager;
class AnimaTexturesManager;
class AnimaDataGeneratorsManager;
class AnimaMaterialsManager;
class AnimaLightsManager;

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
	inline AnimaShadersManager* GetShadersManager()					{ return _engine->GetShadersManager();	}
	inline AnimaCamerasManager* GetCamerasManager()					{ return _camerasManager;				}
	inline AnimaTexturesManager* GetTexturesManager()				{ return _texturesManager;				}
	inline AnimaDataGeneratorsManager* GetDataGeneratorsManager()	{ return _dataGeneratorsManager;		}
	inline AnimaMaterialsManager* GetMaterialsManager()				{ return _materialsManager;				}
	inline AnimaLightsManager* GetLightsManager()					{ return _lightsManager;				}

	inline AnimaAllocator* GetModelsAllocator()			{ return _engine->GetModelsAllocator();			}
	inline AnimaAllocator* GetGenericAllocator()		{ return _engine->GetGenericAllocator();		}
	inline AnimaAllocator* GetModelDataAllocator()		{ return _engine->GetModelDataAllocator();		}
	inline AnimaAllocator* GetStringAllocator()			{ return _engine->GetStringAllocator();			}
	inline AnimaAllocator* GetShadersAllocator()		{ return _engine->GetShadersAllocator();		}
	inline AnimaAllocator* GetCamerasAllocator()		{ return _engine->GetCamerasAllocator();		}
	inline AnimaAllocator* GetTexturesAllocator()		{ return _engine->GetTexturesAllocator();		}
	inline AnimaAllocator* GetMaterialsAllocator()		{ return _engine->GetMaterialsAllocator();		}
	inline AnimaAllocator* GetLightsAllocator()			{ return _engine->GetLightsAllocator();			}
	inline AnimaAllocator* GetDataGeneratorsAllocator()	{ return _engine->GetDataGeneratorsAllocator(); }

protected:
	AnimaEngine* _engine;

	AnimaModelsManager*			_modelsManager;					/*!< Gestore di tutti i modelli dell'istanza corrente di AnimaEngine */
	AnimaLightsManager*			_lightsManager;
	AnimaCamerasManager*		_camerasManager;				/*!< Gestore di tutte le telecamere appartenenti all'istanza corrente di AnimaEngine */
	AnimaTexturesManager*		_texturesManager;				/*!< Gestore di tutte le texture appartenenti all'istanza corrente di AnimaEngine */
	AnimaMaterialsManager*		_materialsManager;
	AnimaDataGeneratorsManager* _dataGeneratorsManager;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScene__) */
