//
//  AnimaString.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

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

AChar& AnimaString::operator[](ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _stringLength - 1);
	return _string[index];
}

const AChar& AnimaString::operator[](ASizeT index) const
{
	ANIMA_ASSERT(index >= 0 && index < _stringLength - 1);
	return const_cast<AChar&>(_string[index]);
}

void AnimaString::Reserve(ASizeT size)
{
	ANIMA_ASSERT(_engine != nullptr);
	
	ClearString();
	
	_stringLength = size + 1;
	_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*(_engine->GetStringAllocator()), _stringLength);
	memset(_string, 0, _stringLength);
}

AInt AnimaString::Find(AChar c, AInt startPos)
{
	if(startPos <= -1)
		startPos = 0;
	
	for(int i = startPos; i < _stringLength; i++)
		if(_string[i] == c)
			return i;
	return -1;
}

AInt AnimaString::Find(AnimaString str, AInt startPos)
{
	return Find(str._string, startPos);
}

AInt AnimaString::Find(const char* str, AInt startPos)
{
	ASizeT lunghezzaStr = strlen(str);
	
	if(lunghezzaStr == 0)
		return -1;
	
	if(lunghezzaStr == 1)
		return Find(str[0], startPos);
	
	if(startPos <= -1)
		startPos = 0;
	
	int offsetStr = 0;
	int posizioneInizio = startPos;
	
	for(int i = startPos; i < _stringLength; i++)
	{
		if(_string[i] == str[offsetStr])
		{
			posizioneInizio = i;
			
			for(; offsetStr < lunghezzaStr && i < _stringLength; i++, offsetStr++)
			{
				if(str[offsetStr] != _string[i])
					break;
			}
			
			if(offsetStr < lunghezzaStr - 1)
				offsetStr = 0;
			else
				return posizioneInizio;
		}
	}
	return -1;
}

AnimaString AnimaString::Substring(AInt startPos, ASizeT len)
{
	if(startPos < 0)
		startPos = 0;
	
	AnimaString resultString(_engine);
	
	if(len <= 0)
		resultString = "";
	else
	{
		if(startPos + len > _stringLength - 1)
			len = (_stringLength - 1) - startPos;
		
		resultString.Reserve(len);
		
		ASizeT offsetString = 0;
		
		for(int i = startPos; i < _stringLength && offsetString < len; i++)
			resultString[offsetString++] = _string[i];
	}
	
	return resultString;
}

AnimaString AnimaString::Left(ASizeT len)
{
	return Substring(0, len);
}

AnimaString AnimaString::Right(ASizeT len)
{
	int startPos = _stringLength - len - 1;
	return Substring(startPos, len);
}

AnimaString AnimaString::TrimLeft()
{
	if(_stringLength - 1 <= 0)
		return *this;
	
	int startPos = 0;
	for(startPos = 0; startPos < _stringLength - 1; startPos++)
	{
		if(_string[startPos] != ' ')
			break;
	}
	
	return Substring(startPos, _stringLength);
}

AnimaString AnimaString::TrimRight()
{
	if(_stringLength - 1 <= 0)
		return *this;
	
	int length = _stringLength - 2;
	for(; length >= 0; length--)
	{
		if(_string[length] != ' ')
			break;
	}
	
	return Substring(0, length + 1);
}

AnimaString AnimaString::Trim()
{
	AnimaString ltrim = TrimLeft();
	return ltrim.TrimRight();
}

void AnimaString::Replace(AChar find, AChar replacement, AInt startPos, AInt count)
{
	int replacements = 0;
	int pos = Find(find, startPos);
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		_string[pos] = replacement;
		pos = Find(find, startPos);
		
		replacements++;
	}
}

void AnimaString::Replace(AChar find, const char* replacement, AInt startPos, AInt count)
{
	int replacements = 0;
	int pos = Find(find, startPos);
	ASizeT replacementLength = strlen(replacement);
	ASizeT findLength = 1;
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		AnimaString tmpString(_engine);
		tmpString.Reserve(_stringLength - 1 + replacementLength);
		
		int offsetString = 0;
		
		for(int i = 0; i < pos; i++)
			tmpString[offsetString++] = _string[i];
		
		for(int i = 0; i < replacementLength; i++)
			tmpString[offsetString++] = replacement[i];
		
		for(int i = pos + findLength; i < _stringLength - 1; i++)
			tmpString[offsetString++] = _string[i];
		
		SetString(tmpString._string);
		
		replacements++;
			
		pos = Find(find, startPos);
	}
}

void AnimaString::Replace(AChar find, const AnimaString& replacement, AInt startPos, AInt count)
{
	Replace(find, replacement._string, startPos, count);
}

void AnimaString::Replace(const char* find, AChar replacement, AInt startPos, AInt count)
{
	int replacements = 0;
	int pos = Find(find, startPos);
	ASizeT replacementLength = 1;
	ASizeT findLength = strlen(find);
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		AnimaString tmpString(_engine);
		tmpString.Reserve(_stringLength - 1 - findLength + replacementLength);
		
		int offsetString = 0;
		
		for(int i = 0; i < pos; i++)
			tmpString[offsetString++] = _string[i];
		
		for(int i = 0; i < replacementLength; i++)
			tmpString[offsetString++] = replacement;
		
		for(int i = pos + findLength; i < _stringLength - 1; i++)
			tmpString[offsetString++] = _string[i];
		
		SetString(tmpString._string);
		
		replacements++;
		
		pos = Find(find, startPos);
	}
}

void AnimaString::Replace(const char* find, const char* replacement, AInt startPos, AInt count)
{
	int replacements = 0;
	int pos = Find(find, startPos);
	ASizeT replacementLength = strlen(replacement);
	ASizeT findLength = strlen(find);
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		AnimaString tmpString(_engine);
		tmpString.Reserve(_stringLength - 1 - findLength + replacementLength);
		
		int offsetString = 0;
		
		for(int i = 0; i < pos; i++)
			tmpString[offsetString++] = _string[i];
		
		for(int i = 0; i < replacementLength; i++)
			tmpString[offsetString++] = replacement[i];
		
		for(int i = pos + findLength; i < _stringLength - 1; i++)
			tmpString[offsetString++] = _string[i];
		
		SetString(tmpString._string);
		
		replacements++;
		
		pos = Find(find, startPos);
	}
}

void AnimaString::Replace(const char* find, const AnimaString& replacement, AInt startPos, AInt count)
{
	Replace(find, replacement._string, startPos, count);
}

void AnimaString::Replace(const AnimaString& find, AChar replacement, AInt startPos, AInt count)
{
	Replace(find._string, replacement, startPos, count);
}

void AnimaString::Replace(const AnimaString& find, const char* replacement, AInt startPos, AInt count)
{
	Replace(find._string, replacement, startPos, count);
}

void AnimaString::Replace(const AnimaString& find, const AnimaString& replacement, AInt startPos, AInt count)
{
	Replace(find._string, replacement, startPos, count);
}


END_ANIMA_ENGINE_NAMESPACE





