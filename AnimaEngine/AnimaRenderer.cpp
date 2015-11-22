#include "AnimaEngine.h"
#include "AnimaRenderer.h"
#include "AnimaCamera.h"
#include "AnimaShaderProgram.h"
#include "AnimaTexture.h"
#include "AnimaLight.h"
#include "AnimaNodesManager.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaLightsManager.h"
#include "AnimaTimer.h"
#include <fstream>
#include "AnimaTypeMappedArray.h"
#include "AnimaMappedArray.h"
#include "AnimaArray.h"
#include "AnimaScene.h"
#include "AnimaGeometryCreator.h"
#include "AnimaLogger.h"

//#define ENGINE_DATA_PATH				"data"
//#define ENGINE_DATA_PATH				"/Users/marco/Documents/Progetti/Repository/Anima/AnimaEngine/data"
#define ENGINE_DATA_PATH				"D:/Git/Anima/AnimaEngine/data"
#define SHADERS_PATH					ENGINE_DATA_PATH "/shaders/"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRenderer::AnimaRenderer(AnimaEngine* engine, AnimaAllocator* allocator)
{
	_allocator = allocator != nullptr ? allocator : engine->GetGenericAllocator();
	_engine = engine;
	_scene = nullptr;

	_filterCamera = nullptr;
	_filterGeometry = nullptr;
	
	//_indexesBufferObject = 0;
	//_verticesBufferObject = 0;
	//_vertexArrayObject = 0;

	_lastUpdatedNode = nullptr;
	_lastUpdatedNodeInstance = nullptr;

	InitTextureSlots();

	AddRenderPassFunction(PreparePass);
	AddRenderPassFunction(LightPass);
	AddRenderPassFunction(CombinePass);
	AddRenderPassFunction(BloomCreationPass);
	AddRenderPassFunction(FinalPass);
//	AddRenderPassFunction(DirectionalLightShadowMap);

	AnimaTexture* environmentTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator);
	AnimaTexture* irradianceTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator);

	environmentTexture->SetName("EnvironmentMap");
	irradianceTexture->SetName("IrradianceMap");

	SetTexture("EnvironmentMap", environmentTexture);
	SetTexture("IrradianceMap", irradianceTexture);

	_programsBufferIndex = 0;
	
	DisablePhysicsDebugDrawing();
	_physicsDebugDrawer = new AnimaPhysicsDebugDrawer;
	
	InitializeShaders();
}

AnimaRenderer::AnimaRenderer(AnimaRenderer& src)
	//: _primitives(src._primitives)
{
	_allocator = src._allocator;

	_texturesMap = src._texturesMap;
	_textureSlotsMap = src._textureSlotsMap;
	_gBuffersMap = src._gBuffersMap;

	_vectors2fMap = src._vectors2fMap;
	_vectors3fMap = src._vectors3fMap;
	_vectors4fMap = src._vectors4fMap;

	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;

	_geometriesMap = src._geometriesMap;

	//_indexesBufferObject = src._indexesBufferObject;
	//_verticesBufferObject = src._verticesBufferObject;
	//_vertexArrayObject = src._vertexArrayObject;

	_scene = src._scene;
	_engine = src._engine;

	_programsBufferIndex = 0;
	
	_physicsDebugDrawer = src._physicsDebugDrawer;
}

AnimaRenderer::AnimaRenderer(AnimaRenderer&& src)
	//: _primitives(src._primitives)
{
	_allocator = src._allocator;
	_texturesMap = src._texturesMap;
	_textureSlotsMap = src._textureSlotsMap;
	_gBuffersMap = src._gBuffersMap;

	_vectors2fMap = src._vectors2fMap;
	_vectors3fMap = src._vectors3fMap;
	_vectors4fMap = src._vectors4fMap;

	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;

	_geometriesMap = src._geometriesMap;

	//_indexesBufferObject = src._indexesBufferObject;
	//_verticesBufferObject = src._verticesBufferObject;
	//_vertexArrayObject = src._vertexArrayObject;

	_scene = src._scene;
	_engine = src._engine;

	_programsBufferIndex = 0;
	
	_physicsDebugDrawer = src._physicsDebugDrawer;
}

AnimaRenderer::~AnimaRenderer()
{
	Clear();
	
	if(_physicsDebugDrawer != nullptr)
	{
		delete _physicsDebugDrawer;
		_physicsDebugDrawer = nullptr;
	}
}

AnimaRenderer& AnimaRenderer::operator=(const AnimaRenderer& src)
{
	if (this != &src)
	{
		Clear();

		_allocator = src._allocator;
		
		_texturesMap = src._texturesMap;
		_textureSlotsMap = src._textureSlotsMap;
		_gBuffersMap = src._gBuffersMap;

		_vectors2fMap = src._vectors2fMap;
		_vectors3fMap = src._vectors3fMap;
		_vectors4fMap = src._vectors4fMap;

		_floatsMap = src._floatsMap;
		_integersMap = src._integersMap;
		_booleansMap = src._booleansMap;

		_geometriesMap = src._geometriesMap;

		//_indexesBufferObject = src._indexesBufferObject;
		//_verticesBufferObject = src._verticesBufferObject;
		//_vertexArrayObject = src._vertexArrayObject;

		_scene = src._scene;
		_engine = src._engine;

		_programsBufferIndex = 0;
		
		_physicsDebugDrawer = src._physicsDebugDrawer;
	}

	return *this;
}

AnimaRenderer& AnimaRenderer::operator=(AnimaRenderer&& src)
{
	if (this != &src)
	{
		Clear();

		_allocator = src._allocator;

		_texturesMap = src._texturesMap;
		_textureSlotsMap = src._textureSlotsMap;
		_gBuffersMap = src._gBuffersMap;

		_vectors2fMap = src._vectors2fMap;
		_vectors3fMap = src._vectors3fMap;
		_vectors4fMap = src._vectors4fMap;

		_floatsMap = src._floatsMap;
		_integersMap = src._integersMap;
		_booleansMap = src._booleansMap;

		_geometriesMap = src._geometriesMap;

		//_indexesBufferObject = src._indexesBufferObject;
		//_verticesBufferObject = src._verticesBufferObject;
		//_vertexArrayObject = src._vertexArrayObject;

		_scene = src._scene;
		_engine = src._engine;

		_programsBufferIndex = 0;
		
		_physicsDebugDrawer = src._physicsDebugDrawer;
	}

	return *this;
}

void AnimaRenderer::InitTextureSlots()
{
	// Slot usati dai materiali
	SetTextureSlot("DiffuseTexture", 0);
	SetTextureSlot("BumpTexture", 1);
	SetTextureSlot("DisplacementTexture", 2);
	SetTextureSlot("OpacityTexture", 3);
	SetTextureSlot("SpecularTexture", 4);

	// Slot usati dal deferred shading
	SetTextureSlot("DepthMap", 0);
	SetTextureSlot("AlbedoMap", 1);
	SetTextureSlot("NormalMap", 2);
	SetTextureSlot("EmissiveMap", 3);
	SetTextureSlot("SpecularMap", 4);
	SetTextureSlot("DILShadowMap", 5);
	SetTextureSlot("SkyBox", 6);
	SetTextureSlot("EnvironmentMap", 7);
	SetTextureSlot("IrradianceMap", 8);

	// Slot usati dal disegno di primitive
	SetTextureSlot("DepthMap", 0);
	SetTextureSlot("Diffuse2Map", 1);
	SetTextureSlot("PDepthMap", 2);
	SetTextureSlot("PColorMap", 3);
	SetTextureSlot("BloomMap", 4);

	// Slot usati dal deferred shading

	// Slot usati dai filtri
	SetTextureSlot("FilterMap", 0);
}

