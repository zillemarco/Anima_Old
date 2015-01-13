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
	AnimaMatrix(AnimaEngine* engine);
	AnimaMatrix(AnimaEngine* engine, AFloat data[ANIMA_MATRIX_SIZE]);
	AnimaMatrix(const AnimaMatrix& src);
	AnimaMatrix(AnimaMatrix&& src);
	~AnimaMatrix();

	AnimaMatrix& operator=(const AnimaMatrix& src);
	AnimaMatrix& operator=(AnimaMatrix&& src);
		
	inline AFloat& operator[](ASizeT index);
	inline const AFloat& operator[](ASizeT index) const;
	
	inline bool operator==(const AnimaMatrix& left);
	inline bool operator==(const AFloat left[ANIMA_MATRIX_SIZE]);
	inline bool operator!=(const AnimaMatrix& left);
	inline bool operator!=(const AFloat left[ANIMA_MATRIX_SIZE]);
	inline operator AFloat*();
	inline operator const AFloat*();
	
public:
	void SetData(AFloat data[ANIMA_MATRIX_SIZE]);
	void SetData(AFloat data, ASizeT index);
	
	AFloat GetElement(ASizeT index);
	AFloat GetElement(ASizeT row, ASizeT col);
	void GetRow(AFloat* row, ASizeT index);
	void GetColumn(AFloat* col, ASizeT index);

	AFloat* GetData() const;
	void GetData(AFloat m[ANIMA_MATRIX_SIZE]) const;
	const AFloat* GetConstData() const;
		
	void SetIdentity();
	
	// Operations
public:
	AnimaMatrix Inverse() const;
	AnimaMatrix Transpose() const;
	inline float Determinant() const;
	
	void Translate(float tx, float ty, float tz);
	void Translate(const AnimaVertex3f& translation);
	void Scale(float sx, float sy, float sz);
	void Scale(const AnimaVertex3f& scale);
	void Rotate(float angle, AnimaVertex3f axis);
	void RotateDeg(float angle, AnimaVertex3f axis);
	void RotateX(float angle);
	void RotateXDeg(float angle);
	void RotateY(float angle);
	void RotateYDeg(float angle);
	void RotateZ(float angle);
	void RotateZDeg(float angle);
	void Perspective(float fov, float ratio, float zNear, float zFar);
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
	void LookAt(const AnimaVertex3f& eye, const AnimaVertex3f& center, const AnimaVertex3f& up);
	void Viewport(float left, float bottom, float width, float height, float zNear, float zFar);
	void Frustum(float left, float right, float bottom, float top, float zNear, float zFar);

	AnimaMatrix& operator+=(const AnimaMatrix& src);
	inline friend AnimaMatrix operator+(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m += srcb;
		return m;
	}

	AnimaMatrix& operator-=(const AnimaMatrix& src);
	inline friend AnimaMatrix operator-(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m -= srcb;
		return m;
	}

	AnimaMatrix& operator*=(const AnimaMatrix& src);
	inline friend AnimaMatrix operator*(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m *= srcb;
		return m;
	}

	AnimaMatrix& operator/=(const AnimaMatrix& src);
	inline friend AnimaMatrix operator/(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m /= srcb;
		return m;
	}

	AnimaMatrix& operator+=(const AFloat& add);
	inline friend AnimaMatrix operator+(const AnimaMatrix& srca, const AFloat& add) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m += add;
		return m;
	}

	AnimaMatrix& operator-=(const AFloat& sub);
	inline friend AnimaMatrix operator-(const AnimaMatrix& srca, const AFloat& sub) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m -= sub;
		return m;
	}

	AnimaMatrix& operator*=(const AFloat& mul);
	inline friend AnimaMatrix operator*(const AnimaMatrix& srca, const AFloat& mul) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m *= mul;
		return m;
	}

	AnimaMatrix& operator/=(const AFloat& div);
	inline friend AnimaMatrix operator/(const AnimaMatrix& srca, const AFloat& div) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m /= div;
		return m;
	}
		
	void DumpMemory(bool bLogFile = true);

private:
	// _matrixData memory structure
	//	[0]		[1]		[2]		[3]
	//	[4]		[5]		[6]		[7]
	//	[8]		[9]		[10]	[11]
	//	[12]	[13]	[14]	[15]
	AFloat* _matrixData;

	AnimaEngine* _engine;
};

END_ANIMA_ENGINE_NAMESPACE