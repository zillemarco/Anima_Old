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

AnimaMappedValues::AnimaMappedValues(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
}

AnimaMappedValues::AnimaMappedValues(const AnimaMappedValues& src)
{
	_engine = src._engine;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_booleansMap = src._booleansMap;
}

AnimaMappedValues::AnimaMappedValues(AnimaMappedValues&& src)
{
	_engine = src._engine;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_booleansMap = src._booleansMap;
}

AnimaMappedValues::~AnimaMappedValues()
{
	_texturesMap.clear();
	_colorsMap.clear();
	_vectorsMap.clear();
	_floatsMap.clear();
	_booleansMap.clear();
}

AnimaMappedValues& AnimaMappedValues::operator=(const AnimaMappedValues& src)
{
	if (this != &src)
	{
		_engine = src._engine;

		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_vectorsMap = src._vectorsMap;
		_floatsMap = src._floatsMap;
		_booleansMap = src._booleansMap;
	}
	
	return *this;
}

AnimaMappedValues& AnimaMappedValues::operator=(AnimaMappedValues&& src)
{
	if (this != &src)
	{
		_engine = src._engine;

		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_vectorsMap = src._vectorsMap;
		_floatsMap = src._floatsMap;
		_booleansMap = src._booleansMap;
	}
	
	return *this;
}

void AnimaMappedValues::AddTexture(AnimaString propertyName, AnimaTexture* value)
{
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
	AnimaColor4f color(_engine);
	color[0] = value[0];
	color[1] = value[1];
	color[2] = value[2];
	color[3] = 1.0f;

	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _engine);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = 1.0f;

	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = 1.0f;

	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(AnimaString propertyName, AnimaColor4f value)
{
	AnimaColorGenerator* generator = _engine->GetDataGeneratorsManager()->CreateColorGenerator(propertyName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_engine);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _engine->GetDataGeneratorsManager()->CreateColorGenerator(propertyName + suffix);

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
	AnimaColor4f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	AddColor(propertyName, color);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVectorGenerator* value)
{
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
	AnimaVertex4f vector(_engine);
	vector[0] = value[0];
	vector[1] = value[1];
	vector[2] = 0.0f;
	vector[3] = 0.0f;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = 0.0f;
	vector[3] = 0.0f;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = 0.0f;
	vector[3] = 0.0f;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(_engine);
	vector[0] = value[0];
	vector[1] = value[1];
	vector[2] = value[2];
	vector[3] = 1.0f;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = 0.0f;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = 0.0f;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AnimaVertex4f value)
{
	AnimaVectorGenerator* generator = _engine->GetDataGeneratorsManager()->CreateVectorGenerator(propertyName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_engine);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _engine->GetDataGeneratorsManager()->CreateVectorGenerator(propertyName + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetColor(value);

	AddVector(propertyName, generator);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _engine);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;

	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddFloat(AnimaString propertyName, AFloat value)
{
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
	if (_booleansMap.find(propertyName) != _booleansMap.end())
		return;
	_booleansMap[propertyName] = value;
}

void AnimaMappedValues::AddBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _engine);
	AddBoolean(str, value);
}

void AnimaMappedValues::SetTexture(AnimaString propertyName, AnimaTexture* value)
{
	if (_texturesMap.find(propertyName) == _texturesMap.end())
		AddTexture(propertyName, value);
	else
		_texturesMap[propertyName] = value;
}

