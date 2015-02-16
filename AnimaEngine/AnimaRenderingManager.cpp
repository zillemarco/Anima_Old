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

AnimaRenderingManager::AnimaRenderingManager(AnimaStage* stage)
{
	// TODO
	//AnimaString str("diffuse", stage->GetStringAllocator());
	//_textureSlots[str] = 0;
}

AnimaRenderingManager::~AnimaRenderingManager()
{
	_textureSlots.clear();
}

void AnimaRenderingManager::Start(AnimaStage* stage)
{
	stage->GetShadersManager()->SetActiveProgram(nullptr);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGetError();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
}

void AnimaRenderingManager::Finish(AnimaStage* stage)
{
	stage->GetShadersManager()->SetActiveProgram(nullptr);
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

		program->UpdateCameraProperies(stage, camera);
		program->UpdateLightsProperies(stage);
	}

	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;

	program->UpdateMeshProperies(stage, mesh, parentTransformation);
	program->UpdateMaterialProperies(stage, this, material);
	
	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	program->EnableInputs(stage, mesh);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	program->DisableInputs(stage);
}

void AnimaRenderingManager::ForwardDrawAllModels(AnimaStage* stage)
{
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	AmbientPass(stage, shadersManager->GetProgramFromName("forward-ambient"));

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	DirectionalPass(stage, shadersManager->GetProgramFromName("forward-directional"));
	PointPass(stage, shadersManager->GetProgramFromName("forward-point"));
	SpotPass(stage, shadersManager->GetProgramFromName("forward-spot"));

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void AnimaRenderingManager::ForwardDrawSingleModel(AnimaStage* stage, AnimaModel* model)
{
	Anima::AnimaShadersManager* shadersManager = stage->GetShadersManager();

	AmbientPass(stage, shadersManager->GetProgramFromName("forward-ambient"), model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	DirectionalPass(stage, shadersManager->GetProgramFromName("forward-directional"), model);
	PointPass(stage, shadersManager->GetProgramFromName("forward-point"), model);
	SpotPass(stage, shadersManager->GetProgramFromName("forward-spot"), model);

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

	program->UpdateMeshProperies(stage, mesh, parentTransformation);
	program->UpdateMaterialProperies(stage, this, material);
		
	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	program->EnableInputs(stage, mesh);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	program->DisableInputs(stage);
}

void AnimaRenderingManager::AmbientPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
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

		program->UpdateCameraProperies(stage, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsAmbientLight())
			continue;

		program->UpdateLightProperies(stage, light);

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

void AnimaRenderingManager::DirectionalPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
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

		program->UpdateCameraProperies(stage, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsDirectionalLight())
			continue;

		program->UpdateLightProperies(stage, light);

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

void AnimaRenderingManager::PointPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
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

		program->UpdateCameraProperies(stage, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsPointLight())
			continue;

		program->UpdateLightProperies(stage, light);

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

void AnimaRenderingManager::SpotPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model)
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

		program->UpdateCameraProperies(stage, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsSpotLight())
			continue;

		program->UpdateLightProperies(stage, light);

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

AUint AnimaRenderingManager::GetTextureSlot(const AnimaString& slotName)
{
	if (_textureSlots.find(slotName) != _textureSlots.end())
		return _textureSlots[slotName];
	return 0;
}

AUint AnimaRenderingManager::GetTextureSlot(AnimaStage* stage, const char* slotName)
{
	AnimaString str(slotName, stage->GetStringAllocator());
	return GetTextureSlot(str);
}

END_ANIMA_ENGINE_NAMESPACE