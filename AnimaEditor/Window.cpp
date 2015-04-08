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
#include <AnimaFrustum.h>
#include <AnimaMappedArray.h>

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

	_fpcamera = nullptr;
	_tpcamera = nullptr;

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
		renderingManager = new Anima::AnimaRenderingManager(GetEngine()->GetGenericAllocator());
		renderingManager->InitRenderingUtilities(w * mul, h * mul);
	}
	renderingManager->InitRenderingTargets(w * mul, h * mul);
		
	//DrawCameraFrustum();
	//DrawLightFrustum();

	renderingManager->DeferredDrawAll(GetEngine()->GetScenesManager()->GetScene("test-scene"));
		
	SwapBuffers();

	_timerFPS.Update();

	if (_timer.Elapsed() >= 0.2)
	{
		//char szBuff[100];
		//sprintf(szBuff, "%f FPS", _timerFPS.GetFPS());
		//SetWindowTitle(szBuff);
		_timer.Reset();
	}
}

void Window::DrawCameraFrustum()
{
	if (_tpcamera->IsActive())
		return;

	Anima::AnimaFrustum* frustum = _tpcamera->GetFrustum();
	
	Anima::AnimaArray<Anima::AnimaVertex3f> vertici(GetEngine()->GetGenericAllocator());
	vertici.Add(frustum->GetFrustumVertex(2));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(0));
	vertici.Add(frustum->GetFrustumVertex(3));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(2));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(1.0f, 0.0f, 0.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(4));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(7));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 1.0f, 0.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(0));
	vertici.Add(frustum->GetFrustumVertex(4));
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(2));
	vertici.Add(frustum->GetFrustumVertex(0));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 0.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(3));
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(1));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 0.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(0));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(4));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(0));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 0.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(2));
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(3));
	vertici.Add(frustum->GetFrustumVertex(2));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 0.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);
}

void Window::DrawLightFrustum()
{
	if (_tpcamera->IsActive())
		return;

	Anima::AnimaFrustum* frustum = _directionalLight->GetFrustum();

	Anima::AnimaArray<Anima::AnimaVertex3f> vertici(GetEngine()->GetGenericAllocator());
	vertici.Add(frustum->GetFrustumVertex(2));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(0));
	vertici.Add(frustum->GetFrustumVertex(3));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(2));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(1.0f, 0.0f, 0.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(4));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(4));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 1.0f, 0.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(0));
	vertici.Add(frustum->GetFrustumVertex(4));
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(2));
	vertici.Add(frustum->GetFrustumVertex(0));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 1.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(3));
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(1));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 1.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(0));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(4));
	vertici.Add(frustum->GetFrustumVertex(1));
	vertici.Add(frustum->GetFrustumVertex(5));
	vertici.Add(frustum->GetFrustumVertex(0));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 1.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);

	vertici.RemoveAll();
	vertici.Add(frustum->GetFrustumVertex(6));
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(2));
	vertici.Add(frustum->GetFrustumVertex(7));
	vertici.Add(frustum->GetFrustumVertex(3));
	vertici.Add(frustum->GetFrustumVertex(2));
	renderingManager->AddPrimitive(&vertici, nullptr, Anima::AnimaColor4f(0.0f, 1.0f, 1.0f, 1.0f), Anima::AnimaMatrix(), GL_TRIANGLES);
}

void Window::FrameBufferResizeCallback(Anima::AnimaWindow* window, int w, int h)
{
	void* ctx = window->GetOpenGLContext();

	if (ctx)
	{
		glViewport(0, 0, w, h);

		Anima::AnimaLightsManager* lightsManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager();
		Anima::AnimaCamerasManager* camerasManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetCamerasManager();

		camerasManager->UpdatePerspectiveCameras(60.0f, Anima::AnimaVertex2f((float)w, (float)h), 0.1f, 5000.0f);
		lightsManager->UpdateLightsMatrix(camerasManager->GetActiveCamera());
		//lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));

		//if (((Window*)window)->_fpcamera != nullptr)
		//	((Window*)window)->_fpcamera->CalculateProjectionMatrix(60.0f, Anima::AnimaVertex2f((float)w, (float)h), 0.1f, 5000.0f);

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
		
		Anima::AnimaLightsManager* lightsManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager();
		Anima::AnimaCamerasManager* camerasManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetCamerasManager();

		Anima::AnimaCamera* activeCamera = camerasManager->GetActiveCamera();
		activeCamera->RotateXDeg(dy * 0.1);
		activeCamera->RotateYDeg(dx * 0.1);

		//lightsManager->UpdateLightsMatrix(activeCamera);
		lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	}

	wnd->_lastPTX = x;
	wnd->_lastPTY = y;
}

