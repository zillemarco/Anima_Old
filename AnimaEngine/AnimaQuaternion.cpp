#include "AnimaQuaternion.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine)
	: AnimaVertex4f(engine)
{
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, AFloat data[4])
	: AnimaVertex4f(engine, data)
{
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w)
	: AnimaVertex4f(engine)
{
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
}

AnimaQuaternion::AnimaQuaternion(AnimaEngine* engine, const AnimaVertex3f& axis, AFloat angle)
	: AnimaVertex4f(engine)
{
	float sinHalfAngle = sinf(angle / 2.0f);
	float cosHalfAngle = cosf(angle / 2.0f);
	
	_data[0] = axis[0] * sinHalfAngle;
	_data[1] = axis[1] * sinHalfAngle;
	_data[2] = axis[2] * sinHalfAngle;
	_data[3] = cosHalfAngle;
}

AnimaQuaternion::AnimaQuaternion(const AnimaQuaternion& src)
	: AnimaVertex4f(src)
{
}

AnimaQuaternion::AnimaQuaternion(AnimaQuaternion&& src)
	: AnimaVertex4f(src)
{
}

AnimaQuaternion::~AnimaQuaternion()
{
}

AnimaQuaternion AnimaQuaternion::Conjugate()
{
	AnimaQuaternion conj(_engine, -_data[0], -_data[1], -_data[2], _data[3]);
	return conj;
}

AnimaMatrix AnimaQuaternion::GetMatrix()
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

END_ANIMA_ENGINE_NAMESPACE