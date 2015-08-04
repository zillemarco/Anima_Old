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
#include "AnimaScene.h"
#include "AnimaNamedObject.h"
#include "AnimaShaderData.h"

#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShadersManager;
class AnimaRenderer;

enum AnimaShaderInfoType {
	SHADER_FILE,
	SHADER_TEXT
};

class ANIMA_ENGINE_EXPORT AnimaShaderProgram : public AnimaNamedObject
{
public:
	struct AnimaShaderInfo {
		AnimaString			_text;
		AnimaShaderInfoType	_infoType;
		AnimaShaderType		_shaderType;

		AnimaShaderInfo() {
			_text = "";
			_infoType = SHADER_FILE;
			_shaderType = VERTEX;
		}
	};

	struct AnimaUniformInfo {
		AInt* _locations;
		AInt _locationsCount;
		AUint _type;
		AnimaString _name;
		AUint _arraySize;

		AnimaString* _nameParts;
		AUint _namePartsCount;

		AnimaUniformInfo() {
			_locations = nullptr;
			_locationsCount = 0;
			_type = GL_FLOAT;
			_nameParts = nullptr;
			_namePartsCount = 0;
			_arraySize = 1;
		}
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
	AnimaShaderProgram(const AnimaString& name, AnimaAllocator* allocator, AnimaShadersManager* shadersManager);
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

	void EnableInputs(AnimaMesh* mesh);
	void EnableInput(const AnimaString& inputName, AInt size, AUint type, AUint buffer);
	void DisableInputs();

	void UpdateSceneObjectProperties(AnimaSceneObject* object, AnimaRenderer* renderingManager);
	void UpdateMappedValuesObjectProperties(AnimaMappedValues* object, AnimaRenderer* renderingManager);
	void UpdateRenderingManagerProperies(AnimaRenderer* renderingManager);

	void SetUniformi(const AnimaString& uniformName, int value);
	void SetUniformf(const AnimaString& uniformName, AFloat value);
	void SetUniform(const AnimaString& uniformName, const AnimaVertex2f& value);
	void SetUniform(const AnimaString& uniformName, const AnimaVertex3f& value);
	void SetUniform(const AnimaString& uniformName, const AnimaColor4f& value);
	void SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c);
	void SetUniform(const AnimaString& uniformName, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(const AnimaString& uniformName, const AnimaMatrix& value, bool transpose = false);
	void SetUniform(const AnimaString& uniformName, const AnimaArray<AnimaVectorGenerator*>* value, AUint type);
	void SetUniform(const AnimaString& uniformName, const AnimaArray<AnimaMatrix>* value);

	void SetUniformi(AInt location, int value);
	void SetUniformf(AInt location, AFloat value);
	void SetUniform(AInt location, const AnimaVertex2f& value);
	void SetUniform(AInt location, const AnimaVertex3f& value);
	void SetUniform(AInt location, const AnimaColor4f& value);
	void SetUniform(AInt location, AFloat a, AFloat b, AFloat c);
	void SetUniform(AInt location, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(AInt location, const AnimaMatrix& value, bool transpose = false);

	void AddShaderData(AnimaShaderData& data);
	AnimaShaderData* GetShaderData(const AnimaString& name);

private:
	void ClearUniforms();
	void UpdateDataLookup();
	
private:
	AnimaShader**	_shaders;
	ASizeT			_shadersCount;

	AnimaShadersManager* _shadersManager;

	AnimaMappedArray<AnimaShaderData> _data;

	AInt			_id;
	bool			_linked;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaUniformInfo, AnimaStringHasher> _uniforms;
	boost::unordered_map<AnimaString, AnimaInputInfo, AnimaStringHasher> _inputs;
	boost::unordered_map<AnimaString, AnimaOutputInfo, AnimaStringHasher> _outputs;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE