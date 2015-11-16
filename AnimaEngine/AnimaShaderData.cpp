#include "AnimaShaderData.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaMappedValues.h"
#include "AnimaGBuffer.h"
#include "AnimaLogger.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShaderData::AnimaShaderData(const AnimaString& name)
	: AnimaNamedObject(name, nullptr)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderData);
	
	_type = ASDT_NONE;
	_sourceObject = ASDSO_NODE;
	_slotName = "";
	_associatedWith = "";
	
	_arraySize = 0;
	_offset = -1;
	_arrayStride = -1;
	_matrixStride = -1;

//	DivideName();
}

AnimaShaderData::AnimaShaderData(const AnimaShaderData& src)
	: AnimaNamedObject(src)
{
	_locations = src._locations;
	_arraySize = src._arraySize;
//	_nameParts = src._nameParts;
	
	_type = src._type;
	_sourceObject = src._sourceObject;
	_slotName = src._slotName;
	_associatedWith = src._associatedWith;

	_offset = src._offset;
	_arrayStride = src._arrayStride;
	_matrixStride = src._matrixStride;
}

AnimaShaderData::AnimaShaderData(AnimaShaderData&& src)
	: AnimaNamedObject(src)
{
	_locations = src._locations;
	_arraySize = src._arraySize;
//	_nameParts = src._nameParts;
	
	_type = src._type;
	_sourceObject = src._sourceObject;
	_slotName = src._slotName;
	_associatedWith = src._associatedWith;

	_offset = src._offset;
	_arrayStride = src._arrayStride;
	_matrixStride = src._matrixStride;
}

AnimaShaderData::~AnimaShaderData()
{
}

AnimaShaderData& AnimaShaderData::operator=(const AnimaShaderData& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		_locations = src._locations;
		_arraySize = src._arraySize;
//		_nameParts = src._nameParts;
		
		_type = src._type;
		_sourceObject = src._sourceObject;
		_slotName = src._slotName;
		_associatedWith = src._associatedWith;

		_offset = src._offset;
		_arrayStride = src._arrayStride;
		_matrixStride = src._matrixStride;
	}

	return *this;
}

AnimaShaderData& AnimaShaderData::operator=(AnimaShaderData&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		_locations = src._locations;
		_arraySize = src._arraySize;
//		_nameParts = src._nameParts;
		
		_type = src._type;
		_sourceObject = src._sourceObject;
		_slotName = src._slotName;
		_associatedWith = src._associatedWith;

		_offset = src._offset;
		_arrayStride = src._arrayStride;
		_matrixStride = src._matrixStride;
	}

	return *this;
}

bool AnimaShaderData::operator==(const AnimaShaderData& left)
{
	if (_name == left._name && _locations == left._locations && _arraySize == left._arraySize && _type == left._type)
		return true;
	return false;
}

bool AnimaShaderData::operator!=(const AnimaShaderData& left)
{
	if (_name != left._name || _locations != left._locations || _arraySize != left._arraySize || _type != left._type)
		return true;
	return false;
}

void AnimaShaderData::SetName(const AnimaString& name)
{
	AnimaNamedObject::SetName(name);

//	DivideName();
}

void AnimaShaderData::Analize(AnimaShaderProgram* program)
{
	if (program == nullptr)
		return;

	if (_arraySize == 0)
	{
		AnimaString aw = AssociatedWith();
		AInt location = glGetUniformLocation(program->GetID(), aw.c_str());

		if (location < 0)
			printf("[AnimaShaderData] Error looking for data location on program:\n\t- Program name: %s\n\t- Data name: %s\n", program->GetName().c_str(), _name.c_str());

		_locations.push_back(location);
	}
	else
	{
		AInt location = -1;
		AnimaString tmpName;
		for (AInt i = 0; i < _arraySize; i++)
		{
			tmpName = FormatString("%s[%d]", AssociatedWith().c_str(), i);

			location = glGetUniformLocation(program->GetID(), tmpName.c_str());

			if (location < 0)
				printf("[AnimaShaderData] Error looking for data location on program:\n\t- Program name: %s\n\t- Data name: %s\n", program->GetName().c_str(), _name.c_str());

			_locations.push_back(location);
		}
	}
}

//void AnimaShaderData::DivideName()
//{
//	AnimaString namePart1 = _name;
//	AnimaString namePart2 = "";
//
//	AInt pos = namePart1.find('_');
//	while (pos != -1)
//	{
//		namePart2 = namePart1.substr(0, pos);
//		namePart1 = namePart1.substr(pos + 1, namePart1.length());
//
//		if (!namePart2.empty())
//			_nameParts.push_back(namePart2);
//		pos = namePart1.find('_');
//	}
//
//	_nameParts.push_back(namePart1);
//}

