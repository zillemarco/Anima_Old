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

	void GetRotation(AFloat& rx, AFloat& ry) const;

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

#endif /* defined(__Anima__AnimaVertex__) */
