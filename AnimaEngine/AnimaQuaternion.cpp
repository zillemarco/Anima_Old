#include "AnimaQuaternion.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);

	_engine = engine;

	_data = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * QUATERNION_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
	memset(_data, 0, sizeof(AFloat) * QUATERNION_SIZE);

	SetIdentity();
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, AFloat data[4])
	: AnimaQuaternion(engine)
{
	SetData(data);
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w)
	: AnimaQuaternion(engine)
{
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, const AnimaVertex3f& axis, AFloat angle, bool angleInRadians)
	: AnimaQuaternion(engine)
{
	if (angleInRadians)
		FromAxisAndAngle(axis, angle);
	else
		FromAxisAndAngleDeg(axis, angle);
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, AFloat head, AFloat pitch, AFloat roll, bool angleInRadians)
	: AnimaQuaternion(engine)
{
	if (angleInRadians)
		FromHeadPitchRoll(head, pitch, roll);
	else
		FromHeadPitchRollDeg(head, pitch, roll);
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, AnimaMatrix matrix)
	: AnimaQuaternion(engine)
{
	FromMatrix(matrix);
}

AnimaQuaternion::AnimaQuaternion(const AnimaQuaternion& src)
{
	if (&src != this)
	{
		_engine = src._engine;
		_data = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * QUATERNION_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
		SetData(src._data);
	}
}

AnimaQuaternion::AnimaQuaternion(AnimaQuaternion&& src)
{
	_engine = src._engine;
	_data = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * QUATERNION_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
	SetData(src._data);
}

AnimaQuaternion::~AnimaQuaternion()
{
	ANIMA_ASSERT(_engine != nullptr);

	if (_data != nullptr)
	{
		(_engine->GetGenericAllocator())->Deallocate(_data);
		_data = nullptr;
	}
}

AnimaQuaternion AnimaQuaternion::Conjugate() const
{
	AnimaQuaternion conj(_engine, -_data[0], -_data[1], -_data[2], _data[3]);
	return conj;
}

AnimaQuaternion AnimaQuaternion::Inverse() const
{
	AFloat invMag = 1.0f / Length();
	return Conjugate() * invMag;
}

AnimaMatrix AnimaQuaternion::GetMatrix() const
{
	AnimaMatrix m(_engine);
	m[0] = 1.0f - (2.0f * _data[1] * _data[1]) - (2.0f * _data[2] * _data[2]);
	m[1] = (2.0f * _data[0] * _data[1]) - (2.0f * _data[2] * _data[3]);
	m[2] = (2.0f * _data[0] * _data[2]) + (2.0f * _data[1] * _data[3]);
	m[3] = 0.0f;
	m[4] = (2.0f * _data[0] * _data[1]) + (2.0f * _data[2] * _data[3]);
	m[5] = 1.0f - (2.0f * _data[0] * _data[0]) - (2.0f * _data[2] * _data[2]);
	m[6] = (2.0f * _data[1] * _data[2]) - (2.0f * _data[0] * _data[3]);
	m[7] = 0.0f;
	m[8] = (2.0f * _data[0] * _data[2]) - (2.0f * _data[1] * _data[3]);
	m[9] = (2.0f * _data[1] * _data[2]) + (2.0f * _data[0] * _data[3]);
	m[10] = 1.0f - (2.0f * _data[0] * _data[0]) - (2.0f * _data[1] * _data[1]);
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;

	return m;
}

AFloat& AnimaQuaternion::operator[](ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < QUATERNION_SIZE);
	return _data[index];
}

const AFloat& AnimaQuaternion::operator[](ASizeT index) const
{
	ANIMA_ASSERT(index >= 0 && index < QUATERNION_SIZE);
	return const_cast<AFloat&>(_data[index]);
}

AnimaQuaternion& AnimaQuaternion::operator+=(const AnimaQuaternion& v)
{
	for (ASizeT i = 0; i < QUATERNION_SIZE; i++)
		_data[i] += v[i];
	return *this;
}

AnimaQuaternion& AnimaQuaternion::operator-=(const AnimaQuaternion& v)
{
	for (ASizeT i = 0; i < QUATERNION_SIZE; i++)
		_data[i] -= v[i];
	return *this;
}

AnimaQuaternion& AnimaQuaternion::operator*=(const AFloat& mul)
{
	for (ASizeT i = 0; i < QUATERNION_SIZE; i++)
		_data[i] *= mul;
	return *this;
}

AnimaQuaternion& AnimaQuaternion::operator/=(const AFloat& div)
{
	for (ASizeT i = 0; i < QUATERNION_SIZE; i++)
		_data[i] /= div;
	return *this;
}

AnimaQuaternion::operator AFloat*()
{
	return _data;
}

AnimaQuaternion::operator const AFloat*()
{
	return const_cast<AFloat*>(_data);
}

void AnimaQuaternion::SetData(AFloat data[QUATERNION_SIZE])
{
	memcpy(_data, data, sizeof(AFloat) * QUATERNION_SIZE);
}

AFloat* AnimaQuaternion::GetData() const
{
	return _data;
}

void AnimaQuaternion::CopyData(AFloat* d) const
{
	memcpy(d, _data, sizeof(AFloat) * QUATERNION_SIZE);
}

const AFloat* AnimaQuaternion::GetConstData() const
{
	return const_cast<AFloat*>(_data);
}

AnimaEngine* AnimaQuaternion::GetEngine() const
{
	return _engine;
}