void AnimaShaderData::UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer)
{
//	AInt count = _nameParts.size();
//	if (count != 2)
//		return;

	AnimaString propertyName = GetName();
	
	switch (_type)
	{
	case Anima::ASDT_FLOAT:
		UpdateValue(object->GetFloat(propertyName));
		break;
	case Anima::ASDT_FLOAT_ARRAY:
		break;
	case Anima::ASDT_FLOAT2:
		UpdateValue(object->GetVector2f(propertyName));
		break;
	case Anima::ASDT_FLOAT2_ARRAY:
		break;
	case Anima::ASDT_FLOAT3:
		if (object->HasColor(propertyName))
			UpdateValue(object->GetColor3f(propertyName));
		else
			UpdateValue(object->GetVector3f(propertyName));
		break;
	case Anima::ASDT_FLOAT3_ARRAY:
		break;
	case Anima::ASDT_FLOAT4:
		if (object->HasColor(propertyName))
			UpdateValue(object->GetColor4f(propertyName));
		else
			UpdateValue(object->GetVector4f(propertyName));
		break;
	case Anima::ASDT_FLOAT4_ARRAY:
		break;
	case Anima::ASDT_MATRIX4x4:
		UpdateValue(object->GetMatrix(propertyName));
		break;
	case Anima::ASDT_MATRIX4x4_ARRAY:
		UpdateValue(object->GetMatrixArray(propertyName));
		break;
	case Anima::ASDT_MATRIX3x3:
		UpdateValue(object->GetMatrix(propertyName));
		break;
	case Anima::ASDT_MATRIX3x3_ARRAY:
		UpdateValue(object->GetMatrixArray(propertyName));
		break;
	case Anima::ASDT_INT:
		UpdateValue(object->GetInteger(propertyName));
		break;
	case Anima::ASDT_INT_ARRAY:
		break;
	case Anima::ASDT_BOOL:
		UpdateValue(object->GetBoolean(propertyName));
		break;
	case Anima::ASDT_BOOL_ARRAY:
		break;
	case Anima::ASDT_TEXTURE2D:
	case Anima::ASDT_TEXTURECUBE:
		if(renderer != nullptr)
			UpdateValue(object->GetTexture(propertyName), renderer->GetTextureSlot(propertyName));
		break;
	case Anima::ASDT_TEXTURE2D_ARRAY:
		break;
	case Anima::ASDT_TEXTURE3D:
		break;
	case Anima::ASDT_TEXTURE3D_ARRAY:
		break;
	default:
		break;
	}
}

