#pragma once

#ifndef __Anima__AnimaRenderingManager__
#define __Anima__AnimaRenderingManager__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaModel.h"
#include "AnimaMaterial.h"
#include "AnimaLight.h"
#include "AnimaShaderProgram.h"
#include "AnimaStage.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaRenderingManager
{
public:
	void Start(AnimaStage* stage);
	void Finish(AnimaStage* stage);

	void DrawAllModels(AnimaStage* stage);
	void DrawSingleModel(AnimaStage* stage, AnimaModel* model);

	void ForwardDrawAllModels(AnimaStage* stage);
	void ForwardDrawSingleModel(AnimaStage* stage, AnimaModel* model);

private:
	void DrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program);
	void DrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void DrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	
	void AmbientPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void DirectionalPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void PointPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void SpotPass(AnimaStage* stage, AnimaShaderProgram* program, AnimaModel* model = nullptr);

	void ForwardDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program);
	void ForwardDrawModel(AnimaStage* stage, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void ForwardDrawModelMesh(AnimaStage* stage, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
};

END_ANIMA_ENGINE_NAMESPACE

#endif __Anima__AnimaRenderingManager__