#include "AnimaMatrix.h"
#include "AnimaAssert.h"
#include "AnimaMath.h"
#include <xmmintrin.h>

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

#define _mm_shufd(xmm, mask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), mask))

void swap(AFloat &a, AFloat &b)
{
	AFloat temp = a;
	a = b;
	b = temp;
}

AnimaMatrix::AnimaMatrix()
{
	SetIdentity();
}

AnimaMatrix::AnimaMatrix(const AnimaMatrix& src)
{
	Fill(src.m);
}

AnimaMatrix::AnimaMatrix(AnimaMatrix&& src)
{
	Fill(src.m);
}

AnimaMatrix::AnimaMatrix(const AFloat mat[16])
{
	Fill(mat);
}

AnimaMatrix::~AnimaMatrix()
{
}

AnimaMatrix& AnimaMatrix::operator=(const AnimaMatrix& src)
{
	if (this != &src)
	{
		Fill(src.m);
	}

	return *this;
}

AnimaMatrix& AnimaMatrix::operator=(AnimaMatrix&& src)
{
	if (this != &src)
	{
		Fill(src.m);
	}

	return *this;
}

AnimaVertex3f AnimaMatrix::operator*(const AnimaVertex3f& p) const
{
	float invw;
	invw = 1.0f / (p.vec[0] * m[3] + p.vec[1] * m[7] + p.vec[2] * m[11] + m[15]);

	AnimaVertex3f r;
	r.vec[0] = (p.vec[0] * m[0] + p.vec[1] * m[4] + p.vec[2] * m[8] + m[12]) * invw;
	r.vec[1] = (p.vec[0] * m[1] + p.vec[1] * m[5] + p.vec[2] * m[9] + m[13]) * invw;
	r.vec[2] = (p.vec[0] * m[2] + p.vec[1] * m[6] + p.vec[2] * m[10] + m[14]) * invw;

	return r;
}

AnimaVertex4f AnimaMatrix::operator*(const AnimaVertex4f& p) const
{
	AnimaVertex4f r;
	r.x = m[0] * p.x + m[4] * p.y + m[8] * p.z + m[12] * p.w;
	r.y = m[1] * p.x + m[5] * p.y + m[9] * p.z + m[13] * p.w;
	r.z = m[2] * p.x + m[6] * p.y + m[10] * p.z + m[14] * p.w;
	r.w = m[3] * p.x + m[7] * p.y + m[11] * p.z + m[15] * p.w;

	return r;
}

AnimaMatrix AnimaMatrix::operator*(const AnimaMatrix& p) const
{
	AnimaMatrix r(this->m);
	r.MultiplyMatrix(p.m);
	return r;
}

AnimaMatrix AnimaMatrix::operator*(const AFloat p[16]) const
{
	AnimaMatrix r(this->m);
	r.MultiplyMatrix(p);
	return r;
}

AnimaMatrix& AnimaMatrix::operator*=(const AnimaMatrix& p)
{
	MultiplyMatrix(p.m);
	return *this;
}

AnimaMatrix& AnimaMatrix::operator*=(const AFloat p[16])
{
	MultiplyMatrix(p);
	return *this;
}

AnimaMatrix AnimaMatrix::operator*(const AFloat p) const
{
	AnimaMatrix r(this->m);
	r.MultiplyMatrix(p);
	return r;
}

AnimaMatrix& AnimaMatrix::operator*=(const AFloat p)
{
	MultiplyMatrix(p);
	return *this;
}

AnimaMatrix AnimaMatrix::operator+(const AnimaMatrix& p) const
{
	AnimaMatrix r(this->m);
	r.AddMatrix(p);
	return r;
}

AnimaMatrix& AnimaMatrix::operator+=(const AnimaMatrix& p)
{
	AddMatrix(p);
	return *this;
}

AnimaMatrix AnimaMatrix::operator+(const AFloat p[16]) const
{
	AnimaMatrix r(this->m);
	r.AddMatrix(p);
	return r;
}

AnimaMatrix& AnimaMatrix::operator+=(const AFloat p[16])
{
	AddMatrix(p);
	return *this;
}

AnimaMatrix AnimaMatrix::operator+(const AFloat p) const
{
	AnimaMatrix r(this->m);
	r.AddMatrix(p);
	return r;
}

AnimaMatrix& AnimaMatrix::operator+=(const AFloat p)
{
	AddMatrix(p);
	return *this;
}

bool AnimaMatrix::operator==(const AnimaMatrix& p) const
{
	for (int i = 0; i < 16; i++)
	{
		if (m[i] != p.m[i])
			return false;
	}
	return true;
}

bool AnimaMatrix::operator!=(const AnimaMatrix& p) const
{
	return !(*this == p);
}

bool AnimaMatrix::operator==(const AFloat p[16]) const
{
	for (int i = 0; i < 16; i++)
	{
		if (m[i] != p[i])
			return false;
	}
	return true;
}

bool AnimaMatrix::operator!=(const AFloat p[16]) const
{
	return !(*this == p);
}

void AnimaMatrix::Fill(const AFloat p[16])
{
	m[0] = p[0];
	m[1] = p[1];
	m[2] = p[2];
	m[3] = p[3];
	m[4] = p[4];
	m[5] = p[5];
	m[6] = p[6];
	m[7] = p[7];
	m[8] = p[8];
	m[9] = p[9];
	m[10] = p[10];
	m[11] = p[11];
	m[12] = p[12];
	m[13] = p[13];
	m[14] = p[14];
	m[15] = p[15];
}

AnimaVertex3f AnimaMatrix::GetXVector() const
{
	return AnimaVertex3f(this->x[0], this->x[1], this->x[2]);
}

