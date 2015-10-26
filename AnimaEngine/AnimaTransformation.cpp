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
#include "AnimaXmlTranslators.h"

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
	_normalMatrix.SetIdentity();
	_modelNodeTransformationMatrix.SetIdentity();
}

AnimaTransformation::AnimaTransformation(const AnimaVertex3f& t, const AnimaVertex3f& r, const AnimaVertex3f& s)
	: _translation(t)
	, _rotation(r)
	, _scale(s)
{
	_parentObject = nullptr;
	_modelNodeTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(AFloat tx, AFloat ty, AFloat tz, AFloat rx, AFloat ry, AFloat rz, AFloat sx, AFloat sy, AFloat sz)
	: _translation(tx, ty, tz)
	, _rotation(rx, ry, rz)
	, _scale(sx, sy, sz)
{
	_parentObject = nullptr;
	_modelNodeTransformationMatrix.SetIdentity();

	UpdateMatrix();
}

AnimaTransformation::AnimaTransformation(const AnimaTransformation& src)
	: _translation(src._translation)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _normalMatrix(src._normalMatrix)
	, _modelNodeTransformationMatrix(src._modelNodeTransformationMatrix)
{
	_parentObject = src._parentObject;
}

AnimaTransformation::AnimaTransformation(AnimaTransformation&& src)
	: _translation(src._translation)
	, _rotation(src._rotation)
	, _scale(src._scale)
	, _transformationMatrix(src._transformationMatrix)
	, _normalMatrix(src._normalMatrix)
	, _modelNodeTransformationMatrix(src._modelNodeTransformationMatrix)
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
		_normalMatrix = src._normalMatrix;
		_modelNodeTransformationMatrix = src._modelNodeTransformationMatrix;
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
		_normalMatrix = src._normalMatrix;
		_modelNodeTransformationMatrix = src._modelNodeTransformationMatrix;
		_parentObject = src._parentObject;
	}
	
	return *this;
}

ptree AnimaTransformation::GetObjectTree() const
{
	ptree tree;
	
	tree.add("AnimaTransformation.Traslation", _translation);
	tree.add("AnimaTransformation.Rotation", _rotation);
	tree.add("AnimaTransformation.Scale", _scale);
	tree.add("AnimaTransformation.TransformationMatrix", _transformationMatrix);
	tree.add("AnimaTransformation.NormalMatrix", _normalMatrix);
	tree.add("AnimaTransformation.ModelNodeTransformationMatrix", _modelNodeTransformationMatrix);
	
	return tree;
}

