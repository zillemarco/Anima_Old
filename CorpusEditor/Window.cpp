#include "Window.h"
#include <stdio.h>
#include <AnimaModelsManager.h>
#include <AnimaModel.h>
#include <AnimaMatrix.h>
#include <AnimaShadersManager.h>
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
}

Window::~Window()
{
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
	GLenum error = glGetError();
	MakeCurrentContext();

	int w, h;
	GetWindowSize(&w, &h);

	glViewport(0, 0, w, h);

	error = glGetError();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	error = glGetError();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	error = glGetError();
	
	glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	error = glGetError();
	_program->Use();
	error = glGetError();

	QMatrix4x4 matrix;
	matrix.perspective(60.0f, w / h, 0.01f, 1000.0f);
	matrix.translate(tx, ty, tz);
	matrix.rotate(rotY, QVector3D(0.0, 1.0, 0.0));
	matrix.rotate(rotX, QVector3D(1.0, 0.0, 0.0));
	matrix.scale(0.5, 0.5, 0.5);

	_program->SetUniformValue(_matrixUniform, matrix.data());
	error = glGetError();

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
		glViewport(0, 0, w, h);
		error = glGetError();
		int i = 0;
	}
}

void Window::MouseMoveCallback(Anima::AnimaWindow* window, double x, double y)
{
}

void Window::Key(Anima::AnimaWindow* window, int key, int scancode, int action, int mods)
{
	Window* wnd = (Window*)window;
	
	switch (key) {
  case ANIMA_ENGINE_KEY_LEFT:
			wnd->rotY -= 1;
			break;
  case ANIMA_ENGINE_KEY_RIGHT:
			wnd->rotY += 1;
			break;
  case ANIMA_ENGINE_KEY_UP:
			wnd->rotX += 1;
			break;
  case ANIMA_ENGINE_KEY_DOWN:
			wnd->rotX -= 1;
			break;
  case ANIMA_ENGINE_KEY_W:
		  wnd->ty += 1;
		  break;
  case ANIMA_ENGINE_KEY_S:
	  wnd->ty -= 1;
	  break;
  case ANIMA_ENGINE_KEY_D:
	  wnd->tx += 1;
	  break;
  case ANIMA_ENGINE_KEY_A:
	  wnd->tx -= 1;
	  break;
  case ANIMA_ENGINE_KEY_PAGE_DOWN:
	  wnd->tz -= 1;
	  break;
  case ANIMA_ENGINE_KEY_PAGE_UP:
	  wnd->tz += 1;
	  break;
			
  default:
			break;
	}
}

void Window::Load()
{
	GLenum error = glGetError();
#if defined _MSC_VER
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.vs", Anima::AnimaShader::VERTEX);
	error = glGetError();
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("D:/Git/AnimaEngine/AnimaEngine/data/shaders/test/shader.fs", Anima::AnimaShader::FRAGMENT);
	error = glGetError();
#else
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Shaders/shader.vs", Anima::AnimaShader::VERTEX);
	error = glGetError();
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("/Users/marco/Documents/Shaders/shader.fs", Anima::AnimaShader::FRAGMENT);
	error = glGetError();
#endif

	_program = GetEngine()->GetShadersManager()->CreateProgram();
	error = glGetError();
	_program->Create();
	error = glGetError();
	_program->AddShader(vs);
	error = glGetError();
	_program->AddShader(fs);
	error = glGetError();
	_program->Link();
	error = glGetError();

	_matrixUniform = glGetUniformLocation(_program->GetID(), "gWorld");
	error = glGetError();
	_posAttr = glGetAttribLocation(_program->GetID(), "posAttr");
	error = glGetError();
}