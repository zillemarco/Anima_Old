//
//  AnimaDefaultMouseInteractor.h
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//
#pragma once

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "IAnimaInteractor.h"
#include "IAnimaEvents.h"
#include "AnimaString.h"
#include "AnimaVertex.h"
#include "AnimaMouseInteractor.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaDefaultMouseInteractor : public AnimaMouseInteractor
{
public:
	AnimaDefaultMouseInteractor();
	AnimaDefaultMouseInteractor(const AnimaDefaultMouseInteractor& src);
	AnimaDefaultMouseInteractor(AnimaDefaultMouseInteractor&& src);
	virtual ~AnimaDefaultMouseInteractor();
	
	AnimaDefaultMouseInteractor& operator=(const AnimaDefaultMouseInteractor& src);
	AnimaDefaultMouseInteractor& operator=(AnimaDefaultMouseInteractor&& src);
	
public:
	bool Install(long windowId, AnimaEngine* engine) override;
};

END_ANIMA_ENGINE_NAMESPACE
