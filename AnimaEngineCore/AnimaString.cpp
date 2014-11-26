//
//  AnimaString.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

AnimaString::AnimaString(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	
	_string = nullptr;
	_stringLength = 0;
}

AnimaString::AnimaString(const char* src, AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	
	_string = nullptr;
	_stringLength = 0;
	
	SetString(src);
}

AnimaString::AnimaString(const AnimaString& src)
{
	_engine = src._engine;
	
	_string = nullptr;
	_stringLength = 0;
	
	SetString(src._string);
}

AnimaString::AnimaString(AnimaString&& src)
: _string(src._string)
, _stringLength(src._stringLength)
, _engine(src._engine)
{
	src._string = nullptr;
	src._stringLength = 0;
}

AnimaString::~AnimaString()
{
	ClearString();
}

AnimaString& AnimaString::operator=(const AnimaString& src)
{
	if(this != &src)
	{
		ClearString();
	
		_engine = src._engine;
		SetString(src._string);
	}
	return *this;
}

AnimaString& AnimaString::operator=(AnimaString&& src)
{
	if(this != &src)
	{
		ClearString();
		
		_engine = src._engine;
		_string = src._string;
		_stringLength = src._stringLength;
		
		src._string = nullptr;
		src._stringLength = 0;
	}
	return *this;
}

AnimaString& AnimaString::operator=(const char* src)
{
	ANIMA_ASSERT(_engine != nullptr);
	
	ClearString();
	SetString(src);
	
	return *this;
}

void AnimaString::SetString(const char* str)
{
	ANIMA_ASSERT(_engine != nullptr);
	
	ClearString();
	
	_stringLength = strlen(str) + 1;
	_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*(_engine->GetStringAllocator()), _stringLength);
	memset(_string, 0, _stringLength);
	
	strcpy(_string, str);
}

void AnimaString::ClearString()
{
	if(_string != nullptr && _stringLength > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray<AChar>(*(_engine->GetStringAllocator()), _string);
		_string = nullptr;
		_stringLength = 0;
	}
}

AChar* AnimaString::GetBuffer()
{
	return _string;
}

void AnimaString::Format(const char* format, ...)
{
	ANIMA_ASSERT(_engine != nullptr);
	ClearString();
	
	va_list args;
	va_start(args, format);
	
	ASizeT length = GetFormatStringLength(format, args);
	
	_stringLength = length + 1;
	_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*(_engine->GetStringAllocator()), _stringLength);
	memset(_string, 0, _stringLength);
	
	vsnprintf(_string, _stringLength, format, args);
	
	va_end(args);
}

ASizeT AnimaString::GetFormatStringLength(const char* format, va_list args)
{
	ASizeT retval;
	va_list argcopy;
	va_copy(argcopy, args);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	
	return retval;
}

ASizeT AnimaString::GetBufferLength()
{
	if(_stringLength == 0)
		return 0;
	
	return _stringLength - 1;
}

AnimaString& AnimaString::operator+=(const AnimaString& src)
{
	ASizeT newStringLength = (this->_stringLength - 1) + (src._stringLength - 1) + 1;
	AChar* newString = AnimaAllocatorNamespace::AllocateArray<AChar>(*(_engine->GetStringAllocator()), newStringLength);
	AChar* tmp = newString;
	
	strcpy(newString, _string);
	
	tmp = tmp + this->_stringLength - 1;
	
	strcpy(tmp, src._string);
	
	SetString(newString);
	
	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetStringAllocator()), newString);
	
	newString = nullptr;
	tmp = nullptr;
	
	return *this;
}

AnimaString& AnimaString::operator+=(const char* src)
{
	ASizeT newStringLength = (this->_stringLength - 1) + strlen(src) + 1;
	AChar* newString = AnimaAllocatorNamespace::AllocateArray<AChar>(*(_engine->GetStringAllocator()), newStringLength);
	AChar* tmp = newString;
	
	strcpy(newString, _string);
	
	tmp = tmp + this->_stringLength - 1;
	
	strcpy(tmp, src);
	
	SetString(newString);
	
	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetStringAllocator()), newString);
	
	newString = nullptr;
	tmp = nullptr;
	
	return *this;
}

END_ANIMA_ENGINE_CORE_NAMESPACE