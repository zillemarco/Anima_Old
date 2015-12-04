//
//  AnimaDefaultInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaDefaultInteractors.h"
#include "AnimaScenesManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaScene.h"
#include "AnimaCamera.h"
#include "AnimaGeometryInstance.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaDefaultMouseInteractor::AnimaDefaultMouseInteractor()
	: AnimaMouseInteractor()
{
}

AnimaDefaultMouseInteractor::AnimaDefaultMouseInteractor(const AnimaDefaultMouseInteractor& src)
	: AnimaMouseInteractor(src)
{
}

AnimaDefaultMouseInteractor::AnimaDefaultMouseInteractor(AnimaDefaultMouseInteractor&& src)
	: AnimaMouseInteractor(src)
{
}

AnimaDefaultMouseInteractor::~AnimaDefaultMouseInteractor()
{
}

AnimaDefaultMouseInteractor& AnimaDefaultMouseInteractor::operator=(const AnimaDefaultMouseInteractor& src)
{
	AnimaMouseInteractor::operator=(src);
	return *this;
}

AnimaDefaultMouseInteractor& AnimaDefaultMouseInteractor::operator=(AnimaDefaultMouseInteractor&& src)
{
	AnimaMouseInteractor::operator=(src);
	return *this;
}

bool AnimaDefaultMouseInteractor::Install(long windowId, AnimaEngine* engine)
{
	if(AnimaMouseInteractor::Install(windowId, engine))
	{
		SetEventHandler("onLeftMouseDragged", [] (AnimaEventArgs* args) {			
			AnimaVertex2f delta = ((AnimaMouseDraggedEventArgs*)args)->GetDelta();
			AnimaEngine* engine = ((AnimaMouseInteractor*)args->GetSourceEvent())->GetEngine();
			
			if(engine)
			{
				AnimaScenesManager* scenesManager = engine->GetScenesManager();
				AnimaScene* scene = scenesManager->GetActiveScene();
				
				if(scene)
				{
					AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
					AnimaCamera* camera = camerasManager->GetActiveCamera();
					
					if(camera)
					{
						AFloat dx = 0.0f, dy = 0.0f, dz = 0.0f;
						
						switch (camera->GetCameraType())
						{
							case ACT_FIRST_PERSON:
							case ACT_SPECTATOR:
								dx = delta.x;
								dy = -delta.y;
								
								camera->SmoothRotateDeg(dx, dy, 0.0f);
								break;
								
							case ACT_FLIGHT:
								dy = delta.y;
								dz = delta.x;
								
								camera->SmoothRotateDeg(0.0f, dy, dz);
								break;
								
							case ACT_ORBIT:
								dx = delta.x;
								dy = delta.y;
								
								camera->SmoothRotateDeg(dx, dy, 0.0f);
								break;
						}
					}
				}
			}
		});
		
		SetEventHandler("onLeftMouseClick", [] (Anima::AnimaEventArgs* args) {
			
			Anima::AnimaVertex2f point = ((Anima::AnimaMouseEventArgs*)args)->GetPoint();
			Anima::AnimaVertex2f size = ((Anima::AnimaMouseEventArgs*)args)->GetWindowSize();
			
			AnimaLogger::LogMessageFormat("click at %f:%f", point.x, point.y);
			
			Anima::AnimaEngine* engine = ((Anima::AnimaMouseInteractor*)args->GetSourceEvent())->GetEngine();
			
			if(engine)
			{
				Anima::AnimaScenesManager* scenesManager = engine->GetScenesManager();
				Anima::AnimaScene* scene = scenesManager->GetActiveScene();
				
				if(scene)
				{
					Anima::AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
					Anima::AnimaCamera* camera = camerasManager->GetActiveCamera();
					
					if(camera)
					{
						Anima::AnimaVertex3f origin = camera->GetPosition();
						Anima::AnimaVertex3f end = camera->ScreenPointToWorldPoint(point);
						
						btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(origin.x, origin.y, origin.z), btVector3(end.x, end.y, end.z));
						
						btDynamicsWorld* world = scene->GetPhysWorld();
						
						if(world)
						{
							world->rayTest(btVector3(origin.x, origin.y, origin.z), btVector3(end.x, end.y, end.z), RayCallback);
							
							if(RayCallback.hasHit())
							{
								AnimaGeometryInstance* instance = (AnimaGeometryInstance*)RayCallback.m_collisionObject->getUserPointer();
								AnimaVertex3f contactPoint(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z());
								
								AnimaLogger::LogMessageFormat("Picked geometry named '%s'", instance->GetName().c_str());
							}
							else
							{
								printf("Picked nothing\n");
							}
						}
					}
				}
			}
		});
		
		return true;
	}
	
	return false;
}

AnimaDefaultKeyboardInteractor::AnimaDefaultKeyboardInteractor()
: AnimaKeyboardInteractor()
{
	_moveForwardsPressed = false;
	_moveBackwardsPressed = false;
	_moveRightPressed = false;
	_moveLeftPressed = false;
	_moveUpPressed = false;
	_moveDownPressed = false;
}