AnimaVertex3f AnimaMatrix::GetYVector() const
{
	return AnimaVertex3f(this->y[0], this->y[1], this->y[2]);
}

AnimaVertex3f AnimaMatrix::GetZVector() const
{
	return AnimaVertex3f(this->z[0], this->z[1], this->z[2]);
}

AnimaVertex3f AnimaMatrix::GetPosition() const
{
	return AnimaVertex3f(this->w[0], this->w[1], this->w[2]);
}

void AnimaMatrix::SetIdentity()
{
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = 1.0f;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = 1.0f;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void AnimaMatrix::Keep3x3()
{
	m[3] = 0.0f;
	m[7] = 0.0f;
	m[11] = 0.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}

void AnimaMatrix::Transpose()
{
	swap(m[1], m[4]);
	swap(m[2], m[8]);
	swap(m[3], m[12]);
	swap(m[6], m[9]);
	swap(m[7], m[13]);
	swap(m[11], m[14]);
}

void AnimaMatrix::TransposeSSE()
{
	__m128 m1 = _mm_loadu_ps(&m[0]);
	__m128 m2 = _mm_loadu_ps(&m[4]);
	__m128 m3 = _mm_loadu_ps(&m[8]);
	__m128 m4 = _mm_loadu_ps(&m[12]);
	
	__m128 t1 = _mm_unpacklo_ps(m1, m2);
	__m128 t2 = _mm_unpacklo_ps(m3, m4);
	__m128 t3 = _mm_unpackhi_ps(m1, m2);
	__m128 t4 = _mm_unpackhi_ps(m3, m4);
	
	__m128 r1 = _mm_movelh_ps(t1, t2);
	__m128 r2 = _mm_movehl_ps(t2, t1);
	__m128 r3 = _mm_movelh_ps(t3, t4);
	__m128 r4 = _mm_movehl_ps(t4, t3);
	
	_mm_storeu_ps(&m[0], r1);
	_mm_storeu_ps(&m[4], r2);
	_mm_storeu_ps(&m[8], r3);
	_mm_storeu_ps(&m[12], r4);
}

AnimaMatrix AnimaMatrix::Transposed() const
{
	AnimaMatrix mat(this->m);
	mat.Transpose();
	return mat;
}

AnimaMatrix AnimaMatrix::TransposedSSE() const
{
	AnimaMatrix mat(this->m);
	mat.TransposeSSE();
	return mat;
}

void AnimaMatrix::Inverse()
{
	AnimaMatrix minv;

	float r1[8], r2[8], r3[8], r4[8];
	float *s[4], *tmprow;

	s[0] = &r1[0];
	s[1] = &r2[0];
	s[2] = &r3[0];
	s[3] = &r4[0];

	register int i, j, p, jj;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			s[i][j] = m[i * 4 + j];

			if (i == j)
				s[i][j + 4] = 1.0f;
			else
				s[i][j + 4] = 0.0f;
		}
	}

	float scp[4];
	for (i = 0; i < 4; i++)
	{
		scp[i] = float(fabs(s[i][0]));
		for (j = 1; j<4; j++)
		{
			if (float(fabs(s[i][j])) > scp[i])
				scp[i] = float(fabs(s[i][j]));
		}
		if (scp[i] == 0.0f)
			return;
	}

	int pivot_to;
	float scp_max;
	for (i = 0; i < 4; i++)
	{
		// select pivot row
		pivot_to = i;
		scp_max = float(fabs(s[i][i] / scp[i]));

		// find out which row should be on top
		for (p = i + 1; p < 4; p++)
		{
			if (float(fabs(s[p][i] / scp[p])) > scp_max)
			{
				scp_max = float(fabs(s[p][i] / scp[p])); pivot_to = p;
			}
		}

		// Pivot if necessary
		if (pivot_to != i)
		{
			tmprow = s[i];
			s[i] = s[pivot_to];
			s[pivot_to] = tmprow;
			float tmpscp;
			tmpscp = scp[i];
			scp[i] = scp[pivot_to];
			scp[pivot_to] = tmpscp;
		}

		float mji;
		// perform gaussian elimination
		for (j = i + 1; j < 4; j++)
		{
			mji = s[j][i] / s[i][i];
			s[j][i] = 0.0f;
			for (jj = i + 1; jj < 8; jj++)
				s[j][jj] -= mji*s[i][jj];
		}
	}

	if (s[3][3] == 0.0f)
		return;


	float mij;
	for (i = 3; i > 0; i--)
	{
		for (j = i - 1; j > -1; j--)
		{
			mij = s[j][i] / s[i][i];
			for (jj = j + 1; jj < 8; jj++)
				s[j][jj] -= mij*s[i][jj];
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
			minv.m[i * 4 + j] = s[i][j + 4] / s[i][i];
	}

	memcpy(m, minv.m, sizeof(float) * 16);
}

