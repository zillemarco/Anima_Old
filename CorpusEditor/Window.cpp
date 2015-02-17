#include "Window.h"
#include <stdio.h>
#include <AnimaModelsManager.h>
#include <AnimaModel.h>
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
#include <AnimaStage.h>
#include <AnimaStagesManager.h>

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

	if (renderingManager == nullptr)
	{
		int mul = 1;

		renderingManager = new Anima::AnimaRenderingManager(GetEngine()->GetSharedMemoryAllocator());
		renderingManager->InitRenderingTargets(w * mul, h * mul);
		renderingManager->InitRenderingUtilities(w * mul, h * mul);
	}

	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetDataGeneratorsManager()->UpdateValues();

	MakeCurrentContext();

	renderingManager->GetTexture("DiffuseMap")->BindAsRenderTarget();
	renderingManager->Start(GetEngine()->GetStagesManager()->GetStage("test-stage"));
	renderingManager->ForwardDrawAllModels(GetEngine()->GetStagesManager()->GetStage("test-stage"));
	renderingManager->Finish(GetEngine()->GetStagesManager()->GetStage("test-stage"));

	if (fxaa)
		renderingManager->ApplyFilter(GetEngine()->GetStagesManager()->GetStage("test-stage")->GetShadersManager()->GetProgramFromName("fxaaFilter"), renderingManager->GetTexture("DiffuseMap"), nullptr);
	else
		renderingManager->ApplyFilter(GetEngine()->GetStagesManager()->GetStage("test-stage")->GetShadersManager()->GetProgramFromName("nullFilter"), renderingManager->GetTexture("DiffuseMap"), nullptr);
	
	SwapBuffers();
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		glViewport(0, 0, w, h);
		window->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, Anima::AnimaVertex2f((float)w, (float)h), 0.1f, 1000.0f);

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

		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy * 0.1);
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx * -0.1);
	}

	wnd->_lastPTX = x;
	wnd->_lastPTY = y;
}