float amount = 0.1f;

void Window::KeyCallback(Anima::AnimaWindow* window, int key, int scancode, int action, int mods)
{
	if (action == ANIMA_ENGINE_RELEASE)
	{
		amount = 0.1f;
		return;
	}

	Window* wnd = (Window*)window;
	
	Anima::AnimaLightsManager* lightsManager = wnd->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager();
	Anima::AnimaCamerasManager* camerasManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetCamerasManager();

	Anima::AnimaCamera* camera = camerasManager->GetActiveCamera();
	Anima::AnimaLight* dirL = lightsManager->GetLightFromName("directional");

	Anima::AnimaVertex3f direction;
	
	if (dirL)
		direction = dirL->GetDirection();

	switch (key)
	{
	case ANIMA_ENGINE_KEY_W:
	case ANIMA_ENGINE_KEY_UP:
		camera->Move(camera->GetForward(), amount);
		lightsManager->UpdateLightsMatrix(camera);
		break;
	case ANIMA_ENGINE_KEY_S:
	case ANIMA_ENGINE_KEY_DOWN:
		camera->Move(camera->GetForward(), -amount);
		lightsManager->UpdateLightsMatrix(camera);
		break;
	case ANIMA_ENGINE_KEY_D:
	case ANIMA_ENGINE_KEY_RIGHT:
		camera->Move(camera->GetRight(), amount);
		lightsManager->UpdateLightsMatrix(camera);
		break;
	case ANIMA_ENGINE_KEY_A:
	case ANIMA_ENGINE_KEY_LEFT:
		camera->Move(camera->GetRight(), -amount);
		lightsManager->UpdateLightsMatrix(camera);
		break;
	//case ANIMA_ENGINE_KEY_W:
	//	camerasManager->GetCameraFromName("tp")->RotateXDeg(1.0f);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_UP:
	//	//camera->Move(camera->GetForward(), amount);
	//	//lightsManager->UpdateLightsMatrix(activeCamera);
	//	camerasManager->GetCameraFromName("tp")->Move(camerasManager->GetCameraFromName("tp")->GetForward(), amount);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_S:
	//	camerasManager->GetCameraFromName("tp")->RotateXDeg(-1.0f);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_DOWN:
	//	//camera->Move(camera->GetForward(), -amount);
	//	//lightsManager->UpdateLightsMatrix(activeCamera);
	//	camerasManager->GetCameraFromName("tp")->Move(camerasManager->GetCameraFromName("tp")->GetForward(), -amount);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_D:
	//	camerasManager->GetCameraFromName("tp")->RotateYDeg(1.0f);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_RIGHT:
	//	//camera->Move(camera->GetRight(), amount);
	//	//lightsManager->UpdateLightsMatrix(activeCamera);
	//	camerasManager->GetCameraFromName("tp")->Move(camerasManager->GetCameraFromName("tp")->GetRight(), amount);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_A:
	//	camerasManager->GetCameraFromName("tp")->RotateYDeg(-1.0f);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	//case ANIMA_ENGINE_KEY_LEFT:
	//	//camera->Move(camera->GetRight(), -amount);
	//	//lightsManager->UpdateLightsMatrix(activeCamera);
	//	camerasManager->GetCameraFromName("tp")->Move(camerasManager->GetCameraFromName("tp")->GetRight(), -amount);
	//	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
	//	break;
	case ANIMA_ENGINE_KEY_F:
		wnd->_fpcamera->Activate();
		break;
	case ANIMA_ENGINE_KEY_T:
		wnd->_tpcamera->Activate();
		break;
	case ANIMA_ENGINE_KEY_L:
		direction.x -= 0.01f;
		break;
	case ANIMA_ENGINE_KEY_J:
			direction.x += 0.01f;
		break;
	case ANIMA_ENGINE_KEY_I:
			direction.z += 0.01f;
		break;
	case ANIMA_ENGINE_KEY_K:
			direction.z -= 0.01f;
			break;
	case ANIMA_ENGINE_KEY_G:
		camerasManager->GetCameraFromName("fp")->Move(camerasManager->GetCameraFromName("fp")->GetForward(), amount);
		break;
	case ANIMA_ENGINE_KEY_B:
		camerasManager->GetCameraFromName("fp")->Move(camerasManager->GetCameraFromName("fp")->GetForward(), -amount);
		break;
	case ANIMA_ENGINE_KEY_N:
		camerasManager->GetCameraFromName("fp")->Move(camerasManager->GetCameraFromName("fp")->GetRight(), amount);
		break;
	case ANIMA_ENGINE_KEY_V:
		camerasManager->GetCameraFromName("fp")->Move(camerasManager->GetCameraFromName("fp")->GetRight(), -amount);
		break;
	}

	if (dirL)
		dirL->SetDirection(direction);
	
	amount += 0.1f;
	
	if(amount >= 5.0f)
		amount = 5.0f;
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
	Anima::AnimaLightsManager* lightsManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager();
	Anima::AnimaCamerasManager* camerasManager = window->GetEngine()->GetScenesManager()->GetScene("test-scene")->GetCamerasManager();

	Anima::AnimaCamera* activeCamera = camerasManager->GetActiveCamera();
	activeCamera->Zoom(y * 0.1f);

	//lightsManager->UpdateLightsMatrix(activeCamera);
	lightsManager->UpdateLightsMatrix(camerasManager->GetCameraFromName("tp"));
}

