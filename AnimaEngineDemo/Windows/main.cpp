#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

// AnimaEngine includes
#include <AnimaGC.h>
#include <AnimaEngine.h>
#include <AnimaModelsManager.h>
#include <AnimaScenesManager.h>
#include <AnimaMaterialsManager.h>
#include <AnimaShadersManager.h>
#include <AnimaCamerasManager.h>
#include <AnimaAnimationsManager.h>
#include <AnimaModelInstancesManager.h>
#include <AnimaMaterialsManager.h>

#include <AnimaScene.h>
#include <AnimaModel.h>
#include <AnimaMesh.h>
#include <AnimaShaderProgram.h>
#include <AnimaRenderer.h>
#include <AnimaFirstPersonCamera.h>
#include <AnimaAnimation.h>
#include <AnimaTimer.h>
#include <AnimaModelInstance.h>

#include "main.h"

#define ANIMA_ENGINE_DEMO_SCENE_NAME "AnimaEngineDemoScene"
#define ANIMA_ENGINE_DEMO_CAMERA_NAME "AnimaEngineDemoCamera"
#define ANIMA_ENGINE_DEMO_MODEL_NAME "AnimaEngineDemoModel"

#define DEFERRED_SHADERS_START "D:/Git/Anima/AnimaEngine/data/shaders/Deferred/"
#define PRIMITIVE_SHADERS_START "D:/Git/Anima/AnimaEngine/data/shaders/Primitive/"
#define FILTERS_SHADERS_START "D:/Git/Anima/AnimaEngine/data/shaders/Filters/"

#define ANIMA_ENGINE_DEMO_NAME "AnimaEngineDemo"
const char* szWindowClass = ANIMA_ENGINE_DEMO_NAME;

Anima::AnimaGC* _gc = nullptr;
Anima::AnimaScene* _scene = nullptr;
Anima::AnimaCamerasManager* _camerasManager = nullptr;
Anima::AnimaCamera* _camera = nullptr;
Anima::AnimaModel* _model = nullptr;
Anima::AnimaAnimationsManager* _animationsManager = nullptr;
Anima::AnimaRenderer* _renderer = nullptr;
Anima::AnimaEngine _engine;
Anima::AnimaTimer _timer;
bool _shouldClose = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		_shouldClose = true;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);

		int nWidth = LOWORD(lParam);
		int nHeight = HIWORD(lParam);

		SetViewport(nWidth, nHeight);

		return result;
		break;
	}
	case WM_CREATE:
	{
		LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);

		_gc = Anima::AnimaGC::CreateContext((long)hWnd, Anima::AnimaGC::GetDefaultContextConfig(), Anima::AnimaGC::GetDefaultFrameBufferConfig());
		if (!_gc)
		{
			MessageBox(NULL, _T("GC creation failed!"), _T(ANIMA_ENGINE_DEMO_NAME), NULL);
			return -1;
		}

		if (!InitEngine())
			return -1;
		
		return result;
		break;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Inizializzazione della finestra
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), _T(ANIMA_ENGINE_DEMO_NAME), NULL);
		return 1;
	}

	static TCHAR szWindowClass[] = _T(ANIMA_ENGINE_DEMO_NAME);
	static TCHAR szTitle[] = _T(ANIMA_ENGINE_DEMO_NAME);
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T(ANIMA_ENGINE_DEMO_NAME), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;

	while (!_shouldClose)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UpdateFrame();
	}

	if (_renderer)
		delete _renderer;

	return (int)msg.wParam;
}

void ChangeColor(Anima::AnimaModelInstance* instance)
{
	if (instance->GetAnimaName() == "Alpha_HighJointsGeo.first-instance")
	{
		for (int i = 0; i < instance->GetMeshesCount(); i++)
		{
			instance->GetMesh(i)->GetMaterial()->SetColor("DiffuseColor", Anima::AnimaVertex4f(0.0, 1.0, 0.0, 1.0));
			instance->GetMesh(i)->GetMaterial()->SetColor("SpecularColor", Anima::AnimaVertex4f(0.0, 1.0, 0.0, 1.0));
			instance->GetMesh(i)->GetMaterial()->SetColor("AmbientColor", Anima::AnimaVertex4f(0.0, 1.0, 0.0, 1.0));
			instance->GetMesh(i)->GetMaterial()->SetColor("EmissionColor", Anima::AnimaVertex4f(0.0, 1.0, 0.0, 1.0));
		}
	}

	for (int i = 0; i < instance->GetChildrenNumber(); i++)
		ChangeColor((Anima::AnimaModelInstance*)instance->GetChild(i));
}

