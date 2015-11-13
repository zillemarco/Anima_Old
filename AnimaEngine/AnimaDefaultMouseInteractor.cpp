//
//  AnimaDefaultMouseInteractor.cpp
//  Anima
//
//  Created by Marco Zille on 03/11/15.
//
//

#include "AnimaDefaultMouseInteractor.h"
#include "AnimaKeyboardInteractor.h"
#include "AnimaScenesManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaLightsManager.h"
#include "AnimaScene.h"
#include "AnimaCamera.h"

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
			bool rotate = false; // ((AnimaMouseDraggedEventArgs*)args)->GetModifiers() & (AInt)AnimaKeyboardModifier::AKM_SUPER;
			
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
//								g_camera.updatePosition(direction, elapsedTimeSec);
								break;
								
							case ACT_FLIGHT:
								dy = delta.y;
								dz = delta.x;
								
								camera->SmoothRotateDeg(0.0f, dy, dz);
								
//								if ((dx = -direction.x * CAMERA_SPEED_FLIGHT_YAW * elapsedTimeSec) != 0.0f)
//									camera->rotate(dx, 0.0f, 0.0f);
								
//								direction.x = 0.0f; // ignore yaw motion when updating camera's velocity
//								g_camera.updatePosition(direction, elapsedTimeSec);
								break;
								
							case ACT_ORBIT:
								dx = delta.x;
								dy = delta.y;
								
								camera->SmoothRotateDeg(dx, dy, 0.0f);
								
//								if (!camera->PreferTargetYAxisOrbiting())
//								{
//									if ((dz = direction.x * CAMERA_SPEED_ORBIT_ROLL * elapsedTimeSec) != 0.0f)
//										camera->RotateDeg(0.0f, 0.0f, dz);
//								}
								
//								if ((dz = -mouse.wheelPos()) != 0.0f)
//									camera->Zoom(dz, camera->GetOrbitMinZoom(), camera->GetOrbitMaxZoom());
								
								break;
						}
						
//						if(rotate == false)
//						{
//							delta /= 30.0f;
//							
////							camera->Move(camera->GetRight(), delta.x);
////							camera->Move(camera->GetUp(), delta.y);
//						}
//						else
//						{
////							camera->RotateXDeg(delta.y);
////							camera->RotateYDeg(delta.x);
//						}
					}
				}
			}
		});
		
		return true;
	}
	
	return false;
}

END_ANIMA_ENGINE_NAMESPACE