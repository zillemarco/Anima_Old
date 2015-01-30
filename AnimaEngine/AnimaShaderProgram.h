#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaShader.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"
#include <boost\unordered_map.hpp>

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

	struct AnimaUniformInfo {
		AInt _location;
		AUint _type;
	};

public:
	AnimaShaderProgram(AnimaEngine* engine, AnimaShadersManager* shadersManager);
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
	void ScanVariables();

	void SetUniformi(const AnimaString& uniformName, int value);
	void SetUniformi(const char* uniformName, int value);
	void SetUniformf(const AnimaString& uniformName, AFloat value);
	void SetUniformf(const char* uniformName, AFloat value);
	void SetUniform(const AnimaString& uniformName, const AnimaVertex3f& value);
	void SetUniform(const char* uniformName, const AnimaVertex3f& value);
	void SetUniform(const AnimaString& uniformName, const AnimaColor4f& value);
	void SetUniform(const char* uniformName, const AnimaColor4f& value);
	void SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c);
	void SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c);
	void SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(const AnimaString& uniformName, const AnimaMatrix& value);
	void SetUniform(const char* uniformName, const AnimaMatrix& value);
	
private:
	AnimaEngine*	_engine;
	
	AnimaShader**	_shaders;
	ASizeT			_shadersNumber;

	AnimaShadersManager* _shadersManager;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaUniformInfo, AnimaString::Hasher> _uniforms;
#pragma warning (default: 4251) 

	AInt			_id;
	bool			_linked;
};

END_ANIMA_ENGINE_NAMESPACE