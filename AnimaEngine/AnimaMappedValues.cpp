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
: AnimaNamedObject(name, allocator)
{
	_dataGeneratorManager = dataGeneratorManager;	
	_uniqueName = MakeRandonString(15) + ".";
}

AnimaMappedValues::AnimaMappedValues(const AnimaMappedValues& src)
	: AnimaNamedObject(src)
{
	_uniqueName = src._uniqueName;
	_dataGeneratorManager = src._dataGeneratorManager;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;
	_matricesMap = src._matricesMap;
	_vectorsArraysMap = src._vectorsArraysMap;
	_matricesArraysMap = src._matricesArraysMap;
}

AnimaMappedValues::AnimaMappedValues(AnimaMappedValues&& src)
	: AnimaNamedObject(src)
{
	_uniqueName = src._uniqueName;
	_dataGeneratorManager = src._dataGeneratorManager;

	_texturesMap = src._texturesMap;
	_colorsMap = src._colorsMap;
	_vectorsMap = src._vectorsMap;
	_floatsMap = src._floatsMap;
	_integersMap = src._integersMap;
	_booleansMap = src._booleansMap;
	_matricesMap = src._matricesMap;
	_vectorsArraysMap = src._vectorsArraysMap;
	_matricesArraysMap = src._matricesArraysMap;
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
	ClearAllVectorsArrays();
	ClearAllMatricesArrays();
}

AnimaMappedValues& AnimaMappedValues::operator=(const AnimaMappedValues& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		CopyData(src);

		//_uniqueName = src._uniqueName;
		//_dataGeneratorManager = src._dataGeneratorManager;

		//_texturesMap = src._texturesMap;
		//_colorsMap = src._colorsMap;
		//_vectorsMap = src._vectorsMap;
		//_floatsMap = src._floatsMap;
		//_integersMap = src._integersMap;
		//_booleansMap = src._booleansMap;
		//_matricesMap = src._matricesMap;
		//_vectorsArraysMap = src._vectorsArraysMap;
	}
	
	return *this;
}

AnimaMappedValues& AnimaMappedValues::operator=(AnimaMappedValues&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);

		CopyData(src);

		//_uniqueName = src._uniqueName;
		//_dataGeneratorManager = src._dataGeneratorManager;

		//_texturesMap = src._texturesMap;
		//_colorsMap = src._colorsMap;
		//_vectorsMap = src._vectorsMap;
		//_floatsMap = src._floatsMap;
		//_integersMap = src._integersMap;
		//_booleansMap = src._booleansMap;
		//_matricesMap = src._matricesMap;
		//_vectorsArraysMap = src._vectorsArraysMap;
	}
	
	return *this;
}

void AnimaMappedValues::CopyData(const AnimaMappedValues& src)
{
	if (this != &src)
	{
		CopyTextures(src);
		CopyColors(src);
		CopyVectors(src);
		CopyMatrices(src);
		CopyFloats(src);
		CopyIntegers(src);
		CopyBooleans(src);
		CopyVectorsArrays(src);
	}
}

void AnimaMappedValues::AddTexture(const AnimaString& propertyName, AnimaTexture* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_texturesMap.find(pName) != _texturesMap.end())
		return;
	_texturesMap[pName] = value;
}

void AnimaMappedValues::AddTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str = propertyName;
	AddTexture(str, value);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AnimaColorGenerator* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_colorsMap.find(pName) != _colorsMap.end())
		return;
	_colorsMap[pName] = value;
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str = propertyName;
	AddColor(str, value);
}

void AnimaMappedValues::AddColor(const AnimaString& propertyName, AnimaColor3f value)
{
	AnimaColor4f color(value, 1.0f);
	AddColor(propertyName, color);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str = propertyName;
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
	AnimaString pName = _uniqueName + propertyName;
	AnimaColorGenerator* generator = _dataGeneratorManager->CreateColorGenerator(pName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix = FormatString(".valueGenerator.%d", i);
		while (generator == nullptr)
		{
			generator = _dataGeneratorManager->CreateColorGenerator(pName + suffix);

			i++;
			suffix = FormatString(".valueGenerator.%d", i);
		}
	}

	generator->SetColor(value);

	AddColor(propertyName, generator);
}

void AnimaMappedValues::AddColor(const char* propertyName, AnimaColor4f value)
{
	AnimaString str = propertyName;
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
	AnimaString pName = _uniqueName + propertyName;
	if (_vectorsMap.find(pName) != _vectorsMap.end())
		return;
	_vectorsMap[pName] = value;
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVectorGenerator* value)
{
	AnimaString str = propertyName;
	AddVector(str, value);
}

