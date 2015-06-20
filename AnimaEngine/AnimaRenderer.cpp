#include "AnimaEngine.h"
#include "AnimaRenderer.h"
#include "AnimaCamera.h"
#include "AnimaShaderProgram.h"
#include "AnimaTexture.h"
#include "AnimaLight.h"
#include "AnimaModelsManager.h"
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

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRenderer::AnimaRenderer(AnimaEngine* engine, AnimaAllocator* allocator)
: _primitives(allocator != nullptr ? allocator : engine->GetGenericAllocator())
{
	_allocator = allocator != nullptr ? allocator : engine->GetGenericAllocator();
	_engine = engine;
	_scene = nullptr;

	_filterCamera = nullptr;
	_filterMesh = nullptr;
	
	_indexesBufferObject = 0;
	_verticesBufferObject = 0;
	_vertexArrayObject = 0;

	_lastUpdatedModel = nullptr;
	_lastUpdatedModelInstance = nullptr;

	InitTextureSlots();
}

AnimaRenderer::AnimaRenderer(AnimaRenderer& src)
	: _primitives(src._primitives)
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

	_lightsMeshMap = src._lightsMeshMap;

	_indexesBufferObject = src._indexesBufferObject;
	_verticesBufferObject = src._verticesBufferObject;
	_vertexArrayObject = src._vertexArrayObject;

	_scene = src._scene;
	_engine = src._engine;
}

AnimaRenderer::AnimaRenderer(AnimaRenderer&& src)
	: _primitives(src._primitives)
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

	_lightsMeshMap = src._lightsMeshMap;

	_indexesBufferObject = src._indexesBufferObject;
	_verticesBufferObject = src._verticesBufferObject;
	_vertexArrayObject = src._vertexArrayObject;

	_scene = src._scene;
	_engine = src._engine;
}

AnimaRenderer::~AnimaRenderer()
{
	Clear();
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

		_lightsMeshMap = src._lightsMeshMap;

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		_vertexArrayObject = src._vertexArrayObject;

		_scene = src._scene;
		_engine = src._engine;
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

		_lightsMeshMap = src._lightsMeshMap;

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		_vertexArrayObject = src._vertexArrayObject;

		_scene = src._scene;
		_engine = src._engine;
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
	SetTextureSlot("ShadowMap", 5);

	// Slot usati dal disegno di primitive
	SetTextureSlot("DepthMap", 0);
	SetTextureSlot("Diffuse2Map", 1);
	SetTextureSlot("PDepthMap", 2);
	SetTextureSlot("PColorMap", 3);

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
			prepassBuffer->AddTexture("DepthMap", GL_TEXTURE_2D, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("AlbedoMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("NormalMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0 + 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("SpecularMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0 + 2, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			ANIMA_ASSERT(prepassBuffer->Create());

			SetGBuffer("PrepassBuffer", prepassBuffer);
		}

		AnimaGBuffer* lightsBuffer = GetGBuffer("LightsBuffer");
		if (lightsBuffer != nullptr)
			lightsBuffer->Resize(width, height);
		else
		{
			lightsBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaGBuffer>(*_allocator, _allocator, width, height);
			lightsBuffer->AddTexture("EmissiveMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			lightsBuffer->AddTexture("SpecularMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0 + 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
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
			diffuseTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, "DiffuseMap", GL_TEXTURE_2D, width, height, nullptr, 0, 0, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
			ANIMA_ASSERT(diffuseTexture->LoadRenderTargets());
			SetTexture("DiffuseMap", diffuseTexture);
		}
		
		//AnimaTexture* diffuse2Texture = GetTexture("Diffuse2Map");
		//if(diffuse2Texture != nullptr)
		//	diffuse2Texture->Resize(screenWidth, screenHeight);
		//else
		//{
		//	diffuse2Texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, screenWidth, screenHeight, nullptr, 0, 0, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
		//	ANIMA_ASSERT(diffuse2Texture->LoadRenderTargets());
		//	SetTexture("Diffuse2Map", diffuse2Texture);
		//}

		//AnimaTexture* ssaoTexture = GetTexture("SSAOMap");
		//if (ssaoTexture != nullptr)
		//	ssaoTexture->Resize(screenWidth, screenHeight);
		//else
		//{
		//	ssaoTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, screenWidth, screenHeight, nullptr, 0, 0, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
		//	ANIMA_ASSERT(ssaoTexture->LoadRenderTargets());
		//	SetTexture("SSAOMap", ssaoTexture);
		//}
		
		SetGBuffer("FilterBuffer", nullptr, false);
		SetTexture("FilterMap", nullptr, false);

		SetVector("ScreenSize", (AFloat)screenWidth, (AFloat)screenHeight);
		SetVector("InverseScreenSize", 1.0f / (AFloat)screenWidth, 1.0f / (AFloat)screenHeight);
	}
}

void AnimaRenderer::InitRenderingUtilities(AInt screenWidth, AInt screenHeight)
{
	if (_filterMesh != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterMesh);
		_filterMesh = nullptr;
	}

	if (_filterCamera != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterCamera);
		_filterCamera = nullptr;
	}
	
	SetFloat("FxaaReduceMin", 1.0f / 128.0f);
	SetFloat("FxaaReduceMul", 1.0f / 8.0f);
	SetFloat("FxaaSpanMax", 8.0f);
	SetColor("AmbientLight", 1.0f, 1.0f, 1.0f);
	//SetColor("BackColor", 0.3f, 0.3f, 0.3f, 1.0f);
	SetColor("BackColor", 0.0f, 0.0f, 0.0f, 1.0f);

	SetVector("SSAOFilterRadius", 5.0f, 5.0f);
	SetFloat("SSAODistanceThreshold", 5.0f);

	//
	// Inizializzazione delle mesh di supporto
	//
	AnimaString name("filter_RENMESH", _engine->GetStringAllocator());
	_filterMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, name, _engine->GetDataGeneratorsManager(), _allocator);
	_filterMesh->MakePlane();
	_filterMesh->GetTransformation()->RotateXDeg(90.0f);

	AnimaMesh* ptlMesh = CreateMeshForLightType<AnimaPointLight>();
	ptlMesh->MakeIcosahedralSphere(3);

	AnimaMesh* dilMesh = CreateMeshForLightType<AnimaDirectionalLight>();
	dilMesh->MakePlane();

	AnimaMesh* helMesh = CreateMeshForLightType<AnimaHemisphereLight>();
	helMesh->MakePlane();
	
	AnimaMesh* splMesh = CreateMeshForLightType<AnimaSpotLight>();
	splMesh->MakeCylinder(0.0f, -1.0f, 1.0f, 60);

	//
	// Inizializzazione della camera di supporto
	//
	_filterCamera = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*_allocator, _allocator, nullptr, _engine->GetDataGeneratorsManager(), AnimaString("$$filterCamera$$", _allocator));
	_filterCamera->SetProjectionMatrix(AnimaMatrix());
	_filterCamera->SetViewMatrix(AnimaMatrix());
	_filterCamera->SetPosition(0.0f, 0.0f, 0.0f);

	glGenVertexArrays(1, &_vertexArrayObject);
	glGenBuffers(1, &_indexesBufferObject);
	glGenBuffers(1, &_verticesBufferObject);
}

