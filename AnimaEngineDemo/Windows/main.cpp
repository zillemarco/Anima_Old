#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <Windowsx.h>

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
#include <AnimaMeshesManager.h>

#include <AnimaScene.h>
#include <AnimaModel.h>
#include <AnimaMesh.h>
#include <AnimaShaderProgram.h>
#include <AnimaRenderer.h>
#include <AnimaFirstPersonCamera.h>
#include <AnimaAnimation.h>
#include <AnimaTimer.h>
#include <AnimaModelInstance.h>
#include <AnimaMaterial.h>

#include "main.h"

#define ANIMA_ENGINE_DEMO_SCENE_NAME "AnimaEngineDemoScene"
#define ANIMA_ENGINE_DEMO_CAMERA_NAME "AnimaEngineDemoCamera"
#define ANIMA_ENGINE_DEMO_MODEL_NAME "AnimaEngineDemoModel"

#define SHADERS_PATH			"D:/Git/Anima/AnimaEngine/data/shaders/"
#define DEFERRED_SHADERS_START	"Deferred/"
#define PRIMITIVE_SHADERS_START "Primitive/"
#define FILTERS_SHADERS_START	"Filters/"

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
Anima::AnimaTimer _fpsTimer;
bool _shouldClose = false;

int lastXPos = 0;
int lastYPos = 0;

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
	case WM_MOUSEMOVE:
	{
		LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);

		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		int xDelta = lastXPos - xPos;
		int yDelta = lastYPos - yPos;

		if (_camera != nullptr)
		{
			if (wParam == MK_MBUTTON)
			{
				_camera->RotateXDeg((float)yDelta);
				_camera->RotateYDeg((float)xDelta);
			}
			else if (wParam == MK_LBUTTON)
			{
				_camera->Move(1.0, 0.0, 0.0, ((float)xDelta) / 100.0f);
				_camera->Move(0.0, 1.0, 0.0, ((float)-yDelta) / 100.0f);
			}
			else if (wParam == MK_RBUTTON)
			{
				_camera->Zoom((float)xDelta);
			}
		}

		lastXPos = xPos;
		lastYPos = yPos;

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
	RedirectIOToConsole();

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
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 720, NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T(ANIMA_ENGINE_DEMO_NAME), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	MSG msg;
	_fpsTimer.Reset();
	int FPS = 0;
	double elapsed = 0.0;

	_fpsTimer.Reset();
	while (!_shouldClose)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UpdateFrame();
		
		FPS++;

		elapsed += _fpsTimer.Elapsed();
		if (elapsed >= 100.0)
		{
			printf("FPS: %d\n", FPS);
			FPS = 0;
			elapsed = 0.0;
			_fpsTimer.Reset();
		}
	}

	if (_renderer)
		delete _renderer;

	return (int)msg.wParam;
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

	Anima::AnimaString shadersPartsPath = "D:/Git/Anima/AnimaEngine/data/shaders/Parts";
	Anima::AnimaString shadersPath = SHADERS_PATH;
	Anima::AnimaString materialsPath = "D:/Git/Anima/AnimaEngine/data/materials";
	Anima::AnimaString modelPath = "D:/Git/Anima/AnimaEngine/data/models/MatTester.obj";

#if !defined _DEBUG
	Anima::AnimaString inputString;
	while (inputString.empty() || (inputString != "y" && inputString != "n"))
	{
		std::cout << "Use defaults? [y/n]: ";
		std::cin >> inputString;
	}

	if (inputString == "n")
	{
		inputString = "";
		while (inputString.empty() || (inputString != "y" && inputString != "n"))
		{
			std::cout << "Use locals? [y/n]: ";
			std::cin >> inputString;
		}
		
		if (inputString == "n")
		{
			std::cout << "\nInsert shaders parts path: ";
			std::cin >> inputString;
			if (!inputString.empty())
				shadersPartsPath = inputString;

			std::cout << "\nInsert shaders path: ";
			std::cin >> inputString;
			if (!inputString.empty())
				shadersPath = inputString;

			std::cout << "\nInsert materials path: ";
			std::cin >> inputString;
			if (!inputString.empty())
				materialsPath = inputString;

			std::cout << "\nInsert model path: ";
			std::cin >> inputString;
			if (!inputString.empty())
				modelPath = inputString;
		}
		else
		{
			shadersPartsPath = "data/shaders/Parts";
			shadersPath = "data/shaders/";
			materialsPath = "data/materials";
			modelPath = "data/models/cubo.3ds";
		}
	}