AFloat AnimaQuaternion::Length() const
{
	AFloat rv = 0;
	for (int i = 0; i < QUATERNION_SIZE; i++)
		rv += _data[i] * _data[i];

	return (AFloat)sqrt(rv);
}

void AnimaQuaternion::Normalize()
{
	AFloat len = Length();
	for (int i = 0; i < QUATERNION_SIZE; i++)
		_data[i] /= len;
}

bool AnimaQuaternion::IsNull() const
{
	for (ASizeT i = 0; i < QUATERNION_SIZE; i++)
	{
		if (_data[i] != 0.0f)
			return false;
	}
	return true;
}


void AnimaQuaternion::SetData(AFloat value, ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < QUATERNION_SIZE);
	_data[index] = value;
}

void AnimaQuaternion::SetData(AFloat x, AFloat y, AFloat z, AFloat w)
{
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
}

void AnimaQuaternion::FromAxisAndAngle(const AnimaVertex3f& axis, AFloat angle)
{
	float sinHalfAngle = sinf(angle / 2.0f);
	float cosHalfAngle = cosf(angle / 2.0f);

	_data[0] = axis[0] * sinHalfAngle;
	_data[1] = axis[1] * sinHalfAngle;
	_data[2] = axis[2] * sinHalfAngle;
	_data[3] = cosHalfAngle;
}

void AnimaQuaternion::FromAxisAndAngleDeg(const AnimaVertex3f& axis, AFloat angle)
{
	FromAxisAndAngle(axis, angle * (AFloat)M_PI / 180.0f);
}

void AnimaQuaternion::FromHeadPitchRoll(AFloat head, AFloat pitch, AFloat roll)
{
	AnimaMatrix m(_engine);
	m.FromHeadPitchRoll(head, pitch, roll);
	FromMatrix(m);
}

void AnimaQuaternion::FromHeadPitchRollDeg(AFloat head, AFloat pitch, AFloat roll)
{
	FromHeadPitchRoll(head * (AFloat)M_PI / 180.0f, pitch * (AFloat)M_PI / 180.0f, roll * (AFloat)M_PI / 180.0f);
}

void AnimaQuaternion::FromMatrix(const AnimaMatrix& matrix)
{
	float s = 0.0f;
	float q[4] = { 0.0f };
	float trace = matrix[0] + matrix[5] + matrix[10];

	if (trace > 0.0f)
	{
		s = sqrtf(trace + 1.0f);
		q[3] = s * 0.5f;
		s = 0.5f / s;
		q[0] = (matrix[6] - matrix[9]) * s;
		q[1] = (matrix[8] - matrix[2]) * s;
		q[2] = (matrix[1] - matrix[4]) * s;
	}
	else
	{
		int nxt[3] = { 1, 2, 0 };
		int i = 0, j = 0, k = 0;

		if (matrix[5] > matrix[0])
			i = 1;

		if (matrix[10] > matrix[4 * i + i])
			i = 2;

		j = nxt[i];
		k = nxt[j];
		s = sqrtf((matrix[4 * i + i] - (matrix[4 * j + j] + matrix[4 * k + k])) + 1.0f);

		q[i] = s * 0.5f;
		s = 0.5f / s;
		q[3] = (matrix[4 * j + k] - matrix[4 * k + j]) * s;
		q[j] = (matrix[4 * i + j] + matrix[4 * j + i]) * s;
		q[k] = (matrix[4 * i + k] + matrix[4 * k + i]) * s;
	}

	_data[0] = q[0];
	_data[1] = q[1];
	_data[2] = q[2];
	_data[3] = q[3];
}

void AnimaQuaternion::SetIdentity()
{
	_data[0] = 0.0f;
	_data[1] = 0.0f;
	_data[2] = 0.0f;
	_data[3] = 1.0f;
}

void AnimaQuaternion::GetHeadPitchRoll(AFloat& head, AFloat& pitch, AFloat& roll) const
{
	AnimaMatrix m = GetMatrix();
	m.GetHeadPitchRoll(head, pitch, roll);
}

void AnimaQuaternion::GetHeadPitchRollDeg(AFloat& head, AFloat& pitch, AFloat& roll) const
{
	GetHeadPitchRoll(head, pitch, roll);
	head = head * 180.0f / (AFloat)M_PI;
	pitch = pitch * 180.0f / (AFloat)M_PI;
	roll = roll * 180.0f / (AFloat)M_PI;
}

void AnimaQuaternion::GetAxisAngle(AnimaVertex3f& axis, AFloat angle) const
{
	float sinHalfThetaSq = 1.0f - _data[3] * _data[3];

	if (sinHalfThetaSq <= 0.0f)
	{
		axis[0] = 1.0f;
		axis[1] = 0.0f;
		axis[2] = 0.0f;
		angle = 0.0f;
	}
	else
	{
		float invSinHalfTheta = 1.0f / sqrtf(sinHalfThetaSq);

		axis[0] = _data[0] * invSinHalfTheta;
		axis[1] = _data[1] * invSinHalfTheta;
		axis[2] = _data[2] * invSinHalfTheta;
		angle = 2.0f * acosf(_data[3]);
	}
}

void AnimaQuaternion::GetAxisAngleDeg(AnimaVertex3f& axis, AFloat angle) const
{
	GetAxisAngle(axis, angle);
	angle = angle * 180.0f / (AFloat)M_PI;
}

END_ANIMA_ENGINE_NAMESPACE