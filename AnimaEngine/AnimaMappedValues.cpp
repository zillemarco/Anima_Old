//
//  AnimaMappedValues.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMappedValues.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaShaderProgram.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMappedValues::AnimaMappedValues(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorManager, const AnimaString& name)
{
	ANIMA_ASSERT(allocator != nullptr);
	_allocator = allocator;
	_dataGeneratorManager = dataGeneratorManager;
	_name = name;

	if (_name.ReverseFind('.') != _name.GetBufferLength())
		_name += ".";
}

AnimaMappedValues::AnimaMappedValues(const AnimaMappedValues& src)
{
	_allocator = src._allocator;
	_name = src._name;
	_dataGeneratorManager = src._dataGeneratorManager;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;
	_matricesMap = src._matricesMap;
}

AnimaMappedValues::AnimaMappedValues(AnimaMappedValues&& src)
{
	_allocator = src._allocator;
	_name = src._name;
	_dataGeneratorManager = src._dataGeneratorManager;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;
	_matricesMap = src._matricesMap;
}

AnimaMappedValues::~AnimaMappedValues()
{
	_texturesMap.clear();
	_colorsMap.clear();
	_vectorsMap.clear();
	_floatsMap.clear();
	_integersMap.clear();
	_booleansMap.clear();
	_matricesMap.clear();
}

AnimaMappedValues& AnimaMappedValues::operator=(const AnimaMappedValues& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
		_dataGeneratorManager = src._dataGeneratorManager;

		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_vectorsMap = src._vectorsMap;
		_floatsMap = src._floatsMap;
		_integersMap = src._integersMap;
		_booleansMap = src._booleansMap;
		_matricesMap = src._matricesMap;
	}
	
	return *this;
}

AnimaMappedValues& AnimaMappedValues::operator=(AnimaMappedValues&& src)
{
	if (this != &src)
	{
		_allocator = src._allocator;
		_name = src._name;
		_dataGeneratorManager = src._dataGeneratorManager;

		_texturesMap = src._texturesMap;
		_colorsMap = src._colorsMap;
		_vectorsMap = src._vectorsMap;
		_floatsMap = src._floatsMap;
		_integersMap = src._integersMap;
		_booleansMap = src._booleansMap;
		_matricesMap = src._matricesMap;
	}
	
	return *this;
}

AnimaString AnimaMappedValues::GetAnimaName() const
{
	return _name;
}

void AnimaMappedValues::AddTexture(const AnimaString& propertyName, AnimaTexture* value)
{
	AnimaString pName = _name + propertyName;
	if (_texturesMap.find(pName) != _texturesMap.end())
		return;
	_texturesMap[pName] = value;
}

void AnimaMappedValues::AddTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str(propertyName, _allocator);
	AddTexture(str, value);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AnimaColorGenerator* value)
{
	AnimaString pName = _name + propertyName;
	if (_colorsMap.find(pName) != _colorsMap.end())
		return;
	_colorsMap[pName] = value;
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str(propertyName, _allocator);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AnimaColor3f value)
{
	AnimaColor4f color(value, 1.0f);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _allocator);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor3f color(r, g, b);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(r, g, b, 1.0f);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AnimaColor4f value)
{
	AnimaString pName = _name + propertyName;
	AnimaColorGenerator* generator = _dataGeneratorManager->CreateColorGenerator(pName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_allocator);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _dataGeneratorManager->CreateColorGenerator(pName + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetColor(value);

	AddColor(propertyName, generator);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str(propertyName, _allocator);
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AnimaVectorGenerator* value)
{
	AnimaString pName = _name + propertyName;
	if (_vectorsMap.find(pName) != _vectorsMap.end())
		return;
	_vectorsMap[pName] = value;
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVectorGenerator* value)
{
	AnimaString str(propertyName, _allocator);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AnimaVertex2f value)
{
	AnimaVertex4f vector(value, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _allocator);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(value, 1.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _allocator);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AnimaVertex4f value)
{
	AnimaString pName = _name + propertyName;
	AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(pName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix(_allocator);
		suffix.Format(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _dataGeneratorManager->CreateVectorGenerator(pName + suffix);

			i++;
			suffix.Format(".valueGenerator.%d", i);
		}
	}

	generator->SetVector(value);

	AddVector(propertyName, generator);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str(propertyName, _allocator);
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddFloat(const AnimaString& propertyName, AFloat value)
{
	AnimaString pName = _name + propertyName;
	if (_floatsMap.find(pName) != _floatsMap.end())
		return;
	_floatsMap[pName] = value;
}

void AnimaMappedValues::AddFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _allocator);
	AddFloat(str, value);
}

void AnimaMappedValues::AddBoolean(const AnimaString& propertyName, bool value)
{
	AnimaString pName = _name + propertyName;
	if (_booleansMap.find(pName) != _booleansMap.end())
		return;
	_booleansMap[pName] = value;
}

void AnimaMappedValues::AddBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _allocator);
	AddBoolean(str, value);
}

