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
#include <AnimaParallelProgramsManager.h>

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
#include <AnimaRandom.h>
#include <AnimaDefaultMouseInteractor.h>
#include <AnimaKeyboardInteractor.h>

#include "main.h"

#define ANIMA_ENGINE_DEMO_SCENE_NAME "AnimaEngineDemoScene"
#define ANIMA_ENGINE_DEMO_CAMERA_NAME "AnimaEngineDemoCamera"
#define ANIMA_ENGINE_DEMO_MODEL_NAME "AnimaEngineDemoModel"

//#define DATA_PATH				"data"
#if defined SAVE_SCENE
	#define DATA_PATH				"D:/Git/Anima/AnimaEngineDemo/Scene"
#else
	#define DATA_PATH				"D:/Git/Anima/AnimaEngineDemo/SceneUff"
#endif

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

Anima::AnimaDefaultMouseInteractor mouseInteractor;
Anima::AnimaKeyboardInteractor keyboardInteractor;

bool _shouldClose = false;
bool sceneSaved = false;

bool mouseMoved = false;

int lastXPos = 0;
int lastYPos = 0;

float camSpeed = 1.0f;
float camSpeedInc = 0.5f;
float camSpeedMax = 10.0f;

bool moveForwardsPressed = false;
bool moveBackwardsPressed = false;
bool moveRightPressed = false;
bool moveLeftPressed = false;
bool moveUpPressed = false;
bool moveDownPressed = false;

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

		Anima::AnimaGCContextConfig context = Anima::AnimaGC::GetDefaultContextConfig();
		context._major = 3;
		context._minor = 3;
		context._debug = false;
		Anima::AnimaGCFrameBufferConfig frameBuffer = Anima::AnimaGC::GetDefaultFrameBufferConfig();
		frameBuffer._depthBits = 32;

		_gc = Anima::AnimaGC::CreateContext((long)hWnd, context, frameBuffer, false);
		if (!_gc)
		{
			MessageBox(NULL, _T("GC creation failed!"), _T(ANIMA_ENGINE_DEMO_NAME), NULL);
			return -1;
		}

		if (!InitEngine())
			return -1;

		if (mouseInteractor.Install((long)hWnd, &_engine))
		{
			mouseInteractor.SetEventHandler("onLeftMouseClick", [&](Anima::AnimaEventArgs* args) {

				Anima::AnimaVertex2f point = ((Anima::AnimaMouseEventArgs*)args)->GetPoint();
				Anima::AnimaVertex2f size = ((Anima::AnimaMouseEventArgs*)args)->GetWindowSize();

				Anima::AnimaEngine* engine = ((Anima::AnimaMouseInteractor*)args->GetSourceEvent())->GetEngine();

				if (engine)
				{
					Anima::AnimaScenesManager* scenesManager = engine->GetScenesManager();
					Anima::AnimaScene* scene = scenesManager->GetActiveScene();

					if (scene)
					{
						Anima::AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
						Anima::AnimaCamera* camera = camerasManager->GetActiveCamera();

						if (camera)
						{
							Anima::AnimaVertex3f origin = camera->GetPosition();
							Anima::AnimaVertex3f end = camera->ScreenPointToWorldPoint(point);

							btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(origin.x, origin.y, origin.z), btVector3(end.x, end.y, end.z));

							btDynamicsWorld* world = scene->GetPhysWorld();

							world->rayTest(btVector3(origin.x, origin.y, origin.z), btVector3(end.x, end.y, end.z), RayCallback);

							if (RayCallback.hasHit())
							{
								Anima::AnimaMeshInstance* instance = (Anima::AnimaMeshInstance*)RayCallback.m_collisionObject->getUserPointer();
								Anima::AnimaVertex3f contactPoint(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z());

								Anima::AnimaString str = Anima::FormatString("Picked material named '%s'", instance->GetMaterial()->GetName().c_str());

								MessageBox(NULL, str.c_str(), "Picking", MB_OK);
							}
							else
							{
								printf("Picked nothing\n");
							}
						}
					}
				}
			});
		}

		if (keyboardInteractor.Install((long)hWnd, &_engine))
		{
			keyboardInteractor.SetEventHandler("onUpdateScene", [&](Anima::AnimaEventArgs* args) {
				Anima::AnimaScene* scene = ((Anima::AnimaUpdateSceneEventArgs*)args)->GetScene();
				Anima::AnimaKeyboardStatusMap keyboardStatus = ((Anima::AnimaKeyboardInteractor*)((Anima::AnimaUpdateSceneEventArgs*)args)->GetSourceEvent())->GetKeyboardStatus();

				if (scene)
				{
					Anima::AnimaCamerasManager* camerasManager = scene->GetCamerasManager();
					Anima::AnimaCamera* camera = camerasManager->GetActiveCamera();

					if (camera)
					{
						Anima::AnimaVertex3f velocity = camera->GetCurrentVelocity();
						Anima::AnimaVertex3f direction(0.0f, 0.0f, 0.0f);

						if (keyboardStatus[Anima::AnimaKeyboardKey::AKK_W])
						{
							if (!moveForwardsPressed)
							{
								moveForwardsPressed = true;
								camera->SetCurrentVelocity(velocity.x, velocity.y, 0.0f);
							}

							direction.z += 1.0f;
						}
						else
						{
							moveForwardsPressed = false;
						}

						if (keyboardStatus[Anima::AnimaKeyboardKey::AKK_S])
						{
							if (!moveBackwardsPressed)
							{
								moveBackwardsPressed = true;
								camera->SetCurrentVelocity(velocity.x, velocity.y, 0.0f);
							}

							direction.z -= 1.0f;
						}
						else
						{
							moveBackwardsPressed = false;
						}

						if (keyboardStatus[Anima::AnimaKeyboardKey::AKK_D])
						{
							if (!moveRightPressed)
							{
								moveRightPressed = true;
								camera->SetCurrentVelocity(0.0f, velocity.y, velocity.z);
							}

							direction.x += 1.0f;
						}
						else
						{
							moveRightPressed = false;
						}

						if (keyboardStatus[Anima::AnimaKeyboardKey::AKK_A])
						{
							if (!moveLeftPressed)
							{
								moveLeftPressed = true;
								camera->SetCurrentVelocity(0.0f, velocity.y, velocity.z);
							}

							direction.x -= 1.0f;
						}
						else
						{
							moveLeftPressed = false;
						}

						if (keyboardStatus[Anima::AnimaKeyboardKey::AKK_E])
						{
							if (!moveUpPressed)
							{
								moveUpPressed = true;
								camera->SetCurrentVelocity(velocity.x, 0.0f, velocity.z);
							}

							direction.y += 1.0f;
						}
						else
						{
							moveUpPressed = false;
						}

						if (keyboardStatus[Anima::AnimaKeyboardKey::AKK_Q])
						{
							if (!moveDownPressed)
							{
								moveDownPressed = true;
								camera->SetCurrentVelocity(velocity.x, 0.0f, velocity.z);
							}

							direction.y -= 1.0f;
						}
						else
						{
							moveDownPressed = false;
						}

						camera->SetUpdateData(direction);
					}
				}
			});
		}

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

	int width, height;

	int scelta = 0;
	printf("Scegliere la dimensione della finestra:\n");
	printf("\t- 0: 1024x768\n");
	printf("\t- 1: 1920x1080\n");
	printf("\t- 2: 1920x1200\n");
	printf("\t- 3: 512x512\n");
	printf("\t- 4: 1280x720\n");
	printf("Inserisci la tua scelta: ");
	std::cin >> scelta;

	switch (scelta)
	{
	case 1:		width = 1920;	height = 1080;	break;
	case 2:		width = 1920;	height = 1200;	break;
	case 3:		width = 512;	height = 512;	break;
	case 4:		width = 1280;	height = 720;	break;
	case 0:
	default:	width = 1024;	height = 768;	break;
	}

	static TCHAR szWindowClass[] = _T(ANIMA_ENGINE_DEMO_NAME);
	static TCHAR szTitle[] = _T(ANIMA_ENGINE_DEMO_NAME);
	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
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
			Anima::AnimaString title = Anima::FormatString("AnimaEngineDemo - FPS: %d", FPS);
			SetWindowText(hWnd, title.c_str());
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

	printf("\nOpenCL available platforms:\n\n");
	Anima::AnimaParallelProgramsManager* ppManager = _engine.GetParallelProgramsManager();
	Anima::AnimaArray<cl_platform_id> platforms = ppManager->GetPlatformIDs();
	for (auto& plat : platforms)
	{
		Anima::AnimaString platName = ppManager->GetPlatformName(plat);
		Anima::AnimaString platVer = ppManager->GetPlatformVersion(plat);
		printf("- Platform name: %s [version %s]\n", platName.c_str(), platVer.c_str());

		Anima::AnimaArray<cl_device_id> devices = ppManager->GetDeviceIDs(plat, Anima::AnimaParallelelProgramType::APP_TYPE_ALL);
		for (auto& dev : devices)
		{
			Anima::AnimaString devName = ppManager->GetDeviceName(dev);
			Anima::AnimaString devVer = ppManager->GetDeviceVersion(dev);
			printf("\t- Device name: %s [version %s]\n", devName.c_str(), devVer.c_str());
		}
	}

	// Creazione del renderer
	_renderer = new Anima::AnimaRenderer(&_engine, _engine.GetGenericAllocator());

	Anima::AnimaString dataPath = DATA_PATH;
	Anima::AnimaString materialsPath = dataPath + "/materials";
	Anima::AnimaString modelPath = dataPath + "/models/";
	Anima::AnimaString modelName = "material2.3ds";
	Anima::AnimaString materialName = "legno";
	Anima::AnimaString sceneModelsPath = modelPath + "scene_models/";
	Anima::AnimaString sceneMeshesPath = modelPath + "scene_models/";

	// Creazione della scena
