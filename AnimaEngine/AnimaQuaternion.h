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

#define QUATERNION_SIZE	4

class AnimaMath;

class ANIMA_ENGINE_EXPORT AnimaQuaternion
{
public:
	AnimaQuaternion(AnimaEngine* engine);
	AnimaQuaternion(AnimaEngine* engine, AFloat data[QUATERNION_SIZE]);
	AnimaQuaternion(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w);
	AnimaQuaternion(AnimaEngine* engine, const AnimaVertex3f& axis, AFloat angle, bool angleInRadians = true);
	AnimaQuaternion(AnimaEngine* engine, AFloat head, AFloat pitch, AFloat roll, bool angleInRadians = true);
	AnimaQuaternion(AnimaEngine* engine, AnimaMatrix matrix);
	AnimaQuaternion(const AnimaQuaternion& src);
	AnimaQuaternion(AnimaQuaternion&& src);
	~AnimaQuaternion();

	AFloat& operator[](ASizeT index);
	const AFloat& operator[](ASizeT index) const;

	AnimaQuaternion& operator+=(const AnimaQuaternion& q);
	inline friend AnimaQuaternion operator+(const AnimaQuaternion& q1, const AnimaQuaternion& q2) {
		AnimaQuaternion res(q1._engine, q1._data);
		res += q2;
		return res;
	}

	AnimaQuaternion& operator-=(const AnimaQuaternion& q);
	inline friend AnimaQuaternion operator-(const AnimaQuaternion& q1, const AnimaQuaternion& q2) {
		AnimaQuaternion res(q1._engine, q1._data);
		res -= q2;
		return res;
	}

	AnimaQuaternion& operator*=(const AnimaQuaternion& q);
	inline friend AnimaQuaternion operator*(const AnimaQuaternion& q1, const AnimaQuaternion& q2) {

		float w = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
		float x = q1[0] * q2[3] + q1[3] * q2[0] + q1[1] * q2[2] - q1[2] * q2[1];
		float y = q1[1] * q2[3] + q1[3] * q2[1] + q1[2] * q2[0] - q1[0] * q2[2];
		float z = q1[2] * q2[3] + q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0];

		AnimaQuaternion res(q1.GetEngine(), x, y, z, w);
		return res;
	}

	AnimaQuaternion& operator/=(const AFloat& div);
	inline friend AnimaQuaternion operator/(const AnimaQuaternion& q, const AFloat& div) {
		AnimaQuaternion res(q._engine, q._data);
		res /= div;
		return res;
	}

	AnimaQuaternion& operator*=(const AFloat& mul);
	inline friend AnimaQuaternion operator*(const AFloat& mul, const AnimaQuaternion& q) {
		AnimaQuaternion res(q._engine, q._data);
		res *= mul;
		return res;
	}

	inline friend AnimaQuaternion operator*(const AnimaQuaternion& q, const AFloat& mul) {
		return operator*(mul, q);
	}
		
	inline operator AFloat*();
	inline operator const AFloat*();

public:
	void SetData(AFloat value, ASizeT index);
	void SetData(AFloat x, AFloat y, AFloat z, AFloat w);
	inline void SetData(AFloat data[QUATERNION_SIZE]);
	inline AFloat* GetData() const;
	inline const AFloat* GetConstData() const;
	inline void CopyData(AFloat* d) const;
	
	void FromAxisAndAngle(const AnimaVertex3f& axis, AFloat angle);
	void FromAxisAndAngleDeg(const AnimaVertex3f& axis, AFloat angle);
	void FromHeadPitchRoll(AFloat head, AFloat pitch, AFloat roll);
	void FromHeadPitchRollDeg(AFloat head, AFloat pitch, AFloat roll);
	void FromMatrix(const AnimaMatrix& matrix);
	void SetIdentity();
	
	AnimaQuaternion Conjugate() const;
	AnimaQuaternion Inverse() const;

	AnimaMatrix GetMatrix() const;
	void GetHeadPitchRoll(AFloat& head, AFloat& pitch, AFloat& roll) const;
	void GetHeadPitchRollDeg(AFloat& head, AFloat& pitch, AFloat& roll) const;
	void GetAxisAngle(AnimaVertex3f& axis, AFloat angle) const;
	void GetAxisAngleDeg(AnimaVertex3f& axis, AFloat angle) const;
	AnimaVertex3f GetVector() const;

	AFloat Length() const;
	bool IsNull() const;

	void Normalize();

	AnimaEngine* GetEngine() const;

protected:
	AnimaEngine* _engine;
	AFloat*	_data;
};

END_ANIMA_ENGINE_NAMESPACE