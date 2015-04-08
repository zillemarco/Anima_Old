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
	inline AnimaModelsManager* GetModelsManager()					{ return _engine->GetModelsManager();			}
	inline AnimaShadersManager* GetShadersManager()					{ return _engine->GetShadersManager();			}
	inline AnimaCamerasManager* GetCamerasManager()					{ return _engine->GetCamerasManager();			}
	inline AnimaTexturesManager* GetTexturesManager()				{ return _engine->GetTexturesManager();			}
	inline AnimaDataGeneratorsManager* GetDataGeneratorsManager()	{ return _engine->GetDataGeneratorsManager();	}
	inline AnimaMaterialsManager* GetMaterialsManager()				{ return _engine->GetMaterialsManager();		}
	inline AnimaLightsManager* GetLightsManager()					{ return _engine->GetLightsManager();			}

protected:
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaScene__) */
