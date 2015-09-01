#include "AnimaShaderGroupData.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaMappedValues.h"
#include "AnimaGBuffer.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define BUFFERS_UPDATE_TIMEOUT	100000000

AUint AnimaShaderGroupData::s_maxBuffersCount = 5;

AnimaShaderGroupData::AnimaShaderGroupData(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderGroupData);

	_ubos.resize(s_maxBuffersCount, 0);
	_fences.resize(s_maxBuffersCount);

	_created = false;
	_groupLocation = -1;
	_bindingPoint = 0;
	_bufferDataSize = 0;
	_bufferLength = 1;
	_needsResize = false;
	_supportsInstance = false;
	_updateDataBuffer = nullptr;
}

AnimaShaderGroupData::AnimaShaderGroupData(const AnimaShaderGroupData& src)
	: AnimaNamedObject(src)
{
	_data = src._data;

	_ubos.resize(s_maxBuffersCount, 0);
	_fences.resize(s_maxBuffersCount);

	_created = false;
	_groupLocation = src._groupLocation;
	_bindingPoint = src._bindingPoint;
	_bufferDataSize = src._bufferDataSize;
	_bufferLength = src._bufferLength;
	_needsResize = false;
	_supportsInstance = src._supportsInstance;
	_updateDataBuffer = nullptr;
}

AnimaShaderGroupData::AnimaShaderGroupData(AnimaShaderGroupData&& src)
	: AnimaNamedObject(src)
{
	_data = src._data;

	_ubos.resize(s_maxBuffersCount, 0);
	_fences.resize(s_maxBuffersCount);

	_created = false;
	_groupLocation = src._groupLocation;
	_bindingPoint = src._bindingPoint;
	_bufferDataSize = src._bufferDataSize;
	_bufferLength = src._bufferLength;
	_needsResize = false;
	_supportsInstance = src._supportsInstance;
	_updateDataBuffer = nullptr;
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
		_needsResize = false;
		_supportsInstance = src._supportsInstance;
		_updateDataBuffer = nullptr;

		_ubos.resize(s_maxBuffersCount, 0);
		_fences.resize(s_maxBuffersCount);
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
		_needsResize = false;
		_supportsInstance = src._supportsInstance;
		_updateDataBuffer = nullptr;

		_ubos.resize(s_maxBuffersCount, 0);
		_fences.resize(s_maxBuffersCount);
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
			for (AUint i = 0; i < s_maxBuffersCount; i++)
			{
				// Attivo il buffer
				glBindBuffer(GL_UNIFORM_BUFFER, _ubos[i]);

				// Alloco la memoria del buffer
				glBufferData(GL_UNIFORM_BUFFER, _bufferDataSize * _bufferLength, nullptr, GL_STREAM_DRAW);

				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				if (glGetError() != GL_NO_ERROR)
					return false;
			}

			_needsResize = false;
		}

		return true;
	}

	glGenBuffers(s_maxBuffersCount, &_ubos[0]);

	if (glGetError() != GL_NO_ERROR)
		return false;

	for (AUint i = 0; i < s_maxBuffersCount; i++)
	{
		// Attivo il buffer
		glBindBuffer(GL_UNIFORM_BUFFER, _ubos[i]);

		// Alloco la memoria del buffer
		glBufferData(GL_UNIFORM_BUFFER, _bufferDataSize * _bufferLength, nullptr, GL_STREAM_DRAW);

		_fences[i] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

		if (glGetError() != GL_NO_ERROR)
			return false;
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	_created = true;

	return true;
}

