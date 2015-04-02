#include "AnimaEngine.h"
#include "AnimaRenderingManager.h"
#include "AnimaCamera.h"
#include "AnimaShaderProgram.h"
#include "AnimaTexture.h"
#include "AnimaLight.h"
#include "AnimaModelsManager.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"
#include "AnimaLightsManager.h"
#include "AnimaBenchmarkTimer.h"
#include <fstream>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRenderingManager::AnimaRenderingManager(AnimaAllocator* allocator)
	: _primitives(allocator)
{
	_allocator = allocator;
	_filterCamera = nullptr;
	_filterMesh = nullptr;
	
	_indexesBufferObject = 0;
	_verticesBufferObject = 0;
	_vertexArrayObject = 0;

	InitTextureSlots();
}

AnimaRenderingManager::AnimaRenderingManager(AnimaRenderingManager& src)
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
}

AnimaRenderingManager::AnimaRenderingManager(AnimaRenderingManager&& src)
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
}

AnimaRenderingManager::~AnimaRenderingManager()
{
	Clear();
}

AnimaRenderingManager& AnimaRenderingManager::operator=(const AnimaRenderingManager& src)
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
	}

	return *this;
}

AnimaRenderingManager& AnimaRenderingManager::operator=(AnimaRenderingManager&& src)
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
	}

	return *this;
}

void AnimaRenderingManager::InitTextureSlots()
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