void AnimaMappedValues::AddVector(const AnimaString& propertyName, AnimaVertex2f value)
{
	AnimaVertex4f vector(value, 0.0f, 0.0f);
	AddVector(propertyName, vector);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str = propertyName;
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
	AnimaString str = propertyName;
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
	AnimaString pName = _uniqueName + propertyName;
	AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(pName);

	if (generator == nullptr)
	{
		int i = 1;

		AnimaString suffix = FormatString(".valueGenerator.%d", i);

		while (generator == nullptr)
		{
			generator = _dataGeneratorManager->CreateVectorGenerator(pName + suffix);

			i++;
			suffix = FormatString(".valueGenerator.%d", i);
		}
	}

	generator->SetVector(value);

	AddVector(propertyName, generator);
}

void AnimaMappedValues::AddVector(const char* propertyName, AnimaVertex4f value)
{
	AnimaString str = propertyName;
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

void AnimaMappedValues::AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVectorGenerator*>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_vectorsArraysMap.find(pName) != _vectorsArraysMap.end())
		return;

	AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);
	newVectorArray->Copy(*value);
	_vectorsArraysMap[pName] = newVectorArray;
}

void AnimaMappedValues::AddVectorArray(const char* propertyName, AnimaArray<AnimaVectorGenerator*>* value)
{
	AnimaString str = propertyName;
	AddVectorArray(str, value);
}

void AnimaMappedValues::AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex2f>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_vectorsArraysMap.find(pName) != _vectorsArraysMap.end())
		return;

	AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);

	AInt valuesCount = value->GetSize();
	for (AInt i = 0; i < valuesCount; i++)
	{
		AnimaString generatorName = FormatString("%s[%d]", pName.c_str(), i);

		AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);
		if (generator == nullptr)
		{
			int kk = 0;
			generatorName = FormatString("%s[%d]_%d", pName.c_str(), i, kk);

			while (generator == nullptr)
			{
				generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);

				kk++;
				generatorName = FormatString("%s[%d]_%d", pName.c_str(), i, kk);
			}
		}

		generator->SetVector(value->GetAt(i));
		newVectorArray->Add(generator);
	}

	_vectorsArraysMap[pName] = newVectorArray;
}

void AnimaMappedValues::AddVectorArray(const char* propertyName, AnimaArray<AnimaVertex2f>* value)
{
	AnimaString str = propertyName;
	AddVectorArray(str, value);
}

void AnimaMappedValues::AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex3f>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_vectorsArraysMap.find(pName) != _vectorsArraysMap.end())
		return;

	AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);

	AInt valuesCount = value->GetSize();
	for (AInt i = 0; i < valuesCount; i++)
	{
		AnimaString generatorName = FormatString("%s[%d]", pName.c_str(), i);

		AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);
		if (generator == nullptr)
		{
			int kk = 0;
			generatorName = FormatString("%s[%d]_%d", pName.c_str(), i, kk);

			while (generator == nullptr)
			{
				generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);

				kk++;
				generatorName = FormatString("%s[%d]_%d", pName.c_str(), i, kk);
			}
		}

		generator->SetVector(value->GetAt(i));
		newVectorArray->Add(generator);
	}

	_vectorsArraysMap[pName] = newVectorArray;
}

void AnimaMappedValues::AddVectorArray(const char* propertyName, AnimaArray<AnimaVertex3f>* value)
{
	AnimaString str = propertyName;
	AddVectorArray(str, value);
}

void AnimaMappedValues::AddVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex4f>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_vectorsArraysMap.find(pName) != _vectorsArraysMap.end())
		return;

	AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);

	AInt valuesCount = value->GetSize();
	for (AInt i = 0; i < valuesCount; i++)
	{
		AnimaString generatorName = FormatString("%s[%d]", pName.c_str(), i);

		AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);
		if (generator == nullptr)
		{
			int kk = 0;
			generatorName = FormatString("%s[%d]_%d", pName.c_str(), i, kk);

			while (generator == nullptr)
			{
				generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);

				kk++;
				generatorName = FormatString("%s[%d]_%d", pName.c_str(), i, kk);
			}
		}

		generator->SetVector(value->GetAt(i));
		newVectorArray->Add(generator);
	}

	_vectorsArraysMap[pName] = newVectorArray;
}

void AnimaMappedValues::AddVectorArray(const char* propertyName, AnimaArray<AnimaVertex4f>* value)
{
	AnimaString str = propertyName;
	AddVectorArray(str, value);
}

void AnimaMappedValues::AddFloat(const AnimaString& propertyName, AFloat value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_floatsMap.find(pName) != _floatsMap.end())
		return;
	_floatsMap[pName] = value;
}

void AnimaMappedValues::AddFloat(const char* propertyName, AFloat value)
{
	AnimaString str = propertyName;
	AddFloat(str, value);
}

void AnimaMappedValues::AddBoolean(const AnimaString& propertyName, bool value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_booleansMap.find(pName) != _booleansMap.end())
		return;
	_booleansMap[pName] = value;
}

