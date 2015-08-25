#include "AnimaShaderGroupData.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaMappedValues.h"
#include "AnimaGBuffer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShaderGroupData::AnimaShaderGroupData(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderGroupData);

	_created = false;
	_groupLocation = -1;
	_ubo = 0;
	_bindingPoint = 0;
	_bufferDataSize = 0;
	_bufferLength = 1;
	_needsResize = false;
}

AnimaShaderGroupData::AnimaShaderGroupData(const AnimaShaderGroupData& src)
	: AnimaNamedObject(src)
{
	_data = src._data;

	_created = false;
	_groupLocation = src._groupLocation;
	_bindingPoint = src._bindingPoint;
	_bufferDataSize = src._bufferDataSize;
	_bufferLength = src._bufferLength;
	_ubo = 0;
	_needsResize = false;
}

AnimaShaderGroupData::AnimaShaderGroupData(AnimaShaderGroupData&& src)
	: AnimaNamedObject(src)
{
	_data = src._data;

	_created = false;
	_groupLocation = src._groupLocation;
	_bindingPoint = src._bindingPoint;
	_bufferDataSize = src._bufferDataSize;
	_bufferLength = src._bufferLength;
	_ubo = 0;
	_needsResize = false;
}

AnimaShaderGroupData::~AnimaShaderGroupData()
{
}

AnimaShaderGroupData& AnimaShaderGroupData::operator=(const AnimaShaderGroupData& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		_data = src._data;

		_created = false;
		_groupLocation = src._groupLocation;
		_bindingPoint = src._bindingPoint;
		_bufferDataSize = src._bufferDataSize;
		_bufferLength = src._bufferLength;
		_ubo = 0;
		_needsResize = false;
	}

	return *this;
}

AnimaShaderGroupData& AnimaShaderGroupData::operator=(AnimaShaderGroupData&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		_data = src._data;

		_created = false;
		_groupLocation = src._groupLocation;
		_bindingPoint = src._bindingPoint;
		_bufferDataSize = src._bufferDataSize;
		_bufferLength = src._bufferLength;
		_ubo = 0;
		_needsResize = false;
	}

	return *this;
}

void AnimaShaderGroupData::AddShaderData(const AnimaShaderData& data)
{
	_data.Add(data.GetName(), data);
}

AInt AnimaShaderGroupData::GetShaderDataCount() const
{
	return _data.GetSize();
}

AnimaShaderData* AnimaShaderGroupData::GetShaderData(const AnimaString& name)
{
	return &_data[name];
}

AnimaShaderData* AnimaShaderGroupData::GetShaderData(const AInt& index)
{
	return &_data[index];
}


bool AnimaShaderGroupData::Create()
{
	if (_created)
	{
		if (_needsResize)
		{
			// Attivo il buffer
			glBindBuffer(GL_UNIFORM_BUFFER, _ubo);

			// Alloco la memoria del buffer
			glBufferData(GL_UNIFORM_BUFFER, CalculateBufferDataSize() * _bufferLength, nullptr, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			if (glGetError() != GL_NO_ERROR)
				return false;

			_needsResize = false;
		}

		return true;
	}

	glGenBuffers(1, &_ubo);

	if (glGetError() != GL_NO_ERROR)
		return false;

	// Attivo il buffer
	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);

	// Alloco la memoria del buffer
	glBufferData(GL_UNIFORM_BUFFER, CalculateBufferDataSize() * _bufferLength, nullptr, GL_DYNAMIC_DRAW);

	if (glGetError() != GL_NO_ERROR)
		return false;

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	_created = true;

	return true;
}

void AnimaShaderGroupData::FindLocation(AnimaShaderProgram* program)
{
	if (program == nullptr)
		return;

	_groupLocation = glGetUniformBlockIndex(program->GetID(), _name.c_str());
}

AUint AnimaShaderGroupData::CalculateBufferDataSize()
{
	_bufferDataSize = 0;

	AInt count = _data.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		AnimaShaderData* data = &_data[i];

		switch (data->GetType())
		{
		case FLOAT:				_bufferDataSize += sizeof(AFloat); break;
		case FLOAT_ARRAY:		_bufferDataSize += sizeof(AFloat) * data->GetArraySize(); break;
		case FLOAT2:			_bufferDataSize += sizeof(AFloat) * 2; break;
		case FLOAT2_ARRAY:		_bufferDataSize += sizeof(AFloat) * 2 * data->GetArraySize(); break;
		case FLOAT3:			_bufferDataSize += sizeof(AFloat) * 4; break;
		case FLOAT3_ARRAY:		_bufferDataSize += sizeof(AFloat) * 4 * data->GetArraySize(); break;
		case FLOAT4:			_bufferDataSize += sizeof(AFloat) * 4; break;
		case FLOAT4_ARRAY:		_bufferDataSize += sizeof(AFloat) * 4 * data->GetArraySize(); break;
		case MATRIX4x4:			_bufferDataSize += sizeof(AFloat) * 16; break;
		case MATRIX4x4_ARRAY:	_bufferDataSize += sizeof(AFloat) * 16 * data->GetArraySize(); break;
		case MATRIX3x3:			_bufferDataSize += sizeof(AFloat) * 16; break;
		case MATRIX3x3_ARRAY:	_bufferDataSize += sizeof(AFloat) * 16 * data->GetArraySize(); break;
		//case INT:				_bufferDataSize += sizeof(AFloat); break;
		//case INT_ARRAY:		_bufferDataSize += sizeof(AFloat) * data->GetArraySize(); break;
		//case BOOL:			_bufferDataSize += sizeof(AFloat); break;
		//case BOOL_ARRAY:		_bufferDataSize += sizeof(AFloat) * data->GetArraySize(); break;
		}
	}

	AInt alignment = sizeof(AFloat);
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);

	AInt span = _bufferDataSize % alignment;
	_bufferDataSize += (alignment - span);

	return _bufferDataSize;
}

