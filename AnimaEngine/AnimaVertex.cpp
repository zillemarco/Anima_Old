#include "AnimaVertex.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#ifndef min
#	define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#	define max(a,b) (a > b ? a : b)
#endif

//---------------------------------------------------------------------------------------
// AnimaVertex2f
//---------------------------------------------------------------------------------------

AnimaVertex2f::AnimaVertex2f()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
}

AnimaVertex2f::AnimaVertex2f(AFloat val)
{
	vec[0] = val;
	vec[1] = val;
}

AnimaVertex2f::AnimaVertex2f(AFloat v[2])
{
	vec[0] = v[0];
	vec[1] = v[1];
}

AnimaVertex2f::AnimaVertex2f(AFloat x, AFloat y)
{
	vec[0] = x;
	vec[1] = y;
}

AnimaVertex2f::AnimaVertex2f(const AnimaVertex2f& src)
{
	if (this != &src)
	{
		this->vec[0] = src.vec[0];
		this->vec[1] = src.vec[1];
	}
}

AnimaVertex2f::AnimaVertex2f(AnimaVertex2f&& src)
{
	if (this != &src)
	{
		this->vec[0] = src.vec[0];
		this->vec[1] = src.vec[1];
	}
}

AnimaVertex2f::AnimaVertex2f(const AnimaVertex3f& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
}

AnimaVertex2f::AnimaVertex2f(const AnimaVertex4f& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
}

AnimaVertex2f::~AnimaVertex2f()
{
}

AnimaVertex2f& AnimaVertex2f::operator=(const AnimaVertex2f& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	return *this;
}

AnimaVertex2f& AnimaVertex2f::operator=(AnimaVertex2f&& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	return *this;
}

bool AnimaVertex2f::operator==(const AnimaVertex2f& param) const
{
	return (vec[0] == param.vec[0] && vec[1] == param.vec[1]);
}

bool AnimaVertex2f::operator!=(const AnimaVertex2f& param) const
{
	return (vec[0] != param.vec[0] || vec[1] != param.vec[1]);
}

bool AnimaVertex2f::IsNull() const
{
	return (vec[0] == 0.0f && vec[1] == 0.0f);
}

AnimaVertex2f AnimaVertex2f::operator-() const
{
	return AnimaVertex2f(-vec[0], -vec[1]);
}

void AnimaVertex2f::operator+=(const AFloat val)
{
	this->vec[0] += val;
	this->vec[1] += val;
}

void AnimaVertex2f::operator+=(const AnimaVertex2f& val)
{
	this->vec[0] += val.vec[0];
	this->vec[1] += val.vec[1];
}

void AnimaVertex2f::operator-=(const AFloat val)
{
	this->vec[0] -= val;
	this->vec[1] -= val;
}

void AnimaVertex2f::operator-=(const AnimaVertex2f& val)
{
	this->vec[0] -= val.vec[0];
	this->vec[1] -= val.vec[1];
}

void AnimaVertex2f::operator*=(const AFloat val)
{
	this->vec[0] *= val;
	this->vec[1] *= val;
}

void AnimaVertex2f::operator*=(const AnimaVertex2f& val)
{
	this->vec[0] *= val.vec[0];
	this->vec[1] *= val.vec[1];
}

void AnimaVertex2f::operator/=(const AFloat val)
{
	float ftemp = 1.0f / val;
	this->vec[0] *= ftemp;
	this->vec[1] *= ftemp;
}

void AnimaVertex2f::operator/=(const AnimaVertex2f& val)
{
	this->vec[0] /= val.vec[0];
	this->vec[1] /= val.vec[1];
}

void AnimaVertex2f::Normalize()
{
	float invlen = 1.0f / sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	vec[0] *= invlen;
	vec[1] *= invlen;
}

AnimaVertex2f AnimaVertex2f::Normalized() const
{
	float invlen = 1.0f / sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	AnimaVertex2f result(*this);
	result *= invlen;
	return result;
}

void AnimaVertex2f::Clamp(const AFloat lower, const AFloat upper)
{
	this->vec[0] = this->vec[0] > upper ? upper : this->vec[0];
	this->vec[1] = this->vec[1] > upper ? upper : this->vec[1];

	this->vec[0] = this->vec[0] < lower ? lower : this->vec[0];
	this->vec[1] = this->vec[1] < lower ? lower : this->vec[1];
}