void AnimaShaderGroupData::Analize(AnimaShaderProgram* program)
{
	if (program == nullptr)
		return;

	_groupLocation = glGetUniformBlockIndex(program->GetID(), _name.c_str());
	glGetActiveUniformBlockiv(program->GetID(), _groupLocation, GL_UNIFORM_BLOCK_DATA_SIZE, &_bufferDataSize);

	AInt alignment = sizeof(AFloat);
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &alignment);

	AInt span = _bufferDataSize % alignment;
	_bufferDataSize += (alignment - span);
	
	AInt count = _data.GetSize();

	AnimaArray<AInt> dataIndexes;
	AnimaArray<AInt> offsets;
	AnimaArray<AInt> arrayStrides;
	AnimaArray<AInt> matrixStrides;

	dataIndexes.resize(count);
	offsets.resize(count);
	arrayStrides.resize(count);
	matrixStrides.resize(count);

	glGetActiveUniformBlockiv(program->GetID(), _groupLocation, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &dataIndexes[0]);

	for (AInt i = 0; i < count; i++)
	{
		AUint location = dataIndexes[i];

		glGetActiveUniformsiv(program->GetID(), 1, &location, GL_UNIFORM_OFFSET, &offsets[i]);
		glGetActiveUniformsiv(program->GetID(), 1, &location, GL_UNIFORM_ARRAY_STRIDE, &arrayStrides[i]);
		glGetActiveUniformsiv(program->GetID(), 1, &location, GL_UNIFORM_MATRIX_STRIDE, &matrixStrides[i]);
	}

	for (AInt i = 1; i < count; i++)
	{
		AInt valO = offsets[i];
		AInt valA = arrayStrides[i];
		AInt valM = matrixStrides[i];

		AInt j = i - 1;
		while (j >= 0 && offsets[j] > valO)
		{
			offsets[j + 1] = offsets[j];
			arrayStrides[j + 1] = arrayStrides[j];
			matrixStrides[j + 1] = matrixStrides[j];

			j--;
		}

		offsets[j + 1] = valO;
		arrayStrides[j + 1] = valA;
		matrixStrides[j + 1] = valM;
	}

	for (AInt i = 0; i < count; i++)
	{
		_data[i].SetOffset(offsets[i]);
		_data[i].SetArrayStride(arrayStrides[i]);
		_data[i].SetMatrixStride(matrixStrides[i]);
	}
}

bool AnimaShaderGroupData::SetBindingPoint(AnimaShaderProgram* program, AUint bindingPoint)
{
	if (program == nullptr || !_created)
		return false;

	_bindingPoint = bindingPoint;

	for (AUint i = 0; i < s_maxBuffersCount; i++)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _ubos[i]);
		if (glGetError() != GL_NO_ERROR)
			return false;

		glUniformBlockBinding(program->GetID(), _groupLocation, _bindingPoint);
		if (glGetError() != GL_NO_ERROR)
			return false;
	}

	return true;
}

#define MAP_BUFFERS

bool AnimaShaderGroupData::BindForUpdate(AUint bufferIndex)
{
	if (!_created)
		return false;

#if defined MAP_BUFFERS
	AUint result = glClientWaitSync(_fences[bufferIndex], 0, BUFFERS_UPDATE_TIMEOUT);
	if(result == GL_TIMEOUT_EXPIRED || result == GL_WAIT_FAILED)
		return false;
	glDeleteSync(_fences[bufferIndex]);
#endif

	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _ubos[bufferIndex]);

#if defined MAP_BUFFERS
	//AUchar* buffer = (AUchar*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	_updateDataBuffer = (AUchar*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, _bufferDataSize, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	if (_updateDataBuffer == nullptr)
		return false;
#endif

	return true;
}

bool AnimaShaderGroupData::UnbindFromUpdate()
{
#if defined MAP_BUFFERS
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	_updateDataBuffer = nullptr;
#endif
	return true;
}

void AnimaShaderGroupData::SyncBuffers(AUint bufferIndex)
{
#if defined MAP_BUFFERS
	_fences[bufferIndex] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
#endif
}

void AnimaShaderGroupData::UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset)
{
	if (_supportsInstance)
		UpdateObjectInstanceValue(object, renderer, program, offset);
	else
		UpdateObjectValue(object, renderer, program, offset);
}

void AnimaShaderGroupData::UpdateObjectValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset)
{
	if (!Create())
		return;

	AInt dataCount = _data.GetSize();
	AUint bufferOffset = _bufferDataSize * offset;
	AUint size = 0;
	AUint arraySize = 0;

#if defined MAP_BUFFERS
	if (_updateDataBuffer == nullptr)
		return;
#endif
	
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

#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + bufferOffset, &val, size);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val);
#endif
			break;
		}
		case FLOAT_ARRAY:
		{
			break;
		}
		case FLOAT2:
		{
			size = sizeof(AFloat) * 2;
#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + bufferOffset, object->GetVector2f(propertyName).vec, size);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetVector2f(propertyName).vec);
#endif
			break;
		}
		case FLOAT2_ARRAY:
		{
			size = sizeof(AFloat) * 2 * arraySize;

			AnimaArray<AnimaVectorGenerator*>* vectorData = object->GetVectorArray(propertyName);

#if defined MAP_BUFFERS
			AUint tmpSize = sizeof(AFloat) * 2;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal->GetVector2f().vec, tmpSize);
				tmpOffset += tmpSize;
			}