void AnimaShaderGroupData::UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset)
{
	if (!Create())
		return;

	if (glGetError() != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _ubo);
	if (glGetError() != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
	}

	glUniformBlockBinding(program->GetID(), _groupLocation, _bindingPoint);
	if (glGetError() != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
	}

	AInt dataCount = _data.GetSize();
	AUint bufferOffset = _bufferDataSize * offset;
	AUint size = 0;
	AUint arraySize = 0;
	
	for (AInt i = 0; i < dataCount; i++)
	{
		AnimaShaderData* data = &_data[i];
		AnimaString propertyName = data->GetNamePart(0);
		arraySize = data->GetArraySize();

		switch (data->GetType())
		{
		case FLOAT:
		{
			size = sizeof(AFloat);
			AFloat val = object->GetFloat(propertyName);
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val);
			break;
		}
		case FLOAT_ARRAY:
		{
			break;
		}
		case FLOAT2:
		{
			size = sizeof(AFloat) * 2;
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetVector2f(propertyName).vec);
			break;
		}
		case FLOAT2_ARRAY:
		{
			size = sizeof(AFloat) * 2 * arraySize;

			AnimaArray<AFloat> val;
			AnimaArray<AnimaVectorGenerator*>* vectorData = object->GetVectorArray(propertyName);

			val.resize(2 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 2;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(&val[tmpOffset], vecVal->GetVector2f().vec, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
			break;
		}
		case FLOAT3:
		{
			size = sizeof(AFloat) * 4;
			AFloat val[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			if (object->HasColor(propertyName))
				memcpy(val, object->GetColor3f(propertyName).vec, sizeof(AFloat) * 3);
			else
				memcpy(val, object->GetVector3f(propertyName).vec, sizeof(AFloat) * 3);

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, val);
			break;
		}
		case FLOAT3_ARRAY:
		{
			size = sizeof(AFloat) * 4 * arraySize;

			AnimaArray<AFloat> val;
			AnimaArray<AnimaVectorGenerator*>* vectorData = object->GetVectorArray(propertyName);

			val.resize(4 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 4;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				if (object->HasColor(propertyName))
					memcpy(&val[tmpOffset], vecVal->GetColor4f().vec, tmpSize);
				else
					memcpy(&val[tmpOffset], vecVal->GetVector4f().vec, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
			break;
		}
		case FLOAT4:
		{
			size = sizeof(AFloat) * 4;
			if (object->HasColor(propertyName))
				glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetColor4f(propertyName).vec);
			else
				glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetVector4f(propertyName).vec);
			break;
		}
		case FLOAT4_ARRAY:
		{
			size = sizeof(AFloat) * 4 * arraySize;

			AnimaArray<AFloat> val;
			AnimaArray<AnimaVectorGenerator*>* vectorData = object->GetVectorArray(propertyName);

			val.resize(4 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 4;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				if (object->HasColor(propertyName))
					memcpy(&val[tmpOffset], vecVal->GetColor4f().vec, tmpSize);
				else
					memcpy(&val[tmpOffset], vecVal->GetVector4f().vec, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
			break;
		}
		case MATRIX4x4:
		{
			size = sizeof(AFloat) * 16;
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetMatrix(propertyName).m);
			break;
		}
		case MATRIX4x4_ARRAY:
		{
			size = sizeof(AFloat) * 16 * arraySize;

			AnimaArray<AFloat> val;
			AnimaArray<AnimaMatrix>* vectorData = object->GetMatrixArray(propertyName);

			val.resize(16 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 16;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(&val[tmpOffset], vecVal.m, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
			break;
		}
		case MATRIX3x3:
		{
			size = sizeof(AFloat) * 16;
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetMatrix(propertyName).m);
			break;
		}
		case MATRIX3x3_ARRAY:
		{
			size = sizeof(AFloat) * 16 * arraySize;

			AnimaArray<AFloat> val;
			AnimaArray<AnimaMatrix>* vectorData = object->GetMatrixArray(propertyName);

			val.resize(16 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 16;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(&val[tmpOffset], vecVal.m, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
			break;
		}
		case INT:
		{
			break;
		}
		case INT_ARRAY:
		{
			break;
		}
		case BOOL:
		{
			break;
		}
		case BOOL_ARRAY:
		{
			break;
		}
		}

		bufferOffset += size;

		if (glGetError() != GL_NO_ERROR)
		{
			ANIMA_ASSERT(false);
		}
	}
}

void AnimaShaderGroupData::UpdateValue(AnimaRenderer* renderer, const AnimaShaderProgram* program)
{
}

void AnimaShaderGroupData::SetBufferLength(AUint length)
{
	if (_bufferLength != length)
	{
		_bufferLength = length;
		_needsResize = true;
	}
}

void AnimaShaderGroupData::EnableValue(AUint offset, bool link)
{
	if (!Create())
		return;

	glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
	if (glGetError() != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
	}

	glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _ubo, _bufferDataSize * offset, _bufferDataSize);
	AUint error = glGetError();
	if (error != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
	}
}

END_ANIMA_ENGINE_NAMESPACE