void AnimaVertex2f::Clamp(const AnimaVertex2f& lower, const AnimaVertex2f& upper)
{
	this->vec[0] = min(upper.vec[0], max(lower.vec[0], this->vec[0]));
	this->vec[1] = min(upper.vec[1], max(lower.vec[1], this->vec[1]));
}

AnimaVertex2f AnimaVertex2f::Clamped(const AFloat lower, const AFloat upper) const
{
	AnimaVertex2f result(*this);
	result.vec[0] = result.vec[0] > upper ? upper : result.vec[0];
	result.vec[1] = result.vec[1] > upper ? upper : result.vec[1];

	result.vec[0] = result.vec[0] < lower ? lower : result.vec[0];
	result.vec[1] = result.vec[1] < lower ? lower : result.vec[1];

	return result;
}

AnimaVertex2f AnimaVertex2f::Clamped(const AnimaVertex2f& lower, const AnimaVertex2f& upper) const
{
	AnimaVertex2f result(*this);
	result.vec[0] = min(upper.vec[0], max(lower.vec[0], result.vec[0]));
	result.vec[1] = min(upper.vec[1], max(lower.vec[1], result.vec[1]));

	return result;
}

AFloat AnimaVertex2f::Length() const
{
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
}

AFloat AnimaVertex2f::Length2() const
{
	return vec[0] * vec[0] + vec[1] * vec[1];
}

//---------------------------------------------------------------------------------------
// AnimaVertex3f
//---------------------------------------------------------------------------------------

AnimaVertex3f::AnimaVertex3f()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
}

AnimaVertex3f::AnimaVertex3f(AFloat val)
{
	vec[0] = val;
	vec[1] = val;
	vec[2] = val;
}

AnimaVertex3f::AnimaVertex3f(AFloat v[3])
{
	vec[0] = v[0];
	vec[1] = v[1];
	vec[2] = v[3];
}

AnimaVertex3f::AnimaVertex3f(AFloat x, AFloat y, AFloat z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

AnimaVertex3f::AnimaVertex3f(const AnimaVertex2f& src, const AFloat z)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = z;
}

AnimaVertex3f::AnimaVertex3f(const AnimaVertex3f& src)
{
	if (this != &src)
	{
		this->vec[0] = src.vec[0];
		this->vec[1] = src.vec[1];
		this->vec[2] = src.vec[2];
	}
}

AnimaVertex3f::AnimaVertex3f(AnimaVertex3f&& src)
{
	if (this != &src)
	{
		this->vec[0] = src.vec[0];
		this->vec[1] = src.vec[1];
		this->vec[2] = src.vec[2];
	}
}

AnimaVertex3f::AnimaVertex3f(const AnimaVertex4f& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = src.vec[2];
}

AnimaVertex3f::~AnimaVertex3f()
{
}

AnimaVertex3f& AnimaVertex3f::operator=(const AnimaVertex3f& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = src.vec[2];
	return *this;
}

AnimaVertex3f& AnimaVertex3f::operator=(AnimaVertex3f&& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = src.vec[2];
	return *this;
}

bool AnimaVertex3f::operator==(const AnimaVertex3f& param) const
{
	return (vec[0] == param.vec[0] && vec[1] == param.vec[1] && vec[2] == param.vec[2]);
}

bool AnimaVertex3f::operator!=(const AnimaVertex3f& param) const
{
	return (vec[0] != param.vec[0] || vec[1] != param.vec[1] || vec[2] != param.vec[2]);
}

bool AnimaVertex3f::IsNull() const
{
	return (vec[0] == 0.0f && vec[1] == 0.0f && vec[2] == 0.0f);
}

AnimaVertex3f AnimaVertex3f::operator-() const
{
	return AnimaVertex3f(-vec[0], -vec[1], -vec[2]);
}

void AnimaVertex3f::operator+=(const AFloat val)
{
	this->vec[0] += val;
	this->vec[1] += val;
	this->vec[2] += val;
}

void AnimaVertex3f::operator+=(const AnimaVertex3f& val)
{
	this->vec[0] += val.vec[0];
	this->vec[1] += val.vec[1];
	this->vec[2] += val.vec[2];
}

