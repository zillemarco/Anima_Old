#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaShader.h"
#include "AnimaShaderProgram.h"
#include "AnimaMappedArray.h"
#include "AnimaArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaMaterial;
class AnimaMesh;
class AnimaMeshInstance;

class ANIMA_ENGINE_EXPORT AnimaShadersManager
{
	friend AnimaShaderProgram;

public:
	AnimaShadersManager(AnimaEngine* engine);
	~AnimaShadersManager();

	AnimaShader* LoadShader(const AnimaString& name, const AnimaString& text, AnimaShaderType type);
	AnimaShader* LoadShader(const AnimaString& name, const AChar* text, AnimaShaderType type);
	AnimaShader* LoadShader(const char* name, const AnimaString& text, AnimaShaderType type);
	AnimaShader* LoadShader(const char* name, const AChar* text, AnimaShaderType type);
	
	AnimaShader* LoadShaderFromFile(const AnimaString& name, const AnimaString& filePath, AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const AnimaString& name, const AChar* filePath, AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const char* name, const AnimaString& filePath, AnimaShaderType type);
	AnimaShader* LoadShaderFromFile(const char* name, const AChar* filePath, AnimaShaderType type);
	
	AnimaShader* LoadShader(const AnimaString& name, AnimaShaderProgram::AnimaShaderInfo info);
	AnimaShader* LoadShader(const char* name, AnimaShaderProgram::AnimaShaderInfo info);

	bool LoadShadersParts(const AnimaString& partsPath);
	bool LoadShadersParts(const char* partsPath);
	AnimaShader* LoadShaderFromPartFile(const AnimaString& partFilePath);
	AnimaShader* LoadShaderFromPartFile(const char* partFilePath);
	
	AnimaShader* CreateShader(const AnimaString& name);
	AnimaShader* CreateShader(const char* name);

	AnimaShader* GetShader(AInt index);
	AnimaShader* GetShaderFromName(const AnimaString& name);
	AnimaShader* GetShaderFromName(const char* name);

	AnimaShaderProgram* CreateProgram(const AnimaString& name);
	AnimaShaderProgram* CreateProgram(const char* name);
	AnimaShaderProgram* CreateProgram(AnimaMesh* mesh, const AnimaMaterial* material);
	AnimaShaderProgram* CreateProgram(AnimaMeshInstance* meshInstance, const AnimaMaterial* material);

	AnimaShaderProgram* GetProgram(AInt index);
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
	AnimaEngine* _engine;	
	AnimaMappedArray<AnimaShader*> _shaders;
	AnimaMappedArray<AnimaShaderProgram*> _programs;

	AnimaShaderProgram* _activeProgram;
};

END_ANIMA_ENGINE_NAMESPACE