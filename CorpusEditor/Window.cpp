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
	projection = true;

	_lastPTX = 0.0;
	_lastPTY = 0.0;
	type = true;
}

Window::~Window()
{
}

void Window::PaintCallback(Anima::AnimaWindow* window)
{
	((Window*)window)->DrawScene();
}

void Window::DrawScene()
{
	MakeCurrentContext();

	GetEngine()->GetStagesManager()->GetStage("test-stage")->GetDataGeneratorsManager()->UpdateValues();

	renderingManager.Start(GetEngine()->GetStagesManager()->GetStage("test-stage"));
	renderingManager.ForwardDrawAllModels(GetEngine()->GetStagesManager()->GetStage("test-stage"));
	renderingManager.Finish(GetEngine()->GetStagesManager()->GetStage("test-stage"));

	SwapBuffers();
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		glViewport(0, 0, w, h);
		window->GetEngine()->GetStagesManager()->GetStage("test-stage")->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, Anima::AnimaVertex2f((float)w, (float)h), 0.1f, 1000.0f);
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
	Anima::AnimaShadersManager* mgr = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetShadersManager();

#if defined _MSC_VER
	mgr->CreateProgram("phong");
	mgr->GetProgramFromName("phong")->Create();
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-cs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.cs", Anima::AnimaShader::TESSELLATION_CONTROL));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-es", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.es", Anima::AnimaShader::TESSELLATION_EVALUATION));
	//mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-gs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.gs", Anima::AnimaShader::GEOMETRY));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("phong")->Link();

	mgr->CreateProgram("forward-ambient");
	mgr->GetProgramFromName("forward-ambient")->Create();
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-ambient")->Link();

	mgr->CreateProgram("forward-directional");
	mgr->GetProgramFromName("forward-directional")->Create();
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-directional")->Link();

	mgr->CreateProgram("forward-point");
	mgr->GetProgramFromName("forward-point")->Create();
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-point")->Link();

	mgr->CreateProgram("forward-spot");
	mgr->GetProgramFromName("forward-spot")->Create();
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-spot")->Link();

	texture = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetTexturesManager()->LoadTextureFromFile("D:/Git/AnimaEngine/AnimaEngine/data/textures/mattoni.bmp", "texture-cubo");
	texture->Load();
#else
	mgr->CreateProgram("phong");
	mgr->GetProgramFromName("phong")->Create();
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-vs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-cs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.cs", Anima::AnimaShader::TESSELLATION_CONTROL));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-es", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.es", Anima::AnimaShader::TESSELLATION_EVALUATION));
	//mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-gs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.gs", Anima::AnimaShader::GEOMETRY));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-fs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("phong")->Link();
	
	mgr->CreateProgram("forward-ambient");
	mgr->GetProgramFromName("forward-ambient")->Create();
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-vs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-fs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-ambient")->Link();
	
	mgr->CreateProgram("forward-directional");
	mgr->GetProgramFromName("forward-directional")->Create();
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-vs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-fs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-directional")->Link();
	
	mgr->CreateProgram("forward-point");
	mgr->GetProgramFromName("forward-point")->Create();
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-vs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-fs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-point")->Link();
	
	mgr->CreateProgram("forward-spot");
	mgr->GetProgramFromName("forward-spot")->Create();
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-vs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-fs", "/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("forward-spot")->Link();
	
	texture = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetTexturesManager()->LoadTextureFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/textures/mattoni.bmp", "texture-cubo");
	texture->Load();
#endif
	
	Anima::AnimaLight* l0 = GetEngine()->GetStagesManager()->GetStage("test-stage")->GetLightsManager()->CreateAmbientLight("ambient");
	l0->SetColor(0.2f, 0.2f, 0.2f);

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
	l4->SetConstantAttenuation(0.1);
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