void AnimaVertex3f::operator-=(const AFloat val)
{
	this->vec[0] -= val;
	this->vec[1] -= val;
	this->vec[2] -= val;
}

void AnimaVertex3f::operator-=(const AnimaVertex3f& val)
{
	this->vec[0] -= val.vec[0];
	this->vec[1] -= val.vec[1];
	this->vec[2] -= val.vec[2];
}

void AnimaVertex3f::operator*=(const AFloat val)
{
	this->vec[0] *= val;
	this->vec[1] *= val;
	this->vec[2] *= val;
}

void AnimaVertex3f::operator*=(const AnimaVertex3f& val)
{
	this->vec[0] *= val.vec[0];
	this->vec[1] *= val.vec[1];
	this->vec[2] *= val.vec[2];
}

void AnimaVertex3f::operator/=(const AFloat val)
{
	float ftemp = 1.0f / val;
	this->vec[0] *= ftemp;
	this->vec[1] *= ftemp;
	this->vec[2] *= ftemp;
}

void AnimaVertex3f::operator/=(const AnimaVertex3f& val)
{
	this->vec[0] /= val.vec[0];
	this->vec[1] /= val.vec[1];
	this->vec[2] /= val.vec[2];
}

void AnimaVertex3f::Normalize()
{
	float invlen = 1.0f / sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] *= invlen;
	vec[1] *= invlen;
	vec[2] *= invlen;
}

AnimaVertex3f AnimaVertex3f::Normalized() const
{
	float invlen = 1.0f / sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	AnimaVertex3f result(*this);
	result *= invlen;
	return result;
}

void AnimaVertex3f::Clamp(const AFloat lower, const AFloat upper)
{
	this->vec[0] = this->vec[0] > upper ? upper : this->vec[0];
	this->vec[1] = this->vec[1] > upper ? upper : this->vec[1];
	this->vec[2] = this->vec[2] > upper ? upper : this->vec[2];

	this->vec[0] = this->vec[0] < lower ? lower : this->vec[0];
	this->vec[1] = this->vec[1] < lower ? lower : this->vec[1];
	this->vec[2] = this->vec[2] < lower ? lower : this->vec[2];
}

void AnimaVertex3f::Clamp(const AnimaVertex3f& lower, const AnimaVertex3f& upper)
{
	this->vec[0] = min(upper.vec[0], max(lower.vec[0], this->vec[0]));
	this->vec[1] = min(upper.vec[1], max(lower.vec[1], this->vec[1]));
	this->vec[2] = min(upper.vec[2], max(lower.vec[2], this->vec[2]));
}

AnimaVertex3f AnimaVertex3f::Clamped(const AFloat lower, const AFloat upper) const
{
	AnimaVertex3f result(*this);
	result.vec[0] = result.vec[0] > upper ? upper : result.vec[0];
	result.vec[1] = result.vec[1] > upper ? upper : result.vec[1];
	result.vec[2] = result.vec[2] > upper ? upper : result.vec[2];

	result.vec[0] = result.vec[0] < lower ? lower : result.vec[0];
	result.vec[1] = result.vec[1] < lower ? lower : result.vec[1];
	result.vec[2] = result.vec[2] < lower ? lower : result.vec[2];

	return result;
}

AnimaVertex3f AnimaVertex3f::Clamped(const AnimaVertex3f& lower, const AnimaVertex3f& upper) const
{
	AnimaVertex3f result(*this);
	result.vec[0] = min(upper.vec[0], max(lower.vec[0], result.vec[0]));
	result.vec[1] = min(upper.vec[1], max(lower.vec[1], result.vec[1]));
	result.vec[2] = min(upper.vec[2], max(lower.vec[2], result.vec[2]));

	return result;
}

AFloat AnimaVertex3f::Length() const
{
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

AFloat AnimaVertex3f::Length2() const
{
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
}

//---------------------------------------------------------------------------------------
// AnimaVertex4f
//---------------------------------------------------------------------------------------

AnimaVertex4f::AnimaVertex4f()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
	vec[3] = 0.0f;
}

