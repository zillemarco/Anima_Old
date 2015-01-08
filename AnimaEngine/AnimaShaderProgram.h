#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaShader.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShadersManager;

class ANIMA_ENGINE_EXPORT AnimaShaderProgram
{
public:
	enum AnimaShaderInfoType {
		SHADER_FILE,
		SHADER_TEXT
	};

	struct AnimaShaderInfo {
		const AChar*					_text;
		AnimaShaderInfoType				_infoType;
		AnimaShader::AnimaShaderType	_shaderType;

		AnimaShaderInfo() {
			_text = "";
			_infoType = SHADER_FILE;
			_shaderType = AnimaShader::VERTEX;
		}
	};

public:
	AnimaShaderProgram(AnimaEngine* engine);
	AnimaShaderProgram(const AnimaShaderProgram& src);
	AnimaShaderProgram(AnimaShaderProgram&& src);
	~AnimaShaderProgram();

	AnimaShaderProgram& operator=(const AnimaShaderProgram& src);
	AnimaShaderProgram& operator=(AnimaShaderProgram&& src);

	inline bool operator==(const AnimaShaderProgram& left);
	inline bool operator!=(const AnimaShaderProgram& left);

public:
	void AddShader(AnimaShader* shader);
	void SetShaders(AnimaShader** shaders, ASizeT count);

	bool CompileShaders();
	void ClearShaders();

	bool Create();
	bool Link();
	bool Use();
	bool Delete();

	bool IsCreated();
	bool IsLinked();

	AInt GetID();

public:
	void SetUniformValue(int uniformIndex, float* value);

private:
	AnimaEngine*	_engine;
	AnimaShader**	_shaders;
	ASizeT			_shadersNumber;

	AInt			_id;
	bool			_linked;
};

END_ANIMA_ENGINE_NAMESPACE