void AnimaRenderer::InitRenderingTargets(AInt screenWidth, AInt screenHeight)
{
	AnimaVertex2f oldSize = GetVector2f("ScreenSize");
	AnimaVertex2f newSize((AFloat)screenWidth, (AFloat)screenHeight);

	AUint width = (AUint)screenWidth;
	AUint height = (AUint)screenHeight;

	if (oldSize != newSize)
	{
		AnimaGBuffer* prepassBuffer = GetGBuffer("PrepassBuffer");
		if (prepassBuffer != nullptr)
			prepassBuffer->Resize(width, height);
		else
		{
			prepassBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaGBuffer>(*_allocator, _allocator, width, height);
			prepassBuffer->AddTexture("DepthMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_DEPTH_STENCIL, TEXTURE_INTERNAL_FORMAT_DEPTH24_STENCIL8, TEXTURE_FORMAT_DEPTH_STENCIL, TEXTURE_DATA_TYPE_UNSIGNED_INT_24_8, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			//prepassBuffer->AddTexture("DepthMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_DEPTH, TEXTURE_INTERNAL_FORMAT_DEPTH24, TEXTURE_FORMAT_DEPTH, TEXTURE_DATA_TYPE_FLOAT, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("AlbedoMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_COLOR0, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_FORMAT_RGBA, TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("NormalMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_COLOR1, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_FORMAT_RGBA, TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("SpecularMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_COLOR2, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_FORMAT_RGBA, TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			ANIMA_ASSERT(prepassBuffer->Create());

			SetGBuffer("PrepassBuffer", prepassBuffer);
		}

		AnimaGBuffer* lightsBuffer = GetGBuffer("LightsBuffer");
		if (lightsBuffer != nullptr)
			lightsBuffer->Resize(width, height);
		else
		{
			lightsBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaGBuffer>(*_allocator, _allocator, width, height);
			lightsBuffer->AddTexture("EmissiveMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_COLOR0, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_FORMAT_RGBA, TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			lightsBuffer->AddTexture("SpecularMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_COLOR1, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_FORMAT_RGBA, TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			lightsBuffer->AddTexture("IrradianceMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_COLOR2, TEXTURE_INTERNAL_FORMAT_RGBA8, TEXTURE_FORMAT_RGBA, TEXTURE_DATA_TYPE_UNSIGNED_BYTE, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
			ANIMA_ASSERT(lightsBuffer->Create());

			SetGBuffer("LightsBuffer", lightsBuffer);
		}
		
		//AnimaGBuffer* primitivesBuffer = GetGBuffer("PrimitivesBuffer");
		//if (primitivesBuffer != nullptr)
		//	primitivesBuffer->Resize(screenWidth, screenHeight);
		//else
		//{
		//	primitivesBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaGBuffer>(*_allocator, _allocator, screenWidth, screenHeight);
		//	primitivesBuffer->AddTexture("PDepthMap", GL_TEXTURE_2D, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
		//	primitivesBuffer->AddTexture("PColorMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
		//	ANIMA_ASSERT(primitivesBuffer->Create());
		//	
		//	SetGBuffer("PrimitivesBuffer", primitivesBuffer);
		//}
		
		AnimaTexture* diffuseTexture = GetTexture("DiffuseMap");
		if(diffuseTexture != nullptr)
			diffuseTexture->Resize(width, height);
		else
		{
			diffuseTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "DiffuseMap", width, height, nullptr, 0);
			diffuseTexture->SetTextureTarget(TEXTURE_TARGET_2D);
			diffuseTexture->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
			diffuseTexture->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
			diffuseTexture->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA32F);
			diffuseTexture->SetFormat(TEXTURE_FORMAT_RGBA);
			diffuseTexture->SetDataType(TEXTURE_DATA_TYPE_FLOAT);
			diffuseTexture->SetClampS(TEXTURE_CLAMP_TO_EDGE);
			diffuseTexture->SetClampT(TEXTURE_CLAMP_TO_EDGE);
			diffuseTexture->SetClampR(TEXTURE_CLAMP_TO_EDGE);
			diffuseTexture->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);

			ANIMA_ASSERT(diffuseTexture->LoadRenderTargets());
			SetTexture("DiffuseMap", diffuseTexture);
		}
		
		
		AnimaTexture* finalTexture = GetTexture("FinalMap");
		if(finalTexture != nullptr)
			diffuseTexture->Resize(width, height);
		else
		{
			finalTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "FinalMap", width, height, nullptr, 0);
			finalTexture->SetTextureTarget(TEXTURE_TARGET_2D);
			finalTexture->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
			finalTexture->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
			finalTexture->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA32F);
			finalTexture->SetFormat(TEXTURE_FORMAT_RGBA);
			finalTexture->SetDataType(TEXTURE_DATA_TYPE_FLOAT);
			finalTexture->SetClampS(TEXTURE_CLAMP_TO_EDGE);
			finalTexture->SetClampT(TEXTURE_CLAMP_TO_EDGE);
			finalTexture->SetClampR(TEXTURE_CLAMP_TO_EDGE);
			finalTexture->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);
			
			ANIMA_ASSERT(finalTexture->LoadRenderTargets());
			SetTexture("FinalMap", finalTexture);
		}

		AnimaTexture* directionalLightShadowMapTextureTemp = GetTexture("DILShadowMapTemp");
		if (directionalLightShadowMapTextureTemp == nullptr)
		{
			AUint w = 1024;
			AUint h = 1024;

			directionalLightShadowMapTextureTemp = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "DILShadowMapTemp", w, h, nullptr, 0);
			directionalLightShadowMapTextureTemp->SetTextureTarget(TEXTURE_TARGET_2D);
			directionalLightShadowMapTextureTemp->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
			directionalLightShadowMapTextureTemp->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
			//directionalLightShadowMapTextureTemp->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_DEPTH24);
			//directionalLightShadowMapTextureTemp->SetFormat(TEXTURE_FORMAT_DEPTH);
			//directionalLightShadowMapTextureTemp->SetDataType(TEXTURE_DATA_TYPE_FLOAT);
			directionalLightShadowMapTextureTemp->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RG32F);
			directionalLightShadowMapTextureTemp->SetFormat(TEXTURE_FORMAT_RG);
			directionalLightShadowMapTextureTemp->SetDataType(TEXTURE_DATA_TYPE_UNSIGNED_BYTE);
			directionalLightShadowMapTextureTemp->SetClampS(TEXTURE_CLAMP_TO_BORDER);
			directionalLightShadowMapTextureTemp->SetClampT(TEXTURE_CLAMP_TO_BORDER);
			directionalLightShadowMapTextureTemp->SetClampR(TEXTURE_CLAMP_TO_BORDER);
			//directionalLightShadowMapTextureTemp->SetAttachment(TEXTURE_ATTACHMENT_DEPTH);
			directionalLightShadowMapTextureTemp->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);
			directionalLightShadowMapTextureTemp->SetBorderColor(AnimaColor4f(1.0f, 1.0f, 1.0f, 1.0f));

			ANIMA_ASSERT(directionalLightShadowMapTextureTemp->LoadRenderTargets());
			SetTexture("DILShadowMapTemp", directionalLightShadowMapTextureTemp);
		}

		AnimaTexture* directionalLightShadowMapTexture = GetTexture("DILShadowMap");
		if(directionalLightShadowMapTexture == nullptr)
		{
			AUint w = 1024;
			AUint h = 1024;
			
			directionalLightShadowMapTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "DILShadowMap", w, h, nullptr, 0);
			directionalLightShadowMapTexture->SetTextureTarget(TEXTURE_TARGET_2D);
			directionalLightShadowMapTexture->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
			directionalLightShadowMapTexture->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
			//directionalLightShadowMapTexture->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_DEPTH24);
			//directionalLightShadowMapTexture->SetFormat(TEXTURE_FORMAT_DEPTH);
			//directionalLightShadowMapTexture->SetDataType(TEXTURE_DATA_TYPE_FLOAT);
			directionalLightShadowMapTexture->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RG32F);
			directionalLightShadowMapTexture->SetFormat(TEXTURE_FORMAT_RG);
			directionalLightShadowMapTexture->SetDataType(TEXTURE_DATA_TYPE_UNSIGNED_BYTE);
			directionalLightShadowMapTexture->SetClampS(TEXTURE_CLAMP_TO_BORDER);
			directionalLightShadowMapTexture->SetClampT(TEXTURE_CLAMP_TO_BORDER);
			directionalLightShadowMapTexture->SetClampR(TEXTURE_CLAMP_TO_BORDER);
			//directionalLightShadowMapTexture->SetAttachment(TEXTURE_ATTACHMENT_DEPTH);
			directionalLightShadowMapTexture->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);
			directionalLightShadowMapTexture->SetBorderColor(AnimaColor4f(1.0f, 1.0f, 1.0f, 1.0f));
			
			ANIMA_ASSERT(directionalLightShadowMapTexture->LoadRenderTargets());
			SetTexture("DILShadowMap", directionalLightShadowMapTexture);
		}

		AUint divisore = 8;

		AnimaTexture* bloomTexture = GetTexture("BloomMap");
		if (bloomTexture != nullptr)
		{
			AUint w = width / divisore;
			AUint h = height / divisore;

			bloomTexture->Resize(w, h);

			AnimaVertex2f bloomMapSize((AFloat)w, (AFloat)h);
			AnimaVertex2f inverseBloomMapSize(1.0f / (AFloat)w, 1.0f / (AFloat)h);

			SetVector("BloomMapSize", bloomMapSize);
			SetVector("InverseBloomMapSize", inverseBloomMapSize);
		}
		else
		{
			AUint w = width / divisore;
			AUint h = height / divisore;

			bloomTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "BloomMap", w, h, nullptr, 0);
			bloomTexture->SetTextureTarget(TEXTURE_TARGET_2D);
			bloomTexture->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
			bloomTexture->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
			bloomTexture->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA32F);
			bloomTexture->SetFormat(TEXTURE_FORMAT_RGBA);
			bloomTexture->SetDataType(TEXTURE_DATA_TYPE_FLOAT);
			bloomTexture->SetClampS(TEXTURE_CLAMP_TO_EDGE);
			bloomTexture->SetClampT(TEXTURE_CLAMP_TO_EDGE);
			bloomTexture->SetClampR(TEXTURE_CLAMP_TO_EDGE);
			bloomTexture->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);

			ANIMA_ASSERT(bloomTexture->LoadRenderTargets());
			SetTexture("BloomMap", bloomTexture);

			AnimaVertex2f bloomMapSize((AFloat)w, (AFloat)h);
			AnimaVertex2f inverseBloomMapSize(1.0f / (AFloat)w, 1.0f / (AFloat)h);

			SetVector("BloomMapSize", bloomMapSize);
			SetVector("InverseBloomMapSize", inverseBloomMapSize);
		}

		AnimaTexture* bloomTextureTemp = GetTexture("BloomMapTemp");
		if (bloomTextureTemp != nullptr)
		{
			AUint w = width / divisore;
			AUint h = height / divisore;

			bloomTextureTemp->Resize(w, h);
		}
		else
		{
			AUint w = width / divisore;
			AUint h = height / divisore;

			bloomTextureTemp = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "BloomMapTemp", w, h, nullptr, 0);
			bloomTextureTemp->SetTextureTarget(TEXTURE_TARGET_2D);
			bloomTextureTemp->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
			bloomTextureTemp->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
			bloomTextureTemp->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA32F);
			bloomTextureTemp->SetFormat(TEXTURE_FORMAT_RGBA);
			bloomTextureTemp->SetDataType(TEXTURE_DATA_TYPE_FLOAT);
			bloomTextureTemp->SetClampS(TEXTURE_CLAMP_TO_EDGE);
			bloomTextureTemp->SetClampT(TEXTURE_CLAMP_TO_EDGE);
			bloomTextureTemp->SetClampR(TEXTURE_CLAMP_TO_EDGE);
			bloomTextureTemp->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);

			ANIMA_ASSERT(bloomTextureTemp->LoadRenderTargets());
			SetTexture("BloomMapTemp", bloomTextureTemp);
		}
		
		SetGBuffer("FilterBuffer", nullptr, false);
		SetTexture("FilterMap", nullptr, false);

		SetVector("ScreenSize", (AFloat)screenWidth, (AFloat)screenHeight);
		SetVector("InverseScreenSize", 1.0f / (AFloat)screenWidth, 1.0f / (AFloat)screenHeight);
	}
}

void AnimaRenderer::InitRenderingUtilities(AInt screenWidth, AInt screenHeight)
{
	if (_filterGeometry != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterGeometry);
		_filterGeometry = nullptr;
	}

	if (_filterCamera != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterCamera);
		_filterCamera = nullptr;
	}

	SetFloat("FxaaReduceMin", 1.0f / 128.0f);
	SetFloat("FxaaReduceMul", 1.0f / 8.0f);
	SetFloat("FxaaSpanMax", 8.0f);
	SetBoolean("ApplyFXAA", true);
	SetFloat("BlurSize", 0.002f);
	SetFloat("BloomBlurScale", 0.002f);
	SetFloat("BrightnessThreshold", 0.9f);
	SetColor("AmbientLight", 0.5f, 0.5f, 0.5f);
	SetColor("BackColor", 0.0f, 0.0f, 0.0f, 1.0f);

	SetVector("SSAOFilterRadius", 5.0f, 5.0f);
	SetFloat("SSAODistanceThreshold", 5.0f);
	SetVector("DILShadowMapTexelSize", AnimaVertex2f(1.0f / 1024.0f));
	
	//
	// Inizializzazione delle geometry di supporto
	//
	AnimaString name = "filter_RENGEOMETRY";
	_filterGeometry = AnimaAllocatorNamespace::AllocateNew<AnimaGeometry>(*_allocator, name, _engine->GetDataGeneratorsManager(), _allocator);
	_filterGeometry->MakePlane();
	_filterGeometry->GetTransformation()->RotateXDeg(90.0f);

	AnimaString nameSkyGeometry = "skybox_RENGEOMETRY";
	AnimaGeometry* skyGeometry = AnimaAllocatorNamespace::AllocateNew<AnimaGeometry>(*_allocator, nameSkyGeometry, _engine->GetDataGeneratorsManager(), _allocator);
	skyGeometry->MakeCube();
	_geometriesMap[nameSkyGeometry] = skyGeometry;

	AnimaGeometry* ptlGeometry = CreateGeometryForLightType<AnimaPointLight>();
	ptlGeometry->MakeIcosahedralSphere(2);

	AnimaGeometry* dilGeometry = CreateGeometryForLightType<AnimaDirectionalLight>();
	dilGeometry->MakePlane();

	AnimaGeometry* helGeometry = CreateGeometryForLightType<AnimaHemisphereLight>();
	helGeometry->MakePlane();
	
	AnimaGeometry* splGeometry = CreateGeometryForLightType<AnimaSpotLight>();
	splGeometry->MakeCylinder(0.0f, -1.0f, 1.0f, 60);

	//
	// Inizializzazione della camera di supporto
	//
	_filterCamera = AnimaAllocatorNamespace::AllocateNew<AnimaCamera>(*_allocator, _allocator, nullptr, _engine->GetDataGeneratorsManager(), AnimaString("$$filterCamera$$"));
	_filterCamera->SetPosition(0.0f, 0.0f, 0.0f);

	//glGenVertexArrays(1, &_vertexArrayObject);
	//glGenBuffers(1, &_indexesBufferObject);
	//glGenBuffers(1, &_verticesBufferObject);
}

