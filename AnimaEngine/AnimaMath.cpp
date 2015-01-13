//
//  AnimaMath.cpp
//  Anima
//
//  Created by Marco Zille on 17/12/14.
//
//

#include "AnimaMath.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define _mm_shufd(xmm, mask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), mask))

AnimaVertex3f AnimaMath::MatrixMulVector(const AnimaMatrix& m, const AnimaVertex3f& v)
{
	AnimaVertex4f v4fi(v.GetEngine(), v.GetData());
	v4fi[3] = 1.0f;

	AnimaVertex4f v4fr = MatrixMulVector(m, v4fi);
	AnimaVertex3f result(v.GetEngine());

	result[0] = v4fr[0];
	result[1] = v4fr[1];
	result[2] = v4fr[2];

	return result;
}

AnimaVertex4f AnimaMath::MatrixMulVector(const AnimaMatrix& m, const AnimaVertex4f& v)
{
	AFloat* src = m.GetData();
	const AFloat* srcV = v.GetConstData();

	__m128 m1 = _mm_set_ps(src[0], src[4], src[8], src[12]);
	__m128 m2 = _mm_set_ps(src[1], src[5], src[9], src[13]);
	__m128 m3 = _mm_set_ps(src[2], src[6], src[10], src[14]);
	__m128 m4 = _mm_set_ps(src[3], src[7], src[11], src[15]);
	__m128 vm = _mm_loadu_ps(srcV);

	__m128 res = _mm_add_ps(_mm_add_ps(_mm_mul_ps(m1, _mm_shufd(vm, 0x00)), _mm_mul_ps(m2, _mm_shufd(vm, 0x55))),
		_mm_add_ps(_mm_mul_ps(m3, _mm_shufd(vm, 0xAA)), _mm_mul_ps(m4, _mm_shufd(vm, 0xFF))));

	float* r = new float[4];
	_mm_storeu_ps(r, res);

	AnimaVertex4f result(v.GetEngine(), r);
	result.Reverse();
	delete[] r;
	return result;
}

AnimaVertex3f AnimaMath::MatrixDivVector(const AnimaMatrix& m, const AnimaVertex3f& v)
{
	AnimaMatrix inverse = m.Inverse();
	return MatrixMulVector(inverse, v);
}

AnimaVertex4f AnimaMath::MatrixDivVector(const AnimaMatrix& m, const AnimaVertex4f& v)
{
	AnimaMatrix inverse = m.Inverse();
	return MatrixMulVector(inverse, v);
}

AFloat AnimaMath::Dot(const AnimaVertex2f& v1, const AnimaVertex2f& v2)
{
	AFloat rv = 0;
	for (int i = 0; i < v1.GetSize(); i++)
		rv += v1[i] * v2[i];
	return rv;
}

AFloat AnimaMath::Dot(const AnimaVertex3f& v1, const AnimaVertex3f& v2)
{
	AFloat rv = 0;
	for (int i = 0; i < v1.GetSize(); i++)
		rv += v1[i] * v2[i];
	return rv;
}

AFloat AnimaMath::Dot(const AnimaVertex4f& v1, const AnimaVertex4f& v2)
{
	AFloat rv = 0;
	for (int i = 0; i < v1.GetSize(); i++)
		rv += v1[i] * v2[i];
	return rv;
}

AChar AnimaMath::Dot(const AnimaColor3b& v1, const AnimaColor3b& v2)
{
	AChar rv = 0;
	for (int i = 0; i < v1.GetSize(); i++)
		rv += v1[i] * v2[i];
	return rv;
}

AChar AnimaMath::Dot(const AnimaColor4b& v1, const AnimaColor4b& v2)
{
	AChar rv = 0;
	for (int i = 0; i < v1.GetSize(); i++)
		rv += v1[i] * v2[i];
	return rv;
}

AnimaQuaternion AnimaMath::QuaternionMulQuaternion(const AnimaQuaternion& q1, const AnimaQuaternion& q2)
{
	float w = q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2];
	float x = q1[0] * q2[3] + q1[3] * q2[0] + q1[1] * q2[2] - q1[2] * q2[1];
	float y = q1[1] * q2[3] + q1[3] * q2[1] + q1[2] * q2[0] - q1[0] * q2[2];
	float z = q1[2] * q2[3] + q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0];

	AnimaQuaternion res(q1.GetEngine(), x, y, z, w);
	return res;
}

AnimaQuaternion AnimaMath::QuaternionMulVector(const AnimaQuaternion& q, const AnimaVertex3f& v)
{
	float w = -q[0] * v[0] - q[1] * v[1] - q[2] * v[2];
	float x =  q[3] * v[0] + q[1] * v[2] - q[2] * v[1];
	float y =  q[3] * v[1] + q[2] * v[0] - q[0] * v[2];
	float z =  q[3] * v[2] + q[0] * v[1] - q[1] * v[0];

	AnimaQuaternion res(q.GetEngine(), x, y, z, w);
	return res;
}

AnimaVertex3f AnimaMath::Cross(const AnimaVertex3f& v1, const AnimaVertex3f& v2)
{
	AnimaVertex4f v1_4(v1.GetEngine(), v1.GetData());
	AnimaVertex4f v2_4(v2.GetEngine(), v2.GetData());
	v1_4[3] = 0.0f;
	v2_4[3] = 0.0f;

	AnimaVertex4f vr4 = Cross(v1_4, v2_4);
	AnimaVertex3f result(v1.GetEngine());

	result[0] = vr4[0];
	result[1] = vr4[1];
	result[2] = vr4[2];

	return result;
}

