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

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRenderingManager::AnimaRenderingManager(AnimaAllocator* allocator)
{
	_allocator = allocator;
	_filterCamera = nullptr;
	_filterMesh = nullptr;

	InitTextureSlots();
}

AnimaRenderingManager::AnimaRenderingManager(const AnimaRenderingManager& src)
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
}

AnimaRenderingManager::AnimaRenderingManager(AnimaRenderingManager&& src)
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
	}

	return *this;
}

void AnimaRenderingManager::InitTextureSlots()
{
	// Slot usati dai materiali
	SetTextureSlot("DiffuseTexture", 0);
	SetTextureSlot("BumpTexture", 1);
	SetTextureSlot("DisplacementTexture", 2);

	// Slot usati dal deferred shading
	SetTextureSlot("DepthMap", 0);
	SetTextureSlot("AlbedoMap", 1);
	SetTextureSlot("NormalMap", 2);
	SetTextureSlot("EmissiveMap", 3);
	SetTextureSlot("SpecularMap", 4);
	SetTextureSlot("ShadowMap", 5);

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
			prepassBuffer->AddTexture("DepthMap", GL_TEXTURE_2D, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
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
		AnimaTexture* diffuseTexture = GetTexture("DiffuseMap");
		if(diffuseTexture != nullptr)
			diffuseTexture->Resize(screenWidth, screenHeight);
		else
		{
			diffuseTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, screenWidth, screenHeight, nullptr, 0, 0, GL_NEAREST, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, GL_COLOR_ATTACHMENT0);
			ANIMA_ASSERT(diffuseTexture->LoadRenderTargets());
			SetTexture("DiffuseMap", diffuseTexture);
		}
		
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

	//
	// Inizializzazione delle mesh di supporto
	//
	_filterMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, _allocator);
	_filterMesh->MakePlane();
	_filterMesh->GetTransformation()->RotateXDeg(90.0f);

	_pointLightMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, _allocator);
	_pointLightMesh->MakeIcosahedralSphere(3);
		
	//
	// Inizializzazione della camera di supporto
	//
	_filterCamera = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*_allocator, _allocator, nullptr);
	_filterCamera->SetProjectionMatrix(AnimaMatrix());
	_filterCamera->SetViewMatrix(AnimaMatrix());
	_filterCamera->SetPosition(0.0f, 0.0f, 0.0f);
	_filterCamera->RotateYDeg(180.0f);
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
	SetVector("TextureSize", AnimaVertex2f(src->GetWidth(), src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
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
	SetVector("TextureSize", AnimaVertex2f(src->GetWidth(), src->GetHeight()));

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
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
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
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
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
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

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glGetError();
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
}

void AnimaRenderingManager::Finish(AnimaScene* scene)
{
	scene->GetShadersManager()->SetActiveProgram(nullptr);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AnimaRenderingManager::DrawAllModels(AnimaScene* scene)
{
	AnimaModelsManager* modelsManager = scene->GetModelsManager();
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* program = shadersManager->GetProgramFromName("phong");

	for (ASizeT j = 0; j < nModels; j++)
	{
		AnimaMesh* model = modelsManager->GetModel(j);

		AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

		ASizeT meshNumber = model->GetMeshesNumber();
		for (ASizeT i = 0; i < meshNumber; i++)
			DrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

		ASizeT childrenNumber = model->GetChildrenNumber();
		for (ASizeT i = 0; i < childrenNumber; i++)
			DrawModel(scene, model->GetChild(i), program, modelMatrix);
	}
}

void AnimaRenderingManager::DrawSingleModel(AnimaScene* scene, AnimaMesh* model)
{
	AnimaModelsManager* modelsManager = scene->GetModelsManager();
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* program = shadersManager->GetProgramFromName("phong");

	AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(scene, model->GetChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	DrawModel(scene, model, program, identityMatrix);
}

void AnimaRenderingManager::DrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (scene == nullptr || model == nullptr || program == nullptr)
		return;
	
	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(scene, model->GetChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (scene == nullptr || mesh == nullptr || program == nullptr)
		return;
	
	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
		program->UpdateLightsProperies(scene);
	}

	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;

	program->UpdateMeshProperies(mesh, parentTransformation);
	program->UpdateMaterialProperies(material, this);
	
	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	program->EnableInputs(mesh);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	program->DisableInputs();
}

void AnimaRenderingManager::ForwardDrawAllModels(AnimaScene* scene)
{
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();
	
	Start(scene);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGetError();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	ForwardAmbientPass(scene, shadersManager->GetProgramFromName("forward-ambient"));

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	ForwardDirectionalPass(scene, shadersManager->GetProgramFromName("forward-directional"));
	//ForwardPointPass(scene, shadersManager->GetProgramFromName("forward-point"));
	//ForwardSpotPass(scene, shadersManager->GetProgramFromName("forward-spot"));

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	Finish(scene);

	//ApplyFilter(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::ForwardDrawSingleModel(AnimaScene* scene, AnimaMesh* model)
{
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();

	//GetTexture("DiffuseMap")->BindAsRenderTarget();
	ForwardAmbientPass(scene, shadersManager->GetProgramFromName("forward-ambient"), model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	ForwardDirectionalPass(scene, shadersManager->GetProgramFromName("forward-directional"), model);
	ForwardPointPass(scene, shadersManager->GetProgramFromName("forward-point"), model);
	ForwardSpotPass(scene, shadersManager->GetProgramFromName("forward-spot"), model);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void AnimaRenderingManager::ForwardDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	ForwardDrawModel(scene, model, program, identityMatrix);
}

void AnimaRenderingManager::ForwardDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (scene == nullptr || model == nullptr || program == nullptr)
		return;

	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		ForwardDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		ForwardDrawModel(scene, model->GetChild(i), program, modelMatrix);
}

void AnimaRenderingManager::ForwardDrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;

	program->UpdateMeshProperies(mesh, parentTransformation);
	program->UpdateMaterialProperies(material, this);
		
	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	program->EnableInputs(mesh);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	program->DisableInputs();
}

void AnimaRenderingManager::ForwardAmbientPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model)
{
	if (model != nullptr)
		return;

	AnimaModelsManager* modelsManager = scene->GetModelsManager();
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nAmbLights = lightsManager->GetAmbientLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nAmbLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsAmbientLight())
			continue;

		program->UpdateLightProperies(light, this);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaMesh* innerModel = modelsManager->GetModel(j);

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ForwardDrawModelMesh(scene, innerModel, program, modelMatrix);

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(scene, innerModel->GetChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(scene, model->GetChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::ForwardDirectionalPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model)
{
	if (model != nullptr)
		return;

	AnimaModelsManager* modelsManager = scene->GetModelsManager();
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsDirectionalLight())
			continue;

		program->UpdateLightProperies(light, this);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaMesh* innerModel = modelsManager->GetModel(j);

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ForwardDrawModelMesh(scene, innerModel, program, modelMatrix);

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(scene, innerModel->GetChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(scene, model->GetChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::ForwardPointPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model)
{
	if (model != nullptr)
		return;

	AnimaModelsManager* modelsManager = scene->GetModelsManager();
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsPointLight())
			continue;

		program->UpdateLightProperies(light, this);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaMesh* innerModel = modelsManager->GetModel(j);

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(scene, innerModel->GetChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(scene, model->GetChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::ForwardSpotPass(AnimaScene* scene, AnimaShaderProgram* program, AnimaMesh* model)
{
	if (model != nullptr)
		return;

	AnimaModelsManager* modelsManager = scene->GetModelsManager();
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsSpotLight())
			continue;

		program->UpdateLightProperies(light, this);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaMesh* innerModel = modelsManager->GetModel(j);

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(scene, innerModel->GetChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(scene, model->GetChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::DeferredDrawAllModels(AnimaScene* scene)
{
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();

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
	DeferredDirectionalPass(scene, shadersManager->GetProgramFromName("deferred-directional"));
	DeferredPointPass(scene, shadersManager->GetProgramFromName("deferred-point"));
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

void AnimaRenderingManager::DeferredDrawSingleModel(AnimaScene* scene, AnimaMesh* model)
{
	Anima::AnimaShadersManager* shadersManager = scene->GetShadersManager();

	//GetTexture("DeferredInputMap")->BindAsRenderTarget();
	Start(scene);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	DeferredPreparePass(scene, shadersManager->GetProgramFromName("deferred-prepare"), model);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	Finish(scene);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//GetTexture("DiffuseMap")->BindAsRenderTarget();
	Start(scene);

	//glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	//glDepthMask(GL_FALSE);
	//glDepthFunc(GL_LEQUAL);

	//DeferredAmbientPass(scene, shadersManager->GetProgramFromName("deferred-ambient"));
	//DeferredDirectionalPass(scene, shadersManager->GetProgramFromName("deferred-directional"));
	//DeferredPointPass(scene, shadersManager->GetProgramFromName("deferred-point"));
	//DeferredSpotPass(scene, shadersManager->GetProgramFromName("deferred-spot"));

	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);

	Finish(scene);

	glDisable(GL_BLEND);

	//ApplyFilter(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, bool updateMaterial)
{
	AnimaMatrix identityMatrix;
	DeferredDrawModel(scene, model, program, identityMatrix, updateMaterial);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaScene* scene, AnimaMesh* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation, bool updateMaterial)
{
	if (scene == nullptr || model == nullptr || program == nullptr)
		return;

	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DeferredDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix, updateMaterial);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DeferredDrawModel(scene, model->GetChild(i), program, modelMatrix, updateMaterial);
}

void AnimaRenderingManager::DeferredDrawModelMesh(AnimaScene* scene, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation, bool updateMaterial)
{
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

	program->UpdateMeshProperies(mesh, parentTransformation);
	
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
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nAmbLights = lightsManager->GetAmbientLightsCount();

	if (nModels == 0 || nAmbLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = scene->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
	}

	if (model == nullptr)
	{
		for (ASizeT j = 0; j < nModels; j++)
		{
			AnimaMesh* innerModel = modelsManager->GetModel(j);

			AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

			DeferredDrawModelMesh(scene, innerModel, program, modelMatrix);

			ASizeT meshNumber = innerModel->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				DeferredDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix);

			ASizeT childrenNumber = innerModel->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				DeferredDrawModel(scene, innerModel->GetChild(i), program, modelMatrix);
		}
	}
	else
	{
		AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();
		DeferredDrawModelMesh(scene, model, program, modelMatrix);

		ASizeT meshNumber = model->GetMeshesNumber();
		for (ASizeT i = 0; i < meshNumber; i++)
			DeferredDrawModelMesh(scene, model->GetMesh(i), program, modelMatrix);

		ASizeT childrenNumber = model->GetChildrenNumber();
		for (ASizeT i = 0; i < childrenNumber; i++)
			DeferredDrawModel(scene, model->GetChild(i), program, modelMatrix);
	}
}

void AnimaRenderingManager::DeferredDirectionalPass(AnimaScene* scene, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = scene->GetLightsManager();
	
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();
	
	if (nDirLights == 0)
		return;
	
	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();
	AnimaCamera* activeCamera = scene->GetCamerasManager()->GetActiveCamera();
	
	if (activeCamera == nullptr)
		return;
	
	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateCameraProperies(_filterCamera);
		program->SetUniform("CAM_ProjectionViewInverseMatrix", activeCamera->GetInversedProjectionViewMatrix());
		program->SetUniform("CAM_Position", activeCamera->GetPosition());
	}
	
	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();
	
	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsDirectionalLight())
			continue;
		
		program->UpdateLightProperies(light, this);
		program->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
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
}

void AnimaRenderingManager::DeferredPointPass(AnimaScene* scene, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nPtLights = lightsManager->GetPointLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nPtLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();
	AnimaCamera* activeCamera = scene->GetCamerasManager()->GetActiveCamera();

	if (activeCamera == nullptr)
		return;

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateCameraProperies(activeCamera);
	}

	if (_pointLightMesh->NeedsBuffersUpdate())
		_pointLightMesh->UpdateBuffers();

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsPointLight())
			continue;

		AnimaVertex3f lPos = light->GetPosition();
		AFloat range = light->GetRange();

		AnimaMatrix m1, m2, m3;
		m1.Translate(lPos);
		m2.Scale(range, range, range, 1.0f);
		m3 = m1 * m2;
		
		float dist = (lPos - activeCamera->GetPosition()).Length();
		if (dist < light->GetRange())
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);

		program->UpdateLightProperies(light, this);
		program->UpdateMeshProperies(_pointLightMesh, m3);
		program->UpdateRenderingManagerProperies(this);
		
#ifdef WIN32
		glBindVertexArray(_pointLightMesh->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, _pointLightMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
#else
		program->EnableInputs(_pointLightMesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _pointLightMesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, _pointLightMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
#endif
	}
}

void AnimaRenderingManager::DeferredSpotPass(AnimaScene* scene, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = scene->GetLightsManager();

	ASizeT nSpLights = lightsManager->GetSpotLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nSpLights == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		program->Use();
		program->UpdateCameraProperies(_filterCamera);
	}

	if (_filterMesh->NeedsBuffersUpdate())
		_filterMesh->UpdateBuffers();

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsSpotLight())
			continue;

		program->UpdateLightProperies(light, this);
		program->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
		program->UpdateRenderingManagerProperies(this);

		program->EnableInputs(_filterMesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
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

	program->UpdateLightProperies(scene->GetLightsManager()->GetLightFromName("ambient"), this);
	program->UpdateMeshProperies(_filterMesh, _filterMesh->GetTransformation()->GetTransformationMatrix());
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
	AnimaShadersManager* shadersManager = scene->GetShadersManager();

	ASizeT nLights = lightsManager->GetTotalLightsCount();
	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nLights == 0 || nModels == 0)
		return;

	AnimaShaderProgram* activeProgram = scene->GetShadersManager()->GetActiveProgram();
	AnimaCamera* activeCamera = scene->GetCamerasManager()->GetActiveCamera();

	if (activeCamera == nullptr)
		return;

	if (activeProgram == nullptr || (*activeProgram) != (*program))
		program->Use();

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsDirectionalLight())
			continue;

		AnimaTexture* tmpShadowMap = light->GetTempShadowTexture();
		AnimaTexture* shadowMap = light->GetShadowTexture();
		if (!tmpShadowMap->AreRenderTargetsReady())
			tmpShadowMap->LoadRenderTargets();
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
			DeferredDrawModelMesh(scene, innerModel, program, modelMatrix, false);
			glCullFace(GL_BACK);

			ASizeT meshNumber = innerModel->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
			{
				glCullFace(GL_FRONT);
				DeferredDrawModelMesh(scene, innerModel->GetMesh(i), program, modelMatrix, false);
				glCullFace(GL_BACK);
			}

			ASizeT childrenNumber = innerModel->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				DeferredDrawModel(scene, innerModel->GetChild(i), program, modelMatrix, false);
		}

		AFloat blurAmount = 1.0f;

		SetVector("BlurScale", AnimaVertex3f(1.0f / shadowMap->GetWidth() * blurAmount, 0.0f, 0.0f));
		ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussBlur7x1Filter"), shadowMap, tmpShadowMap);

		SetVector("BlurScale", AnimaVertex3f(0.0f, 1.0f / shadowMap->GetHeight() * blurAmount, 0.0f));
		ApplyEffectFromTextureToTexture(shadersManager->GetProgramFromName("gaussBlur7x1Filter"), tmpShadowMap, shadowMap);
	}
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