void AnimaRenderer::ApplyEffectFromTextureToTexture(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst)
{
	ANIMA_ASSERT(src != dst);

	if (dst != nullptr)
	{
		dst->BindAsRenderTarget();

		SetVector("DestinationTextureSize", AnimaVertex2f((AFloat)dst->GetWidth(), (AFloat)dst->GetHeight()));
	}
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterGeometry->NeedsBuffersUpdate())
		_filterGeometry->UpdateBuffers();

	SetTexture("FilterMap", src, false);
	SetVector("TextureSize", AnimaVertex2f((AFloat)src->GetWidth(), (AFloat)src->GetHeight()));

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterGeometry->Draw(this, filterProgram, true, true, false);

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderer::ApplyEffectFromTextureToGBuffer(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaGBuffer* dst)
{
	if (dst != nullptr)
	{
		dst->BindAsRenderTarget();

		SetVector("DestinationTextureSize", AnimaVertex2f((AFloat)dst->GetWidth(), (AFloat)dst->GetHeight()));
	}
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterGeometry->NeedsBuffersUpdate())
		_filterGeometry->UpdateBuffers();

	SetTexture("FilterMap", src, false);
	SetVector("TextureSize", AnimaVertex2f((AFloat)src->GetWidth(), (AFloat)src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterGeometry->Draw(this, filterProgram, true, true, false);

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderer::ApplyEffectFromGBufferToGBuffer(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaGBuffer* dst)
{
	ANIMA_ASSERT(src != dst);

	if (dst != nullptr)
	{
		dst->BindAsRenderTarget();

		SetVector("DestinationTextureSize", AnimaVertex2f((AFloat)dst->GetWidth(), (AFloat)dst->GetHeight()));
	}
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterGeometry->NeedsBuffersUpdate())
		_filterGeometry->UpdateBuffers();

	SetGBuffer("FilterBuffer", src, false);

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterGeometry->Draw(this, filterProgram, true, true, false);
	
	SetGBuffer("FilterBuffer", nullptr, false);
}

void AnimaRenderer::ApplyEffectFromGBufferToTexture(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaTexture* dst)
{
	if (dst != nullptr)
	{
		dst->BindAsRenderTarget();

		SetVector("DestinationTextureSize", AnimaVertex2f((AFloat)dst->GetWidth(), (AFloat)dst->GetHeight()));
	}
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterGeometry->NeedsBuffersUpdate())
		_filterGeometry->UpdateBuffers();

	SetGBuffer("FilterBuffer", src, false);

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterGeometry->Draw(this, filterProgram, true, true, false);

	SetGBuffer("FilterBuffer", nullptr, false);
}

void AnimaRenderer::Start(AnimaScene* scene)
{
	ANIMA_ASSERT(scene != nullptr);
	_scene = scene;
	
	if(IsPhysicsDebugDrawingEnabled())
	{
		_physicsDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
		_scene->GetPhysWorld()->setDebugDrawer(_physicsDebugDrawer);
	}
}

void AnimaRenderer::Start()
{
	ANIMA_ASSERT(_scene != nullptr);
	_scene->GetShadersManager()->SetActiveProgram(nullptr);
	
	_lastUpdatedNode = nullptr;
	_lastUpdatedNodeInstance = nullptr;
}

void AnimaRenderer::Finish()
{
	ANIMA_ASSERT(_scene != nullptr);
	_scene->GetShadersManager()->SetActiveProgram(nullptr);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_lastUpdatedNode = nullptr;
	_lastUpdatedNodeInstance = nullptr;
}

void AnimaRenderer::Render()
{
	_renderMutex.lock();
	
	for (auto func : _renderPassesFunction)
		func(this);

	_programsBufferIndex = (_programsBufferIndex + 1) % AnimaShaderGroupData::GetMaxBuffersCount();
	
	_renderMutex.unlock();
}

void AnimaRenderer::DrawGeometry(AnimaGeometry* geometry, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum, bool useInstances)
{
	if (!useInstances || geometry->GetInstancesCount() == 0)
	{
		AnimaTransformation* geometryTransfomation = geometry->GetTransformation();

		if (!forceDraw)
		{
			if (frustum != nullptr && !frustum->SphereInFrustum(geometryTransfomation->GetTransformationMatrix() * geometry->GetBoundingBoxCenter(), (geometry->GetBoundingBoxMin() - geometry->GetBoundingBoxMax()).Length()))
				return;
		}

		AnimaNode* geometryParent = (AnimaNode*)geometry->GetParentObject()->GetAncestorObject();
		if (geometryParent != nullptr)
		{
			if (geometryParent != _lastUpdatedNode)
			{
				_lastUpdatedNode = geometryParent;
				program->UpdateSceneObjectProperties(_lastUpdatedNode, this);
			}
		}

		if (geometry->NeedsBuffersUpdate())
			geometry->UpdateBuffers();

		if (geometry->GetVertexArrayObject() <= 0)
			return;

		geometry->Draw(this, program, true, true, updateMaterial);
	}
	else
	{
		if (geometry->NeedsBuffersUpdate())
			geometry->UpdateBuffers();

		if (geometry->GetVertexArrayObject() <= 0)
			return;

		AInt instancesCount = geometry->GetInstancesCount();
		for (AInt i = 0; i < instancesCount; i++)
		{
			AnimaGeometryInstance* instance = geometry->GetInstance(i);

			AnimaTransformation* instanceTransfomation = instance->GetTransformation();
			if (!forceDraw)
			{
				if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * geometry->GetBoundingBoxCenter(), (geometry->GetBoundingBoxMin() - geometry->GetBoundingBoxMax()).Length()))
				{
					continue;
				}
			}

			AnimaNodeInstance* instanceParent = (AnimaNodeInstance*)instance->GetParentObject()->GetAncestorObject();
			if (instanceParent != nullptr)
			{
				if (instanceParent != _lastUpdatedNodeInstance)
				{
					_lastUpdatedNodeInstance = instanceParent;
					program->UpdateSceneObjectProperties(_lastUpdatedNodeInstance, this);
				}
			}

			instance->Draw(this, program, true, true, updateMaterial);
		}
	}
}

void AnimaRenderer::BuildDrawableObjectsArray(AnimaArray<AnimaRendererDrawableGeometry>* drawableGeometries, AnimaCamera* camera)
{
	if (drawableGeometries == nullptr || camera == nullptr || _scene == nullptr)
		return;

	AnimaFrustum* frustum = camera->GetFrustum();
	AnimaGeometriesManager* geometriesManager = _scene->GetGeometriesManager();

	AInt geometriesCount = geometriesManager->GetGeometriesCount();
	for (AInt i = 0; i < geometriesCount; i++)
	{
		AnimaGeometry* geometry = geometriesManager->GetGeometry(i);

		AnimaRendererDrawableGeometry drawableGeometry;
		drawableGeometry.SetGeometry(geometry);

		AnimaArray<AnimaRendererDrawableGeometryInstances>* drawableGeometryInstances = drawableGeometry.GetDrawableGeometryInstances();

		AInt instancesCount = geometry->GetInstancesCount();
		for (AInt j = 0; j < instancesCount; j++)
		{
			AnimaGeometryInstance* instance = geometry->GetInstance(j);
			
			AnimaTransformation* instanceTransfomation = instance->GetTransformation();
			if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * geometry->GetBoundingBoxCenter(), (geometry->GetBoundingBoxMin() - geometry->GetBoundingBoxMax()).Length()))
				continue;

			AnimaShaderProgram* program = instance->GetShaderProgram();
			if (program == nullptr)
				continue;

			AInt index = FindDrawableObjecsFromProgram(drawableGeometryInstances, program);
			if (index >= 0)
				drawableGeometryInstances->at(index).AddGeometryInstance(instance);
			else
			{
				AnimaRendererDrawableGeometryInstances newDrawableGeometryInstances;
				newDrawableGeometryInstances.SetShaderProgram(program);
				newDrawableGeometryInstances.AddGeometryInstance(instance);

				drawableGeometryInstances->push_back(newDrawableGeometryInstances);
			}
		}

		if (drawableGeometryInstances->size() > 0)
			drawableGeometries->push_back(drawableGeometry);
	}
}

AInt AnimaRenderer::FindDrawableObjecsFromProgram(AnimaArray<AnimaRendererDrawableGeometryInstances>* drawableGeometryInstances, AnimaShaderProgram* program)
{
	if (drawableGeometryInstances == nullptr || program == nullptr)
		return -1;

	AInt count = drawableGeometryInstances->size();
	for (AInt i = 0; i < count; i++)
	{
		if (drawableGeometryInstances->at(i).GetShaderProgram() == program)
			return i;
	}

	return -1;
}

