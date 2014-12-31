#include "AnimaMatrix.h"
#include "AnimaAssert.h"
#include <xmmintrin.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

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
	__m128 row1 = _mm_load_ps(&src._matrixData[0]);
	__m128 row2 = _mm_load_ps(&src._matrixData[4]);
	__m128 row3 = _mm_load_ps(&src._matrixData[8]);
	__m128 row4 = _mm_load_ps(&src._matrixData[12]);
	
	for(int i = 0; i < 4; i++)
	{
		__m128 brod1 = _mm_set1_ps(this->_matrixData[4*i + 0]);
		__m128 brod2 = _mm_set1_ps(this->_matrixData[4*i + 1]);
		__m128 brod3 = _mm_set1_ps(this->_matrixData[4*i + 2]);
		__m128 brod4 = _mm_set1_ps(this->_matrixData[4*i + 3]);
		__m128 row = _mm_add_ps(
								_mm_add_ps(
										   _mm_mul_ps(brod1, row1),
										   _mm_mul_ps(brod2, row2)),
								_mm_add_ps(
										   _mm_mul_ps(brod3, row3),
										   _mm_mul_ps(brod4, row4)));
		_mm_store_ps(&this->_matrixData[4*i], row);
	}
	
	return *this;
}

AnimaMatrix& AnimaMatrix::operator/=(const AnimaMatrix& src)
{
	AnimaMatrix inverse = src.Inverse();
	this->operator*=(inverse);
	
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
	AFloat* src = _matrixData;
	
	__m128 minor0,minor1,minor2,minor3;
	__m128 row0,row1,row2,row3;
	__m128 det,tmp1;
	
	tmp1= _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
	row1= _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));
	row0= _mm_shuffle_ps(tmp1, row1, 0x88);
	row1= _mm_shuffle_ps(row1, tmp1, 0xDD);
	tmp1= _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
	row3= _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));
	row2= _mm_shuffle_ps(tmp1, row3, 0x88);
	row3= _mm_shuffle_ps(row3, tmp1, 0xDD);
	
	tmp1= _mm_mul_ps(row2, row3);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0= _mm_mul_ps(row1, tmp1);
	minor1= _mm_mul_ps(row0, tmp1);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0= _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
	minor1= _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
	minor1= _mm_shuffle_ps(minor1, minor1, 0x4E);
	
	tmp1= _mm_mul_ps(row1, row2);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor0= _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
	minor3= _mm_mul_ps(row0, tmp1);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0= _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
	minor3= _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
	minor3= _mm_shuffle_ps(minor3, minor3, 0x4E);
	
	tmp1= _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	row2= _mm_shuffle_ps(row2, row2, 0x4E);
	minor0= _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
	minor2= _mm_mul_ps(row0, tmp1);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor0= _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
	minor2= _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
	minor2= _mm_shuffle_ps(minor2, minor2, 0x4E);
	
	tmp1= _mm_mul_ps(row0, row1);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	
	minor2= _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3= _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor2= _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
	minor3= _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
	
	tmp1= _mm_mul_ps(row0, row3);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1= _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
	minor2= _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1= _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
	minor2= _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
	
	tmp1= _mm_mul_ps(row0, row2);
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0xB1);
	minor1= _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
	minor3= _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
	tmp1= _mm_shuffle_ps(tmp1, tmp1, 0x4E);
	minor1= _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
	minor3= _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
	// -----------------------------------------------
	// -----------------------------------------------
	// -----------------------------------------------
	det= _mm_mul_ps(row0, minor0);
	det= _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
	det= _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
	tmp1= _mm_rcp_ss(det);
	det= _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
	det= _mm_shuffle_ps(det, det, 0x00);
	minor0 = _mm_mul_ps(det, minor0);
	_mm_storel_pi((__m64*)(src), minor0);
	_mm_storeh_pi((__m64*)(src+2), minor0);
	minor1 = _mm_mul_ps(det, minor1);
	_mm_storel_pi((__m64*)(src+4), minor1);
	_mm_storeh_pi((__m64*)(src+6), minor1);
	minor2 = _mm_mul_ps(det, minor2);
	_mm_storel_pi((__m64*)(src+ 8), minor2);
	_mm_storeh_pi((__m64*)(src+10), minor2);
	minor3 = _mm_mul_ps(det, minor3);
	_mm_storel_pi((__m64*)(src+12), minor3);
	_mm_storeh_pi((__m64*)(src+14), minor3);
	