void AnimaMappedValues::AddBoolean(const char* propertyName, bool value)
{
	AnimaString str = propertyName;
	AddBoolean(str, value);
}

void AnimaMappedValues::AddInteger(const AnimaString& propertyName, AInt value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_integersMap.find(pName) != _integersMap.end())
		return;
	_integersMap[pName] = value;
}

void AnimaMappedValues::AddInteger(const char* propertyName, AInt value)
{
	AnimaString str = propertyName;
	AddInteger(str, value);
}

void AnimaMappedValues::AddMatrix(const AnimaString& propertyName, const AnimaMatrix& value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_matricesMap.find(pName) != _matricesMap.end())
		return;
	_matricesMap[pName] = value;
}

void AnimaMappedValues::AddMatrix(const char* propertyName, const AnimaMatrix& value)
{
	AnimaString str = propertyName;
	AddMatrix(str, value);
}

void AnimaMappedValues::AddMatrix(const AnimaString& propertyName, AFloat value[16])
{
	AnimaMatrix mat(value);
	AddMatrix(propertyName, mat);
}

void AnimaMappedValues::AddMatrix(const char* propertyName, AFloat value[16])
{
	AnimaString str = propertyName;
	AnimaMatrix mat(value);
	AddMatrix(str, mat);
}

void AnimaMappedValues::AddMatrixArray(const AnimaString& propertyName, AnimaArray<AnimaMatrix>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_matricesArraysMap.find(pName) != _matricesArraysMap.end())
		return;

	AnimaArray<AnimaMatrix>* newMatricesArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaMatrix> >(*_allocator, _allocator);
	newMatricesArray->Copy(*value);
	_matricesArraysMap[pName] = newMatricesArray;
}

void AnimaMappedValues::AddMatrixArray(const char* propertyName, AnimaArray<AnimaMatrix>* value)
{
	AnimaString str = propertyName;
	AddMatrixArray(str, value);
}

void AnimaMappedValues::SetTexture(const AnimaString& propertyName, AnimaTexture* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_texturesMap.find(pName) == _texturesMap.end())
		AddTexture(propertyName, value);
	else
		_texturesMap[pName] = value;
}

void AnimaMappedValues::SetTexture(const char* propertyName, AnimaTexture* value)
{
	AnimaString str = propertyName;
	SetTexture(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AnimaColorGenerator* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_colorsMap.find(pName) == _colorsMap.end())
		AddColor(propertyName, value);
	else
		_colorsMap[pName] = value;
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColorGenerator* value)
{
	AnimaString str = propertyName;
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AnimaColor3f value)
{
	AnimaColor4f color(value, 1.0f);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AnimaColor3f value)
{
	AnimaString str = propertyName;
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaColor4f color(r, g, b, 1.0f);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b)
{
	AnimaString str = propertyName;
	SetColor(str, r, g, b);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AnimaColor4f value)
{
	AnimaString pName = _uniqueName + propertyName;
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
	AnimaString str = propertyName;
	SetColor(str, value);
}

void AnimaMappedValues::SetColor(const AnimaString& propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaColor4f color(r, g, b, a);
	SetColor(propertyName, color);
}