#else
			AnimaArray<AFloat> val;

			val.resize(2 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 2;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(&val[tmpOffset], vecVal->GetVector2f().vec, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
#endif
			break;
		}
		case FLOAT3:
		{
			size = sizeof(AFloat) * 4;
#if defined MAP_BUFFERS
			if (object->HasColor(propertyName))
				memcpy(_updateDataBuffer + bufferOffset, object->GetColor3f(propertyName).vec, sizeof(AFloat) * 3);
			else
				memcpy(_updateDataBuffer + bufferOffset, object->GetVector3f(propertyName).vec, sizeof(AFloat) * 3);
#else
			AFloat val[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			if (object->HasColor(propertyName))
				memcpy(val, object->GetColor3f(propertyName).vec, sizeof(AFloat) * 3);
			else
				memcpy(val, object->GetVector3f(propertyName).vec, sizeof(AFloat) * 3);

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, val);
#endif
			break;
		}
		case FLOAT3_ARRAY:
		{
			size = sizeof(AFloat) * 4 * arraySize;

			AnimaArray<AnimaVectorGenerator*>* vectorData = object->GetVectorArray(propertyName);

#if defined MAP_BUFFERS
			AUint tmpSize = sizeof(AFloat) * 4;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				if (object->HasColor(propertyName))
					memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal->GetColor4f().vec, tmpSize);
				else
					memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal->GetVector4f().vec, tmpSize);
				tmpOffset += tmpSize;
			}
#else
			AnimaArray<AFloat> val;

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
#endif
			break;
		}
		case FLOAT4:
		{
			size = sizeof(AFloat) * 4;
#if defined MAP_BUFFERS
			if (object->HasColor(propertyName))
				memcpy(_updateDataBuffer + bufferOffset, object->GetColor4f(propertyName).vec, size);
			else
				memcpy(_updateDataBuffer + bufferOffset, object->GetVector4f(propertyName).vec, size);
#else
			if (object->HasColor(propertyName))
				glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetColor4f(propertyName).vec);
			else
				glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetVector4f(propertyName).vec);
#endif
			break;
		}
		case FLOAT4_ARRAY:
		{
			size = sizeof(AFloat) * 4 * arraySize;

			AnimaArray<AnimaVectorGenerator*>* vectorData = object->GetVectorArray(propertyName);

#if defined MAP_BUFFERS
			AUint tmpSize = sizeof(AFloat) * 4;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				if (object->HasColor(propertyName))
					memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal->GetColor4f().vec, tmpSize);
				else
					memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal->GetVector4f().vec, tmpSize);
				tmpOffset += tmpSize;
			}
#else
			AnimaArray<AFloat> val;

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
#endif
			break;
		}
		case MATRIX4x4:
		{
			size = sizeof(AFloat) * 16;
#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + bufferOffset, object->GetMatrix(propertyName).m, size);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetMatrix(propertyName).m);
#endif
			break;
		}
		case MATRIX4x4_ARRAY:
		{
			size = sizeof(AFloat) * 16 * arraySize;

			AnimaArray<AnimaMatrix>* vectorData = object->GetMatrixArray(propertyName);

#if defined MAP_BUFFERS
			AUint tmpSize = sizeof(AFloat) * 16;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal.m, tmpSize);
				tmpOffset += tmpSize;
			}
#else
			AnimaArray<AFloat> val;

			val.resize(16 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 16;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(&val[tmpOffset], vecVal.m, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
#endif
			break;
		}
		case MATRIX3x3:
		{
			size = sizeof(AFloat) * 16;
#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + bufferOffset, object->GetMatrix(propertyName).m, size);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, object->GetMatrix(propertyName).m);
#endif
			break;
		}
		case MATRIX3x3_ARRAY:
		{
			size = sizeof(AFloat) * 16 * arraySize;
			AnimaArray<AnimaMatrix>* vectorData = object->GetMatrixArray(propertyName);

#if defined MAP_BUFFERS
			AUint tmpSize = sizeof(AFloat) * 16;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(_updateDataBuffer + bufferOffset + tmpOffset, vecVal.m, tmpSize);
				tmpOffset += tmpSize;
			}
#else
			AnimaArray<AFloat> val;

			val.resize(16 * arraySize);

			AUint tmpSize = sizeof(AFloat) * 16;
			AUint tmpOffset = 0;

			for (auto& vecVal : (*vectorData))
			{
				memcpy(&val[tmpOffset], vecVal.m, tmpSize);
				tmpOffset += tmpSize;
			}

			glBufferSubData(GL_UNIFORM_BUFFER, bufferOffset, size, &val[0]);
#endif
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
	}
}

