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
#include <AnimaModelsManager.h>
#include <AnimaScenesManager.h>
#include <AnimaMaterialsManager.h>
#include <AnimaShadersManager.h>
#include <AnimaCamerasManager.h>
#include <AnimaAnimationsManager.h>
#include <AnimaModelInstancesManager.h>
#include <AnimaMaterialsManager.h>
#include <AnimaMeshesManager.h>
#include <AnimaLightsManager.h>
#include <AnimaTexturesManager.h>
#include <AnimaParallelProgramsManager.h>

#include <AnimaScene.h>
#include <AnimaModel.h>
#include <AnimaMesh.h>
#include <AnimaShaderProgram.h>
#include <AnimaRenderer.h>
#include <AnimaFirstPersonCamera.h>
#include <AnimaAnimation.h>
#include <AnimaTimer.h>
#include <AnimaModelInstance.h>
#include <AnimaMaterial.h>
#include <AnimaLight.h>
#include <AnimaTexture.h>
#include <AnimaRandom.h>

#define ANIMA_ENGINE_DEMO_SCENE_NAME "AnimaEngineDemoScene"
#define ANIMA_ENGINE_DEMO_CAMERA_NAME "AnimaEngineDemoCamera"
#define ANIMA_ENGINE_DEMO_MODEL_NAME "AnimaEngineDemoModel"

//#define DATA_PATH				"data"
#define DATA_PATH				"/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngineDemo/Scene"

Anima::AnimaGC* _gc = nullptr;
Anima::AnimaScene* _scene = nullptr;
Anima::AnimaCamerasManager* _camerasManager = nullptr;
Anima::AnimaCamera* _camera = nullptr;
Anima::AnimaModel* _model = nullptr;
Anima::AnimaModel* _floorModel = nullptr;
Anima::AnimaAnimationsManager* _animationsManager = nullptr;
Anima::AnimaRenderer* _renderer = nullptr;
Anima::AnimaEngine _engine;
Anima::AnimaTimer _timer;
Anima::AnimaTimer _fpsTimer;
Anima::AnimaMaterial* _pbrMaterial;

int lastXPos = 0;
int lastYPos = 0;

float camSpeed = 1.0f;
float camSpeedInc = 0.5f;
float camSpeedMax = 10.0f;

@implementation AnimaEngineDemoView
{
	CVDisplayLinkRef displayLink;
}

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
	CVDisplayLinkRelease(displayLink);
	[[NSNotificationCenter defaultCenter] removeObserver: self];
}

- (void) update
{
	if(gc != NULL)
	{
		gc->MakeCurrent();
		[self drawScene];
		gc->SwapBuffers();
	}
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
			
			[self initEngine];
			[self setViewportWidth:rc.size.width Height:rc.size.height];
			
			if(gc->GetContext() != nil && gc->GetPixelFormat() != nil)
			{
				NSOpenGLContext* context = gc->GetContext();
				NSOpenGLPixelFormat* pixelFormat = gc->GetPixelFormat();
				
				GLint swapInt = 1;
				[context setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
				
				// Create a display link capable of being used with all active displays
				CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
				
				// Set the renderer output callback function
				CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, (__bridge void*)self);
				
				// Set the display link for the current renderer
				CGLContextObj cglContext = [context CGLContextObj];
				CGLPixelFormatObj cglPixelFormat = [pixelFormat CGLPixelFormatObj];
				CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
				
				// Activate the display link
				CVDisplayLinkStart(displayLink);
			}
		}
	}
	
	return true;
}

// This is the renderer output callback function
static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
	CVReturn result = [(__bridge AnimaEngineDemoView*)displayLinkContext getFrameForTime:outputTime];
	return result;
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

- (void) mouseDown:(NSEvent *)theEvent
{
	NSPoint pt = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	lastXPos = pt.x;
	lastYPos = pt.y;
}

- (void) rightMouseDown:(NSEvent *)theEvent
{
	NSPoint pt = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	lastXPos = pt.x;
	lastYPos = pt.y;
}

