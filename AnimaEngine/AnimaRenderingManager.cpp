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
	_deferredDataMap = src._deferredDataMap;

	_colors3fMap = src._colors3fMap;
	_colors4fMap = src._colors4fMap;
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
	_deferredDataMap = src._deferredDataMap;

	_colors3fMap = src._colors3fMap;
	_colors4fMap = src._colors4fMap;
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
		_deferredDataMap = src._deferredDataMap;

		_colors3fMap = src._colors3fMap;
		_colors4fMap = src._colors4fMap;
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
		_deferredDataMap = src._deferredDataMap;

		_colors3fMap = src._colors3fMap;
		_colors4fMap = src._colors4fMap;
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
	SetTextureSlot("AlbedoMap", 0);
	SetTextureSlot("NormalMap", 1);

	// Slot usati dai filtri
	SetTextureSlot("FilterMap", 0);

	// Dati usati dal deferred shading
	DeferredData* albedoData = AnimaAllocatorNamespace::AllocateNew<DeferredData>(*_allocator, 1, GL_COLOR_ATTACHMENT0, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
	DeferredData* normalData = AnimaAllocatorNamespace::AllocateNew<DeferredData>(*_allocator, 2, GL_COLOR_ATTACHMENT0 + 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
	DeferredData* depthData = AnimaAllocatorNamespace::AllocateNew<DeferredData>(*_allocator, 0, GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, GL_NEAREST);

	SetDeferredData("AlbedoMap", albedoData);
	SetDeferredData("NormalMap", normalData);
	SetDeferredData("DepthMap", depthData);
}

void AnimaRenderingManager::InitRenderingTargets(AInt screenWidth, AInt screenHeight)
{
	AnimaVertex2f oldSize = GetVector2f("ScreenSize");
	AnimaVertex2f newSize((AFloat)screenWidth, (AFloat)screenHeight);

	if (oldSize != newSize)
	{
		AUint* attachments = nullptr;
		AUint* formats = nullptr;
		AUint* internalFormats = nullptr;
		AUint* dataTypes = nullptr;
		AUint* filters = nullptr;

		ASizeT size = _deferredDataMap.size();
		if (size > 0)
		{
			attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, size);
			formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, size);
			internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, size);
			dataTypes = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, size);
			filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, size);
		}

		AInt offset = 0;
		auto end = _deferredDataMap.end();
		for (auto pair = _deferredDataMap.begin(); pair != end; pair++)
		{
			DeferredData* data = pair->second;
			attachments[offset] = data->_attachment;
			formats[offset] = data->_format;
			internalFormats[offset] = data->_internalFormat;
			dataTypes[offset] = data->_dataType;
			filters[offset] = data->_filter;

			offset++;
		}

		AnimaTexture* diffuseMapTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, screenWidth, screenHeight, nullptr, 0, 0, GL_NEAREST, GL_RGBA, GL_RGBA, GL_FLOAT, false, GL_COLOR_ATTACHMENT0);
		diffuseMapTexture->LoadRenderTargets();
		
		AnimaTexture* deferredMapTexture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, GL_TEXTURE_2D, screenWidth, screenHeight, (AUint)size, nullptr, nullptr, 0, filters, internalFormats, formats, dataTypes, false, attachments);
		deferredMapTexture->LoadRenderTargets();

		SetTexture("DiffuseMap", diffuseMapTexture);
		SetTexture("DeferredMap", deferredMapTexture);
		SetTexture("FilterMap", nullptr, false);

		SetVector("ScreenSize", (AFloat)screenWidth, (AFloat)screenHeight);
		SetVector("InverseScreenSize", 1.0f / (AFloat)screenWidth, 1.0f / (AFloat)screenHeight);

		if (attachments != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, attachments);
			attachments = nullptr;
		}

		if (formats != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, formats);
			formats = nullptr;
		}

		if (internalFormats != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, internalFormats);
			internalFormats = nullptr;
		}

		if (dataTypes != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, dataTypes);
			dataTypes = nullptr;
		}

		if (filters != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, filters);
			filters = nullptr;
		}
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
	// Inizializzazione del modello di supporto
	//
	_filterMesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*_allocator, _allocator);
	_filterMeshTransform.RotateXDeg(90.0f);
	
	AInt numeroVertici = 4;
	AInt numeroFacce = 2;
	AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, numeroVertici);
	AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*_allocator, numeroVertici);
	AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*_allocator, numeroVertici);
	AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*_allocator, numeroFacce, _allocator);

	vertici[0] = AnimaVertex3f(-1.0f, 0.0f, -1.0f);
	vertici[1] = AnimaVertex3f(-1.0f, 0.0f, 1.0f);
	vertici[2] = AnimaVertex3f(1.0f, 0.0f, 1.0f);
	vertici[3] = AnimaVertex3f(1.0f, 0.0f, -1.0f);

	normali[0] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	normali[1] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	normali[2] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	normali[3] = AnimaVertex3f(0.0f, 1.0f, 0.0f);

	textCoords[0] = AnimaVertex2f(1.0f, 0.0f);
	textCoords[1] = AnimaVertex2f(1.0f, 1.0f);
	textCoords[2] = AnimaVertex2f(0.0f, 1.0f);
	textCoords[3] = AnimaVertex2f(0.0f, 0.0f);

	AUint indexes0[] = { 0, 1, 2 };
	AUint indexes1[] = { 0, 2, 3 };
	facce[0].SetIndexes(indexes0, 3);
	facce[1].SetIndexes(indexes1, 3);

	_filterMesh->SetVertices(vertici, numeroVertici);
	_filterMesh->SetNormals(normali, numeroVertici);
	_filterMesh->SetTextureCoords(textCoords, numeroVertici);
	_filterMesh->SetFaces(facce, numeroFacce);

	AnimaAllocatorNamespace::DeallocateArray(*_allocator, vertici);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, normali);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, textCoords);
	AnimaAllocatorNamespace::DeallocateArray(*_allocator, facce);
	
	//
	// Inizializzazione della camera di supporto
	//
	_filterCamera = AnimaAllocatorNamespace::AllocateNew<AnimaFirstPersonCamera>(*_allocator, _allocator, nullptr);
	_filterCamera->SetProjectionMatrix(AnimaMatrix());
	_filterCamera->SetPosition(0.0f, 0.0f, 0.0f);
	_filterCamera->RotateYDeg(180.0f);
}