AnimaDefaultKeyboardInteractor::AnimaDefaultKeyboardInteractor(const AnimaDefaultKeyboardInteractor& src)
: AnimaKeyboardInteractor(src)
{
	_moveForwardsPressed = src._moveForwardsPressed;
	_moveBackwardsPressed = src._moveBackwardsPressed;
	_moveRightPressed = src._moveRightPressed;
	_moveLeftPressed = src._moveLeftPressed;
	_moveUpPressed = src._moveUpPressed;
	_moveDownPressed = src._moveDownPressed;
}

AnimaDefaultKeyboardInteractor::AnimaDefaultKeyboardInteractor(AnimaDefaultKeyboardInteractor&& src)
: AnimaKeyboardInteractor(src)
{
	_moveForwardsPressed = src._moveForwardsPressed;
	_moveBackwardsPressed = src._moveBackwardsPressed;
	_moveRightPressed = src._moveRightPressed;
	_moveLeftPressed = src._moveLeftPressed;
	_moveUpPressed = src._moveUpPressed;
	_moveDownPressed = src._moveDownPressed;
}

AnimaDefaultKeyboardInteractor::~AnimaDefaultKeyboardInteractor()
{
}

AnimaDefaultKeyboardInteractor& AnimaDefaultKeyboardInteractor::operator=(const AnimaDefaultKeyboardInteractor& src)
{
	AnimaKeyboardInteractor::operator=(src);
	
	if(this != &src)
	{
		_moveForwardsPressed = src._moveForwardsPressed;
		_moveBackwardsPressed = src._moveBackwardsPressed;
		_moveRightPressed = src._moveRightPressed;
		_moveLeftPressed = src._moveLeftPressed;
		_moveUpPressed = src._moveUpPressed;
		_moveDownPressed = src._moveDownPressed;
	}

	return *this;
}

AnimaDefaultKeyboardInteractor& AnimaDefaultKeyboardInteractor::operator=(AnimaDefaultKeyboardInteractor&& src)
{
	AnimaKeyboardInteractor::operator=(src);
	
	if(this != &src)
	{
		_moveForwardsPressed = src._moveForwardsPressed;
		_moveBackwardsPressed = src._moveBackwardsPressed;
		_moveRightPressed = src._moveRightPressed;
		_moveLeftPressed = src._moveLeftPressed;
		_moveUpPressed = src._moveUpPressed;
		_moveDownPressed = src._moveDownPressed;
	}
	
	return *this;
}

bool AnimaDefaultKeyboardInteractor::Install(long windowId, AnimaEngine* engine)
{
	if(AnimaKeyboardInteractor::Install(windowId, engine))
	{
		SetEventHandler("onUpdateScene", [] (AnimaEventArgs* args) {
			AnimaScene* scene = ((AnimaUpdateSceneEventArgs*)args)->GetScene();
			
			if(scene)
			{
				AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
				AnimaCamera* camera = camerasManager->GetActiveCamera();
				
				if(camera)
				{
					AnimaDefaultKeyboardInteractor* interactor = (AnimaDefaultKeyboardInteractor*)((AnimaUpdateSceneEventArgs*)args)->GetSourceEvent();
					
					AnimaKeyboardStatusMap keyboardStatus = interactor->GetKeyboardStatus();
					AnimaVertex3f velocity = camera->GetCurrentVelocity();
					AnimaVertex3f direction(0.0f, 0.0f, 0.0f);
					
					if (keyboardStatus[AnimaKeyboardKey::AKK_W])
					{
						if (!interactor->_moveForwardsPressed)
						{
							interactor->_moveForwardsPressed = true;
							camera->SetCurrentVelocity(velocity.x, velocity.y, 0.0f);
						}
						
						direction.z += 1.0f;
					}
					else
					{
						interactor->_moveForwardsPressed = false;
					}
					
					if (keyboardStatus[AnimaKeyboardKey::AKK_S])
					{
						if (!interactor->_moveBackwardsPressed)
						{
							interactor->_moveBackwardsPressed = true;
							camera->SetCurrentVelocity(velocity.x, velocity.y, 0.0f);
						}
						
						direction.z -= 1.0f;
					}
					else
					{
						interactor->_moveBackwardsPressed = false;
					}
					
					if (keyboardStatus[AnimaKeyboardKey::AKK_D])
					{
						if (!interactor->_moveRightPressed)
						{
							interactor->_moveRightPressed = true;
							camera->SetCurrentVelocity(0.0f, velocity.y, velocity.z);
						}
						
						direction.x += 1.0f;
					}
					else
					{
						interactor->_moveRightPressed = false;
					}
					
					if (keyboardStatus[AnimaKeyboardKey::AKK_A])
					{
						if (!interactor->_moveLeftPressed)
						{
							interactor->_moveLeftPressed = true;
							camera->SetCurrentVelocity(0.0f, velocity.y, velocity.z);
						}
						
						direction.x -= 1.0f;
					}
					else
					{
						interactor->_moveLeftPressed = false;
					}
					
					if (keyboardStatus[AnimaKeyboardKey::AKK_E])
					{
						if (!interactor->_moveUpPressed)
						{
							interactor->_moveUpPressed = true;
							camera->SetCurrentVelocity(velocity.x, 0.0f, velocity.z);
						}
						
						direction.y += 1.0f;
					}
					else
					{
						interactor->_moveUpPressed = false;
					}
					
					if (keyboardStatus[AnimaKeyboardKey::AKK_Q])
					{
						if (!interactor->_moveDownPressed)
						{
							interactor->_moveDownPressed = true;
							camera->SetCurrentVelocity(velocity.x, 0.0f, velocity.z);
						}
						
						direction.y -= 1.0f;
					}
					else
					{
						interactor->_moveDownPressed = false;
					}
					
					camera->SetUpdateData(direction);
				}
			}
		});
		return true;
	}
	
	return false;
}