AnimaVertex4f::AnimaVertex4f(AFloat val)
{
	vec[0] = val;
	vec[1] = val;
	vec[2] = val;
	vec[3] = val;
}

AnimaVertex4f::AnimaVertex4f(AFloat v[3])
{
	vec[0] = v[0];
	vec[1] = v[1];
	vec[2] = v[2];
	vec[3] = v[3];
}

AnimaVertex4f::AnimaVertex4f(AFloat x, AFloat y, AFloat z, AFloat w)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	vec[3] = w;
}

AnimaVertex4f::AnimaVertex4f(const AnimaVertex2f& src, const AFloat z, const AFloat w)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = z;
	this->vec[3] = w;
}

AnimaVertex4f::AnimaVertex4f(const AnimaVertex3f& src, const AFloat w)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = src.vec[2];
	this->vec[3] = w;
}

AnimaVertex4f::AnimaVertex4f(const AnimaVertex4f& src)
{
	if (this != &src)
	{
		this->vec[0] = src.vec[0];
		this->vec[1] = src.vec[1];
		this->vec[2] = src.vec[2];
		this->vec[3] = src.vec[3];
	}
}

AnimaVertex4f::AnimaVertex4f(AnimaVertex4f&& src)
{
	if (this != &src)
	{
		this->vec[0] = src.vec[0];
		this->vec[1] = src.vec[1];
		this->vec[2] = src.vec[2];
		this->vec[3] = src.vec[3];
	}
}

AnimaVertex4f::~AnimaVertex4f()
{
}

AnimaVertex4f& AnimaVertex4f::operator=(const AnimaVertex4f& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = src.vec[2];
	this->vec[3] = src.vec[3];
	return *this;
}

AnimaVertex4f& AnimaVertex4f::operator=(AnimaVertex4f&& src)
{
	this->vec[0] = src.vec[0];
	this->vec[1] = src.vec[1];
	this->vec[2] = src.vec[2];
	this->vec[3] = src.vec[3];
	return *this;
}

bool AnimaVertex4f::operator==(const AnimaVertex4f& param) const
{
	return (vec[0] == param.vec[0] && vec[1] == param.vec[1] && vec[2] == param.vec[2] && vec[3] == param.vec[3]);
}

bool AnimaVertex4f::operator!=(const AnimaVertex4f& param) const
{
	return (vec[0] != param.vec[0] || vec[1] != param.vec[1] || vec[2] != param.vec[2] || vec[3] != param.vec[3]);
}

bool AnimaVertex4f::IsNull() const
{
	return (vec[0] == 0.0f && vec[1] == 0.0f && vec[2] == 0.0f && vec[3] == 0.0f);
}

AnimaVertex4f AnimaVertex4f::operator-() const
{
	return AnimaVertex4f(-vec[0], -vec[1], -vec[2], -vec[3]);
}

void AnimaVertex4f::operator+=(const AFloat val)
{
	this->vec[0] += val;
	this->vec[1] += val;
	this->vec[2] += val;
	this->vec[3] += val;
}

void AnimaVertex4f::operator+=(const AnimaVertex4f& val)
{
	this->vec[0] += val.vec[0];
	this->vec[1] += val.vec[1];
	this->vec[2] += val.vec[2];
	this->vec[3] += val.vec[3];
}

void AnimaVertex4f::operator-=(const AFloat val)
{
	this->vec[0] -= val;
	this->vec[1] -= val;
	this->vec[2] -= val;
	this->vec[3] -= val;
}

void AnimaVertex4f::operator-=(const AnimaVertex4f& val)
{
	this->vec[0] -= val.vec[0];
	this->vec[1] -= val.vec[1];
	this->vec[2] -= val.vec[2];
	this->vec[3] -= val.vec[3];
}

void AnimaVertex4f::operator*=(const AFloat val)
{
	this->vec[0] *= val;
	this->vec[1] *= val;
	this->vec[2] *= val;
	this->vec[3] *= val;
}

void AnimaVertex4f::operator*=(const AnimaVertex4f& val)
{
	this->vec[0] *= val.vec[0];
	this->vec[1] *= val.vec[1];
	this->vec[2] *= val.vec[2];
	this->vec[3] *= val.vec[3];
}

