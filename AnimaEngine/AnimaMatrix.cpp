#include "AnimaMatrix.h"
#include "AnimaAssert.h"
#include "AnimaMath.h"
#include <xmmintrin.h>

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#define _mm_shufd(xmm, mask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), mask))

AnimaMatrix::AnimaMatrix(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
	
	SetIdentity();
}

AnimaMatrix::AnimaMatrix(AnimaEngine* engine, AFloat data[ANIMA_MATRIX_SIZE])
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;

	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
	memset(_matrixData, 0, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
	memcpy(_matrixData, data, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

AnimaMatrix::AnimaMatrix(const AnimaMatrix& src)
{
	_engine = src._engine;
	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));

	SetData(src._matrixData);
}

AnimaMatrix::AnimaMatrix(AnimaMatrix&& src)
: _engine(src._engine)
{
	_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));

	SetData(src._matrixData);
}

AnimaMatrix::~AnimaMatrix()
{
	ANIMA_ASSERT(_engine != nullptr);

	if(_matrixData != nullptr)
	{
		(_engine->GetGenericAllocator())->Deallocate(_matrixData);
		_matrixData = nullptr;
	}
}

AnimaMatrix& AnimaMatrix::operator=(const AnimaMatrix& src)
{
	if (this != &src)
	{
		if (_matrixData == nullptr)
			_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));

		memcpy(_matrixData, src._matrixData, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
	}

	return *this;
}

AnimaMatrix& AnimaMatrix::operator=(AnimaMatrix&& src)
{
	if (this != &src)
	{
		_engine = src._engine;

		if (_matrixData == nullptr)
			_matrixData = (AFloat*)(_engine->GetGenericAllocator())->Allocate(sizeof(AFloat) * ANIMA_MATRIX_SIZE, ANIMA_ENGINE_ALIGN_OF(AFloat));
		
		SetData(src._matrixData);
	}
	
	return *this;
}

AFloat& AnimaMatrix::operator[](ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < ANIMA_MATRIX_SIZE);
	return _matrixData[index];
}

const AFloat& AnimaMatrix::operator[](ASizeT index) const
{
	ANIMA_ASSERT(index >= 0 && index < ANIMA_MATRIX_SIZE);
	return const_cast<AFloat&>(_matrixData[index]);
}

bool AnimaMatrix::operator==(const AnimaMatrix& left)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
	{
		if(this->_matrixData[i] != left._matrixData[i])
			return false;
	}
	return true;
}

bool AnimaMatrix::operator==(const AFloat left[ANIMA_MATRIX_SIZE])
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
	{
		if(this->_matrixData[i] != left[i])
			return false;
	}
	return true;
}

bool AnimaMatrix::operator!=(const AnimaMatrix& left)
{
	return !operator==(left);
}

bool AnimaMatrix::operator!=(const AFloat left[ANIMA_MATRIX_SIZE])
{
	return !operator==(left);
}

inline AnimaMatrix::operator AFloat*()
{
	return _matrixData;
}

inline AnimaMatrix::operator const AFloat*()
{
	return const_cast<AFloat*>(_matrixData);
}

AFloat* AnimaMatrix::GetData() const
{
	return _matrixData;
}

