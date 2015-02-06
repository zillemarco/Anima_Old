//
//  AnimaMappedValues.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMappedValues.h"
#include "AnimaDataGeneratorsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMappedValues::AnimaMappedValues(AnimaEngine* engine, const AnimaString& name)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	_name = name;

	if (_name.ReverseFind('.') != _name.GetBufferLength())
		_name += ".";
}

AnimaMappedValues::AnimaMappedValues(const AnimaMappedValues& src)
{
	_engine = src._engine;
	_name = src._name;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;
}

AnimaMappedValues::AnimaMappedValues(AnimaMappedValues&& src)
{
	_engine = src._engine;
	_name = src._name;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;
}

AnimaMappedValues::~AnimaMappedValues()
{
	_texturesMap.clear();
	_colorsMap.clear();
	_vectorsMap.clear();
	_floatsMap.clear();
	_integersMap.clear();
	_booleansMap.clear();
}

AnimaMappedValues& AnimaMappedValues::operator=(const AnimaMappedValues& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_name = src._name;

		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_vectorsMap = src._vectorsMap;
		_floatsMap = src._floatsMap;
		_integersMap = src._integersMap;
		_booleansMap = src._booleansMap;
	}
	
	return *this;
}

AnimaMappedValues& AnimaMappedValues::operator=(AnimaMappedValues&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_name = src._name;

		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_vectorsMap = src._vectorsMap;
		_floatsMap = src._floatsMap;
		_integersMap = src._integersMap;
		_booleansMap = src._booleansMap;
	}
	
	return *this;
}

void AnimaMappedValues::AddTexture(AnimaString propertyName, AnimaTexture* value)
{
	propertyName = _name + propertyName;
	if (_texturesMap.find(propertyName) != _texturesMap.end())
		return;
	_texturesMap[propertyName] = value;
}

void AnimaMappedValues::AddTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str(propertyName, _engine);
	AddTexture(str, value);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AnimaColorGenerator* value)
{
	propertyName = _name + propertyName;
	if (_colorsMap.find(propertyName) != _colorsMap.end())
		return;
	_colorsMap[propertyName] = value;
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str(propertyName, _engine);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AnimaColor3f value)
{
	AnimaColor4f color(value, 1.0f);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _engine);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(r, g, b);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(r, g, b, 1.0f);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AnimaColor4f value)
{
	AnimaString name = _name + propertyName;
	AnimaColorGenerator* generator = _engine->GetDataGeneratorsManager()->CreateColorGenerator(name);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_engine);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _engine->GetDataGeneratorsManager()->CreateColorGenerator(name + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetColor(value);

	AddColor(propertyName, generator);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str(propertyName, _engine);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVectorGenerator* value)
{
	propertyName = _name + propertyName;
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return;
	_vectorsMap[propertyName] = value;
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVectorGenerator* value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVertex2f value)
{
	AnimaVertex4f vector(value, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(value, 1.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVertex4f value)
{
	AnimaString name = _name + propertyName;
	AnimaVectorGenerator* generator = _engine->GetDataGeneratorsManager()->CreateVectorGenerator(name);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_engine);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _engine->GetDataGeneratorsManager()->CreateVectorGenerator(name + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetVector(value);

	AddVector(propertyName, generator);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddFloat(AnimaString propertyName, AFloat value)
{
	propertyName = _name + propertyName;
	if (_floatsMap.find(propertyName) != _floatsMap.end())
		return;
	_floatsMap[propertyName] = value;
}

void AnimaMappedValues::AddFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _engine);
	AddFloat(str, value);
}

void AnimaMappedValues::AddBoolean(AnimaString propertyName, bool value)
{
	propertyName = _name + propertyName;
	if (_booleansMap.find(propertyName) != _booleansMap.end())
		return;
	_booleansMap[propertyName] = value;
}

void AnimaMappedValues::AddBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _engine);
	AddBoolean(str, value);
}

void AnimaMappedValues::AddInteger(AnimaString propertyName, AInt value)
{
	propertyName = _name + propertyName;
	if (_integersMap.find(propertyName) != _integersMap.end())
		return;
	_integersMap[propertyName] = value;
}

void AnimaMappedValues::AddInteger(const char* propertyName, AInt value)
{
	AnimaString str(propertyName, _engine);
	AddInteger(str, value);
}

void AnimaMappedValues::SetTexture(AnimaString propertyName, AnimaTexture* value)
{
	AnimaString name = _name + propertyName;
	if (_texturesMap.find(name) == _texturesMap.end())
		AddTexture(propertyName, value);
	else
		_texturesMap[name] = value;
}

