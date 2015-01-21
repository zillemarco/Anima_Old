#pragma once

#ifndef __Anima__AnimaRenderingManager__
#define __Anima__AnimaRenderingManager__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaModel.h"
#include "AnimaMaterial.h"
#include "AnimaShaderProgram.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaEngine;

class ANIMA_ENGINE_EXPORT AnimaRenderingManager
{
public:
	static void Start(AnimaEngine* engine);
	static void Finish(AnimaEngine* engine);

	static void DrawAllModels(AnimaEngine* engine);
	static void DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program);
	static void DrawModel(AnimaEngine* engine, AnimaModel* model, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);

private:
	static void DrawModelMesh(AnimaEngine* engine, AnimaMesh* mesh, AnimaShaderProgram* program, const AnimaMatrix& parentTransformation);
};

END_ANIMA_ENGINE_NAMESPACE

#endif __Anima__AnimaRenderingManager__