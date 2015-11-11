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
						if(rotate == false)
						{
							delta /= 30.0f;
							
							camera->Move(camera->GetRight(), delta.x);
							camera->Move(camera->GetUp(), delta.y);
						}
						else
						{
							camera->RotateXDeg(delta.y);
							camera->RotateYDeg(delta.x);
						}
						
						scene->GetLightsManager()->UpdateLightsMatrix(camera);
					}
				}
			}
		});
		
		return true;
	}
	
	return false;
}

END_ANIMA_ENGINE_NAMESPACE