void Window::Load()
{
	int w, h;
	this->GetWindowSize(&w, &h);

	Anima::AnimaShadersManager* mgr = GetEngine()->GetScenesManager()->GetScene("test-scene")->GetShadersManager();

	mgr->CreateProgram("deferred-prepare");
	mgr->GetProgramFromName("deferred-prepare")->Create();
	mgr->GetProgramFromName("deferred-prepare")->AddShader(mgr->LoadShaderFromFile("deferred-prepare-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-prepare-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-prepare")->AddShader(mgr->LoadShaderFromFile("deferred-prepare-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-prepare-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-prepare")->Link();

	mgr->CreateProgram("deferred-shadowMap");
	mgr->GetProgramFromName("deferred-shadowMap")->Create();
	mgr->GetProgramFromName("deferred-shadowMap")->AddShader(mgr->LoadShaderFromFile("deferred-shadowMap-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-shadowMap-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-shadowMap")->AddShader(mgr->LoadShaderFromFile("deferred-shadowMap-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-shadowMap-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-shadowMap")->Link();
	
	mgr->CreateProgram("deferred-combine");
	mgr->GetProgramFromName("deferred-combine")->Create();
	mgr->GetProgramFromName("deferred-combine")->AddShader(mgr->LoadShaderFromFile("deferred-combine-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-combine-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("deferred-combine")->AddShader(mgr->LoadShaderFromFile("deferred-combine-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/deferred-combine-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("deferred-combine")->Link();
	
	mgr->CreateProgram("primitive-draw");
	mgr->GetProgramFromName("primitive-draw")->Create();
	mgr->GetProgramFromName("primitive-draw")->AddShader(mgr->LoadShaderFromFile("primitive-draw-vs", ANIMA_ENGINE_SHADERS_PATH "Primitive/primitive-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("primitive-draw")->AddShader(mgr->LoadShaderFromFile("primitive-draw-fs", ANIMA_ENGINE_SHADERS_PATH "Primitive/primitive-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("primitive-draw")->Link();

	mgr->CreateProgram("primitive-combine");
	mgr->GetProgramFromName("primitive-combine")->Create();
	mgr->GetProgramFromName("primitive-combine")->AddShader(mgr->LoadShaderFromFile("primitive-combine-vs", ANIMA_ENGINE_SHADERS_PATH "Primitive/combine-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("primitive-combine")->AddShader(mgr->LoadShaderFromFile("primitive-combine-fs", ANIMA_ENGINE_SHADERS_PATH "Primitive/combine-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("primitive-combine")->Link();
	
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

	mgr->CreateProgram("ssao");
	mgr->GetProgramFromName("ssao")->Create();
	mgr->GetProgramFromName("ssao")->AddShader(mgr->LoadShaderFromFile("ssao-vs", ANIMA_ENGINE_SHADERS_PATH "Deferred/ssao-vs.glsl", Anima::AnimaShader::VERTEX));
	mgr->GetProgramFromName("ssao")->AddShader(mgr->LoadShaderFromFile("ssao-fs", ANIMA_ENGINE_SHADERS_PATH "Deferred/ssao-fs.glsl", Anima::AnimaShader::FRAGMENT));
	mgr->GetProgramFromName("ssao")->Link();

	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetTexture("DiffuseTexture", GetEngine()->GetScenesManager()->GetScene("test-scene")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks.bmp", "texture-mattoni"));
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetTexture("BumpTexture", GetEngine()->GetScenesManager()->GetScene("test-scene")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks_normal_old.bmp", "texture-mattoni-bump"));
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetBoolean("HasBump", true);
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetColor("SpecularColor", 0.3f, 0.3f, 0.3f);
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetFloat("Shininess", 50.0f);
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetTexture("DisplacementTexture", GetEngine()->GetScenesManager()->GetScene("test-scene")->GetTexturesManager()->LoadTextureFromFile(ANIMA_ENGINE_TEXTURES_PATH "bricks_disp.bmp", "texture-mattoni-disp"));
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetFloat("DisplacementScale", 0.05f);
	//GetEngine()->GetScenesManager()->GetScene("test-scene")->GetModelsManager()->GetModelFromName("piano")->GetChild(0)->GetMesh(0)->GetMaterial()->SetFloat("DisplacementBias", -(0.05f / 2.0f));

	Anima::AnimaLightsManager* lightsManager = GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager();
	Anima::AnimaHemisphereLight* hemL = lightsManager->CreateLight<Anima::AnimaHemisphereLight>("hemisphere");
	hemL->SetIntensity(0.1f);
	hemL->SetSkyColor(1.0f, 1.0f, 1.0f);
	hemL->SetGroundColor(0.0f, 0.0f, 0.0f);
	hemL->SetPosition(0.0f, 2000.0f, 0.0f);

	_directionalLight = lightsManager->CreateDirectionalLight("directional");
	_directionalLight->SetColor(1.0f, 1.0f, 1.0f);
	_directionalLight->SetIntensity(1.0f);
	_directionalLight->SetDirection(-1.0f, 0.0f, -1.0f);

	//Anima::AnimaLight* l2 = GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager()->CreatePointLight("pointLight0");
	//l2->SetColor(1.0f, 1.0f, 0.78f);
	//l2->SetConstantAttenuation(1.0f);
	//l2->SetLinearAttenuation(0.001f);
	//l2->SetExponentAttenuation(0.0f);
	//l2->SetIntensity(1.0f);
	//l2->SetPosition(0.0f, 100.0f, 0.0f);
	//l2->SetRange(2000.0f);

	//Anima::AnimaLight* l3 = GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager()->CreatePointLight("pointLight1");
	//l3->SetColor(1.0f, 0.0f, 0.0f);
	//l3->SetConstantAttenuation(0.0f);
	//l3->SetLinearAttenuation(0.0f);
	//l3->SetExponentAttenuation(0.01f);
	//l3->SetIntensity(1.0f);
	//l3->SetPosition(0.0f, 5.0f, -10.0f);
	//l3->SetRange(40.0f);

	//Anima::AnimaLight* l4 = GetEngine()->GetScenesManager()->GetScene("test-scene")->GetLightsManager()->CreateSpotLight("spotLight0");
	//l4->SetColor(0.0f, 1.0f, 0.0f);
	//l4->SetConstantAttenuation(0.0f);
	//l4->SetLinearAttenuation(0.0f);
	//l4->SetExponentAttenuation(0.01f);
	//l4->SetIntensity(1.0f);
	//l4->SetPosition(0.0f, 0.5f, 5.0f);
	//l4->SetDirection(0.0f, 0.0f, -1.0f);
	//l4->SetRange(40.0f);
	//l4->SetCutoff(0.9f);
	
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