void AnimaMatrix::InverseSSE()
{
	__m128 m1 = _mm_loadu_ps(&m[0]);
	__m128 m2 = _mm_loadu_ps(&m[4]);
	__m128 m3 = _mm_loadu_ps(&m[8]);
	__m128 m4 = _mm_loadu_ps(&m[12]);
	
	__m128 f1 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0xAA), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xFF), 0x80)),
		_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0xAA), 0x80), _mm_shuffle_ps(m3, m2, 0xFF)));
	__m128 f2 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x55), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xFF), 0x80)),
		_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x55), 0x80), _mm_shuffle_ps(m3, m2, 0xFF)));
	__m128 f3 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x55), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xAA), 0x80)),
		_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x55), 0x80), _mm_shuffle_ps(m3, m2, 0xAA)));
	__m128 f4 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x00), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xFF), 0x80)),
		_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x00), 0x80), _mm_shuffle_ps(m3, m2, 0xFF)));
	__m128 f5 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x00), _mm_shufd(_mm_shuffle_ps(m4, m3, 0xAA), 0x80)),
		_mm_mul_ps(_mm_shufd(_mm_shuffle_ps(m4, m3, 0x00), 0x80), _mm_shuffle_ps(m3, m2, 0xAA)));
	__m128 f6 = _mm_sub_ps(_mm_mul_ps(_mm_shuffle_ps(m3, m2, 0x00), _mm_shufd(_mm_shuffle_ps(m4, m3, 0x55), 0x80)),
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
	
	_mm_storeu_ps(&m[0], r1);
	_mm_storeu_ps(&m[4], r2);
	_mm_storeu_ps(&m[8], r3);
	_mm_storeu_ps(&m[12], r4);
}

AnimaMatrix AnimaMatrix::Inversed() const
{
	AnimaMatrix mat(this->m);
	mat.Inverse();
	return mat;
}

AnimaMatrix AnimaMatrix::InversedSSE() const
{
	AnimaMatrix mat(this->m);
	mat.InverseSSE();
	return mat;
}

