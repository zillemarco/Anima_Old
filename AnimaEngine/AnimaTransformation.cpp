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

AnimaTransformation::AnimaTransformation(AnimaEngine* engine)
	: _translation(engine)
	, _rotation(engine)
	, _scale(engine)
	, _transformationMatrix(engine)
	, _initialTransformationMatrix(engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_translation[0] = 0.0f;	_translation[1] = 0.0f;	_translation[2] = 0.0f;
	_rotation[0] = 0.0f;	_rotation[1] = 0.0f;	_rotation[2] = 0.0f;
	_scale[0] = 1.0f;		_scale[1] = 1.0f;		_scale[2] = 1.0f;

	_transformationMatrix.SetIdentity();
	_initialTransformationMatrix.SetIdentity();
}

AnimaTransformation::AnimaTransformation(AnimaEngine* engine, const AnimaMatrix& initialTransformationMatrix)
	: _translation(engine)
	, _rotation(engine)
	, _scale(engine)
	, _transformationMatrix(initialTransformationMatrix)
	, _initialTransformationMatrix(initialTransformationMatrix)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_translation[0] = 0.0f;	_translation[1] = 0.0f;	_translation[2] = 0.0f;
	_rotation[0] = 0.0f;	_rotation[1] = 0.0f;	_rotation[2] = 0.0f;
	_scale[0] = 1.0f;		_scale[1] = 1.0f;		_scale[2] = 1.0f;
}