void AnimaMappedValues::AddInteger(const AnimaString& propertyName, AInt value)
{
	AnimaString pName = _name + propertyName;
	if (_integersMap.find(pName) != _integersMap.end())
		return;
	_integersMap[pName] = value;
}

void AnimaMappedValues::AddInteger(const char* propertyName, AInt value)
{
	AnimaString str(propertyName, _allocator);
	AddInteger(str, value);
}

void AnimaMappedValues::AddMatrix(const AnimaString& propertyName, const AnimaMatrix& value)
{
	AnimaString pName = _name + propertyName;
	if (_matricesMap.find(pName) != _matricesMap.end())
		return;
	_matricesMap[pName] = value;
}

void AnimaMappedValues::AddMatrix(const char* propertyName, const AnimaMatrix& value)
{
	AnimaString str(propertyName, _allocator);
	AddMatrix(str, value);
}

void AnimaMappedValues::AddMatrix(const AnimaString& propertyName, AFloat value[16])
{
	AnimaMatrix mat(value);
	AddMatrix(propertyName, mat);
}

void AnimaMappedValues::AddMatrix(const char* propertyName, AFloat value[16])
{
	AnimaString str(propertyName, _allocator);
	AnimaMatrix mat(value);
	AddMatrix(str, mat);
}

void AnimaMappedValues::SetTexture(const AnimaString& propertyName, AnimaTexture* value)
{
	AnimaString pName = _name + propertyName;
	if (_texturesMap.find(pName) == _texturesMap.end())
		AddTexture(propertyName, value);
	else
		_texturesMap[pName] = value;
}

void AnimaMappedValues::SetTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str(propertyName, _allocator);
	SetTexture(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AnimaColorGenerator* value)
{
	AnimaString pName = _name + propertyName;
	if (_colorsMap.find(pName) == _colorsMap.end())
		AddColor(propertyName, value);
	else
		_colorsMap[pName] = value;
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AnimaColor3f value)
{
	AnimaColor4f color(value, 1.0f);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(r, g, b, 1.0f);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, r, g, b);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AnimaColor4f value)
{
	AnimaString pName = _name + propertyName;
	AnimaColorGenerator* generator = (AnimaColorGenerator*)_dataGeneratorManager->GetGenerator(pName);
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
	AnimaString str(propertyName, _allocator);
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaString str(propertyName, _allocator);
	SetColor(str, r, g, b, a);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVectorGenerator* value)
{
	AnimaString pName = _name + propertyName;
	if (_vectorsMap.find(pName) == _vectorsMap.end())
		AddVector(pName, value);
	else
		_vectorsMap[pName] = value;
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVectorGenerator* value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVertex2f value)
{
	AnimaVertex4f vector(value, 0.0f, 0.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(value, 1.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y, z);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVertex4f value)
{
	AnimaString pName = _name + propertyName;
	AnimaVectorGenerator* generator = (AnimaVectorGenerator*)_dataGeneratorManager->GetGenerator(pName);
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
	AnimaString str(propertyName, _allocator);
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaString str(propertyName, _allocator);
	SetVector(str, x, y, z, w);
}

void AnimaMappedValues::SetFloat(const AnimaString& propertyName, AFloat value)
{
	AnimaString pName = _name + propertyName;
	if (_floatsMap.find(pName) == _floatsMap.end())
		AddFloat(propertyName, value);
	else
		_floatsMap[pName] = value;
}

void AnimaMappedValues::SetFloat(const char* propertyName, AFloat value)
{
	AnimaString str(propertyName, _allocator);
	SetFloat(str, value);
}

void AnimaMappedValues::SetBoolean(const AnimaString& propertyName, bool value)
{
	AnimaString pName = _name + propertyName;
	if (_booleansMap.find(pName) == _booleansMap.end())
		AddBoolean(propertyName, value);
	else
		_booleansMap[pName] = value;
}

void AnimaMappedValues::SetBoolean(const char* propertyName, bool value)
{
	AnimaString str(propertyName, _allocator);
	SetBoolean(str, value);
}

void AnimaMappedValues::SetInteger(const AnimaString& propertyName, AInt value)
{
	AnimaString pName = _name + propertyName;
	if (_integersMap.find(pName) == _integersMap.end())
		AddInteger(propertyName, value);
	else
		_integersMap[pName] = value;
}

void AnimaMappedValues::SetInteger(const char* propertyName, AInt value)
{
	AnimaString str(propertyName, _allocator);
	SetInteger(str, value);
}

void AnimaMappedValues::SetMatrix(const AnimaString& propertyName, const AnimaMatrix& value)
{
	AnimaString pName = _name + propertyName;
	if (_matricesMap.find(pName) == _matricesMap.end())
		AddMatrix(propertyName, value);
	else
		_matricesMap[pName] = value;
}

void AnimaMappedValues::SetMatrix(const char* propertyName, const AnimaMatrix& value)
{
	AnimaString str(propertyName, _allocator);
	SetMatrix(str, value);
}

void AnimaMappedValues::SetMatrix(const AnimaString& propertyName, AFloat value[16])
{
	AnimaMatrix mat(value);
	SetMatrix(propertyName, mat);
}

void AnimaMappedValues::SetMatrix(const char* propertyName, AFloat value[16])
{
	AnimaString str(propertyName, _allocator);
	AnimaMatrix mat(value);
	SetMatrix(str, mat);
}

AnimaTexture* AnimaMappedValues::GetTexture(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_texturesMap.find(pName) != _texturesMap.end())
		return _texturesMap[pName];
	return nullptr;
}