void AnimaShaderGroupData::UpdateObjectInstanceValue(const AnimaMappedValues* object, AnimaRenderer* renderer, const AnimaShaderProgram* program, AUint offset)
{
	if (!Create())
		return;

	AInt dataCount = _data.GetSize();
	AUint bufferOffset = 0;

#if defined MAP_BUFFERS
	if (_updateDataBuffer == nullptr)
		return;
#endif

	ANIMA_FRAME_PUSH("cicle");
	for (AInt i = 0; i < dataCount; i++)
	{
		AnimaShaderData* data = &_data[i];
		AnimaString propertyName = data->GetNamePart(0);
		AInt dataOffset = data->GetOffset();
		AUint singleDataSize = 0;

		switch (data->GetType())
		{
		case FLOAT_ARRAY:		singleDataSize = sizeof(AFloat);		break;
		case FLOAT2_ARRAY:		singleDataSize = sizeof(AFloat) * 2;	break;
		case FLOAT3_ARRAY:
		case FLOAT4_ARRAY:		singleDataSize = sizeof(AFloat) * 4;	break;
		case MATRIX4x4_ARRAY:
		case MATRIX3x3_ARRAY:	singleDataSize = sizeof(AFloat) * 16;	break;
		}

		bufferOffset = data->GetArrayStride() * offset;
		
		ANIMA_FRAME_PUSH("data update");
		switch (data->GetType())
		{
		case FLOAT_ARRAY:
		{
			AFloat val = object->GetFloat(propertyName);

#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + dataOffset + bufferOffset, &val, singleDataSize);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, dataOffset + bufferOffset, singleDataSize, &val);
#endif
			break;
		}
		case FLOAT2_ARRAY:
		{
#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + dataOffset + bufferOffset, object->GetVector2f(propertyName).vec, singleDataSize);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, dataOffset + bufferOffset, singleDataSize, object->GetVector2f(propertyName).vec);
#endif
			break;
		}
		case FLOAT3_ARRAY:
		{
#if defined MAP_BUFFERS
			if (object->HasColor(propertyName))
				memcpy(_updateDataBuffer + dataOffset + bufferOffset, object->GetColor3f(propertyName).vec, sizeof(AFloat) * 3);
			else
				memcpy(_updateDataBuffer + dataOffset + bufferOffset, object->GetVector3f(propertyName).vec, sizeof(AFloat) * 3);
#else
			AFloat val[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			if (object->HasColor(propertyName))
				memcpy(val, object->GetColor3f(propertyName).vec, sizeof(AFloat) * 3);
			else
				memcpy(val, object->GetVector3f(propertyName).vec, sizeof(AFloat) * 3);

			glBufferSubData(GL_UNIFORM_BUFFER, dataOffset + bufferOffset, singleDataSize, val);
#endif
			break;
		}
		case FLOAT4_ARRAY:
		{
#if defined MAP_BUFFERS
			if (object->HasColor(propertyName))
				memcpy(_updateDataBuffer + dataOffset + bufferOffset, object->GetColor4f(propertyName).vec, singleDataSize);
			else
				memcpy(_updateDataBuffer + dataOffset + bufferOffset, object->GetVector4f(propertyName).vec, singleDataSize);
#else
			if (object->HasColor(propertyName))
				glBufferSubData(GL_UNIFORM_BUFFER, dataOffset + bufferOffset, singleDataSize, object->GetColor4f(propertyName).vec);
			else
				glBufferSubData(GL_UNIFORM_BUFFER, dataOffset + bufferOffset, singleDataSize, object->GetVector4f(propertyName).vec);
#endif
			break;
		}
		case MATRIX4x4_ARRAY:
		case MATRIX3x3_ARRAY:
		{
#if defined MAP_BUFFERS
			memcpy(_updateDataBuffer + dataOffset + bufferOffset, object->GetMatrix(propertyName).m, singleDataSize);
#else
			glBufferSubData(GL_UNIFORM_BUFFER, dataOffset + bufferOffset, singleDataSize, object->GetMatrix(propertyName).m);
#endif
			break;
		}
		}
		ANIMA_FRAME_POP();
	}
	ANIMA_FRAME_POP();
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

void AnimaShaderGroupData::EnableValue(AUint offset, AUint bufferIndex)
{
	if (!Create())
		return;

	glBindBuffer(GL_UNIFORM_BUFFER, _ubos[bufferIndex]);
	glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _ubos[bufferIndex], _bufferDataSize * offset, _bufferDataSize);
}
void AnimaShaderGroupData::Enable(AUint bufferIndex)
{
	if (!Create())
		return;

	glBindBuffer(GL_UNIFORM_BUFFER, _ubos[bufferIndex]);
	glBindBufferRange(GL_UNIFORM_BUFFER, _bindingPoint, _ubos[bufferIndex], 0, _bufferDataSize * _bufferLength);
}

END_ANIMA_ENGINE_NAMESPACE