void AnimaMatrix::GetData(AFloat m[ANIMA_MATRIX_SIZE]) const
{
	memcpy(m, _matrixData, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

const AFloat* AnimaMatrix::GetConstData() const
{
	return const_cast<AFloat*>(_matrixData);
}

void AnimaMatrix::SetData(AFloat data[ANIMA_MATRIX_SIZE])
{
	memcpy(_matrixData, data, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

void AnimaMatrix::SetData(AFloat data, ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < ANIMA_MATRIX_SIZE);
	_matrixData[index] = data;
}

AFloat AnimaMatrix::GetElement(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < ANIMA_MATRIX_SIZE);
	return _matrixData[index];
}

AFloat AnimaMatrix::GetElement(ASizeT row, ASizeT col)
{
	return GetElement(row * 4 + col);
}

void AnimaMatrix::GetRow(AFloat* row, ASizeT index)
{
	ANIMA_ASSERT(row != nullptr);
	row[0] = GetElement(index, 0);
	row[1] = GetElement(index, 1);
	row[2] = GetElement(index, 2);
	row[3] = GetElement(index, 3);
}

void AnimaMatrix::GetColumn(AFloat* col, ASizeT index)
{
	ANIMA_ASSERT(col != nullptr);
	col[0] = GetElement(0, index);
	col[1] = GetElement(1, index);
	col[2] = GetElement(2, index);
	col[3] = GetElement(3, index);
}

void AnimaMatrix::SetIdentity()
{
	memset(_matrixData, 0, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
	_matrixData[0] = 1.0f;
	_matrixData[5] = 1.0f;
	_matrixData[10] = 1.0f;
	_matrixData[15] = 1.0f;
}

AnimaMatrix& AnimaMatrix::operator+=(const AnimaMatrix& src)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
		this->_matrixData[i] += src._matrixData[i];
	return *this;
}

AnimaMatrix& AnimaMatrix::operator-=(const AnimaMatrix& src)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
		this->_matrixData[i] -= src._matrixData[i];
	return *this;
}

AnimaMatrix& AnimaMatrix::operator*=(const AnimaMatrix& src)
{
	AFloat* srcV = src._matrixData;

	__m128 src1 = _mm_loadu_ps(&srcV[0]);
	__m128 src2 = _mm_loadu_ps(&srcV[4]);
	__m128 src3 = _mm_loadu_ps(&srcV[8]);
	__m128 src4 = _mm_loadu_ps(&srcV[12]);

	__m128 r1 = _mm_loadu_ps(&_matrixData[0]);
	__m128 r2 = _mm_loadu_ps(&_matrixData[4]);
	__m128 r3 = _mm_loadu_ps(&_matrixData[8]);
	__m128 r4 = _mm_loadu_ps(&_matrixData[12]);

	r1 = _mm_add_ps(_mm_add_ps(
					_mm_add_ps(	_mm_mul_ps(_mm_shufd(r1, 0x00), src1),
								_mm_mul_ps(_mm_shufd(r1, 0x55), src2)),
								_mm_mul_ps(_mm_shufd(r1, 0xAA), src3)),
								_mm_mul_ps(_mm_shufd(r1, 0xFF), src4));
	r2 = _mm_add_ps(_mm_add_ps(						 
					_mm_add_ps(	_mm_mul_ps(_mm_shufd(r2, 0x00), src1),
								_mm_mul_ps(_mm_shufd(r2, 0x55), src2)),
								_mm_mul_ps(_mm_shufd(r2, 0xAA), src3)),
								_mm_mul_ps(_mm_shufd(r2, 0xFF), src4));
	r3 = _mm_add_ps(_mm_add_ps(						 
					_mm_add_ps(	_mm_mul_ps(_mm_shufd(r3, 0x00), src1),
								_mm_mul_ps(_mm_shufd(r3, 0x55), src2)),
								_mm_mul_ps(_mm_shufd(r3, 0xAA), src3)),
								_mm_mul_ps(_mm_shufd(r3, 0xFF), src4));
	r4 = _mm_add_ps(_mm_add_ps(						 
					_mm_add_ps(	_mm_mul_ps(_mm_shufd(r4, 0x00), src1),
								_mm_mul_ps(_mm_shufd(r4, 0x55), src2)),
								_mm_mul_ps(_mm_shufd(r4, 0xAA), src3)),
								_mm_mul_ps(_mm_shufd(r4, 0xFF), src4));

	_mm_storeu_ps(&_matrixData[0], r1);
	_mm_storeu_ps(&_matrixData[4], r2);
	_mm_storeu_ps(&_matrixData[8], r3);
	_mm_storeu_ps(&_matrixData[12], r4);

	return *this;
}

AnimaMatrix& AnimaMatrix::operator/=(const AnimaMatrix& src)
{
	AnimaMatrix srcInverse = src.Inverse();
	this->operator*=(srcInverse);

	return *this;
}

AnimaMatrix& AnimaMatrix::operator+=(const AFloat& add)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
		this->_matrixData[i] += add;
	return *this;
}

AnimaMatrix& AnimaMatrix::operator-=(const AFloat& sub)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
		this->_matrixData[i] -= sub;
	return *this;
}

AnimaMatrix& AnimaMatrix::operator*=(const AFloat& mul)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
		this->_matrixData[i] *= mul;
	return *this;
}

AnimaMatrix& AnimaMatrix::operator/=(const AFloat& div)
{
	for(int i = 0; i < ANIMA_MATRIX_SIZE; i++)
		this->_matrixData[i] /= div;
	return *this;
}