void AnimaMappedValues::SetColor(const char* propertyName, AFloat r, AFloat g, AFloat b, AFloat a)
{
	AnimaString str = propertyName;
	SetColor(str, r, g, b, a);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVectorGenerator* value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_vectorsMap.find(pName) == _vectorsMap.end())
		AddVector(pName, value);
	else
		_vectorsMap[pName] = value;
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVectorGenerator* value)
{
	AnimaString str = propertyName;
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVertex2f value)
{
	AnimaVertex4f vector(value, 0.0f, 0.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex2f value)
{
	AnimaString str = propertyName;
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AFloat x, AFloat y)
{
	AnimaVertex4f vector(x, y, 0.0f, 0.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y)
{
	AnimaString str = propertyName;
	SetVector(str, x, y);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVertex3f value)
{
	AnimaVertex4f vector(value, 1.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AnimaVertex3f value)
{
	AnimaString str = propertyName;
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaVertex4f vector(x, y, z, 1.0f);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z)
{
	AnimaString str = propertyName;
	SetVector(str, x, y, z);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AnimaVertex4f value)
{
	AnimaString pName = _uniqueName + propertyName;
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
	AnimaString str = propertyName;
	SetVector(str, value);
}

void AnimaMappedValues::SetVector(const AnimaString& propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaVertex4f vector(x, y, z, w);
	SetVector(propertyName, vector);
}

void AnimaMappedValues::SetVector(const char* propertyName, AFloat x, AFloat y, AFloat z, AFloat w)
{
	AnimaString str = propertyName;
	SetVector(str, x, y, z, w);
}

void AnimaMappedValues::SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVectorGenerator*>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	auto foundValue = _vectorsArraysMap.find(pName);
	if (foundValue == _vectorsArraysMap.end())
		AddVectorArray(propertyName, value);
	else
	{
		AnimaArray<AnimaVectorGenerator*>* currentArray = foundValue->second;

		AInt valueSize = value->GetSize();
		AInt currentSize = currentArray->GetSize();

		// ciclo sui primi elementi dell'array che ho gi� in memoria per sostituire i valori
		AInt offset = 0;
		for (; offset < valueSize && offset < currentSize; offset++)
			currentArray->SetAt(offset, value->GetAt(offset));

		// se non sono riuscito a copiare tutti i valori del nuovo array vuol dire che quello che avevo in memoria aveva
		// una dimensione inferiore quindi devo aggiungere i nuovi valori
		for (; offset < valueSize; offset++)
			currentArray->Add(value->GetAt(offset));

		// se il valore di offset � minore della lunghezza che aveva l'array che avevo gi� in memoria significa
		// che il nuovo array ha una dimensione inferiore, quindi rimuovo gli elementi di troppo
		if (offset < currentSize - 1)
		{
			for (AInt i = currentSize - 1; i > offset; i--)
			{
				_dataGeneratorManager->RemoveGenerator(currentArray->GetAt(i));
				currentArray->RemoveAt(i);
			}
		}

		ANIMA_ASSERT(currentArray->GetSize() == value->GetSize());

		//ClearVectorsArray(currentArray);
		//AnimaAllocatorNamespace::DeallocateObject(*_allocator, currentArray);

		//AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);
		//newVectorArray->Copy(*value);
		//_vectorsArraysMap[pName] = newVectorArray;
	}
}

void AnimaMappedValues::SetVectorArray(const char* propertyName, AnimaArray<AnimaVectorGenerator*>* value)
{
	AnimaString str = propertyName;
	SetVectorArray(str, value);
}

void AnimaMappedValues::SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex2f>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	auto foundValue = _vectorsArraysMap.find(pName);
	if (foundValue == _vectorsArraysMap.end())
		AddVectorArray(propertyName, value);
	else
	{
		AnimaArray<AnimaVectorGenerator*>* currentArray = foundValue->second;

		AInt valueSize = value->GetSize();
		AInt currentSize = currentArray->GetSize();

		// ciclo sui primi elementi dell'array che ho gi� in memoria per sostituire i valori
		AInt offset = 0;
		for (; offset < valueSize && offset < currentSize; offset++)
			currentArray->ElementAt(offset)->SetVector(value->GetAt(offset));

		// se non sono riuscito a copiare tutti i valori del nuovo array vuol dire che quello che avevo in memoria aveva
		// una dimensione inferiore quindi devo aggiungere i nuovi valori
		for (; offset < valueSize; offset++)
		{
			AnimaString generatorName = FormatString("%s[%d]", pName.c_str(), offset);

			AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);
			if (generator == nullptr)
			{
				int kk = 0;
				generatorName = FormatString("%s[%d]_%d", pName.c_str(), offset, kk);

				while (generator == nullptr)
				{
					generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);

					kk++;
					generatorName = FormatString("%s[%d]_%d", pName.c_str(), offset, kk);
				}
			}

			generator->SetVector(value->GetAt(offset));
			currentArray->Add(generator);
		}

		// se il valore di offset � minore della lunghezza che aveva l'array che avevo gi� in memoria significa
		// che il nuovo array ha una dimensione inferiore, quindi rimuovo gli elementi di troppo
		if (offset < currentSize - 1)
		{
			for (AInt i = currentSize - 1; i > offset; i--)
			{
				_dataGeneratorManager->RemoveGenerator(currentArray->GetAt(i));
				currentArray->RemoveAt(i);
			}
		}

		ANIMA_ASSERT(currentArray->GetSize() == value->GetSize());

		//ClearVectorsArray(foundValue->second);
		//AnimaAllocatorNamespace::DeallocateObject(*_allocator, foundValue->second);

		//AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);

		//AInt valuesCount = value->GetSize();
		//for (AInt i = 0; i < valuesCount; i++)
		//{
		//	AnimaVectorGenerator* vectorGenerator = AnimaAllocatorNamespace::AllocateNew<AnimaVectorGenerator>(*_allocator, _allocator, value->GetAt(i));
		//	newVectorArray->Add(vectorGenerator);
		//}
		//_vectorsArraysMap[pName] = newVectorArray;
	}
}

void AnimaMappedValues::SetVectorArray(const char* propertyName, AnimaArray<AnimaVertex2f>* value)
{
	AnimaString str = propertyName;
	SetVectorArray(str, value);
}

