//
//  AnimaDefaultInteractor.h
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
#include "AnimaKeyboardInteractor.h"
#include "AnimaJoystickInteractor.h"

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

class ANIMA_ENGINE_EXPORT AnimaDefaultKeyboardInteractor : public AnimaKeyboardInteractor
{
public:
	AnimaDefaultKeyboardInteractor();
	AnimaDefaultKeyboardInteractor(const AnimaDefaultKeyboardInteractor& src);
	AnimaDefaultKeyboardInteractor(AnimaDefaultKeyboardInteractor&& src);
	virtual ~AnimaDefaultKeyboardInteractor();
	
	AnimaDefaultKeyboardInteractor& operator=(const AnimaDefaultKeyboardInteractor& src);
	AnimaDefaultKeyboardInteractor& operator=(AnimaDefaultKeyboardInteractor&& src);
	
public:
	bool Install(long windowId, AnimaEngine* engine) override;
	
protected:
	bool _moveForwardsPressed;
	bool _moveBackwardsPressed;
	bool _moveRightPressed;
	bool _moveLeftPressed;
	bool _moveUpPressed;
	bool _moveDownPressed;
};

class ANIMA_ENGINE_EXPORT AnimaDefaultJoystickInteractor : public AnimaJoystickInteractor
{
public:
	static AnimaJoystickInteractor* GetInstance();
	
public:
	AnimaDefaultJoystickInteractor();
	AnimaDefaultJoystickInteractor(const AnimaDefaultJoystickInteractor& src);
	AnimaDefaultJoystickInteractor(AnimaDefaultJoystickInteractor&& src);
	virtual ~AnimaDefaultJoystickInteractor();
	
	AnimaDefaultJoystickInteractor& operator=(const AnimaDefaultJoystickInteractor& src);
	AnimaDefaultJoystickInteractor& operator=(AnimaDefaultJoystickInteractor&& src);
	
public:
	bool Install(long windowId, AnimaEngine* engine) override;
};

END_ANIMA_ENGINE_NAMESPACE