AnimaMatrix AnimaMatrix::Inverse() const
{
	AnimaMatrix resultMatrix(_engine, _matrixData);
	AFloat* src = resultMatrix._matrixData;

	__m128 m1 = _mm_loadu_ps(&src[0]);
	__m128 m2 = _mm_loadu_ps(&src[4]);
	__m128 m3 = _mm_loadu_ps(&src[8]);
	__m128 m4 = _mm_loadu_ps(&src[12]);

	__m128 f1 = _mm_sub_ps(	_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0xAA), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xFF), 0x80)),
	 						_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0xAA), 0x80), _mm_shuffle_ps(m3, m2, 0xFF)));
	__m128 f2 = _mm_sub_ps(	_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x55), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xFF), 0x80)),
	 						_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x55), 0x80), _mm_shuffle_ps(m3, m2, 0xFF)));
	__m128 f3 = _mm_sub_ps(	_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x55), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xAA), 0x80)),
							_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x55), 0x80), _mm_shuffle_ps(m3, m2, 0xAA)));
	__m128 f4 = _mm_sub_ps(	_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x00), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xFF), 0x80)),
	 						_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x00), 0x80), _mm_shuffle_ps(m3, m2, 0xFF)));
	__m128 f5 = _mm_sub_ps(	_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x00), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xAA), 0x80)),
	 						_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x00), 0x80), _mm_shuffle_ps(m3, m2, 0xAA)));
	__m128 f6 = _mm_sub_ps(	_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x00), _mm_shufd(_mm_shuffle_ps(m4, m3, 0x55), 0x80)),
	 						_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x00), 0x80), _mm_shuffle_ps(m3, m2, 0x55)));

	__m128 v1 = _mm_shufd(_mm_shuffle_ps(m2, m1, 0x00), 0xA8);
	__m128 v2 = _mm_shufd(_mm_shuffle_ps(m2, m1, 0x55), 0xA8);
	__m128 v3 = _mm_shufd(_mm_shuffle_ps(m2, m1, 0xAA), 0xA8);
	__m128 v4 = _mm_shufd(_mm_shuffle_ps(m2, m1, 0xFF), 0xA8);
	__m128 s1 = _mm_set_ps(-0.0f, 0.0f, -0.0f, 0.0f);
	__m128 s2 = _mm_set_ps(0.0f, -0.0f, 0.0f, -0.0f);
	__m128 i1 = _mm_xor_ps(s1, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v2, f1), _mm_mul_ps(v3, f2)), _mm_mul_ps(v4, f3)));
	__m128 i2 = _mm_xor_ps(s2, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v1, f1), _mm_mul_ps(v3, f4)), _mm_mul_ps(v4, f5)));
	__m128 i3 = _mm_xor_ps(s1, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v1, f2), _mm_mul_ps(v2, f4)), _mm_mul_ps(v4, f6)));
	__m128 i4 = _mm_xor_ps(s2, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(v1, f3), _mm_mul_ps(v2, f5)), _mm_mul_ps(v3, f6)));
	
	__m128 d = _mm_mul_ps(m1, _mm_movelh_ps(_mm_unpacklo_ps(i1, i2), _mm_unpacklo_ps(i3, i4)));
	d = _mm_add_ps(d, _mm_shufd(d, 0x4E));
	d = _mm_add_ps(d, _mm_shufd(d, 0x11));
	d = _mm_div_ps(_mm_set1_ps(1.0f), d);

	__m128 r1 = _mm_mul_ps(i1, d);
	__m128 r2 = _mm_mul_ps(i2, d);
	__m128 r3 = _mm_mul_ps(i3, d);
	__m128 r4 = _mm_mul_ps(i4, d);

	_mm_storeu_ps(&src[0], r1);
	_mm_storeu_ps(&src[4], r2);
	_mm_storeu_ps(&src[8], r3);
	_mm_storeu_ps(&src[12], r4);

	return resultMatrix;
}

float AnimaMatrix::Determinant() const
{
	AFloat* src = _matrixData;

	__m128 m1 = _mm_loadu_ps(&src[0]);
	__m128 m2 = _mm_loadu_ps(&src[4]);
	__m128 m3 = _mm_loadu_ps(&src[8]);
	__m128 m4 = _mm_loadu_ps(&src[12]);

	__m128 r = _mm_shufd(m3, 0x39);
	__m128 v1 = _mm_mul_ps(r, m4);
	__m128 v2 = _mm_mul_ps(r, _mm_shufd(m4, 0x4E));
	__m128 v3 = _mm_mul_ps(r, _mm_shufd(m4, 0x93));

	__m128 r1 = _mm_sub_ps(_mm_shufd(v2, 0x39), _mm_shufd(v1, 0x4E));
	__m128 r2 = _mm_sub_ps(_mm_shufd(v3, 0x4E), v3);
	__m128 r3 = _mm_sub_ps(v2, _mm_shufd(v1, 0x39));
	

	v1 = _mm_shufd(m2, 0x93);
	v2 = _mm_shufd(m2, 0x39);
	v3 = _mm_shufd(m2, 0x4E);
	
	__m128 d = _mm_mul_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(v2, r1), _mm_mul_ps(v3, r2)), _mm_mul_ps(v1, r3)), m1);
	d = _mm_add_ps(d, _mm_shufd(d, 0x4E));
	d = _mm_sub_ss(d, _mm_shufd(d, 0x11));
	return _mm_cvtss_f32(d);
}