void Window::KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods)
{
	if (action == ANIMA_ENGINE_RELEASE)
		return;

	Window* wnd = (Window*)window;
	
	switch (key)
	{
	case ANIMA_ENGINE_KEY_LEFT:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(-0.5f);
		break;
	case ANIMA_ENGINE_KEY_RIGHT:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->RotateYDeg(0.5f);
		break;
	case ANIMA_ENGINE_KEY_UP:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(-0.5f);
		break;
	case ANIMA_ENGINE_KEY_DOWN:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->RotateXDeg(0.5f);
		break;
	case ANIMA_ENGINE_KEY_W:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_S:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Move(0.0f, -1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_D:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Move(-1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_A:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_L:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Zoom(-1.0f);
		break;
	case ANIMA_ENGINE_KEY_O:
		wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Zoom(1.0f);
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
	wnd->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->GetActiveCamera()->Zoom(y * 0.1);
}

void Window::Load()
{
	int w, h;
	this->GetWindowSize(&w, &h);

	Anima::AnimaShadersManager* mgr = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetShadersManager();

	mgr->CreateProgram("phong");
	mgr->GetProgramFromName("phong")->Create();
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-vs", ANIMA_ENGINE_SHADERS_PATH "Phong/phong.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-cs", ANIMA_ENGINE_SHADERS_PATH "Phong/phong.cs", Anima::AnimaShader::TESSELLATION_CONTROL));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-es", ANIMA_ENGINE_SHADERS_PATH "Phong/phong.es", Anima::AnimaShader::TESSELLATION_EVALUATION));
	//mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-gs", ANIMA_ENGINE_SHADERS_PATH "Phong/phong.gs", Anima::AnimaShader::GEOMETRY));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-fs", ANIMA_ENGINE_SHADERS_PATH "Phong/phong.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("phong")->Link();

	mgr->CreateProgram("forward-ambient");
	mgr->GetProgramFromName("forward-ambient")->Create();
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-vs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-ambient.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-fs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-ambient.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-ambient")->Link();

	mgr->CreateProgram("forward-directional");
	mgr->GetProgramFromName("forward-directional")->Create();
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-vs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-directional.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-fs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-directional.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-directional")->Link();

	mgr->CreateProgram("forward-point");
	mgr->GetProgramFromName("forward-point")->Create();
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-vs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-point.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-fs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-point.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-point")->Link();

	mgr->CreateProgram("forward-spot");
	mgr->GetProgramFromName("forward-spot")->Create();
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-vs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-spot.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-fs", ANIMA_ENGINE_SHADERS_PATH "Forward/forward-spot.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-spot")->Link();

	mgr->CreateProgram("nullFilter");
	mgr->GetProgramFromName("nullFilter")->Create();
	mgr->GetProgramFromName("nullFilter")->AddShader(mgr->LoadShaderFromFile("nullFilter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/nullFilter.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("nullFilter")->AddShader(mgr->LoadShaderFromFile("nullFilter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/nullFilter.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("nullFilter")->Link();

	mgr->CreateProgram("fxaaFilter");
	mgr->GetProgramFromName("fxaaFilter")->Create();
	mgr->GetProgramFromName("fxaaFilter")->AddShader(mgr->LoadShaderFromFile("fxaaFilter-vs", ANIMA_ENGINE_SHADERS_PATH "Filters/fxaaFilter.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("fxaaFilter")->AddShader(mgr->LoadShaderFromFile("fxaaFilter-fs", ANIMA_ENGINE_SHADERS_PATH "Filters/fxaaFilter.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("fxaaFilter")->Link();

	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("piano")->GetPChild(0)->GetPMesh(0)->GetMaterial()->SetTexture("DiffuseTexture", GetEngine()->GetStagesManager()->GetStage("test-stage")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks.bmp", "texture-mattoni"));
	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("piano")->GetPChild(0)->GetPMesh(0)->GetMaterial()->SetTexture("BumpTexture", GetEngine()->GetStagesManager()->GetStage("test-stage")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks_normal.bmp", "texture-mattoni-bump"));
	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("piano")->GetPChild(0)->GetPMesh(0)->GetMaterial()->SetBoolean("HasBump", true);
	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("piano")->GetPChild(0)->GetPMesh(0)->GetMaterial()->SetTexture("DisplacementTexture", GetEngine()->GetStagesManager()->GetStage("test-stage")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks_disp.bmp", "texture-mattoni-disp"));
	//GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("piano")->GetPChild(0)->GetPMesh(0)->GetMaterial()->SetFloat("DisplacementScale", 0.05f);
	//GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("piano")->GetPChild(0)->GetPMesh(0)->GetMaterial()->SetFloat("DisplacementBias", -(0.05f / 2.0f));

	Anima::AnimaLight* l0 = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetLightsManager()->CreateAmbientLight("ambient");
	l0->SetColor(0.1f, 0.1f, 0.1f);

	Anima::AnimaLight* l1 = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetLightsManager()->CreateDirectionalLight("directional");
	l1->SetColor(1.0f, 1.0f, 1.0f);
	l1->SetIntensity(0.8f);
	l1->SetDirection(-1.0f, -1.0f, -1.0f);
	
	Anima::AnimaLight* l2 = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetLightsManager()->CreatePointLight("pointLight0");
	l2->SetColor(0.0f, 0.0f, 1.0f);
	l2->SetConstantAttenuation(0.0f);
	l2->SetLinearAttenuation(0.0f);
	l2->SetExponentAttenuation(0.01f);
	l2->SetIntensity(1.0f);
	l2->SetPosition(-10.0f, 0.0f, 0.0f);

	Anima::AnimaLight* l3 = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetLightsManager()->CreatePointLight("pointLight1");
	l3->SetColor(1.0f, 0.0f, 0.0f);
	l3->SetConstantAttenuation(0.0f);
	l3->SetLinearAttenuation(0.0f);
	l3->SetExponentAttenuation(0.01f);
	l3->SetIntensity(1.0f);
	l3->SetPosition(0.0f, 0.0f, -10.0f);

	Anima::AnimaLight* l4 = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetLightsManager()->CreateSpotLight("spotLight0");
	l4->SetColor(0.0f, 1.0f, 0.0f);
	l4->SetConstantAttenuation(0.1f);
	l4->SetLinearAttenuation(0.1f);
	l4->SetExponentAttenuation(0.01f);
	l4->SetIntensity(0.5f);
	l4->SetPosition(0.0f, 0.5f, 5.0f);
	l4->SetDirection(0.0f, 0.0f, -1.0f);

	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("scimmia")->ComputeBoundingBox(true);
	Anima::AnimaVertex3f min = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("scimmia")->GetBoundingBoxMin();
	Anima::AnimaVertex3f max = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetModelsManager()->GetPModelFromName("scimmia")->GetBoundingBoxMax();

	Anima::AnimaVertex3f center((min.x + max.x) / 2.0f, (min.z + max.z) / 2.0f, (min.z + max.z) / 2.0f);
	Anima::AnimaVertex3f pos = center;
	pos.z += 5.0f;

	_tpcamera->LookAt(pos, center);
}

void Window::BindAsRenderTarget()
{
	int w, h;
	this->GetWindowSize(&w, &h);
	glViewport(0, 0, w, h);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}