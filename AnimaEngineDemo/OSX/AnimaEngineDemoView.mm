//
//  AnimaEngineDemoView.m
//  Anima
//
//  Created by Marco Zille on 17/08/15.
//
//

#import "AnimaEngineDemoView.h"

// AnimaEngine includes
#include <AnimaGC.h>
#include <AnimaEngine.h>
#include <AnimaNodesManager.h>
#include <AnimaScenesManager.h>
#include <AnimaMaterialsManager.h>
#include <AnimaShadersManager.h>
#include <AnimaCamerasManager.h>
#include <AnimaAnimationsManager.h>
#include <AnimaNodeInstancesManager.h>
#include <AnimaMaterialsManager.h>
#include <AnimaGeometriesManager.h>
#include <AnimaLightsManager.h>
#include <AnimaTexturesManager.h>
#include <AnimaParallelProgramsManager.h>
#include <AnimaPhysicsDebugDrawer.h>

#include <AnimaScene.h>
#include <AnimaNode.h>
#include <AnimaGeometry.h>
#include <AnimaShaderProgram.h>
#include <AnimaRenderer.h>
#include <AnimaFirstPersonCamera.h>
#include <AnimaAnimation.h>
#include <AnimaTimer.h>
#include <AnimaNodeInstance.h>
#include <AnimaMaterial.h>
#include <AnimaLight.h>
#include <AnimaTexture.h>
#include <AnimaRandom.h>
#include <AnimaDefaultInteractors.h>

#define ANIMA_ENGINE_DEMO_SCENE_NAME "AnimaEngineDemoScene"
#define ANIMA_ENGINE_DEMO_CAMERA_NAME "AnimaEngineDemoCamera"
#define ANIMA_ENGINE_DEMO_MODEL_NAME "AnimaEngineDemoModel"

//#define DATA_PATH				"data"
//#define DATA_PATH				"/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngineDemo/Scene"
#define DATA_PATH				"/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngineDemo/SceneUff"

#include <mutex>
std::mutex mutex;

Anima::AnimaGC* _gc = nullptr;
Anima::AnimaScene* _scene = nullptr;
//Anima::AnimaCamerasManager* _camerasManager = nullptr;
//Anima::AnimaCamera* _camera = nullptr;
Anima::AnimaNode* _model = nullptr;
Anima::AnimaNode* _floorModel = nullptr;
Anima::AnimaAnimationsManager* _animationsManager = nullptr;
Anima::AnimaRenderer* _renderer = nullptr;
Anima::AnimaEngine _engine;
Anima::AnimaTimer _timer;
Anima::AnimaTimer _fpsTimer;
Anima::AnimaMaterial* _pbrMaterial;

Anima::AnimaDefaultMouseInteractor mouseInteractor;
Anima::AnimaDefaultKeyboardInteractor keyboardInteractor;
Anima::AnimaDefaultJoystickInteractor* joystickInteractor = (Anima::AnimaDefaultJoystickInteractor*)Anima::AnimaDefaultJoystickInteractor::GetInstance();

float lastXPos = 0;
float lastYPos = 0;

bool mouseMoved = false;

float camSpeed = 0.1f;
float camSpeedInc = 0.01f;
float camSpeedMax = 1.0f;

bool sceneSaved = false;

bool moveForwardsPressed = false;
bool moveBackwardsPressed = false;
bool moveRightPressed = false;
bool moveLeftPressed = false;
bool moveUpPressed = false;
bool moveDownPressed = false;
bool rendererInitialized = false;

@implementation AnimaEngineDemoView

- (id) initWithFrame: (NSRect)frameRect
{
	return [self initWithFrame:frameRect contextConfig:Anima::AnimaGC::GetDefaultContextConfig() frameBufferConfig:Anima::AnimaGC::GetDefaultFrameBufferConfig()];
}