void AnimaRenderingManager::InitRenderingTargets(AInt screenWidth, AInt screenHeight)
{
	AnimaVertex2f oldSize = GetVector2f("ScreenSize");
	AnimaVertex2f newSize((AFloat)screenWidth, (AFloat)screenHeight);

	if (oldSize != newSize)
	{
		AnimaGBuffer* prepassBuffer = GetGBuffer("PrepassBuffer");
		if (prepassBuffer != nullptr)
			prepassBuffer->Resize(screenWidth, screenHeight);
		else
		{
			prepassBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaGBuffer>(*_allocator, _allocator, screenWidth, screenHeight);
			prepassBuffer->AddTexture("DepthMap", GL_TEXTURE_2D, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, GL_NEAREST, GL_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("AlbedoMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("NormalMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0 + 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			prepassBuffer->AddTexture("SpecularMap", GL_TEXTURE_2D, GL_COLOR_ATTACHMENT0 + 2, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
			ANIMA_ASSERT(prepassBuffer->Create());

			SetGBuffer("PrepassBuffer", prepassBuffer);
		}

		AnimaGBuffer* lightsBuffer = GetGBuffer("LightsBuffer");
		if (lightsBuffer != nullptr)
			lightsBuffer->Resize(screenWidth, screenHeight);
		else
		{
			lightsBuffer = AnimaAllocatorNamespace::AllocateNew<AnimaGBuffer>(*_allocator, _allocator, screenWidth, screenHeight);
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
			diffuseTexture->Resize(screenWidth, screenHeight);
		else
		{
			diffuseTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, screenWidth, screenHeight, nullptr, 0, 0, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
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

void AnimaRenderingManager::InitRenderingUtilities(AInt screenWidth, AInt screenHeight)
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
	SetColor("AmbientLight", 0.1f, 0.1f, 0.1f);

	SetVector("SSAOFilterRadius", 5.0f, 5.0f);
	SetFloat("SSAODistanceThreshold", 5.0f);

	//
	// Inizializzazione delle mesh di supporto
	//
	_filterMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, _allocator);
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
	_filterCamera = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*_allocator, _allocator, nullptr);
	_filterCamera->SetProjectionMatrix(AnimaMatrix());
	_filterCamera->SetViewMatrix(AnimaMatrix());
	_filterCamera->SetPosition(0.0f, 0.0f, 0.0f);

	glGenVertexArrays(1, &_vertexArrayObject);
	glGenBuffers(1, &_indexesBufferObject);
	glGenBuffers(1, &_verticesBufferObject);
}

void AnimaRenderingManager::ApplyEffectFromTextureToTexture(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst)
{
	ANIMA_ASSERT(src != dst);

	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetTexture("FilterMap", src, false);
	SetVector("TextureSize", AnimaVertex2f((AFloat)src->GetWidth(), (AFloat)src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix(), _filterMesh->GetTransformation()->GetNormalMatrix());
	filterProgram->UpdateRenderingManagerProperies(this);
	
#ifdef WIN32
	glBindVertexArray(_filterMesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	filterProgram->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	filterProgram->DisableInputs();
#endif

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderingManager::ApplyEffectFromTextureToGBuffer(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaGBuffer* dst)
{
	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetTexture("FilterMap", src, false);
	SetVector("TextureSize", AnimaVertex2f((AFloat)src->GetWidth(), (AFloat)src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix(), _filterMesh->GetTransformation()->GetNormalMatrix());
	filterProgram->UpdateRenderingManagerProperies(this);
	
#ifdef WIN32
	glBindVertexArray(_filterMesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	filterProgram->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	filterProgram->DisableInputs();
#endif

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderingManager::ApplyEffectFromGBufferToGBuffer(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaGBuffer* dst)
{
	ANIMA_ASSERT(src != dst);

	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetGBuffer("FilterBuffer", src, false);

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix(), _filterMesh->GetTransformation()->GetNormalMatrix());
	filterProgram->UpdateRenderingManagerProperies(this);
	
#ifdef WIN32
	glBindVertexArray(_filterMesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	filterProgram->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	filterProgram->DisableInputs();
#endif
	
	SetGBuffer("FilterBuffer", nullptr, false);
}

void AnimaRenderingManager::ApplyEffectFromGBufferToTexture(AnimaShaderProgram* filterProgram, AnimaGBuffer* src, AnimaTexture* dst)
{
	if (dst != nullptr)
		dst->BindAsRenderTarget();
	else
	{
		AnimaVertex2f size = GetVector2f("ScreenSize");
		glViewport(0, 0, (AUint)size.x, (AUint)size.y);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	SetGBuffer("FilterBuffer", src, false);

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix(), _filterMesh->GetTransformation()->GetNormalMatrix());
	filterProgram->UpdateRenderingManagerProperies(this);
	
#ifdef WIN32
	glBindVertexArray(_filterMesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	filterProgram->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	filterProgram->DisableInputs();
#endif

	SetGBuffer("FilterBuffer", nullptr, false);
}

void AnimaRenderingManager::Start(AnimaScene* scene)
{
	scene->GetShadersManager()->SetActiveProgram(nullptr);
}

void AnimaRenderingManager::Finish(AnimaScene* scene)
{
	scene->GetShadersManager()->SetActiveProgram(nullptr);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AnimaRenderingManager::DeferredDrawAll(AnimaScene* scene)
{
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start(scene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	DeferredPreparePass(scene, shadersManager->GetProgramFromName("deferred-prepare"));
	
	Finish(scene);

	////
	////	Aggiorno il buffer per SSAO
	////
	//ApplyEffectFromGBufferToTexture(shadersManager->GetProgramFromName("ssao"), GetGBuffer("PrepassBuffer"), nullptr);
	
	////
	////	Aggiorno le mappature per le ombre
	////
	//Start(scene);
	//DeferredUpdateShadowMaps(scene, shadersManager->GetProgramFromName("deferred-shadowMap"));
	//Finish(scene);
	
	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start(scene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	boost::unordered_map<AnimaString, AnimaLightsMapData*, AnimaString::Hasher>* lights = lightsManager->GetLightsMap();
	for (auto pair : (*lights))
		DeferredLightPass(scene, pair.second->GetLightsArray());

	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish(scene);
	
	//
	//	Composizione dei buffer nell'immagine finale
	//
	if(_primitives.GetSize() <= 0)
	{
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start(scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		DeferredCombinePass(scene, shadersManager->GetProgramFromName("deferred-combine"));
		Finish(scene);
	}
	else
	{
		// Renderizzo l'immagine finale completa di luci in Diffuse2Map
		GetTexture("Diffuse2Map")->BindAsRenderTarget();
		Start(scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		DeferredCombinePass(scene, shadersManager->GetProgramFromName("deferred-combine"));
		Finish(scene);
		
		// Disegno le primitive in PrimitivesBuffer
		GetGBuffer("PrimitivesBuffer")->BindAsRenderTarget();
		Start(scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		DrawPrimitives(scene, shadersManager->GetProgramFromName("primitive-draw"));
		Finish(scene);
		
		// Compongo l'immagine finale prendendo la profondità da PrepassBuffer, il colore della scena da Diffuse2Map
		// ed il colore + profondità da PrimitivesBuffer
		GetTexture("DiffuseMap")->BindAsRenderTarget();
		Start(scene);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		CombinePrimitives(scene, shadersManager->GetProgramFromName("primitive-combine"));
		Finish(scene);
		
		ClearPrimitives();
	}
	
	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaScene* scene, AnimaMesh* model)
{
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	//
	//	Preparazione dei buffer iniziali
	//
	GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	Start(scene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	DeferredPreparePass(scene, shadersManager->GetProgramFromName("deferred-prepare"), model);

	Finish(scene);

	//
	//	Aggiorno le mappature per le ombre
	//
	Start(scene);
	DeferredUpdateShadowMaps(scene, shadersManager->GetProgramFromName("deferred-shadowMap"));
	Finish(scene);

	//
	//	Preparazione dei buffer della luce
	//
	GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	Start(scene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDepthMask(GL_FALSE);

	boost::unordered_map<AnimaString, AnimaLightsMapData*, AnimaString::Hasher>* lights = lightsManager->GetLightsMap();
	for (auto pair : (*lights))
		DeferredLightPass(scene, pair.second->GetLightsArray());

	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_ONE, GL_ZERO);
	Finish(scene);

	//
	//	Composizione dei buffer nell'immagine finale
	//
	GetTexture("DiffuseMap")->BindAsRenderTarget();
	Start(scene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DeferredCombinePass(scene, shadersManager->GetProgramFromName("deferred-combine"));
	Finish(scene);

	//
	//	Applicazione effetti
	//
	ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum)
{
	AnimaMatrix identityMatrix;
	DeferredDrawModel(scene, model, program, identityMatrix, identityMatrix, updateMaterial, forceDraw, frustum);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation, const AnimaMatrix& parentNormalMatrix, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum)
{
	if (scene == nullptr || model == nullptr || program == nullptr)
		return;
	
	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();
	AnimaMatrix normalMatrix = parentNormalMatrix * model->GetTransformation()->GetNormalMatrix();

	AInt meshNumber = model->GetMeshesNumber();
	for (AInt i = 0; i < meshNumber; i++)
		DeferredDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix, normalMatrix, updateMaterial, forceDraw, frustum);

	AInt childrenNumber = model->GetChildrenNumber();
	for (AInt i = 0; i < childrenNumber; i++)
		DeferredDrawModel(scene, model->GetChild(i), program, modelMatrix, normalMatrix, updateMaterial, forceDraw, frustum);
}

#define TEST

void AnimaRenderingManager::DeferredDrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation, const AnimaMatrix& parentNormalMatrix, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum)
{
#if defined TEST
	if (!forceDraw)
	{
		if (frustum != nullptr && !frustum->SphereInFrustum(parentTransformation * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
			return;
	}
#else
	if (!forceDraw && !mesh->IsVisible())
		return;
#endif

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	if (mesh->GetVertexArrayObject() <= 0)
		return;

	if (updateMaterial)
	{
		AnimaMaterial* material = mesh->GetMaterial();
		if (material == nullptr)
			return;

		program->UpdateMaterialProperies(material, this);
	}

	program->UpdateMeshProperies(mesh, parentTransformation, parentNormalMatrix);
	
#ifdef WIN32
	glBindVertexArray(mesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	program->EnableInputs(mesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaRenderingManager::DeferredPreparePass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model)
{
	if (model != nullptr)
		return;

	AnimaModelsManager* modelsManager = scene->GetModelsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	AnimaFrustum* frustum = nullptr;

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();
		program->UpdateCameraProperies(camera);

		frustum = camera->GetFrustum();
	}
	
	if (model == nullptr)
	{
		for (ASizeT j = 0; j < nModels; j++)
		{
			AnimaMesh* innerModel = modelsManager->GetModel(j);

			AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();
			AnimaMatrix normalMatrix = innerModel->GetTransformation()->GetNormalMatrix();

			DeferredDrawModelMesh(scene, innerModel, program, modelMatrix, normalMatrix, true, false, frustum);

			AInt meshNumber = innerModel->GetMeshesNumber();
			for (AInt i = 0; i < meshNumber; i++)
				DeferredDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix, normalMatrix, true, false, frustum);

			AInt childrenNumber = innerModel->GetChildrenNumber();
			for (AInt i = 0; i < childrenNumber; i++)
				DeferredDrawModel(scene, innerModel->GetChild(i), program, modelMatrix, normalMatrix, true, false, frustum);
		}
	}
	else
	{
		AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();
		AnimaMatrix normalMatrix = model->GetTransformation()->GetNormalMatrix();
		DeferredDrawModelMesh(scene, model, program, modelMatrix, normalMatrix, true, false, frustum);

		AInt meshNumber = model->GetMeshesNumber();
		for (AInt i = 0; i < meshNumber; i++)
			DeferredDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix, normalMatrix, true, false, frustum);

		AInt childrenNumber = model->GetChildrenNumber();
		for (AInt i = 0; i < childrenNumber; i++)
			DeferredDrawModel(scene, model->GetChild(i), program, modelMatrix, normalMatrix, true, false, frustum);
	}
}

void AnimaRenderingManager::DeferredLightPass(AnimaScene* scene, AnimaArray<AnimaLight*>* lights)
{
	AInt size = lights->GetSize();

	if (size == 0)
		return;
	
	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();
	AnimaCamera* activeCamera = scene->GetCamerasManager()->GetActiveCamera();

	if (activeCamera == nullptr)
		return;

	AnimaString type(typeid((*lights->ElementAt(0))).name(), _allocator);
	auto pair = _lightsMeshMap.find(type);

	ANIMA_ASSERT(pair != _lightsMeshMap.end());
	ANIMA_ASSERT(lights->ElementAt(0)->CreateShader(scene->GetShadersManager()));
	AnimaShaderProgram* program = scene->GetShadersManager()->GetProgramFromName(lights->ElementAt(0)->GetShaderName());
	AnimaMesh* mesh = pair->second;

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateCameraProperies(activeCamera);
	}

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	for (AInt i = 0; i < size; i++)
	{
		AnimaLight* light = lights->ElementAt(i);

		light->UpdateMeshTransformation(mesh->GetTransformation());
		light->UpdateCullFace(activeCamera);

		program->UpdateLightProperies(light, this);
		program->UpdateMeshProperies(mesh, mesh->GetTransformation()->GetTransformationMatrix(), mesh->GetTransformation()->GetNormalMatrix());
		program->UpdateRenderingManagerProperies(this);

#ifdef WIN32
		glBindVertexArray(mesh->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
#else
		program->EnableInputs(mesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
#endif
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}

void AnimaRenderingManager::DeferredCombinePass(AnimaScene* scene, AnimaShaderProgram* program)
{
	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateCameraProperies(_filterCamera);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	program->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix(), _filterMesh->GetTransformation()->GetNormalMatrix());
	program->UpdateRenderingManagerProperies(this);
	
#ifdef WIN32
	glBindVertexArray(_filterMesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	program->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaRenderingManager::DeferredUpdateShadowMaps(AnimaScene* scene, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = scene->GetLightsManager();
	AnimaModelsManager* modelsManager = scene->GetModelsManager();

	AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	if (lights == nullptr)
		return;

	AInt nLights = lights->GetSize();
	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nLights == 0 || nModels == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();
	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();

	for (AInt i = 0; i < nLights; i++)
	{
		AnimaLight* light = lights->ElementAt(i);

		//AnimaTexture* tmpShadowMap = light->GetTempShadowTexture();
		AnimaTexture* shadowMap = light->GetShadowTexture();
		//if (!tmpShadowMap->AreRenderTargetsReady())
		//	tmpShadowMap->LoadRenderTargets();
		if (!shadowMap->AreRenderTargetsReady())
			shadowMap->LoadRenderTargets();

		shadowMap->BindAsRenderTarget();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		program->UpdateLightProperies(light, this);
		program->UpdateRenderingManagerProperies(this);

		for (ASizeT j = 0; j < nModels; j++)
		{
			AnimaMesh* innerModel = modelsManager->GetModel(j);
			AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

			glCullFace(GL_FRONT);
			DeferredDrawModelMesh(scene, innerModel, program, modelMatrix, false, true);
			glCullFace(GL_BACK);

			AInt meshNumber = innerModel->GetMeshesNumber();
			for (AInt i = 0; i < meshNumber; i++)
			{
				glCullFace(GL_FRONT);
				DeferredDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix, false, true);
				glCullFace(GL_BACK);
			}

			AInt childrenNumber = innerModel->GetChildrenNumber();
			for (AInt i = 0; i < childrenNumber; i++)
				DeferredDrawModel(scene, innerModel->GetChild(i), program, modelMatrix, false, true);
		}

		//AFloat blurAmount = 1.0f;

		//SetVector("BlurScale", AnimaVertex3f(1.0f / shadowMap->GetWidth() * blurAmount, 0.0f, 0.0f));
		//ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussBlur7x1Filter"), shadowMap, tmpShadowMap);

		//SetVector("BlurScale", AnimaVertex3f(0.0f, 1.0f / shadowMap->GetHeight() * blurAmount, 0.0f));
		//ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussBlur7x1Filter"), tmpShadowMap, shadowMap);
	}
}

void AnimaRenderingManager::UpdateModelsVisibility(AnimaScene* scene)
{
	Anima::AnimaModelsManager* modelsManager = scene->GetModelsManager();
	Anima::AnimaCamerasManager* camerasManager = scene->GetCamerasManager();

	Anima::AnimaFrustum* frustum = camerasManager->GetActiveCamera()->GetFrustum();

	for (int i = 0; i < modelsManager->GetModelsNumber(); i++)
		UpdateModelVisibility(frustum, modelsManager->GetModel(i), Anima::AnimaMatrix());
}

void AnimaRenderingManager::UpdateModelVisibility(AnimaFrustum* frustum, AnimaMesh* mesh, AnimaMatrix parentMeshMatrix)
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

	AInt meshNumber = mesh->GetMeshesNumber();
	for (AInt i = 0; i < meshNumber; i++)
	{
		AnimaMesh* subMesh = mesh->GetMesh(i);
		subMesh->SetIsVisible(frustum->SphereInFrustum(parentMeshMatrix * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()));
	}

	AInt childrenNumber = mesh->GetChildrenNumber();
	for (AInt i = 0; i < childrenNumber; i++)
		UpdateModelVisibility(frustum, mesh->GetChild(i), modelMatrix);
}

void AnimaRenderingManager::AddPrimitive(AnimaArray<AnimaVertex3f>* vertices, AnimaArray<AUint>* indices, AnimaColor4f color, AnimaMatrix modelMatrix, AUint primitiveType)
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

void AnimaRenderingManager::DrawPrimitives(AnimaScene* scene, AnimaShaderProgram* program)
{
	if(program == nullptr)
		return;
	
	program->Use();
	program->SetUniform("_projectionViewMatrix", scene->GetCamerasManager()->GetActiveCamera()->GetProjectionViewMatrix());
	
	AInt nPrimitives = _primitives.GetSize();
	for(AInt i = 0; i < nPrimitives; i++)
		DrawPrimitive(_primitives[i], program);
}

void AnimaRenderingManager::DrawPrimitive(AnimaPrimitiveData* primitive, AnimaShaderProgram* program)
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

void AnimaRenderingManager::CombinePrimitives(AnimaScene* scene, AnimaShaderProgram* program)
{
	program->Use();
	program->UpdateRenderingManagerProperies(this);
	if(glGetError() != GL_NO_ERROR)
		return;
	
	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();
	
	program->Use();
	program->UpdateCameraProperies(_filterCamera);
	program->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix(), _filterMesh->GetTransformation()->GetNormalMatrix());
	program->UpdateRenderingManagerProperies(this);
	
#ifdef WIN32
	glBindVertexArray(_filterMesh->GetVertexArrayObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
#else
	program->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	program->DisableInputs();
#endif
}

void AnimaRenderingManager::SetTexture(AnimaString propertyName, AnimaTexture* value, bool deleteExistent)
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

void AnimaRenderingManager::SetTexture(const char* propertyName, AnimaTexture* value, bool deleteExistent)
{
	AnimaString str(propertyName, _allocator);
	SetTexture(str, value, deleteExistent);
}

void AnimaRenderingManager::SetTextureSlot(AnimaString propertyName, AUint value)
{
	_textureSlotsMap[propertyName] = value;
}

void AnimaRenderingManager::SetTextureSlot(const char* propertyName, AUint value)
{
	AnimaString str(propertyName, _allocator);
	SetTextureSlot(str, value);
}

void AnimaRenderingManager::SetGBuffer(const AnimaString& name, AnimaGBuffer* value, bool deleteExistent)
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

void AnimaRenderingManager::SetGBuffer(const char* name, AnimaGBuffer* value, bool deleteExistent)
{
	AnimaString str(name, _allocator);
	SetGBuffer(str, value, deleteExistent);
}

void AnimaRenderingManager::SetColor(AnimaString propertyName, AnimaColor3f value)
{
	_vectors3fMap[propertyName] = value;
}

void AnimaRenderingManager::SetColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaRenderingManager::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(r, g, b);
	SetColor(propertyName, color);
}

void AnimaRenderingManager::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, r, g, b);
}

void AnimaRenderingManager::SetColor(AnimaString propertyName, AnimaColor4f value)
{
	_vectors4fMap[propertyName] = value;
}

void AnimaRenderingManager::SetColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaRenderingManager::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	SetColor(propertyName, color);
}

void AnimaRenderingManager::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, r, g, b, a);
}

void AnimaRenderingManager::SetVector(AnimaString propertyName, AnimaVertex2f value)
{
	_vectors2fMap[propertyName] = value;
}

void AnimaRenderingManager::SetVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaRenderingManager::SetVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex2f vector(x, y);
	SetVector(propertyName, vector);
}

void AnimaRenderingManager::SetVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y);
}

void AnimaRenderingManager::SetVector(AnimaString propertyName, AnimaVertex3f value)
{
	_vectors3fMap[propertyName] = value;
}

void AnimaRenderingManager::SetVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaRenderingManager::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex3f vector(x, y, z);
	SetVector(propertyName, vector);
}

void AnimaRenderingManager::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y, z);
}

void AnimaRenderingManager::SetVector(AnimaString propertyName, AnimaVertex4f value)
{
	_vectors4fMap[propertyName] = value;
}

void AnimaRenderingManager::SetVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaRenderingManager::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	SetVector(propertyName, vector);
}

void AnimaRenderingManager::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y, z, w);
}

void AnimaRenderingManager::SetFloat(AnimaString propertyName, AFloat value)
{
	_floatsMap[propertyName] = value;
}

void AnimaRenderingManager::SetFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _allocator);
	SetFloat(str, value);
}

void AnimaRenderingManager::SetBoolean(AnimaString propertyName, bool value)
{
	_booleansMap[propertyName] = value;
}

void AnimaRenderingManager::SetBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _allocator);
	SetBoolean(str, value);
}