void AnimaMappedValues::SetTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str(propertyName, _engine);
	SetTexture(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AnimaColorGenerator* value)
{
	if (_colorsMap.find(propertyName) == _colorsMap.end())
		AddColor(propertyName, value);
	else
		_colorsMap[propertyName] = value;
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AnimaColor3f value)
{
	AnimaColor4f color(_engine);
	color[0] = value[0];
	color[1] = value[1];
	color[2] = value[2];
	color[3] = 1.0f;

	AddColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = 1.0f;

	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, r, g, b);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AnimaColor4f value)
{
	AnimaColorGenerator* generator = _engine->GetDataGeneratorsManager()->CreateColorGenerator(propertyName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_engine);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _engine->GetDataGeneratorsManager()->CreateColorGenerator(propertyName + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetColor(value);

	SetColor(propertyName, generator);
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaString str(propertyName, _engine);
	SetColor(str, r, g, b, a);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVectorGenerator* value)
{
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
	AnimaVertex4f vector(_engine);
	vector[0] = value[0];
	vector[1] = value[1];
	vector[2] = value[2];
	vector[3] = 1.0f;

	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = 0.0f;
	vector[3] = 0.0f;

	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, x, y);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(_engine);
	vector[0] = value[0];
	vector[1] = value[1];
	vector[2] = value[2];
	vector[3] = 1.0f;

	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = 0.0f;

	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, x, y, z);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AnimaVertex4f value)
{
	AnimaVectorGenerator* generator = _engine->GetDataGeneratorsManager()->CreateVectorGenerator(propertyName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_engine);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _engine->GetDataGeneratorsManager()->CreateVectorGenerator(propertyName + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetVector(value);

	SetVector(propertyName, generator);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(AnimaString propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(_engine);
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	vector[3] = w;

	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaString str(propertyName, _engine);
	SetVector(str, x, y, z, w);
}

void AnimaMappedValues::SetFloat(AnimaString propertyName, AFloat value)
{
	if (_floatsMap.find(propertyName) == _floatsMap.end())
		AddFloat(propertyName, value);
	else
		_floatsMap[propertyName] = value;
}

void AnimaMappedValues::SetFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _engine);
	SetFloat(str, value);
}

void AnimaMappedValues::SetBoolean(AnimaString propertyName, bool value)
{
	if (_booleansMap.find(propertyName) == _booleansMap.end())
		AddBoolean(propertyName, value);
	else
		_booleansMap[propertyName] = value;
}

void AnimaMappedValues::SetBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _engine);
	SetBoolean(str, value);
}

AnimaTexture* AnimaMappedValues::GetTexture(AnimaString propertyName)
{
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
	if (_colorsMap.find(propertyName) != _colorsMap.end())
		return _colorsMap[propertyName]->GetColor3f();

	AnimaColor3f color(_engine);
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;

	return color;
}

AnimaColor3f AnimaMappedValues::GetColor3f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetColor3f(str);
}

AnimaColor4f AnimaMappedValues::GetColor4f(AnimaString propertyName)
{
	if (_colorsMap.find(propertyName) != _colorsMap.end())
		return _colorsMap[propertyName]->GetColor4f();

	AnimaColor4f color(_engine);
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;

	return color;
}

AnimaColor4f AnimaMappedValues::GetColor4f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetColor4f(str);
}

AnimaVertex2f AnimaMappedValues::GetVector2f(AnimaString propertyName)
{
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return _vectorsMap[propertyName]->GetVector2f();

	AnimaVertex2f vector(_engine);
	vector[0] = 0.0f;
	vector[1] = 0.0f;
	return vector;
}

AnimaVertex2f AnimaMappedValues::GetVector2f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetVector2f(str);
}

AnimaVertex3f AnimaMappedValues::GetVector3f(AnimaString propertyName)
{
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return _vectorsMap[propertyName]->GetVector3f();

	AnimaVertex3f vector(_engine);
	vector[0] = 0.0f;
	vector[1] = 0.0f;
	vector[2] = 0.0f;
	return vector;
}

AnimaVertex3f AnimaMappedValues::GetVector3f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetVector3f(str);
}

AnimaVertex4f AnimaMappedValues::GetVector4f(AnimaString propertyName)
{
	if (_vectorsMap.find(propertyName) != _vectorsMap.end())
		return _vectorsMap[propertyName]->GetVector4f();

	AnimaVertex4f vector(_engine);
	vector[0] = 0.0f;
	vector[1] = 0.0f;
	vector[2] = 0.0f;
	vector[3] = 0.0f;
	return vector;
}

AnimaVertex4f AnimaMappedValues::GetVector4f(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetVector4f(str);
}

AFloat AnimaMappedValues::GetFloat(AnimaString propertyName)
{
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
	if (_booleansMap.find(propertyName) != _booleansMap.end())
		return _booleansMap[propertyName];
	return false;
}

bool AnimaMappedValues::GetBoolean(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetBoolean(str);
}

END_ANIMA_ENGINE_NAMESPACE
