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

	GetEngine()->GetDataGeneratorsManager()->UpdateValues();

	Anima::AnimaRenderingManager::Start(GetEngine());

	Anima::AnimaRenderingManager::DrawModel(GetEngine(), GetEngine()->GetModelsManager()->GetPModel(0), GetEngine()->GetShadersManager()->GetProgram(0));

	Anima::AnimaRenderingManager::Finish(GetEngine());

	//GLenum error = glGetError();

	//int w, h;
	//GetWindowSize(&w, &h);
	//
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glGetError();
	//glClearColor(0.5, 0.5, 0.5, 1.0);
	//
	//error = glGetError();
	//
	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	//
	//error = glGetError();

	//glEnable(GL_TEXTURE_2D);

	//error = glGetError();

	//program->Use();
	//
	//error = glGetError();

	//Anima::AnimaCamera* camera = GetEngine()->GetCamerasManager()->GetActiveCamera();
	//Anima::AnimaMatrix viewMatrix = camera->GetViewMatrix();
	//Anima::AnimaMatrix modelMatrix(GetEngine());
	//modelMatrix.RotateYDeg(rotY);
	//rotY += 0.05;
	//Anima::AnimaMatrix projectionMatrix(GetEngine());
	//projectionMatrix.Perspective(60.0f, w / h, 0.01f, 1000.0f);
	//
	//Anima::AnimaMatrix modelViewMatrix = modelMatrix * viewMatrix;
	//Anima::AnimaMatrix mvpMatrix = modelViewMatrix * projectionMatrix;
	//Anima::AnimaMatrix normalMatrix = modelMatrix;

	//Anima::AnimaVertex3f lightDir(GetEngine());
	//lightDir[0] = -1.0;
	//lightDir[1] = -1.0;
	//lightDir[2] = -1.0;

	//lightDir.Normalize();

	//program->SetUniform("mvpMatrix", mvpMatrix);
	//program->SetUniform("normalMatrix", normalMatrix);
	//program->SetUniform("materialColor", 1.0, 1.0, 1.0);
	//program->SetUniform("ambientLight", 0.0, 0.0, 0.0);
	//program->SetUniform("directionalLight.base.color", 1.0, 1.0, 1.0);
	//program->SetUniformf("directionalLight.base.intensity", 1.0);
	//program->SetUniform("directionalLight.direction", lightDir);
	//program->SetUniformf("specularIntensity", 2.0);
	//program->SetUniformf("specularPower", 32.0);
	//program->SetUniform("eyePosition", camera->GetPosition());

	//error = glGetError();

	//Anima::AnimaModelsManager* mgr = GetEngine()->GetModelsManager();
	//Anima::AnimaMatrix m(GetEngine());
	//if (mgr->GetModelsNumber() > 0)
	//{
	//	error = glGetError();

	//	texture->Bind();
	//	//_program->SetUniformi("sampler", _texture->GetID());
	//	mgr->GetPModel(0)->Draw(m);

	//	error = glGetError();
	//}

	//error = glGetError();

	//glDisable(GL_TEXTURE_2D);

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
		window->GetEngine()->GetCamerasManager()->UpdatePerspectiveCameras(60.0f, w / h, 0.1, 1000.0);
		error = glGetError();

		int i = 0;
		i++;
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

	GLenum error = glGetError();
	
	program = GetEngine()->GetShadersManager()->CreateProgram();
	program->Create();
	program->AddShader(vs);
	program->AddShader(fs);
	program->Link();

	program->AddUniform("viewProjectionMatrix");
	program->AddUniform("modelMatrix");
	program->AddUniform("materialColor");
	program->AddUniform("materialDiffuseTexture");
	program->AddUniform("ambientLight");
	program->AddUniform("directionalLight.base.color");
	program->AddUniform("directionalLight.base.intensity");
	program->AddUniform("directionalLight.direction");
	program->AddUniform("specularIntensity");
	program->AddUniform("specularPower");
	program->AddUniform("eyePosition");

	texture = GetEngine()->GetTexturesManager()->LoadTextureFromFile("D:/Git/AnimaEngine/AnimaEngine/data/textures/cubo.bmp", "texture-cubo");
	texture->Load();

	//material = new Anima::AnimaMaterial(GetEngine());
	//material->AddFloat("lightIntensity", 1.0f);
	//material->AddFloat("specInt", 2.0f);
	//material->AddFloat("specPow", 32.0f);
	//material->AddTexture("diffuse", texture);
	//material->AddColor("materialColor", 1.0, 1.0, 1.0);
	//material->AddColor("lightColor", 1.0, 1.0, 1.0);
	//material->AddColor("ambientColor", 0.0, 0.0, 0.0);
}