AFloat AnimaMatrix::Determinant() const
{
	__m128 m1 = _mm_loadu_ps(&m[0]);
	__m128 m2 = _mm_loadu_ps(&m[4]);
	__m128 m3 = _mm_loadu_ps(&m[8]);
	__m128 m4 = _mm_loadu_ps(&m[12]);
	
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

void AnimaMatrix::MultiplyMatrix(const AnimaMatrix& p)
{
	MultiplyMatrix(p.m);
}

void AnimaMatrix::MultiplyMatrix(const AFloat p[16])
{
	AnimaMatrix r;
	r.m[0] = m[0] * p[0] + m[4] * p[1] + m[8] * p[2] + m[12] * p[3];
	r.m[1] = m[1] * p[0] + m[5] * p[1] + m[9] * p[2] + m[13] * p[3];
	r.m[2] = m[2] * p[0] + m[6] * p[1] + m[10] * p[2] + m[14] * p[3];
	r.m[3] = m[3] * p[0] + m[7] * p[1] + m[11] * p[2] + m[15] * p[3];
	r.m[4] = m[0] * p[4] + m[4] * p[5] + m[8] * p[6] + m[12] * p[7];
	r.m[5] = m[1] * p[4] + m[5] * p[5] + m[9] * p[6] + m[13] * p[7];
	r.m[6] = m[2] * p[4] + m[6] * p[5] + m[10] * p[6] + m[14] * p[7];
	r.m[7] = m[3] * p[4] + m[7] * p[5] + m[11] * p[6] + m[15] * p[7];
	r.m[8] = m[0] * p[8] + m[4] * p[9] + m[8] * p[10] + m[12] * p[11];
	r.m[9] = m[1] * p[8] + m[5] * p[9] + m[9] * p[10] + m[13] * p[11];
	r.m[10] = m[2] * p[8] + m[6] * p[9] + m[10] * p[10] + m[14] * p[11];
	r.m[11] = m[3] * p[8] + m[7] * p[9] + m[11] * p[10] + m[15] * p[11];
	r.m[12] = m[0] * p[12] + m[4] * p[13] + m[8] * p[14] + m[12] * p[15];
	r.m[13] = m[1] * p[12] + m[5] * p[13] + m[9] * p[14] + m[13] * p[15];
	r.m[14] = m[2] * p[12] + m[6] * p[13] + m[10] * p[14] + m[14] * p[15];
	r.m[15] = m[3] * p[12] + m[7] * p[13] + m[11] * p[14] + m[15] * p[15];
	memcpy(m, r.m, sizeof(float) * 16);
}

void AnimaMatrix::MultiplyMatrix(const AFloat p)
{
	for(AInt i = 0; i < 16; i++)
		m[i] *= p;
}

void AnimaMatrix::AddMatrix(const AnimaMatrix& p)
{
	for(AInt i = 0; i < 16; i++)
		m[i] += p.m[i];
}

void AnimaMatrix::AddMatrix(const AFloat p[16])
{
	for(AInt i = 0; i < 16; i++)
		m[i] += p[i];
}

void AnimaMatrix::AddMatrix(const AFloat p)
{
	for(AInt i = 0; i < 16; i++)
		m[i] += p;
}

void AnimaMatrix::MultiplyMatrixSSE(const AnimaMatrix& p)
{
	MultiplyMatrixSSE(p.m);

	//__m128 src1 = _mm_loadu_ps(&p.m[0]);
	//__m128 src2 = _mm_loadu_ps(&p.m[4]);
	//__m128 src3 = _mm_loadu_ps(&p.m[8]);
	//__m128 src4 = _mm_loadu_ps(&p.m[12]);
	//
	//__m128 r1 = _mm_loadu_ps(&m[0]);
	//__m128 r2 = _mm_loadu_ps(&m[4]);
	//__m128 r3 = _mm_loadu_ps(&m[8]);
	//__m128 r4 = _mm_loadu_ps(&m[12]);
	//
	//r1 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r1, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r1, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r1, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r1, 0xFF), src4));
	//r2 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r2, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r2, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r2, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r2, 0xFF), src4));
	//r3 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r3, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r3, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r3, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r3, 0xFF), src4));
	//r4 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r4, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r4, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r4, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r4, 0xFF), src4));
	//
	//_mm_storeu_ps(&m[0], r1);
	//_mm_storeu_ps(&m[4], r2);
	//_mm_storeu_ps(&m[8], r3);
	//_mm_storeu_ps(&m[12], r4);
}

void AnimaMatrix::MultiplyMatrixSSE(const AFloat p[16])
{
	// a: corrisponde a this->m
	// b: corrisponde a p

	AFloat r[16];

	__m128 a_line, b_line, r_line;
	for (AInt i = 0; i < 16; i += 4) 
	{
		// unroll the first step of the loop to avoid having to initialize r_line to zero
		a_line = _mm_load_ps(this->m);									// a_line = vec4(column(a, 0))
		b_line = _mm_set1_ps(p[i]);										// b_line = vec4(b[i][0])
		r_line = _mm_mul_ps(a_line, b_line);							// r_line = a_line * b_line
		for (int j = 1; j < 4; j++) 
		{
			a_line = _mm_load_ps(&this->m[j * 4]);						// a_line = vec4(column(a, j))
			b_line = _mm_set1_ps(p[i + j]);								// b_line = vec4(b[i][j])
																		// r_line += a_line * b_line
			r_line = _mm_add_ps(_mm_mul_ps(a_line, b_line), r_line);
		}

		_mm_store_ps(&r[i], r_line);									// r[i] = r_line
	}

	memcpy(this->m, r, sizeof(AFloat) * 16);

	//__m128 src1 = _mm_loadu_ps(&p[0]);	// _mm_set_ps(p[0], p[4], p[8], p[12]);//	
	//__m128 src2 = _mm_loadu_ps(&p[4]);	// _mm_set_ps(p[1], p[5], p[8], p[13]);//	
	//__m128 src3 = _mm_loadu_ps(&p[8]);	// _mm_set_ps(p[2], p[6], p[10], p[14]);//	
	//__m128 src4 = _mm_loadu_ps(&p[12]);	// _mm_set_ps(p[3], p[7], p[11], p[15]);//	

	//__m128 r1 = _mm_loadu_ps(&m[0]);	// _mm_set_ps(m[0], m[4], m[8], m[12]);//	
	//__m128 r2 = _mm_loadu_ps(&m[4]);	// _mm_set_ps(m[1], m[5], m[8], m[13]);//	
	//__m128 r3 = _mm_loadu_ps(&m[8]);	// _mm_set_ps(m[2], m[6], m[10], m[14]);//	
	//__m128 r4 = _mm_loadu_ps(&m[12]);	// _mm_set_ps(m[3], m[7], m[11], m[15]);//	

	//r1 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r1, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r1, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r1, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r1, 0xFF), src4));
	//r2 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r2, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r2, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r2, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r2, 0xFF), src4));
	//r3 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r3, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r3, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r3, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r3, 0xFF), src4));
	//r4 = _mm_add_ps(_mm_add_ps(
	//	_mm_add_ps(_mm_mul_ps(_mm_shufd(r4, 0x00), src1),
	//	_mm_mul_ps(_mm_shufd(r4, 0x55), src2)),
	//	_mm_mul_ps(_mm_shufd(r4, 0xAA), src3)),
	//	_mm_mul_ps(_mm_shufd(r4, 0xFF), src4));

	//_mm_storeu_ps(&m[0], r1);
	//_mm_storeu_ps(&m[4], r2);
	//_mm_storeu_ps(&m[8], r3);
	//_mm_storeu_ps(&m[12], r4);

	////float fr1[4], fr2[4], fr3[4], fr4[4];

	////_mm_storeu_ps(fr1, r1);
	////_mm_storeu_ps(fr2, r2);
	////_mm_storeu_ps(fr3, r3);
	////_mm_storeu_ps(fr4, r4);

	////m[0] = fr1[0];	m[4] = fr1[1];	m[8] = fr1[2];	m[12] = fr1[3];
	////m[1] = fr2[0];	m[5] = fr2[1];	m[9] = fr2[2];	m[13] = fr2[3];
	////m[2] = fr3[0];	m[6] = fr3[1];	m[10] = fr3[2];	m[14] = fr3[3];
	////m[3] = fr4[0];	m[7] = fr4[1];	m[11] = fr4[2];	m[15] = fr4[3];
}

void AnimaMatrix::LookAt(const AnimaVertex3f &position, const AnimaVertex3f &forward, const AnimaVertex3f &up)
{
	SetIdentity();
	
//	tvec3<T, P> const f(normalize(center - eye));
//	tvec3<T, P> const s(normalize(cross(f, up)));
//	tvec3<T, P> const u(cross(s, f));
//	
//	tmat4x4<T, P> Result(1);
//	Result[0][0] = s.x;
//	Result[1][0] = s.y;
//	Result[2][0] = s.z;
//	Result[0][1] = u.x;
//	Result[1][1] = u.y;
//	Result[2][1] = u.z;
//	Result[0][2] =-f.x;
//	Result[1][2] =-f.y;
//	Result[2][2] =-f.z;
//	Result[3][0] =-dot(s, eye);
//	Result[3][1] =-dot(u, eye);
//	Result[3][2] = dot(f, eye);
	
	AnimaVertex3f f, s, u;

	f = forward.Normalized();			// asse z
	s = Cross(f, up).Normalized();		// asse x
	u = Cross(s, f);					// asse y

	m[0] = s.vec[0];
	m[4] = s.vec[1];
	m[8] = s.vec[2];

	m[1] = u.vec[0];
	m[5] = u.vec[1];
	m[9] = u.vec[2];

	m[2] = -f.vec[0];
	m[6] = -f.vec[1];
	m[10] = -f.vec[2];
	
	m[12] = -(s * position);
	m[13] = -(u * position);
	m[14] = (f * position);

	//Translate(-position);
}

void AnimaMatrix::LookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xForward, AFloat yForward, AFloat zForward, AFloat xUp, AFloat yUp, AFloat zUp)
{
	LookAt(AnimaVertex3f(xPosition, yPosition, zPosition), AnimaVertex3f(xForward, yForward, zForward), AnimaVertex3f(xUp, yUp, zUp));
}

