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
#include "AnimaShaderGroupData.h"

#include <boost/unordered_map.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShadersManager;
class AnimaRenderer;

enum AnimaShaderInfoType 
{
	SHADER_FILE,
	SHADER_TEXT
};

class ANIMA_ENGINE_EXPORT AnimaShaderProgram : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaShaderProgram);

public:	
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

	bool IsCreated() const;
	bool IsLinked() const;

	AInt GetID() const;

public:
	void ScanVariables();

	void EnableInputs(AnimaMesh* mesh);
	void EnableInput(const AnimaString& inputName, AInt size, AUint type, AUint buffer);
	void DisableInputs();

	void UpdateSceneObjectProperties(AnimaSceneObject* object, AnimaRenderer* renderingManager);
	void UpdateMappedValuesObjectProperties(AnimaMappedValues* object, AnimaRenderer* renderingManager);
	void UpdateRenderingManagerProperies(AnimaRenderer* renderingManager);

	void SetUniformi(AInt location, int value);
	void SetUniformf(AInt location, AFloat value);
	void SetUniform(AInt location, const AnimaVertex2f& value);
	void SetUniform(AInt location, const AnimaVertex3f& value);
	void SetUniform(AInt location, const AnimaColor4f& value);
	void SetUniform(AInt location, AFloat a, AFloat b, AFloat c);
	void SetUniform(AInt location, AFloat a, AFloat b, AFloat c, AFloat d);
	void SetUniform(AInt location, const AnimaMatrix& value, bool transpose = false);

	void AddShaderData(const AnimaShaderData& data);
	AInt GetShaderDataCount() const;
	AnimaShaderData* GetShaderData(const AInt& index);
	AnimaShaderData* GetShaderData(const AnimaString& name);

	void AddShaderStaticGroupData(const AnimaShaderGroupData& groupData);
	AInt GetShaderStaticGroupDataCount() const;
	AnimaShaderGroupData* GetShaderStaticGroupData(const AInt& index);
	AnimaShaderGroupData* GetShaderStaticGroupData(const AnimaString& name);

	void AddShaderDynamicGroupData(const AnimaShaderGroupData& groupData);
	AInt GetShaderDynamicGroupDataCount() const;
	AnimaShaderGroupData* GetShaderDynamicGroupData(const AInt& index);
	AnimaShaderGroupData* GetShaderDynamicGroupData(const AnimaString& name);

	void SetSupportsInstance(bool supports) { _supportsInstance = supports; }
	bool CanSupportInstance() { return _supportsInstance; }

	void SetMaxInstances(AUint maxInstances) { _maxInstances = maxInstances; }
	AUint GetMaxInstances() { return _maxInstances; }

	void SyncBuffers(AUint buffersIndex);

private:
	void UpdateDataLookup();
	
private:
	AnimaShader**	_shaders;
	ASizeT			_shadersCount;

	AnimaShadersManager* _shadersManager;

	AnimaMappedArray<AnimaShaderData> _data;
	AnimaMappedArray<AnimaShaderGroupData> _staticGroupData;
	AnimaMappedArray<AnimaShaderGroupData> _dynamicGroupData;

	AInt _id;
	bool _linked;

	bool _supportsInstance;
	AUint _maxInstances;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AnimaInputInfo, AnimaStringHasher> _inputs;
	boost::unordered_map<AnimaString, AnimaOutputInfo, AnimaStringHasher> _outputs;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE