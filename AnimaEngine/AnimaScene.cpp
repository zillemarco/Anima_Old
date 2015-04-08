//
//  AnimaScene.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaScene.h"
#include "AnimaModelsManager.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaMaterialsManager.h"
#include "AnimaLightsManager.h"
#include "AnimaArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaScene::AnimaScene(AnimaEngine* engine)
{
	_engine = engine;
}

AnimaScene::AnimaScene(const AnimaScene& src)
{
	_engine = src._engine;
}

AnimaScene::AnimaScene(AnimaScene&& src)
{
	_engine = src._engine;
}

AnimaScene::~AnimaScene()
{
}

AnimaScene& AnimaScene::operator=(const AnimaScene& src)
{
	if (this != &src)
	{
		_engine = src._engine;
	}
	
	return *this;
}

AnimaScene& AnimaScene::operator=(AnimaScene&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
	}
	
	return *this;
}

END_ANIMA_ENGINE_NAMESPACE