AnimaMatrix AnimaMatrix::MakeLookAt(const AnimaVertex3f &position, const AnimaVertex3f &forward, const AnimaVertex3f &up)
{
	AnimaMatrix mat;
	mat.LookAt(position, forward, up);
	return mat;
}

AnimaMatrix AnimaMatrix::MakeLookAt(AFloat xPosition, AFloat yPosition, AFloat zPosition, AFloat xForward, AFloat yForward, AFloat zForward, AFloat xUp, AFloat yUp, AFloat zUp)
{
	AnimaMatrix mat;
	mat.LookAt(xPosition, yPosition, zPosition, xForward, yForward, zForward, xUp, yUp, zUp);
	return mat;
}

void AnimaMatrix::Perspective(AFloat fov, AFloat aspect, AFloat zNear, AFloat zFar)
{
	SetIdentity();
	
//	T const tanHalfFovy = tan(fovy / static_cast<T>(2));
//	
//	tmat4x4<T, defaultp> Result(static_cast<T>(0));
//	Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
//	Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
//	Result[2][2] = - (zFar + zNear) / (zFar - zNear);
//	Result[2][3] = - static_cast<T>(1);
//	Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

//	float radians = (fov * 0.5f) * (float)M_PI / 180.0f;
//	float sine = sinf(radians);
//
//	if (sine == 0.0f)
//		return;
//
//	float cotan = cosf(radians) / sine;
//	float clip = zFar - zNear;
//
//	m[0] = cotan / aspect;	m[1] = 0.0f;	m[2] = 0.0f;							m[3] = 0.0f;
//	m[4] = 0.0f;			m[5] = cotan;	m[6] = 0.0f;							m[7] = 0.0f;
//	m[8] = 0.0f;			m[9] = 0.0f;	m[10] = -(zNear + zFar) / clip;			m[11] = -1.0f;
//	m[12] = 0.0f;			m[13] = 0.0f;	m[14] = -(2.0f * zNear * zFar) / clip;	m[15] = 0.0f;

	AFloat fovYRad = (fov * 0.5f) * (AFloat)M_PI / 180.0f;
	AFloat tanHalfFovy = tanf(fovYRad / 2.0f);
	float clip = zFar - zNear;
	
	m[0] = 1.0f / (aspect * tanHalfFovy);	m[1] = 0.0f;				m[2] = 0.0f;							m[3] = 0.0f;
	m[4] = 0.0f;							m[5] = 1.0f / tanHalfFovy;	m[6] = 0.0f;							m[7] = 0.0f;
	m[8] = 0.0f;							m[9] = 0.0f;				m[10] = -(zNear + zFar) / clip;			m[11] = -1.0f;
	m[12] = 0.0f;							m[13] = 0.0f;				m[14] = -(2.0f * zNear * zFar) / clip;	m[15] = 0.0f;
}

AnimaMatrix AnimaMatrix::MakePerspective(AFloat fov, AFloat aspect, AFloat zNear, AFloat zFar)
{
	AnimaMatrix mat;
	mat.Perspective(fov, aspect, zNear, zFar);
	return mat;
}

void AnimaMatrix::Ortho(AFloat left, AFloat right, AFloat bottom, AFloat top, AFloat zNear, AFloat zFar)
{
	SetIdentity();
	
	float width = (right - left);
	float height = (top - bottom);
	float depth = (zFar - zNear);

	m[0] = 2.0f / width;				m[1] = 0.0f;						m[2] = 0.0f;						m[3] = 0.0f;
	m[4] = 0.0f;						m[5] = 2.0f / height;				m[6] = 0.0f;						m[7] = 0.0f;
	m[8] = 0.0f;						m[9] = 0.0f;						m[10] = -2.0f / depth;				m[11] = 0.0f;
	m[12] = -(right + left) / width;	m[13] = -(top + bottom) / height;	m[14] = -(zFar + zNear) / depth;	m[15] = 1.0f;
}

AnimaMatrix AnimaMatrix::MakeOrtho(AFloat left, AFloat right, AFloat bottom, AFloat top, AFloat zNear, AFloat zFar)
{
	AnimaMatrix mat;
	mat.Ortho(left, right, bottom, top, zNear, zFar);
	return mat;
}

void AnimaMatrix::Translate(const AnimaVertex3f& v)
{
	AnimaVertex4f m0(this->x);
	AnimaVertex4f m1(this->y);
	AnimaVertex4f m2(this->z);
	AnimaVertex4f m3(this->w);

	AnimaVertex4f res = m0 * v.x + m1 * v.y + m2 * v.z + m3;

	m[12] = res.x;
	m[13] = res.y;
	m[14] = res.z;
	m[15] = res.w;
}

void AnimaMatrix::Translate(const AFloat x, const AFloat y, const AFloat z)
{
	AnimaVertex4f m0(this->x);
	AnimaVertex4f m1(this->y);
	AnimaVertex4f m2(this->z);
	AnimaVertex4f m3(this->w);

	AnimaVertex4f res = m0 * x + m1 * y + m2 * z + m3;

	m[12] = res.x;
	m[13] = res.y;
	m[14] = res.z;
	m[15] = res.w;
}

