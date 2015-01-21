#include "AnimaEngine.h"
#include "AnimaRenderingManager.h"
#include "AnimaCamera.h"
#include "AnimaShaderProgram.h"
#include "AnimaTexture.h"
#include "AnimaShadersManager.h"
#include "AnimaCamerasManager.h"
#include "AnimaTexturesManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

void AnimaRenderingManager::DrawAllModels(AnimaEngine* engine)
{
}

void AnimaRenderingManager::DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (engine == nullptr || model == nullptr || program == nullptr)
		return;

	AnimaShaderProgram* activeProgram = engine->GetShadersManager()->GetActiveProgram();

	if (activeProgram == nullptr || (*activeProgram) != (*program))
	{
		AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();

		if (camera == nullptr)
			return;

		AnimaMatrix viewProjectionMatrix = camera->GetViewMatrix() * camera->GetProjectionMatrix();

		AnimaVertex3f lightDir(engine);
		lightDir[0] = -1.0;
		lightDir[1] = -1.0;
		lightDir[2] = -1.0;

		lightDir.Normalize();

		program->Use();

		program->SetUniform("viewProjectionMatrix", viewProjectionMatrix);
		program->SetUniform("eyePosition", camera->GetPosition());
		program->SetUniform("ambientLight", 0.8, 0.8, 0.8);
		program->SetUniform("directionalLight.base.color", 1.0, 0.0, 0.0);
		program->SetUniformf("directionalLight.base.intensity", 0.8);
		program->SetUniform("directionalLight.direction", lightDir);
	}

	AnimaMatrix modelMatrix = model->GetTransformationMatrix() * parentTransformation;

	ASizeT meshNumber = model->GetMeshesNumber();
	for (ASizeT i = 0; i < meshNumber; i++)
		DrawModelMesh(engine, model->GetPMesh(i), program, modelMatrix);

	ASizeT childrenNumber = model->GetChildrenNumber();
	for (ASizeT i = 0; i < childrenNumber; i++)
		DrawModel(engine, model->GetPChild(i), program, modelMatrix);

	//AnimaCamera* camera = engine->GetCamerasManager()->GetActiveCamera();
	//AnimaShaderProgram* program = engine->GetShadersManager()->GetProgram(0);
	//
	//AnimaMatrix modelMatrix = AnimaMatrix(engine);//object->GetTransformationMatrix();
	//AnimaMatrix viewMatrix = camera->GetViewMatrix();
	//AnimaMatrix projectionMatrix = camera->GetProjectionMatrix();

	//AnimaMatrix mvpMatrix = modelMatrix * viewMatrix * projectionMatrix;
	//AnimaMatrix normalMatrix = modelMatrix;

	//AnimaVertex3f lightDir(engine);
	//lightDir[0] = -1.0;
	//lightDir[1] = -1.0;
	//lightDir[2] = -1.0;

	//lightDir.Normalize();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glGetError();
	//glClearColor(0.5, 0.5, 0.5, 1.0);

	//glFrontFace(GL_CCW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);

	//program->Use();
	//
	//program->SetUniformi("materialDiffuseTexture", 0);
	//
	//material->GetTexture("diffuse")->Bind(0);

	//AnimaColor3f colorMat = material->GetColor("materialColor");
	//AnimaColor3f colorLight = material->GetColor("lightColor");
	//AnimaColor3f colorAmb = material->GetColor("ambientColor");

	//program->SetUniform("materialColor", colorMat);
	//program->SetUniformf("specularIntensity", material->GetFloat("specInt"));
	//program->SetUniformf("specularPower", material->GetFloat("specPow"));

	//object->Draw();
}

void AnimaRenderingManager::DrawModelMesh(AnimaEngine* engine, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation)
{
	if (engine == nullptr || mesh == nullptr || program == nullptr)
		return;

	AnimaMaterial* material = mesh->GetMaterial();
	if (material == nullptr)
		return;
	
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	program->SetUniform("modelMatrix", parentTransformation);

	AnimaTexture* texture = material->GetTexture("diffuse");
	if (texture != nullptr)
	{
		program->SetUniformi("materialDiffuseTexture", 0);
		texture->Bind(0);
	}

	program->SetUniform("materialColor", material->GetColor("diffuseColor"));

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

	//if (mesh->GetFloatVerticesColorCount() > 0)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, mesh->GetColorsBufferObject);
	//	glEnableVertexAttribArray(3);
	//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//	bEnabled_3 = true;
	//}

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

void AnimaRenderingManager::DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program)
{
	AnimaMatrix identityMatrix(engine);
	DrawModel(engine, model, program, identityMatrix);
}

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

END_ANIMA_ENGINE_NAMESPACE