void AnimaRenderer::BuildProgramsData(AnimaArray<AnimaRendererProgramData>* programs, AnimaCamera* camera)
{
	if (programs == nullptr || camera == nullptr || _scene == nullptr)
		return;

//	AnimaFrustum* frustum = camera->GetFrustum();
	AnimaGeometriesManager* geometriesManager = _scene->GetGeometriesManager();

	AInt geometriesCount = geometriesManager->GetGeometriesCount();
	for (AInt i = 0; i < geometriesCount; i++)
	{
		AnimaGeometry* geometry = geometriesManager->GetGeometry(i);

		AInt instancesCount = geometry->GetInstancesCount();
		for (AInt j = 0; j < instancesCount; j++)
		{
			AnimaGeometryInstance* instance = geometry->GetInstance(j);

			//AnimaTransformation* instanceTransfomation = instance->GetTransformation();
			//if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * geometry->GetBoundingBoxCenter(), (geometry->GetBoundingBoxMin() - geometry->GetBoundingBoxMax()).Length()))
			//	continue;

			AnimaShaderProgram* program = instance->GetShaderProgram();
			if (program == nullptr)
				continue;

			AnimaMaterial* material = instance->GetMaterial();
			if (material == nullptr)
				material = AnimaMaterialsManager::GetDefaultMaterial();

			AnimaGeometry* geometry = instance->GetGeometry();

			AInt programDataIndex = FindProgramData(programs, program);
			if (programDataIndex >= 0)
			{
				AnimaRendererProgramData* programData = &programs->at(programDataIndex);

				if (program->CanSupportInstance())
				{
					AInt geometryInstancesIndex = FindGeometryInstances(&programData->_geometries, geometry);
					if (geometryInstancesIndex >= 0)
					{
						AnimaRendererGeometryInstances* geometryInstances = &programData->_geometries.at(geometryInstancesIndex);
						geometryInstances->_instances.push_back(instance);
					}
					else
					{
						AnimaRendererGeometryInstances geometryInstances;
						geometryInstances._geometry = geometry;
						geometryInstances._instances.push_back(instance);

						programData->_geometries.push_back(geometryInstances);
					}
				}
				else
				{
					AInt materialInstancesIndex = FindMaterialInstances(&programData->_materials, material);
					if (materialInstancesIndex >= 0)
					{
						AnimaRendererMaterialInstances* materialInstances = &programData->_materials.at(materialInstancesIndex);
						materialInstances->_instances.push_back(instance);
					}
					else
					{
						AnimaRendererMaterialInstances materialInstances;
						materialInstances._material = material;
						materialInstances._instances.push_back(instance);

						programData->_materials.push_back(materialInstances);
					}
				}
			}
			else
			{
				AnimaRendererProgramData programData;
				programData._program = program;

				if (program->CanSupportInstance())
				{
					AnimaRendererGeometryInstances geometryInstances;
					geometryInstances._geometry = geometry;
					geometryInstances._instances.push_back(instance);

					programData._geometries.push_back(geometryInstances);
				}
				else
				{
					AnimaRendererMaterialInstances materialInstances;
					materialInstances._material = material;
					materialInstances._instances.push_back(instance);

					programData._materials.push_back(materialInstances);
				}

				programs->push_back(programData);
			}
		}
	}
}

AInt AnimaRenderer::FindProgramData(AnimaArray<AnimaRendererProgramData>* programs, AnimaShaderProgram* program)
{
	if (programs == nullptr || program == nullptr)
		return -1;

	AInt count = programs->size();
	for (AInt i = 0; i < count; i++)
	{
		if (programs->at(i)._program == program)
			return i;
	}

	return -1;
}

AInt AnimaRenderer::FindMaterialInstances(AnimaArray<AnimaRendererMaterialInstances>* materials, AnimaMaterial* material)
{
	if (materials == nullptr || material == nullptr)
		return -1;

	AInt count = materials->size();
	for (AInt i = 0; i < count; i++)
	{
		if (materials->at(i)._material == material)
			return i;
	}

	return -1;
}

AInt AnimaRenderer::FindGeometryInstances(AnimaArray<AnimaRendererGeometryInstances>* geometries, AnimaGeometry* geometry)
{
	if (geometries == nullptr || geometry == nullptr)
		return -1;

	AInt count = geometries->size();
	for (AInt i = 0; i < count; i++)
	{
		if (geometries->at(i)._geometry == geometry)
			return i;
	}

	return -1;
}

void AnimaRenderer::SetupProgramDataStaticBuffers(AnimaArray<AnimaRendererProgramData>* programs, AnimaCamera* camera)
{
	for (auto& programData : *programs)
	{
		AnimaShaderProgram* program = programData._program;

		if (program->CanSupportInstance())
			continue;

		AInt count = program->GetShaderStaticGroupDataCount();
		for (AInt i = 0; i < count; i++)
		{
			AnimaShaderGroupData* groupData = program->GetShaderStaticGroupData(i);
			AnimaShaderDataSourceObject sourceObject = groupData->GetSourceObject();

			groupData->BindForUpdate(_programsBufferIndex);
			
			if (sourceObject == ASDSO_MATERIAL)
			{
				AInt materialsCount = programData._materials.size();

				// Il numero dei materiali
				groupData->SetBufferLength(materialsCount);

				for (AInt j = 0; j < materialsCount; j++)
				{
					AnimaMaterial* material = programData._materials.at(j)._material;
					groupData->UpdateValue(material, this, program, j);
				}
			}
			else if (sourceObject == ASDSO_GEOMETRY)
			{
				AInt instancesCount = 0;

				for (auto& programMaterial : programData._materials)
					instancesCount += programMaterial._instances.size();

				if (instancesCount <= 0)
					continue;

				groupData->SetBufferLength(instancesCount);

				AInt instanceOffset = 0;
				for (auto& programMaterial : programData._materials)
				{
					for (auto& instance : programMaterial._instances)
					{
						groupData->UpdateValue(instance, this, program, instanceOffset++);
					}
				}
			}
			else if (sourceObject == ASDSO_CAMERA)
			{
				groupData->UpdateValue(camera, this, program, 0);
			}

			groupData->UnbindFromUpdate();
		}
	}
}

void AnimaRenderer::SetupProgramDataInstancedStaticBuffers(AnimaShaderProgram* program, AnimaRendererGeometryInstances* geometryInstances, AnimaCamera* camera)
{
	AInt count = program->GetShaderStaticGroupDataCount();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderGroupData* groupData = program->GetShaderStaticGroupData(i);
		AnimaShaderDataSourceObject sourceObject = groupData->GetSourceObject();
		
		ANIMA_FRAME_PUSH("bind for update");
		groupData->BindForUpdate(_programsBufferIndex);
		ANIMA_FRAME_POP();
		
		if (sourceObject == ASDSO_MATERIAL)
		{
			ANIMA_FRAME_PUSH("material update");
			AInt instancesCount = geometryInstances->_instances.size();
			for (AInt j = 0; j < instancesCount; j++)
			{
				AnimaGeometryInstance* instance = geometryInstances->_instances.at(j);

				AnimaMaterial* material = instance->GetMaterial();
				if (material == nullptr)
					material = AnimaMaterialsManager::GetDefaultMaterial();

				groupData->UpdateValue(material, this, program, j);
			}
			ANIMA_FRAME_POP();
		}
		else if (sourceObject == ASDSO_GEOMETRY)
		{
			ANIMA_FRAME_PUSH("node update");
			AInt instancesCount = geometryInstances->_instances.size();
			for (AInt j = 0; j < instancesCount; j++)
			{
				AnimaGeometryInstance* instance = geometryInstances->_instances.at(j);
				groupData->UpdateValue(instance, this, program, j);
			}
			ANIMA_FRAME_POP();
		}
		else if (sourceObject == ASDSO_CAMERA)
		{
			ANIMA_FRAME_PUSH("camera update");
			groupData->UpdateValue(camera, this, program, 0);
			ANIMA_FRAME_POP();
		}
		
		ANIMA_FRAME_PUSH("unbind from update");
		groupData->UnbindFromUpdate();
		ANIMA_FRAME_POP();
	}
}

void AnimaRenderer::PreparePass(AnimaRenderer* renderer)
{
	ANIMA_FRAME_PUSH("Prepare pass");

	AnimaCamerasManager* camerasManager = renderer->_scene->GetCamerasManager();

	AnimaCamera* camera = camerasManager->GetActiveCamera();
	if (camera == nullptr)
		return;

	AnimaVertex4f backColor = renderer->GetColor4f("BackColor");

	renderer->GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	renderer->Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	ANIMA_FRAME_PUSH("Build data");
	AnimaArray<AnimaRendererProgramData> programs;
	renderer->BuildProgramsData(&programs, camera);
	renderer->SetupProgramDataStaticBuffers(&programs, camera);
	ANIMA_FRAME_POP();

	for (auto& programData : programs)
	{
		AnimaShaderProgram* program = programData._program;
		bool supportsInstance = program->CanSupportInstance();

		AnimaArray<AnimaShaderGroupData*> materialDataGroups;
		AnimaArray<AnimaShaderGroupData*> geometryDataGroups;
		AnimaArray<AnimaShaderGroupData*> cameraDataGroups;

		AInt staticGroupDataCount = program->GetShaderStaticGroupDataCount();
		for (AInt i = 0; i < staticGroupDataCount; i++)
		{
			AnimaShaderGroupData* groupData = program->GetShaderStaticGroupData(i);
			AnimaShaderDataSourceObject sourceObject = groupData->GetSourceObject();

			if (sourceObject == ASDSO_MATERIAL)
				materialDataGroups.push_back(groupData);
			else if (sourceObject == ASDSO_GEOMETRY)
				geometryDataGroups.push_back(groupData);
			else if (sourceObject == ASDSO_CAMERA)
				cameraDataGroups.push_back(groupData);
		}

		if (supportsInstance)
		{
			renderer->_programsBufferIndex = 0;

			for (auto& programGeometry : programData._geometries)
			{
				ANIMA_FRAME_PUSH("SetupProgramDataInstancedStaticBuffers");
				renderer->SetupProgramDataInstancedStaticBuffers(program, &programGeometry, camera);
				ANIMA_FRAME_POP();
		
				program->Use();

				ANIMA_FRAME_PUSH("Update normal uniforms");
				program->UpdateSceneObjectProperties(camera, renderer);
				program->UpdateRenderingManagerProperies(renderer);
				program->UpdateMappedValuesObjectProperties(programGeometry._instances[0]->GetMaterial(), renderer);
				ANIMA_FRAME_POP();

				ANIMA_FRAME_PUSH("Enable UBO");
				for (auto& group : materialDataGroups)
					group->Enable(renderer->_programsBufferIndex);
				for (auto& group : geometryDataGroups)
					group->Enable(renderer->_programsBufferIndex);
				ANIMA_FRAME_POP();

				AnimaGeometry* geometry = programGeometry._geometry;
				if (geometry->NeedsBuffersUpdate())
					geometry->UpdateBuffers();
				
				ANIMA_FRAME_PUSH("Draw");
#if defined USE_VAOS
				glBindVertexArray(geometry->GetVertexArrayObject());
#else
				program->EnableInputs(geometry);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->GetIndexesBufferObject());
#endif
				glDrawElementsInstanced(GL_TRIANGLES, geometry->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0, programGeometry._instances.size());
#if !defined USE_VAOS
				program->DisableInputs();
#endif
				ANIMA_FRAME_POP();
				
				program->SyncBuffers(renderer->_programsBufferIndex);
				renderer->_programsBufferIndex = (renderer->_programsBufferIndex + 1) % AnimaShaderGroupData::GetMaxBuffersCount();
			}
		}
		else
		{
			program->Use();
			program->UpdateSceneObjectProperties(camera, renderer);
			program->UpdateRenderingManagerProperies(renderer);
			
			AInt materialsOffset = 0;
			AInt instancesOffset = 0;
			renderer->_programsBufferIndex = 0;

			for (auto& programMaterial : programData._materials)
			{
				for (auto& group : materialDataGroups)
					group->EnableValue(materialsOffset, renderer->_programsBufferIndex);

				for (auto& instance : programMaterial._instances)
				{
					AnimaGeometry* geometry = instance->GetGeometry();
					if (geometry->NeedsBuffersUpdate())
						geometry->UpdateBuffers();

					for (auto& group : geometryDataGroups)
						group->EnableValue(instancesOffset, renderer->_programsBufferIndex);

					program->UpdateMappedValuesObjectProperties(programMaterial._material, renderer);
					program->UpdateSceneObjectProperties(instance, renderer);
					
#if defined USE_VAOS
					glBindVertexArray(geometry->GetVertexArrayObject());
#else
					program->EnableInputs(geometry);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->GetIndexesBufferObject());
#endif
					glDrawElements(GL_TRIANGLES, geometry->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
#if !defined USE_VAOS
					program->DisableInputs();
#endif

					program->SyncBuffers(renderer->_programsBufferIndex);

					renderer->_programsBufferIndex = (renderer->_programsBufferIndex + 1) % AnimaShaderGroupData::GetMaxBuffersCount();

					instancesOffset++;
				}

				materialsOffset++;
			}
		}
	}

	glDisable(GL_STENCIL_TEST);

	renderer->Finish();
	ANIMA_FRAME_POP();
}

