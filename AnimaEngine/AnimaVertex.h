//
//  AnimaVertex.h
//  Anima
//
//  Created by Marco Zille on 19/11/14.
//
//

#ifndef __Anima__AnimaVertex__
#define __Anima__AnimaVertex__

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaVertex2f;
class AnimaVertex3f;
class AnimaVertex4f;

typedef AnimaVertex3f AnimaColor3f;
typedef AnimaVertex4f AnimaColor4f;

class ANIMA_ENGINE_EXPORT AnimaVertex2f
{
public:
	union
	{
		AFloat vec[2];
		struct
		{
			AFloat x, y;
		};
		struct
		{
			AFloat u, v;
		};
	};

public:
	AnimaVertex2f();
	AnimaVertex2f(AFloat val);
	AnimaVertex2f(AFloat v[2]);
	AnimaVertex2f(AFloat x, AFloat y);
	AnimaVertex2f(const AnimaVertex2f& src);
	AnimaVertex2f(AnimaVertex2f&& src);
	AnimaVertex2f(const AnimaVertex3f& src);
	AnimaVertex2f(const AnimaVertex4f& src);
	~AnimaVertex2f();

	AnimaVertex2f& operator=(const AnimaVertex2f& src);
	AnimaVertex2f& operator=(AnimaVertex2f&& src);

	bool operator==(const AnimaVertex2f& param) const;
	bool operator!=(const AnimaVertex2f& param) const;
	bool IsNull() const;

	AnimaVertex2f operator-() const;

	void operator+=(const AFloat val);
	void operator+=(const AnimaVertex2f& val);

	void operator-=(const AFloat val);
	void operator-=(const AnimaVertex2f& val);

	void operator*=(const AFloat val);
	void operator*=(const AnimaVertex2f& val);

	void operator/=(const AFloat val);
	void operator/=(const AnimaVertex2f& val);

	void Normalize();
	AnimaVertex2f Normalized() const;

	void Clamp(const AFloat lower, const AFloat upper);
	void Clamp(const AnimaVertex2f& lower, const AnimaVertex2f& upper);
	AnimaVertex2f Clamped(const AFloat lower, const AFloat upper) const;
	AnimaVertex2f Clamped(const AnimaVertex2f& lower, const AnimaVertex2f& upper) const;

	AFloat Length() const;
	AFloat Length2() const;
};

class ANIMA_ENGINE_EXPORT AnimaVertex3f
{
public:
	union
	{
		AFloat vec[3];
		struct
		{
			AFloat x, y, z;
		};
		struct
		{
			AFloat r, g, b;
		};
	};

public:
	AnimaVertex3f();
	AnimaVertex3f(AFloat val);
	AnimaVertex3f(AFloat v[3]);
	AnimaVertex3f(AFloat x, AFloat y, AFloat z);
	AnimaVertex3f(const AnimaVertex2f& src, const AFloat z);
	AnimaVertex3f(const AnimaVertex3f& src);
	AnimaVertex3f(AnimaVertex3f&& src);
	AnimaVertex3f(const AnimaVertex4f& src);
	~AnimaVertex3f();

	AnimaVertex3f& operator=(const AnimaVertex3f& src);
	AnimaVertex3f& operator=(AnimaVertex3f&& src);

	bool operator==(const AnimaVertex3f& param) const;
	bool operator!=(const AnimaVertex3f& param) const;
	bool IsNull() const;

	AnimaVertex3f operator-() const;

	void operator+=(const AFloat val);
	void operator+=(const AnimaVertex3f& val);

	void operator-=(const AFloat val);
	void operator-=(const AnimaVertex3f& val);

	void operator*=(const AFloat val);
	void operator*=(const AnimaVertex3f& val);

	void operator/=(const AFloat val);
	void operator/=(const AnimaVertex3f& val);

	void Normalize();
	AnimaVertex3f Normalized() const;

	void Clamp(const AFloat lower, const AFloat upper);
	void Clamp(const AnimaVertex3f& lower, const AnimaVertex3f& upper);
	AnimaVertex3f Clamped(const AFloat lower, const AFloat upper) const;
	AnimaVertex3f Clamped(const AnimaVertex3f& lower, const AnimaVertex3f& upper) const;

	AFloat Length() const;
	AFloat Length2() const;
};

