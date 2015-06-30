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

bool AnimaString::s_randInitialized = false;
char* AnimaString::s_randomCharset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
AInt AnimaString::s_randomCharsetLenght = 62;

AnimaString::AnimaString(AnimaAllocator* allocator)
{
	_allocator = allocator;
	
	_string = nullptr;
	_stringLength = 0;
}

AnimaString::AnimaString(const char* src, AnimaAllocator* allocator)
{
	_allocator = allocator;
	
	_string = nullptr;
	_stringLength = 0;
	
	SetString(src);
}

AnimaString::AnimaString(const AnimaString& src)
{
	_allocator = src._allocator;
	
	_string = nullptr;
	_stringLength = 0;
	
	SetString(src._string);
}

AnimaString::AnimaString(AnimaString&& src)
: _string(src._string)
, _stringLength(src._stringLength)
, _allocator(src._allocator)
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
		if (_allocator == nullptr)
			_allocator = src._allocator;
		SetString(src._string);
	}
	return *this;
}

AnimaString& AnimaString::operator=(AnimaString&& src)
{
	if(this != &src)
	{
		if (_allocator == nullptr)
			_allocator = src._allocator;
		SetString(src._string);
	}
	return *this;
}

AnimaString& AnimaString::operator=(const char* src)
{
	//ANIMA_ASSERT(_allocator != nullptr);
	
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
	//ANIMA_ASSERT(_allocator != nullptr);
	
	ClearString();
	
	if(str != nullptr)
	{
		_stringLength = (AInt)strlen(str) + 1;
		
		if (_allocator != nullptr)
			_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*_allocator, _stringLength);
		else
			_string = new AChar[_stringLength];
		
		memset(_string, 0, _stringLength);	
		strcpy(_string, str);
	}
}

void AnimaString::ClearString()
{
	//if(_allocator != nullptr && _string != nullptr && _stringLength > 0)
	if (_string != nullptr && _stringLength > 0)
	{
		if (_allocator != nullptr)
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _string);
		else
			delete[] _string;

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
	//ANIMA_ASSERT(_allocator != nullptr);
	ClearString();
	
	va_list args;
	va_start(args, format);
	
	AUint length = GetFormatStringLength(format, args);
	
	_stringLength = length + 1;
	
	if (_allocator != nullptr)
		_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*_allocator, _stringLength);
	else
		_string = new AChar[_stringLength];

	memset(_string, 0, _stringLength);
	
	vsnprintf(_string, _stringLength, format, args);
	
	va_end(args);
}

AUint AnimaString::GetFormatStringLength(const char* format, va_list args)
{
	AUint retval;
	va_list argcopy;
	va_copy(argcopy, args);
	retval = vsnprintf(NULL, 0, format, argcopy);
	va_end(argcopy);
	
	return retval;
}

AUint AnimaString::GetBufferLength() const
{
	if(_stringLength == 0)
		return 0;
	
	return _stringLength - 1;
}

AnimaString& AnimaString::operator+=(const AnimaString& src)
{
	//ANIMA_ASSERT(_allocator != nullptr);
	AUint newStringLength = (this->_stringLength - 1) + (src._stringLength - 1) + 1;
	AChar* newString = nullptr;
	
	if (_allocator != nullptr)
		newString = AnimaAllocatorNamespace::AllocateArray<AChar>(*_allocator, newStringLength);
	else
		newString = new AChar[newStringLength];

	AChar* tmp = newString;

	if (_string != nullptr)
		strcpy(newString, _string);
	else
		memset(newString, 0, newStringLength);
	
	if (this->_stringLength > 0)
		tmp = tmp + this->_stringLength - 1;
	
	strcpy(tmp, src._string);
	
	SetString(newString);

	if (_allocator != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, newString);
	else
		delete[] newString;
	
	newString = nullptr;
	tmp = nullptr;
	
	return *this;
}