void AnimaRenderer::ApplyEffectFromTextureToTexture(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst)
{
	ANIMA_ASSERT(src != dst);

	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetTexture("FilterMap", src, false);
	SetVector("TextureSize", AnimaVertex2f((AFloat)src->GetWidth(), (AFloat)src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterMesh->Draw(this, filterProgram, false);

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderer::ApplyEffectFromTextureToGBuffer(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaGBuffer* dst)
{
	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetTexture("FilterMap", src, false);
	SetVector("TextureSize", AnimaVertex2f((AFloat)src->GetWidth(), (AFloat)src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterMesh->Draw(this, filterProgram, false);

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderer::ApplyEffectFromGBufferToGBuffer(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaGBuffer* dst)
{
	ANIMA_ASSERT(src != dst);

	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetGBuffer("FilterBuffer", src, false);

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterMesh->Draw(this, filterProgram, false);
	
	SetGBuffer("FilterBuffer", nullptr, false);
}

void AnimaRenderer::ApplyEffectFromGBufferToTexture(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaTexture* dst)
{
	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetGBuffer("FilterBuffer", src, false);

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateSceneObjectProperties(_filterCamera, this);
	filterProgram->UpdateRenderingManagerProperies(this);

	_filterMesh->Draw(this, filterProgram, false);

	SetGBuffer("FilterBuffer", nullptr, false);
}

void AnimaRenderer::Start(AnimaScene* scene)
{
	ANIMA_ASSERT(scene != nullptr);
	_scene = scene;
	
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();

	if (lightsManager->GetTotalLightsCount() == 0)
	{
		AnimaHemisphereLight* hemL = lightsManager->CreateLight<AnimaHemisphereLight>("hemisphere");
		hemL->SetIntensity(0.8f);
		hemL->SetSkyColor(1.0f, 1.0f, 1.0f);
		hemL->SetGroundColor(0.0f, 0.0f, 0.0f);
		hemL->SetPosition(0.0f, 2000.0f, 0.0f);
	}
}

void AnimaRenderer::Start()
{
	ANIMA_ASSERT(_scene != nullptr);
	_scene->GetShadersManager()->SetActiveProgram(nullptr);
	
	_lastUpdatedModel = nullptr;
	_lastUpdatedModelInstance = nullptr;
}

void AnimaRenderer::Finish()
{
	ANIMA_ASSERT(_scene != nullptr);
	_scene->GetShadersManager()->SetActiveProgram(nullptr);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_lastUpdatedModel = nullptr;
	_lastUpdatedModelInstance = nullptr;
}

void AnimaRenderer::DrawAll()
{
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();

	AnimaVertex4f backColor = GetColor4f("BackColor");

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	PreparePass(shadersManager->GetProgramFromName("deferred-prepare"));
	
	Finish();

	////
	////	Aggiorno il buffer per SSAO
	////
	//ApplyEffectFromGBufferToTexture(shadersManager->GetProgramFromName("ssao"), GetGBuffer("PrepassBuffer"), nullptr);
	
	////
	////	Aggiorno le mappature per le ombre
	////
	//Start();
	//UpdateShadowMaps(shadersManager->GetProgramFromName("deferred-shadowMap"));
	//Finish();
	
	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = lights->GetArraysMap();
	for (auto pair : (*lightsMap))
		LightPass(pair.second->GetArray());


	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish();
	
	//
	//	Composizione dei buffer nell'immagine finale
	//
	if(_primitives.GetSize() <= 0)
	{
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();
	}
	else
	{
		// Renderizzo l'immagine finale completa di luci in Diffuse2Map
		GetTexture("Diffuse2Map")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();
		
		// Disegno le primitive in PrimitivesBuffer
		GetGBuffer("PrimitivesBuffer")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		DrawPrimitives(shadersManager->GetProgramFromName("primitive-draw"));
		Finish();
		
		// Compongo l'immagine finale prendendo la profondità da PrepassBuffer, il colore della scena da Diffuse2Map
		// ed il colore + profondità da PrimitivesBuffer
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePrimitives(shadersManager->GetProgramFromName("primitive-combine"));
		Finish();
		
		ClearPrimitives();
	}
	
	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderer::DrawMesh(AnimaMesh* mesh)
{
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();

	AnimaVertex4f backColor = GetColor4f("BackColor");

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	PreparePass(shadersManager->GetProgramFromName("deferred-prepare"), mesh);

	Finish();

	////
	////	Aggiorno il buffer per SSAO
	////
	//ApplyEffectFromGBufferToTexture(shadersManager->GetProgramFromName("ssao"), GetGBuffer("PrepassBuffer"), nullptr);

	////
	////	Aggiorno le mappature per le ombre
	////
	//Start();
	//UpdateShadowMaps(shadersManager->GetProgramFromName("deferred-shadowMap"));
	//Finish();

	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = lights->GetArraysMap();
	for (auto pair : (*lightsMap))
		LightPass(pair.second->GetArray());


	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish();

	//
	//	Composizione dei buffer nell'immagine finale
	//
	if (_primitives.GetSize() <= 0)
	{
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();
	}
	else
	{
		// Renderizzo l'immagine finale completa di luci in Diffuse2Map
		GetTexture("Diffuse2Map")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();

		// Disegno le primitive in PrimitivesBuffer
		GetGBuffer("PrimitivesBuffer")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		DrawPrimitives(shadersManager->GetProgramFromName("primitive-draw"));
		Finish();

		// Compongo l'immagine finale prendendo la profondità da PrepassBuffer, il colore della scena da Diffuse2Map
		// ed il colore + profondità da PrimitivesBuffer
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePrimitives(shadersManager->GetProgramFromName("primitive-combine"));
		Finish();

		ClearPrimitives();
	}

	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderer::DrawMesh(AnimaMeshInstance* instance)
{
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();

	AnimaVertex4f backColor = GetColor4f("BackColor");

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	PreparePass(shadersManager->GetProgramFromName("deferred-prepare"), instance);

	Finish();

	////
	////	Aggiorno il buffer per SSAO
	////
	//ApplyEffectFromGBufferToTexture(shadersManager->GetProgramFromName("ssao"), GetGBuffer("PrepassBuffer"), nullptr);

	////
	////	Aggiorno le mappature per le ombre
	////
	//Start();
	//UpdateShadowMaps(shadersManager->GetProgramFromName("deferred-shadowMap"));
	//Finish();

	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = lights->GetArraysMap();
	for (auto pair : (*lightsMap))
		LightPass(pair.second->GetArray());


	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish();

	//
	//	Composizione dei buffer nell'immagine finale
	//
	if (_primitives.GetSize() <= 0)
	{
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();
	}
	else
	{
		// Renderizzo l'immagine finale completa di luci in Diffuse2Map
		GetTexture("Diffuse2Map")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();

		// Disegno le primitive in PrimitivesBuffer
		GetGBuffer("PrimitivesBuffer")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		DrawPrimitives(shadersManager->GetProgramFromName("primitive-draw"));
		Finish();

		// Compongo l'immagine finale prendendo la profondità da PrepassBuffer, il colore della scena da Diffuse2Map
		// ed il colore + profondità da PrimitivesBuffer
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePrimitives(shadersManager->GetProgramFromName("primitive-combine"));
		Finish();

		ClearPrimitives();
	}

	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderer::DrawModel(AnimaModel* model)
{
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();

	AnimaVertex4f backColor = GetColor4f("BackColor");

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	PreparePass(shadersManager->GetProgramFromName("deferred-prepare"), model);

	Finish();

	////
	////	Aggiorno il buffer per SSAO
	////
	//ApplyEffectFromGBufferToTexture(shadersManager->GetProgramFromName("ssao"), GetGBuffer("PrepassBuffer"), nullptr);

	////
	////	Aggiorno le mappature per le ombre
	////
	//Start();
	//UpdateShadowMaps(shadersManager->GetProgramFromName("deferred-shadowMap"));
	//Finish();

	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = lights->GetArraysMap();
	for (auto pair : (*lightsMap))
		LightPass(pair.second->GetArray());


	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish();

	//
	//	Composizione dei buffer nell'immagine finale
	//
	if (_primitives.GetSize() <= 0)
	{
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();
	}
	else
	{
		// Renderizzo l'immagine finale completa di luci in Diffuse2Map
		GetTexture("Diffuse2Map")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();

		// Disegno le primitive in PrimitivesBuffer
		GetGBuffer("PrimitivesBuffer")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		DrawPrimitives(shadersManager->GetProgramFromName("primitive-draw"));
		Finish();

		// Compongo l'immagine finale prendendo la profondità da PrepassBuffer, il colore della scena da Diffuse2Map
		// ed il colore + profondità da PrimitivesBuffer
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePrimitives(shadersManager->GetProgramFromName("primitive-combine"));
		Finish();

		ClearPrimitives();
	}

	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderer::DrawModel(AnimaModelInstance* instance)
{
	Anima::AnimaShadersManager* shadersManager = _scene->GetShadersManager();
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();

	AnimaVertex4f backColor = GetColor4f("BackColor");

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	PreparePass(shadersManager->GetProgramFromName("deferred-prepare"), instance);

	Finish();

	////
	////	Aggiorno il buffer per SSAO
	////
	//ApplyEffectFromGBufferToTexture(shadersManager->GetProgramFromName("ssao"), GetGBuffer("PrepassBuffer"), nullptr);

	////
	////	Aggiorno le mappature per le ombre
	////
	//Start();
	//UpdateShadowMaps(shadersManager->GetProgramFromName("deferred-shadowMap"));
	//Finish();

	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaString::Hasher>* lightsMap = lights->GetArraysMap();
	for (auto pair : (*lightsMap))
		LightPass(pair.second->GetArray());


	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish();

	//
	//	Composizione dei buffer nell'immagine finale
	//
	if (_primitives.GetSize() <= 0)
	{
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();
	}
	else
	{
		// Renderizzo l'immagine finale completa di luci in Diffuse2Map
		GetTexture("Diffuse2Map")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePass(shadersManager->GetProgramFromName("deferred-combine"));
		Finish();

		// Disegno le primitive in PrimitivesBuffer
		GetGBuffer("PrimitivesBuffer")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		DrawPrimitives(shadersManager->GetProgramFromName("primitive-draw"));
		Finish();

		// Compongo l'immagine finale prendendo la profondità da PrepassBuffer, il colore della scena da Diffuse2Map
		// ed il colore + profondità da PrimitivesBuffer
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
		CombinePrimitives(shadersManager->GetProgramFromName("primitive-combine"));
		Finish();

		ClearPrimitives();
	}

	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderer::DrawMesh(AnimaMesh* mesh, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum, bool useInstances)
{
	if (!useInstances)
	{
		AnimaTransformation* meshTransfomation = mesh->GetTransformation();

		if (!forceDraw)
		{
			if (frustum != nullptr && !frustum->SphereInFrustum(meshTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
				return;
		}

		AnimaModel* meshParent = (AnimaModel*)mesh->GetParentObject()->GetAncestorObject();
		if (meshParent != _lastUpdatedModel)
		{
			_lastUpdatedModel = meshParent;
			program->UpdateSceneObjectProperties(_lastUpdatedModel, this);
		}

		if (mesh->NeedsBuffersUpdate())
			mesh->UpdateBuffers();

		if (mesh->GetVertexArrayObject() <= 0)
			return;

		mesh->Draw(this, program, updateMaterial);
	}
	else
	{
		if (mesh->NeedsBuffersUpdate())
			mesh->UpdateBuffers();

		if (mesh->GetVertexArrayObject() <= 0)
			return;

		AInt instancesCount = mesh->GetInstancesCount();
		for (AInt i = 0; i < instancesCount; i++)
		{
			AnimaMeshInstance* instance = mesh->GetInstance(i);
			AnimaTransformation* instanceTransfomation = instance->GetTransformation();
			if (!forceDraw)
			{
				if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
					continue;
			}

			AnimaModelInstance* instanceParent = (AnimaModelInstance*)instance->GetParentObject()->GetAncestorObject();
			if (instanceParent != _lastUpdatedModelInstance)
			{
				_lastUpdatedModelInstance = instanceParent;
				program->UpdateSceneObjectProperties(_lastUpdatedModelInstance, this);
			}

			instance->Draw(this, program, updateMaterial);
		}
	}
}

void AnimaRenderer::DrawMesh(AnimaMeshInstance* instance, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum)
{
	AnimaMesh* mesh = instance->GetMesh();

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	if (mesh->GetVertexArrayObject() <= 0)
		return;

	AnimaTransformation* instanceTransfomation = instance->GetTransformation();
	if (!forceDraw)
	{
		if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
			return;
	}

	instance->Draw(this, program, updateMaterial);
}

void AnimaRenderer::DrawModel(AnimaModel* model, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum, bool useInstances)
{
	if (!useInstances)
	{
		AInt meshesCount = model->GetMeshesCount();
		for (AInt i = 0; i < meshesCount; i++)
		{
			AnimaMesh* mesh = model->GetMesh(i);
			AnimaTransformation* meshTransfomation = mesh->GetTransformation();

			if (!forceDraw)
			{
				if (frustum != nullptr && !frustum->SphereInFrustum(meshTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
					continue;
			}

			if (mesh->NeedsBuffersUpdate())
				mesh->UpdateBuffers();

			if (mesh->GetVertexArrayObject() <= 0)
				continue;

			mesh->Draw(this, program, updateMaterial);
		}
	}
	else
	{
		AInt meshesCount = model->GetMeshesCount();
		for (AInt i = 0; i < meshesCount; i++)
		{
			AnimaMesh* mesh = model->GetMesh(i);
			if (mesh->NeedsBuffersUpdate())
				mesh->UpdateBuffers();

			if (mesh->GetVertexArrayObject() <= 0)
				continue;

			AInt instancesCount = mesh->GetInstancesCount();
			for (AInt j = 0; j < instancesCount; j++)
			{
				AnimaMeshInstance* instance = mesh->GetInstance(j);
				AnimaTransformation* instanceTransfomation = instance->GetTransformation();
				if (!forceDraw)
				{
					if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
						continue;
				}

				instance->Draw(this, program, updateMaterial);
			}
		}
	}

	AInt childrenCount = model->GetChildrenNumber();
	for (AInt i = 0; i < childrenCount; i++)
		DrawModel((AnimaModel*)model->GetChild(i), program, updateMaterial, forceDraw, frustum, useInstances);
}

void AnimaRenderer::DrawModel(AnimaModelInstance* instance, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum)
{
	AInt meshesCount = instance->GetMeshesCount();
	for (AInt i = 0; i < meshesCount; i++)
	{
		AnimaMeshInstance* meshInstance = instance->GetMesh(i);
		AnimaMesh* mesh = meshInstance->GetMesh();
		if (mesh->NeedsBuffersUpdate())
			mesh->UpdateBuffers();

		if (mesh->GetVertexArrayObject() <= 0)
			continue;

		AnimaTransformation* instanceTransfomation = meshInstance->GetTransformation();
		if (!forceDraw)
		{
			if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
				continue;
		}

		meshInstance->Draw(this, program, updateMaterial);
	}

	AInt childrenCount = instance->GetChildrenNumber();
	for (AInt i = 0; i < childrenCount; i++)
		DrawModel((AnimaModelInstance*)instance->GetChild(i), program, updateMaterial, forceDraw, frustum);
}

void AnimaRenderer::PreparePass(AnimaShaderProgram* program)
{
	AnimaMeshesManager* meshesManager = _scene->GetMeshesManager();
	AInt meshesCount = meshesManager->GetMeshesCount();
	if (meshesCount == 0)
		return;

	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();

	AnimaFrustum* frustum = nullptr;

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();
		program->UpdateSceneObjectProperties(camera, this);

		frustum = camera->GetFrustum();
	}

	for (AInt i = 0; i < meshesCount; i++)
	{
		AnimaMesh* mesh = meshesManager->GetMesh(i);
		DrawMesh(mesh, program, true, false, frustum);
	}
}

void AnimaRenderer::PreparePass(AnimaShaderProgram* program, AnimaMesh* mesh)
{
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();

	AnimaFrustum* frustum = nullptr;
	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();
		program->UpdateSceneObjectProperties(camera, this);

		frustum = camera->GetFrustum();
	}

	DrawMesh(mesh, program, true, false, frustum, false);
}

void AnimaRenderer::PreparePass(AnimaShaderProgram* program, AnimaMeshInstance* instance)
{
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();

	AnimaFrustum* frustum = nullptr;
	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();
		program->UpdateSceneObjectProperties(camera, this);

		frustum = camera->GetFrustum();
	}

	DrawMesh(instance, program, true, false, frustum);
}

void AnimaRenderer::PreparePass(AnimaShaderProgram* program, AnimaModel* model)
{
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();

	AnimaFrustum* frustum = nullptr;
	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();
		program->UpdateSceneObjectProperties(camera, this);

		frustum = camera->GetFrustum();
	}

	AInt meshesCount = model->GetMeshesCount();
	for (AInt i = 0; i < meshesCount; i++)
		DrawMesh(model->GetMesh(i), program, true, false, frustum, false);

	AInt childrenCount = model->GetChildrenNumber();
	for (AInt i = 0; i < childrenCount; i++)
		PreparePass(program, (AnimaModel*)model->GetChild(i));
}

void AnimaRenderer::PreparePass(AnimaShaderProgram* program, AnimaModelInstance* instance)
{
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();

	AnimaFrustum* frustum = nullptr;
	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();
		program->UpdateSceneObjectProperties(camera, this);

		frustum = camera->GetFrustum();
	}

	AInt meshesCount = instance->GetMeshesCount();
	for (AInt i = 0; i < meshesCount; i++)
		DrawMesh(instance->GetMesh(i), program, true, false, frustum);

	AInt childrenCount = instance->GetChildrenNumber();
	for (AInt i = 0; i < childrenCount; i++)
		PreparePass(program, (AnimaModelInstance*)instance->GetChild(i));
}

void AnimaRenderer::LightPass(AnimaArray<AnimaLight*>* lights)
{
	AInt size = lights->GetSize();

	if (size == 0)
		return;
	
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	AnimaCamera* activeCamera = _scene->GetCamerasManager()->GetActiveCamera();

	if (activeCamera == nullptr)
		return;

	AnimaString type(typeid((*lights->ElementAt(0))).name(), _allocator);
	auto pair = _lightsMeshMap.find(type);

	ANIMA_ASSERT(pair != _lightsMeshMap.end());
	ANIMA_ASSERT(lights->ElementAt(0)->CreateShader(_scene->GetShadersManager()));
	AnimaShaderProgram* program = _scene->GetShadersManager()->GetProgramFromName(lights->ElementAt(0)->GetShaderName());
	AnimaMesh* mesh = pair->second;

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateSceneObjectProperties(activeCamera, this);
	}

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	for (AInt i = 0; i < size; i++)
	{
		AnimaLight* light = lights->ElementAt(i);

		light->UpdateMeshTransformation(mesh->GetTransformation());
		light->UpdateCullFace(activeCamera);

		program->UpdateSceneObjectProperties(light, this);
		program->UpdateRenderingManagerProperies(this);

		mesh->Draw(this, program, false);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}

void AnimaRenderer::CombinePass(AnimaShaderProgram* program)
{
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateSceneObjectProperties(_filterCamera, this);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	program->UpdateRenderingManagerProperies(this);
	
	_filterMesh->Draw(this, program, false);
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	AnimaMeshesManager* meshesManager = _scene->GetMeshesManager();

	AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	if (lights == nullptr)
		return;

	AInt nLights = lights->GetSize();
	AInt nMeshes = meshesManager->GetMeshesCount();

	if (nLights == 0 || nMeshes == 0)
		return;

	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();
	
	for (AInt i = 0; i < nLights; i++)
	{
		AnimaLight* light = lights->ElementAt(i);
		AnimaTexture* shadowMap = light->GetShadowTexture();
		if (!shadowMap->AreRenderTargetsReady())
			shadowMap->LoadRenderTargets();

		shadowMap->BindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		program->UpdateSceneObjectProperties(light, this);
		program->UpdateRenderingManagerProperies(this);

		for (AInt j = 0; j < nMeshes; j++)
		{
			DrawMesh(meshesManager->GetMesh(j), program, false, true, nullptr, true);
			//	AnimaMesh* innerModel = modelsManager->GetModel(j);
			//	AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();
			//	
			//	glCullFace(GL_FRONT);
			//	DrawModelMesh(_scene, innerModel, program, modelMatrix, false, true);
			//	glCullFace(GL_BACK);
			//	
			//	AInt meshNumber = innerModel->GetMeshesNumber();
			//	for (AInt i = 0; i < meshNumber; i++)
			//	{
			//		glCullFace(GL_FRONT);
			//		DrawModelMesh(_scene, innerModel->GetMesh(i), program, modelMatrix, false, true);
			//		glCullFace(GL_BACK);
			//	}
			//	
			//	AInt childrenNumber = innerModel->GetChildrenNumber();
			//	for (AInt i = 0; i < childrenNumber; i++)
			//		DrawModel(_scene, innerModel->GetChild(i), program, modelMatrix, false, true);
		}
	}
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program, AnimaMesh* mesh)
{
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	if (lights == nullptr || mesh == nullptr)
		return;

	AInt nLights = lights->GetSize();
	if (nLights == 0)
		return;
	
	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();

	for (AInt i = 0; i < nLights; i++)
	{
		AnimaLight* light = lights->ElementAt(i);
		AnimaTexture* shadowMap = light->GetShadowTexture();
		if (!shadowMap->AreRenderTargetsReady())
			shadowMap->LoadRenderTargets();

		shadowMap->BindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		program->UpdateSceneObjectProperties(light, this);
		program->UpdateRenderingManagerProperies(this);

		DrawMesh(mesh, program, false, true, nullptr, true);
	}
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program, AnimaMeshInstance* instance)
{
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	if (lights == nullptr || instance == nullptr)
		return;

	AInt nLights = lights->GetSize();
	if (nLights == 0)
		return;

	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();

	for (AInt i = 0; i < nLights; i++)
	{
		AnimaLight* light = lights->ElementAt(i);
		AnimaTexture* shadowMap = light->GetShadowTexture();
		if (!shadowMap->AreRenderTargetsReady())
			shadowMap->LoadRenderTargets();

		shadowMap->BindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		program->UpdateSceneObjectProperties(light, this);
		program->UpdateRenderingManagerProperies(this);

		DrawMesh(instance, program, false, true, nullptr);
	}
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program, AnimaModel* model)
{
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	if (lights == nullptr || model == nullptr)
		return;

	AInt nLights = lights->GetSize();
	if (nLights == 0)
		return;

	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();

	for (AInt i = 0; i < nLights; i++)
	{
		AnimaLight* light = lights->ElementAt(i);
		AnimaTexture* shadowMap = light->GetShadowTexture();
		if (!shadowMap->AreRenderTargetsReady())
			shadowMap->LoadRenderTargets();

		shadowMap->BindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		program->UpdateSceneObjectProperties(light, this);
		program->UpdateRenderingManagerProperies(this);

		DrawModel(model, program, false, true, nullptr, true);
	}
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program, AnimaModelInstance* instance)
{
	AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	if (lights == nullptr || instance == nullptr)
		return;

	AInt nLights = lights->GetSize();
	if (nLights == 0)
		return;

	AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();
	
	for (AInt i = 0; i < nLights; i++)
	{
		AnimaLight* light = lights->ElementAt(i);
		AnimaTexture* shadowMap = light->GetShadowTexture();
		if (!shadowMap->AreRenderTargetsReady())
			shadowMap->LoadRenderTargets();

		shadowMap->BindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		program->UpdateSceneObjectProperties(light, this);
		program->UpdateRenderingManagerProperies(this);

		DrawModel(instance, program, false, true, nullptr);
	}
}

void AnimaRenderer::UpdateModelsVisibility()
{
//	Anima::AnimaModelsManager* modelsManager = _scene->GetModelsManager();
//	Anima::AnimaCamerasManager* camerasManager = _scene->GetCamerasManager();
//
//	Anima::AnimaFrustum* frustum = camerasManager->GetActiveCamera()->GetFrustum();

	//	for (int i = 0; i < modelsManager->GetModelsNumber(); i++)
	//		UpdateModelVisibility(frustum, modelsManager->GetModel(i), Anima::AnimaMatrix());
}

void AnimaRenderer::UpdateModelVisibility(AnimaFrustum* frustum, AnimaMesh* mesh, AnimaMatrix parentMeshMatrix)
{
	AnimaMatrix modelMatrix = parentMeshMatrix * mesh->GetTransformation()->GetTransformationMatrix();

	if (mesh->GetVerticesNumber() > 0)
	{
		if (frustum->SphereInFrustum(parentMeshMatrix * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
			mesh->SetIsVisible(true);
		else
		{
			mesh->SetIsVisible(false);
			return;
		}
	}

	//AInt meshNumber = mesh->GetMeshesNumber();
	//for (AInt i = 0; i < meshNumber; i++)
	//{
	//	AnimaMesh* subMesh = mesh->GetMesh(i);
	//	subMesh->SetIsVisible(frustum->SphereInFrustum(parentMeshMatrix * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()));
	//}

	//AInt childrenNumber = mesh->GetChildrenNumber();
	//for (AInt i = 0; i < childrenNumber; i++)
	//	UpdateModelVisibility(frustum, mesh->GetChild(i), modelMatrix);
}

void AnimaRenderer::AddPrimitive(AnimaArray<AnimaVertex3f>* vertices, AnimaArray<AUint>* indices, AnimaColor4f color, AnimaMatrix modelMatrix, AUint primitiveType)
{
	if (vertices == nullptr)
		return;
	AnimaPrimitiveData* primitive = AnimaAllocatorNamespace::AllocateNew<AnimaPrimitiveData>(*_allocator, _allocator);
	primitive->SetVertices(vertices);
	primitive->SetIndices(indices);
	primitive->SetColor(color);
	primitive->SetType(primitiveType);
	primitive->SetModelMatrix(modelMatrix);
	
	_primitives.Add(primitive);
}

void AnimaRenderer::DrawPrimitives( AnimaShaderProgram* program)
{
	if(program == nullptr)
		return;
	
	program->Use();
	program->SetUniform("_projectionViewMatrix", _scene->GetCamerasManager()->GetActiveCamera()->GetProjectionViewMatrix());
	
	AInt nPrimitives = _primitives.GetSize();
	for(AInt i = 0; i < nPrimitives; i++)
		DrawPrimitive(_primitives[i], program);
}

void AnimaRenderer::DrawPrimitive(AnimaPrimitiveData* primitive, AnimaShaderProgram* program)
{
	if(primitive == nullptr)
		return;
	
	AnimaColor4f color = primitive->GetColor();
	AnimaArray<AnimaVertex3f>* vertices = primitive->GetVertices();
	AnimaArray<AUint>* indices = primitive->GetIndices();
	AUint type = primitive->GetType();
	AnimaMatrix modelMatrix = primitive->GetModelMatrix();
	
	if (color.w < 1.0f)
		glEnable(GL_BLEND);
	
	program->SetUniform("_color", color);
	program->SetUniform("_modelMatrix", modelMatrix);
	
	AInt nVertices = vertices->GetSize();
	AInt nCoordinate = nVertices * 3;
	AFloat* coordinate = AnimaAllocatorNamespace::AllocateArray<AFloat>(*_allocator, (ASizeT)nCoordinate);
	
	AInt offset = 0;
	for (AInt i = 0; i < nVertices; i++)
	{
		coordinate[offset++] = (*vertices)[i].x;
		coordinate[offset++] = (*vertices)[i].y;
		coordinate[offset++] = (*vertices)[i].z;
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _verticesBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AFloat) * nCoordinate, coordinate, GL_STATIC_DRAW);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, coordinate);
	coordinate = nullptr;
	
	if (indices->GetSize() > 0)
	{
		AInt nIndici = indices->GetSize();
		AUint* indici = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, (ASizeT)nIndici);
		
		for (AInt i = 0; i < nIndici; i++)
			indici[i] = (*indices)[i];
		
		glBindVertexArray(_vertexArrayObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(AUint) * nIndici, indici, GL_STATIC_DRAW);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, indici);
		indici = nullptr;
		
		program->EnableInput("_position", 3, GL_FLOAT, _verticesBufferObject);
		glDrawElements(type, nIndici, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glBindVertexArray(_vertexArrayObject);
		program->EnableInput("_position", 3, GL_FLOAT, _verticesBufferObject);
		glDrawArrays(type, 0, nCoordinate);
	}
	
	glDisable(GL_BLEND);
}

void AnimaRenderer::CombinePrimitives(AnimaShaderProgram* program)
{
	program->Use();
	program->UpdateRenderingManagerProperies(this);
	if(glGetError() != GL_NO_ERROR)
		return;
	
	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();
	
	program->Use();
	program->UpdateSceneObjectProperties(_filterCamera, this);
	program->UpdateRenderingManagerProperies(this);

	_filterMesh->Draw(this, program, false);
}

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

void AnimaRenderer::SetTexture(const char* propertyName, AnimaTexture* value, bool deleteExistent)
{
	AnimaString str(propertyName, _allocator);
	SetTexture(str, value, deleteExistent);
}

void AnimaRenderer::SetTextureSlot(AnimaString propertyName, AUint value)
{
	_textureSlotsMap[propertyName] = value;
}

void AnimaRenderer::SetTextureSlot(const char* propertyName, AUint value)
{
	AnimaString str(propertyName, _allocator);
	SetTextureSlot(str, value);
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

void AnimaRenderer::SetGBuffer(const char* name, AnimaGBuffer* value, bool deleteExistent)
{
	AnimaString str(name, _allocator);
	SetGBuffer(str, value, deleteExistent);
}

void AnimaRenderer::SetColor(AnimaString propertyName, AnimaColor3f value)
{
	_vectors3fMap[propertyName] = value;
}

void AnimaRenderer::SetColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaRenderer::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(r, g, b);
	SetColor(propertyName, color);
}

void AnimaRenderer::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, r, g, b);
}

void AnimaRenderer::SetColor(AnimaString propertyName, AnimaColor4f value)
{
	_vectors4fMap[propertyName] = value;
}

void AnimaRenderer::SetColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaRenderer::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	SetColor(propertyName, color);
}

void AnimaRenderer::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, r, g, b, a);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AnimaVertex2f value)
{
	_vectors2fMap[propertyName] = value;
}

void AnimaRenderer::SetVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex2f vector(x, y);
	SetVector(propertyName, vector);
}

void AnimaRenderer::SetVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AnimaVertex3f value)
{
	_vectors3fMap[propertyName] = value;
}

void AnimaRenderer::SetVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex3f vector(x, y, z);
	SetVector(propertyName, vector);
}

void AnimaRenderer::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y, z);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AnimaVertex4f value)
{
	_vectors4fMap[propertyName] = value;
}