void AnimaRenderingManager::SetInteger(AnimaString propertyName, AInt value)
{
	_integersMap[propertyName] = value;
}

void AnimaRenderingManager::SetInteger(const char* propertyName, AInt value)
{
	AnimaString str(propertyName, _allocator);
	SetInteger(str, value);
}

AnimaTexture* AnimaRenderingManager::GetTexture(AnimaString propertyName)
{
	if (_texturesMap.find(propertyName) != _texturesMap.end())
		return _texturesMap[propertyName];
	return nullptr;
}

AnimaTexture* AnimaRenderingManager::GetTexture(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetTexture(str);
}

AUint AnimaRenderingManager::GetTextureSlot(const AnimaString& slotName)
{
	if (_textureSlotsMap.find(slotName) != _textureSlotsMap.end())
		return _textureSlotsMap[slotName];
	return 0;
}

AUint AnimaRenderingManager::GetTextureSlot(const char* slotName)
{
	AnimaString str(slotName, _allocator);
	return GetTextureSlot(str);
}

AnimaGBuffer* AnimaRenderingManager::GetGBuffer(const AnimaString& gBufferName)
{
	if (_gBuffersMap.find(gBufferName) != _gBuffersMap.end())
		return _gBuffersMap[gBufferName];
	return nullptr;
}