AnimaString& AnimaString::operator+=(const char* src)
{
	//ANIMA_ASSERT(_allocator != nullptr);
	AUint newStringLength = (this->_stringLength - 1) + (AInt)strlen(src) + 1;
	AChar* newString = nullptr;

	if (_allocator != nullptr)
		newString = AnimaAllocatorNamespace::AllocateArray<AChar>(*_allocator, newStringLength);
	else
		newString = new AChar[newStringLength];

	AChar* tmp = newString;

	if (_string != nullptr)
		strcpy(newString, _string);
	else
		memset(newString, 0, newStringLength);

	if (this->_stringLength > 0)
		tmp = tmp + this->_stringLength - 1;
	
	strcpy(tmp, src);
	
	SetString(newString);

	if (_allocator != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, newString);
	else
		delete[] newString;
	
	newString = nullptr;
	tmp = nullptr;
	
	return *this;
}

AChar& AnimaString::operator[](AUint index)
{
	ANIMA_ASSERT(index >= 0 && index < _stringLength - 1);
	return _string[index];
}

const AChar& AnimaString::operator[](AUint index) const
{
	ANIMA_ASSERT(index >= 0 && index < _stringLength - 1);
	return const_cast<AChar&>(_string[index]);
}

void AnimaString::Reserve(AUint size)
{
	//ANIMA_ASSERT(_allocator != nullptr);
	
	ClearString();
	
	_stringLength = size + 1;

	if (_allocator != nullptr)
		_string = AnimaAllocatorNamespace::AllocateArray<AChar>(*_allocator, _stringLength);
	else
		_string = new AChar[_stringLength];
	
	memset(_string, 0, _stringLength);
}

AInt AnimaString::Find(AChar c, AInt startPos) const
{
	if(startPos <= -1)
		startPos = 0;
	
	for (AUint i = startPos; i < _stringLength; i++)
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
	AUint lunghezzaStr = (AInt)strlen(str);
	
	if(lunghezzaStr == 0)
		return -1;
	
	if(lunghezzaStr == 1)
		return Find(str[0], startPos);
	
	if(startPos <= -1)
		startPos = 0;
	
	AInt offsetStr = 0;
	AInt offsetInteralStr = 0;
	AInt posizioneInizio = startPos;
	
	for (AInt i = startPos; i < (AInt)_stringLength; i++)
	{
		if(_string[i] == str[offsetStr])
		{
			posizioneInizio = i;
			offsetInteralStr = i;
			
			for (; offsetStr < (AInt)lunghezzaStr && offsetInteralStr < (AInt)_stringLength; offsetInteralStr++, offsetStr++)
			{
				if(str[offsetStr] != _string[offsetInteralStr])
					break;
			}
			
			if (offsetStr < (AInt)lunghezzaStr - 1)
				offsetStr = 0;
			else
				return posizioneInizio;
		}
	}
	return -1;
}