AnimaDefaultJoystickInteractor::AnimaDefaultJoystickInteractor()
: AnimaJoystickInteractor()
{
}

AnimaDefaultJoystickInteractor::~AnimaDefaultJoystickInteractor()
{
}

AnimaJoystickInteractor* AnimaDefaultJoystickInteractor::GetInstance()
{
	if(_instance == nullptr)
		_instance = new AnimaDefaultJoystickInteractor;
	return _instance;
}

bool AnimaDefaultJoystickInteractor::Install(long windowId, AnimaEngine* engine)
{
	if(AnimaJoystickInteractor::Install(windowId, engine))
	{
		SetEventHandler("onButtonPressed", [](AnimaEventArgs* args) {
			AnimaJoystickButtonEventArgs* buttonArgs = (AnimaJoystickButtonEventArgs*)args;
			
			if (buttonArgs->GetButtonId() == AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_CLICK)
			{
				AnimaEngine* engine = ((AnimaJoystickInteractor*)args->GetSourceEvent())->GetEngine();
				
				if (engine)
				{
					AnimaScenesManager* scenesManager = engine->GetScenesManager();
					AnimaScene* scene = scenesManager->GetActiveScene();
					
					if (scene)
					{
						AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
						AnimaCamera* camera = camerasManager->GetActiveCamera();
						
						if (camera)
						{
							AnimaCameraType type = camera->GetCameraType();
							if (type == ACT_FIRST_PERSON || type == ACT_SPECTATOR)
								camera->SetCameraType(ACT_FLIGHT);
							else if (type == ACT_FLIGHT)
								camera->SetCameraType(ACT_ORBIT);
							else if (type == ACT_ORBIT)
								camera->SetCameraType(ACT_FIRST_PERSON);
						}
					}
				}
			}
		});
		
		SetEventHandler("onUpdateScene", [](AnimaEventArgs* args){
			AnimaScene* scene = ((AnimaUpdateSceneEventArgs*)args)->GetScene();
			AnimaJoystickDevice* joystick = ((AnimaJoystickInteractor*)((AnimaUpdateSceneEventArgs*)args)->GetSourceEvent())->GetJoystick(0);
			
			if(scene && joystick->IsPresent())
			{
				AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
				AnimaCamera* camera = camerasManager->GetActiveCamera();
				
				if(camera)
				{
					AnimaArray<AFloat> axis = joystick->GetAxis();
					
					AnimaVertex3f velocity = camera->GetCurrentVelocity();
					AnimaVertex3f direction(0.0f, 0.0f, 0.0f);
					
					AnimaVertex3f maxVel = camera->GetDefaultMaximumVelocity();
					
					float rotationMultiplier = 15.0f;
					float velocityMultiplier = 30.0f;
					
					if (axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_X] != 0.0f)
					{
						direction.x = axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_X] > 0.0f ? 1.0f : -1.0f;
						maxVel.x = fabs(axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_X]) * velocityMultiplier;
					}
					
					if (axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_Y] != 0.0f)
					{
						direction.z = axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_Y] > 0.0f ? -1.0f : 1.0f;
						maxVel.z = fabs(axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_LS_Y]) * velocityMultiplier;
					}
					
					camera->SetMaximumVelocity(maxVel);
					
					switch (camera->GetCameraType())
					{
						case ACT_FIRST_PERSON:
						case ACT_SPECTATOR:
							camera->SmoothRotateDeg(-axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_X] * rotationMultiplier, -axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_Y] * rotationMultiplier, 0.0f);
							break;
						case ACT_FLIGHT:
							camera->SmoothRotateDeg(0.0f, axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_Y] * rotationMultiplier, -axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_X] * rotationMultiplier);
							break;
						case ACT_ORBIT:
							camera->SmoothRotateDeg(-axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_X] * rotationMultiplier, axis[AnimaJoystickInteractor::AJ_XBOX_360_DEFAULT_RS_Y] * rotationMultiplier, 0.0f);
							break;
					}
					camera->SetUpdateData(direction);
				}
			}
		});
		return true;
	}
	
	return false;
}

END_ANIMA_ENGINE_NAMESPACE