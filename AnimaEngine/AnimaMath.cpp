//
//  AnimaMath.cpp
//  Anima
//
//  Created by Marco Zille on 17/12/14.
//
//

#include "AnimaMath.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#define _mm_shufd(xmm, mask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), mask))

//AnimaVertex3f AnimaMath::MatrixMulVector(const AnimaMatrix& m, const AnimaVertex3f& v)
//{
//	AnimaVertex4f v4fi(v.GetEngine(), v.GetData());
//	v4fi[3] = 1.0f;
//
//	AnimaVertex4f v4fr = MatrixMulVector(m, v4fi);
//	AnimaVertex3f result(v.GetEngine());
//
//	result[0] = v4fr[0];
//	result[1] = v4fr[1];
//	result[2] = v4fr[2];
//
//	return result;
//}
//
//AnimaVertex4f AnimaMath::MatrixMulVector(const AnimaMatrix& m, const AnimaVertex4f& v)
//{
//	AFloat* src = m.GetData();
//	const AFloat* srcV = v.GetConstData();
//
//	__m128 m1 = _mm_set_ps(src[0], src[4], src[8], src[12]);
//	__m128 m2 = _mm_set_ps(src[1], src[5], src[9], src[13]);
//	__m128 m3 = _mm_set_ps(src[2], src[6], src[10], src[14]);
//	__m128 m4 = _mm_set_ps(src[3], src[7], src[11], src[15]);
//	__m128 vm = _mm_loadu_ps(srcV);
//
//	__m128 res = _mm_add_ps(_mm_add_ps(_mm_mul_ps(m1, _mm_shufd(vm, 0x00)), _mm_mul_ps(m2, _mm_shufd(vm, 0x55))),
//		_mm_add_ps(_mm_mul_ps(m3, _mm_shufd(vm, 0xAA)), _mm_mul_ps(m4, _mm_shufd(vm, 0xFF))));
//
//	float* r = new float[4];
//	_mm_storeu_ps(r, res);
//
//	AnimaVertex4f result(v.GetEngine(), r);
//	result.Reverse();
//	delete[] r;
//	return result;
//}
//
//AnimaVertex3f AnimaMath::MatrixDivVector(const AnimaMatrix& m, const AnimaVertex3f& v)
//{
//	AnimaMatrix inverse = m.Inverse();
//	return MatrixMulVector(inverse, v);
//}
//
//AnimaVertex4f AnimaMath::MatrixDivVector(const AnimaMatrix& m, const AnimaVertex4f& v)
//{
//	AnimaMatrix inverse = m.Inverse();
//	return MatrixMulVector(inverse, v);
//}
//
AnimaQuaternion AnimaMath::QuaternionMulQuaternion(const AnimaQuaternion& q1, const AnimaQuaternion& q2)
{
	float w = q1.vec[3] * q2.vec[3] - q1.vec[0] * q2.vec[0] - q1.vec[1] * q2.vec[1] - q1.vec[2] * q2.vec[2];
	float x = q1.vec[0] * q2.vec[3] + q1.vec[3] * q2.vec[0] + q1.vec[1] * q2.vec[2] - q1.vec[2] * q2.vec[1];
	float y = q1.vec[1] * q2.vec[3] + q1.vec[3] * q2.vec[1] + q1.vec[2] * q2.vec[0] - q1.vec[0] * q2.vec[2];
	float z = q1.vec[2] * q2.vec[3] + q1.vec[3] * q2.vec[2] + q1.vec[0] * q2.vec[1] - q1.vec[1] * q2.vec[0];

	AnimaQuaternion res(x, y, z, w);
	return res;
}

AnimaQuaternion AnimaMath::QuaternionMulVector(const AnimaQuaternion& q, const AnimaVertex3f& v)
{
	float w = -q.vec[0] * v.vec[0] - q.vec[1] * v.vec[1] - q.vec[2] * v.vec[2];
	float x =  q.vec[3] * v.vec[0] + q.vec[1] * v.vec[2] - q.vec[2] * v.vec[1];
	float y =  q.vec[3] * v.vec[1] + q.vec[2] * v.vec[0] - q.vec[0] * v.vec[2];
	float z =  q.vec[3] * v.vec[2] + q.vec[0] * v.vec[1] - q.vec[1] * v.vec[0];

	AnimaQuaternion res(x, y, z, w);
	return res;
}
//
//AFloat AnimaMath::AngleBetweenVectors(const AnimaVertex3f& v1, const AnimaVertex3f& v2)
//{
//	ANIMA_ASSERT(!v1.IsNull() && !v2.IsNull());
//	AnimaVertex3f nv1 = v1;
//	AnimaVertex3f nv2 = v2;
//
//	nv1.Normalize();
//	nv2.Normalize();
//
//	return acosf(Dot(nv1, nv1) / (nv1.Length() * nv1.Length()));
//}
//
//AFloat AnimaMath::AngleBetweenVectorsDeg(const AnimaVertex3f& v1, const AnimaVertex3f& v2)
//{
//	ANIMA_ASSERT(!v1.IsNull() && !v2.IsNull());
//	AnimaVertex3f nv1 = v1;
//	AnimaVertex3f nv2 = v2;
//
//	nv1.Normalize();
//	nv2.Normalize();
//
//	return acosf(Dot(nv1, nv1) / (nv1.Length() * nv1.Length())) * 180.0f / (float)M_PI;
//}
//
AnimaVertex3f AnimaMath::RotateVector(const AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis)
{
	float sinHalfAngle = sinf(angle / 2.0f);
	float cosHalfAngle = cosf(angle / 2.0f);

	float rx = axis.vec[0] * sinHalfAngle;
	float ry = axis.vec[1] * sinHalfAngle;
	float rz = axis.vec[2] * sinHalfAngle;
	float rw = cosHalfAngle;

	AnimaQuaternion rotation(rx, ry, rz, rw);
	AnimaQuaternion conjugate = rotation.Conjugated();

	AnimaQuaternion w = QuaternionMulQuaternion(QuaternionMulVector(rotation, v), conjugate);

	AnimaVertex3f res(w.x, w.y, w.z);
	return res;
}

void AnimaMath::RotateVector(AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis)
{
	float sinHalfAngle = sinf(angle / 2.0f);
	float cosHalfAngle = cosf(angle / 2.0f);

	float rx = axis.vec[0] * sinHalfAngle;
	float ry = axis.vec[1] * sinHalfAngle;
	float rz = axis.vec[2] * sinHalfAngle;
	float rw = cosHalfAngle;

	AnimaQuaternion rotation(rx, ry, rz, rw);
	AnimaQuaternion conjugate = rotation.Conjugated();

	AnimaQuaternion w = QuaternionMulQuaternion(QuaternionMulVector(rotation, v), conjugate);

	v.vec[0] = w.vec[0];
	v.vec[1] = w.vec[1];
	v.vec[2] = w.vec[2];
}

#undef _mm_shufd

END_ANIMA_ENGINE_NAMESPACE