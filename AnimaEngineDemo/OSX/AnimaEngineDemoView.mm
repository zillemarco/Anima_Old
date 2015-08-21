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

#define ANIMA_ENGINE_DEMO_SCENE_NAME "AnimaEngineDemoScene"
#define ANIMA_ENGINE_DEMO_CAMERA_NAME "AnimaEngineDemoCamera"
#define ANIMA_ENGINE_DEMO_MODEL_NAME "AnimaEngineDemoModel"

#define SHADERS_PATH			"/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/"
#define DEFERRED_SHADERS_START	"Deferred/"
#define PRIMITIVE_SHADERS_START "Primitive/"
#define FILTERS_SHADERS_START	"Filters/"

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
	
	_camera->Move(1.0, 0.0, 0.0, ((float)xDelta) / 100.0f);
	_camera->Move(0.0, 1.0, 0.0, ((float)yDelta) / 100.0f);
	
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
	
	lastXPos = pt.x;
	lastYPos = pt.y;
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
			switch (keyChar)
			{
				case 'p':
				case 'P':
				{
					_pbrMaterial->SetFloat("Metallic", 0.0f);
					break;
				}
				case 'm':
				case 'M':
				{
					_pbrMaterial->SetFloat("Metallic", 1.0f);
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
	
	// Creazione del renderer
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());
	
	// Creazione della scena
	_scene = _engine.GetScenesManager()->CreateScene(ANIMA_ENGINE_DEMO_SCENE_NAME);
	if (!_scene)
		return false;
	
	Anima::AnimaString dataPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data";
	Anima::AnimaString shadersPartsPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/shaders/Parts";
	Anima::AnimaString shadersPath = SHADERS_PATH;
	Anima::AnimaString materialsPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/materials";
	//Anima::AnimaString modelPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/models/material.3ds";
	Anima::AnimaString modelPath = "/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data/models/matTester.obj";
	
	// Caricamento degli shader
	if (!_renderer->InitializeShaders(shadersPath, shadersPartsPath))
		return false;
	
	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	Anima::AnimaModelInstancesManager* modelInstancesManager = _scene->GetModelInstancesManager();
	materialsManager->LoadMaterials(materialsPath);
	
	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
	_camera = _camerasManager->CreateThirdPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;
	
	_model = _scene->GetModelsManager()->LoadModelFromExternalFile(modelPath, ANIMA_ENGINE_DEMO_MODEL_NAME);
	if (!_model)
		return false;
	
	//_model->GetTransformation()->RotateXDeg(-90.0);
	_model->GetTransformation()->RotateYDeg(180.0);
	
	_floorModel = _scene->GetModelsManager()->CreateModel("floorModel");
	Anima::AnimaMesh* floorModelMesh = _scene->GetMeshesManager()->CreateMesh("floorModelMesh");
	floorModelMesh->MakePlane();
	floorModelMesh->SetParentObject(_floorModel);
	_floorModel->AddMesh(floorModelMesh);
	
	Anima::AnimaModelInstance* modelInstance1 = modelInstancesManager->CreateInstance("modelInstance-1", _model);
	Anima::AnimaModelInstance* modelInstance2 = modelInstancesManager->CreateInstance("modelInstance-2", _model);
	Anima::AnimaModelInstance* modelInstance3 = modelInstancesManager->CreateInstance("modelInstance-3", _model);
	Anima::AnimaModelInstance* modelInstance4 = modelInstancesManager->CreateInstance("modelInstance-4", _model);
	
	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance1Meshes;
	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance2Meshes;
	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance3Meshes;
	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance4Meshes;
	
	modelInstance1->GetAllMeshes(&modelInstance1Meshes);
	modelInstance2->GetAllMeshes(&modelInstance2Meshes);
	modelInstance3->GetAllMeshes(&modelInstance3Meshes);
	modelInstance4->GetAllMeshes(&modelInstance4Meshes);
	
	floorModelMesh->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
	
	for (int i = 0; i < modelInstance1Meshes.size(); i++)
		modelInstance1Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model1-material"));
	
	for (int i = 0; i < modelInstance2Meshes.size(); i++)
		modelInstance2Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model2-material"));
	
	for (int i = 0; i < modelInstance3Meshes.size(); i++)
		modelInstance3Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model3-material"));
	
	for (int i = 0; i < modelInstance4Meshes.size(); i++)
		modelInstance4Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model4-material"));
	
	modelInstance1->GetTransformation()->SetTranslation(-40, 0, -40);
	modelInstance1->GetTransformation()->RotateYDeg(180.0);
	modelInstance2->GetTransformation()->SetTranslation(40, 0, -40);
	modelInstance2->GetTransformation()->RotateYDeg(180.0);
	modelInstance3->GetTransformation()->SetTranslation(40, 0, 40);
	modelInstance3->GetTransformation()->RotateYDeg(180.0);
	modelInstance4->GetTransformation()->SetTranslation(-40, 0, 40);
	modelInstance4->GetTransformation()->RotateYDeg(180.0);
	
	_floorModel->GetTransformation()->SetScale(200, 0, 200);
	
	_pbrMaterial = materialsManager->GetMaterialFromName("model3-material");
	
	Anima::AnimaDirectionalLight* light = _scene->GetLightsManager()->CreateDirectionalLight("light-0");
	light->SetDirection(-1.0, -1.0, -1.0);
	light->SetColor(1.0, 1.0, 1.0);
	light->SetIntensity(0.8);
	
	_camera->LookAt(0.0, 40.0, 100.0, 0.0, 15.0, 0.0);
	//_camera->LookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0);
	_camera->Activate();
	
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
			
			// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = fmax(mipMapsCount, 1);
			
			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}
			
			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);
			
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
			
			// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = fmax(mipMapsCount, 1);
			
			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}
			
			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);
			
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
	
	return true;
}

- (void) setViewportWidth: (float)w Height: (float)h
{
	if (_camerasManager)
	{
		Anima::AnimaVertex2f size((float)w, (float)h);
		_camerasManager->UpdatePerspectiveCameras(45.0f, size, 0.1f, 10000.0f);
	}
	
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
