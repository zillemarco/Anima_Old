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

	GetEngine()->GetDataGeneratorsManager()->UpdateValues();

	renderingManager.Start(GetEngine());
	renderingManager.DrawAllModels(GetEngine());
	renderingManager.Finish(GetEngine());

	SwapBuffers();
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		glViewport(0, 0, w * window->GetResolutionMutiplier(), h * window->GetResolutionMutiplier());
		window->GetEngine()->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, Anima::AnimaVertex2f((float)w, (float)h), 0.1f, 1000.0f);
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

		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateXDeg(dy * 0.1);
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx * -0.1);
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
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateYDeg(-0.5f);
		break;
	case ANIMA_ENGINE_KEY_RIGHT:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateYDeg(0.5f);
		break;
	case ANIMA_ENGINE_KEY_UP:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateXDeg(-0.5f);
		break;
	case ANIMA_ENGINE_KEY_DOWN:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateXDeg(0.5f);
		break;
	case ANIMA_ENGINE_KEY_W:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Move(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_S:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Move(0.0f, -1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_D:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Move(-1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_A:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Move(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_L:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Zoom(-1.0f);
		break;
	case ANIMA_ENGINE_KEY_O:
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Zoom(1.0f);
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
	wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Zoom(y * 0.1);
}

void Window::Load()
{
	Anima::AnimaShadersManager* mgr = GetEngine()->GetShadersManager();

	mgr->CreateProgram("phong");
	mgr->GetProgramFromName("phong")->Create();
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.vs", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-cs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.cs", Anima::AnimaShader::TESSELLATION_CONTROL));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-es", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.es", Anima::AnimaShader::TESSELLATION_EVALUATION));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-gs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.gs", Anima::AnimaShader::GEOMETRY));
	mgr->GetProgramFromName("phong")->AddShader(mgr->LoadShaderFromFile("phong-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Phong/phong.fs", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("phong")->Link();

	//mgr->CreateProgram("forward-ambient");
	//mgr->GetProgramFromName("forward-ambient")->Create();
	//mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.vs", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("forward-ambient")->AddShader(mgr->LoadShaderFromFile("forward-ambient-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-ambient.fs", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("forward-ambient")->Link();

	//mgr->CreateProgram("forward-directional");
	//mgr->GetProgramFromName("forward-directional")->Create();
	//mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.vs", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("forward-directional")->AddShader(mgr->LoadShaderFromFile("forward-directional-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-directional.fs", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("forward-directional")->Link();

	//mgr->CreateProgram("forward-point");
	//mgr->GetProgramFromName("forward-point")->Create();
	//mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.vs", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("forward-point")->AddShader(mgr->LoadShaderFromFile("forward-point-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-point.fs", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("forward-point")->Link();

	//mgr->CreateProgram("forward-spot");
	//mgr->GetProgramFromName("forward-spot")->Create();
	//mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.vs", Anima::AnimaShader::VERTEX));
	//mgr->GetProgramFromName("forward-spot")->AddShader(mgr->LoadShaderFromFile("forward-spot-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/Forward/forward-spot.fs", Anima::AnimaShader::FRAGMENT));
	//mgr->GetProgramFromName("forward-spot")->Link();

	texture = GetEngine()->GetTexturesManager()->LoadTextureFromFile("D:/Git/AnimaEngine/AnimaEngine/data/textures/mattoni.bmp", "texture-cubo");
	texture->Load();
	
	Anima::AnimaLight* l = GetEngine()->GetLightsManager()->CreateAmbientLight("ambient");
	l->SetColor(0.2f, 0.2f, 0.2f);

	l = GetEngine()->GetLightsManager()->CreateDirectionalLight("directional");
	l->SetColor(1.0f, 1.0f, 1.0f);
	l->SetIntensity(0.8f);
	l->SetDirection(-1.0f, -1.0f, -1.0f);
	
	l = GetEngine()->GetLightsManager()->CreatePointLight("pointLight0");
	l->SetColor(0.0f, 0.0f, 1.0f);
	l->SetConstantAttenuation(0.0f);
	l->SetLinearAttenuation(0.0f);
	l->SetExponentAttenuation(0.01f);
	l->SetIntensity(1.0f);
	l->SetPosition(-10.0f, 0.0f, 0.0f);

	l = GetEngine()->GetLightsManager()->CreatePointLight("pointLight1");
	l->SetColor(1.0f, 0.0f, 0.0f);
	l->SetConstantAttenuation(0.0f);
	l->SetLinearAttenuation(0.0f);
	l->SetExponentAttenuation(0.01f);
	l->SetIntensity(1.0f);
	l->SetPosition(0.0f, 0.0f, -10.0f);

	l = GetEngine()->GetLightsManager()->CreateSpotLight("spotLight0");
	l->SetColor(0.0f, 1.0f, 0.0f);
	l->SetConstantAttenuation(0.1);
	l->SetLinearAttenuation(0.1f);
	l->SetExponentAttenuation(0.01f);
	l->SetIntensity(0.5f);
	l->SetPosition(0.0f, 3.0f, -5.0f);
	l->SetDirection(0.0f, -1.0f, 0.0f);

	GetEngine()->GetModelsManager()->GetPModelFromName("scimmia")->ComputeBoundingBox(true);
	Anima::AnimaVertex3f min = GetEngine()->GetModelsManager()->GetPModelFromName("scimmia")->GetBoundingBoxMin();
	Anima::AnimaVertex3f max = GetEngine()->GetModelsManager()->GetPModelFromName("scimmia")->GetBoundingBoxMax();

	Anima::AnimaVertex3f center((min.x + max.x) / 2.0f, (min.z + max.z) / 2.0f, (min.z + max.z) / 2.0f);
	Anima::AnimaVertex3f pos = center;
	pos.z += 5.0f;

	_tpcamera->LookAt(pos, center);
}