void AnimaMappedValues::SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex3f>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	auto foundValue = _vectorsArraysMap.find(pName);
	if (foundValue == _vectorsArraysMap.end())
		AddVectorArray(propertyName, value);
	else
	{
		AnimaArray<AnimaVectorGenerator*>* currentArray = foundValue->second;

		AInt valueSize = value->GetSize();
		AInt currentSize = currentArray->GetSize();

		// ciclo sui primi elementi dell'array che ho gi� in memoria per sostituire i valori
		AInt offset = 0;
		for (; offset < valueSize && offset < currentSize; offset++)
			currentArray->ElementAt(offset)->SetVector(value->GetAt(offset));

		// se non sono riuscito a copiare tutti i valori del nuovo array vuol dire che quello che avevo in memoria aveva
		// una dimensione inferiore quindi devo aggiungere i nuovi valori
		for (; offset < valueSize; offset++)
		{
			AnimaString generatorName = FormatString("%s[%d]", pName.c_str(), offset);

			AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);
			if (generator == nullptr)
			{
				int kk = 0;
				generatorName = FormatString("%s[%d]_%d", pName.c_str(), offset, kk);

				while (generator == nullptr)
				{
					generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);

					kk++;
					generatorName = FormatString("%s[%d]_%d", pName.c_str(), offset, kk);
				}
			}

			generator->SetVector(value->GetAt(offset));
			currentArray->Add(generator);
		}

		// se il valore di offset � minore della lunghezza che aveva l'array che avevo gi� in memoria significa
		// che il nuovo array ha una dimensione inferiore, quindi rimuovo gli elementi di troppo
		if (offset < currentSize - 1)
		{
			for (AInt i = currentSize - 1; i > offset; i--)
			{
				_dataGeneratorManager->RemoveGenerator(currentArray->GetAt(i));
				currentArray->RemoveAt(i);
			}
		}

		ANIMA_ASSERT(currentArray->GetSize() == value->GetSize());

		//ClearVectorsArray(foundValue->second);
		//AnimaAllocatorNamespace::DeallocateObject(*_allocator, foundValue->second);

		//AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);

		//AInt valuesCount = value->GetSize();
		//for (AInt i = 0; i < valuesCount; i++)
		//{
		//	AnimaVectorGenerator* vectorGenerator = AnimaAllocatorNamespace::AllocateNew<AnimaVectorGenerator>(*_allocator, _allocator, value->GetAt(i));
		//	newVectorArray->Add(vectorGenerator);
		//}
		//_vectorsArraysMap[pName] = newVectorArray;
	}
}

void AnimaMappedValues::SetVectorArray(const char* propertyName, AnimaArray<AnimaVertex3f>* value)
{
	AnimaString str = propertyName;
	SetVectorArray(str, value);
}

void AnimaMappedValues::SetVectorArray(const AnimaString& propertyName, AnimaArray<AnimaVertex4f>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	auto foundValue = _vectorsArraysMap.find(pName);
	if (foundValue == _vectorsArraysMap.end())
		AddVectorArray(propertyName, value);
	else
	{
		AnimaArray<AnimaVectorGenerator*>* currentArray = foundValue->second;

		AInt valueSize = value->GetSize();
		AInt currentSize = currentArray->GetSize();

		// ciclo sui primi elementi dell'array che ho gi� in memoria per sostituire i valori
		AInt offset = 0;
		for (; offset < valueSize && offset < currentSize; offset++)
			currentArray->ElementAt(offset)->SetVector(value->GetAt(offset));

		// se non sono riuscito a copiare tutti i valori del nuovo array vuol dire che quello che avevo in memoria aveva
		// una dimensione inferiore quindi devo aggiungere i nuovi valori
		for (; offset < valueSize; offset++)
		{
			AnimaString generatorName = FormatString("%s[%d]", pName.c_str(), offset);

			AnimaVectorGenerator* generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);
			if (generator == nullptr)
			{
				int kk = 0;
				generatorName = FormatString("%s[%d]_%d", pName.c_str(), offset, kk);

				while (generator == nullptr)
				{
					generator = _dataGeneratorManager->CreateVectorGenerator(generatorName);

					kk++;
					generatorName = FormatString("%s[%d]_%d", pName.c_str(), offset, kk);
				}
			}

			generator->SetVector(value->GetAt(offset));
			currentArray->Add(generator);
		}

		// se il valore di offset � minore della lunghezza che aveva l'array che avevo gi� in memoria significa
		// che il nuovo array ha una dimensione inferiore, quindi rimuovo gli elementi di troppo
		if (offset < currentSize - 1)
		{
			for (AInt i = currentSize - 1; i > offset; i--)
			{
				_dataGeneratorManager->RemoveGenerator(currentArray->GetAt(i));
				currentArray->RemoveAt(i);
			}
		}

		ANIMA_ASSERT(currentArray->GetSize() == value->GetSize());

		//ClearVectorsArray(foundValue->second);
		//AnimaAllocatorNamespace::DeallocateObject(*_allocator, foundValue->second);

		//AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);

		//AInt valuesCount = value->GetSize();
		//for (AInt i = 0; i < valuesCount; i++)
		//{
		//	AnimaVectorGenerator* vectorGenerator = AnimaAllocatorNamespace::AllocateNew<AnimaVectorGenerator>(*_allocator, _allocator, value->GetAt(i));
		//	newVectorArray->Add(vectorGenerator);
		//}
		//_vectorsArraysMap[pName] = newVectorArray;
	}
}