void AnimaShaderData::UpdateValue(AnimaRenderer* renderer)
{
	if(renderer == nullptr)
		return;
	
//	AInt count = _nameParts.size();
//	if (count < 2)
//		return;
//
//	AnimaString propertyName = _nameParts[1];
	AnimaString propertyName = GetName();

	switch (_type)
	{
	case Anima::ASDT_FLOAT:
		UpdateValue(renderer->GetFloat(propertyName));
		break;
	case Anima::ASDT_FLOAT_ARRAY:
		break;
	case Anima::ASDT_FLOAT2:
		UpdateValue(renderer->GetVector2f(propertyName));
		break;
	case Anima::ASDT_FLOAT2_ARRAY:
		break;
	case Anima::ASDT_FLOAT3:
		UpdateValue(renderer->GetVector3f(propertyName));
		break;
	case Anima::ASDT_FLOAT3_ARRAY:
		break;
	case Anima::ASDT_FLOAT4:
		UpdateValue(renderer->GetVector4f(propertyName));
		break;
	case Anima::ASDT_FLOAT4_ARRAY:
		break;
	case Anima::ASDT_MATRIX4x4:
		break;
	case Anima::ASDT_MATRIX4x4_ARRAY:
		break;
	case Anima::ASDT_MATRIX3x3:
		break;
	case Anima::ASDT_MATRIX3x3_ARRAY:
		break;
	case Anima::ASDT_INT:
		UpdateValue(renderer->GetInteger(propertyName));
		break;
	case Anima::ASDT_INT_ARRAY:
		break;
	case Anima::ASDT_BOOL:
		UpdateValue(renderer->GetBoolean(propertyName));
		break;
	case Anima::ASDT_BOOL_ARRAY:
		break;
	case Anima::ASDT_TEXTURE2D:
	case Anima::ASDT_TEXTURECUBE:
	{
//		if (_nameParts.size() == 4 && _nameParts[1] == GBUFFER_PREFIX)
//		{
//			AnimaGBuffer* gBuffer = renderer->GetGBuffer(_nameParts[2]);
//
//			if (gBuffer != nullptr)
//				UpdateValue(gBuffer->GetTexture(_nameParts[3]), renderer->GetTextureSlot(_nameParts[3]));
//		}
//		else
//		{
//			UpdateValue(renderer->GetTexture(propertyName), renderer->GetTextureSlot(propertyName));
//		}
		if(_sourceObject == AnimaShaderDataSourceObject::ASDSO_GBUFFER)
		{
			if(_slotName.empty())
			{
				AnimaLogger::LogMessageFormat("ERROR - Error updating shader data. GBuffer slot name not set for GBuffer '%s'", propertyName.c_str());
				return;
			}
			
			AnimaGBuffer* gBuffer = renderer->GetGBuffer(propertyName);
			if(gBuffer != nullptr)
				UpdateValue(gBuffer->GetTexture(_slotName), renderer->GetTextureSlot(_slotName));
			else
				AnimaLogger::LogMessageFormat("ERROR - Error updating shader data. Cannot find GBuffer named '%s'", propertyName.c_str());
		}
		else
		{
			UpdateValue(renderer->GetTexture(propertyName), renderer->GetTextureSlot(propertyName));
		}
		
		break;
	}
	case Anima::ASDT_TEXTURE2D_ARRAY:
		break;
	case Anima::ASDT_TEXTURE3D:
		break;
	case Anima::ASDT_TEXTURE3D_ARRAY:
		break;
	default:
		break;
	}
}

void AnimaShaderData::UpdateValue(const AFloat& value)
{
	glUniform1f(_locations[0], value);
}

void AnimaShaderData::UpdateValue(const AnimaArray<AFloat>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
		glUniform1f(_locations[i], value[i]);
}

void AnimaShaderData::UpdateValue(const AnimaVertex2f& value)
{
	glUniform2f(_locations[0], value.x, value.y);
}

void AnimaShaderData::UpdateValue(const AnimaArray<AnimaVertex2f>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaVertex2f v = value[i];
		glUniform2f(_locations[i], v.x, v.y);
	}
}

void AnimaShaderData::UpdateValue(const AnimaVertex3f& value)
{
	glUniform3f(_locations[0], value.x, value.y, value.z);
}

void AnimaShaderData::UpdateValue(const AnimaArray<AnimaVertex3f>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaVertex3f v = value[i];
		glUniform3f(_locations[i], v.x, v.y, v.z);
	}
}

void AnimaShaderData::UpdateValue(const AnimaVertex4f& value)
{
	glUniform4f(_locations[0], value.x, value.y, value.z, value.w);
}

void AnimaShaderData::UpdateValue(const AnimaArray<AnimaVertex4f>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaVertex4f v = value[i];
		glUniform4f(_locations[i], v.x, v.y, v.z, v.w);
	}
}

void AnimaShaderData::UpdateValue(const AnimaMatrix& value)
{
	glUniformMatrix4fv(_locations[0], 1, GL_FALSE, value.m);
}

void AnimaShaderData::UpdateValue(const AnimaArray<AnimaMatrix>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaMatrix m = value[i];
		glUniformMatrix4fv(_locations[0], 1, GL_FALSE, m.m);
	}
}

void AnimaShaderData::UpdateValue(const AInt& value)
{
	glUniform1i(_locations[0], value);
}

void AnimaShaderData::UpdateValue(const AnimaArray<AInt>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		glUniform1i(_locations[0], value[i]);
	}
}

void AnimaShaderData::UpdateValue(const bool& value)
{
	glUniform1i(_locations[0], value ? 1 : 0);
}

void AnimaShaderData::UpdateValue(const AnimaArray<bool>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		glUniform1i(_locations[0], value[i] ? 1 : 0);
	}
}

void AnimaShaderData::UpdateValue(AnimaTexture* value, const AInt& slot)
{
	UpdateValue(slot);

	if (value == nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		value->Load();
		value->Bind(slot);
	}
}

void AnimaShaderData::UpdateValue(AnimaArray<AnimaTexture*>& value, const AInt& slot)
{
}

END_ANIMA_ENGINE_NAMESPACE