AnimaMatrix AnimaMatrix::MakeTranslation(const AnimaVertex3f& v)
{
	AnimaMatrix mat;
	mat.m[0] = 1.0f;
	mat.m[1] = 0.0f;
	mat.m[2] = 0.0f;
	mat.m[3] = 0.0f;
	mat.m[4] = 0.0f;
	mat.m[5] = 1.0f;
	mat.m[6] = 0.0f;
	mat.m[7] = 0.0f;
	mat.m[8] = 0.0f;
	mat.m[9] = 0.0f;
	mat.m[10] = 1.0f;
	mat.m[11] = 0.0f;
	mat.m[12] = v.x;
	mat.m[13] = v.y;
	mat.m[14] = v.z;
	mat.m[15] = 1.0f;
	return mat;
}

AnimaMatrix AnimaMatrix::MakeTranslation(const AFloat x, const AFloat y, const AFloat z)
{
	AnimaMatrix mat;
	mat.m[0] = 1.0f;
	mat.m[1] = 0.0f;
	mat.m[2] = 0.0f;
	mat.m[3] = 0.0f;
	mat.m[4] = 0.0f;
	mat.m[5] = 1.0f;
	mat.m[6] = 0.0f;
	mat.m[7] = 0.0f;
	mat.m[8] = 0.0f;
	mat.m[9] = 0.0f;
	mat.m[10] = 1.0f;
	mat.m[11] = 0.0f;
	mat.m[12] = x;
	mat.m[13] = y;
	mat.m[14] = z;
	mat.m[15] = 1.0f;
	return mat;
}

void AnimaMatrix::Scale(const AnimaVertex4f& v)
{
	AnimaVertex4f m0(this->x);
	AnimaVertex4f m1(this->y);
	AnimaVertex4f m2(this->z);
	AnimaVertex4f m3(this->w);
	
	AnimaVertex4f r0 = m0 * v.x;
	AnimaVertex4f r1 = m1 * v.y;
	AnimaVertex4f r2 = m2 * v.z;
	AnimaVertex4f r3 = m3;
	
	m[0] = r0.x;	m[1] = r0.y;	m[2] = r0.z;	m[3] = r0.w;
	m[4] = r1.x;	m[5] = r1.y;	m[6] = r1.z;	m[7] = r1.w;
	m[8] = r2.x;	m[9] = r2.y;	m[10] = r2.z;	m[11] = r2.w;
	m[12] = r3.x;	m[13] = r3.y;	m[14] = r3.z;	m[15] = r3.w;
}

void AnimaMatrix::Scale(const AFloat x, const AFloat y, const AFloat z, const AFloat w)
{
	AnimaVertex4f m0(this->x);
	AnimaVertex4f m1(this->y);
	AnimaVertex4f m2(this->z);
	AnimaVertex4f m3(this->w);

	AnimaVertex4f r0 = m0 * x;
	AnimaVertex4f r1 = m1 * y;
	AnimaVertex4f r2 = m2 * z;
	AnimaVertex4f r3 = m3;

	m[0] = r0.x;	m[1] = r0.y;	m[2] = r0.z;	m[3] = r0.w;
	m[4] = r1.x;	m[5] = r1.y;	m[6] = r1.z;	m[7] = r1.w;
	m[8] = r2.x;	m[9] = r2.y;	m[10] = r2.z;	m[11] = r2.w;
	m[12] = r3.x;	m[13] = r3.y;	m[14] = r3.z;	m[15] = r3.w;
}

AnimaMatrix AnimaMatrix::MakeScale(const AnimaVertex4f& v)
{
	AnimaMatrix mat;
	mat.m[0] = v.x;
	mat.m[1] = 0.0f;
	mat.m[2] = 0.0f;
	mat.m[3] = 0.0f;
	mat.m[4] = 0.0f;
	mat.m[5] = v.y;
	mat.m[6] = 0.0f;
	mat.m[7] = 0.0f;
	mat.m[8] = 0.0f;
	mat.m[9] = 0.0f;
	mat.m[10] = v.z;
	mat.m[11] = 0.0f;
	mat.m[12] = 0.0f;
	mat.m[13] = 0.0f;
	mat.m[14] = 0.0f;
	mat.m[15] = v.w;
	return mat;
}

AnimaMatrix AnimaMatrix::MakeScale(const AFloat x, const AFloat y, const AFloat z, const AFloat w)
{
	AnimaMatrix mat;
	mat.m[0] = x;
	mat.m[1] = 0.0f;
	mat.m[2] = 0.0f;
	mat.m[3] = 0.0f;
	mat.m[4] = 0.0f;
	mat.m[5] = y;
	mat.m[6] = 0.0f;
	mat.m[7] = 0.0f;
	mat.m[8] = 0.0f;
	mat.m[9] = 0.0f;
	mat.m[10] = z;
	mat.m[11] = 0.0f;
	mat.m[12] = 0.0f;
	mat.m[13] = 0.0f;
	mat.m[14] = 0.0f;
	mat.m[15] = w;
	return mat;
}

