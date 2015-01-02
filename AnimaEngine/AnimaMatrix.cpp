#include "AnimaMatrix.h"
#include "AnimaAssert.h"
#include <xmmintrin.h>

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
: _matrixData(src._matrixData)
, _engine(src._engine)
{
	src._matrixData = nullptr;
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
		_matrixData = src._matrixData;
		
		src._matrixData = nullptr;
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

inline AFloat* AnimaMatrix::GetData() const
{
	return _matrixData;
}

inline void AnimaMatrix::GetData(AFloat m[ANIMA_MATRIX_SIZE]) const
{
	memcpy(m, _matrixData, sizeof(AFloat) * ANIMA_MATRIX_SIZE);
}

inline const AFloat* AnimaMatrix::GetConstData() const
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

void AnimaMatrix::DumpMemory()
{
	printf("%f\t%f\t%f\t%f\n", _matrixData[0], _matrixData[1], _matrixData[2], _matrixData[3]);
	printf("%f\t%f\t%f\t%f\n", _matrixData[4], _matrixData[5], _matrixData[6], _matrixData[7]);
	printf("%f\t%f\t%f\t%f\n", _matrixData[8], _matrixData[9], _matrixData[10], _matrixData[11]);
	printf("%f\t%f\t%f\t%f\n\n", _matrixData[12], _matrixData[13], _matrixData[14], _matrixData[15]);
}

#undef _mm_shufd

END_ANIMA_ENGINE_NAMESPACE












