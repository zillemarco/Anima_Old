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

bool AnimaMath::PointInsideSphere(const AnimaVertex3f& point, const AnimaVertex3f& sphereCenter, const AFloat& sphereRadius)
{
	return (point - sphereCenter).Length() <= sphereRadius;
}

bool AnimaMath::PointInsideCone(const AnimaVertex3f& point, const AnimaVertex3f& coneTip, const AnimaVertex3f& coneDirection, float coneHeight, float coneAngle)
{
	Anima::AnimaVertex3f dv = point - coneTip;	
	if (dv.Length() > coneHeight)
		return false;

	dv.Normalize();
	return (coneDirection * dv >= cosf(coneAngle));
}

END_ANIMA_ENGINE_NAMESPACE