void AnimaRenderer::SetVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaRenderer::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	SetVector(propertyName, vector);
}

void AnimaRenderer::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y, z, w);
}

void AnimaRenderer::SetFloat(AnimaString propertyName, AFloat value)
{
	_floatsMap[propertyName] = value;
}

void AnimaRenderer::SetFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _allocator);
	SetFloat(str, value);
}

void AnimaRenderer::SetBoolean(AnimaString propertyName, bool value)
{
	_booleansMap[propertyName] = value;
}

void AnimaRenderer::SetBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _allocator);
	SetBoolean(str, value);
}

void AnimaRenderer::SetInteger(AnimaString propertyName, AInt value)
{
	_integersMap[propertyName] = value;
}

void AnimaRenderer::SetInteger(const char* propertyName, AInt value)
{
	AnimaString str(propertyName, _allocator);
	SetInteger(str, value);
}

AnimaTexture* AnimaRenderer::GetTexture(AnimaString propertyName)
{
	if (_texturesMap.find(propertyName) != _texturesMap.end())
		return _texturesMap[propertyName];
	return nullptr;
}

AnimaTexture* AnimaRenderer::GetTexture(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetTexture(str);
}

AUint AnimaRenderer::GetTextureSlot(const AnimaString& slotName)
{
	if (_textureSlotsMap.find(slotName) != _textureSlotsMap.end())
		return _textureSlotsMap[slotName];
	return 0;
}