void AnimaRenderer::LightPass(AnimaRenderer* renderer)
{
	AnimaCamera* activeCamera = renderer->_scene->GetCamerasManager()->GetActiveCamera();
	if (activeCamera == nullptr)
		return;
	
	AnimaLightsManager* lightsManager = renderer->_scene->GetLightsManager();
	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaStringHasher>* lightsMap = lights->GetArraysMap();
	
	// Pulisco i buffer per le luci
	AnimaGBuffer* lightsBuffrer = renderer->GetGBuffer("LightsBuffer");
	lightsBuffrer->BindAsRenderTarget();
	renderer->Start();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	ANIMA_FRAME_PUSH("Stencil copy");
	AnimaGBuffer* prepassBuffer = renderer->GetGBuffer("PrepassBuffer");
	prepassBuffer->BindAsReadingSource();
	glBlitFramebuffer(0, 0, prepassBuffer->GetWidth(), prepassBuffer->GetHeight(), 0, 0, lightsBuffrer->GetWidth(), lightsBuffrer->GetHeight(), GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	ANIMA_FRAME_POP()
	
	for (auto pair : (*lightsMap))
	{
		AnimaArray<AnimaLight*>* lightsArray = pair.second->GetArray();
		AInt size = lightsArray->size();
		if (size == 0)
			continue;
		
		if (lightsArray->at(0)->IsOfClass(ANIMA_CLASS_NAME(AnimaDirectionalLight)))
			renderer->DirectionalLightsPass(lightsArray);
		else if (lightsArray->at(0)->IsOfClass(ANIMA_CLASS_NAME(AnimaPointLight)))
			renderer->PointLightsPass(lightsArray);
	}
	
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_ONE, GL_ZERO);
	
	renderer->Finish();
}

void AnimaRenderer::DirectionalLightsPass(AnimaArray<AnimaLight*>* directionalLights)
{
	ANIMA_FRAME_PUSH("DirectionalLightsPass");

	AnimaShadersManager* shadersManager = _engine->GetShadersManager();
	AnimaCamerasManager* camerasManager = _scene->GetCamerasManager();
	
	AnimaString type = ANIMA_CLASS_NAME(AnimaDirectionalLight);
	auto lightGeometryPair = _geometriesMap.find(type);
	
	ANIMA_ASSERT(lightGeometryPair != _geometriesMap.end());
	ANIMA_ASSERT(directionalLights->at(0)->CreateShader(shadersManager));
	
	AnimaShaderProgram* program = shadersManager->GetProgramFromName(directionalLights->at(0)->GetShaderName());
	AnimaGeometry* geometry = lightGeometryPair->second;
	
	AnimaShaderProgram* activeProgram = nullptr;
	AnimaCamera* activeCamera = camerasManager->GetActiveCamera();
	
	AInt count = directionalLights->size();
	for (AInt i = 0; i < count; i++)
	{
		AnimaDirectionalLight* light = (AnimaDirectionalLight*)directionalLights->at(i);
		
		if(light->GetBoolean("CastShadows"))
			UpdateDirectionalLightShadowMap(light);
				
		// Attivo il buffer delle luci e lo imposto che potrebbe essere stato cambiato da UpdateDirectionalLights
		GetGBuffer("LightsBuffer")->BindAsRenderTarget();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_EQUAL, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		
		activeProgram = shadersManager->GetActiveProgram();
		if (activeProgram == nullptr || (*activeProgram) != (*program))
		{
			program->Use();
			program->UpdateSceneObjectProperties(activeCamera, this);
			program->UpdateRenderingManagerProperies(this);
		}
		
		if (geometry->NeedsBuffersUpdate())
			geometry->UpdateBuffers();
		
		light->UpdateCullFace(activeCamera);
		
		program->UpdateSceneObjectProperties(light, this);
		
		geometry->Draw(this, program, true, true, false);

		glDisable(GL_STENCIL_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	ANIMA_FRAME_POP();
}

void AnimaRenderer::PointLightsPass(AnimaArray<AnimaLight*>* pointLights)
{
	AnimaShadersManager* shadersManager = _engine->GetShadersManager();
	AnimaCamerasManager* camerasManager = _scene->GetCamerasManager();

	AnimaString type = ANIMA_CLASS_NAME(AnimaPointLight);
	auto lightGeometryPair = _geometriesMap.find(type);

	ANIMA_ASSERT(lightGeometryPair != _geometriesMap.end());
	ANIMA_ASSERT(pointLights->at(0)->CreateShader(shadersManager));

	AnimaShaderProgram* program = shadersManager->GetProgramFromName(pointLights->at(0)->GetShaderName());
	AnimaGeometry* geometry = lightGeometryPair->second;

	AnimaShaderProgram* activeProgram = nullptr;
	AnimaCamera* activeCamera = camerasManager->GetActiveCamera();

	AInt count = pointLights->size();
	for (AInt i = 0; i < count; i++)
	{
		AnimaPointLight* light = (AnimaPointLight*)pointLights->at(i);

		//if (light->GetBoolean("CastShadows"))
		//	UpdateDirectionalLightShadowMap(light);

		// Attivo il buffer delle luci e lo imposto che potrebbe essere stato cambiato da UpdateDirectionalLights
		GetGBuffer("LightsBuffer")->BindAsRenderTarget();
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);

		activeProgram = shadersManager->GetActiveProgram();
		if (activeProgram == nullptr || (*activeProgram) != (*program))
		{
			program->Use();
			program->UpdateSceneObjectProperties(activeCamera, this);
			program->UpdateRenderingManagerProperies(this);
		}

		if (geometry->NeedsBuffersUpdate())
			geometry->UpdateBuffers();

		light->UpdateCullFace(activeCamera);

		program->UpdateSceneObjectProperties(light, this);

		geometry->Draw(this, program, true, true, false);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}

void AnimaRenderer::DirectionalLightShadowMap(AnimaRenderer* renderer)
{
	ANIMA_FRAME_PUSH("DirectionalLightsPass");
	
	AnimaLightsManager* lightsManager = renderer->_scene->GetLightsManager();
	
	AnimaString type = ANIMA_CLASS_NAME(AnimaDirectionalLight);
	auto lightGeometryPair = renderer->_geometriesMap.find(type);
	
	ANIMA_ASSERT(lightGeometryPair != renderer->_geometriesMap.end());
	AnimaDirectionalLight* light = (AnimaDirectionalLight*)lightsManager->GetLightFromName("light-0");
	
	if(light->GetBoolean("CastShadows"))
		renderer->UpdateDirectionalLightShadowMap(light);
	
	ANIMA_FRAME_POP();
}

void AnimaRenderer::BuildShadowMapGeometries(AnimaArray<AnimaRendererGeometryInstances>* geometries)
{
	if (geometries == nullptr || _scene == nullptr)
		return;

	AnimaGeometriesManager* geometriesManager = _scene->GetGeometriesManager();

	AInt geometriesCount = geometriesManager->GetGeometriesCount();
	for (AInt i = 0; i < geometriesCount; i++)
	{
		AnimaGeometry* geometry = geometriesManager->GetGeometry(i);

		AInt instancesCount = geometry->GetInstancesCount();
		for (AInt j = 0; j < instancesCount; j++)
		{
			AnimaGeometryInstance* instance = geometry->GetInstance(j);

			//if (instance->GetBoolean("CastShadows"))
			//{
				AnimaGeometry* geometry = instance->GetGeometry();

				AInt geometryInstancesIndex = FindGeometryInstances(geometries, geometry);
				if (geometryInstancesIndex >= 0)
				{
					AnimaRendererGeometryInstances* geometryInstances = &geometries->at(geometryInstancesIndex);
					geometryInstances->_instances.push_back(instance);
				}
				else
				{
					AnimaRendererGeometryInstances geometryInstances;
					geometryInstances._geometry = geometry;
					geometryInstances._instances.push_back(instance);

					geometries->push_back(geometryInstances);
				}
			//}
		}
	}
}

void AnimaRenderer::SetupShadowMapStaticBuffers(AnimaShaderProgram* program, AnimaArray<AnimaRendererGeometryInstances>* geometries, AnimaLight* light)
{
	if (program->CanSupportInstance())
		return;

	AInt count = program->GetShaderStaticGroupDataCount();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderGroupData* groupData = program->GetShaderStaticGroupData(i);
		AnimaShaderDataSourceObject sourceObject = groupData->GetSourceObject();

		groupData->BindForUpdate(_programsBufferIndex);

		if (sourceObject == ASDSO_GEOMETRY)
		{
			AInt instancesCount = 0;

			for (auto& geometryInstances : (*geometries))
				instancesCount += geometryInstances._instances.size();

			if (instancesCount <= 0)
				continue;

			groupData->SetBufferLength(instancesCount);

			AInt instanceOffset = 0;
			for (auto& geometryInstances : (*geometries))
			{
				for (auto& instance : geometryInstances._instances)
				{
					groupData->UpdateValue(instance, this, program, instanceOffset++);
				}
			}
		}
		if (sourceObject == ASDSO_LIGHT)
		{
			ANIMA_FRAME_PUSH("light update");
			groupData->UpdateValue(light, this, program, 0);
			ANIMA_FRAME_POP();
		}

		groupData->UnbindFromUpdate();
	}
}

void AnimaRenderer::SetupShadowMapInstancedStaticBuffers(AnimaShaderProgram* program, AnimaRendererGeometryInstances* geometryInstances, AnimaLight* light)
{
	AInt count = program->GetShaderStaticGroupDataCount();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderGroupData* groupData = program->GetShaderStaticGroupData(i);
		AnimaShaderDataSourceObject sourceObject = groupData->GetSourceObject();

		ANIMA_FRAME_PUSH("bind for update");
		groupData->BindForUpdate(_programsBufferIndex);
		ANIMA_FRAME_POP();

		if (sourceObject == ASDSO_GEOMETRY)
		{
			ANIMA_FRAME_PUSH("node update");
			AInt instancesCount = geometryInstances->_instances.size();
			for (AInt j = 0; j < instancesCount; j++)
			{
				AnimaGeometryInstance* instance = geometryInstances->_instances.at(j);
				groupData->UpdateValue(instance, this, program, j);
			}
			ANIMA_FRAME_POP();
		}
		else if (sourceObject == ASDSO_LIGHT)
		{
			ANIMA_FRAME_PUSH("light update");
			groupData->UpdateValue(light, this, program, 0);
			ANIMA_FRAME_POP();
		}

		ANIMA_FRAME_PUSH("unbind from update");
		groupData->UnbindFromUpdate();
		ANIMA_FRAME_POP();
	}
}