void AnimaRenderingManager::ApplyFilter(AnimaShaderProgram* filterProgram, AnimaTexture* src, AnimaTexture* dst)
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

	glClear(GL_DEPTH_BUFFER_BIT);
	filterProgram->Use();
	filterProgram->UpdateCameraProperies(_filterCamera);
	filterProgram->UpdateMeshProperies(_filterMesh, _filterMeshTransform.GetTransformationMatrix());
	filterProgram->UpdateRenderingManagerProperies(this);
	
	filterProgram->EnableInputs(_filterMesh);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
	filterProgram->DisableInputs();

	SetTexture("FilterMap", nullptr, false);
}

void AnimaRenderingManager::Start(AnimaStage* stage)
{
	stage->GetShadersManager()->SetActiveProgram(nullptr);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glGetError();
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	//glEnable(GL_DEPTH_TEST);
}

void AnimaRenderingManager::Finish(AnimaStage* stage)
{
	stage->GetShadersManager()->SetActiveProgram(nullptr);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AnimaRenderingManager::DrawAllModels(AnimaStage* stage)
{
	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* program = shadersManager->GetProgramFromName("phong");

	for (ASizeT j = 0; j < nModels; j++)
	{
		AnimaModel* model = modelsManager->GetPModel(j);

		if (!model->DrawingEnabled())
			continue;

		AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

		ASizeT meshNumber = model->GetMeshesNumber();
		for (ASizeT i = 0; i < meshNumber; i++)
			DrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

		ASizeT childrenNumber = model->GetChildrenNumber();
		for (ASizeT i = 0; i < childrenNumber; i++)
			DrawModel(stage, model->GetPChild(i), program, modelMatrix);
	}
}

void AnimaRenderingManager::DrawSingleModel(AnimaStage* stage, AnimaModel* model)
{
	if (!model->DrawingEnabled())
		return;

	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* program = shadersManager->GetProgramFromName("phong");

	AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(stage, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	DrawModel(stage, model, program, identityMatrix);
}

void AnimaRenderingManager::DrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (!model->DrawingEnabled())
		return;

	if (stage == nullptr || model == nullptr || program == nullptr)
		return;
	
	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(stage, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (stage == nullptr || mesh == nullptr || program == nullptr)
		return;
	
	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = stage->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(camera);
		program->UpdateLightsProperies(stage);
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

void AnimaRenderingManager::ForwardDrawAllModels(AnimaStage* stage)
{
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();
	
	GetTexture("DiffuseMap")->BindAsRenderTarget();
	Start(stage);

	ForwardAmbientPass(stage, shadersManager->GetProgramFromName("forward-ambient"));

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	ForwardDirectionalPass(stage, shadersManager->GetProgramFromName("forward-directional"));
	ForwardPointPass(stage, shadersManager->GetProgramFromName("forward-point"));
	ForwardSpotPass(stage, shadersManager->GetProgramFromName("forward-spot"));

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	Finish(stage);

	ApplyFilter(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::ForwardDrawSingleModel(AnimaStage* stage, AnimaModel* model)
{
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	GetTexture("DiffuseMap")->BindAsRenderTarget();
	ForwardAmbientPass(stage, shadersManager->GetProgramFromName("forward-ambient"), model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	ForwardDirectionalPass(stage, shadersManager->GetProgramFromName("forward-directional"), model);
	ForwardPointPass(stage, shadersManager->GetProgramFromName("forward-point"), model);
	ForwardSpotPass(stage, shadersManager->GetProgramFromName("forward-spot"), model);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void AnimaRenderingManager::ForwardDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	ForwardDrawModel(stage, model, program, identityMatrix);
}

void AnimaRenderingManager::ForwardDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (!model->DrawingEnabled())
		return;

	if (stage == nullptr || model == nullptr || program == nullptr)
		return;

	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		ForwardDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		ForwardDrawModel(stage, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::ForwardDrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
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

void AnimaRenderingManager::ForwardAmbientPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
{
	if (model != nullptr && !model->DrawingEnabled())
		return;

	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nAmbLights = lightsManager->GetAmbientLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nAmbLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = stage->GetCamerasManager()->GetActiveCamera();
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

		program->UpdateLightProperies(light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);

				if (!innerModel->DrawingEnabled())
					continue;

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(stage, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(stage, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(stage, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::ForwardDirectionalPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
{
	if (model != nullptr && !model->DrawingEnabled())
		return;

	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = stage->GetCamerasManager()->GetActiveCamera();
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

		program->UpdateLightProperies(light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);

				if (!innerModel->DrawingEnabled())
					continue;

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(stage, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(stage, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(stage, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::ForwardPointPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
{
	if (model != nullptr && !model->DrawingEnabled())
		return;

	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = stage->GetCamerasManager()->GetActiveCamera();
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

		program->UpdateLightProperies(light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);

				if (!innerModel->DrawingEnabled())
					continue;

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(stage, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(stage, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(stage, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::ForwardSpotPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
{
	if (model != nullptr && !model->DrawingEnabled())
		return;

	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = stage->GetCamerasManager()->GetActiveCamera();
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

		program->UpdateLightProperies(light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);

				if (!innerModel->DrawingEnabled())
					continue;

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(stage, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(stage, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(stage, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::DeferredDrawAllModels(AnimaStage* stage)
{
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	GetTexture("DeferredMap")->BindAsRenderTarget();

	Start(stage);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	DeferredPreparePass(stage, shadersManager->GetProgramFromName("deferred-prepare"));

	Finish(stage);

	GetTexture("DiffuseMap")->BindAsRenderTarget();
	Start(stage);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	glDisable(GL_BLEND);

	DeferredAmbientPass(stage, shadersManager->GetProgramFromName("deferred-ambient"));
	//DeferredDirectionalPass(stage, shadersManager->GetProgramFromName("deferred-directional"));
	//DeferredPointPass(stage, shadersManager->GetProgramFromName("deferred-point"));
	//DeferredSpotPass(stage, shadersManager->GetProgramFromName("deferred-spot"));

	Finish(stage);

	ApplyFilter(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::DeferredDrawSingleModel(AnimaStage* stage, AnimaModel* model)
{
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	GetTexture("DeferredMap")->BindAsRenderTarget();
	Start(stage);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	DeferredPreparePass(stage, shadersManager->GetProgramFromName("deferred-prepare"), model);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	Finish(stage);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GetTexture("DiffuseMap")->BindAsRenderTarget();
	Start(stage);

	//glEnable(GL_BLEND);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ONE);
	//glDepthMask(GL_FALSE);
	//glDepthFunc(GL_LEQUAL);

	DeferredAmbientPass(stage, shadersManager->GetProgramFromName("deferred-ambient"));
	//DeferredDirectionalPass(stage, shadersManager->GetProgramFromName("deferred-directional"));
	//DeferredPointPass(stage, shadersManager->GetProgramFromName("deferred-point"));
	//DeferredSpotPass(stage, shadersManager->GetProgramFromName("deferred-spot"));

	//glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);

	Finish(stage);

	ApplyFilter(shadersManager->GetProgramFromName("fxaaFilter"), GetTexture("DiffuseMap"), nullptr);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	DeferredDrawModel(stage, model, program, identityMatrix);
}

void AnimaRenderingManager::DeferredDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (!model->DrawingEnabled())
		return;

	if (stage == nullptr || model == nullptr || program == nullptr)
		return;

	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DeferredDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DeferredDrawModel(stage, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DeferredDrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
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

void AnimaRenderingManager::DeferredPreparePass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
{
	if (model != nullptr && !model->DrawingEnabled())
		return;

	AnimaModelsManager* modelsManager = stage->GetModelsManager();
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nAmbLights = lightsManager->GetAmbientLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nAmbLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = stage->GetCamerasManager()->GetActiveCamera();
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

		program->UpdateLightProperies(light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);

				if (!innerModel->DrawingEnabled())
					continue;

				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					DeferredDrawModelMesh(stage, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					DeferredDrawModel(stage, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				DeferredDrawModelMesh(stage, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				DeferredDrawModel(stage, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::DeferredAmbientPass(AnimaStage* stage, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nAmbLights = lightsManager->GetAmbientLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nAmbLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

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
		if (!light->IsAmbientLight())
			continue;

		program->UpdateLightProperies(light);
		program->UpdateMeshProperies(_filterMesh, _filterMeshTransform.GetTransformationMatrix());
		program->UpdateRenderingManagerProperies(this);

		program->EnableInputs(_filterMesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
	}
}

void AnimaRenderingManager::DeferredDirectionalPass(AnimaStage* stage, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

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
		if (!light->IsDirectionalLight())
			continue;

		program->UpdateLightProperies(light);
		program->UpdateMeshProperies(_filterMesh, _filterMeshTransform.GetTransformationMatrix());
		program->UpdateRenderingManagerProperies(this);

		program->EnableInputs(_filterMesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
	}
}

void AnimaRenderingManager::DeferredPointPass(AnimaStage* stage, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nPtLights = lightsManager->GetPointLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nPtLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

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
		if (!light->IsPointLight())
			continue;

		program->UpdateLightProperies(light);
		program->UpdateMeshProperies(_filterMesh, _filterMeshTransform.GetTransformationMatrix());
		program->UpdateRenderingManagerProperies(this);

		program->EnableInputs(_filterMesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
	}
}

void AnimaRenderingManager::DeferredSpotPass(AnimaStage* stage, AnimaShaderProgram* program)
{
	AnimaLightsManager* lightsManager = stage->GetLightsManager();

	ASizeT nSpLights = lightsManager->GetSpotLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nSpLights == 0)
		return;

	AnimaShaderProgram* activeProgram = stage->GetShadersManager()->GetActiveProgram();

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

		program->UpdateLightProperies(light);
		program->UpdateMeshProperies(_filterMesh, _filterMeshTransform.GetTransformationMatrix());
		program->UpdateRenderingManagerProperies(this);

		program->EnableInputs(_filterMesh);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _filterMesh->GetIndexesBufferObject());
		glDrawElements(GL_TRIANGLES, _filterMesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);
		program->DisableInputs();
	}
}

void AnimaRenderingManager::SetTexture(AnimaString propertyName, AnimaTexture* value, bool deleteExistent)
{
	auto& pair = _texturesMap.find(propertyName);
	
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

void AnimaRenderingManager::SetDeferredData(AnimaString propertyName, AnimaRenderingManager::DeferredData* value)
{
	_deferredDataMap[propertyName] = value;
}

void AnimaRenderingManager::SetDeferredData(const char* propertyName, AnimaRenderingManager::DeferredData* value)
{
	AnimaString str(propertyName, _allocator);
	SetDeferredData(str, value);
}

void AnimaRenderingManager::SetColor(AnimaString propertyName, AnimaColor3f value)
{
	_colors3fMap[propertyName] = value;
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
	_colors3fMap[propertyName] = value;
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

AUint AnimaRenderingManager::GetTextureIndex(const AnimaString& textureName)
{
	if (_deferredDataMap.find(textureName) != _deferredDataMap.end())
		return _deferredDataMap[textureName]->_index;
	return 0;
}

AUint AnimaRenderingManager::GetTextureIndex(const char* textureName)
{
	AnimaString str(textureName, _allocator);
	return GetTextureIndex(str);
}

AnimaRenderingManager::DeferredData* AnimaRenderingManager::GetDeferredData(const AnimaString& slotName)
{
	if (_deferredDataMap.find(slotName) != _deferredDataMap.end())
		return _deferredDataMap[slotName];
	return nullptr;
}

AnimaRenderingManager::DeferredData* AnimaRenderingManager::GetDeferredData(const char* slotName)
{
	AnimaString str(slotName, _allocator);
	return GetDeferredData(str);
}

AnimaColor3f AnimaRenderingManager::GetColor3f(AnimaString propertyName)
{
	if (_colors3fMap.find(propertyName) != _colors3fMap.end())
		return _colors3fMap[propertyName];

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
	if (_colors4fMap.find(propertyName) != _colors4fMap.end())
		return _colors4fMap[propertyName];

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

	for (auto& pair : _texturesMap)
	{
		if (pair.second != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
			pair.second = nullptr;
		}
	}

	for (auto& pair : _deferredDataMap)
	{
		if (pair.second != nullptr)
		{
			AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
			pair.second = nullptr;
		}
	}

	_texturesMap.clear();
	_textureSlotsMap.clear();
	_deferredDataMap.clear();

	_colors3fMap.clear();
	_colors4fMap.clear();

	_vectors2fMap.clear();
	_vectors3fMap.clear();
	_vectors4fMap.clear();

	_floatsMap.clear();
	_integersMap.clear();
	_booleansMap.clear();
}

END_ANIMA_ENGINE_NAMESPACE