void AnimaMappedValues::SetTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str(propertyName, _engine);
	SetTexture(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AnimaColorGenerator* value)
{
	AnimaString name = _name + propertyName;
	if (_colorsMap.find(name) == _colorsMap.end())
		AddColor(propertyName, value);
	else
		_colorsMap[name] = value;
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AnimaColor3f value)
{
	AnimaColor4f color(value, 1.0f);
	AddColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(r, g, b, 1.0f);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, r, g, b);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AnimaColor4f value)
{
	AnimaString name = _name + propertyName;
	AnimaColorGenerator* generator = (AnimaColorGenerator*)_engine->GetDataGeneratorsManager()->GetGenerator(name);
	if (generator == nullptr)
		AddColor(propertyName, value);
	else
	{
		generator->SetColor(value);
		//SetColor(propertyName, generator);
	}
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, r, g, b, a);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVectorGenerator* value)
{
	propertyName = _name + propertyName;
	if (_vectorsMap.find(propertyName) == _vectorsMap.end())
		AddVector(propertyName, value);
	else
		_vectorsMap[propertyName] = value;
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVectorGenerator* value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVertex2f value)
{
	AnimaVertex4f vector(value, 0.0f, 0.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, x, y);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(value, 1.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, x, y, z);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVertex4f value)
{
	AnimaString name = _name + propertyName;
	AnimaVectorGenerator* generator = (AnimaVectorGenerator*)_engine->GetDataGeneratorsManager()->GetGenerator(name);
	if (generator == nullptr)
		AddVector(propertyName, value);
	else
	{
		generator->SetVector(value);
		//SetVector(propertyName, generator);
	}
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, x, y, z, w);
}

void AnimaMappedValues::SetFloat(AnimaString propertyName, AFloat value)
{
	AnimaString name = _name + propertyName;
	if (_floatsMap.find(name) == _floatsMap.end())
		AddFloat(propertyName, value);
	else
		_floatsMap[name] = value;
}

void AnimaMappedValues::SetFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _engine);
	SetFloat(str, value);
}

void AnimaMappedValues::SetBoolean(AnimaString propertyName, bool value)
{
	AnimaString name = _name + propertyName;
	if (_booleansMap.find(name) == _booleansMap.end())
		AddBoolean(propertyName, value);
	else
		_booleansMap[name] = value;
}

void AnimaMappedValues::SetBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _engine);
	SetBoolean(str, value);
}

void AnimaMappedValues::SetInteger(AnimaString propertyName, AInt value)
{
	AnimaString name = _name + propertyName;
	if (_integersMap.find(name) == _integersMap.end())
		AddInteger(propertyName, value);
	else
		_integersMap[name] = value;
}

void AnimaMappedValues::SetInteger(const char* propertyName, AInt value)
{
	AnimaString str(propertyName, _engine);
	SetInteger(str, value);
}

AnimaTexture* AnimaMappedValues::GetTexture(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_texturesMap.find(propertyName) != _texturesMap.end())
		return _texturesMap[propertyName];
	return nullptr;
}

AnimaTexture* AnimaMappedValues::GetTexture(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetTexture(str);
}

AnimaColor3f AnimaMappedValues::GetColor3f(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_colorsMap.find(propertyName) != _colorsMap.end())
		return _colorsMap[propertyName]->GetColor3f();

	AnimaColor3f color(0.0f, 0.0f, 0.0f);
	return color;
}

AnimaColor3f AnimaMappedValues::GetColor3f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetColor3f(str);
}

AnimaColor4f AnimaMappedValues::GetColor4f(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_colorsMap.find(propertyName) != _colorsMap.end())
		return _colorsMap[propertyName]->GetColor4f();

	AnimaColor4f color(0.0f, 0.0f, 0.0f, 1.0f);
	return color;
}

AnimaColor4f AnimaMappedValues::GetColor4f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetColor4f(str);
}

AnimaVertex2f AnimaMappedValues::GetVector2f(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return _vectorsMap[propertyName]->GetVector2f();

	AnimaVertex2f vector(0.0f, 0.0f);
	return vector;
}

AnimaVertex2f AnimaMappedValues::GetVector2f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetVector2f(str);
}

AnimaVertex3f AnimaMappedValues::GetVector3f(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return _vectorsMap[propertyName]->GetVector3f();

	AnimaVertex3f vector(0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex3f AnimaMappedValues::GetVector3f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetVector3f(str);
}

AnimaVertex4f AnimaMappedValues::GetVector4f(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return _vectorsMap[propertyName]->GetVector4f();

	AnimaVertex4f vector(0.0f, 0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex4f AnimaMappedValues::GetVector4f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetVector4f(str);
}

AFloat AnimaMappedValues::GetFloat(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_floatsMap.find(propertyName) != _floatsMap.end())
		return _floatsMap[propertyName];
	return 0.0;
}

AFloat AnimaMappedValues::GetFloat(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetFloat(str);
}

bool AnimaMappedValues::GetBoolean(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_booleansMap.find(propertyName) != _booleansMap.end())
		return _booleansMap[propertyName];
	return false;
}

bool AnimaMappedValues::GetBoolean(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetBoolean(str);
}

AInt AnimaMappedValues::GetInteger(AnimaString propertyName)
{
	propertyName = _name + propertyName;
	if (_integersMap.find(propertyName) != _integersMap.end())
		return _integersMap[propertyName];
	return 0;
}

AInt AnimaMappedValues::GetInteger(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetInteger(str);
}

END_ANIMA_ENGINE_NAMESPACE