AnimaTexture* AnimaMappedValues::GetTexture(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetTexture(str);
}

AnimaColor3f AnimaMappedValues::GetColor3f(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_colorsMap.find(pName) != _colorsMap.end())
		return _colorsMap[pName]->GetColor3f();

	AnimaColor3f color(0.0f, 0.0f, 0.0f);
	return color;
}

AnimaColor3f AnimaMappedValues::GetColor3f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetColor3f(str);
}

AnimaColor4f AnimaMappedValues::GetColor4f(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_colorsMap.find(pName) != _colorsMap.end())
		return _colorsMap[pName]->GetColor4f();

	AnimaColor4f color(0.0f, 0.0f, 0.0f, 1.0f);
	return color;
}

AnimaColor4f AnimaMappedValues::GetColor4f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetColor4f(str);
}

AnimaVertex2f AnimaMappedValues::GetVector2f(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_vectorsMap.find(pName) != _vectorsMap.end())
		return _vectorsMap[pName]->GetVector2f();

	AnimaVertex2f vector(0.0f, 0.0f);
	return vector;
}

AnimaVertex2f AnimaMappedValues::GetVector2f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector2f(str);
}

AnimaVertex3f AnimaMappedValues::GetVector3f(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_vectorsMap.find(pName) != _vectorsMap.end())
		return _vectorsMap[pName]->GetVector3f();

	AnimaVertex3f vector(0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex3f AnimaMappedValues::GetVector3f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector3f(str);
}

AnimaVertex4f AnimaMappedValues::GetVector4f(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_vectorsMap.find(pName) != _vectorsMap.end())
		return _vectorsMap[pName]->GetVector4f();

	AnimaVertex4f vector(0.0f, 0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex4f AnimaMappedValues::GetVector4f(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetVector4f(str);
}

AFloat AnimaMappedValues::GetFloat(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_floatsMap.find(pName) != _floatsMap.end())
		return _floatsMap[pName];
	return 0.0;
}

AFloat AnimaMappedValues::GetFloat(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetFloat(str);
}

bool AnimaMappedValues::GetBoolean(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_booleansMap.find(pName) != _booleansMap.end())
		return _booleansMap[pName];
	return false;
}

bool AnimaMappedValues::GetBoolean(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetBoolean(str);
}

AInt AnimaMappedValues::GetInteger(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_integersMap.find(pName) != _integersMap.end())
		return _integersMap[pName];
	return 0;
}

AInt AnimaMappedValues::GetInteger(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetInteger(str);
}

AnimaMatrix AnimaMappedValues::GetMatrix(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	if (_matricesMap.find(pName) != _matricesMap.end())
		return _matricesMap[pName];
	return AnimaMatrix();
}

AnimaMatrix AnimaMappedValues::GetMatrix(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return GetMatrix(str);
}

bool AnimaMappedValues::HasTexture(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_texturesMap.find(pName) != _texturesMap.end());
}

bool AnimaMappedValues::HasTexture(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasTexture(str);
}

bool AnimaMappedValues::HasColor(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_colorsMap.find(pName) != _colorsMap.end());
}

bool AnimaMappedValues::HasColor(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasColor(str);
}

bool AnimaMappedValues::HasVector(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_vectorsMap.find(pName) != _vectorsMap.end());
}

bool AnimaMappedValues::HasVector(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasVector(str);
}

bool AnimaMappedValues::HasFloat(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_floatsMap.find(pName) != _floatsMap.end());
}

bool AnimaMappedValues::HasFloat(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasFloat(str);
}

bool AnimaMappedValues::HasInteger(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_integersMap.find(pName) != _integersMap.end());
}

bool AnimaMappedValues::HasInteger(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasInteger(str);
}

bool AnimaMappedValues::HasBoolean(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_booleansMap.find(pName) != _booleansMap.end());
}

bool AnimaMappedValues::HasBoolean(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasBoolean(str);
}

bool AnimaMappedValues::HasMatrix(const AnimaString& propertyName)
{
	AnimaString pName = _name + propertyName;
	return (_matricesMap.find(pName) != _matricesMap.end());
}

bool AnimaMappedValues::HasMatrix(const char* propertyName)
{
	AnimaString str(propertyName, _allocator);
	return HasMatrix(str);
}

END_ANIMA_ENGINE_NAMESPACE
