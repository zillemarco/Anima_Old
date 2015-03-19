#include "Window.h"
#include <stdio.h>
#include <AnimaModelsManager.h>
#include <AnimaMesh.h>
#include <AnimaMatrix.h>
#include <AnimaShadersManager.h>
#include <AnimaAllocators.h>
#include <QtGui/QMatrix4x4>
#include <AnimaCamerasManager.h>
#include <AnimaTexturesManager.h>
#include <AnimaRenderingManager.h>
#include <AnimaDataGeneratorsManager.h>
#include <AnimaLight.h>
#include <AnimaLightsManager.h>
#include <AnimaMath.h>
#include <AnimaScene.h>
#include <AnimaScenesManager.h>

#define _USE_MATH_DEFINES
#include <math.h>

bool fxaa = true;

BEGIN_MESSAGE_MAP(Window, Anima::AnimaEngineWindow_Base)
	ANIMA_WINDOW_MOUSE_CLICK_EVENT(MouseClickCallback)
	ANIMA_WINDOW_PAINT_EVENT(PaintCallback)
	ANIMA_WINDOW_FRAMEBUFFER_SIZE_CHANGED_EVENT(FrameBufferResizeCallback)
	ANIMA_WINDOW_CURSOR_MOVE_EVENT(MouseMoveCallback)
	ANIMA_WINDOW_KEY_EVENT(KeyCallback)
	ANIMA_WINDOW_MOUSE_SCROLL_EVENT(ScrollCallback)
END_MESSAGE_MAP()

Window::Window()
{
	renderingManager = nullptr;
	projection = true;

	_lastPTX = 0.0;
	_lastPTY = 0.0;
	type = true;
}

Window::~Window()
{
	delete renderingManager;
}

void Window::PaintCallback(Anima::AnimaWindow* window)
{
	((Window*)window)->DrawScene();
}

void Window::DrawScene()
{
	int w, h;
	this->GetWindowSize(&w, &h);
	int mul = (int)GetResolutionMutiplier();
	if (renderingManager == nullptr)
	{
		renderingManager = new Anima::AnimaRenderingManager(GetEngine()->GetSharedMemoryAllocator());
		renderingManager->InitRenderingUtilities(w * mul, h * mul);
	}
	renderingManager->InitRenderingTargets(w * mul, h * mul);
	
//	GetEngine()->GetScenesManager()->GetStage("test-scene")->GetLightsManager()->GetLightFromName("spotLight0")->UpdateMeshTransformation(	GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("cilindro")->GetTransformation());
	
	GetEngine()->GetScenesManager()->GetStage("test-scene")->GetDataGeneratorsManager()->UpdateValues();
	
	renderingManager->DeferredDrawAllModels(GetEngine()->GetScenesManager()->GetStage("test-scene"));
	
	SwapBuffers();
	
	_timerFPS.Update();

	if (_timer.Elapsed() >= 0.2)
	{
		char szBuff[100];
		sprintf(szBuff, "%f FPS", _timerFPS.GetFPS());
		SetWindowTitle(szBuff);
		_timer.Reset();
	}
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		glViewport(0, 0, w, h);
		window->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, Anima::AnimaVertex2f((float)w, (float)h), 0.1f, 1000.0f);

		if (((Window*)window)->renderingManager != nullptr)
		{
			int mul = 1;
			((Window*)window)->renderingManager->InitRenderingTargets(w * mul, h * mul);
		}
	}
	else
	{
#if defined _MSC_VER
		OutputDebugStringA("ctx not valid\n");
#else
		printf("ctx not valid\n");
#endif
	}
}

void Window::MouseMoveCallback(Anima::AnimaWindow* window, double x, double y)
{
	Window* wnd = (Window*)window;

	if (window->GetMouseButtons()[ANIMA_ENGINE_MOUSE_BUTTON_LEFT] == ANIMA_ENGINE_PRESS)
	{
		double dy = wnd->_lastPTY - y;
		double dx = wnd->_lastPTX - x;

		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy * 0.1);
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx * -0.1);
	}

	wnd->_lastPTX = x;
	wnd->_lastPTY = y;
}

float amount = 0.01f;

