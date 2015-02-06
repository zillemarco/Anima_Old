#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaShader.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"
#include "AnimaMesh.h"
#include "AnimaMaterial.h"
#include "AnimaLight.h"
#include "AnimaCamera.h"
#include <boost/unordered_map.hpp>

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
		AUint _arraySize;
		AnimaString _name;
	};

	struct AnimaInputInfo {
		AInt _location;
		AUint _type;
		AnimaString _name;
	};

	struct AnimaOutputInfo {
		AInt _location;
		AUint _type;
		AnimaString _name;
	};

public:
	AnimaShaderProgram(AnimaEngine* engine, AnimaShadersManager* shadersManager);
	AnimaShaderProgram(const AnimaShaderProgram& src);
	AnimaShaderProgram(AnimaShaderProgram&& src);
	~AnimaShaderProgram();

	AnimaShaderProgram& operator=(const AnimaShaderProgram& src);
	AnimaShaderProgram& operator=(AnimaShaderProgram&& src);

	bool operator==(const AnimaShaderProgram& left);
	bool operator!=(const AnimaShaderProgram& left);

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

	void EnableInputs(AnimaEngine* engine, AnimaMesh* mesh);
	void DisableInputs(AnimaEngine* engine);

	void UpdateMeshProperies(AnimaEngine* engine, AnimaMesh* mesh, const AnimaMatrix& transformation);
	void UpdateCameraProperies(AnimaEngine* engine, AnimaCamera* camera);
	void UpdateMaterialProperies(AnimaEngine* engine, AnimaMaterial* material);
	void UpdateLightProperies(AnimaEngine* engine, AnimaLight* light);
	void UpdateLightsProperies(AnimaEngine* engine);

	void SetUniformi(const AnimaString& uniformName, int value);
	void SetUniformi(const char* uniformName, int value);
	void SetUniformf(const AnimaString& uniformName, AFloat value);
	void SetUniformf(const char* uniformName, AFloat value);
	void SetUniform(const AnimaString& uniformName, const AnimaVertex2f& value);
	void SetUniform(const char* uniformName, const AnimaVertex2f& value);
	void SetUniform(const AnimaString& uniformName, const AnimaVertex3f& value);
	void SetUniform(const char* uniformName, const AnimaVertex3f& value);
	void SetUniform(const AnimaString& uniformName, const AnimaColor4f& value);
	void SetUniform(const char* uniformName, const AnimaColor4f& value);
	void SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c);
	void SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c);
	void SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(const char* uniformName, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(const AnimaString& uniformName, const AnimaMatrix& value, bool transpose = false);
	void SetUniform(const char* uniformName, const AnimaMatrix& value, bool transpose = false);
	
private:
	AnimaEngine*	_engine;
	
	AnimaShader**	_shaders;
	ASizeT			_shadersNumber;

	AnimaShadersManager* _shadersManager;

	AUint _maxPointLights;
	AUint _maxSpotLights;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaUniformInfo, AnimaString::Hasher> _uniforms;
	boost::unordered_map<AnimaString, AnimaInputInfo, AnimaString::Hasher> _inputs;
	boost::unordered_map<AnimaString, AnimaOutputInfo, AnimaString::Hasher> _outputs;
#pragma warning (default: 4251) 

	AInt			_id;
	bool			_linked;
};

END_ANIMA_ENGINE_NAMESPACE