void AnimaRenderer::UpdateShadowMap(AnimaLight* light)
{
	if(light->IsOfClass(ANIMA_CLASS_NAME(AnimaDirectionalLight)))
	{
		UpdateDirectionalLightShadowMap((AnimaDirectionalLight*)light);
	}
}

void AnimaRenderer::UpdateDirectionalLightShadowMap(AnimaDirectionalLight* light)
{
	ANIMA_FRAME_PUSH("UpdateDirectionalLightShadowMap");

	AnimaShadersManager* shadersManager = _engine->GetShadersManager();
	
	AnimaMatrix m = light->GetMatrix("ProjectionViewMatrix");
	
	AnimaTexture* shadowMapTemp = GetTexture("DILShadowMapTemp");
	AnimaTexture* shadowMap = GetTexture("DILShadowMap");
	AnimaShaderProgram* program = shadersManager->GetProgramFromName("dil-shadow-map");
	
	shadowMap->BindAsRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	ANIMA_FRAME_PUSH("Build data");
	AnimaArray<AnimaRendererGeometryInstances> geometryInstances;
	BuildShadowMapGeometries(&geometryInstances);
	SetupShadowMapStaticBuffers(program, &geometryInstances, light);
	ANIMA_FRAME_POP();
	
	bool supportsInstance = program->CanSupportInstance();

	AnimaArray<AnimaShaderGroupData*> geometryDataGroups;
	AnimaArray<AnimaShaderGroupData*> lightDataGroups;

	AInt staticGroupDataCount = program->GetShaderStaticGroupDataCount();
	for (AInt i = 0; i < staticGroupDataCount; i++)
	{
		AnimaShaderGroupData* groupData = program->GetShaderStaticGroupData(i);
		AnimaShaderDataSourceObject sourceObject = groupData->GetSourceObject();

		if (sourceObject == ASDSO_GEOMETRY)
			geometryDataGroups.push_back(groupData);
		else if (sourceObject == ASDSO_LIGHT)
			lightDataGroups.push_back(groupData);
	}

	program->Use();

	ANIMA_FRAME_PUSH("Update normal uniforms");
	program->UpdateRenderingManagerProperies(this);
	program->UpdateSceneObjectProperties(light, this);
	ANIMA_FRAME_POP();

	if (supportsInstance)
	{
		_programsBufferIndex = 0;
		for (auto& programGeometry : geometryInstances)
		{
			ANIMA_FRAME_PUSH("SetupShadowMapInstancedStaticBuffers");
			SetupShadowMapInstancedStaticBuffers(program, &programGeometry, light);
			ANIMA_FRAME_POP();
			
			ANIMA_FRAME_PUSH("Enable UBO");
			for (auto& group : geometryDataGroups)
				group->Enable(_programsBufferIndex);
			for (auto& group : lightDataGroups)
				group->Enable(_programsBufferIndex);
			ANIMA_FRAME_POP();

			AnimaGeometry* geometry = programGeometry._geometry;
			if (geometry->NeedsBuffersUpdate())
				geometry->UpdateBuffers();

			ANIMA_FRAME_PUSH("Draw");
#if defined USE_VAOS
			glBindVertexArray(geometry->GetVertexArrayObject());
#else
			program->EnableInputs(geometry);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->GetIndexesBufferObject());
#endif
			glDrawElementsInstanced(GL_TRIANGLES, geometry->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0, programGeometry._instances.size());
#if !defined USE_VAOS
			program->DisableInputs();
#endif
			ANIMA_FRAME_POP();

			program->SyncBuffers(_programsBufferIndex);
			_programsBufferIndex = (_programsBufferIndex + 1) % AnimaShaderGroupData::GetMaxBuffersCount();
		}
	}
	else
	{
		AInt instancesOffset = 0;
		_programsBufferIndex = 0;

		for (auto& programGeometry : geometryInstances)
		{
			for (auto& instance : programGeometry._instances)
			{
				AnimaGeometry* geometry = instance->GetGeometry();
				if (geometry->NeedsBuffersUpdate())
					geometry->UpdateBuffers();

				for (auto& group : geometryDataGroups)
					group->EnableValue(instancesOffset, _programsBufferIndex);

				for (auto& group : lightDataGroups)
					group->Enable(_programsBufferIndex);

				program->UpdateSceneObjectProperties(instance, this);

#if defined USE_VAOS
				glBindVertexArray(geometry->GetVertexArrayObject());
#else
				program->EnableInputs(geometry);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->GetIndexesBufferObject());
#endif
				glDrawElements(GL_TRIANGLES, geometry->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
#if !defined USE_VAOS
				program->DisableInputs();
#endif

				program->SyncBuffers(_programsBufferIndex);
				_programsBufferIndex = (_programsBufferIndex + 1) % AnimaShaderGroupData::GetMaxBuffersCount();

				instancesOffset++;
			}
		}
	}

	AFloat b = GetFloat("BlurSize");
	SetVector("BlurScale", b, 0.0);
	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussianFilter"), shadowMap, shadowMapTemp);
	SetVector("BlurScale", 0.0, b);
	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussianFilter"), shadowMapTemp, shadowMap);

	ANIMA_FRAME_POP();
}

void AnimaRenderer::CombinePass(AnimaRenderer* renderer)
{
	AnimaVertex4f backColor = renderer->GetColor4f("BackColor");
	Anima::AnimaShadersManager* shadersManager = renderer->_scene->GetShadersManager();
	
	renderer->Start();

	AnimaTexture* diffuseMap = renderer->GetTexture("DiffuseMap");
	diffuseMap->BindAsRenderTarget();
	AnimaVertex2f size(diffuseMap->GetWidth(), diffuseMap->GetHeight());

	//AnimaVertex2f size = renderer->GetVector2f("ScreenSize");
	//glViewport(0, 0, (AUint)size.x, (AUint)size.y);
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);

	ANIMA_FRAME_PUSH("Stencil copy");
	AnimaGBuffer* prepassBuffer = renderer->GetGBuffer("PrepassBuffer");
	prepassBuffer->BindAsReadingSource();
	glBlitFramebuffer(0, 0, prepassBuffer->GetWidth(), prepassBuffer->GetHeight(), 0, 0, (AUint)size.x, (AUint)size.y, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	ANIMA_FRAME_POP();
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	AnimaShaderProgram* activeProgram = renderer->_scene->GetShadersManager()->GetActiveProgram();
	AnimaShaderProgram* program = shadersManager->GetProgramFromName("deferred-combine");

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateSceneObjectProperties(renderer->_filterCamera, renderer);
	}

	if (renderer->_filterGeometry->NeedsBuffersUpdate())
		renderer->_filterGeometry->UpdateBuffers();

	program->UpdateRenderingManagerProperies(renderer);
	renderer->_filterGeometry->Draw(renderer, program, true, true, false);

	AnimaTexture* skyTexture = renderer->_scene->GetTexture("SkyBox");
	if (skyTexture != nullptr)
	{
		AnimaGeometry* skyGeometry = renderer->_geometriesMap["skybox_RENGEOMETRY"];
		if (skyGeometry != nullptr)
		{
			AnimaShaderProgram* skyProgram = shadersManager->GetProgramFromName("skybox");
			AnimaCamera* camera = renderer->_scene->GetCamerasManager()->GetActiveCamera();

			skyProgram->Use();
			skyProgram->UpdateSceneObjectProperties(camera, renderer);
			
			if (skyGeometry->NeedsBuffersUpdate())
				skyGeometry->UpdateBuffers();

			skyGeometry->GetTransformation()->SetTranslation(camera->GetPosition());

			skyProgram->UpdateMappedValuesObjectProperties(renderer->_scene, renderer);
			skyProgram->UpdateRenderingManagerProperies(renderer);
						
			glStencilFunc(GL_EQUAL, 0, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			glCullFace(GL_FRONT);
			glDisable(GL_DEPTH_TEST);
			skyGeometry->Draw(renderer, skyProgram, true, true, true);
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
		}
	}

	glDisable(GL_STENCIL_TEST);
	
	if(renderer->IsPhysicsDebugDrawingEnabled())
	{
		ANIMA_FRAME_PUSH("Depth copy");
		prepassBuffer->BindAsReadingSource();
		glBlitFramebuffer(0, 0, prepassBuffer->GetWidth(), prepassBuffer->GetHeight(), 0, 0, (AUint)size.x, (AUint)size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		ANIMA_FRAME_POP();
	
		// Disegno il debug di Bullet
		renderer->_scene->GetPhysWorld()->debugDrawWorld();
		renderer->_physicsDebugDrawer->DrawDebugScene(renderer->_scene);
	}

	renderer->Finish();
}

void AnimaRenderer::BloomCreationPass(AnimaRenderer* renderer)
{
	Anima::AnimaShadersManager* shadersManager = renderer->_scene->GetShadersManager();
	
	AnimaTexture* diffuseMap = renderer->GetTexture("DiffuseMap");
	AnimaTexture* bloomMap = renderer->GetTexture("BloomMap");
	AnimaTexture* bloomMapTemp = renderer->GetTexture("BloomMapTemp");

	renderer->ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("brightnessFilter"), diffuseMap, bloomMap);

	AFloat b = renderer->GetFloat("BloomBlurScale");

	for (int i = 0; i < 5; i++)
	{
		renderer->SetVector("BlurScale", b, 0.0);
		renderer->ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussianFilter"), bloomMap, bloomMapTemp);
		renderer->SetVector("BlurScale", 0.0, b);
		renderer->ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussianFilter"), bloomMapTemp, bloomMap);
	}
}