AUint AnimaRenderer::GetTextureSlot(const char* slotName)
{
	AnimaString str(slotName, _allocator);
	return GetTextureSlot(str);
}

AnimaGBuffer* AnimaRenderer::GetGBuffer(const AnimaString& gBufferName)
{
	if (_gBuffersMap.find(gBufferName) != _gBuffersMap.end())
		return _gBuffersMap[gBufferName];
	return nullptr;
}

AnimaGBuffer* AnimaRenderer::GetGBuffer(const char* gBufferName)
{
	AnimaString str(gBufferName, _allocator);
	return GetGBuffer(str);
}

AnimaColor3f AnimaRenderer::GetColor3f(AnimaString propertyName)
{
	if (_vectors3fMap.find(propertyName) != _vectors3fMap.end())
		return _vectors3fMap[propertyName];

	AnimaColor3f color(0.0f, 0.0f, 0.0f);
	return color;
}

AnimaColor3f AnimaRenderer::GetColor3f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetColor3f(str);
}

AnimaColor4f AnimaRenderer::GetColor4f(AnimaString propertyName)
{
	if (_vectors4fMap.find(propertyName) != _vectors4fMap.end())
		return _vectors4fMap[propertyName];

	AnimaColor4f color(0.0f, 0.0f, 0.0f, 1.0f);
	return color;
}

AnimaColor4f AnimaRenderer::GetColor4f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetColor4f(str);
}

AnimaVertex2f AnimaRenderer::GetVector2f(AnimaString propertyName)
{
	if (_vectors2fMap.find(propertyName) != _vectors2fMap.end())
		return _vectors2fMap[propertyName];

	AnimaVertex2f vector(0.0f, 0.0f);
	return vector;
}

AnimaVertex2f AnimaRenderer::GetVector2f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector2f(str);
}

AnimaVertex3f AnimaRenderer::GetVector3f(AnimaString propertyName)
{
	if (_vectors3fMap.find(propertyName) != _vectors3fMap.end())
		return _vectors3fMap[propertyName];

	AnimaVertex3f vector(0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex3f AnimaRenderer::GetVector3f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector3f(str);
}

AnimaVertex4f AnimaRenderer::GetVector4f(AnimaString propertyName)
{
	if (_vectors4fMap.find(propertyName) != _vectors4fMap.end())
		return _vectors4fMap[propertyName];

	AnimaVertex4f vector(0.0f, 0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex4f AnimaRenderer::GetVector4f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector4f(str);
}

AFloat AnimaRenderer::GetFloat(AnimaString propertyName)
{
	propertyName = propertyName;
	if (_floatsMap.find(propertyName) != _floatsMap.end())
		return _floatsMap[propertyName];
	return 0.0;
}

AFloat AnimaRenderer::GetFloat(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetFloat(str);
}

bool AnimaRenderer::GetBoolean(AnimaString propertyName)
{
	propertyName = propertyName;
	if (_booleansMap.find(propertyName) != _booleansMap.end())
		return _booleansMap[propertyName];
	return false;
}

bool AnimaRenderer::GetBoolean(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetBoolean(str);
}

AInt AnimaRenderer::GetInteger(AnimaString propertyName)
{
	propertyName = propertyName;
	if (_integersMap.find(propertyName) != _integersMap.end())
		return _integersMap[propertyName];
	return 0;
}

AInt AnimaRenderer::GetInteger(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetInteger(str);
}

void AnimaRenderer::Clear()
{
	glDeleteVertexArrays(1, &_vertexArrayObject);
	glDeleteBuffers(1, &_verticesBufferObject);
	glDeleteBuffers(1, &_indexesBufferObject);

	if (_filterCamera != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterCamera);
		_filterCamera = nullptr;
	}

	if (_filterMesh != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _filterMesh);
		_filterMesh = nullptr;
	}
	
	for (auto& pair : _lightsMeshMap)
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

	_lightsMeshMap.clear();
	
	ClearPrimitives();
}