void AnimaVertex4f::operator/=(const AFloat val)
{
	float ftemp = 1.0f / val;
	this->vec[0] *= ftemp;
	this->vec[1] *= ftemp;
	this->vec[2] *= ftemp;
	this->vec[3] *= ftemp;
}

void AnimaVertex4f::operator/=(const AnimaVertex4f& val)
{
	this->vec[0] /= val.vec[0];
	this->vec[1] /= val.vec[1];
	this->vec[2] /= val.vec[2];
	this->vec[3] /= val.vec[3];
}

void AnimaVertex4f::Normalize()
{
	float invlen = 1.0f / sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]);
	vec[0] *= invlen;
	vec[1] *= invlen;
	vec[2] *= invlen;
	vec[3] *= invlen;
}

AnimaVertex4f AnimaVertex4f::Normalized() const
{
	float invlen = 1.0f / sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]);
	AnimaVertex4f result(*this);
	result *= invlen;
	return result;
}

void AnimaVertex4f::Clamp(const AFloat lower, const AFloat upper)
{
	this->vec[0] = this->vec[0] > upper ? upper : this->vec[0];
	this->vec[1] = this->vec[1] > upper ? upper : this->vec[1];
	this->vec[2] = this->vec[2] > upper ? upper : this->vec[2];
	this->vec[3] = this->vec[3] > upper ? upper : this->vec[3];

	this->vec[0] = this->vec[0] < lower ? lower : this->vec[0];
	this->vec[1] = this->vec[1] < lower ? lower : this->vec[1];
	this->vec[2] = this->vec[2] < lower ? lower : this->vec[2];
	this->vec[3] = this->vec[3] < lower ? lower : this->vec[3];
}

void AnimaVertex4f::Clamp(const AnimaVertex4f& lower, const AnimaVertex4f& upper)
{
	this->vec[0] = min(upper.vec[0], max(lower.vec[0], this->vec[0]));
	this->vec[1] = min(upper.vec[1], max(lower.vec[1], this->vec[1]));
	this->vec[2] = min(upper.vec[2], max(lower.vec[2], this->vec[2]));
	this->vec[3] = min(upper.vec[3], max(lower.vec[3], this->vec[3]));
}

AnimaVertex4f AnimaVertex4f::Clamped(const AFloat lower, const AFloat upper) const
{
	AnimaVertex4f result(*this);
	result.vec[0] = result.vec[0] > upper ? upper : result.vec[0];
	result.vec[1] = result.vec[1] > upper ? upper : result.vec[1];
	result.vec[2] = result.vec[2] > upper ? upper : result.vec[2];
	result.vec[3] = result.vec[3] > upper ? upper : result.vec[3];

	result.vec[0] = result.vec[0] < lower ? lower : result.vec[0];
	result.vec[1] = result.vec[1] < lower ? lower : result.vec[1];
	result.vec[2] = result.vec[2] < lower ? lower : result.vec[2];
	result.vec[3] = result.vec[3] < lower ? lower : result.vec[3];

	return result;
}

AnimaVertex4f AnimaVertex4f::Clamped(const AnimaVertex4f& lower, const AnimaVertex4f& upper) const
{
	AnimaVertex4f result(*this);
	result.vec[0] = min(upper.vec[0], max(lower.vec[0], result.vec[0]));
	result.vec[1] = min(upper.vec[1], max(lower.vec[1], result.vec[1]));
	result.vec[2] = min(upper.vec[2], max(lower.vec[2], result.vec[2]));
	result.vec[3] = min(upper.vec[3], max(lower.vec[3], result.vec[3]));

	return result;
}

AFloat AnimaVertex4f::Length() const
{
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3]);
}

AFloat AnimaVertex4f::Length2() const
{
	return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2] + vec[3] * vec[3];
}

AFloat operator * (const AnimaVertex2f &a, const AnimaVertex2f &b)
{
	return Dot(a, b);
}

AnimaVertex2f operator * (const float param, const AnimaVertex2f &v)
{
	return AnimaVertex2f(v.vec[0] * param, v.vec[1] * param);
}

AnimaVertex2f operator * (const AnimaVertex2f &v, const float param)
{
	return AnimaVertex2f(v.vec[0] * param, v.vec[1] * param);
}

