#include "AnimaShaderInput.h"
#include "AnimaShaderProgram.h"
#include "AnimaRenderer.h"
#include "AnimaMappedValues.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShaderInput::AnimaShaderInput(const AnimaString& name)
	: AnimaNamedObject(name, nullptr)
{
	_arraySize = 0;
	_type = NONE;

	DivideName();
}

AnimaShaderInput::AnimaShaderInput(const AnimaShaderInput& src)
	: AnimaNamedObject(src)
{
	_locations = src._locations;
	_arraySize = src._arraySize;
	_nameParts = src._nameParts;
	_type = src._type;
}

AnimaShaderInput::AnimaShaderInput(AnimaShaderInput&& src)
	: AnimaNamedObject(src)
{
	_locations = src._locations;
	_arraySize = src._arraySize;
	_nameParts = src._nameParts;
	_type = src._type;
}

AnimaShaderInput::~AnimaShaderInput()
{
}

AnimaShaderInput& AnimaShaderInput::operator=(const AnimaShaderInput& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		_locations = src._locations;
		_arraySize = src._arraySize;
		_nameParts = src._nameParts;
		_type = src._type;
	}

	return *this;
}

AnimaShaderInput& AnimaShaderInput::operator=(AnimaShaderInput&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		_locations = src._locations;
		_arraySize = src._arraySize;
		_nameParts = src._nameParts;
		_type = src._type;
	}

	return *this;
}

bool AnimaShaderInput::operator==(const AnimaShaderInput& left)
{
	if (_name == left._name && _locations == left._locations && _arraySize == left._arraySize && _type == left._type)
		return true;
	return false;
}

bool AnimaShaderInput::operator!=(const AnimaShaderInput& left)
{
	if (_name != left._name || _locations != left._locations || _arraySize != left._arraySize || _type != left._type)
		return true;
	return false;
}

void AnimaShaderInput::FindLocation(AnimaShaderProgram* program)
{
	if (program == nullptr)
		return;

	if (_arraySize == 0)
	{
		AInt location = glGetUniformLocation(program->GetID(), _name.c_str());
		_locations.push_back(location);
	}
	else
	{
		AInt location = -1;
		AnimaString tmpName;
		for (AInt i = 0; i < _arraySize; i++)
		{
			tmpName = FormatString("%s[%d]", _name, i);

			location = glGetUniformLocation(program->GetID(), _name.c_str());
			_locations.push_back(location);
		}
	}
}

void AnimaShaderInput::DivideName()
{
	AnimaString namePart1 = _name;
	AnimaString namePart2 = "";
	AUint offset = std::count(namePart1.begin(), namePart1.end(), ('_')) + 1;

	AInt pos = namePart1.find('_');
	while (pos != -1)
	{
		namePart2 = namePart1.substr(0, pos);
		namePart1 = namePart1.substr(pos + 1, namePart1.length());

		if (!namePart2.empty())
			_nameParts.push_back(namePart2);
		pos = namePart1.find('_');
	}

	_nameParts.push_back(namePart1);
}

void AnimaShaderInput::UpdateValue(const AnimaMappedValues* object, AnimaRenderer* renderer)
{
	AInt count = _nameParts.size();
	if (count != 2)
		return;

	AnimaString propertyName = _nameParts[1];

	switch (_type)
	{
	case Anima::FLOAT:
		UpdateValue(object->GetFloat(propertyName));
		break;
	case Anima::FLOAT_ARRAY:
		break;
	case Anima::FLOAT2:
		UpdateValue(object->GetVector2f(propertyName));
		break;
	case Anima::FLOAT2_ARRAY:
		break;
	case Anima::FLOAT3:
		if (object->HasColor(propertyName))
			UpdateValue(object->GetColor3f(propertyName));
		else
			UpdateValue(object->GetVector3f(propertyName));
		break;
	case Anima::FLOAT3_ARRAY:
		break;
	case Anima::FLOAT4:
		if (object->HasColor(propertyName))
			UpdateValue(object->GetColor4f(propertyName));
		else
			UpdateValue(object->GetVector4f(propertyName));
		break;
	case Anima::FLOAT4_ARRAY:
		break;
	case Anima::MATRIX4x4:
		UpdateValue(object->GetMatrix(propertyName));
		break;
	case Anima::MATRIX4x4_ARRAY:
		UpdateValue(object->GetMatrixArray(propertyName));
		break;
	case Anima::MATRIX3x3:
		UpdateValue(object->GetMatrix(propertyName));
		break;
	case Anima::MATRIX3x3_ARRAY:
		UpdateValue(object->GetMatrixArray(propertyName));
		break;
	case Anima::INT:
		UpdateValue(object->GetInteger(propertyName));
		break;
	case Anima::INT_ARRAY:
		break;
	case Anima::BOOL:
		UpdateValue(object->GetBoolean(propertyName));
		break;
	case Anima::BOOL_ARRAY:
		break;
	case Anima::TEXTURE2D:
		UpdateValue(object->GetTexture(propertyName), renderer->GetTextureSlot(propertyName));
		break;
	case Anima::TEXTURE2D_ARRAY:
		break;
	case Anima::TEXTURE3D:
		break;
	case Anima::TEXTURE3D_ARRAY:
		break;
	default:
		break;
	}
}

void AnimaShaderInput::UpdateValue(const AFloat& value)
{
	glUniform1f(_locations[0], value);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<AFloat>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
		glUniform1f(_locations[i], value[i]);
}

void AnimaShaderInput::UpdateValue(const AnimaVertex2f& value)
{
	glUniform2f(_locations[0], value.x, value.y);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<AnimaVertex2f>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaVertex2f v = value[i];
		glUniform2f(_locations[i], v.x, v.y);
	}
}

void AnimaShaderInput::UpdateValue(const AnimaVertex3f& value)
{
	glUniform3f(_locations[0], value.x, value.y, value.z);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<AnimaVertex3f>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaVertex3f v = value[i];
		glUniform3f(_locations[i], v.x, v.y, v.z);
	}
}

void AnimaShaderInput::UpdateValue(const AnimaVertex4f& value)
{
	glUniform4f(_locations[0], value.x, value.y, value.z, value.w);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<AnimaVertex4f>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaVertex4f v = value[i];
		glUniform4f(_locations[i], v.x, v.y, v.z, v.w);
	}
}

void AnimaShaderInput::UpdateValue(const AnimaMatrix& value)
{
	glUniformMatrix4fv(_locations[0], 1, GL_FALSE, value.m);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<AnimaMatrix>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		AnimaMatrix m = value[i];
		glUniformMatrix4fv(_locations[0], 1, GL_FALSE, m.m);
	}
}

void AnimaShaderInput::UpdateValue(const AInt& value)
{
	glUniform1i(_locations[0], value);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<AInt>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		glUniform1i(_locations[0], value[i]);
	}
}

void AnimaShaderInput::UpdateValue(const bool& value)
{
	glUniform1i(_locations[0], value ? 1 : 0);
}

void AnimaShaderInput::UpdateValue(const AnimaArray<bool>& value)
{
	AInt locationsCount = _locations.size();
	AInt valuesCount = value.size();

	for (AInt i = 0; i < locationsCount && i < valuesCount; i++)
	{
		glUniform1i(_locations[0], value[i] ? 1 : 0);
	}
}

void AnimaShaderInput::UpdateValue(AnimaTexture* value, const AInt& slot)
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

void AnimaShaderInput::UpdateValue(AnimaArray<AnimaTexture*>& value, const AInt& slot)
{
}

END_ANIMA_ENGINE_NAMESPACE