//	__m128 minor0, minor1, minor2, minor3;
//	__m128 row0, row1, row2, row3;
//	__m128 det, tmp1;
//
//	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src + 4));
//	row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src + 8)), (__m64*)(src + 12));
//	
//	row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
//	row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);
//	
//	tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src + 2)), (__m64*)(src + 6));
//	row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src + 10)), (__m64*)(src + 14));
//	
//	row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
//	row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);
//	
//	tmp1 = _mm_mul_ps(row2, row3);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	
//	minor0 = _mm_mul_ps(row1, tmp1);
//	minor1 = _mm_mul_ps(row0, tmp1);
//	
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//	
//	minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
//	minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
//	minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
//	
//	tmp1 = _mm_mul_ps(row1, row2);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	
//	minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
//	minor3 = _mm_mul_ps(row0, tmp1);
//	
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//	
//	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
//	minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
//	minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
//	
//	tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	row2 = _mm_shuffle_ps(row2, row2, 0x4E);
//	
//	minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
//	minor2 = _mm_mul_ps(row0, tmp1);
//	
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//	
//	minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
//	minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
//	minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
//	
//	tmp1 = _mm_mul_ps(row0, row1);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	
//	minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
//	minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);
//	
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//	
//	minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
//	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));
//	
//	tmp1 = _mm_mul_ps(row0, row3);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	
//	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
//	minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);
//	
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//	
//	minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
//	minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));
//	
//	tmp1 = _mm_mul_ps(row0, row2);
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
//	
//	minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
//	minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));
//	
//	tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);
//	
//	minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
//	minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);
//	
//	det = _mm_mul_ps(row0, minor0);
//	det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
//	det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
//	tmp1 = _mm_rcp_ss(det);
//	
//	det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
//	det = _mm_shuffle_ps(det, det, 0x00);
//	
//	minor0 = _mm_mul_ps(det, minor0);
//	_mm_storel_pi((__m64*)(src), minor0);
//	_mm_storeh_pi((__m64*)(src + 2), minor0);
//	
//	minor1 = _mm_mul_ps(det, minor1);
//	_mm_storel_pi((__m64*)(src + 4), minor1);
//	_mm_storeh_pi((__m64*)(src + 6), minor1);
//	
//	minor2 = _mm_mul_ps(det, minor2);
//	_mm_storel_pi((__m64*)(src + 8), minor2);
//	_mm_storeh_pi((__m64*)(src + 10), minor2);
//	
//	minor3 = _mm_mul_ps(det, minor3);
//	_mm_storel_pi((__m64*)(src + 12), minor3);
//	_mm_storeh_pi((__m64*)(src + 14), minor3);
	
	return resultMatrix;
}

void AnimaMatrix::DumpMemory()
{
	printf("%f\t%f\t%f\t%f\n", _matrixData[0], _matrixData[1], _matrixData[2], _matrixData[3]);
	printf("%f\t%f\t%f\t%f\n", _matrixData[4], _matrixData[5], _matrixData[6], _matrixData[7]);
	printf("%f\t%f\t%f\t%f\n", _matrixData[8], _matrixData[9], _matrixData[10], _matrixData[11]);
	printf("%f\t%f\t%f\t%f\n\n", _matrixData[12], _matrixData[13], _matrixData[14], _matrixData[15]);
}

END_ANIMA_ENGINE_NAMESPACE












