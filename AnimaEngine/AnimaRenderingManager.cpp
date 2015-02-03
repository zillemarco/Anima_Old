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

		AnimaMatrix viewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		program->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
		program->SetUniform("eyePosition", camera->GetPosition());
		
		AnimaLight* ambL = engine->GetLightsManager()->GetLightFromName("ambient");
		if (ambL != nullptr)
		{
			program->SetUniform("ambientLight", ambL->GetColor());
		}

		AnimaLight* dirL = engine->GetLightsManager()->GetLightFromName("directional");
		if (dirL != nullptr)
		{
			program->SetUniform("directionalLight.base.color", dirL->GetColor());
			program->SetUniformf("directionalLight.base.intensity", dirL->GetIntensity());
			program->SetUniform("directionalLight.direction", dirL->GetDirection().Normalized());
		}

		AnimaLight* pointL = engine->GetLightsManager()->GetLightFromName("pointLight0");
		if (pointL != nullptr)
		{
			program->SetUniform("pointLights[0].base.color", pointL->GetColor());
			program->SetUniformf("pointLights[0].base.intensity", pointL->GetIntensity());
			program->SetUniformf("pointLights[0].attenuation.constant", pointL->GetConstantAttenuation());
			program->SetUniformf("pointLights[0].attenuation.linear", pointL->GetLinearAttenuation());
			program->SetUniformf("pointLights[0].attenuation.exponent", pointL->GetExponentAttenuation());
			program->SetUniform("pointLights[0].position", pointL->GetPosition());
			program->SetUniformf("pointLights[0].range", pointL->GetRange());
		}

		pointL = engine->GetLightsManager()->GetLightFromName("pointLight1");
		if (pointL != nullptr)
		{
			program->SetUniform("pointLights[1].base.color", pointL->GetColor());
			program->SetUniformf("pointLights[1].base.intensity", pointL->GetIntensity());
			program->SetUniformf("pointLights[1].attenuation.constant", pointL->GetConstantAttenuation());
			program->SetUniformf("pointLights[1].attenuation.linear", pointL->GetLinearAttenuation());
			program->SetUniformf("pointLights[1].attenuation.exponent", pointL->GetExponentAttenuation());
			program->SetUniform("pointLights[1].position", pointL->GetPosition());
			program->SetUniformf("pointLights[1].range", pointL->GetRange());
		}

		AnimaLight* spotL = engine->GetLightsManager()->GetLightFromName("spotLight0");
		if (spotL != nullptr)
		{
			program->SetUniform("spotLights[0].pointLight.base.color", spotL->GetColor());
			program->SetUniformf("spotLights[0].pointLight.base.intensity", spotL->GetIntensity());
			program->SetUniformf("spotLights[0].pointLight.attenuation.constant", spotL->GetConstantAttenuation());
			program->SetUniformf("spotLights[0].pointLight.attenuation.linear", spotL->GetLinearAttenuation());
			program->SetUniformf("spotLights[0].pointLight.attenuation.exponent", spotL->GetExponentAttenuation());
			program->SetUniform("spotLights[0].pointLight.position", spotL->GetPosition());
			program->SetUniformf("spotLights[0].pointLight.range", spotL->GetRange());
			program->SetUniform("spotLights[0].direction", spotL->GetDirection());
			program->SetUniformf("spotLights[0].cutoff", spotL->GetCutoff());
		}

		spotL = engine->GetLightsManager()->GetLightFromName("spotLight1");
		if (spotL != nullptr)
		{
			program->SetUniform("spotLights[1].pointLight.base.color", spotL->GetColor());
			program->SetUniformf("spotLights[1].pointLight.base.intensity", spotL->GetIntensity());
			program->SetUniformf("spotLights[1].pointLight.attenuation.constant", spotL->GetConstantAttenuation());
			program->SetUniformf("spotLights[1].pointLight.attenuation.linear", spotL->GetLinearAttenuation());
			program->SetUniformf("spotLights[1].pointLight.attenuation.exponent", spotL->GetExponentAttenuation());
			program->SetUniform("spotLights[1].pointLight.position", spotL->GetPosition());
			program->SetUniformf("spotLights[1].pointLight.range", spotL->GetRange());
			program->SetUniform("spotLights[1].direction", spotL->GetDirection());
			program->SetUniformf("spotLights[1].cutoff", spotL->GetCutoff());
		}
	}

	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;
	
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	if (material->GetBoolean("twoSided"))
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);

	program->SetUniform("modelMatrix", parentTransformation);

	AnimaTexture* texture = material->GetTexture("diffuse");
	if (texture != nullptr)
	{
		program->SetUniformi("materialDiffuseTexture", 0);
		texture->Bind(0);
	}

	program->SetUniform("materialColor", material->GetColor4f("diffuseColor"));

	program->SetUniformf("specularIntensity", 2.0f);
	program->SetUniformf("specularPower", 32.0f);

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();
	
	bool bEnabled_0 = true;
	bool bEnabled_1 = false;
	bool bEnabled_2 = false;
	bool bEnabled_3 = false;

	glBindVertexArray(mesh->GetVertexArrayObject());

	glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVerticesBufferObject());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (mesh->GetFloatVerticesNormalCount() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetNormalsBufferObject());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		bEnabled_1 = true;
	}

	if (mesh->GetFloatVerticesTextureCoordCount() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetTextureCoordsBufferObject());
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		bEnabled_2 = true;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());

	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	if (bEnabled_3)
		glDisableVertexAttribArray(3);

	if (bEnabled_2)
		glDisableVertexAttribArray(2);

	if (bEnabled_1)
		glDisableVertexAttribArray(1);

	if (bEnabled_0)
		glDisableVertexAttribArray(0);
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

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	if (material->GetBoolean("twoSided"))
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);

	program->SetUniform("modelMatrix", parentTransformation);

	AnimaTexture* texture = material->GetTexture("diffuse");
	if (texture != nullptr)
	{
		program->SetUniformi("materialDiffuseTexture", 0);
		texture->Bind(0);
	}

	program->SetUniform("materialColor", material->GetColor4f("diffuseColor"));

	program->SetUniformf("specularIntensity", 2.0f);
	program->SetUniformf("specularPower", 32.0f);

	if (mesh->NeedsBuffersUpdate())
		mesh->UpdateBuffers();

	bool bEnabled_0 = true;
	bool bEnabled_1 = false;
	bool bEnabled_2 = false;
	bool bEnabled_3 = false;

	glBindVertexArray(mesh->GetVertexArrayObject());

	glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVerticesBufferObject());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (mesh->GetFloatVerticesNormalCount() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetNormalsBufferObject());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		bEnabled_1 = true;
	}

	if (mesh->GetFloatVerticesTextureCoordCount() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->GetTextureCoordsBufferObject());
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

		bEnabled_2 = true;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexesBufferObject());

	glDrawElements(GL_TRIANGLES, mesh->GetFacesIndicesCount(), GL_UNSIGNED_INT, 0);

	if (bEnabled_3)
		glDisableVertexAttribArray(3);

	if (bEnabled_2)
		glDisableVertexAttribArray(2);

	if (bEnabled_1)
		glDisableVertexAttribArray(1);

	if (bEnabled_0)
		glDisableVertexAttribArray(0);
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

		AnimaMatrix viewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		program->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsAmbientLight())
			continue;

		program->SetUniform("ambientLight", light->GetColor());

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

		AnimaMatrix viewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		program->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
		program->SetUniform("eyePosition", camera->GetPosition());
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsDirectionalLight())
			continue;

		program->SetUniform("directionalLight.base.color", light->GetColor());
		program->SetUniformf("directionalLight.base.intensity", light->GetIntensity());
		program->SetUniform("directionalLight.direction", light->GetDirection().Normalized());

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

		AnimaMatrix viewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		program->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
		program->SetUniform("eyePosition", camera->GetPosition());
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsPointLight())
			continue;

		program->SetUniform("pointLight.base.color", light->GetColor());
		program->SetUniformf("pointLight.base.intensity", light->GetIntensity());
		program->SetUniformf("pointLight.attenuation.constant", light->GetConstantAttenuation());
		program->SetUniformf("pointLight.attenuation.linear", light->GetLinearAttenuation());
		program->SetUniformf("pointLight.attenuation.exponent", light->GetExponentAttenuation());
		program->SetUniform("pointLight.position", light->GetPosition());
		program->SetUniformf("pointLight.range", light->GetRange());

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

		AnimaMatrix viewProjectionMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();

		program->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
		program->SetUniform("eyePosition", camera->GetPosition());
	}

	for (ASizeT i = 0; i < nLights; i++)
	{
		AnimaLight* light = lightsManager->GetLight((AUint)i);
		if (!light->IsSpotLight())
			continue;

		program->SetUniform("spotLight.pointLight.base.color", light->GetColor());
		program->SetUniformf("spotLight.pointLight.base.intensity", light->GetIntensity());
		program->SetUniformf("spotLight.pointLight.attenuation.constant", light->GetConstantAttenuation());
		program->SetUniformf("spotLight.pointLight.attenuation.linear", light->GetLinearAttenuation());
		program->SetUniformf("spotLight.pointLight.attenuation.exponent", light->GetExponentAttenuation());
		program->SetUniform("spotLight.pointLight.position", light->GetPosition());
		program->SetUniformf("spotLight.pointLight.range", light->GetRange());
		program->SetUniform("spotLight.direction", light->GetDirection());
		program->SetUniformf("spotLight.cutoff", light->GetCutoff());

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