class ANIMA_ENGINE_EXPORT AnimaVertex4f
{
public:
	union
	{
		AFloat vec[4];
		struct
		{
			AFloat x, y, z, w;
		};
		struct
		{
			AFloat r, g, b, a;
		};
	};

public:
	AnimaVertex4f();
	AnimaVertex4f(AFloat val);
	AnimaVertex4f(AFloat v[3]);
	AnimaVertex4f(AFloat x, AFloat y, AFloat z, const AFloat w);
	AnimaVertex4f(const AnimaVertex2f& src, const AFloat z, const AFloat w);
	AnimaVertex4f(const AnimaVertex3f& src, const AFloat w);
	AnimaVertex4f(const AnimaVertex4f& src);
	AnimaVertex4f(AnimaVertex4f&& src);
	~AnimaVertex4f();

	AnimaVertex4f& operator=(const AnimaVertex4f& src);
	AnimaVertex4f& operator=(AnimaVertex4f&& src);

	bool operator==(const AnimaVertex4f& param) const;
	bool operator!=(const AnimaVertex4f& param) const;
	bool IsNull() const;

	AnimaVertex4f operator-() const;

	void operator+=(const AFloat val);
	void operator+=(const AnimaVertex4f& val);

	void operator-=(const AFloat val);
	void operator-=(const AnimaVertex4f& val);

	void operator*=(const AFloat val);
	void operator*=(const AnimaVertex4f& val);

	void operator/=(const AFloat val);
	void operator/=(const AnimaVertex4f& val);

	void Normalize();
	AnimaVertex4f Normalized() const;

	void Clamp(const AFloat lower, const AFloat upper);
	void Clamp(const AnimaVertex4f& lower, const AnimaVertex4f& upper);
	AnimaVertex4f Clamped(const AFloat lower, const AFloat upper) const;
	AnimaVertex4f Clamped(const AnimaVertex4f& lower, const AnimaVertex4f& upper) const;

	AFloat Length() const;
	AFloat Length2() const;
};

AFloat ANIMA_ENGINE_EXPORT operator * (const AnimaVertex2f &a, const AnimaVertex2f &b);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator * (const float param, const AnimaVertex2f &v);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator * (const AnimaVertex2f &v, const float param);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator / (const AnimaVertex2f &a, const AnimaVertex2f &b);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator / (const float param, const AnimaVertex2f &v);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator / (const AnimaVertex2f &v, const float param);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator - (const AnimaVertex2f &a, const AnimaVertex2f &b);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator - (const float param, const AnimaVertex2f &v);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator - (const AnimaVertex2f &v, const float param);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator + (const AnimaVertex2f &a, const AnimaVertex2f &b);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator + (const float param, const AnimaVertex2f &v);
AnimaVertex2f ANIMA_ENGINE_EXPORT operator + (const AnimaVertex2f &v, const float param);

AFloat ANIMA_ENGINE_EXPORT operator * (const AnimaVertex3f &a, const AnimaVertex3f &b);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator * (const float param, const AnimaVertex3f &v);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator * (const AnimaVertex3f &v, const float param);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator / (const AnimaVertex3f &a, const AnimaVertex3f &b);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator / (const float param, const AnimaVertex3f &v);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator / (const AnimaVertex3f &v, const float param);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator - (const AnimaVertex3f &a, const AnimaVertex3f &b);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator - (const float param, const AnimaVertex3f &v);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator - (const AnimaVertex3f &v, const float param);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator + (const AnimaVertex3f &a, const AnimaVertex3f &b);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator + (const float param, const AnimaVertex3f &v);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator + (const AnimaVertex3f &v, const float param);
AnimaVertex3f ANIMA_ENGINE_EXPORT operator ^ (const AnimaVertex3f &a, const AnimaVertex3f &b);

AFloat ANIMA_ENGINE_EXPORT operator * (const AnimaVertex4f &a, const AnimaVertex4f &b);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator * (const float param, const AnimaVertex4f &v);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator * (const AnimaVertex4f &v, const float param);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator / (const AnimaVertex4f &a, const AnimaVertex4f &b);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator / (const float param, const AnimaVertex4f &v);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator / (const AnimaVertex4f &v, const float param);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator - (const AnimaVertex4f &a, const AnimaVertex4f &b);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator - (const float param, const AnimaVertex4f &v);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator - (const AnimaVertex4f &v, const float param);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator + (const AnimaVertex4f &a, const AnimaVertex4f &b);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator + (const float param, const AnimaVertex4f &v);
AnimaVertex4f ANIMA_ENGINE_EXPORT operator + (const AnimaVertex4f &v, const float param);

float ANIMA_ENGINE_EXPORT Dot(const AnimaVertex2f &a, const AnimaVertex2f &b);
float ANIMA_ENGINE_EXPORT Dot(const AnimaVertex3f &a, const AnimaVertex3f &b);
float ANIMA_ENGINE_EXPORT Dot(const AnimaVertex4f &a, const AnimaVertex4f &b);