void AnimaRenderer::FinalPass(AnimaRenderer* renderer)
{
	AnimaVertex4f backColor = renderer->GetColor4f("BackColor");
	Anima::AnimaShadersManager* shadersManager = renderer->_scene->GetShadersManager();

	renderer->Start();

	bool applyFxaa = renderer->GetBoolean("ApplyFXAA");
	
	if(applyFxaa)
	{
		renderer->GetTexture("FinalMap")->BindAsRenderTarget();
	}
	else
	{
		AnimaVertex2f size = renderer->GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);

	AnimaShaderProgram* activeProgram = renderer->_scene->GetShadersManager()->GetActiveProgram();
	AnimaShaderProgram* program = shadersManager->GetProgramFromName("final");

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateSceneObjectProperties(renderer->_filterCamera, renderer);
	}

	if (renderer->_filterGeometry->NeedsBuffersUpdate())
		renderer->_filterGeometry->UpdateBuffers();

	program->UpdateRenderingManagerProperies(renderer);
	renderer->_filterGeometry->Draw(renderer, program, true, true, false);
	
	if(applyFxaa)
	{
		renderer->ApplyEffectFromTextureToTexture(renderer->_scene->GetShadersManager()->GetProgramFromName("fxaaFilter"), renderer->GetTexture("FinalMap"), nullptr);
	}
	renderer->Finish();
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program)
{
	//AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	//AnimaGeometriesManager* geometriesManager = _scene->GetGeometriesManager();

	//AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	//if (lights == nullptr)
	//	return;

	//AInt nLights = lights->size();
	//AInt nGeometries = geometriesManager->GetGeometriesCount();

	//if (nLights == 0 || nGeometries == 0)
	//	return;

	//AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	//if (activeProgram == nullptr || (*activeProgram) != (*program))
	//	program->Use();
	//
	//for (AInt i = 0; i < nLights; i++)
	//{
	//	AnimaLight* light = lights->at(i);
	//	AnimaTexture* shadowMap = light->GetShadowTexture();
	//	if (!shadowMap->AreRenderTargetsReady())
	//		shadowMap->LoadRenderTargets();

	//	shadowMap->BindAsRenderTarget();
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glClearColor(0.0, 0.0, 0.0, 0.0);
	//	glEnable(GL_DEPTH_TEST);
	//	glDepthMask(GL_TRUE);

	//	program->UpdateSceneObjectProperties(light, this);
	//	program->UpdateRenderingManagerProperies(this);

	//	for (AInt j = 0; j < nGeometries; j++)
	//	{
	//		DrawGeometry(geometriesManager->GetGeometry(j), program, false, true, nullptr, true);
	//		//	AnimaGeometry* innerNode = nodesManager->GetNode(j);
	//		//	AnimaMatrix nodeMatrix = innerNode->GetTransformation()->GetTransformationMatrix();
	//		//	
	//		//	glCullFace(GL_FRONT);
	//		//	DrawNodeGeometry(_scene, innerNode, program, nodeMatrix, false, true);
	//		//	glCullFace(GL_BACK);
	//		//	
	//		//	AInt geometryCount = innerNode->GetGeometriesCount();
	//		//	for (AInt i = 0; i < geometryCount; i++)
	//		//	{
	//		//		glCullFace(GL_FRONT);
	//		//		DrawNodeGeometry(_scene, innerNode->GetGeometry(i), program, nodeMatrix, false, true);
	//		//		glCullFace(GL_BACK);
	//		//	}
	//		//	
	//		//	AInt childrenCount = innerNode->GetChildrenCount();
	//		//	for (AInt i = 0; i < childrenCount; i++)
	//		//		DrawNode(_scene, innerNode->GetChild(i), program, nodeMatrix, false, true);
	//	}
	//}
}

void AnimaRenderer::UpdateNodesVisibility()
{
//	Anima::AnimaNodesManager* nodesManager = _scene->GetNodesManager();
//	Anima::AnimaCamerasManager* camerasManager = _scene->GetCamerasManager();
//
//	Anima::AnimaFrustum* frustum = camerasManager->GetActiveCamera()->GetFrustum();

	//	for (int i = 0; i < nodesManager->GetNodesCount(); i++)
	//		UpdateNodeVisibility(frustum, nodesManager->GetNode(i), Anima::AnimaMatrix());
}

void AnimaRenderer::UpdateNodeVisibility(AnimaFrustum* frustum, AnimaGeometry* geometry, AnimaMatrix parentGeometryMatrix)
{
	AnimaMatrix nodeMatrix = parentGeometryMatrix * geometry->GetTransformation()->GetTransformationMatrix();

	if (geometry->GetVerticesCount() > 0)
	{
		if (frustum->SphereInFrustum(parentGeometryMatrix * geometry->GetBoundingBoxCenter(), (geometry->GetBoundingBoxMin() - geometry->GetBoundingBoxMax()).Length()))
			geometry->SetIsVisible(true);
		else
		{
			geometry->SetIsVisible(false);
			return;
		}
	}

	//AInt geometryCount = geometry->GetGeometriesCount();
	//for (AInt i = 0; i < geometryCount; i++)
	//{
	//	AnimaGeometry* subGeometry = geometry->GetGeometry(i);
	//	subGeometry->SetIsVisible(frustum->SphereInFrustum(parentGeometryMatrix * geometry->GetBoundingBoxCenter(), (geometry->GetBoundingBoxMin() - geometry->GetBoundingBoxMax()).Length()));
	//}

	//AInt childrenCount = geometry->GetChildrenCount();
	//for (AInt i = 0; i < childrenCount; i++)
	//	UpdateNodeVisibility(frustum, geometry->GetChild(i), nodeMatrix);
}

//void AnimaRenderer::AddPrimitive(AnimaArray<AnimaVertex3f>* vertices, AnimaArray<AUint>* indices, AnimaColor4f color, AnimaMatrix nodeMatrix, AUint primitiveType)
//{
//	if (vertices == nullptr)
//		return;
//	AnimaPrimitiveData* primitive = AnimaAllocatorNamespace::AllocateNew<AnimaPrimitiveData>(*_allocator, _allocator);
//	primitive->SetVertices(vertices);
//	primitive->SetIndices(indices);
//	primitive->SetColor(color);
//	primitive->SetType(primitiveType);
//	primitive->SetNodeMatrix(nodeMatrix);
//	
//	_primitives.push_back(primitive);
//}
//
//void AnimaRenderer::DrawPrimitives( AnimaShaderProgram* program)
//{
//	if(program == nullptr)
//		return;
//	
//	program->Use();
//	program->SetUniform("_projectionViewMatrix", _scene->GetCamerasManager()->GetActiveCamera()->GetProjectionViewMatrix());
//	
//	AInt nPrimitives = _primitives.size();
//	for(AInt i = 0; i < nPrimitives; i++)
//		DrawPrimitive(_primitives[i], program);
//}
//
//void AnimaRenderer::DrawPrimitive(AnimaPrimitiveData* primitive, AnimaShaderProgram* program)
//{
//	if(primitive == nullptr)
//		return;
//	
//	AnimaColor4f color = primitive->GetColor();
//	AnimaArray<AnimaVertex3f>* vertices = primitive->GetVertices();
//	AnimaArray<AUint>* indices = primitive->GetIndices();
//	AUint type = primitive->GetType();
//	AnimaMatrix nodeMatrix = primitive->GetNodeMatrix();
//	
//	if (color.w < 1.0f)
//		glEnable(GL_BLEND);
//	
//	program->SetUniform("_color", color);
//	program->SetUniform("_nodeMatrix", nodeMatrix);
//	
//	AInt nVertices = vertices->size();
//	AInt nCoordinate = nVertices * 3;
//	AFloat* coordinate = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, (ASizeT)nCoordinate);
//	
//	AInt offset = 0;
//	for (AInt i = 0; i < nVertices; i++)
//	{
//		coordinate[offset++] = (*vertices)[i].x;
//		coordinate[offset++] = (*vertices)[i].y;
//		coordinate[offset++] = (*vertices)[i].z;
//	}
//	
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _verticesBufferObject);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AFloat) * nCoordinate, coordinate, GL_STATIC_DRAW);
//	AnimaAllocatorNamespace::DeallocateArray(*_allocator, coordinate);
//	coordinate = nullptr;
//	
//	if (indices->size() > 0)
//	{
//		AInt nIndici = indices->size();
//		AUint* indici = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, (ASizeT)nIndici);
//		
//		for (AInt i = 0; i < nIndici; i++)
//			indici[i] = (*indices)[i];
//		
//		glBindVertexArray(_vertexArrayObject);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesBufferObject);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AUint) * nIndici, indici, GL_STATIC_DRAW);
//		AnimaAllocatorNamespace::DeallocateArray(*_allocator, indici);
//		indici = nullptr;
//		
//		program->EnableInput("_position", 3, GL_FLOAT, _verticesBufferObject);
//		glDrawElements(type, nIndici, GL_UNSIGNED_INT, 0);
//	}
//	else
//	{
//		glBindVertexArray(_vertexArrayObject);
//		program->EnableInput("_position", 3, GL_FLOAT, _verticesBufferObject);
//		glDrawArrays(type, 0, nCoordinate);
//	}
//	
//	glDisable(GL_BLEND);
//}
//
//void AnimaRenderer::CombinePrimitives(AnimaShaderProgram* program)
//{
//	program->Use();
//	program->UpdateRenderingManagerProperies(this);
//	if(glGetError() != GL_NO_ERROR)
//		return;
//	
//	if (_filterGeometry->NeedsBuffersUpdate())
//		_filterGeometry->UpdateBuffers();
//	
//	program->Use();
//	program->UpdateSceneObjectProperties(_filterCamera, this);
//	program->UpdateRenderingManagerProperies(this);
//
//	_filterGeometry->Draw(this, program, false);
//}

void AnimaRenderer::SetTexture(AnimaString propertyName, AnimaTexture* value, bool deleteExistent)
{
	auto pair = _texturesMap.find(propertyName);
	
	if (pair == _texturesMap.end())
		_texturesMap[propertyName] = value;
	else
	{
		if (pair->second != nullptr && deleteExistent)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair->second);
			pair->second = nullptr;
		}

		_texturesMap[propertyName] = value;
	}
}

void AnimaRenderer::SetTextureSlot(AnimaString propertyName, AUint value)
{
	_textureSlotsMap[propertyName] = value;
}

void AnimaRenderer::SetGBuffer(const AnimaString& name, AnimaGBuffer* value, bool deleteExistent)
{
	auto element = _gBuffersMap.find(name);

	if (element == _gBuffersMap.end())
		_gBuffersMap[name] = value;
	else
	{
		if (element->second != nullptr && deleteExistent)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, element->second);
			element->second = nullptr;
		}

		_gBuffersMap[name] = value;
	}
}

void AnimaRenderer::SetColor(AnimaString propertyName, AnimaColor3f value)
{
	_vectors3fMap[propertyName] = value;
}

void AnimaRenderer::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(r, g, b);
	SetColor(propertyName, color);
}

void AnimaRenderer::SetColor(AnimaString propertyName, AnimaColor4f value)
{
	_vectors4fMap[propertyName] = value;
}

void AnimaRenderer::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	SetColor(propertyName, color);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AnimaVertex2f value)
{
	_vectors2fMap[propertyName] = value;
}

void AnimaRenderer::SetVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex2f vector(x, y);
	SetVector(propertyName, vector);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AnimaVertex3f value)
{
	_vectors3fMap[propertyName] = value;
}

void AnimaRenderer::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex3f vector(x, y, z);
	SetVector(propertyName, vector);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AnimaVertex4f value)
{
	_vectors4fMap[propertyName] = value;
}

void AnimaRenderer::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	SetVector(propertyName, vector);
}

void AnimaRenderer::SetFloat(AnimaString propertyName, AFloat value)
{
	_floatsMap[propertyName] = value;
}

void AnimaRenderer::SetBoolean(AnimaString propertyName, bool value)
{
	_booleansMap[propertyName] = value;
}

void AnimaRenderer::SetInteger(AnimaString propertyName, AInt value)
{
	_integersMap[propertyName] = value;
}

