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
	AUint GetBindingPoint()	{ return _bindingPoint; }
	
	AUint GetBufferDataSize() const { return _bufferDataSize; }

	void SetBufferLength(AUint length);
	AUint GetBufferLength() const { return _bufferLength; }

	void SetSupportsInstance(bool supports) { _supportsInstance = supports; }
	bool CanSupportInstance() { return _supportsInstance; }

	bool BindForUpdate(AUint bufferIndex);
	bool UnbindFromUpdate();
	void SyncBuffers(AUint bufferIndex);

	static AUint GetMaxBuffersCount() { return s_maxBuffersCount; }

public:
	void UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset = 0);
	void UpdateValue(AnimaRenderer* renderer, const AnimaShaderProgram* program);

	void Enable(AUint bufferIndex);
	void EnableValue(AUint offset, AUint bufferIndex);
	
protected:
	void UpdateObjectValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset);
	void UpdateObjectInstanceValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset);
	
protected:
	AnimaMappedArray<AnimaShaderData> _data;

	AInt _groupLocation;
	AInt _bufferDataSize;
	AUint _bufferLength;
	AUint _bindingPoint;

	AnimaArray<AUint> _ubos;
	AnimaArray<GLsync> _fences;

	AUchar* _updateDataBuffer;
	
	bool _supportsInstance;

	bool _created;
	bool _needsResize;

	static AUint s_maxBuffersCount;
};

END_ANIMA_ENGINE_NAMESPACE