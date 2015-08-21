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
#include "AnimaMeshCreator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRenderer::AnimaRenderer(AnimaEngine* engine, AnimaAllocator* allocator)
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

	AddRenderPassFunction(PreparePass);
	AddRenderPassFunction(LightPass);
	AddRenderPassFunction(CombinePass);

	AnimaTexture* environmentTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator);
	AnimaTexture* irradianceTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator);

	environmentTexture->SetName("EnvironmentMap");
	irradianceTexture->SetName("IrradianceMap");

	SetTexture("EnvironmentMap", environmentTexture);
	SetTexture("IrradianceMap", irradianceTexture);
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

	_meshesMap = src._meshesMap;

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

	_meshesMap = src._meshesMap;

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

		_meshesMap = src._meshesMap;

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

		_meshesMap = src._meshesMap;

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
	SetTextureSlot("SkyBox", 6);
	SetTextureSlot("EnvironmentMap", 7);
	SetTextureSlot("IrradianceMap", 8);

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
			prepassBuffer->AddTexture("DepthMap", TEXTURE_TARGET_2D, TEXTURE_ATTACHMENT_DEPTH, TEXTURE_INTERNAL_FORMAT_DEPTH24, TEXTURE_FORMAT_DEPTH, TEXTURE_DATA_TYPE_FLOAT, TEXTURE_MIN_FILTER_MODE_NEAREST, TEXTURE_MAG_FILTER_MODE_NEAREST, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE, TEXTURE_CLAMP_TO_EDGE);
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
			diffuseTexture->SetMinFilter(TEXTURE_MIN_FILTER_MODE_NEAREST);
			diffuseTexture->SetMagFilter(TEXTURE_MAG_FILTER_MODE_NEAREST);
			diffuseTexture->SetInternalFormat(TEXTURE_INTERNAL_FORMAT_RGBA8);
			diffuseTexture->SetFormat(TEXTURE_FORMAT_RGBA);
			diffuseTexture->SetDataType(TEXTURE_DATA_TYPE_UNSIGNED_BYTE);
			diffuseTexture->SetClampS(TEXTURE_CLAMP_TO_EDGE);
			diffuseTexture->SetClampT(TEXTURE_CLAMP_TO_EDGE);
			diffuseTexture->SetClampR(TEXTURE_CLAMP_TO_EDGE);
			diffuseTexture->SetAttachment(TEXTURE_ATTACHMENT_COLOR0);

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

		printf("New screen size: %.0f x %.0f\n", (AFloat)screenWidth, (AFloat)screenHeight);
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
	SetColor("AmbientLight", 0.1f, 0.1f, 0.1f);
	//SetColor("BackColor", 0.3f, 0.3f, 0.3f, 1.0f);
	SetColor("BackColor", 0.0f, 0.0f, 0.0f, 1.0f);

	SetVector("SSAOFilterRadius", 5.0f, 5.0f);
	SetFloat("SSAODistanceThreshold", 5.0f);
	
	//
	// Inizializzazione delle mesh di supporto
	//
	AnimaString name = "filter_RENMESH";
	_filterMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, name, _engine->GetDataGeneratorsManager(), _allocator);
	_filterMesh->MakePlane();
	_filterMesh->GetTransformation()->RotateXDeg(90.0f);

	//AnimaString nameSkyMesh = "skybox_RENMESH";
	//AnimaMesh* skyMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, nameSkyMesh, _engine->GetDataGeneratorsManager(), _allocator);
	//skyMesh->MakeCube();
	//skyMesh->GetTransformation()->RotateZDeg(180.0f);
	//_meshesMap[nameSkyMesh] = skyMesh;

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
	_filterCamera = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*_allocator, _allocator, nullptr, _engine->GetDataGeneratorsManager(), AnimaString("$$filterCamera$$"));
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

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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

void AnimaRenderer::Render()
{
	for (auto func : _renderPassesFunction)
		func(this);
}