AUint AnimaString::CountOf(AChar c) const
{
	AUint res = 0;
	for (AUint i = 0; i < (AInt)_stringLength; i++)
	{
		if (_string[i] == c)
			res++;
	}

	return res;
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
	AUint lunghezzaStr = (AUint)strlen(str);
	
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

AnimaString AnimaString::Substring(AInt startPos, AUint len) const
{
	//ANIMA_ASSERT(_allocator != nullptr);
	if(startPos < 0)
		startPos = 0;
	
	AnimaString resultString(_allocator);
	
	if(len <= 0)
		resultString = "";
	else
	{
		if ((AInt)startPos + (AInt)len > (AInt)_stringLength - 1)
			len = (_stringLength - 1) - startPos;
		
		resultString.Reserve(len);
		
		AUint offsetString = 0;
		
		for (AInt i = startPos; i < (AInt)_stringLength && offsetString < (AInt)len; i++)
			resultString[offsetString++] = _string[i];
	}
	
	return resultString;
}

AnimaString AnimaString::Left(AUint len) const
{
	return Substring(0, len);
}

AnimaString AnimaString::Right(AUint len) const
{
	int startPos = (int)_stringLength - (int)len - 1;
	return Substring(startPos, len);
}

AnimaString AnimaString::TrimLeft() const
{
	if(_stringLength - 1 <= 0)
		return *this;
	
	int startPos = 0;
	for (startPos = 0; startPos < (AInt)_stringLength - 1; startPos++)
	{
		if (_string[startPos] != ' ' && _string[startPos] != '\t'&& _string[startPos] != '\n'&& _string[startPos] != '\0')
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
	//ANIMA_ASSERT(_allocator != nullptr);
	int replacements = 0;
	int pos = Find(find, startPos);
	AUint replacementLength = (AUint)strlen(replacement);
	AUint findLength = 1;
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		AnimaString tmpString(_allocator);
		tmpString.Reserve(_stringLength - 1 + replacementLength);
		
		int offsetString = 0;
		
		for (AInt i = 0; i < pos; i++)
			tmpString[offsetString++] = _string[i];
		
		for (AInt i = 0; i < (AInt)replacementLength; i++)
			tmpString[offsetString++] = replacement[i];
		
		for (AInt i = pos + (AInt)findLength; i < (AInt)_stringLength - 1; i++)
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
	//ANIMA_ASSERT(_allocator != nullptr);
	int replacements = 0;
	int pos = Find(find, startPos);
	AUint replacementLength = 1;
	AUint findLength = (AUint)strlen(find);
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		AnimaString tmpString(_allocator);
		tmpString.Reserve(_stringLength - 1 - findLength + replacementLength);
		
		AInt offsetString = 0;
		
		for (AInt i = 0; i < (AInt)pos; i++)
			tmpString[offsetString++] = _string[i];
		
		for (AInt i = 0; i < (AInt)replacementLength; i++)
			tmpString[offsetString++] = replacement;
		
		for (AInt i = pos + (AInt)findLength; i < (AInt)_stringLength - 1; i++)
			tmpString[offsetString++] = _string[i];
		
		SetString(tmpString._string);
		
		replacements++;
		
		pos = Find(find, startPos);
	}
}

void AnimaString::Replace(const char* find, const char* replacement, AInt startPos, AInt count)
{
	//ANIMA_ASSERT(_allocator != nullptr);
	int replacements = 0;
	int pos = Find(find, startPos);
	AUint replacementLength = (AUint)strlen(replacement);
	AUint findLength = (AUint)strlen(find);
	
	while(pos != -1)
	{
		if(count > 0 && replacements >= count)
			break;
		
		AnimaString tmpString(_allocator);
		tmpString.Reserve(_stringLength - 1 - findLength + replacementLength);
		
		AInt offsetString = 0;
		
		for (AInt i = 0; i < pos; i++)
			tmpString[offsetString++] = _string[i];
		
		for (AInt i = 0; i < (AInt)replacementLength; i++)
			tmpString[offsetString++] = replacement[i];
		
		for (AInt i = pos + (AInt)findLength; i < (AInt)_stringLength - 1; i++)
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

bool AnimaString::StartsWith(AChar c) const
{
	if (GetBufferLength() > 0 && _string[0] == c)
		return true;
	return false;
}

bool AnimaString::StartsWith(const AnimaString& str) const
{
	return StartsWith(str._string);
}

bool AnimaString::StartsWith(const char* str) const
{
	AUint lenThis = (AUint)strlen(this->_string);
	AUint lenStr = (AUint)strlen(str);

	if (lenThis < lenStr)
		return false;
	if (lenThis == lenStr)
		return Compare(str);

	for (AUint i = 0; i < lenStr; i++)
	{
		if (str[i] != _string[i])
			return false;
	}
	return true;
}

bool AnimaString::IsEmpty() const
{
	if (_stringLength == 0)
		return true;

	if (_stringLength == 1 && _string[0] == '\0')
		return true;

	return false;
}

AnimaString AnimaString::MakeRandom(AnimaAllocator* allocator, AInt lenght)
{
	AnimaString str(allocator);
	str.MakeRandom(lenght);
	return str;
}

void AnimaString::MakeRandom(AInt lenght)
{
	ClearString();
	Reserve((AUint)lenght);

	if (!s_randInitialized)
	{
		srand((unsigned int)time(0));
		s_randInitialized = true;
	}
	
	for (AInt i = 0; i < lenght; i++)
		_string[i] = s_randomCharset[rand() % s_randomCharsetLenght];

	_string[lenght] = 0;
}

END_ANIMA_ENGINE_NAMESPACE