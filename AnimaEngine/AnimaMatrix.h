#pragma once

#include "AnimaEngineCore.h"
#include "AnimaVertex.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"

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
	
	AnimaMatrix& operator+=(const AnimaMatrix& src);
	friend AnimaMatrix operator+(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m += srcb;
		return m;
	}
	
	AnimaMatrix& operator-=(const AnimaMatrix& src);
	friend AnimaMatrix operator-(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m -= srcb;
		return m;
	}
	
	AnimaMatrix& operator*=(const AnimaMatrix& src);
	friend AnimaMatrix operator*(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m *= srcb;
		return m;
	}
	
	AnimaMatrix& operator/=(const AnimaMatrix& src);
	friend AnimaMatrix operator/(const AnimaMatrix& srca, const AnimaMatrix& srcb) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m /= srcb;
		return m;
	}
	
	AnimaMatrix& operator+=(const AFloat& add);
	friend AnimaMatrix operator+(const AnimaMatrix& srca, const AFloat& add) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m += add;
		return m;
	}
	
	AnimaMatrix& operator-=(const AFloat& sub);
	friend AnimaMatrix operator-(const AnimaMatrix& srca, const AFloat& sub) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m -= sub;
		return m;
	}
	
	AnimaMatrix& operator*=(const AFloat& mul);
	friend AnimaMatrix operator*(const AnimaMatrix& srca, const AFloat& mul) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m *= mul;
		return m;
	}
	
	AnimaMatrix& operator/=(const AFloat& div);
	friend AnimaMatrix operator/(const AnimaMatrix& srca, const AFloat& div) {
		AnimaMatrix m(srca._engine, srca._matrixData);
		m /= div;
		return m;
	}
	
	AFloat& operator[](ASizeT index);
	const AFloat& operator[](ASizeT index) const;
	
	bool operator==(const AnimaMatrix& left);
	bool operator==(const AFloat left[ANIMA_MATRIX_SIZE]);
	bool operator!=(const AnimaMatrix& left);
	bool operator!=(const AFloat left[ANIMA_MATRIX_SIZE]);
	
public:
	void SetData(AFloat data[ANIMA_MATRIX_SIZE]);
	void SetData(AFloat data, ASizeT index);
	
	AFloat GetElement(ASizeT index);
	AFloat GetElement(ASizeT row, ASizeT col);
	void GetRow(AFloat* row, ASizeT index);
	void GetColumn(AFloat* col, ASizeT index);
	
	void SetIdentity();
	
	AnimaMatrix Inverse() const;
	
	void DumpMemory();

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