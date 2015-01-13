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
	AnimaQuaternion conj(_engine, -_data[0], -_data[1], -_data[2], -_data[3]);
	return conj;
}

END_ANIMA_ENGINE_NAMESPACE