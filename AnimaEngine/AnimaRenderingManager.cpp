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

void AnimaRenderingManager::Start(AnimaEngine* engine)
{
	engine->GetShadersManager()->SetActiveProgram(nullptr);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glGetError();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glEnable(GL_DEPTH_TEST);
}

void AnimaRenderingManager::Finish(AnimaEngine* engine)
{
	engine->GetShadersManager()->SetActiveProgram(nullptr);
}

void AnimaRenderingManager::DrawAllModels(AnimaEngine* engine)
{
	AnimaModelsManager* modelsManager = engine->GetModelsManager();
	AnimaLightsManager* lightsManager = engine->GetLightsManager();
	Anima::AnimaShadersManager* shadersManager = engine->GetShadersManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* program = shadersManager->GetProgramFromName("phong");

	for (ASizeT j = 0; j < nModels; j++)
	{
		AnimaModel* model = modelsManager->GetPModel(j);
		AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

		ASizeT meshNumber = model->GetMeshesNumber();
		for (ASizeT i = 0; i < meshNumber; i++)
			DrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

		ASizeT childrenNumber = model->GetChildrenNumber();
		for (ASizeT i = 0; i < childrenNumber; i++)
			DrawModel(engine, model->GetPChild(i), program, modelMatrix);
	}
}

