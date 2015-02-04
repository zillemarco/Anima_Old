//
//  AnimaString.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaEngine.h"
#include "AnimaString.h"
#include <boost/algorithm/string/predicate.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaString::AnimaString(AnimaEngine* engine)
{
	_engine = engine;
	
	_string = nullptr;
	_stringLength = 0;
}

AnimaString::AnimaString(const char* src, AnimaEngine* engine)
{
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

bool AnimaString::operator==(const AnimaString& left)
{
	return Compare(left);
}

bool AnimaString::operator!=(const AnimaString& left)
{
	return !Compare(left);
}

bool AnimaString::operator==(const char* left)
{
	return Compare(left);
}

bool AnimaString::operator!=(const char* left)
{
	return !Compare(left);
}

void AnimaString::SetString(const char* str)
{
	ANIMA_ASSERT(_engine != nullptr);
	
	ClearString();
	
	if(str != nullptr)
	{
		_stringLength = strlen(str) + 1;
		_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*(_engine->GetStringAllocator()), _stringLength);
		memset(_string, 0, _stringLength);
	
		strcpy(_string, str);
	}
}

void AnimaString::ClearString()
{
	if(_engine != nullptr && _string != nullptr && _stringLength > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray<AChar>(*(_engine->GetStringAllocator()), _string);
		_string = nullptr;
		_stringLength = 0;
	}
}

const AChar* AnimaString::GetConstBuffer() const
{
	return const_cast<AChar*>(_string);
}

AChar* AnimaString::GetBuffer() const
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

ASizeT AnimaString::GetBufferLength() const
{
	if(_stringLength == 0)
		return 0;
	
	return _stringLength - 1;
}

AnimaString& AnimaString::operator+=(const AnimaString& src)
{
	ANIMA_ASSERT(_engine != nullptr);
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
	ANIMA_ASSERT(_engine != nullptr);
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

AInt AnimaString::Find(AChar c, AInt startPos) const
{
	if(startPos <= -1)
		startPos = 0;
	
	for(int i = startPos; i < _stringLength; i++)
		if(_string[i] == c)
			return i;
	return -1;
}

AInt AnimaString::Find(AnimaString str, AInt startPos) const
{
	return Find(str._string, startPos);
}

AInt AnimaString::Find(const char* str, AInt startPos) const
{
	ASizeT lunghezzaStr = strlen(str);
	
	if(lunghezzaStr == 0)
		return -1;
	
	if(lunghezzaStr == 1)
		return Find(str[0], startPos);
	
	if(startPos <= -1)
		startPos = 0;
	
	int offsetStr = 0;
	int offsetInteralStr = 0;
	int posizioneInizio = startPos;
	
	for(int i = startPos; i < _stringLength; i++)
	{
		if(_string[i] == str[offsetStr])
		{
			posizioneInizio = i;
			offsetInteralStr = i;
			
			for(; offsetStr < lunghezzaStr && offsetInteralStr < _stringLength; offsetInteralStr++, offsetStr++)
			{
				if(str[offsetStr] != _string[offsetInteralStr])
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

AInt AnimaString::ReverseFind(AChar c, AInt startPos) const
{
	if(startPos <= -1)
		startPos = (int)_stringLength - 1;
	
	for(int i = startPos; i >= 0; i--)
		if(_string[i] == c)
			return i;
	return -1;
}

AInt AnimaString::ReverseFind(AnimaString str, AInt startPos) const
{
	return ReverseFind(str.GetConstBuffer(), startPos);
}

AInt AnimaString::ReverseFind(const char* str, AInt startPos) const
{
	ASizeT lunghezzaStr = strlen(str);
	
	if(lunghezzaStr == 0)
		return -1;
	
	if(lunghezzaStr == 1)
		return Find(str[0], startPos);
	
	if(startPos <= -1)
		startPos = (int)_stringLength - 1;
	
	int offsetStr = (int)lunghezzaStr - 1;
	int offsetInteralStr = (int)_stringLength - 1;
	int posizioneInizio = startPos;
	
	for(int i = startPos; i >= 0; i--)
	{
		if(_string[i] == str[offsetStr])
		{
			posizioneInizio = i;
			offsetInteralStr = i;
			
			for(; offsetStr >= 0 && offsetInteralStr >= 0; offsetInteralStr--, offsetStr--)
			{
				if(str[offsetStr] != _string[offsetInteralStr])
					break;
			}
			
			if(offsetStr > 0)
				offsetStr = (int)lunghezzaStr - 1;
			else
				return posizioneInizio;
		}
	}
	return -1;
}

AnimaString AnimaString::Substring(AInt startPos, ASizeT len) const
{
	ANIMA_ASSERT(_engine != nullptr);
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

AnimaString AnimaString::Left(ASizeT len) const
{
	return Substring(0, len);
}

AnimaString AnimaString::Right(ASizeT len) const
{
	int startPos = (int)_stringLength - (int)len - 1;
	return Substring(startPos, len);
}

AnimaString AnimaString::TrimLeft() const
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

AnimaString AnimaString::TrimRight() const
{
	if(_stringLength - 1 <= 0)
		return *this;
	
	int length = (int)_stringLength - 2;
	for(; length >= 0; length--)
	{
		if(_string[length] != ' ')
			break;
	}
	
	return Substring(0, length + 1);
}

AnimaString AnimaString::Trim() const
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
	ANIMA_ASSERT(_engine != nullptr);
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
		
		for (int i = pos + (int)findLength; i < (int)_stringLength - 1; i++)
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
	ANIMA_ASSERT(_engine != nullptr);
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
		
		for (int i = pos + (int)findLength; i < (int)_stringLength - 1; i++)
			tmpString[offsetString++] = _string[i];
		
		SetString(tmpString._string);
		
		replacements++;
		
		pos = Find(find, startPos);
	}
}

void AnimaString::Replace(const char* find, const char* replacement, AInt startPos, AInt count)
{
	ANIMA_ASSERT(_engine != nullptr);
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
		
		for (int i = pos + (int)findLength; i < (int)_stringLength - 1; i++)
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

bool AnimaString::Compare(const AnimaString& left) const
{
	return Compare(left._string);
}

bool AnimaString::CompareNoCase(const AnimaString& left) const
{
	return CompareNoCase(left._string);
}

bool AnimaString::Compare(const char* left) const
{
	return boost::equals(this->_string, left);
}

bool AnimaString::CompareNoCase(const char* left) const
{
	return boost::iequals(this->_string, left);
}

END_ANIMA_ENGINE_NAMESPACE






