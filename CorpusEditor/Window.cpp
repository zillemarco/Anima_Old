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
	rotX = rotY = 0.0;
	tx = ty = tz = 0.0;
	projection = true;

	_lastPTX = 0.0;
	_lastPTY = 0.0;
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

	//spotL0->SetPosition(GetEngine()->GetCamerasManager()->GetActiveCamera()->GetPosition());
	//spotL0->SetDirection(-GetEngine()->GetCamerasManager()->GetActiveCamera()->GetForward());
	
	GetEngine()->GetDataGeneratorsManager()->UpdateValues();

	Anima::AnimaRenderingManager::Start(GetEngine());

	for (int i = 0; i < GetEngine()->GetModelsManager()->GetModelsNumber(); i++)
	{
		Anima::AnimaModel* pModel = GetEngine()->GetModelsManager()->GetPModel(i);
		Anima::AnimaRenderingManager::DrawModel(GetEngine(), pModel, GetEngine()->GetShadersManager()->GetProgramFromName("basic"));
	}
	Anima::AnimaRenderingManager::Finish(GetEngine());

	SwapBuffers();
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		glViewport(0, 0, w * window->GetResolutionMutiplier(), h * window->GetResolutionMutiplier());
		window->GetEngine()->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, w / h, 0.1f, 1000.0f);
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
	}
}

void Window::MouseClickCallback(Anima::AnimaWindow* window, int button, int action, int mods)
{
	window->GetMouseButtons()[button] = action;

	if (action == ANIMA_ENGINE_PRESS)
		window->SetWindowTitle("press");
	else if (action == ANIMA_ENGINE_RELEASE)
		window->SetWindowTitle("release");
}

void Window::ScrollCallback(Anima::AnimaWindow* window, double x, double y)
{
	Window* wnd = (Window*)window;
	wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->Zoom(y * 0.1);
}

void Window::Load()
{
#if defined _MSC_VER
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("basic-vs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("basic-fs", "D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
#else
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
#endif

	GLenum error = glGetError();
	
	program = GetEngine()->GetShadersManager()->CreateProgram("basic");
	program->Create();
	program->AddShader(vs);
	program->AddShader(fs);
	program->Link();

	texture = GetEngine()->GetTexturesManager()->LoadTextureFromFile("D:/Git/AnimaEngine/AnimaEngine/data/textures/mattoni.bmp", "texture-cubo");
	texture->Load();
	
	Anima::AnimaAmbientLight* ambL = GetEngine()->GetLightsManager()->CreateAmbientLight("ambient");
	ambL->SetColor(0.2, 0.2, 0.2);

	Anima::AnimaDirectionalLight* dirL = GetEngine()->GetLightsManager()->CreateDirectionalLight("directional");
	dirL->SetColor(0.2f, 0.0f, 0.0f);
	dirL->SetIntensity(1.0f);
	dirL->SetDirection(1.0f, 1.0f, 1.0f);
	
	//pointL0 = GetEngine()->GetLightsManager()->CreatePointLight("pointLight0");
	//pointL0->SetColor(0.0f, 0.0f, 1.0f);
	//pointL0->SetConstantAttenuation(0.0f);
	//pointL0->SetLinearAttenuation(0.0f);
	//pointL0->SetExponentAttenuation(0.01f);
	//pointL0->SetIntensity(1.0f);
	//pointL0->SetPosition(-10.0f, 0.0f, 0.0f);

	//pointL1 = GetEngine()->GetLightsManager()->CreatePointLight("pointLight1");
	//pointL1->SetColor(1.0f, 0.0f, 0.0f);
	//pointL1->SetConstantAttenuation(0.0f);
	//pointL1->SetLinearAttenuation(0.0f);
	//pointL1->SetExponentAttenuation(0.01f);
	//pointL1->SetIntensity(1.0f);
	//pointL1->SetPosition(0.0f, 0.0f, -10.0f);

	spotL0 = GetEngine()->GetLightsManager()->CreateSpotLight("spotLight0");
	spotL0->SetColor(0.0f, 1.0f, 0.0f);
	spotL0->SetConstantAttenuation(0.1);
	spotL0->SetLinearAttenuation(0.1f);
	spotL0->SetExponentAttenuation(0.01f);
	spotL0->SetIntensity(0.5f);
	spotL0->SetPosition(0.0f, 3.0f, -5.0f);
	spotL0->SetDirection(0.0f, -1.0f, 0.0f);

	GetEngine()->GetModelsManager()->GetPModelFromName("scimmia")->GetTransformation()->TranslateY(1.2f);
}
