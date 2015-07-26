#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"
#include "AnimaArray.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"
#include "AnimaTexture.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaRenderer;
class AnimaMappedValues;

enum AnimaShaderInputType {
	FLOAT,
	FLOAT_ARRAY,
	FLOAT2,
	FLOAT2_ARRAY,
	FLOAT3,
	FLOAT3_ARRAY,
	FLOAT4,
	FLOAT4_ARRAY,
	MATRIX4x4,
	MATRIX4x4_ARRAY,
	MATRIX3x3,
	MATRIX3x3_ARRAY,
	INT,
	INT_ARRAY,
	BOOL,
	BOOL_ARRAY,
	TEXTURE2D,
	TEXTURE2D_ARRAY,
	TEXTURE3D,
	TEXTURE3D_ARRAY,
	NONE
};

class ANIMA_ENGINE_EXPORT AnimaShaderInput : public AnimaNamedObject
{
public:
	AnimaShaderInput(const AnimaString& name);
	AnimaShaderInput(const AnimaShaderInput& src);
	AnimaShaderInput(AnimaShaderInput&& src);
	~AnimaShaderInput();
	
	AnimaShaderInput& operator=(const AnimaShaderInput& src);
	AnimaShaderInput& operator=(AnimaShaderInput&& src);

	bool operator==(const AnimaShaderInput& left);
	bool operator!=(const AnimaShaderInput& left);

public:
	void FindLocation(AnimaShaderProgram* program);

	void SetType(AnimaShaderInputType type) { _type = type; }
	AnimaShaderInputType GetType() const { return _type; }

	AnimaString GetNamePart(AInt index) const { return _nameParts[index]; }
	AnimaString GetPrefix() { return _nameParts[0]; }

public:
	void UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer);

	void UpdateValue(const AFloat& value);
	void UpdateValue(const AnimaArray<AFloat>& value);
	void UpdateValue(const AnimaVertex2f& value);
	void UpdateValue(const AnimaArray<AnimaVertex2f>& value);
	void UpdateValue(const AnimaVertex3f& value);
	void UpdateValue(const AnimaArray<AnimaVertex3f>& value);
	void UpdateValue(const AnimaVertex4f& value);
	void UpdateValue(const AnimaArray<AnimaVertex4f>& value);
	void UpdateValue(const AnimaMatrix& value);
	void UpdateValue(const AnimaArray<AnimaMatrix>& value);
	void UpdateValue(const AInt& value);
	void UpdateValue(const AnimaArray<AInt>& value);
	void UpdateValue(const bool& value);
	void UpdateValue(const AnimaArray<bool>& value);
	void UpdateValue(AnimaTexture* value, const AInt& slot);
	void UpdateValue(AnimaArray<AnimaTexture*>& value, const AInt& slot);

protected:
	void DivideName();

protected:
	AnimaArray<AnimaString> _nameParts;
	AnimaArray<AInt> _locations;
	
	AnimaShaderInputType _type;

	AInt _arraySize;
};

END_ANIMA_ENGINE_NAMESPACE