void AnimaMappedValues::SetVectorArray(const char* propertyName, AnimaArray<AnimaVertex4f>* value)
{
	AnimaString str = propertyName;
	SetVectorArray(str, value);
}

void AnimaMappedValues::SetFloat(const AnimaString& propertyName, AFloat value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_floatsMap.find(pName) == _floatsMap.end())
		AddFloat(propertyName, value);
	else
		_floatsMap[pName] = value;
}

void AnimaMappedValues::SetFloat(const char* propertyName, AFloat value)
{
	AnimaString str = propertyName;
	SetFloat(str, value);
}

void AnimaMappedValues::SetBoolean(const AnimaString& propertyName, bool value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_booleansMap.find(pName) == _booleansMap.end())
		AddBoolean(propertyName, value);
	else
		_booleansMap[pName] = value;
}

void AnimaMappedValues::SetBoolean(const char* propertyName, bool value)
{
	AnimaString str = propertyName;
	SetBoolean(str, value);
}

void AnimaMappedValues::SetInteger(const AnimaString& propertyName, AInt value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_integersMap.find(pName) == _integersMap.end())
		AddInteger(propertyName, value);
	else
		_integersMap[pName] = value;
}

void AnimaMappedValues::SetInteger(const char* propertyName, AInt value)
{
	AnimaString str = propertyName;
	SetInteger(str, value);
}

void AnimaMappedValues::SetMatrix(const AnimaString& propertyName, const AnimaMatrix& value)
{
	AnimaString pName = _uniqueName + propertyName;
	if (_matricesMap.find(pName) == _matricesMap.end())
		AddMatrix(propertyName, value);
	else
		_matricesMap[pName] = value;
}

void AnimaMappedValues::SetMatrix(const char* propertyName, const AnimaMatrix& value)
{
	AnimaString str = propertyName;
	SetMatrix(str, value);
}

void AnimaMappedValues::SetMatrix(const AnimaString& propertyName, AFloat value[16])
{
	AnimaMatrix mat(value);
	SetMatrix(propertyName, mat);
}

void AnimaMappedValues::SetMatrix(const char* propertyName, AFloat value[16])
{
	AnimaString str = propertyName;
	AnimaMatrix mat(value);
	SetMatrix(str, mat);
}

void AnimaMappedValues::SetMatrixArray(const AnimaString& propertyName, AnimaArray<AnimaMatrix>* value)
{
	AnimaString pName = _uniqueName + propertyName;
	auto foundValue = _matricesArraysMap.find(pName);
	if (foundValue == _matricesArraysMap.end())
		AddMatrixArray(propertyName, value);
	else
	{
		AnimaArray<AnimaMatrix>* currentArray = foundValue->second;

		AInt valueSize = value->GetSize();
		AInt currentSize = currentArray->GetSize();

		// ciclo sui primi elementi dell'array che ho gi� in memoria per sostituire i valori
		AInt offset = 0;
		for (; offset < valueSize && offset < currentSize; offset++)
			currentArray->SetAt(offset, value->GetAt(offset));

		// se non sono riuscito a copiare tutti i valori del nuovo array vuol dire che quello che avevo in memoria aveva
		// una dimensione inferiore quindi devo aggiungere i nuovi valori
		for (; offset < valueSize; offset++)
			currentArray->Add(value->GetAt(offset));

		// se il valore di offset � minore della lunghezza che aveva l'array che avevo gi� in memoria significa
		// che il nuovo array ha una dimensione inferiore, quindi rimuovo gli elementi di troppo
		if (offset < currentSize - 1)
		{
			for (AInt i = currentSize - 1; i > offset; i--)
				currentArray->RemoveAt(i);
		}

		ANIMA_ASSERT(currentArray->GetSize() == value->GetSize());

		//ClearVectorsArray(currentArray);
		//AnimaAllocatorNamespace::DeallocateObject(*_allocator, currentArray);

		//AnimaArray<AnimaVectorGenerator*>* newVectorArray = AnimaAllocatorNamespace::AllocateNew<AnimaArray<AnimaVectorGenerator*> >(*_allocator, _allocator);
		//newVectorArray->Copy(*value);
		//_vectorsArraysMap[pName] = newVectorArray;
	}
}

void AnimaMappedValues::SetMatrixArray(const char* propertyName, AnimaArray<AnimaMatrix>* value)
{
	AnimaString str = propertyName;
	SetMatrixArray(str, value);
}