AnimaVertex3f ANIMA_ENGINE_EXPORT Cross(const AnimaVertex3f &a, const AnimaVertex3f &b);

END_ANIMA_ENGINE_NAMESPACE

//template<class Type, ASizeT Size>
//class ANIMA_ENGINE_EXPORT AnimaVertex
//{
//public:
//	AnimaVertex(AnimaEngine* engine)
//	{
//		ANIMA_ASSERT(engine != nullptr);
//
//		_engine = engine;
//		_size = Size;
//
//		//_data = AnimaAllocatorNamespace::AllocateArray<Type>(*(_engine->GetGenericAllocator()), _size);
//		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
//		memset(_data, 0, sizeof(Type) * Size);
//	}
//
//	AnimaVertex(AnimaEngine* engine, Type data[Size])
//	{
//		ANIMA_ASSERT(engine != nullptr);
//
//		_engine = engine;
//		_size = Size;
//		
//		//_data = AnimaAllocatorNamespace::AllocateArray<Type>(*(_engine->GetGenericAllocator()), _size);
//		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
//		SetData(data);
//	}
//
//	AnimaVertex(const AnimaVertex& src)
//	{
//		if (&src != this)
//		{
//			_data = nullptr;
//			_size = src._size;
//			_engine = src._engine;
//			
//			//_data = AnimaAllocatorNamespace::AllocateArray<Type>(*(_engine->GetGenericAllocator()), _size);
//			_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
//			SetData(src._data);
//		}
//	}
//
//	AnimaVertex(AnimaVertex&& src)
//		: _engine(src._engine)
//		, _size(src._size)
//	{
//		//_data = AnimaAllocatorNamespace::AllocateArray<Type>(*(_engine->GetGenericAllocator()), _size);
//		_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
//		SetData(src._data);
//	}
//
//	~AnimaVertex()
//	{
//		ANIMA_ASSERT(_engine != nullptr);
//
//		if (_data != nullptr)
//		{
//			//AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), _data);
//			(_engine->GetGenericAllocator())->Deallocate(_data);
//			_data = nullptr;
//		}
//	}
//
//	AnimaVertex& operator=(const AnimaVertex& src)
//	{
//		if (this != &src)
//		{
//			ANIMA_ASSERT(src._size == _size);
//			ANIMA_ASSERT(_engine != nullptr && _data != nullptr);
//			//if (_data != nullptr)
//			//{
//			//	//AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), _data);
//			//	(_engine->GetGenericAllocator())->Deallocate(_data);
//			//	_data = nullptr;
//			//}
//
//			//_size = src._size;
//			////_data = AnimaAllocatorNamespace::AllocateArray<Type>(*(_engine->GetGenericAllocator()), _size);
//			//_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
//			SetData(src._data);
//		}
//
//		return *this;
//	}
//
//	AnimaVertex& operator=(AnimaVertex&& src)
//	{
//		if (this != &src)
//		{
//			ANIMA_ASSERT(src._size == _size);
//			ANIMA_ASSERT(_engine != nullptr && _data != nullptr);
//			SetData(src._data);
//			//_data = src._data;
//			//_size = src._size;
//			//if (_data != nullptr)
//			//{
//			//	//AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), _data);
//			//	(_engine->GetGenericAllocator())->Deallocate(_data);
//			//	_data = nullptr;
//			//}
//			//
//			////_data = AnimaAllocatorNamespace::AllocateArray<Type>(*(_engine->GetGenericAllocator()), _size);
//			//_data = (Type*)(_engine->GetGenericAllocator())->Allocate(sizeof(Type) * _size, ANIMA_ENGINE_ALIGN_OF(Type));
//			//SetData(src._data);
//		}
//
//		return *this;
//	}
//
//	Type& operator[](ASizeT index)
//	{
//		ANIMA_ASSERT(index >= 0 && index < _size);
//		return _data[index];
//	}
//
//	const Type& operator[](ASizeT index) const
//	{
//		ANIMA_ASSERT(index >= 0 && index < _size);
//		return const_cast<Type&>(_data[index]);
//	}
//
//	AnimaVertex& operator+=(const AnimaVertex& v)
//	{
//		for (ASizeT i = 0; i < _size; i++)
//			_data[i] += v[i];
//		return *this;
//	}
//	inline friend AnimaVertex operator+(const AnimaVertex& v1, const AnimaVertex& v2) {
//		AnimaVertex res(v1._engine, v1._data);
//		res += v2;
//		return res;
//	}
//	AnimaVertex& operator-=(const AnimaVertex& v)
//	{
//		for (ASizeT i = 0; i < _size; i++)
//			_data[i] -= v[i];
//		return *this;
//	}
//	inline friend AnimaVertex operator-(const AnimaVertex& v1, const AnimaVertex& v2) {
//		AnimaVertex res(v1._engine, v1._data);
//		res -= v2;
//		return res;
//	}
//	AnimaVertex& operator*=(const Type& mul)
//	{
//		for (ASizeT i = 0; i < _size; i++)
//			_data[i] *= mul;
//		return *this;
//	}
//	inline friend AnimaVertex operator*(const AnimaVertex& v, const Type& mul) {
//		AnimaVertex res(v._engine, v._data);
//		res *= mul;
//		return res;
//	}
//	AnimaVertex& operator/=(const Type& div)
//	{
//		for (ASizeT i = 0; i < _size; i++)
//			_data[i] /= div;
//		return *this;
//	}
//	inline friend AnimaVertex operator/(const AnimaVertex& v, const Type& div) {
//		AnimaVertex res(v._engine, v._data);
//		res /= div;
//		return res;
//	}
//	inline friend AnimaVertex operator*(const Type& mul, const AnimaVertex& v) {
//		AnimaVertex res(v._engine, v._data);
//		res *= mul;
//		return res;
//	}
//		
//	inline operator Type*()
//	{
//		return _data;
//	}
//
//	inline operator const Type*()
//	{
//		return const_cast<Type*>(_data);
//	}
//
//	inline void SetData(Type data[Size])
//	{
//		memcpy(_data, data, sizeof(Type) * _size);
//	}
//
//	inline Type* GetData() const
//	{
//		return _data;
//	}
//
//	inline void CopyData(Type* d) const
//	{
//		memcpy(d, _data, sizeof(Type) * _size);
//	}
//
//	inline const Type* GetConstData() const
//	{
//		return const_cast<Type*>(_data);
//	}
//
//	inline void Reverse()
//	{
//		for (int i = 0; i < _size / 2; i++)
//		{
//			Type tmp = _data[i];
//			_data[i] = _data[_size - i - 1];
//			_data[_size - i - 1] = tmp;
//		}
//	}
//
//	inline void Inverse()
//	{
//		for (int i = 0; i < _size; i++)
//			_data[i] *= (Type)-1;
//	}
//
//	void DumpMemory(bool bLogFile = true)
//	{
//		char szBuff[1024];
//		memset(szBuff, 0, 1024);
//
//		for (int i = 0; i < _size; i++)
//			sprintf(szBuff, "%s%f\t", szBuff, _data[i]);
//		sprintf(szBuff, "%s\n", szBuff);
//
//		if (bLogFile)
//		{
//			bool bCanClose = true;
//			if (!freopen(_engine->GetLogFilePath(), "a", stdout))
//				bCanClose = false;
//
//			puts(szBuff);
//
//			if (bCanClose)
//				fclose(stdout);
//		}
//		else
//		{
//			#if defined _MSC_VER
//				OutputDebugStringA(szBuff);
//			#else
//				puts(szBuff);
//			#endif
//		}
//	}
//
//	AnimaEngine* GetEngine() const
//	{
//		return _engine;
//	}
//
//	Type Length() const
//	{
//		Type rv = 0;
//		for (int i = 0; i < _size; i++)
//			rv += _data[i] * _data[i];
//
//		return (Type)sqrt(rv);
//	}
//
//	void Normalize()
//	{
//		Type len = Length();
//		for (int i = 0; i < _size; i++)
//			_data[i] /= len;
//	}
//
//	AnimaVertex Normalized()
//	{
//		AnimaVertex rv(this->_engine, this->_data);
//		rv.Normalize();
//		return rv;
//	}
//
//	ASizeT GetSize() const
//	{
//		return _size;
//	}
//		
//	bool IsNull() const
//	{
//		for (ASizeT i = 0; i < _size; i++)
//		{
//			if (_data[i] != (Type)0)
//				return false;
//		}
//		return true;
//	}
//
//protected:
//	AnimaEngine* _engine;
//	
//	Type*	_data;
//	ASizeT	_size;
//};
//
//typedef AnimaVertex<float, 2> AnimaVertex2f;
//typedef AnimaVertex<float, 3> AnimaVertex3f;
//typedef AnimaVertex<float, 4> AnimaVertex4f;
//
//typedef AnimaVertex<char, 3> AnimaColor3b;
//typedef AnimaVertex<char, 4> AnimaColor4b;
//
//typedef AnimaVertex<float, 3> AnimaColor3f;
//typedef AnimaVertex<float, 4> AnimaColor4f;

#endif /* defined(__Anima__AnimaVertex__) */
