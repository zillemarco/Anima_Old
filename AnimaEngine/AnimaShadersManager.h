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
	AnimaShader* LoadShader(const AnimaString& name, AnimaShaderProgram::AnimaShaderInfo info);
	AnimaShader* LoadShaderFromFile(const AnimaString& name, const AnimaString& filePath, AnimaShaderType type);
	
	bool LoadShadersParts(const AnimaString& partsPath);
	AnimaShader* LoadShaderFromPartFile(const AnimaString& partFilePath);
	
	AnimaShader* CreateShader(const AnimaString& name);

	AnimaShader* GetShader(AInt index);
	AnimaShader* GetShaderFromName(const AnimaString& name);

	AnimaShaderProgram* CreateProgram(const AnimaString& name);
	AnimaShaderProgram* CreateProgram(AnimaMesh* mesh, const AnimaMaterial* material);
	AnimaShaderProgram* CreateProgram(AnimaMeshInstance* meshInstance, const AnimaMaterial* material);

	AnimaShaderProgram* GetProgram(AInt index);
	AnimaShaderProgram* GetProgramFromName(const AnimaString& name);

	void SetActiveProgram(AnimaShaderProgram* program);
	void SetActiveProgramFromName(const AnimaString& name);

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