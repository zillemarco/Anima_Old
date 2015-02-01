#pragma once

#include "AnimaEngineCore.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"
#include "AnimaVertex.h"

#define ANIMA_MATRIX_SIZE	16

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMatrix
{
public:
	union
	{
		float m[16];
		struct
		{
			float x[4];
			float y[4];
			float z[4];
			float w[4];
		};
	};

public:
	AnimaMatrix();
	AnimaMatrix(const AnimaMatrix& src);
	AnimaMatrix(AnimaMatrix&& src);
	AnimaMatrix(const AFloat mat[16]);
	~AnimaMatrix();
	
	AnimaMatrix& operator=(const AnimaMatrix& src);
	AnimaMatrix& operator=(AnimaMatrix&& src);

	AnimaVertex3f operator*(const AnimaVertex3f& p) const;
	AnimaMatrix operator*(const AnimaMatrix& p) const;
	AnimaMatrix& operator*=(const AnimaMatrix& p);
	AnimaMatrix operator*(const AFloat p[16]) const;
	AnimaMatrix& operator*=(const AFloat p[16]);
	
	bool operator==(const AnimaMatrix& p) const;
	bool operator!=(const AnimaMatrix& p) const;
	bool operator==(const AFloat p[16]) const;
	bool operator!=(const AFloat p[16]) const;

	void Fill(const AFloat p[16]);

	AnimaVertex3f GetXVector() const;
	AnimaVertex3f GetYVector() const;
	AnimaVertex3f GetZVector() const;
	AnimaVertex3f GetPosition() const;

	void SetIdentity();
	void Keep3x3();
	
	void Transpose();
	void TransposeSSE();
	AnimaMatrix Transposed() const;
	AnimaMatrix TransposedSSE() const;

	void Inverse();
	void InverseSSE();
	AnimaMatrix Inversed() const;
	AnimaMatrix InversedSSE() const;

	AFloat Determinant() const;

	void MultiplyMatrix(const AnimaMatrix& p);
	void MultiplyMatrixSSE(const AnimaMatrix& p);

	void LookAt(const AnimaVertex3f &from, const AnimaVertex3f &to, const AnimaVertex3f &up);
	void LookAt(AFloat xFrom, AFloat yFrom, AFloat zFrom, AFloat xTo, AFloat yTo, AFloat zTo, AFloat xUp, AFloat yUp, AFloat zUp);
	static AnimaMatrix MakeLookAt(const AnimaVertex3f &from, const AnimaVertex3f &to, const AnimaVertex3f &up);
	static AnimaMatrix MakeLookAt(AFloat xFrom, AFloat yFrom, AFloat zFrom, AFloat xTo, AFloat yTo, AFloat zTo, AFloat xUp, AFloat yUp, AFloat zUp);

	void Perspective(AFloat fov, AFloat aspect, AFloat zNear, AFloat zFar);
	static AnimaMatrix MakePerspective(AFloat fov, AFloat aspect, AFloat zNear, AFloat zFar);

	void Ortho(AFloat left, AFloat right, AFloat bottom, AFloat top, AFloat zNear, AFloat zFar);
	static AnimaMatrix MakeOrtho(AFloat left, AFloat right, AFloat bottom, AFloat top, AFloat zNear, AFloat zFar);

	void Translate(const AnimaVertex3f& v);
	void Translate(const AFloat x, const AFloat y, const AFloat z);
	static AnimaMatrix MakeTranslation(const AnimaVertex3f& v);
	static AnimaMatrix MakeTranslation(const AFloat x, const AFloat y, const AFloat z);

	void Scale(const AnimaVertex4f& v);
	void Scale(const AFloat x, const AFloat y, const AFloat z, const AFloat w);
	static AnimaMatrix MakeScale(const AnimaVertex4f& v);
	static AnimaMatrix MakeScale(const AFloat x, const AFloat y, const AFloat z, const AFloat w);

	void RotateRad(const AnimaVertex3f& v, AFloat rad);
	void RotateDeg(const AnimaVertex3f& v, AFloat deg);
	void RotateRad(const AFloat x, const AFloat y, const AFloat z, AFloat rad);
	void RotateDeg(const AFloat x, const AFloat y, const AFloat z, AFloat deg);
	static AnimaMatrix MakeRotationRad(const AnimaVertex3f& v, AFloat rad);
	static AnimaMatrix MakeRotationDeg(const AnimaVertex3f& v, AFloat deg);
	static AnimaMatrix MakeRotationRad(const AFloat x, const AFloat y, const AFloat z, AFloat rad);
	static AnimaMatrix MakeRotationDeg(const AFloat x, const AFloat y, const AFloat z, AFloat deg);

	void RotateXRad(AFloat rad);
	void RotateXDeg(AFloat deg);
	static AnimaMatrix MakeRotationXRad(AFloat rad);
	static AnimaMatrix MakeRotationXDeg(AFloat deg);

	void RotateYRad(AFloat rad);
	void RotateYDeg(AFloat deg);
	static AnimaMatrix MakeRotationYRad(AFloat rad);
	static AnimaMatrix MakeRotationYDeg(AFloat deg);

	void RotateZRad(AFloat rad);
	void RotateZDeg(AFloat deg);
	static AnimaMatrix MakeRotationZRad(AFloat rad);
	static AnimaMatrix MakeRotationZDeg(AFloat deg);

	AnimaVertex3f GetRotationAxes() const;

	void Transform4x4(AnimaVertex3f& dest, const AnimaVertex3f& src) const;
	AnimaVertex3f Transform4x4(const AnimaVertex3f& p) const;
	void Transform4x3(AnimaVertex3f& dest, const AnimaVertex3f& src) const;
	AnimaVertex3f Transform4x3(const AnimaVertex3f& p) const;
	void Transform3x3(AnimaVertex3f& dest, const AnimaVertex3f& src) const;
	AnimaVertex3f Transform3x3(AnimaVertex3f& p) const;

	void DumpMemory() const;
};

END_ANIMA_ENGINE_NAMESPACE