bool InitEngine()
{
	if (_gc)
		_gc->MakeCurrent();

	// Inizializzazione del motore
	if (!_engine.Initialize())
		return false;

	// Creazione della scena
	_scene = _engine.GetScenesManager()->CreateScene(ANIMA_ENGINE_DEMO_SCENE_NAME);
	if (!_scene)
		return false;

	// Caricamento degli shader
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	shadersManager->LoadShadersParts("D:/Git/Anima/AnimaEngine/data/shaders/Parts");

	Anima::AnimaShaderProgram* prepareProgram = shadersManager->CreateProgram("deferred-prepare");
	prepareProgram->Create();
	prepareProgram->AddShader(shadersManager->LoadShaderFromFile("deferred-prepare-vs", DEFERRED_SHADERS_START "deferred-prepare-vs.glsl", Anima::AnimaShaderType::VERTEX));
	prepareProgram->AddShader(shadersManager->LoadShaderFromFile("deferred-prepare-fs", DEFERRED_SHADERS_START "deferred-prepare-fs.glsl", Anima::AnimaShaderType::FRAGMENT));
	if (!prepareProgram->Link())
		return false;

	Anima::AnimaShaderProgram* shadowMapProgram = shadersManager->CreateProgram("deferred-shadowMap");
	shadowMapProgram->Create();
	shadowMapProgram->AddShader(shadersManager->LoadShaderFromFile("deferred-shadowMap-vs", DEFERRED_SHADERS_START "deferred-shadowMap-vs.glsl", Anima::AnimaShaderType::VERTEX));
	shadowMapProgram->AddShader(shadersManager->LoadShaderFromFile("deferred-shadowMap-fs", DEFERRED_SHADERS_START "deferred-shadowMap-fs.glsl", Anima::AnimaShaderType::FRAGMENT));
	if (!shadowMapProgram->Link())
		return false;

	Anima::AnimaShaderProgram* combineProgram = shadersManager->CreateProgram("deferred-combine");
	combineProgram->Create();
	combineProgram->AddShader(shadersManager->LoadShaderFromFile("deferred-combine-vs", DEFERRED_SHADERS_START "deferred-combine-vs.glsl", Anima::AnimaShaderType::VERTEX));
	combineProgram->AddShader(shadersManager->LoadShaderFromFile("deferred-combine-fs", DEFERRED_SHADERS_START "deferred-combine-fs.glsl", Anima::AnimaShaderType::FRAGMENT));
	if (!combineProgram->Link())
		return false;

	Anima::AnimaShaderProgram* primitiveDrawProgram = shadersManager->CreateProgram("primitive-draw");
	primitiveDrawProgram->Create();
	primitiveDrawProgram->AddShader(shadersManager->LoadShaderFromFile("primitive-draw-vs", PRIMITIVE_SHADERS_START "primitive-vs.glsl", Anima::AnimaShaderType::VERTEX));
	primitiveDrawProgram->AddShader(shadersManager->LoadShaderFromFile("primitive-draw-fs", PRIMITIVE_SHADERS_START "primitive-fs.glsl", Anima::AnimaShaderType::FRAGMENT));
	if (!primitiveDrawProgram->Link())
		return false;

	Anima::AnimaShaderProgram* primitiveCombineProgram = shadersManager->CreateProgram("primitive-combine");
	primitiveCombineProgram->Create();
	primitiveCombineProgram->AddShader(shadersManager->LoadShaderFromFile("primitive-combine-vs", PRIMITIVE_SHADERS_START "combine-vs.glsl", Anima::AnimaShaderType::VERTEX));
	primitiveCombineProgram->AddShader(shadersManager->LoadShaderFromFile("primitive-combine-fs", PRIMITIVE_SHADERS_START "combine-fs.glsl", Anima::AnimaShaderType::FRAGMENT));
	if (!primitiveCombineProgram->Link())
		return false;

	Anima::AnimaShaderProgram* fxaa = shadersManager->CreateProgram("fxaaFilter");
	fxaa->Create();
	fxaa->AddShader(shadersManager->LoadShaderFromFile("fxaaFilter-vs", FILTERS_SHADERS_START "fxaaFilter-vs.glsl", Anima::AnimaShaderType::VERTEX));
	fxaa->AddShader(shadersManager->LoadShaderFromFile("fxaaFilter-fs", FILTERS_SHADERS_START "fxaaFilter-fs.glsl", Anima::AnimaShaderType::FRAGMENT));
	if (!fxaa->Link())
		return false;

	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	materialsManager->LoadMaterials("D:/Git/Anima/AnimaEngine/data/materials");

	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
	_camera = _camerasManager->CreateFirstPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;

	//_camera->LookAt(0.0, 90.0, 200.0, 0.0, 0.0, -1.0);
	_camera->LookAt(0.0, 0.0, 20.0, 0.0, 0.0, -1.0);
	_camera->Activate();

	// Caricamento di un modello
	//_model = _scene->GetModelsManager()->LoadModel("C:/Users/Marco/Desktop/Model/Model_MR.dae", ANIMA_ENGINE_DEMO_MODEL_NAME);
	_model = _scene->GetModelsManager()->LoadModel("D:\\Git\\Anima\\AnimaEngine\\data\\models\\cubo.3ds", ANIMA_ENGINE_DEMO_MODEL_NAME);
	if (!_model)
		return false;

	((Anima::AnimaModel*)_model->GetChild(0))->GetMesh(0)->SetMaterial(materialsManager->GetMaterialFromName("default-material"));
	((Anima::AnimaModel*)_model->GetChild(0))->GetMesh(0)->AddShader(shadersManager->GetShaderFromName("mesh-default-vs"));


	// Creazione di due istanze del modello
	Anima::AnimaModelInstance* firstInstance = _scene->GetModelInstancesManager()->CreateInstance("first-instance", _model);
	Anima::AnimaModelInstance* secondInstance = _scene->GetModelInstancesManager()->CreateInstance("second-instance", _model);

	firstInstance->GetTransformation()->TranslateX(5.0f);
	secondInstance->GetTransformation()->TranslateX(-5.0f);

	ChangeColor(firstInstance);

	_animationsManager = _scene->GetAnimationsManager();
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());

	_timer.Reset();

	return true;
}

void SetViewport(int w, int h)
{
	if (_camerasManager)
	{
		Anima::AnimaVertex2f size((float)w, (float)h);
		_camerasManager->UpdatePerspectiveCameras(60.0f, size, 0.1f, 10000.0f);
	}

	if (_renderer)
	{
		_renderer->InitRenderingTargets(w, h);
		_renderer->InitRenderingUtilities(w, h);
	}
}

void UpdateFrame()
{
	if (_gc)
	{
		_gc->MakeCurrent();

		_renderer->Start(_scene);
		_renderer->DrawAll();

		_gc->SwapBuffers();
	}
}