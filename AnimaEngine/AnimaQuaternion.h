#pragma once

#include <stdio.h>
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaAllocators.h"
#include "AnimaAssert.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

//#define QUATERNION_SIZE	4
//
//class AnimaMath;

class ANIMA_ENGINE_EXPORT AnimaQuaternion
{
public:
	union
	{
		AFloat vec[4];
		struct
		{
			AFloat x, y, z, w;
		};
	};

public:
	AnimaQuaternion();
	AnimaQuaternion(const AnimaQuaternion& src);
	AnimaQuaternion(AnimaQuaternion&& src);
	AnimaQuaternion(const AFloat x, const AFloat y, const AFloat z, const AFloat w);
	AnimaQuaternion(const AnimaVertex3f& axis, AFloat rad);
	AnimaQuaternion(const AFloat axis[3], AFloat rad);
	AnimaQuaternion(const AnimaMatrix& mat);
	AnimaQuaternion(const AFloat mat[16]);
	~AnimaQuaternion();

	AnimaQuaternion& operator=(const AnimaQuaternion& src);
	AnimaQuaternion& operator=(AnimaQuaternion&& src);

	AnimaQuaternion operator+(const AnimaQuaternion& p) const;
	AnimaQuaternion& operator+=(const AnimaQuaternion& p);

	AnimaQuaternion operator*(const AnimaQuaternion& p) const;
	AnimaQuaternion& operator*=(const AnimaQuaternion& p);
	AnimaQuaternion& operator*=(const AFloat p);
	AnimaVertex3f operator*(const AnimaVertex3f& v) const;

	bool operator==(const AnimaQuaternion& p) const;
	bool operator!=(const AnimaQuaternion& p) const;

	void Set(const AFloat x, const AFloat y, const AFloat z, const AFloat w);

	void FromAxisAndAngleRad(const AnimaVertex3f& axis, AFloat rad);
	void FromAxisAndAngleDeg(const AnimaVertex3f& axis, AFloat deg);
	void FromAxisAndAngleRad(const AFloat axis[3], AFloat rad);
	void FromAxisAndAngleDeg(const AFloat axis[3], AFloat deg);

	void FromMatrix(const AnimaMatrix& mat);
	void FromMatrix(const AFloat mat[16]);

	AnimaMatrix GetMatrix() const;
	void GetMatrix(AFloat mat[16]) const;
	AnimaMatrix GetMatrix4x3() const;
	void GetMatrix4x3(AFloat mat[12]) const;

	void SetIdentity();

	void Slerp(const AnimaQuaternion& q, const AFloat c);
	AnimaQuaternion Slerped(const AnimaQuaternion& q, const AFloat c) const;

	void Conjugate();
	AnimaQuaternion Conjugated() const;

	void Inverse();
	AnimaQuaternion Inversed() const;

	AFloat Magnitude() const;
	AFloat Magnitude2() const;

	void Normalize();
	AnimaQuaternion Normalized() const;

	void Scale(const AFloat s);
	AnimaQuaternion Scaled(const AFloat s) const;
};

END_ANIMA_ENGINE_NAMESPACE