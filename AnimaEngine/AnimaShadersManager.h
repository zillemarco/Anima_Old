#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaModel.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"
#include "AnimaStage.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaShadersManager
{
	friend AnimaShaderProgram;

public:
	AnimaShadersManager(AnimaStage* stage);
	~AnimaShadersManager();

	AnimaShader* LoadShader(const AnimaString& name, const AnimaString& text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(const AnimaString& name, const AChar* text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(const char* name, const AnimaString& text, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShader(const char* name, const AChar* text, AnimaShader::AnimaShaderType type);
	
	AnimaShader* LoadShaderFromFile(const AnimaString& name, const AnimaString& filePath, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const AnimaString& name, const AChar* filePath, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const char* name, const AnimaString& filePath, AnimaShader::AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const char* name, const AChar* filePath, AnimaShader::AnimaShaderType type);
	
	AnimaShader* LoadShader(const AnimaString& name, AnimaShaderProgram::AnimaShaderInfo info);
	AnimaShader* LoadShader(const char* name, AnimaShaderProgram::AnimaShaderInfo info);
	
	AnimaShader* CreateShader(const AnimaString& name);
	AnimaShader* CreateShader(const char* name);

	AnimaShaderProgram* CreateProgram(const AnimaString& name);
	AnimaShaderProgram* CreateProgram(const char* name);

	AnimaShaderProgram* GetProgram(ASizeT index);
	AnimaShaderProgram* GetProgramFromName(const AnimaString& name);
	AnimaShaderProgram* GetProgramFromName(const char* name);

	void SetActiveProgram(AnimaShaderProgram* program);
	void SetActiveProgramFromName(const AnimaString& name);
	void SetActiveProgramFromName(const char* name);

	AnimaShaderProgram* GetActiveProgram();

private:
	void ClearShaders(bool bDeleteObjects = true, bool bResetNumber = true);
	void ClearPrograms(bool bDeleteObjects = true, bool bResetNumber = true);

	void NotifyProgramActivation(AnimaShaderProgram* program);
	void NotifyProgramDeactivation(AnimaShaderProgram* program);

private:
	AnimaStage* _stage;

	AnimaShader**	_shaders;
	ASizeT			_shadersNumber;

	AnimaShaderProgram* _activeProgram;

	AnimaShaderProgram**	_programs;
	ASizeT					_programsNumber;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _shadersMap;
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _programsMap;
#pragma warning (default: 4251)
};

END_ANIMA_ENGINE_NAMESPACE