#if defined SAVE_SCENE
	_scene = _engine.GetScenesManager()->CreateScene(ANIMA_ENGINE_DEMO_SCENE_NAME);	
#else
	_engine.GetScenesManager()->LoadScenes(dataPath);

	_scene = _engine.GetScenesManager()->GetSceneFromName(ANIMA_ENGINE_DEMO_SCENE_NAME);
#endif

	if (!_scene)
		return false;

	_scene->InitializePhysics();
	_scene->InitializePhysicObjects();

	_scene->SetKeyboardInteractor(&keyboardInteractor);
	
	// Caricamento dei materiali
	Anima::AnimaMaterialsManager* materialsManager = _scene->GetMaterialsManager();
	Anima::AnimaModelInstancesManager* modelInstancesManager = _scene->GetModelInstancesManager();
#if defined SAVE_SCENE
	if (!materialsManager->LoadMaterials(materialsPath))
		return false;
#endif

	// Creazione di una telecamera
	_camerasManager = _scene->GetCamerasManager();
#if defined SAVE_SCENE
	_camera = _camerasManager->CreateFirstPersonCamera(ANIMA_ENGINE_DEMO_CAMERA_NAME);
	if (!_camera)
		return false;
#else
	_camera = _camerasManager->GetCameraFromName(ANIMA_ENGINE_DEMO_CAMERA_NAME);
