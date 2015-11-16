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
class AnimaGBuffer;

enum AnimaShaderDataType {
	ASDT_FLOAT,
	ASDT_FLOAT_ARRAY,
	ASDT_FLOAT2,
	ASDT_FLOAT2_ARRAY,
	ASDT_FLOAT3,
	ASDT_FLOAT3_ARRAY,
	ASDT_FLOAT4,
	ASDT_FLOAT4_ARRAY,
	ASDT_MATRIX4x4,
	ASDT_MATRIX4x4_ARRAY,
	ASDT_MATRIX3x3,
	ASDT_MATRIX3x3_ARRAY,
	ASDT_INT,
	ASDT_INT_ARRAY,
	ASDT_BOOL,
	ASDT_BOOL_ARRAY,
	ASDT_TEXTURE2D,
	ASDT_TEXTURE2D_ARRAY,
	ASDT_TEXTURE3D,
	ASDT_TEXTURE3D_ARRAY,
	ASDT_TEXTURECUBE,
	ASDT_TEXTURECUBE_ARRAY,
	ASDT_NONE
};

enum AnimaShaderDataSourceObject {
	ASDSO_GEOMETRY,
	ASDSO_NODE,
	ASDSO_RENDERER,
	ASDSO_MATERIAL,
	ASDSO_LIGHT,
	ASDSO_GBUFFER,
	ASDSO_CAMERA,
	ASDSO_NONE
};

class ANIMA_ENGINE_EXPORT AnimaShaderData : public AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaShaderData);

public:
	AnimaShaderData(const AnimaString& name);
	AnimaShaderData(const AnimaShaderData& src);
	AnimaShaderData(AnimaShaderData&& src);
	~AnimaShaderData();
	
	AnimaShaderData& operator=(const AnimaShaderData& src);
	AnimaShaderData& operator=(AnimaShaderData&& src);

	bool operator==(const AnimaShaderData& left);
	bool operator!=(const AnimaShaderData& left);

public:
	void Analize(AnimaShaderProgram* program);

	void SetName(const AnimaString& name) override;

	void SetType(AnimaShaderDataType type) { _type = type; }
	AnimaShaderDataType GetType() const { return _type; }
	
	void SetSourceObject(AnimaShaderDataSourceObject sourceObject, const AnimaString& slotName = "") { _sourceObject = sourceObject; _slotName = slotName; }
	AnimaShaderDataSourceObject GetSourceObject() const { return _sourceObject; }
	AnimaString GetSlotName() const { return _slotName; }

	void SetArraySize(AInt size) { _arraySize = size; }
	AInt GetArraySize() const { return _arraySize; }

//	AnimaString GetNamePart(AInt index) const { return _nameParts[index]; }
//	AnimaString GetPrefix() { return _nameParts[0]; }
//	AInt GetNamePartsCount() const { return _nameParts.size(); }

	void SetOffset(AInt offset) { _offset = offset; }
	AInt GetOffset() const { return _offset; }
	
	void SetArrayStride(AInt arrayStride) { _arrayStride = arrayStride; }
	AInt GetArrayStride() const { return _arrayStride; }

	void SetMatrixStride(AInt matrixStride) { _matrixStride = matrixStride; }
	AInt GetMatrixStride() const { return _matrixStride; }
	
	void SetAssociatedWith(const AnimaString& associatedWith) { _associatedWith = associatedWith; }
	AnimaString AssociatedWith() const { return _associatedWith.empty() ? GetName() : _associatedWith; }
	
public:
	void UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer);
	void UpdateValue(AnimaRenderer* renderer);

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
	
//protected:
//	void DivideName();

protected:
//	AnimaArray<AnimaString> _nameParts;
	AnimaArray<AInt> _locations;
	
	AnimaShaderDataType _type;
	AnimaShaderDataSourceObject _sourceObject;
	AnimaString _slotName;
	AnimaString _associatedWith;

	AInt _arraySize;

	AInt _offset;
	AInt _arrayStride;
	AInt _matrixStride;
};

END_ANIMA_ENGINE_NAMESPACE