- (void) mouseDragged:(NSEvent *)theEvent
{
	NSPoint pt = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	int xDelta = lastXPos - pt.x;
	int yDelta = lastYPos - pt.y;
	
	_camera->Move(_camera->GetRight(), ((float)xDelta) / 3.0f);
	_camera->Move(_camera->GetUp(), ((float)yDelta) / 3.0f);

	_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
	
	lastXPos = pt.x;
	lastYPos = pt.y;
}

- (void) rightMouseDragged:(NSEvent *)theEvent
{
	NSPoint pt = [self convertPoint:[theEvent locationInWindow] fromView:nil];
	
	int xDelta = lastXPos - pt.x;
	int yDelta = lastYPos - pt.y;
	
	_camera->RotateXDeg((float)yDelta);
	_camera->RotateYDeg((float)xDelta);
	
	_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
	
	lastXPos = pt.x;
	lastYPos = pt.y;
}

- (void) keyUp:(NSEvent *)theEvent
{
	camSpeed = 1.0f;
}

- (void) keyDown:(NSEvent *)theEvent
{
	NSString *theArrow = [theEvent charactersIgnoringModifiers];
	unichar keyChar = 0;
	if ( [theArrow length] == 0 )
		return;
	
	if ( [theArrow length] == 1 )
	{
		keyChar = [theArrow characterAtIndex:0];
		
		if (_pbrMaterial != nullptr)
		{
			float inc = 0.02f;
			camSpeed = fmin(camSpeed + camSpeedInc, camSpeedMax);
			switch (keyChar)
			{
				case 'w':
				case 'W':
				{
					_camera->Move(_camera->GetForward(), camSpeed);
					_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
					break;
				}
				case 's':
				case 'S':
				{
					_camera->Move(_camera->GetForward(), -camSpeed);
					_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
					break;
				}
				case 'd':
				case 'D':
				{
					_camera->Move(_camera->GetRight(), camSpeed);
					_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
					break;
				}
				case 'a':
				case 'A':
				{
					_camera->Move(_camera->GetRight(), -camSpeed);
					_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
					break;
				}
				case 'p':
				case 'P':
				{
					float val = _pbrMaterial->GetFloat("Metallic");
					val = fmax(0.0f, val - inc);
					_pbrMaterial->SetFloat("Metallic", val);
					break;
				}
				case 'm':
				case 'M':
				{
					float val = _pbrMaterial->GetFloat("Metallic");
					val = fmin(1.0f, val + inc);
					_pbrMaterial->SetFloat("Metallic", val);
					break;
				}
				case 'x':
				case 'X':
				{
					float val = _pbrMaterial->GetFloat("Specular");
					val = fmax(0.0f, val - inc);
					_pbrMaterial->SetFloat("Specular", val);
					break;
				}
				case 'z':
				case 'Z':
				{
					float val = _pbrMaterial->GetFloat("Specular");
					val = fmin(1.0f, val + inc);
					_pbrMaterial->SetFloat("Specular", val);
					break;
				}
				case 'v':
				case 'V':
				{
					float val = _renderer->GetFloat("BlurSize");
					val = fmax(0.0f, val - (inc / 1000.0));
					_renderer->SetFloat("BlurSize", val);
					printf("%f\n", val);
					break;
				}
				case 'b':
				case 'B':
				{
					float val = _renderer->GetFloat("BlurSize");
					val = fmin(0.5f, val + (inc / 1000.0));
					_renderer->SetFloat("BlurSize", val);
					printf("%f\n", val);
					break;
				}
				case 'l':
				case 'L':
				{
					float val = _renderer->GetFloat("BrightnessThreshold");
					val = fmax(0.0f, val - inc);
					_renderer->SetFloat("BrightnessThreshold", val);
					printf("%f\n", val);
					break;
				}
				case 'k':
				case 'K':
				{
					float val = _renderer->GetFloat("BrightnessThreshold");
					val = fmin(1.0f, val + inc);
					_renderer->SetFloat("BrightnessThreshold", val);
					printf("%f\n", val);
					break;
				}
				case 'h':
				case 'H':
				{
					float val = _renderer->GetFloat("BloomBlurScale");
					val = fmax(0.0f, val - (inc / 1000.0));
					_renderer->SetFloat("BloomBlurScale", val);
					printf("%f\n", val);
					break;
				}
				case 'g':
				case 'G':
				{
					float val = _renderer->GetFloat("BloomBlurScale");
					val = fmin(0.5f, val + (inc / 1000.0));
					_renderer->SetFloat("BloomBlurScale", val);
					printf("%f\n", val);
					break;
				}
				case '+':
				{
					float val = _pbrMaterial->GetFloat("Roughness");
					val = fmin(1.0f, val + inc);
					_pbrMaterial->SetFloat("Roughness", val);
					break;
				}
				case '-':
				{
					float val = _pbrMaterial->GetFloat("Roughness");
					val = fmax(0.0f, val - inc);
					_pbrMaterial->SetFloat("Roughness", val);
					break;
				}
				case '*':
				{
					float val = _pbrMaterial->GetFloat("ReflectionIntensity");
					val = fmin(1.0f, val + inc);
					_pbrMaterial->SetFloat("ReflectionIntensity", val);
					break;
				}
				case '/':
				{
					float val = _pbrMaterial->GetFloat("ReflectionIntensity");
					val = fmax(0.0f, val - inc);
					_pbrMaterial->SetFloat("ReflectionIntensity", val);
					break;
				}
				case '1':
				{
					_pbrMaterial = _scene->GetMaterialsManager()->GetMaterialFromName("oro");
					break;
				}
				case '2':
				{
					_pbrMaterial = _scene->GetMaterialsManager()->GetMaterialFromName("rame");
					break;
				}
				case '3':
				{
					_pbrMaterial = _scene->GetMaterialsManager()->GetMaterialFromName("gomma");
					break;
				}
				case '4':
				{
					_pbrMaterial = _scene->GetMaterialsManager()->GetMaterialFromName("legno");
					break;
				}
				case '5':
				{
					_pbrMaterial = _scene->GetMaterialsManager()->GetMaterialFromName("floor-material");
					break;
				}
				case '0':
				{
					_renderer->SetBoolean("ApplyFXAA", !(_renderer->GetBoolean("ApplyFXAA")));
					break;
				}
			}
		}
		[super keyDown:theEvent];
	}
	[super keyDown:theEvent];
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
	
	printf("\nOpenCL available platforms:\n\n");
	Anima::AnimaParallelProgramsManager* ppManager = _engine.GetParallelProgramsManager();
	Anima::AnimaArray<cl_platform_id> platforms = ppManager->GetPlatformIDs();
	for (auto& plat : platforms)
	{
		Anima::AnimaString platName = ppManager->GetPlatformName(plat);
		Anima::AnimaString platVer = ppManager->GetPlatformVersion(plat);
		printf("- Platform name: %s [version %s]\n", platName.c_str(), platVer.c_str());
		
		Anima::AnimaArray<cl_device_id> devices = ppManager->GetDeviceIDs(plat, Anima::AnimaParallelelProgramType::APP_TYPE_ALL);
		for (auto& dev : devices)
		{
			Anima::AnimaString devName = ppManager->GetDeviceName(dev);
			Anima::AnimaString devVer = ppManager->GetDeviceVersion(dev);
			printf("\t- Device name: %s [version %s]\n", devName.c_str(), devVer.c_str());
		}
	}
	// Creazione del renderer
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());
	
	// Creazione della scena
