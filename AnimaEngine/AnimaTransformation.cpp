//
//  AnimaTransformation.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaTransformation.h"
#include "AnimaMath.h"
#include "AnimaSceneObject.h"

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaTransformation::AnimaTransformation()
{
	_translation.x = 0.0f;	_translation.y = 0.0f;	_translation.z = 0.0f;
	_rotation.x = 0.0f;		_rotation.y = 0.0f;		_rotation.z = 0.0f;
	_scale.x = 1.0f;		_scale.y = 1.0f;		_scale.z = 1.0f;

	_parentObject = nullptr;

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

	_parentObject = nullptr;
}

AnimaTransformation::AnimaTransformation(const AnimaVertex3f& t, const AnimaVertex3f& r, const AnimaVertex3f& s)
	: _translation(t)
	, _rotation(r)
	, _scale(s)
{
	_parentObject = nullptr;
	_initialTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(AFloat tx, AFloat ty, AFloat tz, AFloat rx, AFloat ry, AFloat rz, AFloat sx, AFloat sy, AFloat sz)
	: _translation(tx, ty, tz)
	, _rotation(rx, ry, rz)
	, _scale(sx, sy, sz)
{
	_parentObject = nullptr;
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
	_parentObject = src._parentObject;
}

AnimaTransformation::AnimaTransformation(AnimaTransformation&& src)
	: _translation(src._translation)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _initialTransformationMatrix(src._initialTransformationMatrix)
{
	_parentObject = src._parentObject;
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
		_parentObject = src._parentObject;
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
		_parentObject = src._parentObject;
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

void AnimaTransformation::Rotate(AFloat rx, AFloat ry, AFloat rz)
{
	_rotation.x += rx;
	_rotation.y += ry;
	_rotation.z += rz;
	UpdateMatrix();
}

void AnimaTransformation::RotateX(AFloat rx)
{
	_rotation.x += rx;
	UpdateMatrix();
}

void AnimaTransformation::RotateY(AFloat ry)
{
	_rotation.y += ry;
	UpdateMatrix();
}

void AnimaTransformation::RotateZ(AFloat rz)
{
	_rotation.z += rz;
	UpdateMatrix();
}

void AnimaTransformation::SetRotation(const AnimaVertex3f& r)
{
	_rotation = r;
	UpdateMatrix();
}

void AnimaTransformation::SetRotation(AFloat rx, AFloat ry, AFloat rz)
{
	_rotation.x = rx;
	_rotation.y = ry;
	_rotation.z = rz;
	UpdateMatrix();
}

void AnimaTransformation::SetRotationX(AFloat rx)
{
	_rotation.x = rx;
	UpdateMatrix();
}

void AnimaTransformation::SetRotationY(AFloat ry)
{
	_rotation.y = ry;
	UpdateMatrix();
}

void AnimaTransformation::SetRotationZ(AFloat rz)
{
	_rotation.z = rz;
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

void AnimaTransformation::Scale(const AnimaVertex3f& s)
{
	_scale += s;
	UpdateMatrix();
}

void AnimaTransformation::Scale(AFloat sx, AFloat sy, AFloat sz)
{
	_scale.x += sx;
	_scale.y += sy;
	_scale.z += sz;
	UpdateMatrix();
}

void AnimaTransformation::ScaleX(AFloat sx)
{
	_scale.x += sx;
	UpdateMatrix();
}

void AnimaTransformation::ScaleY(AFloat sy)
{
	_scale.y += sy;
	UpdateMatrix();
}

void AnimaTransformation::ScaleZ(AFloat sz)
{
	_scale.z += sz;
	UpdateMatrix();
}

void AnimaTransformation::SetScale(const AnimaVertex3f& s)
{
	_scale = s;
	UpdateMatrix();
}

void AnimaTransformation::SetScale(AFloat sx, AFloat sy, AFloat sz)
{
	_scale.x = sx;
	_scale.y = sy;
	_scale.z = sz;
	UpdateMatrix();
}

void AnimaTransformation::SetScaleX(AFloat sx)
{
	_scale.x = sx;
	UpdateMatrix();
}

void AnimaTransformation::SetScaleY(AFloat sy)
{
	_scale.y = sy;
	UpdateMatrix();
}

void AnimaTransformation::SetScaleZ(AFloat sz)
{
	_scale.z = sz;
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
	AnimaMatrix rotationMatrix = AnimaMatrix::MakeRotationZRad(_rotation.z) * (AnimaMatrix::MakeRotationYRad(_rotation.y) * AnimaMatrix::MakeRotationXRad(_rotation.x));
	AnimaMatrix scaleMatrix = AnimaMatrix::MakeScale(_scale.x, _scale.y, _scale.z, 1.0f);

	_transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	_normalMatrix = rotationMatrix;

	if (_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();

		if (parentObjectParent != nullptr)
		{
			AnimaMatrix absoluteTransformationMatrix = parentObjectParent->GetTransformation()->GetTransformationMatrix() * _transformationMatrix;
			AnimaMatrix absoluteNormalMatrix = parentObjectParent->GetTransformation()->GetNormalMatrix() * _normalMatrix;

			_parentObject->SetMatrix("AModelMatrix", _transformationMatrix);
			_parentObject->SetMatrix("ANormalMatrix", _normalMatrix);
		}
		else
		{
			_parentObject->SetMatrix("AModelMatrix", _transformationMatrix);
			_parentObject->SetMatrix("ANormalMatrix", _normalMatrix);
		}

		_parentObject->SetMatrix("RModelMatrix", _transformationMatrix);
		_parentObject->SetMatrix("RNormalMatrix", _normalMatrix);

		_parentObject->UpdateChildrenTransformation();
	}
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

AnimaMatrix AnimaTransformation::GetNormalMatrix()
{
	return _normalMatrix;
}

AnimaMatrix* AnimaTransformation::GetPNormalMatrix()
{
	return &_normalMatrix;
}

void AnimaTransformation::SetInitialTransformationMatrix(const AnimaMatrix& m)
{
	_initialTransformationMatrix = m;
	UpdateMatrix();
}

void AnimaTransformation::SetInitialTransformationMatrix(AFloat m[16])
{
	_initialTransformationMatrix = m;
	UpdateMatrix();
}

END_ANIMA_ENGINE_NAMESPACE