#endif
	
#if defined SAVE_SCENE
	_scene->GetMeshesManager()->LoadMeshes(sceneMeshesPath);
	_scene->GetModelsManager()->LoadModels(sceneModelsPath);
#endif

#if defined SAVE_SCENE
	_model = _scene->GetModelsManager()->GetModelFromName("AnimaEngineDemoModel");
	if (!_model)
		return false;

	_floorModel = _scene->GetModelsManager()->GetModelFromName("floorModel");
	if (!_floorModel)
		return false;
	
	// Creo le istanze del pavimento e imposto i loro materiali
	Anima::AnimaArray<Anima::AnimaMeshInstance*> floorModelInstanceMeshes;

	Anima::AnimaModelInstance* floorModelInstance1 = modelInstancesManager->CreateInstance("floorModelInstance1", _floorModel);
	Anima::AnimaModelInstance* floorModelInstance2 = modelInstancesManager->CreateInstance("floorModelInstance2", _floorModel);
	Anima::AnimaModelInstance* floorModelInstance3 = modelInstancesManager->CreateInstance("floorModelInstance3", _floorModel);
	Anima::AnimaModelInstance* floorModelInstance4 = modelInstancesManager->CreateInstance("floorModelInstance4", _floorModel);

	floorModelInstance1->GetTransformation()->SetScale(0.6, 0.6, 0.6);
	floorModelInstance1->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance1->GetTransformation()->SetRotationYDeg(-90);
	floorModelInstance1->GetAllMeshes(&floorModelInstanceMeshes);

	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));

	floorModelInstanceMeshes.clear();
	floorModelInstance2->GetTransformation()->SetScale(0.6, 0.6, 0.6);
	floorModelInstance2->GetTransformation()->TranslateX(3);
	floorModelInstance2->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance2->GetTransformation()->SetRotationYDeg(-90);
	floorModelInstance2->GetAllMeshes(&floorModelInstanceMeshes);

	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));

	floorModelInstanceMeshes.clear();
	floorModelInstance3->GetTransformation()->SetScale(0.6, 0.6, 0.6);
	floorModelInstance3->GetTransformation()->TranslateZ(3);
	floorModelInstance3->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance3->GetTransformation()->SetRotationYDeg(-270);
	floorModelInstance3->GetAllMeshes(&floorModelInstanceMeshes);

	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));

	floorModelInstanceMeshes.clear();
	floorModelInstance4->GetTransformation()->SetScale(0.6, 0.6, 0.6);
	floorModelInstance4->GetTransformation()->TranslateX(3);
	floorModelInstance4->GetTransformation()->TranslateZ(3);
	floorModelInstance4->GetTransformation()->SetRotationXDeg(-90);
	floorModelInstance4->GetTransformation()->SetRotationYDeg(-270);
	floorModelInstance4->GetAllMeshes(&floorModelInstanceMeshes);

	for (int j = 0; j < floorModelInstanceMeshes.size(); j++)
		floorModelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("floor-material"));

	// Creo le istanze degli oggetti applicando i materiali
	Anima::AnimaArray<Anima::AnimaMeshInstance*> modelInstanceMeshes;

	Anima::AnimaModelInstance* modelInstanceOro = modelInstancesManager->CreateInstance("modelInstanceOro", _model);
	Anima::AnimaModelInstance* modelInstanceRame = modelInstancesManager->CreateInstance("modelInstanceRame", _model);
	Anima::AnimaModelInstance* modelInstanceGomma = modelInstancesManager->CreateInstance("modelInstanceGomma", _model);
	Anima::AnimaModelInstance* modelInstanceLegno = modelInstancesManager->CreateInstance("modelInstanceLegno", _model);

	modelInstanceOro->GetTransformation()->SetScale(0.2, 0.2, 0.2);
	modelInstanceOro->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceOro->GetTransformation()->SetRotationYDeg(0);
	modelInstanceOro->GetAllMeshes(&modelInstanceMeshes);

	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("oro"));

	modelInstanceMeshes.clear();
	modelInstanceRame->GetTransformation()->SetScale(0.2, 0.2, 0.2);
	modelInstanceRame->GetTransformation()->TranslateX(3);
	modelInstanceRame->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceRame->GetTransformation()->SetRotationYDeg(0);
	modelInstanceRame->GetAllMeshes(&modelInstanceMeshes);

	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("rame"));

	modelInstanceMeshes.clear();
	modelInstanceGomma->GetTransformation()->SetScale(0.2, 0.2, 0.2);
	modelInstanceGomma->GetTransformation()->TranslateZ(3);
	modelInstanceGomma->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceGomma->GetTransformation()->SetRotationYDeg(180);
	modelInstanceGomma->GetAllMeshes(&modelInstanceMeshes);

	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("gomma"));

	modelInstanceMeshes.clear();
	modelInstanceLegno->GetTransformation()->SetScale(0.2, 0.2, 0.2);
	modelInstanceLegno->GetTransformation()->TranslateX(3);
	modelInstanceLegno->GetTransformation()->TranslateZ(3);
	modelInstanceLegno->GetTransformation()->SetRotationXDeg(-90);
	modelInstanceLegno->GetTransformation()->SetRotationYDeg(180);
	modelInstanceLegno->GetAllMeshes(&modelInstanceMeshes);

	for (int j = 0; j < modelInstanceMeshes.size(); j++)
		modelInstanceMeshes[j]->SetMaterial(materialsManager->GetMaterialFromName("legno"));

	_camera->LookAt(-3.5, 1.0, 1.5, 1.0, -0.2, 0.0);