#if defined SAVE_SCENE
	_scene = _engine.GetScenesManager()->CreateScene(ANIMA_ENGINE_DEMO_SCENE_NAME);
#else
	_engine.GetScenesManager()->LoadScenes("/Users/marco/Desktop/Scene");
	
#endif
	
	_scene = _engine.GetScenesManager()->GetSceneFromName(ANIMA_ENGINE_DEMO_SCENE_NAME);

	if (!_scene)
		return false;
	
	Anima::AnimaString dataPath = DATA_PATH;
	Anima::AnimaString materialsPath = dataPath + "/materials";
	Anima::AnimaString modelPath = dataPath + "/models/";
	Anima::AnimaString modelName = "";
	Anima::AnimaString materialName = "legno";
	Anima::AnimaString sceneModelsPath = modelPath + "scene_models/";
	Anima::AnimaString sceneMeshesPath = modelPath + "scene_models/";
	
	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	Anima::AnimaModelInstancesManager* modelInstancesManager = _scene->GetModelInstancesManager();
#if defined SAVE_SCENE
	if (!materialsManager->LoadMaterials(materialsPath))
		return false;
#endif
	
	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
	_camera = _camerasManager->CreateFirstPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;
	
#if defined SAVE_SCENE
	_scene->GetMeshesManager()->LoadMeshes(sceneMeshesPath);
	_scene->GetModelsManager()->LoadModels(sceneModelsPath);