AnimaVertex4f AnimaMath::Cross(const AnimaVertex4f& v1, const AnimaVertex4f& v2)
{
	AFloat srcv1[4], srcv2[4];
	v1.CopyData(srcv1);
	v2.CopyData(srcv2);

	srcv1[3] = 0.0f;
	srcv2[3] = 0.0f;

	__m128 a = _mm_loadu_ps(srcv1);
	__m128 b = _mm_loadu_ps(srcv2);

	__m128 res = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))),
							_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1))));

	float* r = new float[4];
	_mm_storeu_ps(r, res);

	AnimaVertex4f result(v1.GetEngine(), r);
	delete[] r;
	return result;
}

AnimaVertex3f AnimaMath::RotateVector(const AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis)
{
	float sinHalfAngle = sinf(angle / 2.0f);
	float cosHalfAngle = cosf(angle / 2.0f);

	float rx = axis[0] * sinHalfAngle;
	float ry = axis[1] * sinHalfAngle;
	float rz = axis[2] * sinHalfAngle;
	float rw = cosHalfAngle;

	AnimaQuaternion rotation(v.GetEngine(), rx, ry, rz, rw);
	AnimaQuaternion conjugate = rotation.Conjugate();

	AnimaQuaternion w = QuaternionMulQuaternion(QuaternionMulVector(rotation, v), conjugate);

	AnimaVertex3f res(v.GetEngine());
	res[0] = w[0];
	res[1] = w[1];
	res[2] = w[2];

	return res;
}

void AnimaMath::RotateVector(AnimaColor3f& v, AFloat angle, const AnimaColor3f& axis)
{
	float sinHalfAngle = sinf(angle / 2.0f);
	float cosHalfAngle = cosf(angle / 2.0f);

	float rx = axis[0] * sinHalfAngle;
	float ry = axis[1] * sinHalfAngle;
	float rz = axis[2] * sinHalfAngle;
	float rw = cosHalfAngle;

	AnimaQuaternion rotation(v.GetEngine(), rx, ry, rz, rw);
	AnimaQuaternion conjugate = rotation.Conjugate();

	AnimaQuaternion w = QuaternionMulQuaternion(QuaternionMulVector(rotation, v), conjugate);

	v[0] = w[0];
	v[1] = w[1];
	v[2] = w[2];
}

#undef _mm_shufd

END_ANIMA_ENGINE_NAMESPACE

Anima::AnimaVertex3f operator*(const Anima::AnimaMatrix& m, const Anima::AnimaVertex3f& v)
{
	return Anima::AnimaMath::MatrixMulVector(m, v);
}

Anima::AnimaVertex4f operator*(const Anima::AnimaMatrix& m, const Anima::AnimaVertex4f& v)
{
	return Anima::AnimaMath::MatrixMulVector(m, v);
}

Anima::AnimaVertex3f operator/(const Anima::AnimaMatrix& m, const Anima::AnimaVertex3f& v)
{
	return Anima::AnimaMath::MatrixDivVector(m, v);
}

Anima::AnimaVertex4f operator/(const Anima::AnimaMatrix& m, const Anima::AnimaVertex4f& v)
{
	return Anima::AnimaMath::MatrixDivVector(m, v);
}

void operator*=(Anima::AnimaVertex3f& v, const Anima::AnimaMatrix& m)
{
	v = m * v;
}

void operator*=(Anima::AnimaVertex4f& v, const Anima::AnimaMatrix& m)
{
	v = m * v;
}

void operator/=(Anima::AnimaVertex3f& v, const Anima::AnimaMatrix& m)
{
	v = m / v;
}

void operator/=(Anima::AnimaVertex4f& v, const Anima::AnimaMatrix& m)
{
	v = m / v;
}

Anima::AFloat operator*(const Anima::AnimaVertex2f& v1, const Anima::AnimaVertex2f& v2)
{
	return Anima::AnimaMath::Dot(v1, v2);
}

Anima::AFloat operator*(const Anima::AnimaVertex3f& v1, const Anima::AnimaVertex3f& v2)
{
	return Anima::AnimaMath::Dot(v1, v2);
}

Anima::AFloat operator*(const Anima::AnimaVertex4f& v1, const Anima::AnimaVertex4f& v2)
{
	return Anima::AnimaMath::Dot(v1, v2);
}

Anima::AChar operator*(const Anima::AnimaColor3b& v1, const Anima::AnimaColor3b& v2)
{
	return Anima::AnimaMath::Dot(v1, v2);
}

Anima::AChar operator*(const Anima::AnimaColor4b& v1, const Anima::AnimaColor4b& v2)
{
	return Anima::AnimaMath::Dot(v1, v2);
}

Anima::AnimaVertex3f operator^(const Anima::AnimaVertex3f& v1, const Anima::AnimaVertex3f& v2)
{
	return Anima::AnimaMath::Cross(v1, v2);
}

Anima::AnimaVertex4f operator^(const Anima::AnimaVertex4f& v1, const Anima::AnimaVertex4f& v2)
{
	return Anima::AnimaMath::Cross(v1, v2);
}

void operator*=(Anima::AnimaQuaternion& q, const Anima::AnimaVertex3f& v)
{
	q = q * v;
}

void operator*=(Anima::AnimaQuaternion& q1, const Anima::AnimaQuaternion& q2)
{
	q1 = q1 * q2;
}

Anima::AnimaQuaternion operator*(const Anima::AnimaQuaternion& q1, const Anima::AnimaQuaternion& q2)
{
	return Anima::AnimaMath::QuaternionMulQuaternion(q1, q2);
}

Anima::AnimaQuaternion operator*(const Anima::AnimaQuaternion& q, const Anima::AnimaVertex3f& v)
{
	return Anima::AnimaMath::QuaternionMulVector(q, v);
}