AnimaGBuffer* AnimaRenderingManager::GetGBuffer(const char* gBufferName)
{
	AnimaString str(gBufferName, _allocator);
	return GetGBuffer(str);
}

AnimaColor3f AnimaRenderingManager::GetColor3f(AnimaString propertyName)
{
	if (_vectors3fMap.find(propertyName) != _vectors3fMap.end())
		return _vectors3fMap[propertyName];

	AnimaColor3f color(0.0f, 0.0f, 0.0f);
	return color;
}

AnimaColor3f AnimaRenderingManager::GetColor3f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetColor3f(str);
}

AnimaColor4f AnimaRenderingManager::GetColor4f(AnimaString propertyName)
{
	if (_vectors4fMap.find(propertyName) != _vectors4fMap.end())
		return _vectors4fMap[propertyName];

	AnimaColor4f color(0.0f, 0.0f, 0.0f, 1.0f);
	return color;
}

AnimaColor4f AnimaRenderingManager::GetColor4f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetColor4f(str);
}

AnimaVertex2f AnimaRenderingManager::GetVector2f(AnimaString propertyName)
{
	if (_vectors2fMap.find(propertyName) != _vectors2fMap.end())
		return _vectors2fMap[propertyName];

	AnimaVertex2f vector(0.0f, 0.0f);
	return vector;
}