#endif

	_camera->Activate();

#if defined SAVE_SCENE
	Anima::AnimaDirectionalLight* directionalLight = _scene->GetLightsManager()->CreateDirectionalLight("light-0");
	directionalLight->SetDirection(-1.0, -1.0, -1.0);
	directionalLight->SetColor(1.0, 1.0, 1.0);
	directionalLight->SetIntensity(1.0);
	
	//Anima::AnimaPointLight* pointLight1 = _scene->GetLightsManager()->CreatePointLight("light-1");
	//pointLight1->SetPosition(-40.0, 20.0, 0.0);
	//pointLight1->SetColor(1.0, 0.0, 0.0);
	//pointLight1->SetIntensity(1.0);
	//pointLight1->SetConstantAttenuation(1.0);
	//pointLight1->SetLinearAttenuation(0.0);
	//pointLight1->SetExponentAttenuation(0.0);
	//pointLight1->SetRange(200);
#endif

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

	if (texturesManager->GetTextureDataFromDDSFile(dataPath + "/../Scene/textures/Roma/cubemap.dds", &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
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

	if (texturesManager->GetTextureDataFromDDSFile(dataPath + "/../Scene/textures/Roma/Irradiance.dds", &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
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

#if defined SAVE_SCENE
	Anima::AnimaTexture* textureSkyBox = texturesManager->LoadTextureFromDDSFile(dataPath + "/textures/Roma/cubemap.dds", "dds-skybox-texture");
#else
	Anima::AnimaTexture* textureSkyBox = texturesManager->GetTextureFromName("dds-skybox-texture");
	textureSkyBox->Load();
#endif
	_renderer->SetTexture("SkyBox", textureSkyBox, false);
	_renderer->CheckPrograms(_scene);
		
	return true;
}

void SetViewport(int w, int h)
{
	if (_camerasManager)
	{
		Anima::AnimaVertex2f size((float)w, (float)h);
		_camerasManager->UpdatePerspectiveCameras(90.0f, size, 0.1f, 100.0f);
	}

	if (_scene)
		_scene->GetLightsManager()->UpdateLightsMatrix(_camera);

	if (_renderer)
	{
		_renderer->InitRenderingTargets(w, h);
		_renderer->InitRenderingUtilities(w, h);
	}
	
	if (!sceneSaved)
	{
#if defined SAVE_SCENE
		_engine.GetScenesManager()->SaveSceneToFile(_scene, "C:/Users/Marco/Desktop/Scene", true);
#endif
		sceneSaved = true;
	}
}

void UpdateFrame()
{
	if (_gc)
	{
		_gc->MakeCurrent();

		if (_renderer && _scene)
		{
			if(!_scene->IsRunning())
				_scene->StartScene();

			_renderer->Start(_scene);
			_renderer->Render();

			_scene->StepScene();
		}

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