AnimaMatrix AnimaMatrix::Transpose() const
{
	AnimaMatrix resultMatrix(_engine, _matrixData);
	AFloat* src = resultMatrix._matrixData;

	__m128 m1 = _mm_loadu_ps(&src[0]);
	__m128 m2 = _mm_loadu_ps(&src[4]);
	__m128 m3 = _mm_loadu_ps(&src[8]);
	__m128 m4 = _mm_loadu_ps(&src[12]);

	__m128 t1 = _mm_unpacklo_ps(m1, m2);
	__m128 t2 = _mm_unpacklo_ps(m3, m4);
	__m128 t3 = _mm_unpackhi_ps(m1, m2);
	__m128 t4 = _mm_unpackhi_ps(m3, m4);
	
	__m128 r1 = _mm_movelh_ps(t1, t2);
	__m128 r2 = _mm_movehl_ps(t2, t1);
	__m128 r3 = _mm_movelh_ps(t3, t4);
	__m128 r4 = _mm_movehl_ps(t4, t3);
	
	_mm_storeu_ps(&src[0], r1);
	_mm_storeu_ps(&src[4], r2);
	_mm_storeu_ps(&src[8], r3);
	_mm_storeu_ps(&src[12], r4);

	return resultMatrix;
}

void AnimaMatrix::DumpMemory(bool bLogFile)
{
	char szBuff[1024];
	sprintf(szBuff, "%f\t%f\t%f\t%f\n %f\t%f\t%f\t%f\n %f\t%f\t%f\t%f\n %f\t%f\t%f\t%f\n\n", 
			_matrixData[0], _matrixData[1], _matrixData[2], _matrixData[3], 
			_matrixData[4], _matrixData[5], _matrixData[6], _matrixData[7], 
			_matrixData[8], _matrixData[9], _matrixData[10], _matrixData[11], 
			_matrixData[12], _matrixData[13], _matrixData[14], _matrixData[15]);

	if (bLogFile)
	{
		bool bCanClose = true;
		if (!freopen(_engine->GetLogFilePath(), "a", stdout))
			bCanClose = false;

		puts(szBuff);

		if (bCanClose)
			fclose(stdout);
	}
	else
	{
	#if defined _MSC_VER
		OutputDebugStringA(szBuff);
	#else
		puts(szBuff);
	#endif
	}
}

void AnimaMatrix::Translate(float tx, float ty, float tz)
{
	AnimaVertex4f m0(_engine, _matrixData);
	AnimaVertex4f m1(_engine, _matrixData + 4);
	AnimaVertex4f m2(_engine, _matrixData + 8);
	AnimaVertex4f m3(_engine, _matrixData + 12);

	AnimaVertex4f res = m0 * tx + m1 * ty + m2 * tz + m3;

	_matrixData[12] = res[0];
	_matrixData[13] = res[1];
	_matrixData[14] = res[2];
	_matrixData[15] = res[3];
}

void AnimaMatrix::Translate(const AnimaVertex3f& translation)
{
	Translate(translation[0], translation[1], translation[2]);
}

void AnimaMatrix::Scale(float sx, float sy, float sz)
{
	AnimaVertex4f m0(_engine, _matrixData);
	AnimaVertex4f m1(_engine, _matrixData + 4);
	AnimaVertex4f m2(_engine, _matrixData + 8);
	AnimaVertex4f m3(_engine, _matrixData + 12);

	AnimaVertex4f r0 = m0 * sx;
	AnimaVertex4f r1 = m1 * sy;
	AnimaVertex4f r2 = m2 * sz;
	AnimaVertex4f r3 = m3;

	_matrixData[0] = r0[0];		_matrixData[1] = r0[1];		_matrixData[2] = r0[2];		_matrixData[3] = r0[3];
	_matrixData[4] = r1[0];		_matrixData[5] = r1[1];		_matrixData[6] = r1[2];		_matrixData[7] = r1[3];
	_matrixData[8] = r2[0];		_matrixData[9] = r2[1];		_matrixData[10] = r2[2];	_matrixData[11] = r2[3];
	_matrixData[12] = r3[0];	_matrixData[13] = r3[1];	_matrixData[14] = r3[2];	_matrixData[15] = r3[3];
}

void AnimaMatrix::Scale(const AnimaVertex3f& scale)
{
	Scale(scale[0], scale[10], scale[2]);
}

