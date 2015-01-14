#include "Window.h"
#include <stdio.h>
#include <AnimaModelsManager.h>
#include <AnimaModel.h>
#include <AnimaMatrix.h>
#include <AnimaShadersManager.h>
#include <AnimaAllocators.h>
#include <QtGui/QMatrix4x4>

BEGIN_MESSAGE_MAP(Window, Anima::AnimaEngineWindow_Base)
	ANIMA_WINDOW_MOUSE_CLICK_EVENT(MouseClickCallback)
	ANIMA_WINDOW_PAINT_EVENT(PaintCallback)
	ANIMA_WINDOW_FRAMEBUFFER_SIZE_CHANGED_EVENT(FrameBufferResizeCallback)
	ANIMA_WINDOW_CURSOR_MOVE_EVENT(MouseMoveCallback)
	ANIMA_WINDOW_KEY_EVENT(Key)
END_MESSAGE_MAP()

Window::Window()
{
	rotX = rotY = 0.0;
	tx = ty = tz = 0.0;
	projection = true;

	_camera = nullptr;
}

Window::~Window()
{
	delete _camera;
}

void Window::MouseClickCallback(Anima::AnimaEngineWindow_Base* window, int button, int action, int mods)
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

	Anima::AnimaMatrix camera(GetEngine());
	camera.LookAt(_camera->GetPosition(), _camera->GetForward(), _camera->GetUp());
	
	Anima::AnimaMatrix model(GetEngine());
	model.Scale(0.01, 0.01, 0.01);
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
}

void Window::Key(Anima::AnimaWindow* window, int key, int scancode, int action, int mods)
{
	Window* wnd = (Window*)window;
	
	switch (key)
	{
	case ANIMA_ENGINE_KEY_LEFT:
		//wnd->rotY -= 1;
		wnd->_camera->RotateYDeg(-1.0f);
		break;
	case ANIMA_ENGINE_KEY_RIGHT:
		//wnd->rotY += 1;
		wnd->_camera->RotateYDeg(1.0f);
		break;
	case ANIMA_ENGINE_KEY_UP:
		//wnd->rotX += 1;
		wnd->_camera->RotateXDeg(-1.0f);
		break;
	case ANIMA_ENGINE_KEY_DOWN:
		//wnd->rotX -= 1;
		wnd->_camera->RotateXDeg(1.0f);
		break;
	case ANIMA_ENGINE_KEY_W:
		//wnd->ty += 1;
		wnd->_camera->Move(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_S:
		//wnd->ty -= 1;
		wnd->_camera->Move(0.0f, -1.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_D:
		//wnd->tx += 1;
		wnd->_camera->Move(-1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_A:
		//wnd->tx -= 1;
		wnd->_camera->Move(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_L:
		//wnd->tz -= 1;
		wnd->_camera->Move(0.0f, 0.0f, -1.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_O:
		//wnd->tz += 1;
		wnd->_camera->Move(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case ANIMA_ENGINE_KEY_P:
		wnd->projection = !wnd->projection;
		break;
	}
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

	Anima::AnimaVertex3f pos(GetEngine());
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = -5;

	_camera = new Anima::AnimaFPSCamera(GetEngine());
	_camera->SetPosition(pos);
}