AnimaVertex2f operator / (const AnimaVertex2f &a, const AnimaVertex2f &b)
{
	return AnimaVertex2f(a.vec[0] / b.vec[0], a.vec[1] / b.vec[1]);
}

AnimaVertex2f operator / (const float param, const AnimaVertex2f &v)
{
	return AnimaVertex2f(param / v.vec[0], param / v.vec[1]);
}

AnimaVertex2f operator / (const AnimaVertex2f &v, const float param)
{
	return AnimaVertex2f(v.vec[0] / param, v.vec[1] / param);
}

AnimaVertex2f operator - (const AnimaVertex2f &a, const AnimaVertex2f &b)
{
	return AnimaVertex2f(a.vec[0] - b.vec[0], a.vec[1] - b.vec[1]);
}

AnimaVertex2f operator - (const float param, const AnimaVertex2f &v)
{
	return AnimaVertex2f(param - v.vec[0], param - v.vec[1]);
}

AnimaVertex2f operator - (const AnimaVertex2f &v, const float param)
{
	return AnimaVertex2f(v.vec[0] - param, v.vec[1] - param);
}

AnimaVertex2f operator + (const AnimaVertex2f &a, const AnimaVertex2f &b)
{
	return AnimaVertex2f(a.vec[0] + b.vec[0], a.vec[1] + b.vec[1]);
}

AnimaVertex2f operator + (const float param, const AnimaVertex2f &v)
{
	return AnimaVertex2f(param + v.vec[0], param + v.vec[1]);
}

AnimaVertex2f operator + (const AnimaVertex2f &v, const float param)
{
	return AnimaVertex2f(v.vec[0] + param, v.vec[1] + param);
}

AFloat operator * (const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return Dot(a, b);
}

AnimaVertex3f operator * (const float param, const AnimaVertex3f &v)
{
	return AnimaVertex3f(v.vec[0] * param, v.vec[1] * param, v.vec[2] * param);
}

AnimaVertex3f operator * (const AnimaVertex3f &v, const float param)
{
	return AnimaVertex3f(v.vec[0] * param, v.vec[1] * param, v.vec[2] * param);
}

AnimaVertex3f operator / (const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return AnimaVertex3f(a.vec[0] / b.vec[0], a.vec[1] / b.vec[1], a.vec[2] / b.vec[2]);
}

AnimaVertex3f operator / (const float param, const AnimaVertex3f &v)
{
	return AnimaVertex3f(param / v.vec[0], param / v.vec[1], param / v.vec[2]);
}

AnimaVertex3f operator / (const AnimaVertex3f &v, const float param)
{
	return AnimaVertex3f(v.vec[0] / param, v.vec[1] / param, v.vec[2] / param);
}

AnimaVertex3f operator - (const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return AnimaVertex3f(a.vec[0] - b.vec[0], a.vec[1] - b.vec[1], a.vec[2] - b.vec[2]);
}

AnimaVertex3f operator - (const float param, const AnimaVertex3f &v)
{
	return AnimaVertex3f(param - v.vec[0], param - v.vec[1], param - v.vec[2]);
}

AnimaVertex3f operator - (const AnimaVertex3f &v, const float param)
{
	return AnimaVertex3f(v.vec[0] - param, v.vec[1] - param, v.vec[2] - param);
}

AnimaVertex3f operator + (const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return AnimaVertex3f(a.vec[0] + b.vec[0], a.vec[1] + b.vec[1], a.vec[2] + b.vec[2]);
}

AnimaVertex3f operator + (const float param, const AnimaVertex3f &v)
{
	return AnimaVertex3f(param + v.vec[0], param + v.vec[1], param + v.vec[2]);
}

AnimaVertex3f operator + (const AnimaVertex3f &v, const float param)
{
	return AnimaVertex3f(v.vec[0] + param, v.vec[1] + param, v.vec[2] + param);
}

AnimaVertex3f operator ^ (const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return Cross(a, b);
}

AFloat operator * (const AnimaVertex4f &a, const AnimaVertex4f &b)
{
	return Dot(a, b);
}

AnimaVertex4f operator * (const float param, const AnimaVertex4f &v)
{
	return AnimaVertex4f(v.vec[0] * param, v.vec[1] * param, v.vec[2] * param, v.vec[3] * param);
}