void AnimaMatrix::Rotate(float angle, AnimaVertex3f axis)
{
	float a = angle;
	float c = cosf(a);
	float s = sinf(a);

	AnimaVertex3f ax = axis;
	ax.Normalize();
	AnimaVertex3f temp = ((float(1) - c) * axis);

	AnimaMatrix rotMatrix(_engine);
	rotMatrix[0] = c + temp[0] * axis[0];
	rotMatrix[1] = 0 + temp[0] * axis[1] + s * axis[2];
	rotMatrix[2] = 0 + temp[0] * axis[2] - s * axis[1];
	
	rotMatrix[4] = 0 + temp[1] * axis[0] - s * axis[2];
	rotMatrix[5] = c + temp[1] * axis[1];
	rotMatrix[6] = 0 + temp[1] * axis[2] + s * axis[0];
				 
	rotMatrix[8] = 0 + temp[2] * axis[0] + s * axis[1];
	rotMatrix[9] = 0 + temp[2] * axis[1] - s * axis[0];
	rotMatrix[10] = c + temp[2] * axis[2];

	AnimaVertex4f m0(_engine, _matrixData);
	AnimaVertex4f m1(_engine, _matrixData + 4);
	AnimaVertex4f m2(_engine, _matrixData + 8);
	AnimaVertex4f m3(_engine, _matrixData + 12);

	AnimaVertex4f r0 = m0 * rotMatrix[0] + m1 * rotMatrix[1] + m2 * rotMatrix[2];
	AnimaVertex4f r1 = m0 * rotMatrix[4] + m1 * rotMatrix[5] + m2 * rotMatrix[6];
	AnimaVertex4f r2 = m0 * rotMatrix[8] + m1 * rotMatrix[9] + m2 * rotMatrix[10];
	AnimaVertex4f r3 = m3;

	_matrixData[0] = r0[0];		_matrixData[1] = r0[1];		_matrixData[2] = r0[2];		_matrixData[3] = r0[3];
	_matrixData[4] = r1[0];		_matrixData[5] = r1[1];		_matrixData[6] = r1[2];		_matrixData[7] = r1[3];
	_matrixData[8] = r2[0];		_matrixData[9] = r2[1];		_matrixData[10] = r2[2];	_matrixData[11] = r2[3];
	_matrixData[12] = r3[0];	_matrixData[13] = r3[1];	_matrixData[14] = r3[2];	_matrixData[15] = r3[3];
}

void AnimaMatrix::RotateDeg(float angle, AnimaVertex3f axis)
{
	Rotate(angle * (float)M_PI / 180.0f, axis);
}

void AnimaMatrix::RotateX(float angle)
{
	AnimaVertex3f dir(_engine);
	dir[0] = 1.0f;
	dir[1] = 0.0f;
	dir[2] = 0.0f;

	Rotate(angle, dir);
}

void AnimaMatrix::RotateXDeg(float angle)
{
	AnimaVertex3f dir(_engine);
	dir[0] = 1.0f;
	dir[1] = 0.0f;
	dir[2] = 0.0f;

	Rotate(angle *(float)M_PI / 180.0f, dir);
}

void AnimaMatrix::RotateY(float angle)
{
	AnimaVertex3f dir(_engine);
	dir[0] = 0.0f;
	dir[1] = 1.0f;
	dir[2] = 0.0f;

	Rotate(angle, dir);
}

void AnimaMatrix::RotateYDeg(float angle)
{
	AnimaVertex3f dir(_engine);
	dir[0] = 0.0f;
	dir[1] = 1.0f;
	dir[2] = 0.0f;

	Rotate(angle *(float)M_PI / 180.0f, dir);
}

void AnimaMatrix::RotateZ(float angle)
{
	AnimaVertex3f dir(_engine);
	dir[0] = 0.0f;
	dir[1] = 0.0f;
	dir[2] = 1.0f;

	Rotate(angle, dir);
}

void AnimaMatrix::RotateZDeg(float angle)
{
	AnimaVertex3f dir(_engine);
	dir[0] = 0.0f;
	dir[1] = 0.0f;
	dir[2] = 1.0f;

	Rotate(angle *(float)M_PI / 180.0f, dir);
}

void AnimaMatrix::Perspective(float fov, float ratio, float zNear, float zFar)
{
	float radians = (fov / 2.0f) *(float)M_PI / 180.0f;
	float sine = sinf(radians);

	if (sine == 0.0f)
		return;

	float cotan = cosf(radians) / sine;
	float clip = zFar - zNear;
	
	AnimaMatrix m(_engine);
	m._matrixData[0] = cotan / ratio;	m._matrixData[1] = 0.0f;	m._matrixData[2] = 0.0f;							m._matrixData[3] = 0.0f;
	m._matrixData[4] = 0.0f;			m._matrixData[5] = cotan;	m._matrixData[6] = 0.0f;							m._matrixData[7] = 0.0f;
	m._matrixData[8] = 0.0f;			m._matrixData[9] = 0.0f;	m._matrixData[10] = -(zNear + zFar) / clip;			m._matrixData[11] = -1.0f;
	m._matrixData[12] = 0.0f;			m._matrixData[13] = 0.0f;	m._matrixData[14] = -(2.0f * zNear * zFar) / clip;	m._matrixData[15] = 0.0f;

	//m = m.Transpose();

	*this *= m;
}

void AnimaMatrix::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	if (left == right || bottom == top || zNear == zFar)
		return;

	float width = right - left;
	float invheight = top - bottom;
	float clip = zFar - zNear;

	AnimaMatrix m(_engine);
	m._matrixData[0] = 2.0f / width;	m._matrixData[1] = 0.0f;				m._matrixData[2] = 0.0f;			m._matrixData[3] = -(left + right) / width;
	m._matrixData[4] = 0.0f;			m._matrixData[5] = 2.0f / invheight;	m._matrixData[6] = 0.0f;			m._matrixData[7] = -(top + bottom) / invheight;
	m._matrixData[8] = 0.0f;			m._matrixData[9] = 0.0f;				m._matrixData[10] = -2.0f / clip;	m._matrixData[11] = -(zNear + zFar) / clip;
	m._matrixData[12] = 0.0f;			m._matrixData[13] = 0.0f;				m._matrixData[14] = 0.0f;			m._matrixData[15] = 1.0f;

	*this *= m;
}

