#include "AnimaShaderInclude.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShaderInclude::AnimaShaderInclude(const AnimaString& name, AnimaAllocator* allocator)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderInclude);
}

AnimaShaderInclude::AnimaShaderInclude(const AnimaString& name, AnimaAllocator* allocator, AnimaString shaderText)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaShaderInclude);
	_text = shaderText;
}

AnimaShaderInclude::AnimaShaderInclude(const AnimaShaderInclude& src)
	: AnimaNamedObject(src)
	, _text(src._text)
	, _data(src._data)
	, _staticGroupData(src._staticGroupData)
	, _dynamicGroupData(src._dynamicGroupData)
{
}

AnimaShaderInclude::AnimaShaderInclude(AnimaShaderInclude&& src)
	: AnimaNamedObject(src)
	, _text(src._text)
	, _data(src._data)
	, _staticGroupData(src._staticGroupData)
	, _dynamicGroupData(src._dynamicGroupData)
{
}

AnimaShaderInclude::~AnimaShaderInclude()
{
}

AnimaShaderInclude& AnimaShaderInclude::operator=(const AnimaShaderInclude& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_text = src._text;
		_data = src._data;
		_staticGroupData = src._staticGroupData;
		_dynamicGroupData = src._dynamicGroupData;
	}

	return *this;
}

AnimaShaderInclude& AnimaShaderInclude::operator=(AnimaShaderInclude&& src)
{
	if (this != &src)
	{
		AnimaNamedObject::operator=(src);
		_text = src._text;
		_data = src._data;
		_staticGroupData = src._staticGroupData;
		_dynamicGroupData = src._dynamicGroupData;
	}

	return *this;
}

void AnimaShaderInclude::SetText(AnimaString text)
{
	_text = text;
}

AnimaString AnimaShaderInclude::GetText()
{
	return _text;
}

void AnimaShaderInclude::AddShaderData(const AnimaShaderData& data)
{
	_data.Add(data.GetName(), data);
}

AInt AnimaShaderInclude::GetShaderDataCount() const
{
	return _data.GetSize();
}

AnimaShaderData* AnimaShaderInclude::GetShaderData(const AnimaString& name)
{
	return &_data[name];
}

AnimaShaderData* AnimaShaderInclude::GetShaderData(const AInt& index)
{
	return &_data[index];
}

void AnimaShaderInclude::AddShaderStaticGroupData(const AnimaShaderGroupData& groupData)
{
	_staticGroupData.Add(groupData.GetName(), groupData);
}

AInt AnimaShaderInclude::GetShaderStaticGroupDataCount() const
{
	return _staticGroupData.GetSize();
}

AnimaShaderGroupData* AnimaShaderInclude::GetShaderStaticGroupData(const AInt& index)
{
	return &_staticGroupData[index];
}

AnimaShaderGroupData* AnimaShaderInclude::GetShaderStaticGroupData(const AnimaString& name)
{
	return &_staticGroupData[name];
}

void AnimaShaderInclude::AddShaderDynamicGroupData(const AnimaShaderGroupData& groupData)
{
	_dynamicGroupData.Add(groupData.GetName(), groupData);
}

AInt AnimaShaderInclude::GetShaderDynamicGroupDataCount() const
{
	return _dynamicGroupData.GetSize();
}

AnimaShaderGroupData* AnimaShaderInclude::GetShaderDynamicGroupData(const AInt& index)
{
	return &_dynamicGroupData[index];
}

AnimaShaderGroupData* AnimaShaderInclude::GetShaderDynamicGroupData(const AnimaString& name)
{
	return &_dynamicGroupData[name];
}

END_ANIMA_ENGINE_NAMESPACE