- (id) initWithFrame: (NSRect)frameRect contextConfig: (Anima::AnimaGCContextConfig)context frameBufferConfig: (Anima::AnimaGCFrameBufferConfig)frameBuffer
{
	self = [super initWithFrame:frameRect];
	if(!self)
		return nil;
	
	if(![self prepareWithContextConfig:context frameBufferConfig:frameBuffer])
		return nil;
	
	return self;
}

- (void) dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver: self];
	[super dealloc];
}

- (void) update
{
	mutex.lock();
	if(gc != NULL)
	{
		gc->MakeCurrent();
		[self drawScene];
		gc->SwapBuffers();
	}
	mutex.unlock();
}

- (bool) prepareWithContextConfig: (Anima::AnimaGCContextConfig)context frameBufferConfig: (Anima::AnimaGCFrameBufferConfig)frameBuffer
{
	if(gc == NULL)
	{
		[self setNeedsDisplay:YES];
		
		gc = Anima::AnimaGC::CreateContext((long)self, context, frameBuffer);
		
		if(gc == NULL)
		{
			NSLog(@"Error creating Graphic Context");
			return false;
		}
		else
		{
			[[NSNotificationCenter defaultCenter] addObserver: self
													 selector: @selector(_frameChanged:)
														 name: NSViewGlobalFrameDidChangeNotification
													   object: self];
			
			[self setPostsFrameChangedNotifications: YES];
			[[NSNotificationCenter defaultCenter] addObserver: self
													 selector: @selector(_frameChanged:)
														 name: NSViewFrameDidChangeNotification
													   object: self];
			
			NSRect rc = [self bounds];
			
			gc->SetUpdateFrameCallback((long)self, [](Anima::AnimaGC* context, long userData) {
				AnimaEngineDemoView* view = (AnimaEngineDemoView*)userData;
				[view update];
			});
			
			[self initEngine];
			[self setViewportWidth:rc.size.width Height:rc.size.height];
			
			joystickInteractor->Install(0, &_engine);
			if(keyboardInteractor.Install((long)self, &_engine))
			{
				keyboardInteractor.SetEventHandler("onKeyDown", [&](Anima::AnimaEventArgs* args) {
					Anima::AnimaKeyboardEventArgs* kArgs = (Anima::AnimaKeyboardEventArgs*)args;
					
					if(kArgs->GetKey() == Anima::AnimaKeyboardKey::AKK_L)
					{
						Anima::AnimaNodesManager* nodesManager = _scene->GetNodesManager();
						Anima::AnimaNodeInstancesManager* nodeInstancesManager = _scene->GetNodeInstancesManager();
						
						Anima::AnimaNode* asset = nodesManager->LoadAssetFromExternalFile("/Users/marco/Desktop/base_cassettiera/143853.3ds", "asset");
						if(asset)
						{
							Anima::AnimaNodeInstance* assetInstance = nodeInstancesManager->CreateAssetInstance("asset-inst", asset);
							if(assetInstance)
							{
								assetInstance->GetTransformation()->SetScale(0.01, 0.01, 0.01);
								assetInstance->GetTransformation()->TranslateX(-20);
								assetInstance->GetTransformation()->RotateXDeg(-90);
								
								_renderer->CheckPrograms(_scene);
							}
						}
					}
				});
			}
			
			if(mouseInteractor.Install((long)self, &_engine))
			{
				mouseInteractor.SetEventHandler("onLeftMouseClick", [] (Anima::AnimaEventArgs* args) {
					
					Anima::AnimaVertex2f point = ((Anima::AnimaMouseEventArgs*)args)->GetPoint();
					Anima::AnimaVertex2f size = ((Anima::AnimaMouseEventArgs*)args)->GetWindowSize();
					
					NSView* view = (NSView*)((Anima::AnimaMouseInteractor*)args->GetSourceEvent())->GetWindowId();
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
										Anima::AnimaGeometryInstance* instance = (Anima::AnimaGeometryInstance*)RayCallback.m_collisionObject->getUserPointer();
										Anima::AnimaVertex3f contactPoint(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z());
									
										Anima::AnimaString str = Anima::FormatString("Picked material named '%s'", instance->GetMaterial()->GetName().c_str());
									
										NSString *message = [NSString stringWithCString:str.c_str() encoding:[NSString defaultCStringEncoding]];
									
										NSAlert* alert = [[NSAlert alloc] init];
										[alert addButtonWithTitle:@"OK"];
										[alert setMessageText:message];
										[alert setAlertStyle:NSCriticalAlertStyle];
										[alert beginSheetModalForWindow:[view window] completionHandler:nil];
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
			}
		}
	}
	
	return true;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
	[self update];
	return kCVReturnSuccess;
}

- (BOOL) isOpaque
{
	return YES;
}

- (BOOL) acceptsFirstResponder
{
	return YES;
}

- (void) _frameChanged: (NSNotification *) aNot
{
	printf("Update frame da frame changed\n");
	[self update];
}

- (id) initWithCoder: (NSCoder *)aDecoder
{
	self = [super initWithCoder: aDecoder];
	if (!self)
		return nil;
	
	if(![self prepareWithContextConfig:Anima::AnimaGC::GetDefaultContextConfig() frameBufferConfig:Anima::AnimaGC::GetDefaultFrameBufferConfig()])
		return nil;
	
	return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
}

- (void) lockFocus
{
	if(gc != NULL)
	{
		NSOpenGLContext* context = gc->GetContext();
		
		[super lockFocus];
		
		if([context view] != self)
		{
			[context setView:self];
		}
		
		gc->MakeCurrent();
	}
}

-(void) viewDidMoveToWindow
{
	[super viewDidMoveToWindow];
	if ([self window] == nil)
	{
		if(gc != NULL)
		{
			[gc->GetContext() clearDrawable];
		}
	}
}

- (void) viewDidEndLiveResize
{
	[super viewDidEndLiveResize];
	
	NSRect rc = [self bounds];
	[self setViewportWidth:rc.size.width Height:rc.size.height];
}

- (bool) initEngine
{
	if (gc)
		gc->MakeCurrent();
	
	// Inizializzazione del motore
	if (!_engine.Initialize())
		return false;

	// Creazione del renderer
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());
	
	Anima::AnimaScenesManager* scenesManager = _engine.GetScenesManager();
	
//	_scene = scenesManager->LoadSceneFromFile("/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngineDemo/SceneUff/AnimaEngineDemoScene.ascene");
//	_renderer->CheckPrograms(_scene);
	
	_scene = scenesManager->CreateScene("scena");
	
	if(_scene)
	{
		Anima::AnimaCamerasManager* camerasManager = _scene->GetCamerasManager();
		Anima::AnimaLightsManager* lightsManager = _scene->GetLightsManager();
		Anima::AnimaTexturesManager* texturesManager = _scene->GetTexturesManager();
		
		Anima::AnimaCamera* newCamera = camerasManager->CreateCamera("default-camera");
		Anima::AnimaDirectionalLight* newLight = lightsManager->CreateDirectionalLight("default-light");
		
		Anima::AnimaString texturesPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngineDemo/Scene/textures";
		
		Anima::AnimaTexture* textureCube = texturesManager->LoadTextureFromDDSFile(texturesPath + "/Roma/cubemap.dds", "dds-skybox-texture");
		_scene->SetTexture("SkyBox", textureCube);
		_scene->SetTexture("EnvironmentMap", textureCube);
		
		Anima::AnimaTexture* textureIrr = texturesManager->LoadTextureFromDDSFile(texturesPath + "/Roma/Irradiance.dds", "dds-skybox-texture-irr");
		_scene->SetTexture("IrradianceMap", textureIrr);
		
		if(newLight)
		{
			newLight->SetDirection(1, -1, -1);
			newLight->SetColor(1, 1, 1);
			newLight->SetIntensity(1);
		}
		
		if(newCamera)
		{
			Anima::AnimaVertex3f position(-35, 10, 15);
			Anima::AnimaVertex3f target(-34, 9.8, 15);
			Anima::AnimaVertex3f up(0, 1, 0);
			newCamera->LookAt(position, target, up);
			newCamera->Activate();
		}
		
		_scene->SetKeyboardInteractor(&keyboardInteractor);
	}
	
//	Anima::AnimaString dataPath = DATA_PATH;
//	Anima::AnimaString materialsPath = dataPath + "/materials";
//	Anima::AnimaString modelPath = dataPath + "/models/";
//	Anima::AnimaString modelName = "";
//	Anima::AnimaString materialName = "oro";
//	Anima::AnimaString sceneModelsPath = modelPath + "scene_models/";
//	Anima::AnimaString sceneMeshesPath = modelPath + "scene_models/";
//	
//	// Creazione della scena
//#if defined SAVE_SCENE
//	_scene = _engine.GetScenesManager()->CreateScene(ANIMA_ENGINE_DEMO_SCENE_NAME);
//#else
//	_engine.GetScenesManager()->LoadScenes(dataPath);
//#endif
//	
//	_scene = _engine.GetScenesManager()->GetSceneFromName(ANIMA_ENGINE_DEMO_SCENE_NAME);
//
//	if (!_scene)
//		return false;
//
//	_scene->InitializePhysics();
//	_scene->InitializePhysicObjects();
//	
//	_scene->SetKeyboardInteractor(&keyboardInteractor);
//	_scene->SetJoystickInteractor(joystickInteractor);
//	
//	// Caricamento dei materiali
//	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
//	Anima::AnimaNodeInstancesManager* modelInstancesManager = _scene->GetNodeInstancesManager();
//#if defined SAVE_SCENE
//	if (!materialsManager->LoadMaterials(materialsPath))
//		return false;
//#endif
//	
//	// Creazione di una telecamera
//	_camerasManager = _scene->GetCamerasManager();
//#if defined SAVE_SCENE
//	_camera = _camerasManager->CreateFirstPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
//	if (!_camera)
//		return false;
//#else
//	_camera = _camerasManager->GetCameraFromName(ANIMA_ENGINE_DEMO_CAMERA_NAME);
//	if (!_camera)
//		return false;
//#endif
//	
//	_pbrMaterial = materialsManager->GetMaterialFromName(materialName);
//	
//#if defined SAVE_SCENE
//	_scene->GetMeshesManager()->LoadMeshes(sceneMeshesPath);
//	_scene->GetModelsManager()->LoadModels(sceneModelsPath);
//#endif
//	
//#if defined SAVE_SCENE
//	_model = _scene->GetModelsManager()->GetModelFromName("AnimaEngineDemoModel");
//	if (!_model)
//		return false;
//	
//	_floorModel = _scene->GetModelsManager()->GetModelFromName("floorModel");
//	if (!_floorModel)
//		return false;
//	
//	// Creo le istanze del pavimento e imposto i loro materiali
//	Anima::AnimaArray<Anima::AnimaMeshInstance*> floorModelInstanceMeshes;
//	
//	Anima::AnimaModelInstance* floorModelInstance1 = modelInstancesManager->CreateInstance("floorModelInstance1", _floorModel);
//	Anima::AnimaModelInstance* floorModelInstance2 = modelInstancesManager->CreateInstance("floorModelInstance2", _floorModel);
//	Anima::AnimaModelInstance* floorModelInstance3 = modelInstancesManager->CreateInstance("floorModelInstance3", _floorModel);
//	Anima::AnimaModelInstance* floorModelInstance4 = modelInstancesManager->CreateInstance("floorModelInstance4", _floorModel);
//	
//	floorModelInstance1->GetTransformation()->SetScale(0.6, 0.6, 0.6);
//	floorModelInstance1->GetTransformation()->SetRotationXDeg(-90);
//	floorModelInstance1->GetTransformation()->SetRotationYDeg(-90);
//	floorModelInstance1->GetAllMeshes(&floorModelInstanceMeshes);
//	
//	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
//		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
//
//	floorModelInstanceMeshes.clear();
//	floorModelInstance2->GetTransformation()->SetScale(0.6, 0.6, 0.6);
//	floorModelInstance2->GetTransformation()->TranslateX(3);
//	floorModelInstance2->GetTransformation()->SetRotationXDeg(-90);
//	floorModelInstance2->GetTransformation()->SetRotationYDeg(-90);
//	floorModelInstance2->GetAllMeshes(&floorModelInstanceMeshes);
//	
//	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
//		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
//	
//	floorModelInstanceMeshes.clear();
//	floorModelInstance3->GetTransformation()->SetScale(0.6, 0.6, 0.6);
//	floorModelInstance3->GetTransformation()->TranslateZ(3);
//	floorModelInstance3->GetTransformation()->SetRotationXDeg(-90);
//	floorModelInstance3->GetTransformation()->SetRotationYDeg(-270);
//	floorModelInstance3->GetAllMeshes(&floorModelInstanceMeshes);
//	
//	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
//		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
//
//	floorModelInstanceMeshes.clear();
//	floorModelInstance4->GetTransformation()->SetScale(0.6, 0.6, 0.6);
//	floorModelInstance4->GetTransformation()->TranslateX(3);
//	floorModelInstance4->GetTransformation()->TranslateZ(3);
//	floorModelInstance4->GetTransformation()->SetRotationXDeg(-90);
//	floorModelInstance4->GetTransformation()->SetRotationYDeg(-270);
//	floorModelInstance4->GetAllMeshes(&floorModelInstanceMeshes);
//	
//	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
//		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
//	
//	// Creo le istanze degli oggetti applicando i materiali
//	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstanceMeshes;
//	
//	Anima::AnimaModelInstance* modelInstanceOro = modelInstancesManager->CreateInstance("modelInstanceOro", _model);
//	Anima::AnimaModelInstance* modelInstanceRame = modelInstancesManager->CreateInstance("modelInstanceRame", _model);
//	Anima::AnimaModelInstance* modelInstanceGomma = modelInstancesManager->CreateInstance("modelInstanceGomma", _model);
//	Anima::AnimaModelInstance* modelInstanceLegno = modelInstancesManager->CreateInstance("modelInstanceLegno", _model);
//	
//	modelInstanceOro->GetTransformation()->SetScale(0.2, 0.2, 0.2);
//	modelInstanceOro->GetTransformation()->SetRotationXDeg(-90);
//	modelInstanceOro->GetTransformation()->SetRotationYDeg(0);
//	modelInstanceOro->GetAllMeshes(&modelInstanceMeshes);
//	
//	for (int j = 0; j < modelInstanceMeshes.size(); j++)
//		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("oro"));
//	
//	modelInstanceMeshes.clear();
//	modelInstanceRame->GetTransformation()->SetScale(0.2, 0.2, 0.2);
//	modelInstanceRame->GetTransformation()->TranslateX(3);
//	modelInstanceRame->GetTransformation()->SetRotationXDeg(-90);
//	modelInstanceRame->GetTransformation()->SetRotationYDeg(0);
//	modelInstanceRame->GetAllMeshes(&modelInstanceMeshes);
//	
//	for (int j = 0; j < modelInstanceMeshes.size(); j++)
//		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("rame"));
//	
//	modelInstanceMeshes.clear();
//	modelInstanceGomma->GetTransformation()->SetScale(0.2, 0.2, 0.2);
//	modelInstanceGomma->GetTransformation()->TranslateZ(3);
//	modelInstanceGomma->GetTransformation()->SetRotationXDeg(-90);
//	modelInstanceGomma->GetTransformation()->SetRotationYDeg(180);
//	modelInstanceGomma->GetAllMeshes(&modelInstanceMeshes);
//	
//	for (int j = 0; j < modelInstanceMeshes.size(); j++)
//		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("gomma"));
//
//	modelInstanceMeshes.clear();
//	modelInstanceLegno->GetTransformation()->SetScale(0.2, 0.2, 0.2);
//	modelInstanceLegno->GetTransformation()->TranslateX(3);
//	modelInstanceLegno->GetTransformation()->TranslateZ(3);
//	modelInstanceLegno->GetTransformation()->SetRotationXDeg(-90);
//	modelInstanceLegno->GetTransformation()->SetRotationYDeg(180);
//	modelInstanceLegno->GetAllMeshes(&modelInstanceMeshes);
//	
//	for (int j = 0; j < modelInstanceMeshes.size(); j++)
//		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("legno"));
//	
//	_camera->LookAt(-3.5, 1.0, 1.5, 1.0, -0.2, 0.0);
//#endif
//	
//	_camera->Activate();
//	_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
//	
//#if defined SAVE_SCENE
//	Anima::AnimaDirectionalLight* directionalLight = _scene->GetLightsManager()->CreateDirectionalLight("light-0");
//	directionalLight->SetDirection(1.0, -1.0, -1.0);
//	directionalLight->SetColor(1.0, 1.0, 1.0);
//	directionalLight->SetIntensity(1.0);
//	
//	//Anima::AnimaPointLight* pointLight1 = _scene->GetLightsManager()->CreatePointLight("light-1");
//	//pointLight1->SetPosition(-40.0, 20.0, 0.0);
//	//pointLight1->SetColor(1.0, 0.0, 0.0);
//	//pointLight1->SetIntensity(1.0);
//	//pointLight1->SetConstantAttenuation(1.0);
//	//pointLight1->SetLinearAttenuation(0.0);
//	//pointLight1->SetExponentAttenuation(0.0);
//	//pointLight1->SetRange(200);
//#endif
//	
//	_animationsManager = _scene->GetAnimationsManager();
//	
//	_timer.Reset();
//	
//#if defined SAVE_SCENE
//	Anima::AnimaTexturesManager* texturesManager = _scene->GetTexturesManager();
//	
//	Anima::AnimaTexture* textureCube = texturesManager->LoadTextureFromDDSFile(dataPath + "/../Scene/textures/Roma/cubemap.dds", "dds-skybox-texture");
//	_scene->SetTexture("SkyBox", textureCube);
//	_scene->SetTexture("EnvironmentMap", textureCube);
//	
//	Anima::AnimaTexture* textureIrr = texturesManager->LoadTextureFromDDSFile(dataPath + "/../Scene/textures/Roma/Irradiance.dds", "dds-skybox-texture-irr");
//	_scene->SetTexture("IrradianceMap", textureIrr);
//#endif
//	
//	Anima::AnimaNode* newAsset = _scene->GetNodesManager()->LoadAssetFromExternalFile("/Users/marco/Desktop/base_cassettiera/143853.3ds", "base");
//	Anima::AnimaNodeInstance* newInstace = _scene->GetNodeInstancesManager()->CreateAssetInstance("base-instance", newAsset);
//	newInstace->GetTransformation()->SetScale(0.01, 0.01, 0.01);
//	newInstace->GetTransformation()->TranslateY(20);
//	newInstace->GetTransformation()->RotateXDeg(-90);
//	
//	_renderer->CheckPrograms(_scene);
	
	return true;
}

- (void) setViewportWidth: (float)w Height: (float)h
{
	if (_scene)
	{
		Anima::AnimaVertex2f size((float)w, (float)h);
		_scene->GetCamerasManager()->UpdatePerspectiveCameras(90.0f, size, 0.1f, 10000.0f);
	}
	
	if (_renderer)
	{
		_renderer->InitRenderingTargets(w, h);
		_renderer->InitRenderingUtilities(w, h);
		
		rendererInitialized = true;
	}
	
	if(!sceneSaved)
	{
#if defined SAVE_SCENE
		_engine.GetScenesManager()->SaveSceneToFile(_scene, "/Users/marco/Desktop/scene4", true);
#endif
		sceneSaved = true;
	}
}

- (void) drawScene
{
	if(_renderer && rendererInitialized && _scene)
	{
		if(!_scene->IsRunning())
			_scene->StartScene();
		
		_scene->StepScene();
		
		_renderer->Start(_scene);
		_renderer->Render();
	}
}

@end