void AnimaMatrix::LookAt(const AnimaVertex3f& eye, const AnimaVertex3f& forward, const AnimaVertex3f& up)
{
	SetIdentity();

	AnimaVertex3f side = forward ^ up;
	side.Normalize();
	AnimaVertex3f upVector = side ^ forward;
	upVector.Normalize();

	AnimaMatrix m(_engine);
	m._matrixData[0] = side[0];		m._matrixData[1] = side[1];		m._matrixData[2] = side[2];			m._matrixData[3] = 0.0f;
	m._matrixData[4] = upVector[0];	m._matrixData[5] = upVector[1];	m._matrixData[6] = upVector[2];		m._matrixData[7] = 0.0f;
	m._matrixData[8] = -forward[0];	m._matrixData[9] = -forward[1];	m._matrixData[10] = -forward[2];	m._matrixData[11] = 0.0f;
	m._matrixData[12] = 0.0f;		m._matrixData[13] = 0.0f;		m._matrixData[14] = 0.0f;			m._matrixData[15] = 1.0f;

	*this *= m;
	Translate(-eye[0], -eye[1], -eye[2]);
}

void AnimaMatrix::Viewport(float left, float bottom, float width, float height, float zNear, float zFar)
{
	const float w2 = width / 2.0f;
	const float h2 = height / 2.0f;

	AnimaMatrix m(_engine);
	m._matrixData[0] = w2;		m._matrixData[1] = 0.0f;	m._matrixData[2] = 0.0f;					m._matrixData[3] = left + w2;
	m._matrixData[4] = 0.0f;	m._matrixData[5] = h2;		m._matrixData[6] = 0.0f;					m._matrixData[7] = bottom + h2;
	m._matrixData[8] = 0.0f;	m._matrixData[9] = 0.0f;	m._matrixData[10] = (zFar - zNear) / 2.0f;	m._matrixData[11] = (zNear + zFar) / 2.0f;
	m._matrixData[12] = 0.0f;	m._matrixData[13] = 0.0f;	m._matrixData[14] = 0.0f;					m._matrixData[15] = 1.0f;

	*this *= m;
}

void AnimaMatrix::Frustum(float left, float right, float bottom, float top, float zNear, float zFar)
{
	if (left == right || bottom == top || zNear == zFar)
		return;

	float width = right - left;
	float invheight = top - bottom;
	float clip = zFar - zNear;

	AnimaMatrix m(_engine);
	m._matrixData[0] = 2.0f * zNear / width;	m._matrixData[1] = 0.0f;						m._matrixData[2] = (left + right) / width;		m._matrixData[3] = 0.0f;
	m._matrixData[4] = 0.0f;					m._matrixData[5] = 2.0f * zNear / invheight;	m._matrixData[6] = (top + bottom) / invheight;	m._matrixData[7] = 0.0f;
	m._matrixData[8] = 0.0f;					m._matrixData[9] = 0.0f;						m._matrixData[10] = -(zNear + zFar) / clip;		m._matrixData[11] = -2.0f * zNear * zFar / clip;
	m._matrixData[12] = 0.0f;					m._matrixData[13] = 0.0f;						m._matrixData[14] = -1.0f;						m._matrixData[15] = 0.0f;

	*this *= m;
}

void AnimaMatrix::FromHeadPitchRoll(AFloat head, AFloat pitch, AFloat roll)
{
	float cosH = cosf(head);
	float cosP = cosf(pitch);
	float cosR = cosf(roll);
	float sinH = sinf(head);
	float sinP = sinf(pitch);
	float sinR = sinf(roll);

	_matrixData[0] = cosR * cosH - sinR * sinP * sinH;
	_matrixData[1] = sinR * cosH + cosR * sinP * sinH;
	_matrixData[2] = -cosP * sinH;
	_matrixData[3] = 0.0f;

	_matrixData[4] = -sinR * cosP;
	_matrixData[5] = cosR * cosP;
	_matrixData[6] = sinP;
	_matrixData[7] = 0.0f;

	_matrixData[8] = cosR * sinH + sinR * sinP * cosH;
	_matrixData[9] = sinR * sinH - cosR * sinP * cosH;
	_matrixData[10] = cosP * cosH;
	_matrixData[11] = 0.0f;

	_matrixData[12] = 0.0f;
	_matrixData[13] = 0.0f;
	_matrixData[14] = 0.0f;
	_matrixData[15] = 1.0f;
	//_matrixData[0] = cosR * cosH - sinR * sinP * sinH;
	//_matrixData[4] = sinR * cosH + cosR * sinP * sinH;
	//_matrixData[8] = -cosP * sinH;
	//_matrixData[12] = 0.0f;

	//_matrixData[1] = -sinR * cosP;
	//_matrixData[5] = cosR * cosP;
	//_matrixData[9] = sinP;
	//_matrixData[13] = 0.0f;

	//_matrixData[2] = cosR * sinH + sinR * sinP * cosH;
	//_matrixData[6] = sinR * sinH - cosR * sinP * cosH;
	//_matrixData[10] = cosP * cosH;
	//_matrixData[14] = 0.0f;

	//_matrixData[3] = 0.0f;
	//_matrixData[7] = 0.0f;
	//_matrixData[11] = 0.0f;
	//_matrixData[15] = 1.0f;
}

