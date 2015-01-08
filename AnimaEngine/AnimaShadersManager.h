#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaModel.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaShadersManager
{
public:
	AnimaShadersManager(AnimaEngine* engine);
	~AnimaShadersManager();

	AnimaShader* LoadShader(AnimaString text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(const AChar* text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(AnimaShaderProgram::AnimaShaderInfo info);
	bool LoadShaders(const AnimaShaderProgram::AnimaShaderInfo* info, ASizeT count, AnimaShader** output);

	AnimaShader* CreateShader(bool allocate = true);
	AnimaShaderProgram* CreateProgram(bool allocate = true);

private:
	AnimaShader* AddShader(const AnimaShader& shader);
	AnimaShaderProgram* AddProgram(const AnimaShaderProgram& program);

private:
	AnimaEngine* _engine;

	AnimaShader**	_shaders;
	ASizeT			_shadersNumber;

	AnimaShaderProgram**	_programs;
	ASizeT					_programsNumber;
};

END_ANIMA_ENGINE_NAMESPACE