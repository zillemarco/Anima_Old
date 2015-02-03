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

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaEngine;

class ANIMA_ENGINE_EXPORT AnimaRenderingManager
{
public:
	void Start(AnimaEngine* engine);
	void Finish(AnimaEngine* engine);

	void DrawAllModels(AnimaEngine* engine);
	void DrawSingleModel(AnimaEngine* engine, AnimaModel* model);

	void ForwardDrawAllModels(AnimaEngine* engine);
	void ForwardDrawSingleModel(AnimaEngine* engine, AnimaModel* model);

private:
	void DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program);
	void DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void DrawModelMesh(AnimaEngine* engine, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	
	void AmbientPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void DirectionalPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void PointPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model = nullptr);
	void SpotPass(AnimaEngine* engine, AnimaShaderProgram* program, AnimaModel* model = nullptr);

	void ForwardDrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program);
	void ForwardDrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
	void ForwardDrawModelMesh(AnimaEngine* engine, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
};

END_ANIMA_ENGINE_NAMESPACE

#endif __Anima__AnimaRenderingManager__