void AnimaMatrix::FromHeadPitchRollDeg(AFloat head, AFloat pitch, AFloat roll)
{
	FromHeadPitchRoll(head * (AFloat)M_PI / 180.0f, pitch * (AFloat)M_PI / 180.0f, roll * (AFloat)M_PI / 180.0f);
}

void AnimaMatrix::GetHeadPitchRoll(AFloat& head, AFloat& pitch, AFloat& roll) const
{
	float thetaX = asinf(_matrixData[6]);
	float thetaY = 0.0f;
	float thetaZ = 0.0f;

	if (thetaX < ((AFloat)M_PI / 2.0f))
	{
		if (thetaX >((AFloat)M_PI / -2.0f))
		{
			thetaZ = atan2f(-_matrixData[4], _matrixData[5]);
			thetaY = atan2f(-_matrixData[2], _matrixData[10]);
			//thetaZ = atan2f(-_matrixData[1], _matrixData[5]);
			//thetaY = atan2f(-_matrixData[8], _matrixData[10]);
		}
		else
		{
			// Not a unique solution.
			thetaZ = -atan2f(_matrixData[8], _matrixData[0]);
			//thetaZ = -atan2f(_matrixData[2], _matrixData[0]);
			thetaY = 0.0f;
		}
	}
	else
	{
		// Not a unique solution.
		thetaZ = atan2f(_matrixData[8], _matrixData[0]);
		//thetaZ = atan2f(_matrixData[2], _matrixData[0]);
		thetaY = 0.0f;
	}

	head = thetaY;
	pitch = thetaX;
	roll = thetaZ;
}

void AnimaMatrix::GetHeadPitchRollDeg(AFloat& head, AFloat& pitch, AFloat& roll) const
{
	GetHeadPitchRoll(head, pitch, roll);
	head = head * 180.0f / (AFloat)M_PI;
	pitch = pitch * 180.0f / (AFloat)M_PI;
	roll = roll * 180.0f / (AFloat)M_PI;
}

void AnimaMatrix::InitTranslation(float tx, float ty, float tz)
{
	_matrixData[0] =	1.0f;	_matrixData[1] =	0.0f;	_matrixData[2] =	0.0f;	_matrixData[3] = 0.0f;
	_matrixData[4] =	0.0f;	_matrixData[5] =	1.0f;	_matrixData[6] =	0.0f;	_matrixData[7] = 0.0f;
	_matrixData[8] =	0.0f;	_matrixData[9] =	0.0f;	_matrixData[10] =	1.0f;	_matrixData[11] = 0.0f;
	_matrixData[12] =	tx;		_matrixData[13] =	ty;		_matrixData[14] =	tz;		_matrixData[15] = 1.0f;
}

void AnimaMatrix::InitTranslation(const AnimaVertex3f& translation)
{
	InitTranslation(translation[0], translation[1], translation[2]);
}