#endif
	
	_model = _scene->GetModelsManager()->GetModelFromName("AnimaEngineDemoModel");
	if (!_model)
		return false;
	
	_floorModel = _scene->GetModelsManager()->GetModelFromName("floorModel");
	if (!_floorModel)
		return false;
	
	_pbrMaterial = materialsManager->GetMaterialFromName(materialName);
	
	// Creo le istanze del pavimento e imposto i loro materiali
	Anima::AnimaArray<Anima::AnimaMeshInstance*> floorModelInstanceMeshes;
	
	Anima::AnimaModelInstance* floorModelInstance1 = modelInstancesManager->CreateInstance("floorModelInstance1", _floorModel);
	Anima::AnimaModelInstance* floorModelInstance2 = modelInstancesManager->CreateInstance("floorModelInstance2", _floorModel);
	Anima::AnimaModelInstance* floorModelInstance3 = modelInstancesManager->CreateInstance("floorModelInstance3", _floorModel);
	Anima::AnimaModelInstance* floorModelInstance4 = modelInstancesManager->CreateInstance("floorModelInstance4", _floorModel);
	
	floorModelInstance1->GetTransformation()->SetScale(60, 60, 60);
	floorModelInstance1->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance1->GetTransformation()->SetRotationYDeg(-90);
	floorModelInstance1->GetAllMeshes(&floorModelInstanceMeshes);
	
	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));

	floorModelInstanceMeshes.clear();
	floorModelInstance2->GetTransformation()->SetScale(60, 60, 60);
	floorModelInstance2->GetTransformation()->TranslateX(300);
	floorModelInstance2->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance2->GetTransformation()->SetRotationYDeg(-90);
	floorModelInstance2->GetAllMeshes(&floorModelInstanceMeshes);
	
	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
	
	floorModelInstanceMeshes.clear();
	floorModelInstance3->GetTransformation()->SetScale(60, 60, 60);
	floorModelInstance3->GetTransformation()->TranslateZ(300);
	floorModelInstance3->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance3->GetTransformation()->SetRotationYDeg(-270);
	floorModelInstance3->GetAllMeshes(&floorModelInstanceMeshes);
	
	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));

	floorModelInstanceMeshes.clear();
	floorModelInstance4->GetTransformation()->SetScale(60, 60, 60);
	floorModelInstance4->GetTransformation()->TranslateX(300);
	floorModelInstance4->GetTransformation()->TranslateZ(300);
	floorModelInstance4->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance4->GetTransformation()->SetRotationYDeg(-270);
	floorModelInstance4->GetAllMeshes(&floorModelInstanceMeshes);
	
	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
	
	// Creo le istanze degli oggetti applicando i materiali
	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstanceMeshes;
	
	Anima::AnimaModelInstance* modelInstanceOro = modelInstancesManager->CreateInstance("modelInstanceOro", _model);
	Anima::AnimaModelInstance* modelInstanceRame = modelInstancesManager->CreateInstance("modelInstanceRame", _model);
	Anima::AnimaModelInstance* modelInstanceGomma = modelInstancesManager->CreateInstance("modelInstanceGomma", _model);
	Anima::AnimaModelInstance* modelInstanceLegno = modelInstancesManager->CreateInstance("modelInstanceLegno", _model);
	
	modelInstanceOro->GetTransformation()->SetScale(20, 20, 20);
	modelInstanceOro->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceOro->GetTransformation()->SetRotationYDeg(0);
	modelInstanceOro->GetAllMeshes(&modelInstanceMeshes);
	
	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("oro"));
	
	modelInstanceMeshes.clear();
	modelInstanceRame->GetTransformation()->SetScale(20, 20, 20);
	modelInstanceRame->GetTransformation()->TranslateX(300);
	modelInstanceRame->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceRame->GetTransformation()->SetRotationYDeg(0);
	modelInstanceRame->GetAllMeshes(&modelInstanceMeshes);
	
	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("rame"));
	
	modelInstanceMeshes.clear();
	modelInstanceGomma->GetTransformation()->SetScale(20, 20, 20);
	modelInstanceGomma->GetTransformation()->TranslateZ(300);
	modelInstanceGomma->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceGomma->GetTransformation()->SetRotationYDeg(180);
	modelInstanceGomma->GetAllMeshes(&modelInstanceMeshes);
	
	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("gomma"));

	modelInstanceMeshes.clear();
	modelInstanceLegno->GetTransformation()->SetScale(20, 20, 20);
	modelInstanceLegno->GetTransformation()->TranslateX(300);
	modelInstanceLegno->GetTransformation()->TranslateZ(300);
	modelInstanceLegno->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceLegno->GetTransformation()->SetRotationYDeg(180);
	modelInstanceLegno->GetAllMeshes(&modelInstanceMeshes);
	
	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("legno"));

	