AnimaTexture* AnimaRenderer::GetTexture(AnimaString propertyName)
{
	auto pair = _texturesMap.find(propertyName);
	if (pair != _texturesMap.end())
		return pair->second;
	return nullptr;
}

AUint AnimaRenderer::GetTextureSlot(const AnimaString& slotName)
{
	auto pair = _textureSlotsMap.find(slotName);
	if (pair != _textureSlotsMap.end())
		return pair->second;
	return 0;
}

AnimaGBuffer* AnimaRenderer::GetGBuffer(const AnimaString& gBufferName)
{
	auto pair = _gBuffersMap.find(gBufferName);
	if (pair != _gBuffersMap.end())
		return pair->second;
	return nullptr;
}

AnimaColor3f AnimaRenderer::GetColor3f(AnimaString propertyName)
{
	auto pair = _vectors3fMap.find(propertyName);
	if (pair != _vectors3fMap.end())
		return pair->second;

	AnimaColor3f color(0.0f, 0.0f, 0.0f);
	return color;
}

AnimaColor4f AnimaRenderer::GetColor4f(AnimaString propertyName)
{
	auto pair = _vectors4fMap.find(propertyName);
	if (pair != _vectors4fMap.end())
		return pair->second;

	AnimaColor4f color(0.0f, 0.0f, 0.0f, 1.0f);
	return color;
}

AnimaVertex2f AnimaRenderer::GetVector2f(AnimaString propertyName)
{
	auto pair = _vectors2fMap.find(propertyName);
	if (pair != _vectors2fMap.end())
		return pair->second;

	AnimaVertex2f vector(0.0f, 0.0f);
	return vector;
}

AnimaVertex3f AnimaRenderer::GetVector3f(AnimaString propertyName)
{
	auto pair = _vectors3fMap.find(propertyName);
	if (pair != _vectors3fMap.end())
		return pair->second;

	AnimaVertex3f vector(0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex4f AnimaRenderer::GetVector4f(AnimaString propertyName)
{
	auto pair = _vectors4fMap.find(propertyName);
	if (pair != _vectors4fMap.end())
		return pair->second;

	AnimaVertex4f vector(0.0f, 0.0f, 0.0f, 0.0f);
	return vector;
}

AFloat AnimaRenderer::GetFloat(AnimaString propertyName)
{
	auto pair = _floatsMap.find(propertyName);
	if (pair != _floatsMap.end())
		return pair->second;
	return 0.0;
}

bool AnimaRenderer::GetBoolean(AnimaString propertyName)
{
	auto pair = _booleansMap.find(propertyName);
	if (pair != _booleansMap.end())
		return pair->second;
	return false;
}

AInt AnimaRenderer::GetInteger(AnimaString propertyName)
{
	auto pair = _integersMap.find(propertyName);
	if (pair != _integersMap.end())
		return pair->second;
	return 0;
}

void AnimaRenderer::Clear()
{
	//glDeleteVertexArrays(1, &_vertexArrayObject);
	//glDeleteBuffers(1, &_verticesBufferObject);
	//glDeleteBuffers(1, &_indexesBufferObject);

	if (_filterCamera != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterCamera);
		_filterCamera = nullptr;
	}

	if (_filterGeometry != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterGeometry);
		_filterGeometry = nullptr;
	}
	
	for (auto& pair : _geometriesMap)
	{
		if (pair.second != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
			pair.second = nullptr;
		}
	}

	for (auto& pair : _texturesMap)
	{
		if (pair.second != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
			pair.second = nullptr;
		}
	}

	for (auto& pair : _gBuffersMap)
	{
		if (pair.second != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
			pair.second = nullptr;
		}
	}
	
	_texturesMap.clear();
	_textureSlotsMap.clear();
	_gBuffersMap.clear();

	_vectors2fMap.clear();
	_vectors3fMap.clear();
	_vectors4fMap.clear();

	_floatsMap.clear();
	_integersMap.clear();
	_booleansMap.clear();

	_geometriesMap.clear();
	
	//ClearPrimitives();

	_renderPassesFunction.clear();
}

//void AnimaRenderer::ClearPrimitives()
//{
//	AInt nPrimitives = _primitives.size();
//	for(AInt i = 0; i < nPrimitives; i++)
//	{
//		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _primitives[i]);
//		_primitives[i] = nullptr;
//	}
//	
//	_primitives.clear();
//}

bool AnimaRenderer::InitializeShaders()
{	
	AnimaString shadersPartsPath = SHADERS_PATH "Parts";
	AnimaString shadersIncludesPath = SHADERS_PATH "Includes";
	AnimaString shadersPath = SHADERS_PATH;
	
	AnimaShadersManager* shadersManager = _engine->GetShadersManager();
	shadersManager->LoadShadersIncludes(shadersIncludesPath);
	shadersManager->LoadShadersParts(shadersPartsPath);

	AnimaShaderProgram* prepareProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/static-geometry-base-material-pbr-inst.asp");
	if (!prepareProgram->Link())
		return false;

	AnimaShaderProgram* directionalLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/directional-light-pbr.asp");
	if (!directionalLightProgram->Link())
		return false;

	AnimaShaderProgram* skyboxProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/skybox.asp");
	if (!skyboxProgram->Link())
		return false;

	AnimaShaderProgram* hemisphereLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/hemisphere-light.asp");
	if (!hemisphereLightProgram->Link())
		return false;

	AnimaShaderProgram* pointLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/point-light-pbr.asp");
	if (!pointLightProgram->Link())
		return false;

	AnimaShaderProgram* spotLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/spot-light.asp");
	if (!spotLightProgram->Link())
		return false;

	AnimaShaderProgram* nullFilterProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/nullFilter.asp");
	if (!nullFilterProgram->Link())
		return false;
	
	AnimaShaderProgram* combineProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/combine-pbr.asp");
	if (!combineProgram->Link())
		return false;

	AnimaShaderProgram* directionalLightShadowMapProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/dil-shadow-map-inst-vsm.asp");
	if (!directionalLightShadowMapProgram->Link())
		return false;

	AnimaShaderProgram* brightnessFilterProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/brightnessFilter.asp");
	if (!brightnessFilterProgram->Link())
		return false;

	AnimaShaderProgram* gaussianFilterProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/gaussianFilter.asp");
	if (!gaussianFilterProgram->Link())
		return false;

	AnimaShaderProgram* finalProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/final.asp");
	if (!finalProgram->Link())
		return false;
	
	AnimaShaderProgram* fxaaFilterProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/fxaaFilter.asp");
	if (!fxaaFilterProgram->Link())
		return false;
	
	AnimaShaderProgram* physicsDebugProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/physicsDebug.asp");
	if (!physicsDebugProgram->Link())
		return false;

	_defaultShaderProgram = prepareProgram;
	shadersManager->SetDefaultFragmentShader(shadersManager->GetShaderFromName("base-material-pbr-fs-inst"));
	shadersManager->SetDefaultVertexShader(shadersManager->GetShaderFromName("static-mesh-vs-inst"));

	if(_physicsDebugDrawer != nullptr)
		_physicsDebugDrawer->SetupPrograms(_engine);
	
	return true;
}

void AnimaRenderer::CheckPrograms(AnimaScene* scene)
{
	AnimaGeometriesManager* geometriesManager = scene->GetGeometriesManager();
	AnimaShadersManager* shadersManager = scene->GetShadersManager();
	AnimaMaterialsManager* materialsManager = scene->GetMaterialsManager();

	AInt geometriesCount = geometriesManager->GetGeometriesCount();
	for (AInt i = 0; i < geometriesCount; i++)
	{
		AnimaGeometry* geometry = geometriesManager->GetGeometry(i);

		AnimaRendererDrawableGeometry drawableGeometry;
		drawableGeometry.SetGeometry(geometry);

		AInt instancesCount = geometry->GetInstancesCount();
		for (AInt j = 0; j < instancesCount; j++)
		{
			AnimaGeometryInstance* instance = geometry->GetInstance(j);

			AnimaShaderProgram* program = instance->GetShaderProgram();
			AnimaString programName = instance->GetShaderProgramName();
			if (program == nullptr)
			{
				if (programName.empty())
				{
					AnimaMaterial* material = instance->GetMaterial();
					if (material == nullptr)
						material = materialsManager->GetDefaultMaterial();

					if (material)
						program = shadersManager->CreateProgram(instance, material);
				}
				else
					program = shadersManager->GetProgramFromName(programName);

				if (program == nullptr)
					program = _defaultShaderProgram;
				program->Link();

				instance->SetShaderProgram(program);
			}
			else if (program->GetName() != programName)
			{
				program = shadersManager->GetProgramFromName(programName);

				if (program == nullptr)
					program = _defaultShaderProgram;
				program->Link();

				instance->SetShaderProgram(program);
			}
		}
	}
}

//AnimaPrimitiveData::AnimaPrimitiveData(AnimaAllocator* allocator)
//{
//}
//
//AnimaPrimitiveData::AnimaPrimitiveData(AnimaPrimitiveData& src)
//	: _vertices(src._vertices)
//	, _indices(src._indices)
//{
//}
//
//AnimaPrimitiveData::AnimaPrimitiveData(AnimaPrimitiveData&& src)
//	: _vertices(src._vertices)
//	, _indices(src._indices)
//{
//}
//
//AnimaPrimitiveData::~AnimaPrimitiveData()
//{
//	_vertices.clear();
//	_indices.clear();
//}
//
//void AnimaPrimitiveData::SetVertices(AnimaArray<AnimaVertex3f>* vertices)
//{
//	_vertices.clear();
//	
//	if(vertices != nullptr)
//	{
//		AInt vSize = vertices->size();
//		for(AInt i = 0; i < vSize; i++)
//			_vertices.push_back(vertices->at(i));
//	}
//}
//
//AnimaArray<AnimaVertex3f>* AnimaPrimitiveData::GetVertices()
//{
//	return &_vertices;
//}
//
//void AnimaPrimitiveData::SetIndices(AnimaArray<AUint>* indices)
//{
//	_indices.clear();
//	
//	if(indices != nullptr)
//	{
//		AInt vSize = indices->size();
//		for(AInt i = 0; i < vSize; i++)
//			_indices.push_back(indices->at(i));
//	}
//}
//
//AnimaArray<AUint>* AnimaPrimitiveData::GetIndices()
//{
//	return &_indices;
//}
//
//void AnimaPrimitiveData::SetColor(const AnimaColor4f& color)
//{
//	_color = color;
//}
//
//AnimaColor4f AnimaPrimitiveData::GetColor()
//{
//	return _color;
//}
//
//void AnimaPrimitiveData::SetType(AUint type)
//{
//	_type = type;
//}
//
//AUint AnimaPrimitiveData::GetType()
//{
//	return _type;
//}
//
//void AnimaPrimitiveData::SetNodeMatrix(const AnimaMatrix& nodeMatrix)
//{
//	_nodeMatrix = nodeMatrix;
//}
//
//AnimaMatrix AnimaPrimitiveData::GetNodeMatrix()
//{
//	return _nodeMatrix;
//}

END_ANIMA_ENGINE_NAMESPACE