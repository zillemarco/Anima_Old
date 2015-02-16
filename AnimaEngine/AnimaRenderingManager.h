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
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaRenderingManager
{
public:
	AnimaRenderingManager(AnimaStage* stage);
	~AnimaRenderingManager();

	void Start(AnimaStage* stage);
	void Finish(AnimaStage* stage);

	void DrawAllModels(AnimaStage* stage);
	void DrawSingleModel(AnimaStage* stage, AnimaModel* model);

	void ForwardDrawAllModels(AnimaStage* stage);
	void ForwardDrawSingleModel(AnimaStage* stage, AnimaModel* model);

public:
	AUint GetTextureSlot(const AnimaString& slotName);
	AUint GetTextureSlot(AnimaStage* stage, const char* slotName);

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

protected:
#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _textureSlots;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif __Anima__AnimaRenderingManager__