void Window::KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods)
{
	if (action == ANIMA_ENGINE_RELEASE)
	{
		amount = 0.01f;
		return;
	}

	Window* wnd = (Window*)window;
	
	switch (key)
	{
	case ANIMA_ENGINE_KEY_LEFT:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->Translate(-amount, 0.0f, 0.0f);
		break;
	case ANIMA_ENGINE_KEY_RIGHT:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->Translate(amount, 0.0f, 0.0f);
		break;
	case ANIMA_ENGINE_KEY_UP:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->Translate(0.0f, amount, 0.0f);
		break;
	case ANIMA_ENGINE_KEY_DOWN:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->Translate(0.0f, -amount, 0.0f);
		break;
	case ANIMA_ENGINE_KEY_W:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_S:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, -1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_D:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(-1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_A:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_L:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->Translate(0.0f, 0.0f, amount);
		break;
	case ANIMA_ENGINE_KEY_O:
		wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->Translate(0.0f, 0.0f, -amount);
		break;
	case ANIMA_ENGINE_KEY_C:

		if (wnd->_tpcamera->IsActive())
			wnd->_fpcamera->Activate();
		else if (wnd->_fpcamera->IsActive())
			wnd->_tpcamera->Activate();

		break;
	case ANIMA_ENGINE_KEY_T:
		wnd->type = !wnd->type;
		break;
	case ANIMA_ENGINE_KEY_F:
		fxaa = !fxaa;
		break;
	}
	
	amount += 0.01f;
	
	if(amount >= 0.3f)
		amount = 0.3f;
}

void Window::MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods)
{
	window->GetMouseButtons()[button] = action;

	if (action == ANIMA_ENGINE_PRESS)
		window->SetWindowTitle("press");
	else if (action == ANIMA_ENGINE_RELEASE)
		window->SetWindowTitle("release");

	((Window*)window)->DrawScene();
}

void Window::ScrollCallback(Anima::AnimaWindow* window, double x, double y)
{
	Window* wnd = (Window*)window;
	wnd->GetEngine()->GetScenesManager()->GetStage("test-scene")->GetCamerasManager()->GetActiveCamera()->Zoom(y * 0.1);
}

