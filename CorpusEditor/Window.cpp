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
END_MESSAGE_MAP()

Window::Window()
{
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
	matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//matrix.translate(0, 0, -2);

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
	}
}

void Window::MouseMoveCallback(Anima::AnimaWindow* window, double x, double y)
{
}

void Window::Load()
{
	GLenum error = glGetError();
	Anima::AnimaShader* vs = GetEngine()->GetShadersManager()->LoadShaderFromFile("C:/Users/Marco/Desktop/ogldev-source/tutorial10/shader.vs", Anima::AnimaShader::VERTEX);
	error = glGetError();
	Anima::AnimaShader* fs = GetEngine()->GetShadersManager()->LoadShaderFromFile("C:/Users/Marco/Desktop/ogldev-source/tutorial10/shader.fs", Anima::AnimaShader::FRAGMENT);
	error = glGetError();

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