void AnimaMatrix::RotateRad(const AnimaVertex3f& v, AFloat rad)
{
	float a = rad;
	float c = cosf(a);
	float s = sinf(a);

	AnimaVertex3f ax = v;
	ax.Normalize();
	AnimaVertex3f temp = ((float(1) - c) * v);

	AnimaMatrix rotMatrix;
	rotMatrix.m[0] = c + temp.x * ax.x;
	rotMatrix.m[1] = 0 + temp.x * ax.y + s * ax.z;
	rotMatrix.m[2] = 0 + temp.x * ax.z - s * ax.y;

	rotMatrix.m[4] = 0 + temp.y * ax.x - s * ax.z;
	rotMatrix.m[5] = c + temp.y * ax.y;
	rotMatrix.m[6] = 0 + temp.y * ax.z + s * ax.x;

	rotMatrix.m[8] = 0 + temp.z * ax.x + s * ax.y;
	rotMatrix.m[9] = 0 + temp.z * ax.y - s * ax.x;
	rotMatrix.m[10] = c + temp.z * ax.z;

	AnimaVertex4f m0(this->x);
	AnimaVertex4f m1(this->y);
	AnimaVertex4f m2(this->z);
	AnimaVertex4f m3(this->w);

	AnimaVertex4f r0 = m0 * rotMatrix.m[0] + m1 * rotMatrix.m[1] + m2 * rotMatrix.m[2];
	AnimaVertex4f r1 = m0 * rotMatrix.m[4] + m1 * rotMatrix.m[5] + m2 * rotMatrix.m[6];
	AnimaVertex4f r2 = m0 * rotMatrix.m[8] + m1 * rotMatrix.m[9] + m2 * rotMatrix.m[10];
	AnimaVertex4f r3 = m3;

	m[0] = r0.x;	m[1] = r0.y;	m[2] = r0.z;	m[3] = r0.w;
	m[4] = r1.x;	m[5] = r1.y;	m[6] = r1.z;	m[7] = r1.w;
	m[8] = r2.x;	m[9] = r2.y;	m[10] = r2.z;	m[11] = r2.w;
	m[12] = r3.x;	m[13] = r3.y;	m[14] = r3.z;	m[15] = r3.w;
}

void AnimaMatrix::RotateDeg(const AnimaVertex3f& v, AFloat deg)
{
	RotateRad(v, deg * (AFloat)M_PI / 180.0f);
}

void AnimaMatrix::RotateRad(const AFloat x, const AFloat y, const AFloat z, AFloat rad)
{
	AnimaVertex3f v(x, y, z);
	RotateRad(v, rad);
}

void AnimaMatrix::RotateDeg(const AFloat x, const AFloat y, const AFloat z, AFloat deg)
{
	AnimaVertex3f v(x, y, z);
	RotateRad(v, deg * (AFloat)M_PI / 180.0f);
}

AnimaMatrix AnimaMatrix::MakeRotationRad(const AnimaVertex3f& v, AFloat rad)
{
	AnimaMatrix m;
	m.RotateRad(v, rad);
	return m;
}

AnimaMatrix AnimaMatrix::MakeRotationDeg(const AnimaVertex3f& v, AFloat deg)
{
	AnimaMatrix m;
	m.RotateRad(v, deg * (AFloat)M_PI / 180.0f);
	return m;
}

AnimaMatrix AnimaMatrix::MakeRotationRad(const AFloat x, const AFloat y, const AFloat z, AFloat rad)
{
	AnimaVertex3f v(x, y, z);
	return MakeRotationRad(v, rad);
}

AnimaMatrix AnimaMatrix::MakeRotationDeg(const AFloat x, const AFloat y, const AFloat z, AFloat deg)
{
	AnimaVertex3f v(x, y, z);
	return MakeRotationRad(v, deg * (AFloat)M_PI / 180.0f);
}

void AnimaMatrix::RotateXRad(AFloat rad)
{
	RotateRad(AnimaVertex3f(1.0f, 0.0f, 0.0f), rad);
}

void AnimaMatrix::RotateXDeg(AFloat deg)
{
	RotateRad(AnimaVertex3f(1.0f, 0.0f, 0.0f), deg * (AFloat)M_PI / 180.0f);
}

AnimaMatrix AnimaMatrix::MakeRotationXRad(AFloat rad)
{
	AnimaMatrix m;
	m.RotateXRad(rad);
	return m;
}

AnimaMatrix AnimaMatrix::MakeRotationXDeg(AFloat deg)
{
	AnimaMatrix m;
	m.RotateXRad(deg * (AFloat)M_PI / 180.0f);
	return m;
}

void AnimaMatrix::RotateYRad(AFloat rad)
{
	RotateRad(AnimaVertex3f(0.0f, 1.0f, 0.0f), rad);
}

void AnimaMatrix::RotateYDeg(AFloat deg)
{
	RotateRad(AnimaVertex3f(0.0f, 1.0f, 0.0f), deg * (AFloat)M_PI / 180.0f);
}

AnimaMatrix AnimaMatrix::MakeRotationYRad(AFloat rad)
{
	AnimaMatrix m;
	m.RotateYRad(rad);
	return m;
}

AnimaMatrix AnimaMatrix::MakeRotationYDeg(AFloat deg)
{
	AnimaMatrix m;
	m.RotateYRad(deg * (AFloat)M_PI / 180.0f);
	return m;
}

void AnimaMatrix::RotateZRad(AFloat rad)
{
	RotateRad(AnimaVertex3f(0.0f, 0.0f, 1.0f), rad);
}

void AnimaMatrix::RotateZDeg(AFloat deg)
{
	RotateRad(AnimaVertex3f(0.0f, 0.0f, 1.0f), deg * (AFloat)M_PI / 180.0f);
}

AnimaMatrix AnimaMatrix::MakeRotationZRad(AFloat rad)
{
	AnimaMatrix m;
	m.RotateZRad(rad);
	return m;
}

AnimaMatrix AnimaMatrix::MakeRotationZDeg(AFloat deg)
{
	AnimaMatrix m;
	m.RotateZRad(deg * (AFloat)M_PI / 180.0f);
	return m;
}

