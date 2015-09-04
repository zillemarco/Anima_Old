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
#include <AnimaLightsManager.h>
#include <AnimaTexturesManager.h>

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
#include <AnimaLight.h>
#include <AnimaTexture.h>

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
Anima::AnimaModel* _floorModel = nullptr;
Anima::AnimaAnimationsManager* _animationsManager = nullptr;
Anima::AnimaRenderer* _renderer = nullptr;
Anima::AnimaEngine _engine;
Anima::AnimaTimer _timer;
Anima::AnimaTimer _fpsTimer;
Anima::AnimaMaterial* _pbrMaterial;
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
				_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
			}
			else if (wParam == MK_LBUTTON)
			{
				_camera->Move(1.0, 0.0, 0.0, ((float)xDelta) / 100.0f);
				_camera->Move(0.0, 1.0, 0.0, ((float)-yDelta) / 100.0f);
				_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
			}
			else if (wParam == MK_RBUTTON)
			{
				_camera->Zoom((float)xDelta);
				_scene->GetLightsManager()->UpdateLightsMatrix(_camera);
			}
		}

		lastXPos = xPos;
		lastYPos = yPos;

		return result;
		break;
	}
	case WM_CHAR:
	{
		if (_pbrMaterial != nullptr)
		{
			float inc = 0.02f;
			switch (wParam)
			{
			case 'p':
			case 'P':
			{
				_pbrMaterial->SetFloat("Metallic", 0.0f);
				break;
			}
			case 'm':
			case 'M':
			{
				_pbrMaterial->SetFloat("Metallic", 1.0f);
				break;
			}
			case '+':
			{
				float val = _pbrMaterial->GetFloat("Roughness");
				val = min(1.0f, val + inc);
				_pbrMaterial->SetFloat("Roughness", val);
				break;
			}
			case '-':
			{
				float val = _pbrMaterial->GetFloat("Roughness");
				val = max(0.0f, val - inc);
				_pbrMaterial->SetFloat("Roughness", val);
				break;
			}
			case '*':
			{
				float val = _pbrMaterial->GetFloat("ReflectionIntensity");
				val = min(1.0f, val + inc);
				_pbrMaterial->SetFloat("ReflectionIntensity", val);
				break;
			}
			case '/':
			{
				float val = _pbrMaterial->GetFloat("ReflectionIntensity");
				val = max(0.0f, val - inc);
				_pbrMaterial->SetFloat("ReflectionIntensity", val);
				break;
			}
			}
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	case WM_CREATE:
	{
		LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);

		Anima::AnimaGCContextConfig context = Anima::AnimaGC::GetDefaultContextConfig();
		context._major = 3;
		context._minor = 3;
		context._debug = false;

		_gc = Anima::AnimaGC::CreateContext((long)hWnd, context, Anima::AnimaGC::GetDefaultFrameBufferConfig(), false);
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
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, hInstance, NULL);
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

	// Creazione del renderer
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());

	// Creazione della scena
	_scene = _engine.GetScenesManager()->CreateScene(ANIMA_ENGINE_DEMO_SCENE_NAME);
	if (!_scene)
		return false;

	Anima::AnimaString dataPath = "D:/Git/Anima/AnimaEngine/data";
	Anima::AnimaString shadersPartsPath = "D:/Git/Anima/AnimaEngine/data/shaders/Parts";
	Anima::AnimaString shadersIncludesPath = "D:/Git/Anima/AnimaEngine/data/shaders/Includes";
	Anima::AnimaString shadersPath = SHADERS_PATH;
	Anima::AnimaString materialsPath = "D:/Git/Anima/AnimaEngine/data/materials";
	//Anima::AnimaString modelPath = "D:/Git/Anima/AnimaEngine/data/models/material2.3ds";
	Anima::AnimaString modelPath = "D:/Git/Anima/AnimaEngine/data/models/matTester.obj";

	int numInstances = 20;
	Anima::AnimaString materialName = "legno";
	
	// Caricamento degli shader
	if (!_renderer->InitializeShaders(shadersPath, shadersPartsPath, shadersIncludesPath))
		return false;
			
	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	Anima::AnimaModelInstancesManager* modelInstancesManager = _scene->GetModelInstancesManager();
	materialsManager->LoadMaterials(materialsPath);

	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
	_camera = _camerasManager->CreateThirdPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;
	
	_model = _scene->GetModelsManager()->LoadModelFromExternalFile(modelPath, ANIMA_ENGINE_DEMO_MODEL_NAME);
	if (!_model)
		return false;
		
	_floorModel = _scene->GetModelsManager()->CreateModel("floorModel");
	Anima::AnimaMesh* floorModelMesh = _scene->GetMeshesManager()->CreateMesh("floorModelMesh");
	floorModelMesh->MakePlane();
	floorModelMesh->SetParentObject(_floorModel);
	_floorModel->AddMesh(floorModelMesh);

	Anima::AnimaModelInstance* floorModelInstance = modelInstancesManager->CreateInstance("floorModelInstance", _floorModel);
	
	float degOffset = 0.0f;
	float span = M_PI * 2.0 / numInstances;
	float raggio = 50.0;
	//float scale = 5.0;
	//float scale = 10.0;
	float scale = 1.0;

	for (int i = 0; i < numInstances; i++)
	{
		Anima::AnimaString name = Anima::FormatString("modelInstance-%d", i);
		Anima::AnimaModelInstance* modelInstance = modelInstancesManager->CreateInstance(name, _model);
		Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstanceMeshes;

		modelInstance->GetAllMeshes(&modelInstanceMeshes);

		for (int j = 0; j < modelInstanceMeshes.size(); j++)
			modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName(materialName));

		if (numInstances > 1)
			modelInstance->GetTransformation()->SetTranslation(cos(degOffset) * raggio, 0, sin(degOffset) * raggio);

		modelInstance->GetTransformation()->RotateYDeg(180.0);
		//modelInstance->GetTransformation()->RotateXDeg(-90.0);
		modelInstance->GetTransformation()->SetScale(scale, scale, scale);

		degOffset += span;
	}

	//Anima::AnimaModelInstance* modelInstance1 = modelInstancesManager->CreateInstance("modelInstance-1", _model);
	//Anima::AnimaModelInstance* modelInstance2 = modelInstancesManager->CreateInstance("modelInstance-2", _model);
	//Anima::AnimaModelInstance* modelInstance3 = modelInstancesManager->CreateInstance("modelInstance-3", _model);
	//Anima::AnimaModelInstance* modelInstance4 = modelInstancesManager->CreateInstance("modelInstance-4", _model);
	//
	//Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance1Meshes;
	//Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance2Meshes;
	//Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance3Meshes;
	//Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstance4Meshes;
	//
	//modelInstance1->GetAllMeshes(&modelInstance1Meshes);
	//modelInstance2->GetAllMeshes(&modelInstance2Meshes);
	//modelInstance3->GetAllMeshes(&modelInstance3Meshes);
	//modelInstance4->GetAllMeshes(&modelInstance4Meshes);
	//
	//for (int i = 0; i < modelInstance1Meshes.size(); i++)
	//	modelInstance1Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model1-material"));

	//for (int i = 0; i < modelInstance2Meshes.size(); i++)
	//	modelInstance2Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model2-material"));

	//for (int i = 0; i < modelInstance3Meshes.size(); i++)
	//	modelInstance3Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model3-material"));

	//for (int i = 0; i < modelInstance4Meshes.size(); i++)
	//	modelInstance4Meshes[i]->SetMaterial(materialsManager->GetMaterialFromName("model4-material"));

	//modelInstance1->GetTransformation()->SetTranslation(-40, 0, -40);
	//modelInstance2->GetTransformation()->SetTranslation(40, 0, -40);
	//modelInstance3->GetTransformation()->SetTranslation(40, 0, 40);
	//modelInstance4->GetTransformation()->SetTranslation(-40, 0, 40);

	//modelInstance1->GetTransformation()->RotateYDeg(180.0);
	//modelInstance2->GetTransformation()->RotateYDeg(180.0);
	//modelInstance3->GetTransformation()->RotateYDeg(180.0);
	//modelInstance4->GetTransformation()->RotateYDeg(180.0);

	////modelInstance1->GetTransformation()->RotateXDeg(-90.0);
	////modelInstance2->GetTransformation()->RotateXDeg(-90.0);
	////modelInstance3->GetTransformation()->RotateXDeg(-90.0);
	////modelInstance4->GetTransformation()->RotateXDeg(-90.0);
	////modelInstance1->GetTransformation()->SetScale(5.0f, 5.0f, 5.0f);
	////modelInstance2->GetTransformation()->SetScale(5.0f, 5.0f, 5.0f);
	////modelInstance3->GetTransformation()->SetScale(5.0f, 5.0f, 5.0f);
	////modelInstance4->GetTransformation()->SetScale(5.0f, 5.0f, 5.0f);

	floorModelMesh->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));
	floorModelInstance->GetTransformation()->SetScale(200, 0, 200);

	_pbrMaterial = materialsManager->GetMaterialFromName(materialName);
		
	_camera->LookAt(0.0, 40.0, 100.0, 0.0, 15.0, 0.0);
	//_camera->LookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0);
	_camera->Activate();

	Anima::AnimaDirectionalLight* directionalLight = _scene->GetLightsManager()->CreateDirectionalLight("light-0");
	directionalLight->SetDirection(-1.0, -1.0, -1.0);
	directionalLight->SetColor(1.0, 1.0, 1.0);
	directionalLight->SetIntensity(1.0);

	//Anima::AnimaDirectionalLight* directionalLight2 = _scene->GetLightsManager()->CreateDirectionalLight("light-01");
	//directionalLight->SetDirection(1.0, -1.0, 1.0);
	//directionalLight->SetColor(1.0, 1.0, 1.0);
	//directionalLight->SetIntensity(1.0);

	//Anima::AnimaPointLight* pointLight1 = _scene->GetLightsManager()->CreatePointLight("light-1");
	//pointLight1->SetPosition(-40.0, 20.0, 0.0);
	//pointLight1->SetColor(1.0, 0.0, 0.0);
	//pointLight1->SetIntensity(1.0);
	//pointLight1->SetConstantAttenuation(1.0);
	//pointLight1->SetLinearAttenuation(0.0);
	//pointLight1->SetExponentAttenuation(0.0);
	//pointLight1->SetRange(200);

	//Anima::AnimaPointLight* pointLight2 = _scene->GetLightsManager()->CreatePointLight("light-2");
	//pointLight2->SetPosition(0.0, 20.0, 40.0);
	//pointLight2->SetColor(0.0, 1.0, 0.0);
	//pointLight2->SetIntensity(1.0);
	//pointLight2->SetConstantAttenuation(1.0);
	//pointLight2->SetLinearAttenuation(0.0);
	//pointLight2->SetExponentAttenuation(0.0);
	//pointLight2->SetRange(200);

	//Anima::AnimaPointLight* pointLight3 = _scene->GetLightsManager()->CreatePointLight("light-3");
	//pointLight3->SetPosition(40.0, 20.0, 0.0);
	//pointLight3->SetColor(1.0, 0.0, 1.0);
	//pointLight3->SetIntensity(1.0);
	//pointLight3->SetConstantAttenuation(1.0);
	//pointLight3->SetLinearAttenuation(0.0);
	//pointLight3->SetExponentAttenuation(0.0);
	//pointLight3->SetRange(200);

	//Anima::AnimaPointLight* pointLight4 = _scene->GetLightsManager()->CreatePointLight("light-4");
	//pointLight4->SetPosition(0.0, 20.0, -40.0);
	//pointLight4->SetColor(1.0, 0.0, 1.0);
	//pointLight4->SetIntensity(1.0);
	//pointLight4->SetConstantAttenuation(1.0);
	//pointLight4->SetLinearAttenuation(0.0);
	//pointLight4->SetExponentAttenuation(0.0);
	//pointLight4->SetRange(200);

	//Anima::AnimaPointLight* pointLight5 = _scene->GetLightsManager()->CreatePointLight("light-5");
	//pointLight5->SetPosition(-40.0, 20.0, -40.0);
	//pointLight5->SetColor(1.0, 0.5, 1.0);
	//pointLight5->SetIntensity(1.0);
	//pointLight5->SetConstantAttenuation(1.0);
	//pointLight5->SetLinearAttenuation(0.0);
	//pointLight5->SetExponentAttenuation(0.0);
	//pointLight5->SetRange(200);
	//
	//Anima::AnimaPointLight* pointLight6 = _scene->GetLightsManager()->CreatePointLight("light-6");
	//pointLight6->SetPosition(40.0, 20.0, -40.0);
	//pointLight6->SetColor(1.0, 0.0, 0.5);
	//pointLight6->SetIntensity(1.0);
	//pointLight6->SetConstantAttenuation(1.0);
	//pointLight6->SetLinearAttenuation(0.0);
	//pointLight6->SetExponentAttenuation(0.0);
	//pointLight6->SetRange(200);
	//
	//Anima::AnimaPointLight* pointLight7 = _scene->GetLightsManager()->CreatePointLight("light-7");
	//pointLight7->SetPosition(40.0, 20.0, 40.0);
	//pointLight7->SetColor(1.0, 1.0, .0);
	//pointLight7->SetIntensity(1.0);
	//pointLight7->SetConstantAttenuation(1.0);
	//pointLight7->SetLinearAttenuation(0.0);
	//pointLight7->SetExponentAttenuation(0.0);
	//pointLight7->SetRange(200);

	//Anima::AnimaPointLight* pointLight8 = _scene->GetLightsManager()->CreatePointLight("light-8");
	//pointLight8->SetPosition(-40.0, 20.0, 40.0);
	//pointLight8->SetColor(1.0, 1.0, 1.0);
	//pointLight8->SetIntensity(1.0);
	//pointLight8->SetConstantAttenuation(1.0);
	//pointLight8->SetLinearAttenuation(0.0);
	//pointLight8->SetExponentAttenuation(0.0);
	//pointLight8->SetRange(200);

	_scene->GetLightsManager()->UpdateLightsMatrix(_camera);

	_animationsManager = _scene->GetAnimationsManager();

	_timer.Reset();

	Anima::AnimaTexturesManager* texturesManager = _scene->GetTexturesManager();

	Anima::AnimaArray<Anima::AnimaArray<Anima::AUchar> > data;
	Anima::AUint width;
	Anima::AUint height;
	Anima::AUint depth;
	Anima::AUint mipMapsCount;
	Anima::AUint imagesCount;
	Anima::AnimaTextureFormat format;
	Anima::AnimaTextureInternalFormat internalFormat;
	Anima::AnimaTextureTarget target;

	if (texturesManager->GetTextureDataFromDDSFile(dataPath + "/textures/Roma/cubemap.dds", &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
	{
		Anima::AnimaTexture* texture = _renderer->GetTexture("EnvironmentMap");
		if (texture != nullptr)
		{
			texture->SetWidth(width);
			texture->SetHeight(height);
			texture->SetDepth(depth);

			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}

			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);

			// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = max(mipMapsCount, 1);
			
			Anima::AInt offset = 0;
			for (Anima::AUint i = 0; i < imagesCount; i++)
			{
				for (Anima::AUint j = 0; j < mipMapsCount; j++)
				{
					Anima::AUchar* buffer = &data[offset][0];
					Anima::AUint bufferSize = data[offset].size();

					texture->SetData(buffer, bufferSize, (Anima::AnimaTextureCubeIndex)i, j);

					offset++;
				}
			}

			texture->SetMinFilter(Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR);
			texture->SetMagFilter(Anima::TEXTURE_MAG_FILTER_MODE_LINEAR);
		}
	}

	if (texturesManager->GetTextureDataFromDDSFile(dataPath + "/textures/Roma/Irradiance.dds", &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
	{
		Anima::AnimaTexture* texture = _renderer->GetTexture("IrradianceMap");
		if (texture != nullptr)
		{
			texture->SetWidth(width);
			texture->SetHeight(height);
			texture->SetDepth(depth);

			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}

			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);

			// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = max(mipMapsCount, 1);

			texture->SetTextureTarget(target);

			Anima::AInt offset = 0;
			for (Anima::AUint i = 0; i < imagesCount; i++)
			{
				for (Anima::AUint j = 0; j < mipMapsCount; j++)
				{
					Anima::AUchar* buffer = &data[offset][0];
					Anima::AUint bufferSize = data[offset].size();

					texture->SetData(buffer, bufferSize, (Anima::AnimaTextureCubeIndex)i, j);

					offset++;
				}
			}

			texture->SetMinFilter(Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR);
			texture->SetMagFilter(Anima::TEXTURE_MAG_FILTER_MODE_LINEAR);
		}
	}

	//Anima::AnimaTexture* textureEnv = texturesManager->LoadTextureFromDDSFile(dataPath + "/textures/Roma/cubemap.dds", "dds-env-texture");
	//textureEnv->SetMinFilter(Anima::TEXTURE_MIN_FILTER_MODE_LINEAR_MIPMAP_LINEAR);
	//textureEnv->SetMagFilter(Anima::TEXTURE_MAG_FILTER_MODE_LINEAR);
	//_renderer->SetTexture("EnvironmentMap", textureEnv, false);

	//Anima::AnimaTexture* textureIrr = texturesManager->LoadTextureFromDDSFile(dataPath + "/textures/Roma/Irradiance.dds", "dds-irr-texture");
	//_renderer->SetTexture("IrradianceMap", textureIrr, false);

	_renderer->CheckPrograms(_scene);
	
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