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
	friend AnimaShaderProgram;

public:
	AnimaShadersManager(AnimaEngine* engine);
	~AnimaShadersManager();

	AnimaShader* LoadShader(AnimaString text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(const AChar* text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(AnimaString filePath, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const AChar* filePath, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(AnimaShaderProgram::AnimaShaderInfo info);
	bool LoadShaders(const AnimaShaderProgram::AnimaShaderInfo* info, ASizeT count, AnimaShader** output);

	AnimaShader* CreateShader();
	AnimaShaderProgram* CreateProgram();

	AnimaShaderProgram* GetProgram(ASizeT index);

	void SetActiveProgram(AnimaShaderProgram* program);
	AnimaShaderProgram* GetActiveProgram();

private:
	void ClearShaders(bool bDeleteObjects = true, bool bResetNumber = true);
	void ClearPrograms(bool bDeleteObjects = true, bool bResetNumber = true);

	void NotifyProgramActivation(AnimaShaderProgram* program);
	void NotifyProgramDeactivation(AnimaShaderProgram* program);

private:
	AnimaEngine* _engine;

	AnimaShader**	_shaders;
	ASizeT			_shadersNumber;

	AnimaShaderProgram* _activeProgram;

	AnimaShaderProgram**	_programs;
	ASizeT					_programsNumber;
};

END_ANIMA_ENGINE_NAMESPACE