AnimaTransformation::AnimaTransformation(AnimaEngine* engine, const AnimaVertex3f& t, const AnimaVertex3f& r, const AnimaVertex3f& s)
	: _translation(engine)
	, _rotation(engine)
	, _scale(engine)
	, _transformationMatrix(engine)
	, _initialTransformationMatrix(engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_translation[0] = t[0];	_translation[1] = t[1];	_translation[2] = t[2];
	_rotation[0] = r[0];	_rotation[1] = r[1];	_rotation[2] = r[2];
	_scale[0] = s[0];		_scale[1] = s[1];		_scale[2] = s[2];

	_initialTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(AnimaEngine* engine, AFloat tx, AFloat ty, AFloat tz, AFloat rx, AFloat ry, AFloat rz, AFloat sx, AFloat sy, AFloat sz)
	: _translation(engine)
	, _rotation(engine)
	, _scale(engine)
	, _transformationMatrix(engine)
	, _initialTransformationMatrix(engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_translation[0] = tx;	_translation[1] = ty;	_translation[2] = tz;
	_rotation[0] = rx;		_rotation[1] = ry;		_rotation[2] = rz;
	_scale[0] = sx;			_scale[1] = sy;			_scale[2] = sz;

	_initialTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(const AnimaTransformation& src)
	: _translation(src._engine)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _initialTransformationMatrix(src._initialTransformationMatrix)
{
	_engine = src._engine;
}

AnimaTransformation::AnimaTransformation(AnimaTransformation&& src)
	: _translation(src._translation)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _initialTransformationMatrix(src._initialTransformationMatrix)
{
	_engine = src._engine;
}

AnimaTransformation::~AnimaTransformation()
{
}

AnimaTransformation& AnimaTransformation::operator=(const AnimaTransformation& src)
{
	if (this != &src)
	{
		_engine = src._engine;
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
		_engine = src._engine;
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
	_translation[0] += t[0];	
	_translation[1] += t[1];	
	_translation[2] += t[2];

	UpdateMatrix();
}

void AnimaTransformation::Translate(AFloat tx, AFloat ty, AFloat tz)
{
	_translation[0] += tx;
	_translation[1] += ty;
	_translation[2] += tz;

	UpdateMatrix();
}

void AnimaTransformation::TranslateX(AFloat tx)
{
	_translation[0] += tx;

	UpdateMatrix();
}

void AnimaTransformation::TranslateY(AFloat ty)
{
	_translation[1] += ty;

	UpdateMatrix();
}

void AnimaTransformation::TranslateZ(AFloat tz)
{
	_translation[2] += tz;

	UpdateMatrix();
}

void AnimaTransformation::SetTranslation(const AnimaVertex3f& t)
{
	_translation[0] = t[0];
	_translation[1] = t[1];
	_translation[2] = t[2];

	UpdateMatrix();
}

void AnimaTransformation::SetTranslation(AFloat tx, AFloat ty, AFloat tz)
{
	_translation[0] = tx;
	_translation[1] = ty;
	_translation[2] = tz;

	UpdateMatrix();
}

void AnimaTransformation::SetTranslationX(AFloat tx)
{
	_translation[0] = tx;

	UpdateMatrix();
}

void AnimaTransformation::SetTranslationY(AFloat ty)
{
	_translation[1] = ty;

	UpdateMatrix();
}

void AnimaTransformation::SetTranslationZ(AFloat tz)
{
	_translation[2] = tz;

	UpdateMatrix();
}

AnimaVertex3f AnimaTransformation::GetTranslation()
{
	return _translation;
}

AFloat AnimaTransformation::GetTranslationX()
{
	return _translation[0];
}

AFloat AnimaTransformation::GetTranslationY()
{
	return _translation[1];
}

AFloat AnimaTransformation::GetTranslationZ()
{
	return _translation[2];
}

void AnimaTransformation::Rotate(const AnimaVertex3f& t)
{
	_rotation[0] += t[0];
	_rotation[1] += t[1];
	_rotation[2] += t[2];

	UpdateMatrix();
}

void AnimaTransformation::Rotate(AFloat tx, AFloat ty, AFloat tz)
{
	_rotation[0] += tx;
	_rotation[1] += ty;
	_rotation[2] += tz;

	UpdateMatrix();
}

void AnimaTransformation::RotateX(AFloat tx)
{
	_rotation[0] += tx;

	UpdateMatrix();
}

void AnimaTransformation::RotateY(AFloat ty)
{
	_rotation[1] += ty;

	UpdateMatrix();
}

void AnimaTransformation::RotateZ(AFloat tz)
{
	_rotation[2] += tz;

	UpdateMatrix();
}

void AnimaTransformation::SetRotation(const AnimaVertex3f& t)
{
	_rotation[0] = t[0];
	_rotation[1] = t[1];
	_rotation[2] = t[2];

	UpdateMatrix();
}

void AnimaTransformation::SetRotation(AFloat tx, AFloat ty, AFloat tz)
{
	_rotation[0] = tx;
	_rotation[1] = ty;
	_rotation[2] = tz;

	UpdateMatrix();
}

void AnimaTransformation::SetRotationX(AFloat tx)
{
	_rotation[0] = tx;

	UpdateMatrix();
}

void AnimaTransformation::SetRotationY(AFloat ty)
{
	_rotation[1] = ty;

	UpdateMatrix();
}

void AnimaTransformation::SetRotationZ(AFloat tz)
{
	_rotation[2] = tz;

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
	return _rotation[0];
}

AFloat AnimaTransformation::GetRotationY()
{
	return _rotation[1];
}

AFloat AnimaTransformation::GetRotationZ()
{
	return _rotation[2];
}

void AnimaTransformation::Scale(const AnimaVertex3f& t)
{
	_scale[0] += t[0];
	_scale[1] += t[1];
	_scale[2] += t[2];

	UpdateMatrix();
}

void AnimaTransformation::Scale(AFloat tx, AFloat ty, AFloat tz)
{
	_scale[0] += tx;
	_scale[1] += ty;
	_scale[2] += tz;

	UpdateMatrix();
}

void AnimaTransformation::ScaleX(AFloat tx)
{
	_scale[0] += tx;

	UpdateMatrix();
}

void AnimaTransformation::ScaleY(AFloat ty)
{
	_scale[1] += ty;

	UpdateMatrix();
}

void AnimaTransformation::ScaleZ(AFloat tz)
{
	_scale[2] += tz;

	UpdateMatrix();
}

void AnimaTransformation::SetScale(const AnimaVertex3f& t)
{
	_scale[0] = t[0];
	_scale[1] = t[1];
	_scale[2] = t[2];

	UpdateMatrix();
}

void AnimaTransformation::SetScale(AFloat tx, AFloat ty, AFloat tz)
{
	_scale[0] = tx;
	_scale[1] = ty;
	_scale[2] = tz;

	UpdateMatrix();
}

void AnimaTransformation::SetScaleX(AFloat tx)
{
	_scale[0] = tx;

	UpdateMatrix();
}

void AnimaTransformation::SetScaleY(AFloat ty)
{
	_scale[1] = ty;

	UpdateMatrix();
}

void AnimaTransformation::SetScaleZ(AFloat tz)
{
	_scale[2] = tz;

	UpdateMatrix();
}

AnimaVertex3f AnimaTransformation::GetScale()
{
	return _scale;
}

AFloat AnimaTransformation::GetScaleX()
{
	return _scale[0];
}

AFloat AnimaTransformation::GetScaleY()
{
	return _scale[1];
}

AFloat AnimaTransformation::GetScaleZ()
{
	return _scale[2];
}

void AnimaTransformation::UpdateMatrix()
{
	AnimaMatrix translationMatrix(_engine);
	AnimaMatrix rotationMatrix(_engine);
	AnimaMatrix scaleMatrix(_engine);

	translationMatrix.InitTranslation(_translation[0], _translation[1], _translation[2]);
	scaleMatrix.InitScale(_scale[0], _scale[1], _scale[2]);
	rotationMatrix.InitRotation(_rotation[0], _rotation[1], _rotation[2]);

	//_transformationMatrix = _initialTransformationMatrix * scaleMatrix * rotationMatrix * translationMatrix;
	_transformationMatrix =  translationMatrix * rotationMatrix * scaleMatrix * _initialTransformationMatrix;

	//_transformationMatrix = _initialTransformationMatrix;
	//_transformationMatrix.Scale(_scale[0], _scale[1], _scale[2]);
	//_transformationMatrix.RotateX(_rotation[0]);
	//_transformationMatrix.RotateY(_rotation[1]);
	//_transformationMatrix.RotateZ(_rotation[2]);
	//_transformationMatrix.Translate(_translation[0], _translation[1], _translation[2]);
}

void AnimaTransformation::SetTransformationMatrix(const AnimaMatrix& m)
{
	_initialTransformationMatrix = m;
	_transformationMatrix = m;
}

void AnimaTransformation::SetTransformationMatrix(AFloat m[16])
{
	_initialTransformationMatrix.SetData(m);
	_transformationMatrix.SetData(m);
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