#endif

	// Caricamento degli shader
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	shadersManager->LoadShadersParts(shadersPartsPath);

	Anima::AnimaShaderProgram* prepareProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/static-mesh-base-material.asp");
	if (!prepareProgram->Link())
		return false;

	Anima::AnimaShaderProgram* directionalLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/directional-light.asp");
	if (!directionalLightProgram->Link())
		return false;

	Anima::AnimaShaderProgram* hemisphereLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/hemisphere-light.asp");
	if (!hemisphereLightProgram->Link())
		return false;

	Anima::AnimaShaderProgram* pointLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/point-light.asp");
	if (!pointLightProgram->Link())
		return false;

	Anima::AnimaShaderProgram* spotLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/spot-light.asp");
	if (!spotLightProgram->Link())
		return false;

	Anima::AnimaShaderProgram* combineProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/combine.asp");
	if (!combineProgram->Link())
		return false;

	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	materialsManager->LoadMaterials(materialsPath);

	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
	_camera = _camerasManager->CreateThirdPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;
	
	// Caricamento di un modello

	Anima::AnimaTimer timer;
	timer.Reset();

	//_model = _scene->GetModelsManager()->LoadModelFromExternalFile(modelPath, ANIMA_ENGINE_DEMO_MODEL_NAME);
	//if (!_model)
	//	return false;
	//
	//Anima::AnimaArray<Anima::AnimaMesh*> modelMeshes;
	//_model->GetAllMeshes(&modelMeshes);

	//_scene->GetMeshesManager()->SaveMeshToFile(modelMeshes[0], "D:/Progetti/Anima/Build/bin/Debug/mesh.amesh");
	//_scene->GetModelsManager()->SaveModelToFile(_model, "D:/Progetti/Anima/Build/bin/Debug/model.amodel");

	_scene->GetMeshesManager()->LoadMeshFromFile("D:/Progetti/Anima/Build/bin/Debug/mesh.amesh");
	_model = _scene->GetModelsManager()->LoadModelFromFile("D:/Progetti/Anima/Build/bin/Debug/model.amodel");

	printf("Custom loader time: %f\n", timer.Elapsed());

	//_model->GetTransformation()->RotateXDeg(-90.0);
	
	_model->ComputeBoundingBox();

	_camera->LookAt(0.0, 5.0, 10.0, 0.0, 1.0, 0.0);
	_camera->Activate();

	//Anima::AnimaMesh* laodedMesh = _scene->GetMeshesManager()->LoadMeshFromFile("D:/Progetti/Anima/Build/bin/Debug/mesh.amesh");

	//Anima::AnimaModelInstancesManager* modelInstancesManager = _scene->GetModelInstancesManager();

	//for (int i = -2; i < 4; i++)
	//{
	//	for (int j = -2; j < 4; j++)
	//	{
	//		Anima::AnimaString str = Anima::FormatString("%d-%d", i, j);
	//		Anima::AnimaModelInstance* instance = modelInstancesManager->CreateInstance(str, _model);

	//		instance->GetTransformation()->RotateXDeg(-90);
	//		instance->GetTransformation()->TranslateX((float)i * 4);
	//		instance->GetTransformation()->TranslateZ((float)j * 4);
	//	}
	//}

	//modelMeshes[0]->SetMaterial(materialsManager->GetMaterialFromName("material-1"));
	//modelMeshes[1]->SetMaterial(materialsManager->GetMaterialFromName("material-2"));
	//modelMeshes[2]->SetMaterial(materialsManager->GetMaterialFromName("material-3"));

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
		_camerasManager->UpdatePerspectiveCameras(45.0f, size, 0.1f, 10000.0f);
	}

	if (_renderer)
	{
		std::cout << "Resize: " << w << " x " << h << std::endl;
		_renderer->InitRenderingTargets(w, h);
		_renderer->InitRenderingUtilities(w, h);
	}
}

void UpdateFrame()
{
	if (_gc)
	{
		_gc->MakeCurrent();

		Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
		Anima::AnimaMaterial* defaultMtl = materialsManager->GetDefaultMaterial();
		bool b1 = defaultMtl->HasColor("DiffuseColor");

		_renderer->Start(_scene);
		_renderer->Render();

		_gc->SwapBuffers();
	}
}

#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

// maximum mumber of lines the output console should have

static const WORD MAX_CONSOLE_LINES = 500;

void RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;

	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

	coninfo.dwSize.Y = MAX_CONSOLE_LINES;

	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog 
	// point to console as well
	ios::sync_with_stdio();
}

#endif