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
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	shadersManager->LoadShadersParts(shadersPartsPath);
	
	Anima::AnimaShaderProgram* prepareProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/static-mesh-base-material-pbr.asp");
	//Anima::AnimaShaderProgram* prepareProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/static-mesh-base-material.asp");
	if (!prepareProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* directionalLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/directional-light-pbr.asp");
	//Anima::AnimaShaderProgram* directionalLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/directional-light.asp");
	if (!directionalLightProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* skyboxProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/skybox.asp");
	if (!skyboxProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* hemisphereLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/hemisphere-light.asp");
	if (!hemisphereLightProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* pointLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/point-light.asp");
	if (!pointLightProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* spotLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/spot-light.asp");
	if (!spotLightProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* nullFilterProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/nullFilter.asp");
	if (!nullFilterProgram->Link())
		return false;
	
	Anima::AnimaShaderProgram* combineProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/combine-pbr.asp");
	//Anima::AnimaShaderProgram* combineProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/combine.asp");
	if (!combineProgram->Link())
		return false;
	
	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	materialsManager->LoadMaterials(materialsPath);
	
	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
	_camera = _camerasManager->CreateThirdPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;
	
	//_model = _scene->GetModelsManager()->LoadModelFromExternalFile(modelPath, ANIMA_ENGINE_DEMO_MODEL_NAME);
	//if (!_model)
	//	return false;
	
	//_model->GetTransformation()->RotateXDeg(-90.0);
	//_model->GetTransformation()->RotateYDeg(180.0);
	
	_model = _scene->GetModelsManager()->CreateModel("ANIMA_ENGINE_DEMO_MODEL_NAME");
	Anima::AnimaMesh* mesh = _scene->GetMeshesManager()->CreateMesh("mesh");
	mesh->MakeCube();
	mesh->SetParentObject(_model);
	_model->AddMesh(mesh);
	
	Anima::AnimaArray<Anima::AnimaMesh*> modelMeshes;
	_model->GetAllMeshes(&modelMeshes);
	
	_pbrMaterial = materialsManager->GetMaterialFromName("pbr-material-osx");
	
	for (int i = 0; i < modelMeshes.size(); i++)
		modelMeshes[i]->SetMaterial(_pbrMaterial);
	
	////_model = _scene->GetModelsManager()->LoadModelFromExternalFile("C:/Users/Marco/Desktop/10001/exp.fbx", ANIMA_ENGINE_DEMO_MODEL_NAME);
	////
	////Anima::AnimaArray<Anima::AnimaMesh*> modelMeshes;
	////_model->GetAllMeshes(&modelMeshes);
	
	////Anima::AnimaString str;
	
	////str = "C:/Users/Marco/Desktop/10001/AnimaData/" + _model->GetName() + ".amodel";
	////_scene->GetModelsManager()->SaveModelToFile(_model, str);
	
	////for (int i = 0; i < modelMeshes.size(); i++)
	////{
	////	str = "C:/Users/Marco/Desktop/10001/AnimaData/" + modelMeshes[i]->GetName() + ".amesh";
	////	_scene->GetMeshesManager()->SaveMeshToFile(modelMeshes[i], str);
	////}
	
	//_scene->GetMeshesManager()->LoadMeshes("C:/Users/Marco/Desktop/10001/AnimaData/");
	//_scene->GetModelsManager()->LoadModels("C:/Users/Marco/Desktop/10001/AnimaData/");
	
	Anima::AnimaDirectionalLight* light = _scene->GetLightsManager()->CreateDirectionalLight("light-0");
	light->SetDirection(-1.0, -1.0, -1.0);
	light->SetColor(1.0, 1.0, 1.0);
	light->SetIntensity(0.8);
	
	//_camera->LookAt(0.0, 40.0, 40.0, 0.0, 15.0, 0.0);
	_camera->LookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0);
	_camera->Activate();
	
	_animationsManager = _scene->GetAnimationsManager();
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());
	
	_timer.Reset();
	
	Anima::AnimaTexturesManager* texturesManager = _scene->GetTexturesManager();
	
	unsigned char *dataPX = nullptr, *dataNX = nullptr, *dataPY = nullptr, *dataNY = nullptr, *dataPZ = nullptr, *dataNZ = nullptr;
	unsigned int dataSizePX, dataSizeNX, dataSizePY, dataSizeNY, dataSizePZ, dataSizeNZ;
	unsigned int width, height;
	Anima::AnimaTextureFormat format;
	
	texturesManager->GetTextureDataFromFile(dataPath + "/textures/San Francisco/negx.tga", &dataPX, dataSizePX, width, height, format);
	texturesManager->GetTextureDataFromFile(dataPath + "/textures/San Francisco/posx.tga", &dataNX, dataSizeNX, width, height, format);
	texturesManager->GetTextureDataFromFile(dataPath + "/textures/San Francisco/negy.tga", &dataPY, dataSizePY, width, height, format);
	texturesManager->GetTextureDataFromFile(dataPath + "/textures/San Francisco/posy.tga", &dataNY, dataSizeNY, width, height, format);
	texturesManager->GetTextureDataFromFile(dataPath + "/textures/San Francisco/negz.tga", &dataPZ, dataSizePZ, width, height, format);
	texturesManager->GetTextureDataFromFile(dataPath + "/textures/San Francisco/posz.tga", &dataNZ, dataSizeNZ, width, height, format);
	
	Anima::AnimaTexture* texture = texturesManager->CreateTexture("skybox-map");
	texture->SetTextureTarget(Anima::TEXTURE_CUBE);
	texture->SetData(dataPX, dataSizePX, Anima::POSITIVE_X);
	texture->SetData(dataNX, dataSizeNX, Anima::NEGATIVE_X);
	texture->SetData(dataPY, dataSizePY, Anima::POSITIVE_Y);
	texture->SetData(dataNY, dataSizeNY, Anima::NEGATIVE_Y);
	texture->SetData(dataPZ, dataSizePZ, Anima::POSITIVE_Z);
	texture->SetData(dataNZ, dataSizeNZ, Anima::NEGATIVE_Z);
	
	texture->SetInternalFormat(Anima::IF_RGB);
	texture->SetDataType(GL_UNSIGNED_BYTE);
	texture->SetWidth(width);
	texture->SetHeight(height);
	texture->SetFilter(Anima::LINEAR);
	texture->SetClamp(Anima::TO_EDGE);
	texture->SetTextureTarget(Anima::TEXTURE_CUBE);
	//texture->SetMipMapLevels(5, true);
	
	free(dataPX);
	free(dataNX);
	free(dataPY);
	free(dataNY);
	free(dataPZ);
	free(dataNZ);
	
	_renderer->SetTexture("EnvironmentMap", texture, false);
	
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