AnimaVertex4f operator * (const AnimaVertex4f &v, const float param)
{
	return AnimaVertex4f(v.vec[0] * param, v.vec[1] * param, v.vec[2] * param, v.vec[3] * param);
}

AnimaVertex4f operator / (const AnimaVertex4f &a, const AnimaVertex4f &b)
{
	return AnimaVertex4f(a.vec[0] / b.vec[0], a.vec[1] / b.vec[1], a.vec[2] / b.vec[2], a.vec[3] / b.vec[3]);
}

AnimaVertex4f operator / (const float param, const AnimaVertex4f &v)
{
	return AnimaVertex4f(param / v.vec[0], param / v.vec[1], param / v.vec[2], param / v.vec[3]);
}

AnimaVertex4f operator / (const AnimaVertex4f &v, const float param)
{
	return AnimaVertex4f(v.vec[0] / param, v.vec[1] / param, v.vec[2] / param, v.vec[3] / param);
}

AnimaVertex4f operator - (const AnimaVertex4f &a, const AnimaVertex4f &b)
{
	return AnimaVertex4f(a.vec[0] - b.vec[0], a.vec[1] - b.vec[1], a.vec[2] - b.vec[2], a.vec[3] - b.vec[3]);
}

AnimaVertex4f operator - (const float param, const AnimaVertex4f &v)
{
	return AnimaVertex4f(param - v.vec[0], param - v.vec[1], param - v.vec[2], param - v.vec[3]);
}

AnimaVertex4f operator - (const AnimaVertex4f &v, const float param)
{
	return AnimaVertex4f(v.vec[0] - param, v.vec[1] - param, v.vec[2] - param, v.vec[3] - param);
}

AnimaVertex4f operator + (const AnimaVertex4f &a, const AnimaVertex4f &b)
{
	return AnimaVertex4f(a.vec[0] + b.vec[0], a.vec[1] + b.vec[1], a.vec[2] + b.vec[2], a.vec[3] + b.vec[3]);
}

AnimaVertex4f operator + (const float param, const AnimaVertex4f &v)
{
	return AnimaVertex4f(param + v.vec[0], param + v.vec[1], param + v.vec[2], param + v.vec[3]);
}

AnimaVertex4f operator + (const AnimaVertex4f &v, const float param)
{
	return AnimaVertex4f(v.vec[0] + param, v.vec[1] + param, v.vec[2] + param, v.vec[3] + param);
}

float Dot(const AnimaVertex2f &a, const AnimaVertex2f &b)
{
	return a.vec[0] * b.vec[0] + a.vec[1] * b.vec[1];
}

float Dot(const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return a.vec[0] * b.vec[0] + a.vec[1] * b.vec[1] + a.vec[2] * b.vec[2];
}

float Dot(const AnimaVertex4f &a, const AnimaVertex4f &b)
{
	return a.vec[0] * b.vec[0] + a.vec[1] * b.vec[1] + a.vec[2] * b.vec[2] + a.vec[3] * b.vec[3];
}

AnimaVertex3f Cross(const AnimaVertex3f &a, const AnimaVertex3f &b)
{
	return AnimaVertex3f(	a.vec[1] * b.vec[2] - a.vec[2] * b.vec[1],
							a.vec[2] * b.vec[0] - a.vec[0] * b.vec[2],
							a.vec[0] * b.vec[1] - a.vec[1] * b.vec[0]);
}

//AnimaVertex4f AnimaMath::Cross(const AnimaVertex4f& v1, const AnimaVertex4f& v2)
//{
//	AFloat srcv1[4], srcv2[4];
//	v1.CopyData(srcv1);
//	v2.CopyData(srcv2);
//
//	srcv1[3] = 0.0f;
//	srcv2[3] = 0.0f;
//
//	__m128 a = _mm_loadu_ps(srcv1);
//	__m128 b = _mm_loadu_ps(srcv2);
//
//	__m128 res = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 1, 0, 2))),
//		_mm_mul_ps(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1))));
//
//	float* r = new float[4];
//	_mm_storeu_ps(r, res);
//
//	AnimaVertex4f result(v1.GetEngine(), r);
//	delete[] r;
//	return result;
//}

END_ANIMA_ENGINE_NAMESPACE
