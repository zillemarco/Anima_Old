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

class AnimaQuaternion
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

	AnimaQuaternion& operator+=(const AnimaQuaternion& v);
	inline friend AnimaQuaternion operator+(const AnimaQuaternion& v1, const AnimaQuaternion& v2) {
		AnimaQuaternion res(v1._engine, v1._data);
		res += v2;
		return res;
	}

	AnimaQuaternion& operator-=(const AnimaQuaternion& v);
	inline friend AnimaQuaternion operator-(const AnimaQuaternion& v1, const AnimaQuaternion& v2) {
		AnimaQuaternion res(v1._engine, v1._data);
		res -= v2;
		return res;
	}

	AnimaQuaternion& operator/=(const AFloat& div);
	inline friend AnimaQuaternion operator/(const AnimaQuaternion& v, const AFloat& div) {
		AnimaQuaternion res(v._engine, v._data);
		res /= div;
		return res;
	}

	AnimaQuaternion& operator*=(const AFloat& mul);
	inline friend AnimaQuaternion operator*(const AFloat& mul, const AnimaQuaternion& v) {
		AnimaQuaternion res(v._engine, v._data);
		res *= mul;
		return res;
	}

	inline friend AnimaQuaternion operator*(const AnimaQuaternion& v, const AFloat& mul) {
		return operator*(mul, v);
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

	AFloat Length() const;
	bool IsNull() const;

	void Normalize();

	AnimaEngine* GetEngine() const;

protected:
	AnimaEngine* _engine;
	AFloat*	_data;
};

END_ANIMA_ENGINE_NAMESPACE