AnimaTexture* AnimaMappedValues::GetTexture(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _texturesMap.find(pName);
	if (pair != _texturesMap.end())
		return pair->second;
	return nullptr;
}

AnimaTexture* AnimaMappedValues::GetTexture(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetTexture(str);
}

AnimaColor3f AnimaMappedValues::GetColor3f(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _colorsMap.find(pName);
	if (pair != _colorsMap.end())
		return pair->second->GetColor3f();

	AnimaColor3f color(0.0f, 0.0f, 0.0f);
	return color;
}

AnimaColor3f AnimaMappedValues::GetColor3f(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetColor3f(str);
}

AnimaColor4f AnimaMappedValues::GetColor4f(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _colorsMap.find(pName);
	if (pair != _colorsMap.end())
		return pair->second->GetColor4f();

	AnimaColor4f color(0.0f, 0.0f, 0.0f, 1.0f);
	return color;
}

AnimaColor4f AnimaMappedValues::GetColor4f(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetColor4f(str);
}

AnimaVertex2f AnimaMappedValues::GetVector2f(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _vectorsMap.find(pName);
	if (pair != _vectorsMap.end())
		return pair->second->GetVector2f();

	AnimaVertex2f vector(0.0f, 0.0f);
	return vector;
}

AnimaVertex2f AnimaMappedValues::GetVector2f(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetVector2f(str);
}

AnimaVertex3f AnimaMappedValues::GetVector3f(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _vectorsMap.find(pName);
	if (pair != _vectorsMap.end())
		return pair->second->GetVector3f();

	AnimaVertex3f vector(0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex3f AnimaMappedValues::GetVector3f(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetVector3f(str);
}

AnimaVertex4f AnimaMappedValues::GetVector4f(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _vectorsMap.find(pName);
	if (pair != _vectorsMap.end())
		return pair->second->GetVector4f();

	AnimaVertex4f vector(0.0f, 0.0f, 0.0f, 0.0f);
	return vector;
}

AnimaVertex4f AnimaMappedValues::GetVector4f(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetVector4f(str);
}

AnimaArray<AnimaVectorGenerator*>* AnimaMappedValues::GetVectorArray(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _vectorsArraysMap.find(pName);
	if (pair != _vectorsArraysMap.end())
		return pair->second;

	return nullptr;
}

AnimaArray<AnimaVectorGenerator*>* AnimaMappedValues::GetVectorArray(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetVectorArray(str);
}

AFloat AnimaMappedValues::GetFloat(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _floatsMap.find(pName);
	if (pair != _floatsMap.end())
		return pair->second;
	return 0.0f;
}

AFloat AnimaMappedValues::GetFloat(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetFloat(str);
}

bool AnimaMappedValues::GetBoolean(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _booleansMap.find(pName);
	if (pair != _booleansMap.end())
		return pair->second;
	return false;
}

bool AnimaMappedValues::GetBoolean(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetBoolean(str);
}

AInt AnimaMappedValues::GetInteger(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _integersMap.find(pName);
	if (pair != _integersMap.end())
		return pair->second;
	return 0;
}

AInt AnimaMappedValues::GetInteger(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetInteger(str);
}

AnimaMatrix AnimaMappedValues::GetMatrix(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _matricesMap.find(pName);
	if (pair != _matricesMap.end())
		return pair->second;
	return AnimaMatrix();
}

AnimaMatrix AnimaMappedValues::GetMatrix(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetMatrix(str);
}

AnimaArray<AnimaMatrix>* AnimaMappedValues::GetMatrixArray(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	auto pair = _matricesArraysMap.find(pName);
	if (pair != _matricesArraysMap.end())
		return pair->second;

	return nullptr;
}

AnimaArray<AnimaMatrix>* AnimaMappedValues::GetMatrixArray(const char* propertyName)
{
	AnimaString str = propertyName;
	return GetMatrixArray(str);
}

bool AnimaMappedValues::HasTexture(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_texturesMap.find(pName) != _texturesMap.end());
}

bool AnimaMappedValues::HasTexture(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasTexture(str);
}

bool AnimaMappedValues::HasColor(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_colorsMap.find(pName) != _colorsMap.end());
}

bool AnimaMappedValues::HasColor(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasColor(str);
}

bool AnimaMappedValues::HasVector(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_vectorsMap.find(pName) != _vectorsMap.end());
}

bool AnimaMappedValues::HasVector(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasVector(str);
}

bool AnimaMappedValues::HasVectorArray(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_vectorsArraysMap.find(pName) != _vectorsArraysMap.end());
}

bool AnimaMappedValues::HasVectorArray(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasVectorArray(str);
}

bool AnimaMappedValues::HasFloat(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_floatsMap.find(pName) != _floatsMap.end());
}

bool AnimaMappedValues::HasFloat(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasFloat(str);
}

bool AnimaMappedValues::HasInteger(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_integersMap.find(pName) != _integersMap.end());
}

