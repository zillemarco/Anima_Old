#include "Window.h"
#include <stdio.h>
#include <AnimaModelsManager.h>
#include <AnimaModel.h>
#include <AnimaMatrix.h>
#include <AnimaShadersManager.h>
#include <AnimaAllocators.h>
#include <QtGui/QMatrix4x4>
#include <AnimaCamerasManager.h>

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

	int w, h;
	GetWindowSize(&w, &h);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGetError();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	
	glFrontFace(GL_CW);
//	glCullFace(GL_BACK);
//	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	_program->Use();
	
	Anima::AnimaMatrix projection(GetEngine());
	projection.Perspective(60.0f, w / h, 0.01f, 1000.0f);

	Anima::AnimaMatrix camera = GetEngine()->GetCamerasManager()->GetActiveCamera()->GetViewMatrix();	
	Anima::AnimaMatrix model(GetEngine());
	_program->SetUniformValue(_matrixUniform, (model * camera * projection).GetData());

	Anima::AnimaModelsManager* mgr = GetEngine()->GetModelsManager();
	Anima::AnimaMatrix m(GetEngine());
	if (mgr->GetModelsNumber() > 0)
	{
		mgr->GetPModel(0)->Draw(m);
	}

	SwapBuffers();
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		GLenum error = glGetError();
		glViewport(0, 0, w * window->GetResolutionMutiplier(), h * window->GetResolutionMutiplier());
		error = glGetError();
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
		wnd->GetEngine()->GetCamerasManager()->GetActiveCamera()->RotateYDeg(dx * 0.1);
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
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
#else
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Progetti/Repository/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
#endif

	_program = GetEngine()->GetShadersManager()->CreateProgram();
	_program->Create();
	_program->AddShader(vs);
	_program->AddShader(fs);
	_program->Link();

	_matrixUniform = glGetUniformLocation(_program->GetID(), "gWorld");
	_posAttr = glGetAttribLocation(_program->GetID(), "posAttr");
}