void AnimaMatrix::FromHeadPitchRollRad(AFloat head, AFloat pitch, AFloat roll)
{
	SetIdentity();
	
	float cosH = cosf(head);
	float cosP = cosf(pitch);
	float cosR = cosf(roll);
	float sinH = sinf(head);
	float sinP = sinf(pitch);
	float sinR = sinf(roll);
	
	vecM[0][0] = cosR * cosH - sinR * sinP * sinH;
	vecM[0][1] = sinR * cosH + cosR * sinP * sinH;
	vecM[0][2] = -cosP * sinH;
	
	vecM[1][0] = -sinR * cosP;
	vecM[1][1] = cosR * cosP;
	vecM[1][2] = sinP;
	
	vecM[2][0] = cosR * sinH + sinR * sinP * cosH;
	vecM[2][1] = sinR * sinH - cosR * sinP * cosH;
	vecM[2][2] = cosP * cosH;
}

void AnimaMatrix::FromHeadPitchRollDeg(AFloat head, AFloat pitch, AFloat roll)
{
	FromHeadPitchRollDeg(AnimaMath::DegToRad(head), AnimaMath::DegToRad(pitch), AnimaMath::DegToRad(roll));
}

AnimaMatrix AnimaMatrix::MakeFromHeadPitchRollRad(AFloat head, AFloat pitch, AFloat roll)
{
	AnimaMatrix m;
	m.FromHeadPitchRollRad(head, pitch, roll);
	
	return m;
}

AnimaMatrix AnimaMatrix::MakeFromHeadPitchRollDeg(AFloat head, AFloat pitch, AFloat roll)
{
	AnimaMatrix m;
	m.FromHeadPitchRollRad(AnimaMath::DegToRad(head), AnimaMath::DegToRad(pitch), AnimaMath::DegToRad(roll));
	
	return m;
}

AnimaVertex3f AnimaMatrix::GetRotationAxes() const
{
	AnimaVertex3f rot;
	if (z[0] == -1.0f)
	{
		rot.z = 0.0f;
		float w = atan2(x[1], x[2]);

		rot.y = (AFloat)M_PI * 0.5f;
		rot.x = rot.z + w;
	}
	else if (z[0] == 1.0f)
	{
		rot.z = 0.0f;
		float w = atan2(x[1], x[2]);

		rot.y = (AFloat)M_PI * -0.5f;
		rot.x = -rot.z + w;
	}
	else
	{
		// there are two solutions, we're just going to find the first
		rot.y = -asin(z[0]);
		float invCosTheta = 1.0f / cos(rot.y);

		rot.x = atan2(z[1] * invCosTheta, z[2] * invCosTheta);
		rot.z = atan2(y[0] * invCosTheta, x[0] * invCosTheta);
	}

	return rot;
}

void AnimaMatrix::Transform4x4(AnimaVertex3f& dest, const AnimaVertex3f& src) const
{
	float invw;
	invw = 1.0f / (src.vec[0] * m[3] + src.vec[1] * m[7] + src.vec[2] * m[11] + m[15]);

	dest.vec[0] = (src.vec[0] * m[0] + src.vec[1] * m[4] + src.vec[2] * m[8] + m[12]) * invw;
	dest.vec[1] = (src.vec[0] * m[1] + src.vec[1] * m[5] + src.vec[2] * m[9] + m[13]) * invw;
	dest.vec[2] = (src.vec[0] * m[2] + src.vec[1] * m[6] + src.vec[2] * m[10] + m[14]) * invw;
}

AnimaVertex3f AnimaMatrix::Transform4x4(const AnimaVertex3f& p) const
{
	AnimaVertex3f res;
	Transform4x4(res, p);
	return res;
}

void AnimaMatrix::Transform4x3(AnimaVertex3f& dest, const AnimaVertex3f& src) const
{
	dest.vec[0] = src.vec[0] * m[0] + src.vec[1] * m[4] + src.vec[2] * m[8] + m[12];
	dest.vec[1] = src.vec[0] * m[1] + src.vec[1] * m[5] + src.vec[2] * m[9] + m[13];
	dest.vec[2] = src.vec[0] * m[2] + src.vec[1] * m[6] + src.vec[2] * m[10] + m[14];
}

AnimaVertex3f AnimaMatrix::Transform4x3(const AnimaVertex3f& p) const
{
	AnimaVertex3f res;
	Transform4x3(res, p);
	return res;
}

void AnimaMatrix::Transform3x3(AnimaVertex3f& dest, const AnimaVertex3f& src) const
{
	dest.vec[0] = src.vec[0] * m[0] + src.vec[1] * m[4] + src.vec[2] * m[8];
	dest.vec[1] = src.vec[0] * m[1] + src.vec[1] * m[5] + src.vec[2] * m[9];
	dest.vec[2] = src.vec[0] * m[2] + src.vec[1] * m[6] + src.vec[2] * m[10];
}

AnimaVertex3f AnimaMatrix::Transform3x3(AnimaVertex3f& p) const
{
	AnimaVertex3f res;
	Transform3x3(res, p);
	return res;
}

void AnimaMatrix::DumpMemory() const
{
	char szBuff[1024];
	sprintf(szBuff, "%f\t%f\t%f\t%f\n %f\t%f\t%f\t%f\n %f\t%f\t%f\t%f\n %f\t%f\t%f\t%f\n\n",
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

#if defined _MSC_VER
	OutputDebugStringA(szBuff);
#else
	puts(szBuff);
#endif
}

#undef _mm_shufd

END_ANIMA_ENGINE_NAMESPACE