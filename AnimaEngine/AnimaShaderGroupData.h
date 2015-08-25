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
	void FindLocation(AnimaShaderProgram* program);

	void AddShaderData(const AnimaShaderData& data);
	AInt GetShaderDataCount() const;
	AnimaShaderData* GetShaderData(const AInt& index);
	AnimaShaderData* GetShaderData(const AnimaString& name);

	void SetBindingPoint(AUint index)	{ _bindingPoint = index; }
	AUint GetBindingPoint()				{ return _bindingPoint; }
	
	AUint GetBufferDataSize() const { return _bufferDataSize; }

	void SetBufferLength(AUint length);
	AUint GetBufferLength() const { return _bufferLength; }

public:
	void UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset = 0);
	void UpdateValue(AnimaRenderer* renderer, const AnimaShaderProgram* program);

	void EnableValue(AUint offset, bool link);

protected:
	AUint CalculateBufferDataSize();
	
protected:
	AnimaMappedArray<AnimaShaderData> _data;

	AInt _groupLocation;
	AUint _ubo;
	AUint _bufferDataSize;
	AUint _bufferLength;
	AUint _bindingPoint;

	bool _created;
	bool _needsResize;
};

END_ANIMA_ENGINE_NAMESPACE