void AnimaRenderer::DrawMesh(AnimaMesh* mesh, AnimaShaderProgram* program, bool updateMaterial, bool forceDraw, AnimaFrustum* frustum, bool useInstances)
{
	AnimaShaderProgram* originalProgram = program;
	if (!useInstances || mesh->GetInstancesCount() == 0)
	{
		AnimaString shaderProgramName = mesh->GetShaderProgramName();
		if (shaderProgramName.empty())
		{
			AnimaMaterial* material = mesh->GetMaterial();
			if (material == nullptr)
				material = _scene->GetMaterialsManager()->GetDefaultMaterial();

			if (material)
			{
				program = _scene->GetShadersManager()->CreateProgram(mesh, material);
				if (program)
					program->Link();
			}
		}
		else
			program = _scene->GetShadersManager()->GetProgramFromName(mesh->GetShaderProgramName());

		if (program == nullptr)
			program = originalProgram;

		AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
		if (activeProgram == nullptr || (*activeProgram) != (*program))
		{
			AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
			if (camera == nullptr)
				return;

			program->Use();
			program->UpdateSceneObjectProperties(camera, this);

			frustum = camera->GetFrustum();
		}

		AnimaTransformation* meshTransfomation = mesh->GetTransformation();

		if (!forceDraw)
		{
			if (frustum != nullptr && !frustum->SphereInFrustum(meshTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
				return;
		}

		AnimaModel* meshParent = (AnimaModel*)mesh->GetParentObject()->GetAncestorObject();
		if (meshParent != nullptr)
		{
			if (meshParent != _lastUpdatedModel)
			{
				_lastUpdatedModel = meshParent;
				program->UpdateSceneObjectProperties(_lastUpdatedModel, this);
			}
		}

		if (mesh->NeedsBuffersUpdate())
			mesh->UpdateBuffers();

		if (mesh->GetVertexArrayObject() <= 0)
			return;

		mesh->Draw(this, program, updateMaterial);
	}
	else
	{
		ANIMA_FRAME_PUSH("BuffersUpdate");
		if (mesh->NeedsBuffersUpdate())
			mesh->UpdateBuffers();
		ANIMA_FRAME_POP();

		if (mesh->GetVertexArrayObject() <= 0)
			return;

		AInt instancesCount = mesh->GetInstancesCount();
		for (AInt i = 0; i < instancesCount; i++)
		{
			ANIMA_FRAME_PUSH("InstanceGet");
			AnimaMeshInstance* instance = mesh->GetInstance(i);
			ANIMA_FRAME_POP();

			ANIMA_FRAME_PUSH("ProgramGet");
			AnimaString shaderProgramName = instance->GetShaderProgramName();
			if (shaderProgramName.empty())
			{
				AnimaMaterial* material = instance->GetMaterial();
				if (material == nullptr)
					material = _scene->GetMaterialsManager()->GetDefaultMaterial();

				if (material)
				{
					program = _scene->GetShadersManager()->CreateProgram(instance, material);
					if (program)
						program->Link();
				}
			}
			else
				program = _scene->GetShadersManager()->GetProgramFromName(instance->GetShaderProgramName());

			if (program == nullptr)
				program = originalProgram;
			ANIMA_FRAME_POP();

			ANIMA_FRAME_PUSH("ActiveProgramGet");
			AnimaShaderProgram* activeProgram = _scene->GetShadersManager()->GetActiveProgram();
			ANIMA_FRAME_POP();

			ANIMA_FRAME_PUSH("ActiveProgramCheck");
			if (activeProgram == nullptr || (*activeProgram) != (*program))
			{
				ANIMA_FRAME_PUSH("ActiveCameraGet");
				AnimaCamera* camera = _scene->GetCamerasManager()->GetActiveCamera();
				ANIMA_FRAME_POP();
				if (camera == nullptr)
					return;

				ANIMA_FRAME_PUSH("ProgramAtivation");
				program->Use();
				ANIMA_FRAME_POP();

				ANIMA_FRAME_PUSH("ProgramUpdateCamera");
				program->UpdateSceneObjectProperties(camera, this);
				ANIMA_FRAME_POP();

				ANIMA_FRAME_PUSH("CameraGetFrustum");
				frustum = camera->GetFrustum();
				ANIMA_FRAME_POP();

				ANIMA_FRAME_POP();
			}
			else
			{
				ANIMA_FRAME_POP();
			}

			ANIMA_FRAME_PUSH("InstanceTransformationGet");
			AnimaTransformation* instanceTransfomation = instance->GetTransformation();
			ANIMA_FRAME_POP();
			if (!forceDraw)
			{
				ANIMA_FRAME_PUSH("FrustumCheck");
				if (frustum != nullptr && !frustum->SphereInFrustum(instanceTransfomation->GetTransformationMatrix() * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
				{
					ANIMA_FRAME_POP();
					continue;
				}
				ANIMA_FRAME_POP();
			}

			ANIMA_FRAME_PUSH("InstanceModelGet");
			AnimaModelInstance* instanceParent = (AnimaModelInstance*)instance->GetParentObject()->GetAncestorObject();
			ANIMA_FRAME_POP();
			if (instanceParent != nullptr)
			{
				if (instanceParent != _lastUpdatedModelInstance)
				{
					_lastUpdatedModelInstance = instanceParent;
					ANIMA_FRAME_PUSH("InstanceModelGetUpdate");
					program->UpdateSceneObjectProperties(_lastUpdatedModelInstance, this);
					ANIMA_FRAME_POP();
				}
			}

			ANIMA_FRAME_PUSH("InstanceDraw");
			instance->Draw(this, program, updateMaterial);
			ANIMA_FRAME_POP();
		}
	}
}

void AnimaRenderer::PreparePass(AnimaRenderer* renderer)
{
	ANIMA_FRAME_PUSH("DrawAll");
	Anima::AnimaShadersManager* shadersManager = renderer->_scene->GetShadersManager();
	AnimaVertex4f backColor = renderer->GetColor4f("BackColor");

	ANIMA_FRAME_PUSH("PreparePass");
	renderer->GetGBuffer("PrepassBuffer")->BindAsRenderTarget();
	renderer->Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	AnimaMeshesManager* meshesManager = renderer->_scene->GetMeshesManager();
	AInt meshesCount = meshesManager->GetMeshesCount();
	if (meshesCount == 0)
		return;

	AnimaFrustum* frustum = nullptr;
	for (AInt i = 0; i < meshesCount; i++)
	{
		ANIMA_FRAME_PUSH("MeshGet");
		AnimaMesh* mesh = meshesManager->GetMesh(i);
		ANIMA_FRAME_POP();

		ANIMA_FRAME_PUSH("MeshDraw");
		renderer->DrawMesh(mesh, shadersManager->GetProgramFromName("deferred-prepare"), true, false, frustum);
		ANIMA_FRAME_POP();
	}

	renderer->Finish();
	ANIMA_FRAME_POP();
}

void AnimaRenderer::LightPass(AnimaRenderer* renderer)
{
	AnimaCamera* activeCamera = renderer->_scene->GetCamerasManager()->GetActiveCamera();
	if (activeCamera == nullptr)
		return;

	ANIMA_FRAME_PUSH("LightPass");

	ANIMA_FRAME_PUSH("BindTarget");
	renderer->GetGBuffer("LightsBuffer")->BindAsRenderTarget();
	ANIMA_FRAME_POP();

	ANIMA_FRAME_PUSH("Setup");
	renderer->Start();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	ANIMA_FRAME_POP();

	ANIMA_FRAME_PUSH("Lightning");
	AnimaLightsManager* lightsManager = renderer->_scene->GetLightsManager();
	AnimaShadersManager* shadersManager = renderer->_scene->GetShadersManager();
	AnimaTypeMappedArray<AnimaLight*>* lights = lightsManager->GetLights();
	boost::unordered_map<AnimaString, AnimaMappedArray<AnimaLight*>*, AnimaStringHasher>* lightsMap = lights->GetArraysMap();
	
	AnimaShaderProgram* activeProgram = nullptr;

	for (auto pair : (*lightsMap))
	{
		AnimaArray<AnimaLight*>* lightsArray = pair.second->GetArray();
		AInt size = lightsArray->size();
		if (size == 0)
			continue;
		
		AnimaString type = typeid((*lightsArray->at(0))).name();
		auto lightMeshPair = renderer->_meshesMap.find(type);

		ANIMA_ASSERT(lightMeshPair != renderer->_meshesMap.end());
		ANIMA_ASSERT(lightsArray->at(0)->CreateShader(renderer->_scene->GetShadersManager()));

		AnimaString name = lightsArray->at(0)->_GetClassName();

		AnimaShaderProgram* program = shadersManager->GetProgramFromName(lightsArray->at(0)->GetShaderName());
		AnimaMesh* mesh = lightMeshPair->second;

		activeProgram = renderer->_scene->GetShadersManager()->GetActiveProgram();

		if (activeProgram == nullptr || (*activeProgram) != (*program))
		{
			program->Use();
			program->UpdateSceneObjectProperties(activeCamera, renderer);
		}

		if (mesh->NeedsBuffersUpdate())
			mesh->UpdateBuffers();

		for (AInt i = 0; i < size; i++)
		{
			AnimaLight* light = lightsArray->at(i);

			light->UpdateMeshTransformation(mesh->GetTransformation());
			light->UpdateCullFace(activeCamera);

			program->UpdateSceneObjectProperties(light, renderer);
			program->UpdateRenderingManagerProperies(renderer);

			mesh->Draw(renderer, program, false);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
	}
	ANIMA_FRAME_POP();

	ANIMA_FRAME_PUSH("Finish");
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_ONE, GL_ZERO);
	renderer->Finish();
	ANIMA_FRAME_POP();

	ANIMA_FRAME_POP();
}

void AnimaRenderer::CombinePass(AnimaRenderer* renderer)
{
	AnimaVertex4f backColor = renderer->GetColor4f("BackColor");
	Anima::AnimaShadersManager* shadersManager = renderer->_scene->GetShadersManager();

	//renderer->ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("nullFilter"), renderer->GetGBuffer("PrepassBuffer")->GetTexture("AlbedoMap"), nullptr);
	//return;

	renderer->Start();
	AnimaVertex2f size = renderer->GetVector2f("ScreenSize");
	glViewport(0, 0, (AUint)size.x, (AUint)size.y);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);

	AnimaTexture* skyTexture = renderer->GetTexture("SkyBox");
	if (skyTexture != nullptr)
	{
		AnimaMesh* skyMesh = renderer->_meshesMap["skybox_RENMESH"];
		if (skyMesh != nullptr)
		{
			AnimaShaderProgram* skyProgram = shadersManager->GetProgramFromName("skybox");
			AnimaCamera* camera = renderer->_scene->GetCamerasManager()->GetActiveCamera();

			skyProgram->Use();
			skyProgram->UpdateSceneObjectProperties(camera, renderer);
			
			if (skyMesh->NeedsBuffersUpdate())
				skyMesh->UpdateBuffers();

			skyMesh->GetTransformation()->SetTranslation(camera->GetPosition());

			skyProgram->UpdateRenderingManagerProperies(renderer);

			GLint OldCullFaceMode;
			glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
			GLint OldDepthFuncMode;
			glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);
			
			glCullFace(GL_FRONT);
			glDepthFunc(GL_LEQUAL);

			skyMesh->Draw(renderer, skyProgram, true);

			glCullFace(OldCullFaceMode);
			glDepthFunc(OldDepthFuncMode);
		}
	}
	else
	{
		AnimaShaderProgram* activeProgram = renderer->_scene->GetShadersManager()->GetActiveProgram();
		AnimaShaderProgram* program = shadersManager->GetProgramFromName("deferred-combine");

		if (activeProgram == nullptr || (*activeProgram) != (*program))
		{
			program->Use();
			program->UpdateSceneObjectProperties(renderer->_filterCamera, renderer);
		}

		if (renderer->_filterMesh->NeedsBuffersUpdate())
			renderer->_filterMesh->UpdateBuffers();

		program->UpdateRenderingManagerProperies(renderer);
		renderer->_filterMesh->Draw(renderer, program, false);
	}

	renderer->Finish();
}

void AnimaRenderer::UpdateShadowMaps(AnimaShaderProgram* program)
{
	//AnimaLightsManager* lightsManager = _scene->GetLightsManager();
	//AnimaMeshesManager* meshesManager = _scene->GetMeshesManager();

	//AnimaArray<AnimaLight*>* lights = lightsManager->GetLightsArrayOfType<AnimaDirectionalLight>();

	//if (lights == nullptr)
	//	return;

	//AInt nLights = lights->size();
	//AInt nMeshes = meshesManager->GetMeshesCount();

	//if (nLights == 0 || nMeshes == 0)
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

	//	for (AInt j = 0; j < nMeshes; j++)
	//	{
	//		DrawMesh(meshesManager->GetMesh(j), program, false, true, nullptr, true);
	//		//	AnimaMesh* innerModel = modelsManager->GetModel(j);
	//		//	AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();
	//		//	
	//		//	glCullFace(GL_FRONT);
	//		//	DrawModelMesh(_scene, innerModel, program, modelMatrix, false, true);
	//		//	glCullFace(GL_BACK);
	//		//	
	//		//	AInt meshCount = innerModel->GetMeshesCount();
	//		//	for (AInt i = 0; i < meshCount; i++)
	//		//	{
	//		//		glCullFace(GL_FRONT);
	//		//		DrawModelMesh(_scene, innerModel->GetMesh(i), program, modelMatrix, false, true);
	//		//		glCullFace(GL_BACK);
	//		//	}
	//		//	
	//		//	AInt childrenCount = innerModel->GetChildrenCount();
	//		//	for (AInt i = 0; i < childrenCount; i++)
	//		//		DrawModel(_scene, innerModel->GetChild(i), program, modelMatrix, false, true);
	//	}
	//}
}

void AnimaRenderer::UpdateModelsVisibility()
{
//	Anima::AnimaModelsManager* modelsManager = _scene->GetModelsManager();
//	Anima::AnimaCamerasManager* camerasManager = _scene->GetCamerasManager();
//
//	Anima::AnimaFrustum* frustum = camerasManager->GetActiveCamera()->GetFrustum();

	//	for (int i = 0; i < modelsManager->GetModelsCount(); i++)
	//		UpdateModelVisibility(frustum, modelsManager->GetModel(i), Anima::AnimaMatrix());
}

void AnimaRenderer::UpdateModelVisibility(AnimaFrustum* frustum, AnimaMesh* mesh, AnimaMatrix parentMeshMatrix)
{
	AnimaMatrix modelMatrix = parentMeshMatrix * mesh->GetTransformation()->GetTransformationMatrix();

	if (mesh->GetVerticesCount() > 0)
	{
		if (frustum->SphereInFrustum(parentMeshMatrix * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()))
			mesh->SetIsVisible(true);
		else
		{
			mesh->SetIsVisible(false);
			return;
		}
	}

	//AInt meshCount = mesh->GetMeshesCount();
	//for (AInt i = 0; i < meshCount; i++)
	//{
	//	AnimaMesh* subMesh = mesh->GetMesh(i);
	//	subMesh->SetIsVisible(frustum->SphereInFrustum(parentMeshMatrix * mesh->GetBoundingBoxCenter(), (mesh->GetBoundingBoxMin() - mesh->GetBoundingBoxMax()).Length()));
	//}

	//AInt childrenCount = mesh->GetChildrenCount();
	//for (AInt i = 0; i < childrenCount; i++)
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
	
	_primitives.push_back(primitive);
}

void AnimaRenderer::DrawPrimitives( AnimaShaderProgram* program)
{
	if(program == nullptr)
		return;
	
	program->Use();
	program->SetUniform("_projectionViewMatrix", _scene->GetCamerasManager()->GetActiveCamera()->GetProjectionViewMatrix());
	
	AInt nPrimitives = _primitives.size();
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
	
	AInt nVertices = vertices->size();
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
	
	if (indices->size() > 0)
	{
		AInt nIndici = indices->size();
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
	
	for (auto& pair : _meshesMap)
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

	_meshesMap.clear();
	
	ClearPrimitives();

	_renderPassesFunction.clear();
}

void AnimaRenderer::ClearPrimitives()
{
	AInt nPrimitives = _primitives.size();
	for(AInt i = 0; i < nPrimitives; i++)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _primitives[i]);
		_primitives[i] = nullptr;
	}
	
	_primitives.clear();
}

bool AnimaRenderer::InitializeShaders(const AnimaString& shadersPath, const AnimaString& shadersPartsPath)
{
	Anima::AnimaShadersManager* shadersManager = _engine->GetShadersManager();
	shadersManager->LoadShadersParts(shadersPartsPath);

	Anima::AnimaShaderProgram* prepareProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/static-mesh-base-material-pbr.asp");
	if (!prepareProgram->Link())
		return false;

	Anima::AnimaShaderProgram* directionalLightProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/directional-light-pbr.asp");
	if (!directionalLightProgram->Link())
		return false;

	Anima::AnimaShaderProgram* skyboxProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/skybox.asp");
	if (!skyboxProgram->Link())
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

	Anima::AnimaShaderProgram* nullFilterProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/nullFilter.asp");
	if (!nullFilterProgram->Link())
		return false;

	Anima::AnimaShaderProgram* combineProgram = shadersManager->LoadShaderProgramFromFile(shadersPath + "Shaders/combine-pbr.asp");
	if (!combineProgram->Link())
		return false;
	
	return true;
}

AnimaPrimitiveData::AnimaPrimitiveData(AnimaAllocator* allocator)
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
	_vertices.clear();
	_indices.clear();
}

void AnimaPrimitiveData::SetVertices(AnimaArray<AnimaVertex3f>* vertices)
{
	_vertices.clear();
	
	if(vertices != nullptr)
	{
		AInt vSize = vertices->size();
		for(AInt i = 0; i < vSize; i++)
			_vertices.push_back(vertices->at(i));
	}
}

AnimaArray<AnimaVertex3f>* AnimaPrimitiveData::GetVertices()
{
	return &_vertices;
}

void AnimaPrimitiveData::SetIndices(AnimaArray<AUint>* indices)
{
	_indices.clear();
	
	if(indices != nullptr)
	{
		AInt vSize = indices->size();
		for(AInt i = 0; i < vSize; i++)
			_indices.push_back(indices->at(i));
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