void AnimaMatrix::InitRotation(float rx, float ry, float rz)
{
	AnimaMatrix mx(_engine);
	AnimaMatrix my(_engine);
	AnimaMatrix mz(_engine);
	
	//mz._matrixData[0] = cosf(rz);	mz._matrixData[1] = -sinf(rz);	mz._matrixData[2] = 0.0f;	mz._matrixData[3] = 0.0f;
	//mz._matrixData[4] = sinf(rz);	mz._matrixData[5] = cosf(rz);	mz._matrixData[6] = 0.0f;	mz._matrixData[7] = 0.0f;
	//mz._matrixData[8] = 0.0f;		mz._matrixData[9] = 0.0f;		mz._matrixData[10] = 1.0f;	mz._matrixData[11] = 0.0f;
	//mz._matrixData[12] = 0.0f;		mz._matrixData[13] = 0.0f;		mz._matrixData[14] = 0.0f;	mz._matrixData[15] = 1.0f;

	//mx._matrixData[0] = 1.0f;	mx._matrixData[1] = 0.0f;		mx._matrixData[2] = 0.0f;		mx._matrixData[3] = 0.0f;
	//mx._matrixData[4] = 0.0f;	mx._matrixData[5] = cosf(rx);	mx._matrixData[6] = -sinf(rx);	mx._matrixData[7] = 0.0f;
	//mx._matrixData[8] = 0.0f;	mx._matrixData[9] = sinf(rx);	mx._matrixData[10] = cosf(rx);	mx._matrixData[11] = 0.0f;
	//mx._matrixData[12] = 0.0f;	mx._matrixData[13] = 0.0f;		mx._matrixData[14] = 0.0f;		mx._matrixData[15] = 1.0f;

	//my._matrixData[0] = cosf(ry);	my._matrixData[1] = 0.0f;	my._matrixData[2] = -sinf(ry);	my._matrixData[3] = 0.0f;
	//my._matrixData[4] = 0.0f;		my._matrixData[5] = 1.0f;	my._matrixData[6] = 0.0f;		my._matrixData[7] = 0.0f;
	//my._matrixData[8] = sinf(ry);	my._matrixData[9] = 0.0f;	my._matrixData[10] = cosf(ry);	my._matrixData[11] = 0.0f;
	//my._matrixData[12] = 0.0f;		my._matrixData[13] = 0.0f;	my._matrixData[14] = 0.0f;		my._matrixData[15] = 1.0f;

	mz._matrixData[0] = cosf(rz);	mz._matrixData[4] = -sinf(rz);	mz._matrixData[8] = 0.0f;	mz._matrixData[12] = 0.0f;
	mz._matrixData[1] = sinf(rz);	mz._matrixData[5] = cosf(rz);	mz._matrixData[9] = 0.0f;	mz._matrixData[13] = 0.0f;
	mz._matrixData[2] = 0.0f;		mz._matrixData[6] = 0.0f;		mz._matrixData[10] = 1.0f;	mz._matrixData[14] = 0.0f;
	mz._matrixData[3] = 0.0f;		mz._matrixData[7] = 0.0f;		mz._matrixData[11] = 0.0f;	mz._matrixData[15] = 1.0f;

	mx._matrixData[0] = 1.0f;	mx._matrixData[4] = 0.0f;		mx._matrixData[8] = 0.0f;		mx._matrixData[12] = 0.0f;
	mx._matrixData[1] = 0.0f;	mx._matrixData[5] = cosf(rx);	mx._matrixData[9] = -sinf(rx);	mx._matrixData[13] = 0.0f;
	mx._matrixData[2] = 0.0f;	mx._matrixData[6] = sinf(rx);	mx._matrixData[10] = cosf(rx);	mx._matrixData[14] = 0.0f;
	mx._matrixData[3] = 0.0f;	mx._matrixData[7] = 0.0f;		mx._matrixData[11] = 0.0f;		mx._matrixData[15] = 1.0f;

	my._matrixData[0] = cosf(ry);	my._matrixData[4] = 0.0f;	my._matrixData[8] = -sinf(ry);	my._matrixData[12] = 0.0f;
	my._matrixData[1] = 0.0f;		my._matrixData[5] = 1.0f;	my._matrixData[9] = 0.0f;		my._matrixData[13] = 0.0f;
	my._matrixData[2] = sinf(ry);	my._matrixData[6] = 0.0f;	my._matrixData[10] = cosf(ry);	my._matrixData[14] = 0.0f;
	my._matrixData[3] = 0.0f;		my._matrixData[7] = 0.0f;	my._matrixData[11] = 0.0f;		my._matrixData[15] = 1.0f;

	SetIdentity();
	*this =  mz * my * mx;
}

void AnimaMatrix::InitRotation(const AnimaVertex3f& rotation)
{
	InitRotation(rotation[0], rotation[1], rotation[2]);
}

void AnimaMatrix::InitRotationDeg(float rx, float ry, float rz)
{
	rx = rx * (float)M_PI / 180.0f;
	ry = ry * (float)M_PI / 180.0f;
	rz = rz * (float)M_PI / 180.0f;
	InitRotation(rx, ry, rz);
}

void AnimaMatrix::InitRotationDeg(const AnimaVertex3f& rotation)
{
	InitRotation(rotation * ((float)M_PI / 180.0f));
}

void AnimaMatrix::InitScale(float sx, float sy, float sz)
{
	_matrixData[0] = sx;	_matrixData[1] = 0.0f;	_matrixData[2] = 0.0f;	_matrixData[3] = 0.0f;
	_matrixData[4] = 0.0f;	_matrixData[5] = sy;	_matrixData[6] = 0.0f;	_matrixData[7] = 0.0f;
	_matrixData[8] = 0.0f;	_matrixData[9] = 0.0f;	_matrixData[10] = sz;	_matrixData[11] = 0.0f;
	_matrixData[12] = 0.0f;	_matrixData[13] = 0.0f;	_matrixData[14] = 0.0f;	_matrixData[15] = 1.0f;
}

void AnimaMatrix::InitScale(const AnimaVertex3f& scale)
{
	InitScale(scale[0], scale[1], scale[2]);
}

#undef _mm_shufd

END_ANIMA_ENGINE_NAMESPACE