//	for (int i = 0; i < numInstances; i++)
//	{
//		Anima::AnimaString name = Anima::FormatString("modelInstance-%d", i);
//		Anima::AnimaModelInstance* modelInstance = modelInstancesManager->CreateInstance(name, _model);
//		Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstanceMeshes;
//		
//		modelInstance->GetAllMeshes(&modelInstanceMeshes);
//		
//		for (int j = 0; j < modelInstanceMeshes.size(); j++)
//			modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName(materialName));
//		
//		if (nufminstances > 1)
//			modelInstance->GetTransformation()->SetTranslation(cos(degOffset) * raggio, 0, sin(degOffset) * raggio);
//		
//		modelInstance->GetTransformation()->RotateYDeg(ry);
//		modelInstance->GetTransformation()->RotateXDeg(rx);
//		modelInstance->GetTransformation()->SetScale(scale, scale, scale);
//		
//		degOffset += span;
//	}
	
	_camera->LookAt(-350.0, 100.0, 150.0, 1.0, -0.2, 0.0);
	_camera->Activate();
	
	Anima::AnimaDirectionalLight* directionalLight = _scene->GetLightsManager()->CreateDirectionalLight("light-0");
	directionalLight->SetDirection(1.0, -1.0, -1.0);
	directionalLight->SetColor(1.0, 1.0, 1.0);
	directionalLight->SetIntensity(1.0);
	
	//Anima::AnimaPointLight* pointLight1 = _scene->GetLightsManager()->CreatePointLight("light-1");
	//pointLight1->SetPosition(-40.0, 20.0, 0.0);
	//pointLight1->SetColor(1.0, 0.0, 0.0);
	//pointLight1->SetIntensity(1.0);
	//pointLight1->SetConstantAttenuation(1.0);
	//pointLight1->SetLinearAttenuation(0.0);
	//pointLight1->SetExponentAttenuation(0.0);
	//pointLight1->SetRange(200);
	
	_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
	
	_animationsManager = _scene->GetAnimationsManager();
	
	_timer.Reset();
	
	Anima::AnimaTexturesManager* texturesManager = _scene->GetTexturesManager();
	
	Anima::AnimaArray<Anima::AnimaArray<Anima::AUchar> > data;
	Anima::AUint width;
	Anima::AUint height;
	Anima::AUint depth;
	Anima::AUint mipMapsCount;
	Anima::AUint imagesCount;
	Anima::AnimaTextureFormat format;
	Anima::AnimaTextureInternalFormat internalFormat;
	Anima::AnimaTextureTarget target;
	
	if (texturesManager->GetTextureDataFromDDSFile(dataPath + "/textures/Roma/cubemap.dds", &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
	{
		Anima::AnimaTexture* texture = _renderer->GetTexture("EnvironmentMap");
		if (texture != nullptr)
		{
			texture->SetWidth(width);
			texture->SetHeight(height);
			texture->SetDepth(depth);
			
			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}
			
			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);
			
			// Forzo ad avere come fminimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = fmax(mipMapsCount, 1);
			
			Anima::AInt offset = 0;
			for (Anima::AUint i = 0; i < imagesCount; i++)
			{
				for (Anima::AUint j = 0; j < mipMapsCount; j++)
				{
					Anima::AUchar* buffer = &data[offset][0];
					Anima::AUint bufferSize = data[offset].size();
					
					texture->SetData(buffer, bufferSize, (Anima::AnimaTextureCubeIndex)i, j);
					
					offset++;
				}
			}
			
			texture->SetMinFilter(Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR);
			texture->SetMagFilter(Anima::TEXTURE_MAG_FILTER_MODE_LINEAR);
		}
	}
	
	if (texturesManager->GetTextureDataFromDDSFile(dataPath + "/textures/Roma/Irradiance.dds", &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
	{
		Anima::AnimaTexture* texture = _renderer->GetTexture("IrradianceMap");
		if (texture != nullptr)
		{
			texture->SetWidth(width);
			texture->SetHeight(height);
			texture->SetDepth(depth);
			
			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}
			
			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);
			
			// Forzo ad avere come fminimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = fmax(mipMapsCount, 1);
			
			texture->SetTextureTarget(target);
			
			Anima::AInt offset = 0;
			for (Anima::AUint i = 0; i < imagesCount; i++)
			{
				for (Anima::AUint j = 0; j < mipMapsCount; j++)
				{
					Anima::AUchar* buffer = &data[offset][0];
					Anima::AUint bufferSize = data[offset].size();
					
					texture->SetData(buffer, bufferSize, (Anima::AnimaTextureCubeIndex)i, j);
					
					offset++;
				}
			}
			
			texture->SetMinFilter(Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR);
			texture->SetMagFilter(Anima::TEXTURE_MAG_FILTER_MODE_LINEAR);
		}
	}
	
