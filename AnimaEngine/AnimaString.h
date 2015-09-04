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
#include "AnimaTypes.h"
#include <boost/algorithm/string.hpp>

BEGIN_ANIMA_ENGINE_NAMESPACE

typedef std::string AnimaString;

ANIMA_ENGINE_EXPORT AnimaString MakeRandonString(AInt length);
ANIMA_ENGINE_EXPORT AnimaString FormatString(const AnimaString& format, ...);
ANIMA_ENGINE_EXPORT AnimaString FormatString(const AnimaString& format, va_list params);

//class AnimaAllocator;
//
//class ANIMA_ENGINE_EXPORT AnimaString
//{
//public:
	struct ANIMA_ENGINE_EXPORT AnimaStringHasher
	{
		std::size_t operator()(const Anima::AnimaString& stringKey) const
		{
			unsigned int seed = 5;
			const void* key = stringKey.c_str();
			size_t len = strlen((const char*)key);
			
			const uint64_t m = 0xc6a4a7935bd1e995;
			const int r = 47;

			uint64_t h = seed ^ len;

			const uint64_t * data = (const uint64_t *)key;
			const uint64_t * end = data + (len / 8);

			while (data != end)
			{
				uint64_t k = *data++;

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;
			}

			const unsigned char * data2 = (const unsigned char*)data;

			switch (len & 7)
			{
			case 7: h ^= uint64_t(data2[6]) << 48;
			case 6: h ^= uint64_t(data2[5]) << 40;
			case 5: h ^= uint64_t(data2[4]) << 32;
			case 4: h ^= uint64_t(data2[3]) << 24;
			case 3: h ^= uint64_t(data2[2]) << 16;
			case 2: h ^= uint64_t(data2[1]) << 8;
			case 1: h ^= uint64_t(data2[0]);
				h *= m;
			};

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}
	};
//
//public:
//	AnimaString(AnimaAllocator* allocator = nullptr);
//	AnimaString(const char* src, AnimaAllocator* allocator);
//	AnimaString(const AnimaString& src);
//	AnimaString(AnimaString&& src);
//	~AnimaString();
//	
//	AnimaString& operator=(const AnimaString& src);
//	AnimaString& operator=(AnimaString&& src);
//	AnimaString& operator=(const char* src);
//	
//	friend AnimaString operator+(const AnimaString& srca, const AnimaString& srcb)
//	{
//		AnimaString result(srca);
//		return result += srcb;
//	}
//	
//	friend AnimaString operator+(const AnimaString& srca, const char* srcb)
//	{
//		AnimaString result(srca);
//		return result += srcb;
//	}
//	
//	friend AnimaString operator+(const char* srca, const AnimaString& srcb)
//	{
//		AnimaString result(srca, srcb._allocator);
//		return result += srcb;
//	}
//	
//	AnimaString& operator+=(const AnimaString& src);
//	AnimaString& operator+=(const char* src);
//	
//	AChar& operator[](AUint index);
//	const AChar& operator[](AUint index) const;
//	
//	bool operator==(const AnimaString& left);
//	bool operator!=(const AnimaString& left);
//	bool operator==(const char* left);
//	bool operator!=(const char* left);
//
//	friend bool operator==(const AnimaString& left, const AnimaString& right) {
//		return left.Compare(right);
//	}
//
//	friend bool operator!=(const AnimaString& left, const AnimaString& right) {
//		return left.Compare(right);
//	}
//
//	friend bool operator==(const char* left, const AnimaString& right) {
//		return right.Compare(left);
//	}
//
//	friend bool operator!=(const char* left, const AnimaString& right) {
//		return right.Compare(left);
//	}
//
//	friend bool operator==(const AnimaString& left, const char* right) {
//		return left.Compare(right);
//	}
//
//	friend bool operator!=(const AnimaString& left, const char* right) {
//		return left.Compare(right);
//	}
//	
//public:
//	void SetString(const char* str);
//	void ClearString();
//	
//	const AChar* GetConstBuffer() const;
//	AChar* GetBuffer() const;
//	AUint GetBufferLength() const;
//	
//	void Format(const char* format, ...);
//	
//	void Reserve(AUint size);
//	
//	AInt Find(AChar c, AInt startPos = -1) const;
//	AInt Find(AnimaString str, AInt startPos = -1) const;
//	AInt Find(const char* str, AInt startPos = -1) const;
//
//	AUint CountOf(AChar c) const;
//	
//	AInt ReverseFind(AChar c, AInt startPos = -1) const;
//	AInt ReverseFind(AnimaString str, AInt startPos = -1) const;
//	AInt ReverseFind(const char* str, AInt startPos = -1) const;
//	
//	AnimaString Substring(AInt startPos, AUint len) const;
//	AnimaString Left(AUint len) const;
//	AnimaString Right(AUint len) const;
//	
//	AnimaString TrimLeft() const;
//	AnimaString TrimRight() const;
//	AnimaString Trim() const;
//	
//	void Replace(AChar find, AChar replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(AChar find, const char* replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(AChar find, const AnimaString& replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(const char* find, AChar replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(const char* find, const char* replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(const char* find, const AnimaString& replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(const AnimaString& find, AChar replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(const AnimaString& find, const char* replacement, AInt startPos = -1, AInt count = -1);
//	void Replace(const AnimaString& find, const AnimaString& replacement, AInt startPos = -1, AInt count = -1);
//	
//	bool Compare(const AnimaString& left) const;
//	bool CompareNoCase(const AnimaString& left) const;
//	bool Compare(const char* left) const;
//	bool CompareNoCase(const char* left) const;
//	
//	bool StartsWith(AChar c) const;
//	bool StartsWith(const AnimaString& str) const;
//	bool StartsWith(const char* str) const;
//
//	bool IsEmpty() const;
//
//	static AnimaString MakeRandom(AnimaAllocator* allocator, AInt lenght);
//	void MakeRandom(AInt lenght);
//
//private:
//	AUint GetFormatStringLength(const char* format, va_list args);
//	
//private:
//	AChar* _string;
//	AUint _stringLength;
//	
//	AnimaAllocator* _allocator;
//
//	static bool s_randInitialized;
//	static char* s_randomCharset;
//	static AInt s_randomCharsetLenght;
//};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaString__) */
