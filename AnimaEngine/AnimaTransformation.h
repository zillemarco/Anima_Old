//
//  AnimaTransformation.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTransformation__
#define __Anima__AnimaTransformation__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaSceneObject;

class ANIMA_ENGINE_EXPORT AnimaTransformation
{
	friend class AnimaSceneObject;

public:
	AnimaTransformation();
	AnimaTransformation(const AnimaMatrix& initialTransformationMatrix);
	AnimaTransformation(const AnimaVertex3f& t, const AnimaVertex3f& r, const AnimaVertex3f& s);
	AnimaTransformation(AFloat tx, AFloat ty, AFloat tz, AFloat rx, AFloat ry, AFloat rz, AFloat sx, AFloat sy, AFloat sz);
	AnimaTransformation(const AnimaTransformation& src);
	AnimaTransformation(AnimaTransformation&& src);
	~AnimaTransformation();
	
	AnimaTransformation& operator=(const AnimaTransformation& src);
	AnimaTransformation& operator=(AnimaTransformation&& src);
	
public:
	void Translate(const AnimaVertex3f& t);
	void Translate(AFloat tx, AFloat ty, AFloat tz);
	void TranslateX(AFloat tx);
	void TranslateY(AFloat ty);
	void TranslateZ(AFloat tz);
	void SetTranslation(const AnimaVertex3f& t);
	void SetTranslation(AFloat tx, AFloat ty, AFloat tz);
	void SetTranslationX(AFloat tx);
	void SetTranslationY(AFloat ty);
	void SetTranslationZ(AFloat tz);
	AnimaVertex3f GetTranslation();
	AFloat GetTranslationX();
	AFloat GetTranslationY();
	AFloat GetTranslationZ();

	void Rotate(const AnimaVertex3f& r);
	void Rotate(AFloat rx, AFloat ry, AFloat rz);
	void RotateX(AFloat rx);
	void RotateY(AFloat ry);
	void RotateZ(AFloat rz);
	void SetRotation(const AnimaVertex3f& r);
	void SetRotation(AFloat rx, AFloat ry, AFloat rz);
	void SetRotationX(AFloat rx);
	void SetRotationY(AFloat ry);
	void SetRotationZ(AFloat rz);
	void RotateDeg(const AnimaVertex3f& r);
	void RotateDeg(AFloat rx, AFloat ry, AFloat rz);
	void RotateXDeg(AFloat rx);
	void RotateYDeg(AFloat ry);
	void RotateZDeg(AFloat rz);
	void SetRotationDeg(const AnimaVertex3f& r);
	void SetRotationDeg(AFloat rx, AFloat ry, AFloat rz);
	void SetRotationXDeg(AFloat rx);
	void SetRotationYDeg(AFloat ry);
	void SetRotationZDeg(AFloat rz);
	AnimaVertex3f GetRotation();
	AFloat GetRotationX();
	AFloat GetRotationY();
	AFloat GetRotationZ();

	void Scale(const AnimaVertex3f& s);
	void Scale(AFloat sx, AFloat sy, AFloat sz);
	void ScaleX(AFloat sx);
	void ScaleY(AFloat sy);
	void ScaleZ(AFloat sz);
	void SetScale(const AnimaVertex3f& s);
	void SetScale(AFloat sx, AFloat sy, AFloat sz);
	void SetScaleX(AFloat sx);
	void SetScaleY(AFloat sy);
	void SetScaleZ(AFloat sz);
	AnimaVertex3f GetScale();
	AFloat GetScaleX();
	AFloat GetScaleY();
	AFloat GetScaleZ();

	void UpdateMatrix();
	void SetTransformationMatrix(const AnimaMatrix& m);
	void SetTransformationMatrix(AFloat m[16]);
	AnimaMatrix GetTransformationMatrix();
	AnimaMatrix* GetPTransformationMatrix();
	AnimaMatrix GetNormalMatrix();
	AnimaMatrix* GetPNormalMatrix();

	AnimaSceneObject* GetParentObject() const;

protected:
	void SetInitialTransformationMatrix(const AnimaMatrix& m);
	void SetInitialTransformationMatrix(AFloat m[16]);

	void SetParentObject(AnimaSceneObject* parentObject);
	
protected:
	AnimaVertex3f _translation;
	AnimaVertex3f _rotation;
	AnimaVertex3f _scale;

	AnimaSceneObject* _parentObject;

	AnimaMatrix _transformationMatrix;
	AnimaMatrix _normalMatrix;
	AnimaMatrix _initialTransformationMatrix;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTransformation__) */