#if defined SAVE_SCENE
	Anima::AnimaTexture* textureSkyBox = texturesManager->LoadTextureFromDDSFile(dataPath + "/textures/Roma/cubemap.dds", "dds-skybox-texture");
#else
	Anima::AnimaTexture* textureSkyBox = texturesManager->GetTextureFromName("dds-skybox-texture");
	textureSkyBox->Load();
#endif
	_renderer->SetTexture("SkyBox", textureSkyBox, false);
	_renderer->CheckPrograms(_scene);
	
#if defined SAVE_SCENE
	_engine.GetScenesManager()->SaveSceneToFile(_scene, "/Users/marco/Desktop/Scene", true);
#endif
	
	return true;
}

- (void) setViewportWidth: (float)w Height: (float)h
{
	if (_camerasManager)
	{
		Anima::AnimaVertex2f size((float)w, (float)h);
		_camerasManager->UpdatePerspectiveCameras(45.0f, size, 0.1f, 1000.0f);
	}
	
	if(_scene)
		_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
	
	if (_renderer)
	{
		_renderer->InitRenderingTargets(w, h);
		_renderer->InitRenderingUtilities(w, h);
	}
}

- (void) drawScene
{
	if(_renderer && _scene)
	{
		_renderer->Start(_scene);
		_renderer->Render();
	}
}

@end
