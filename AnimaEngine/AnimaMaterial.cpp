//
//  AnimaMaterial.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMaterial.h"
#include "AnimaDataGeneratorsManager.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMaterial::AnimaMaterial(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
}

AnimaMaterial::AnimaMaterial(const AnimaMaterial& src)
{
	_engine = src._engine;
	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_floatsMap = src._floatsMap;
}

AnimaMaterial::AnimaMaterial(AnimaMaterial&& src)
{
	_engine = src._engine;
	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_floatsMap = src._floatsMap;
}

AnimaMaterial::~AnimaMaterial()
{
	_texturesMap.clear();
	_colorsMap.clear();
	_floatsMap.clear();
}

AnimaMaterial& AnimaMaterial::operator=(const AnimaMaterial& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_floatsMap = src._floatsMap;
	}

	return *this;
}

AnimaMaterial& AnimaMaterial::operator=(AnimaMaterial&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_floatsMap = src._floatsMap;
	}

	return *this;
}

void AnimaMaterial::AddTexture(AnimaString propertyName, AnimaTexture* value)
{
	_texturesMap[propertyName] = value;
}

void AnimaMaterial::AddTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str(propertyName, _engine);
	AddTexture(str, value);
}

void AnimaMaterial::AddColor(AnimaString propertyName, AnimaColorGenerator* value)
{
	_colorsMap[propertyName] = value;
}

void AnimaMaterial::AddColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str(propertyName, _engine);
	AddColor(str, value);
}

void AnimaMaterial::AddColor(AnimaString propertyName, AnimaColor3f value)
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

	generator->SetColor3f(value);

	AddColor(propertyName, generator);
}

void AnimaMaterial::AddColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _engine);
	AddColor(str, value);
}

void AnimaMaterial::AddColor(AnimaString propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;

	AddColor(propertyName, color);
}

void AnimaMaterial::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(_engine);
	color[0] = r;
	color[1] = g;
	color[2] = b;

	AddColor(propertyName, color);
}

void AnimaMaterial::AddFloat(AnimaString propertyName, AFloat value)
{
	_floatsMap[propertyName] = value;
}

void AnimaMaterial::AddFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _engine);
	AddFloat(str, value);
}

AnimaTexture* AnimaMaterial::GetTexture(AnimaString propertyName)
{
	if (_texturesMap.find(propertyName) != _texturesMap.end())
		return _texturesMap[propertyName];
	return nullptr;
}

AnimaTexture* AnimaMaterial::GetTexture(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetTexture(str);
}

AnimaColor3f AnimaMaterial::GetColor(AnimaString propertyName)
{
	if (_colorsMap.find(propertyName) != _colorsMap.end())
		return _colorsMap[propertyName]->GetColor3f();

	AnimaColor3f color(_engine);
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;

	return color;
}

AnimaColor3f AnimaMaterial::GetColor(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetColor(str);
}

AFloat AnimaMaterial::GetFloat(AnimaString propertyName)
{
	if (_floatsMap.find(propertyName) != _floatsMap.end())
		return _floatsMap[propertyName];
	return 0.0;
}

AFloat AnimaMaterial::GetFloat(const char* propertyName)
{
	AnimaString str(propertyName, _engine);
	return GetFloat(str);
}

END_ANIMA_ENGINE_NAMESPACE