void AnimaRenderingManager::DrawSingleModel(AnimaEngine* engine, AnimaModel* model)
{
	AnimaModelsManager* modelsManager = engine->GetModelsManager();
	AnimaLightsManager* lightsManager = engine->GetLightsManager();
	Anima::AnimaShadersManager* shadersManager = engine->GetShadersManager();

	ASizeT nModels = modelsManager->GetModelsNumber();

	if (nModels == 0)
		return;

	AnimaShaderProgram* program = shadersManager->GetProgramFromName("phong");

	AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(engine, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	DrawModel(engine, model, program, identityMatrix);
}

void AnimaRenderingManager::DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (engine == nullptr || model == nullptr || program == nullptr)
		return;
	
	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(engine, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::DrawModelMesh(AnimaEngine* engine, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (engine == nullptr || mesh == nullptr || program == nullptr)
		return;
	
	AnimaShaderProgram* activeProgram = engine->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(engine, camera);
		program->UpdateLightsProperies(engine);
	}

	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;

	program->UpdateMeshProperies(engine, mesh, parentTransformation);
	program->UpdateMaterialProperies(engine, material);

	//AnimaTexture* texture = material->GetTexture("diffuse");
	//if (texture != nullptr)
	//{
	//	program->SetUniformi("materialDiffuseTexture", 0);
	//	texture->Bind(0);
	//}

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	program->EnableInputs(engine, mesh);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glDrawElements(GL_PATCHES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	program->DisableInputs(engine);
}

void AnimaRenderingManager::ForwardDrawAllModels(AnimaEngine* engine)
{
	Anima::AnimaShadersManager* shadersManager = engine->GetShadersManager();

	AmbientPass(engine, shadersManager->GetProgramFromName("forward-ambient"));

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	DirectionalPass(engine, shadersManager->GetProgramFromName("forward-directional"));
	PointPass(engine, shadersManager->GetProgramFromName("forward-point"));
	SpotPass(engine, shadersManager->GetProgramFromName("forward-spot"));

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void AnimaRenderingManager::ForwardDrawSingleModel(AnimaEngine* engine, AnimaModel* model)
{
	Anima::AnimaShadersManager* shadersManager = engine->GetShadersManager();

	AmbientPass(engine, shadersManager->GetProgramFromName("forward-ambient"), model);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	DirectionalPass(engine, shadersManager->GetProgramFromName("forward-directional"), model);
	PointPass(engine, shadersManager->GetProgramFromName("forward-point"), model);
	SpotPass(engine, shadersManager->GetProgramFromName("forward-spot"), model);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void AnimaRenderingManager::ForwardDrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix;
	ForwardDrawModel(engine, model, program, identityMatrix);
}

void AnimaRenderingManager::ForwardDrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (engine == nullptr || model == nullptr || program == nullptr)
		return;

	AnimaMatrix modelMatrix = parentTransformation * model->GetTransformation()->GetTransformationMatrix();

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		ForwardDrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		ForwardDrawModel(engine, model->GetPChild(i), program, modelMatrix);
}

void AnimaRenderingManager::ForwardDrawModelMesh(AnimaEngine* engine, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;

	program->UpdateMeshProperies(engine, mesh, parentTransformation);
	program->UpdateMaterialProperies(engine, material);

	//AnimaTexture* texture = material->GetTexture("diffuse");
	//if (texture != nullptr)
	//{
	//	program->SetUniformi("materialDiffuseTexture", 0);
	//	texture->Bind(0);
	//}
	
	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	program->EnableInputs(engine, mesh);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());
	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	program->DisableInputs(engine);
}

void AnimaRenderingManager::AmbientPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model)
{
	AnimaModelsManager* modelsManager = engine->GetModelsManager();
	AnimaLightsManager* lightsManager = engine->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nAmbLights = lightsManager->GetAmbientLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nAmbLights == 0)
		return;

	AnimaShaderProgram* activeProgram = engine->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(engine, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsAmbientLight())
			continue;

		program->UpdateLightProperies(engine, light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);
				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(engine, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(engine, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(engine, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::DirectionalPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model)
{
	AnimaModelsManager* modelsManager = engine->GetModelsManager();
	AnimaLightsManager* lightsManager = engine->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = engine->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(engine, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsDirectionalLight())
			continue;

		program->UpdateLightProperies(engine, light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);
				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(engine, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(engine, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(engine, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::PointPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model)
{
	AnimaModelsManager* modelsManager = engine->GetModelsManager();
	AnimaLightsManager* lightsManager = engine->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = engine->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(engine, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsPointLight())
			continue;

		program->UpdateLightProperies(engine, light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);
				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(engine, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(engine, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(engine, model->GetPChild(i), program, modelMatrix);
		}
	}
}

void AnimaRenderingManager::SpotPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model)
{
	AnimaModelsManager* modelsManager = engine->GetModelsManager();
	AnimaLightsManager* lightsManager = engine->GetLightsManager();

	ASizeT nModels = modelsManager->GetModelsNumber();
	ASizeT nDirLights = lightsManager->GetDirectionalLightsCount();
	ASizeT nLights = lightsManager->GetTotalLightsCount();

	if (nModels == 0 || nDirLights == 0)
		return;

	AnimaShaderProgram* activeProgram = engine->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();
		if (camera == nullptr)
			return;

		program->Use();

		program->UpdateCameraProperies(engine, camera);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsSpotLight())
			continue;

		program->UpdateLightProperies(engine, light);

		if (model == nullptr)
		{
			for (ASizeT j = 0; j < nModels; j++)
			{
				AnimaModel* innerModel = modelsManager->GetPModel(j);
				AnimaMatrix modelMatrix = innerModel->GetTransformation()->GetTransformationMatrix();

				ASizeT meshNumber = innerModel->GetMeshesNumber();
				for (ASizeT i = 0; i < meshNumber; i++)
					ForwardDrawModelMesh(engine, innerModel->GetPMesh(i), program, modelMatrix);

				ASizeT childrenNumber = innerModel->GetChildrenNumber();
				for (ASizeT i = 0; i < childrenNumber; i++)
					ForwardDrawModel(engine, innerModel->GetPChild(i), program, modelMatrix);
			}
		}
		else
		{
			AnimaMatrix modelMatrix = model->GetTransformation()->GetTransformationMatrix();

			ASizeT meshNumber = model->GetMeshesNumber();
			for (ASizeT i = 0; i < meshNumber; i++)
				ForwardDrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

			ASizeT childrenNumber = model->GetChildrenNumber();
			for (ASizeT i = 0; i < childrenNumber; i++)
				ForwardDrawModel(engine, model->GetPChild(i), program, modelMatrix);
		}
	}
}

END_ANIMA_ENGINE_NAMESPACE