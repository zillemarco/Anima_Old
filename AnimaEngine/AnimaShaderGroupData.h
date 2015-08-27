#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"
#include "AnimaMappedArray.h"
#include "AnimaShaderData.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaRenderer;
class AnimaMappedValues;
class AnimaGBuffer;

class ANIMA_ENGINE_EXPORT AnimaShaderGroupData : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaShaderGroupData);

public:
	AnimaShaderGroupData(const AnimaString& name, AnimaAllocator* allocator);
	AnimaShaderGroupData(const AnimaShaderGroupData& src);
	AnimaShaderGroupData(AnimaShaderGroupData&& src);
	~AnimaShaderGroupData();
	
	AnimaShaderGroupData& operator=(const AnimaShaderGroupData& src);
	AnimaShaderGroupData& operator=(AnimaShaderGroupData&& src);

	bool operator==(const AnimaShaderGroupData& left);
	bool operator!=(const AnimaShaderGroupData& left);

public:
	bool Create();
	void Analize(AnimaShaderProgram* program);

	void AddShaderData(const AnimaShaderData& data);
	AInt GetShaderDataCount() const;
	AnimaShaderData* GetShaderData(const AInt& index);
	AnimaShaderData* GetShaderData(const AnimaString& name);

	bool SetBindingPoint(AnimaShaderProgram* program, AUint bindingPoint);
	AUint GetBindingPoint()				{ return _bindingPoint; }
	
	AUint GetBufferDataSize() const { return _bufferDataSize; }

	void SetBufferLength(AUint length);
	AUint GetBufferLength() const { return _bufferLength; }

	void SetSupportsInstance(bool supports) { _supportsInstance = supports; }
	bool CanSupportInstance() { return _supportsInstance; }

	bool BindForUpdate() const;

public:
	void UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset = 0);
	void UpdateValue(AnimaRenderer* renderer, const AnimaShaderProgram* program);

	void Enable();
	void EnableValue(AUint offset);

protected:
	void UpdateObjectValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset = 0);
	void UpdateObjectInstanceValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset = 0);
	
protected:
	AnimaMappedArray<AnimaShaderData> _data;

	AInt _groupLocation;
	AUint _ubo;
	AInt _bufferDataSize;
	AUint _bufferLength;
	AUint _bindingPoint;
	
	bool _supportsInstance;

	bool _created;
	bool _needsResize;
};

END_ANIMA_ENGINE_NAMESPACE