AnimaVertex2f AnimaRenderingManager::GetVector2f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector2f(str);
}

AnimaVertex3f AnimaRenderingManager::GetVector3f(AnimaString propertyName)
{
	if (_vectors3fMap.find(propertyName) != _vectors3fMap.end())
		return _vectors3fMap[propertyName];

	AnimaVertex3f vector(0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex3f AnimaRenderingManager::GetVector3f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector3f(str);
}

AnimaVertex4f AnimaRenderingManager::GetVector4f(AnimaString propertyName)
{
	if (_vectors4fMap.find(propertyName) != _vectors4fMap.end())
		return _vectors4fMap[propertyName];

	AnimaVertex4f vector(0.0f, 0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex4f AnimaRenderingManager::GetVector4f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector4f(str);
}

AFloat AnimaRenderingManager::GetFloat(AnimaString propertyName)
{
	propertyName = propertyName;
	if (_floatsMap.find(propertyName) != _floatsMap.end())
		return _floatsMap[propertyName];
	return 0.0;
}

AFloat AnimaRenderingManager::GetFloat(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetFloat(str);
}

bool AnimaRenderingManager::GetBoolean(AnimaString propertyName)
{
	propertyName = propertyName;
	if (_booleansMap.find(propertyName) != _booleansMap.end())
		return _booleansMap[propertyName];
	return false;
}

bool AnimaRenderingManager::GetBoolean(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetBoolean(str);
}

AInt AnimaRenderingManager::GetInteger(AnimaString propertyName)
{
	propertyName = propertyName;
	if (_integersMap.find(propertyName) != _integersMap.end())
		return _integersMap[propertyName];
	return 0;
}

AInt AnimaRenderingManager::GetInteger(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetInteger(str);
}

void AnimaRenderingManager::Clear()
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

void AnimaRenderingManager::ClearPrimitives()
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