void Window::Load()
{
	int w, h;
	this->GetWindowSize(&w, &h);

	Anima::AnimaShadersManager* mgr = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetShadersManager();

	//mgr->CreateProgram("forward-ambient");
	//mgr->GetProgramFromName("forward-ambient")->Create();
	//mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-vs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-ambient-vs.glsl", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-fs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-ambient-fs.glsl", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("forward-ambient")->Link();

	//mgr->CreateProgram("forward-directional");
	//mgr->GetProgramFromName("forward-directional")->Create();
	//mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-vs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-directional-vs.glsl", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-fs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-directional-fs.glsl", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("forward-directional")->Link();

	mgr->CreateProgram("deferred-prepare");
	mgr->GetProgramFromName("deferred-prepare")->Create();
	mgr->GetProgramFromName("deferred-prepare")->AddShader(mgr->LoadShaderFromFile("deferred-prepare-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-prepare-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-prepare")->AddShader(mgr->LoadShaderFromFile("deferred-prepare-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-prepare-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-prepare")->Link();

	//mgr->CreateProgram("deferred-shadowMap");
	//mgr->GetProgramFromName("deferred-shadowMap")->Create();
	//mgr->GetProgramFromName("deferred-shadowMap")->AddShader(mgr->LoadShaderFromFile("deferred-shadowMap-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-shadowMap-vs.glsl", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("deferred-shadowMap")->AddShader(mgr->LoadShaderFromFile("deferred-shadowMap-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-shadowMap-fs.glsl", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("deferred-shadowMap")->Link();
	
	//mgr->CreateProgram("deferred-spot");
	//mgr->GetProgramFromName("deferred-spot")->Create();
	//mgr->GetProgramFromName("deferred-spot")->AddShader(mgr->LoadShaderFromFile("deferred-spot-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-spot-vs.glsl", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("deferred-spot")->AddShader(mgr->LoadShaderFromFile("deferred-spot-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-spot-fs.glsl", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("deferred-spot")->Link();

	//mgr->CreateProgram("nullFilter");
	//mgr->GetProgramFromName("nullFilter")->Create();
	//mgr->GetProgramFromName("nullFilter")->AddShader(mgr->LoadShaderFromFile("nullFilter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/nullFilter-vs.glsl", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("nullFilter")->AddShader(mgr->LoadShaderFromFile("nullFilter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/nullFilter-fs.glsl", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("nullFilter")->Link();

	mgr->CreateProgram("deferred-combine");
	mgr->GetProgramFromName("deferred-combine")->Create();
	mgr->GetProgramFromName("deferred-combine")->AddShader(mgr->LoadShaderFromFile("deferred-combine-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-combine-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-combine")->AddShader(mgr->LoadShaderFromFile("deferred-combine-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-combine-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-combine")->Link();

	mgr->CreateProgram("fxaaFilter");
	mgr->GetProgramFromName("fxaaFilter")->Create();
	mgr->GetProgramFromName("fxaaFilter")->AddShader(mgr->LoadShaderFromFile("fxaaFilter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/fxaaFilter-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("fxaaFilter")->AddShader(mgr->LoadShaderFromFile("fxaaFilter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/fxaaFilter-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("fxaaFilter")->Link();

	mgr->CreateProgram("gaussBlur7x1Filter");
	mgr->GetProgramFromName("gaussBlur7x1Filter")->Create();
	mgr->GetProgramFromName("gaussBlur7x1Filter")->AddShader(mgr->LoadShaderFromFile("gaussBlur7x1Filter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/gaussBlur7x1Filter-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("gaussBlur7x1Filter")->AddShader(mgr->LoadShaderFromFile("gaussBlur7x1Filter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/gaussBlur7x1Filter-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("gaussBlur7x1Filter")->Link();

	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetTexture("DiffuseTexture", GetEngine()->GetScenesManager()->GetStage("test-scene")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks.bmp", "texture-mattoni"));
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetTexture("BumpTexture", GetEngine()->GetScenesManager()->GetStage("test-scene")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks_normal_old.bmp", "texture-mattoni-bump"));
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetBoolean("HasBump", true);
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetColor("SpecularColor", 0.3f, 0.3f, 0.3f);
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetFloat("Shininess", 50.0f);
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetTexture("DisplacementTexture", GetEngine()->GetScenesManager()->GetStage("test-scene")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks_disp.bmp", "texture-mattoni-disp"));
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetFloat("DisplacementScale", 0.05f);
	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetFloat("DisplacementBias", -(0.05f / 2.0f));
	
	Anima::AnimaLight* l0 = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetLightsManager()->CreateAmbientLight("ambient");
	l0->SetColor(0.0f, 0.0f, 0.0f);

//	Anima::AnimaLight* l1 = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetLightsManager()->CreateDirectionalLight("directional");
//	l1->SetColor(1.0f, 1.0f, 1.0f);
//	l1->SetIntensity(1.0f);
//	l1->SetDirection(-1.0f, -1.0f, -1.0f);
	
	//Anima::AnimaLight* l2 = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetLightsManager()->CreatePointLight("pointLight0");
	//l2->SetColor(0.0f, 0.0f, 1.0f);
	//l2->SetConstantAttenuation(0.0f);
	//l2->SetLinearAttenuation(0.0f);
	//l2->SetExponentAttenuation(0.01f);
	//l2->SetIntensity(1.0f);
	//l2->SetPosition(-10.0f, 5.0f, 0.0f);
	//l2->SetRange(40.0f);

	//Anima::AnimaLight* l3 = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetLightsManager()->CreatePointLight("pointLight1");
	//l3->SetColor(1.0f, 0.0f, 0.0f);
	//l3->SetConstantAttenuation(0.0f);
	//l3->SetLinearAttenuation(0.0f);
	//l3->SetExponentAttenuation(0.01f);
	//l3->SetIntensity(1.0f);
	//l3->SetPosition(0.0f, 5.0f, -10.0f);
	//l3->SetRange(40.0f);

	Anima::AnimaLight* l4 = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetLightsManager()->CreateSpotLight("spotLight0");
	l4->SetColor(0.0f, 1.0f, 0.0f);
	l4->SetConstantAttenuation(0.0f);
	l4->SetLinearAttenuation(0.0f);
	l4->SetExponentAttenuation(0.01f);
	l4->SetIntensity(1.0f);
	l4->SetPosition(0.0f, 0.5f, 5.0f);
	l4->SetDirection(0.0f, 0.0f, -1.0f);
	l4->SetRange(40.0f);
//	l4->SetCutoff(1.0f);

	//GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->ComputeBoundingBox(true);
	//Anima::AnimaVertex3f min = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetBoundingBoxMin();
	//Anima::AnimaVertex3f max = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetBoundingBoxMax();

	//Anima::AnimaVertex3f center((min.x + max.x) / 2.0f, (min.z + max.z) / 2.0f, (min.z + max.z) / 2.0f);
	//
	//center = GetEngine()->GetScenesManager()->GetStage("test-scene")->GetModelsManager()->GetModelFromName("scimmia")->GetTransformation()->GetTransformationMatrix() * center;
	//
	//Anima::AnimaVertex3f pos = center;
	//pos.z += 5.0f;

	//_tpcamera->LookAt(pos, center);

	_timerFPS.Init();
	_timer.Reset();
}

void Window::BindAsRenderTarget()
{
	int w, h;
	this->GetWindowSize(&w, &h);
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}