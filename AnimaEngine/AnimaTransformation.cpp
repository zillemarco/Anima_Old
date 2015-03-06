//
//  AnimaTransformation.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaTransformation.h"
#include "AnimaMath.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaTransformation::AnimaTransformation()
{
	_translation.x = 0.0f;	_translation.y = 0.0f;	_translation.z = 0.0f;
	_rotation.x = 0.0f;		_rotation.y = 0.0f;		_rotation.z = 0.0f;
	_scale.x = 1.0f;		_scale.y = 1.0f;		_scale.z = 1.0f;

	_transformationMatrix.SetIdentity();
	_initialTransformationMatrix.SetIdentity();
}

AnimaTransformation::AnimaTransformation(const AnimaMatrix& initialTransformationMatrix)
	: _transformationMatrix(initialTransformationMatrix)
	, _initialTransformationMatrix(initialTransformationMatrix)
{
	_translation.x = 0.0f;	_translation.y = 0.0f;	_translation.z = 0.0f;
	_rotation.x = 0.0f;		_rotation.y = 0.0f;		_rotation.z = 0.0f;
	_scale.x = 1.0f;		_scale.y = 1.0f;		_scale.z = 1.0f;
}

AnimaTransformation::AnimaTransformation(const AnimaVertex3f& t, const AnimaVertex3f& r, const AnimaVertex3f& s)
	: _translation(t)
	, _rotation(r)
	, _scale(s)
{
	_initialTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(AFloat tx, AFloat ty, AFloat tz, AFloat rx, AFloat ry, AFloat rz, AFloat sx, AFloat sy, AFloat sz)
	: _translation(tx, ty, tz)
	, _rotation(rx, ry, rz)
	, _scale(sx, sy, sz)
{
	_initialTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(const AnimaTransformation& src)
	: _translation(src._translation)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _initialTransformationMatrix(src._initialTransformationMatrix)
{
}

AnimaTransformation::AnimaTransformation(AnimaTransformation&& src)
	: _translation(src._translation)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _initialTransformationMatrix(src._initialTransformationMatrix)
{
}

AnimaTransformation::~AnimaTransformation()
{
}

AnimaTransformation& AnimaTransformation::operator=(const AnimaTransformation& src)
{
	if (this != &src)
	{
		_translation = src._translation;
		_rotation = src._rotation;
		_scale = src._scale;
		_transformationMatrix = src._transformationMatrix;
		_initialTransformationMatrix = src._initialTransformationMatrix;
	}
	
	return *this;
}

AnimaTransformation& AnimaTransformation::operator=(AnimaTransformation&& src)
{
	if (this != &src)
	{
		_translation = src._translation;
		_rotation = src._rotation;
		_scale = src._scale;
		_transformationMatrix = src._transformationMatrix;
		_initialTransformationMatrix = src._initialTransformationMatrix;
	}
	
	return *this;
}

void AnimaTransformation::Translate(const AnimaVertex3f& t)
{
	_translation += t;
	UpdateMatrix();
}

void AnimaTransformation::Translate(AFloat tx, AFloat ty, AFloat tz)
{
	_translation.x += tx;
	_translation.y += ty;
	_translation.z += tz;

	UpdateMatrix();
}

void AnimaTransformation::TranslateX(AFloat tx)
{
	_translation.x += tx;
	UpdateMatrix();
}

void AnimaTransformation::TranslateY(AFloat ty)
{
	_translation.y += ty;
	UpdateMatrix();
}

void AnimaTransformation::TranslateZ(AFloat tz)
{
	_translation.z += tz;
	UpdateMatrix();
}

void AnimaTransformation::SetTranslation(const AnimaVertex3f& t)
{
	_translation = t;
	UpdateMatrix();
}

void AnimaTransformation::SetTranslation(AFloat tx, AFloat ty, AFloat tz)
{
	_translation.x = tx;
	_translation.y = ty;
	_translation.z = tz;
	UpdateMatrix();
}

void AnimaTransformation::SetTranslationX(AFloat tx)
{
	_translation.x = tx;
	UpdateMatrix();
}

void AnimaTransformation::SetTranslationY(AFloat ty)
{
	_translation.y = ty;
	UpdateMatrix();
}

void AnimaTransformation::SetTranslationZ(AFloat tz)
{
	_translation.z = tz;
	UpdateMatrix();
}

AnimaVertex3f AnimaTransformation::GetTranslation()
{
	return _translation;
}

AFloat AnimaTransformation::GetTranslationX()
{
	return _translation.x;
}

AFloat AnimaTransformation::GetTranslationY()
{
	return _translation.y;
}

AFloat AnimaTransformation::GetTranslationZ()
{
	return _translation.z;
}

void AnimaTransformation::Rotate(const AnimaVertex3f& t)
{
	_rotation += t;
	UpdateMatrix();
}

void AnimaTransformation::Rotate(AFloat tx, AFloat ty, AFloat tz)
{
	_rotation.x += tx;
	_rotation.y += ty;
	_rotation.z += tz;
	UpdateMatrix();
}

void AnimaTransformation::RotateX(AFloat tx)
{
	_rotation.x += tx;
	UpdateMatrix();
}

void AnimaTransformation::RotateY(AFloat ty)
{
	_rotation.y += ty;
	UpdateMatrix();
}

void AnimaTransformation::RotateZ(AFloat tz)
{
	_rotation.z += tz;
	UpdateMatrix();
}

void AnimaTransformation::SetRotation(const AnimaVertex3f& t)
{
	_rotation = t;
	UpdateMatrix();
}

void AnimaTransformation::SetRotation(AFloat tx, AFloat ty, AFloat tz)
{
	_rotation.x = tx;
	_rotation.y = ty;
	_rotation.z = tz;
	UpdateMatrix();
}

void AnimaTransformation::SetRotationX(AFloat tx)
{
	_rotation.x = tx;
	UpdateMatrix();
}

void AnimaTransformation::SetRotationY(AFloat ty)
{
	_rotation.y = ty;
	UpdateMatrix();
}

void AnimaTransformation::SetRotationZ(AFloat tz)
{
	_rotation.z = tz;
	UpdateMatrix();
}

void AnimaTransformation::RotateDeg(const AnimaVertex3f& r)
{
	Rotate(r * ((AFloat)M_PI / 180.0f));
}

void AnimaTransformation::RotateDeg(AFloat rx, AFloat ry, AFloat rz)
{
	rx = rx * (AFloat)M_PI / 180.0f;
	ry = ry * (AFloat)M_PI / 180.0f;
	rz = rz * (AFloat)M_PI / 180.0f;
	Rotate(rx, ry, rz);
}

void AnimaTransformation::RotateXDeg(AFloat rx)
{
	rx = rx * (AFloat)M_PI / 180.0f;
	RotateX(rx);
}

void AnimaTransformation::RotateYDeg(AFloat ry)
{
	ry = ry * (AFloat)M_PI / 180.0f;
	RotateY(ry);
}

void AnimaTransformation::RotateZDeg(AFloat rz)
{
	rz = rz * (AFloat)M_PI / 180.0f;
	RotateZ(rz);
}

void AnimaTransformation::SetRotationDeg(const AnimaVertex3f& r)
{
	SetRotation(r * ((AFloat)M_PI / 180.0f));
}

void AnimaTransformation::SetRotationDeg(AFloat rx, AFloat ry, AFloat rz)
{
	rx = rx * (AFloat)M_PI / 180.0f;
	ry = ry * (AFloat)M_PI / 180.0f;
	rz = rz * (AFloat)M_PI / 180.0f;
	SetRotation(rx, ry, rz);
}

void AnimaTransformation::SetRotationXDeg(AFloat rx)
{
	rx = rx * (AFloat)M_PI / 180.0f;
	SetRotationX(rx);
}

void AnimaTransformation::SetRotationYDeg(AFloat ry)
{
	ry = ry * (AFloat)M_PI / 180.0f;
	SetRotationY(ry);
}

void AnimaTransformation::SetRotationZDeg(AFloat rz)
{
	rz = rz * (AFloat)M_PI / 180.0f;
	SetRotationZ(rz);
}

AnimaVertex3f AnimaTransformation::GetRotation()
{
	return _rotation;
}

AFloat AnimaTransformation::GetRotationX()
{
	return _rotation.x;
}

AFloat AnimaTransformation::GetRotationY()
{
	return _rotation.y;
}

AFloat AnimaTransformation::GetRotationZ()
{
	return _rotation.z;
}

void AnimaTransformation::Scale(const AnimaVertex3f& t)
{
	_scale += t;
	UpdateMatrix();
}

void AnimaTransformation::Scale(AFloat tx, AFloat ty, AFloat tz)
{
	_scale.x += tx;
	_scale.y += ty;
	_scale.z += tz;
	UpdateMatrix();
}

void AnimaTransformation::ScaleX(AFloat tx)
{
	_scale.x += tx;
	UpdateMatrix();
}

void AnimaTransformation::ScaleY(AFloat ty)
{
	_scale.y += ty;
	UpdateMatrix();
}

void AnimaTransformation::ScaleZ(AFloat tz)
{
	_scale.z += tz;
	UpdateMatrix();
}

void AnimaTransformation::SetScale(const AnimaVertex3f& t)
{
	_scale = t;
	UpdateMatrix();
}

void AnimaTransformation::SetScale(AFloat tx, AFloat ty, AFloat tz)
{
	_scale.x = tx;
	_scale.y = ty;
	_scale.z = tz;
	UpdateMatrix();
}

void AnimaTransformation::SetScaleX(AFloat tx)
{
	_scale.x = tx;
	UpdateMatrix();
}

void AnimaTransformation::SetScaleY(AFloat ty)
{
	_scale.y = ty;
	UpdateMatrix();
}

void AnimaTransformation::SetScaleZ(AFloat tz)
{
	_scale.z = tz;
	UpdateMatrix();
}

AnimaVertex3f AnimaTransformation::GetScale()
{
	return _scale;
}

AFloat AnimaTransformation::GetScaleX()
{
	return _scale.x;
}

AFloat AnimaTransformation::GetScaleY()
{
	return _scale.y;
}

AFloat AnimaTransformation::GetScaleZ()
{
	return _scale.z;
}

void AnimaTransformation::UpdateMatrix()
{
	AnimaMatrix translationMatrix = AnimaMatrix::MakeTranslation(_translation.x, _translation.y, _translation.z);
	AnimaMatrix rotationMatrix = (AnimaQuaternion(AnimaVertex3f(1.0, 0.0, 0.0), _rotation.x) * AnimaQuaternion(AnimaVertex3f(0.0, 1.0, 0.0), _rotation.y) * AnimaQuaternion(AnimaVertex3f(0.0, 0.0, 1.0), _rotation.z)).GetMatrix();
	AnimaMatrix scaleMatrix = AnimaMatrix::MakeScale(_scale.x, _scale.y, _scale.z, 1.0f);

	_transformationMatrix = rotationMatrix * translationMatrix * scaleMatrix * _initialTransformationMatrix;
}

void AnimaTransformation::SetTransformationMatrix(const AnimaMatrix& m)
{
	_initialTransformationMatrix = m;
	_transformationMatrix = m;
}

void AnimaTransformation::SetTransformationMatrix(AFloat m[16])
{
	_initialTransformationMatrix.Fill(m);
	_transformationMatrix.Fill(m);
}

AnimaMatrix AnimaTransformation::GetTransformationMatrix()
{
	return _transformationMatrix;
}

AnimaMatrix* AnimaTransformation::GetPTransformationMatrix()
{
	return &_transformationMatrix;
}

END_ANIMA_ENGINE_NAMESPACE