//void AnimaRenderingManager::SetDeferredData(AnimaRenderingManager::DeferredData* value)
//{
//	_gBuffersMap.insert(value);
//}

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

//AUint AnimaRenderingManager::GetTextureIndex(const AnimaString& textureName)
//{
//	DeferredDataSetByName& nameIterator = get<1>(_gBuffersMap);
//	auto res = nameIterator.find(textureName);
//	if (res != nameIterator.end())
//		return (*res)->_index;
//	return 0;
//}
//
//AUint AnimaRenderingManager::GetTextureIndex(const char* textureName)
//{
//	AnimaString str(textureName, _allocator);
//	return GetTextureIndex(str);
//}

//AnimaRenderingManager::DeferredData* AnimaRenderingManager::GetDeferredData(const AnimaString& slotName)
//{
//	DeferredDataSetByName& nameIterator = get<1>(_gBuffersMap);
//	auto res = nameIterator.find(slotName);
//	if (res != nameIterator.end())
//		return *res;
//	return nullptr;
//}
//
//AnimaRenderingManager::DeferredData* AnimaRenderingManager::GetDeferredData(const char* slotName)
//{
//	AnimaString str(slotName, _allocator);
//	return GetDeferredData(str);
//}

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

	if (_pointLightMesh != nullptr)
	{
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, _pointLightMesh);
		_pointLightMesh = nullptr;
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
}

END_ANIMA_ENGINE_NAMESPACE