void AnimaRenderer::ClearPrimitives()
{
	AInt nPrimitives = _primitives.GetSize();
	for(AInt i = 0; i < nPrimitives; i++)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _primitives[i]);
		_primitives[i] = nullptr;
	}
	
	_primitives.RemoveAll();
}

AnimaPrimitiveData::AnimaPrimitiveData(AnimaAllocator* allocator)
	: _vertices(allocator)
	, _indices(allocator)
{
}

AnimaPrimitiveData::AnimaPrimitiveData(AnimaPrimitiveData& src)
	: _vertices(src._vertices)
	, _indices(src._indices)
{
}

AnimaPrimitiveData::AnimaPrimitiveData(AnimaPrimitiveData&& src)
	: _vertices(src._vertices)
	, _indices(src._indices)
{
}

AnimaPrimitiveData::~AnimaPrimitiveData()
{
	_vertices.RemoveAll();
	_indices.RemoveAll();
}

void AnimaPrimitiveData::SetVertices(AnimaArray<AnimaVertex3f>* vertices)
{
	_vertices.RemoveAll();
	
	if(vertices != nullptr)
	{
		AInt vSize = vertices->GetSize();
		for(AInt i = 0; i < vSize; i++)
			_vertices.Add(vertices->ElementAt(i));
	}
}

AnimaArray<AnimaVertex3f>* AnimaPrimitiveData::GetVertices()
{
	return &_vertices;
}

void AnimaPrimitiveData::SetIndices(AnimaArray<AUint>* indices)
{
	_indices.RemoveAll();
	
	if(indices != nullptr)
	{
		AInt vSize = indices->GetSize();
		for(AInt i = 0; i < vSize; i++)
			_indices.Add(indices->ElementAt(i));
	}
}

AnimaArray<AUint>* AnimaPrimitiveData::GetIndices()
{
	return &_indices;
}

void AnimaPrimitiveData::SetColor(const AnimaColor4f& color)
{
	_color = color;
}

AnimaColor4f AnimaPrimitiveData::GetColor()
{
	return _color;
}

void AnimaPrimitiveData::SetType(AUint type)
{
	_type = type;
}

AUint AnimaPrimitiveData::GetType()
{
	return _type;
}

void AnimaPrimitiveData::SetModelMatrix(const AnimaMatrix& modelMatrix)
{
	_modelMatrix = modelMatrix;
}

AnimaMatrix AnimaPrimitiveData::GetModelMatrix()
{
	return _modelMatrix;
}

END_ANIMA_ENGINE_NAMESPACE