bool AnimaMappedValues::HasInteger(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasInteger(str);
}

bool AnimaMappedValues::HasBoolean(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_booleansMap.find(pName) != _booleansMap.end());
}

bool AnimaMappedValues::HasBoolean(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasBoolean(str);
}

bool AnimaMappedValues::HasMatrix(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_matricesMap.find(pName) != _matricesMap.end());
}

bool AnimaMappedValues::HasMatrix(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasMatrix(str);
}


bool AnimaMappedValues::HasMatrixArray(const AnimaString& propertyName)
{
	AnimaString pName = _uniqueName + propertyName;
	return (_matricesArraysMap.find(pName) != _matricesArraysMap.end());
}

bool AnimaMappedValues::HasMatrixArray(const char* propertyName)
{
	AnimaString str = propertyName;
	return HasMatrixArray(str);
}

AnimaString AnimaMappedValues::ExtractName(const AnimaString& src) const
{
	AInt pos = src.find(".");
	if (pos == -1)
		return src;
	return src.substr(pos + 1, src.length());
}

void AnimaMappedValues::ClearAllVectorsArrays()
{
	for (auto pair : _vectorsArraysMap)
	{
		pair.second->RemoveAll();
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
	}
	_vectorsArraysMap.clear();
}

void AnimaMappedValues::ClearAllMatricesArrays()
{
	for (auto pair : _matricesArraysMap)
	{
		pair.second->RemoveAll();
		AnimaAllocatorNamespace::DeallocateObject(*_allocator, pair.second);
	}
	_matricesArraysMap.clear();
}

void AnimaMappedValues::CopyTextures(const AnimaMappedValues& src)
{
	_texturesMap.clear();

	const boost::unordered_map<AnimaString, AnimaTexture*>* srcTexturesMap = &src._texturesMap;
	for (auto pair : *srcTexturesMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetTexture(propertyName, pair.second);
	}
}

void AnimaMappedValues::CopyColors(const AnimaMappedValues& src)
{
	_colorsMap.clear();

	const boost::unordered_map<AnimaString, AnimaColorGenerator*>* srcColorsMap = &src._colorsMap;
	for (auto pair : *srcColorsMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetColor(propertyName, pair.second->GetColor4f());
	}
}

void AnimaMappedValues::CopyVectors(const AnimaMappedValues& src)
{
	_vectorsMap.clear();

	const boost::unordered_map<AnimaString, AnimaVectorGenerator*>* srcVectorsMap = &src._vectorsMap;
	for (auto pair : *srcVectorsMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetVector(propertyName, pair.second->GetVector4f());
	}
}

void AnimaMappedValues::CopyVectorsArrays(const AnimaMappedValues& src)
{
	ClearAllVectorsArrays();

	const boost::unordered_map<AnimaString, AnimaArray<AnimaVectorGenerator*>*>* srcVectorsArraysMap = &src._vectorsArraysMap;
	for (auto pair : *srcVectorsArraysMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetVectorArray(propertyName, pair.second);
	}
}

void AnimaMappedValues::CopyMatrices(const AnimaMappedValues& src)
{
	_matricesMap.clear();

	const boost::unordered_map<AnimaString, AnimaMatrix>* srcMatricesMap = &src._matricesMap;
	for (auto pair : *srcMatricesMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetMatrix(propertyName, pair.second);
	}
}

void AnimaMappedValues::CopyMatricesArrays(const AnimaMappedValues& src)
{
	ClearAllMatricesArrays();

	const boost::unordered_map<AnimaString, AnimaArray<AnimaMatrix>*>* srcMatricesArraysMap = &src._matricesArraysMap;
	for (auto pair : *srcMatricesArraysMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetMatrixArray(propertyName, pair.second);
	}
}

void AnimaMappedValues::CopyFloats(const AnimaMappedValues& src)
{
	_floatsMap.clear();

	const boost::unordered_map<AnimaString, AFloat>* srcFloatsMap = &src._floatsMap;
	for (auto pair : *srcFloatsMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetFloat(propertyName, pair.second);
	}
}

void AnimaMappedValues::CopyIntegers(const AnimaMappedValues& src)
{
	_integersMap.clear();

	const boost::unordered_map<AnimaString, AInt>* srcIntegersMap = &src._integersMap;
	for (auto pair : *srcIntegersMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetInteger(propertyName, pair.second);
	}
}

void AnimaMappedValues::CopyBooleans(const AnimaMappedValues& src)
{
	_booleansMap.clear();

	const boost::unordered_map<AnimaString, bool>* srcBooleansMap = &src._booleansMap;
	for (auto pair : *srcBooleansMap)
	{
		AnimaString propertyName = ExtractName(pair.first);
		SetBoolean(propertyName, pair.second);
	}
}

END_ANIMA_ENGINE_NAMESPACE
