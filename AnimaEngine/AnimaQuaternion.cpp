#include "AnimaQuaternion.h"
#include "AnimaMath.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE
AnimaQuaternion::AnimaQuaternion()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
	, w(0.0f)
{
}

AnimaQuaternion::AnimaQuaternion(const AnimaQuaternion& src)
	: x(src.x)
	, y(src.y)
	, z(src.z)
	, w(src.w)
{
}

AnimaQuaternion::AnimaQuaternion(AnimaQuaternion&& src)
	: x(src.x)
	, y(src.y)
	, z(src.z)
	, w(src.w)
{
}

AnimaQuaternion::AnimaQuaternion(const AFloat x, const AFloat y, const AFloat z, const AFloat w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{
}

AnimaQuaternion::AnimaQuaternion(const AnimaVertex3f& axis, AFloat rad)
{
	FromAxisAndAngleRad(axis, rad);
}

AnimaQuaternion::AnimaQuaternion(const AFloat axis[3], AFloat rad)
{
	FromAxisAndAngleRad(axis, rad);
}

AnimaQuaternion::AnimaQuaternion(const AnimaMatrix& mat)
{
	FromMatrix(mat);
}

AnimaQuaternion::AnimaQuaternion(const AFloat mat[16])
{
	FromMatrix(mat);
}

AnimaQuaternion& AnimaQuaternion::operator=(const AnimaQuaternion& src)
{
	if (this != &src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
	}
	return *this;
}

AnimaQuaternion& AnimaQuaternion::operator=(AnimaQuaternion&& src)
{
	if (this != &src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		w = src.w;
	}
	return *this;
}

AnimaQuaternion::~AnimaQuaternion()
{
}

AnimaQuaternion AnimaQuaternion::operator+(const AnimaQuaternion& p) const
{
	return AnimaQuaternion(x + p.x, y + p.y, z + p.z, w + p.w);
}

AnimaQuaternion& AnimaQuaternion::operator+=(const AnimaQuaternion& p)
{
	x += p.x;
	y += p.y;
	z += p.z;
	w += p.w;
	return *this;
}

AnimaQuaternion AnimaQuaternion::operator*(const AnimaQuaternion& p) const
{
	AnimaQuaternion qp(*this);
	qp *= p;
	return qp;
}

AnimaVertex3f AnimaQuaternion::operator*(const AnimaVertex3f& v) const
{
	AnimaVertex3f uv, uuv;
	AnimaVertex3f qvec(x, y, z);
	uv = Cross(qvec, v);
	uuv = Cross(qvec, uv);
	uv *= 2 * w;
	uuv *= 2;
	return v + uv + uuv;
}

AnimaQuaternion& AnimaQuaternion::operator*=(const AnimaQuaternion& p)
{
	float vp[3];

	vp[0] = w * p.x + x * p.w + y * p.z - z * p.y;
	vp[1] = w * p.y - x * p.z + y * p.w + z * p.x;
	vp[2] = w * p.z + x * p.y - y * p.x + z * p.w;

	w = w * p.w - (x * p.x + y * p.y + z * p.z);

	x = vp[0];
	y = vp[1];
	z = vp[2];

	return *this;
}

AnimaQuaternion& AnimaQuaternion::operator*=(const AFloat p)
{
	x *= p;
	y *= p;
	z *= p;
	w *= p;
	return *this;
}

bool AnimaQuaternion::operator==(const AnimaQuaternion& p) const
{
	return (x == p.x && y == p.y && z == p.z && w == p.w);
}

bool AnimaQuaternion::operator!=(const AnimaQuaternion& p) const
{
	return (x != p.x || y != p.y || z != p.z || w != p.w);
}

void AnimaQuaternion::Set(const AFloat x, const AFloat y, const AFloat z, const AFloat w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void AnimaQuaternion::FromAxisAndAngleRad(const AnimaVertex3f& axis, AFloat rad)
{
	rad *= 0.5f;
	w = cosf(rad);
	float sr = sinf(rad);
	x = axis.vec[0] * sr;
	y = axis.vec[1] * sr;
	z = axis.vec[2] * sr;
}

void AnimaQuaternion::FromAxisAndAngleDeg(const AnimaVertex3f& axis, AFloat deg)
{
	FromAxisAndAngleRad(axis, deg * (AFloat)M_PI / 180.0f);
}

void AnimaQuaternion::FromAxisAndAngleRad(const AFloat axis[3], AFloat rad)
{
	rad *= 0.5f;
	w = cosf(rad);
	float sr = sinf(rad);
	x = axis[0] * sr;
	y = axis[1] * sr;
	z = axis[2] * sr;
}

void AnimaQuaternion::FromAxisAndAngleDeg(const AFloat axis[3], AFloat deg)
{
	FromAxisAndAngleRad(axis, deg * (AFloat)M_PI / 180.0f);
}

void AnimaQuaternion::FromMatrix(const AnimaMatrix& mat)
{
	w = 1.0f + mat.m[0] + mat.m[5] + mat.m[10];
	if (w <= 0.0f)
		SetIdentity();
	else
	{
		w = sqrt(w) * 0.5f;
		float invW4 = 0.25f / w;
		x = (mat.m[9] - mat.m[6]) * invW4;
		y = (mat.m[2] - mat.m[8]) * invW4;
		z = (mat.m[4] - mat.m[1]) * invW4;
	}
}

void AnimaQuaternion::FromMatrix(const AFloat mat[16])
{
	w = 1.0f + mat[0] + mat[5] + mat[10];
	if (w <= 0.0f)
		SetIdentity();
	else
	{
		w = sqrt(w) * 0.5f;
		float invW4 = 0.25f / w;
		x = (mat[9] - mat[6]) * invW4;
		y = (mat[2] - mat[8]) * invW4;
		z = (mat[4] - mat[1]) * invW4;
	}
}

AnimaMatrix AnimaQuaternion::GetMatrix() const
{
	float x2, y2, z2, xx, yy, zz, xy, yz, xz, wx, wy, wz;
	x2 = x + x;
	y2 = y + y;
	z2 = z + z;
	xx = x * x2;
	yy = y * y2;
	zz = z * z2;
	xy = x * y2;
	yz = y * z2;
	xz = z * x2;
	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	AnimaMatrix m;
	m.m[0] = 1.0f - (yy + zz);
	m.m[1] = xy + wz;
	m.m[2] = xz - wy;
	m.m[3] = 0.0f;
	m.m[4] = xy - wz;
	m.m[5] = 1.0f - (xx + zz);
	m.m[6] = yz + wx;
	m.m[7] = 0.0f;
	m.m[8] = xz + wy;
	m.m[9] = yz - wx;
	m.m[10] = 1.0f - (xx + yy);
	m.m[11] = 0.0f;
	m.m[12] = 0.0f;
	m.m[13] = 0.0f;
	m.m[14] = 0.0f;
	m.m[15] = 1.0f;

	return m;
}

void AnimaQuaternion::GetMatrix(AFloat mat[16]) const
{
	float x2, y2, z2, xx, yy, zz, xy, yz, xz, wx, wy, wz;
	x2 = x + x;
	y2 = y + y;
	z2 = z + z;
	xx = x * x2;
	yy = y * y2;
	zz = z * z2;
	xy = x * y2;
	yz = y * z2;
	xz = z * x2;
	wx = w * x2;
	wy = w * y2;
	wz = w * z2;
	mat[0] = 1.0f - (yy + zz);
	mat[1] = xy + wz;
	mat[2] = xz - wy;
	mat[3] = 0.0f;
	mat[4] = xy - wz;
	mat[5] = 1.0f - (xx + zz);
	mat[6] = yz + wx;
	mat[7] = 0.0f;
	mat[8] = xz + wy;
	mat[9] = yz - wx;
	mat[10] = 1.0f - (xx + yy);
	mat[11] = 0.0f;
	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
}

AnimaMatrix AnimaQuaternion::GetMatrix4x3() const
{
	float x2, y2, z2, xx, yy, zz, xy, yz, xz, wx, wy, wz;
	x2 = x + x;
	y2 = y + y;
	z2 = z + z;
	xx = x * x2;
	yy = y * y2;
	zz = z * z2;
	xy = x * y2;
	yz = y * z2;
	xz = z * x2;
	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	AnimaMatrix m;
	m.m[0] = 1.0f - (yy + zz);
	m.m[1] = xy + wz;
	m.m[2] = xz - wy;
	m.m[3] = 0.0f;
	m.m[4] = xy - wz;
	m.m[5] = 1.0f - (xx + zz);
	m.m[6] = yz + wx;
	m.m[7] = 0.0f;
	m.m[8] = xz + wy;
	m.m[9] = yz - wx;
	m.m[10] = 1.0f - (xx + yy);
	m.m[11] = 0.0f;

	return m;
}

void AnimaQuaternion::GetMatrix4x3(AFloat mat[12]) const
{
	float x2, y2, z2, xx, yy, zz, xy, yz, xz, wx, wy, wz;
	x2 = x + x;
	y2 = y + y;
	z2 = z + z;
	xx = x * x2;
	yy = y * y2;
	zz = z * z2;
	xy = x * y2;
	yz = y * z2;
	xz = z * x2;
	wx = w * x2;
	wy = w * y2;
	wz = w * z2;
	mat[0] = 1.0f - (yy + zz);
	mat[1] = xy + wz;
	mat[2] = xz - wy;
	mat[3] = 0.0f;
	mat[4] = xy - wz;
	mat[5] = 1.0f - (xx + zz);
	mat[6] = yz + wx;
	mat[7] = 0.0f;
	mat[8] = xz + wy;
	mat[9] = yz - wx;
	mat[10] = 1.0f - (xx + yy);
	mat[11] = 0.0f;
}

void AnimaQuaternion::SetIdentity()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

void AnimaQuaternion::Slerp(const AnimaQuaternion& q, const AFloat c)
{
	float omega, cosfomega, sinfomega, k1, k2;
	AnimaQuaternion q2;
	cosfomega = x * q.x + y * q.y + z * q.z + w * q.w;
	if (cosfomega < 0)
	{
		cosfomega = -cosfomega;
		q2.x = -q.x;
		q2.y = -q.y;
		q2.z = -q.z;
		q2.w = -q.w;
	}
	else
		q2 = q;

	if (1 - cosfomega > .000001f)
	{
		omega = acosf(cosfomega);
		sinfomega = sinf(omega);
		k1 = sinf((1 - c) * omega) / sinfomega;
		k2 = sinf(c * omega) / sinfomega;
	}
	else
	{
		k1 = 1 - c;
		k2 = c;
	}

	this->x = x * k1 + q2.x * k2;
	this->y = y * k1 + q2.y * k2;
	this->z = z * k1 + q2.z * k2;
	this->w = w * k1 + q2.w * k2;
}

AnimaQuaternion AnimaQuaternion::Slerped(const AnimaQuaternion& q, const AFloat c) const
{
	float omega, cosfomega, sinfomega, k1, k2;
	AnimaQuaternion q2, q3;
	cosfomega = x * q.x + y * q.y + z * q.z + w * q.w;
	if (cosfomega < 0)
	{
		cosfomega = -cosfomega;
		q2.x = -q.x;
		q2.y = -q.y;
		q2.z = -q.z;
		q2.w = -q.w;
	}
	else
		q2 = q;

	if (1 - cosfomega > .000001f)
	{
		omega = acosf(cosfomega);
		sinfomega = sinf(omega);
		k1 = sinf((1 - c) * omega) / sinfomega;
		k2 = sinf(c * omega) / sinfomega;
	}
	else
	{
		k1 = 1 - c;
		k2 = c;
	}

	q3.x = x * k1 + q2.x * k2;
	q3.y = y * k1 + q2.y * k2;
	q3.z = z * k1 + q2.z * k2;
	q3.w = w * k1 + q2.w * k2;

	return q3;
}

void AnimaQuaternion::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

AnimaQuaternion AnimaQuaternion::Conjugated() const
{
	return AnimaQuaternion(-x, -y, -z, w);
}

void AnimaQuaternion::Inverse()
{
	Conjugate();
	float invmag2 = 1.0f / Magnitude2();
	x *= invmag2;
	y *= invmag2;
	z *= invmag2;
	w *= invmag2;
}

AnimaQuaternion AnimaQuaternion::Inversed() const
{
	AnimaQuaternion q = Conjugated();
	float invmag2 = 1.0f / Magnitude2();
	q.x *= invmag2;
	q.y *= invmag2;
	q.z *= invmag2;
	q.w *= invmag2;
	return q;
}

AFloat AnimaQuaternion::Magnitude() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

AFloat AnimaQuaternion::Magnitude2() const
{
	return x * x + y * y + z * z + w * w;
}

void AnimaQuaternion::Normalize()
{
	float mag = Magnitude();
	
	if (mag < 0.000001f)
		return;

	float invmag = 1.0f / mag;
	x *= invmag;
	y *= invmag;
	z *= invmag;
	w *= invmag;
}

AnimaQuaternion AnimaQuaternion::Normalized() const
{
	float mag = Magnitude();
	if (mag < .000001f)
		return *this;
	float invmag = 1.0f / mag;

	AnimaQuaternion q(*this);
	q.x *= invmag;
	q.y *= invmag;
	q.z *= invmag;
	q.w *= invmag;

	return q;
}

void AnimaQuaternion::Scale(const AFloat s)
{
	w *= s;
	float s1 = sqrt(1.0f - w * w);
	float s2 = sqrt(x * x + y * y + z * z);
	float s3 = s1 / s2;
	if (s2 < 0.00001f)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	else
	{
		x *= s3;
		y *= s3;
		z *= s3;
	}
}

AnimaQuaternion AnimaQuaternion::Scaled(const AFloat s) const
{
	AnimaQuaternion q(*this);

	q.w *= s;
	float s1 = sqrt(1.0f - q.w * q.w);
	float s2 = sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
	float s3 = s1 / s2;
	if (s2 < 0.00001f)
	{
		q.x = 0.0f;
		q.y = 0.0f;
		q.z = 0.0f;
	}
	else
	{
		q.x *= s3;
		q.y *= s3;
		q.z *= s3;
	}

	return q;
}

END_ANIMA_ENGINE_NAMESPACE