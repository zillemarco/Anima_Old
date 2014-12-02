//
//  AnimaString.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaString__
#define __Anima__AnimaString__

#include "AnimaEngineCore.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaString
{
public:
	AnimaString(AnimaEngine* engine);
	AnimaString(const char* src, AnimaEngine* engine);
	AnimaString(const AnimaString& src);
	AnimaString(AnimaString&& src);
	~AnimaString();
	
	AnimaString& operator=(const AnimaString& src);
	AnimaString& operator=(AnimaString&& src);
	AnimaString& operator=(const char* src);
	
	friend AnimaString operator+(const AnimaString& srca, const AnimaString& srcb)
	{
		AnimaString result(srca);
		return result += srcb;
	}
	
	friend AnimaString operator+(const AnimaString& srca, const char* srcb)
	{
		AnimaString result(srca);
		return result += srcb;
	}
	
	friend AnimaString operator+(const char* srca, const AnimaString& srcb)
	{
		AnimaString result(srca, srcb._engine);
		return result += srcb;
	}
	
	AnimaString& operator+=(const AnimaString& src);
	AnimaString& operator+=(const char* src);
	
	AChar& operator[](ASizeT index);
	const AChar& operator[](ASizeT index) const;
	
public:
	void SetString(const char* str);
	void ClearString();
	
	AChar* GetBuffer();
	ASizeT GetBufferLength();
	
	void Format(const char* format, ...);
	
	void Reserve(ASizeT size);
	
	AInt Find(AChar c, AInt startPos = -1);
	AInt Find(AnimaString str, AInt startPos = -1);
	AInt Find(const char* str, AInt startPos = -1);
	
	AnimaString Substring(AInt startPos, ASizeT len);
	AnimaString Left(ASizeT len);
	AnimaString Right(ASizeT len);
	
	AnimaString TrimLeft();
	AnimaString TrimRight();
	AnimaString Trim();
	
	void Replace(AChar find, AChar replacement, AInt startPos = -1, AInt count = -1);
	void Replace(AChar find, const char* replacement, AInt startPos = -1, AInt count = -1);
	void Replace(AChar find, const AnimaString& replacement, AInt startPos = -1, AInt count = -1);
	void Replace(const char* find, AChar replacement, AInt startPos = -1, AInt count = -1);
	void Replace(const char* find, const char* replacement, AInt startPos = -1, AInt count = -1);
	void Replace(const char* find, const AnimaString& replacement, AInt startPos = -1, AInt count = -1);
	void Replace(const AnimaString& find, AChar replacement, AInt startPos = -1, AInt count = -1);
	void Replace(const AnimaString& find, const char* replacement, AInt startPos = -1, AInt count = -1);
	void Replace(const AnimaString& find, const AnimaString& replacement, AInt startPos = -1, AInt count = -1);
	
private:
	ASizeT GetFormatStringLength(const char* format, va_list args);
	
private:
	AChar* _string;
	ASizeT _stringLength;
	
	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaString__) */