bool AnimaTransformation::ReadObject(const ptree& objectTree)
{
	try
	{
		_translation = objectTree.get<AnimaVertex3f>("AnimaTransformation.Traslation", AnimaVertex3f(0.0f));
		_rotation = objectTree.get<AnimaVertex3f>("AnimaTransformation.Rotation", AnimaVertex3f(0.0f));
		_scale = objectTree.get<AnimaVertex3f>("AnimaTransformation.Scale", AnimaVertex3f(1.0f));
		_transformationMatrix = objectTree.get<AnimaMatrix>("AnimaTransformation.Traslation", AnimaMatrix());
		_normalMatrix = objectTree.get<AnimaMatrix>("AnimaTransformation.Traslation", AnimaMatrix());
		_modelNodeTransformationMatrix = objectTree.get<AnimaMatrix>("AnimaTransformation.Traslation", AnimaMatrix());
	}
	catch (boost::property_tree::ptree_bad_path& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing transformation: %s", exception.what());
		return false;
	}
	catch (boost::property_tree::ptree_bad_data& exception)
	{
		AnimaLogger::LogMessageFormat("ERROR - Error parsing transformation: %s", exception.what());
		return false;
	}
	
	return true;
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
			AnimaMatrix absoluteTransformationMatrix = parentObjectParent->GetMatrix("AModelMatrix") * _transformationMatrix;
			AnimaMatrix absoluteNormalMatrix = parentObjectParent->GetMatrix("ANormalMatrix") * _normalMatrix;

			_parentObject->SetMatrix("AModelMatrix", absoluteTransformationMatrix);
			_parentObject->SetMatrix("ANormalMatrix", absoluteNormalMatrix);
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
	_transformationMatrix = m;

	if (_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();

		if (parentObjectParent != nullptr)
		{
			AnimaMatrix absoluteTransformationMatrix = parentObjectParent->GetMatrix("AModelMatrix") * _transformationMatrix;
			AnimaMatrix absoluteNormalMatrix = parentObjectParent->GetMatrix("ANormalMatrix") * _normalMatrix;

			_parentObject->SetMatrix("AModelMatrix", absoluteTransformationMatrix);
			_parentObject->SetMatrix("ANormalMatrix", absoluteNormalMatrix);
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

void AnimaTransformation::SetTransformationMatrix(AFloat m[16])
{
	_transformationMatrix.Fill(m);

	if (_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();

		if (parentObjectParent != nullptr)
		{
			AnimaMatrix absoluteTransformationMatrix = parentObjectParent->GetMatrix("AModelMatrix") * _transformationMatrix;
			AnimaMatrix absoluteNormalMatrix = parentObjectParent->GetMatrix("ANormalMatrix") * _normalMatrix;

			_parentObject->SetMatrix("AModelMatrix", absoluteTransformationMatrix);
			_parentObject->SetMatrix("ANormalMatrix", absoluteNormalMatrix);
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

void AnimaTransformation::SetNormalMatrix(const AnimaMatrix& m)
{
	_normalMatrix = m;

	if (_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();

		if (parentObjectParent != nullptr)
		{
			AnimaMatrix absoluteTransformationMatrix = parentObjectParent->GetMatrix("AModelMatrix") * _transformationMatrix;
			AnimaMatrix absoluteNormalMatrix = parentObjectParent->GetMatrix("ANormalMatrix") * _normalMatrix;

			_parentObject->SetMatrix("AModelMatrix", absoluteTransformationMatrix);
			_parentObject->SetMatrix("ANormalMatrix", absoluteNormalMatrix);
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

void AnimaTransformation::SetNormalMatrix(AFloat m[16])
{
	_normalMatrix.Fill(m);

	if (_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();

		if (parentObjectParent != nullptr)
		{
			AnimaMatrix absoluteTransformationMatrix = parentObjectParent->GetMatrix("AModelMatrix") * _transformationMatrix;
			AnimaMatrix absoluteNormalMatrix = parentObjectParent->GetMatrix("ANormalMatrix") * _normalMatrix;

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

void AnimaTransformation::SetModelNodeTransformationMatrix(const AnimaMatrix& m)
{
	_modelNodeTransformationMatrix = m;
}

void AnimaTransformation::SetModelNodeTransformationMatrix(AFloat m[16])
{
	_modelNodeTransformationMatrix.Fill(m);
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

AnimaMatrix	AnimaTransformation::GetModelNodeTransformationMatrix()
{
	return _modelNodeTransformationMatrix;
}

AnimaMatrix* AnimaTransformation::GetPTModelNoderansformationMatrix()
{
	return &_modelNodeTransformationMatrix;
}

AnimaSceneObject* AnimaTransformation::GetParentObject() const
{
	return _parentObject;
}

void AnimaTransformation::SetParentObject(AnimaSceneObject* parentObject)
{
	_parentObject = parentObject;
}

AnimaVertex3f AnimaTransformation::GetCompleteTranslation() const
{
	AnimaVertex3f completeTranslation = _translation;
	
	if(_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();
		if(parentObjectParent)
			completeTranslation += parentObjectParent->GetTransformation()->GetCompleteTranslation();
	}
	
	return completeTranslation;
}

AnimaVertex3f AnimaTransformation::GetCompleteRotation() const
{
	AnimaVertex3f completeRotation = _rotation;
	
	if(_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();
		if(parentObjectParent)
			completeRotation += parentObjectParent->GetTransformation()->GetCompleteRotation();
	}
	
	return completeRotation;
}

AnimaVertex3f AnimaTransformation::GetCompleteScale() const
{
	AnimaVertex3f completeScale = _scale;
	
	if(_parentObject != nullptr)
	{
		AnimaSceneObject* parentObjectParent = _parentObject->GetParentObject();
		if(parentObjectParent)
			completeScale *= parentObjectParent->GetTransformation()->GetCompleteScale();
	}
	
	return completeScale;
}

END_ANIMA_ENGINE_NAMESPACE
