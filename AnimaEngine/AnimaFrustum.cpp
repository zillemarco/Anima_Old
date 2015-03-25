//
//  AnimaFrustum.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFrustum.h"
#include "AnimaMath.h"

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef min
#	define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#	define max(a,b) (a > b ? a : b)
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFrustum::AnimaFrustum()
{
	for (AInt i = 0; i < 6; i++)
	{
		_frustum[i][0] = 0.0f;
		_frustum[i][1] = 0.0f;
		_frustum[i][2] = 0.0f;
		_frustum[i][3] = 0.0f;
	}
}

AnimaFrustum::AnimaFrustum(const AnimaFrustum& src)
{
	for (AInt i = 0; i < 8; i++)
		_frustumVertices[i] = src._frustumVertices[i];

	for (AInt i = 0; i < 6; i++)
	{
		_frustum[i][0] = src._frustum[i][0];
		_frustum[i][1] = src._frustum[i][1];
		_frustum[i][2] = src._frustum[i][2];
		_frustum[i][3] = src._frustum[i][3];
	}

	_boundingBoxMin = src._boundingBoxMin;
	_boundingBoxMax = src._boundingBoxMax;
	_boundingBoxCenter = src._boundingBoxCenter;
}

AnimaFrustum::AnimaFrustum(AnimaFrustum&& src)
{
	for (AInt i = 0; i < 8; i++)
		_frustumVertices[i] = src._frustumVertices[i];

	for (AInt i = 0; i < 6; i++)
	{
		_frustum[i][0] = src._frustum[i][0];
		_frustum[i][1] = src._frustum[i][1];
		_frustum[i][2] = src._frustum[i][2];
		_frustum[i][3] = src._frustum[i][3];
	}

	_boundingBoxMin = src._boundingBoxMin;
	_boundingBoxMax = src._boundingBoxMax;
	_boundingBoxCenter = src._boundingBoxCenter;
}

AnimaFrustum::~AnimaFrustum()
{
}

AnimaFrustum& AnimaFrustum::operator=(const AnimaFrustum& src)
{
	if (this != &src)
	{
		for (AInt i = 0; i < 8; i++)
			_frustumVertices[i] = src._frustumVertices[i];

		for (AInt i = 0; i < 6; i++)
		{
			_frustum[i][0] = src._frustum[i][0];
			_frustum[i][1] = src._frustum[i][1];
			_frustum[i][2] = src._frustum[i][2];
			_frustum[i][3] = src._frustum[i][3];
		}

		_boundingBoxMin = src._boundingBoxMin;
		_boundingBoxMax = src._boundingBoxMax;
		_boundingBoxCenter = src._boundingBoxCenter;
	}

	return *this;
}

AnimaFrustum& AnimaFrustum::operator=(AnimaFrustum&& src)
{
	if (this != &src)
	{
		for (AInt i = 0; i < 8; i++)
			_frustumVertices[i] = src._frustumVertices[i];

		for (AInt i = 0; i < 6; i++)
		{
			_frustum[i][0] = src._frustum[i][0];
			_frustum[i][1] = src._frustum[i][1];
			_frustum[i][2] = src._frustum[i][2];
			_frustum[i][3] = src._frustum[i][3];
		}

		_boundingBoxMin = src._boundingBoxMin;
		_boundingBoxMax = src._boundingBoxMax;
		_boundingBoxCenter = src._boundingBoxCenter;
	}

	return *this;
}

AnimaVertex3f AnimaFrustum::GetFrustumVertex(AInt index) const
{
	ANIMA_ASSERT(index >= 0 && index < 8);
	return _frustumVertices[index];
}

void AnimaFrustum::GetFrustumVertices(AnimaVertex3f vertices[8]) const
{
	for (AInt i = 0; i < 8; i++)
		vertices[i] = _frustumVertices[i];
}

AnimaVertex3f AnimaFrustum::GetBoundingBoxMin() const
{
	return _boundingBoxMin;
}

AnimaVertex3f AnimaFrustum::GetBoundingBoxMax() const
{
	return _boundingBoxMax;
}

AnimaVertex3f AnimaFrustum::GetBoundingBoxCenter() const
{
	return _boundingBoxCenter;
}

void AnimaFrustum::ComputeFrustum(const AnimaMatrix& projectionViewMatrix)
{	
	AnimaMatrix inverseProjectionViewMatrix = projectionViewMatrix.Inversed();

	AnimaVertex3f points[8];

	_frustumVertices[0] = inverseProjectionViewMatrix * AnimaVertex3f(-1.0f,  1.0f, -1.0f);	// near top-left
	_frustumVertices[1] = inverseProjectionViewMatrix * AnimaVertex3f( 1.0f,  1.0f, -1.0f);	// near top-right
	_frustumVertices[2] = inverseProjectionViewMatrix * AnimaVertex3f(-1.0f, -1.0f, -1.0f);	// near bottom-left
	_frustumVertices[3] = inverseProjectionViewMatrix * AnimaVertex3f( 1.0f, -1.0f, -1.0f);	// near bottom-right
	_frustumVertices[4] = inverseProjectionViewMatrix * AnimaVertex3f(-1.0f,  1.0f,  1.0f);	// far top-left
	_frustumVertices[5] = inverseProjectionViewMatrix * AnimaVertex3f( 1.0f,  1.0f,  1.0f);	// far top-right
	_frustumVertices[6] = inverseProjectionViewMatrix * AnimaVertex3f(-1.0f, -1.0f,  1.0f);	// far bottom-left
	_frustumVertices[7] = inverseProjectionViewMatrix * AnimaVertex3f( 1.0f, -1.0f,  1.0f);	// far bottom-right

	_boundingBoxMin = _frustumVertices[0];
	_boundingBoxMax = _frustumVertices[0];

	for (AInt i = 1; i < 8; i++)
	{
		_boundingBoxMin.x = min(_boundingBoxMin.x, _frustumVertices[i].x);
		_boundingBoxMin.y = min(_boundingBoxMin.y, _frustumVertices[i].y);
		_boundingBoxMin.z = min(_boundingBoxMin.z, _frustumVertices[i].z);

		_boundingBoxMax.x = max(_boundingBoxMax.x, _frustumVertices[i].x);
		_boundingBoxMax.y = max(_boundingBoxMax.y, _frustumVertices[i].y);
		_boundingBoxMax.z = max(_boundingBoxMax.z, _frustumVertices[i].z);
	}

	_boundingBoxCenter = AnimaVertex3f((_boundingBoxMin.x + _boundingBoxMax.x) / 2.0f, (_boundingBoxMin.y + _boundingBoxMax.y) / 2.0f, (_boundingBoxMin.z + _boundingBoxMax.z) / 2.0f);

	/* Extract the numbers for the RIGHT plane */
	_frustum[0][0] = projectionViewMatrix.m[3] - projectionViewMatrix.m[0];
	_frustum[0][1] = projectionViewMatrix.m[7] - projectionViewMatrix.m[4];
	_frustum[0][2] = projectionViewMatrix.m[11] - projectionViewMatrix.m[8];
	_frustum[0][3] = projectionViewMatrix.m[15] - projectionViewMatrix.m[12];

	/* Normalize the result */
	AFloat t = sqrt(_frustum[0][0] * _frustum[0][0] + _frustum[0][1] * _frustum[0][1] + _frustum[0][2] * _frustum[0][2]);
	_frustum[0][0] /= t;
	_frustum[0][1] /= t;
	_frustum[0][2] /= t;
	_frustum[0][3] /= t;

	/* Extract the numbers for the LEFT plane */
	_frustum[1][0] = projectionViewMatrix.m[3] + projectionViewMatrix.m[0];
	_frustum[1][1] = projectionViewMatrix.m[7] + projectionViewMatrix.m[4];
	_frustum[1][2] = projectionViewMatrix.m[11] + projectionViewMatrix.m[8];
	_frustum[1][3] = projectionViewMatrix.m[15] + projectionViewMatrix.m[12];

	/* Normalize the result */
	t = sqrt(_frustum[1][0] * _frustum[1][0] + _frustum[1][1] * _frustum[1][1] + _frustum[1][2] * _frustum[1][2]);
	_frustum[1][0] /= t;
	_frustum[1][1] /= t;
	_frustum[1][2] /= t;
	_frustum[1][3] /= t;

	/* Extract the BOTTOM plane */
	_frustum[2][0] = projectionViewMatrix.m[3] + projectionViewMatrix.m[1];
	_frustum[2][1] = projectionViewMatrix.m[7] + projectionViewMatrix.m[5];
	_frustum[2][2] = projectionViewMatrix.m[11] + projectionViewMatrix.m[9];
	_frustum[2][3] = projectionViewMatrix.m[15] + projectionViewMatrix.m[13];

	/* Normalize the result */
	t = sqrt(_frustum[2][0] * _frustum[2][0] + _frustum[2][1] * _frustum[2][1] + _frustum[2][2] * _frustum[2][2]);
	_frustum[2][0] /= t;
	_frustum[2][1] /= t;
	_frustum[2][2] /= t;
	_frustum[2][3] /= t;

	/* Extract the TOP plane */
	_frustum[3][0] = projectionViewMatrix.m[3] - projectionViewMatrix.m[1];
	_frustum[3][1] = projectionViewMatrix.m[7] - projectionViewMatrix.m[5];
	_frustum[3][2] = projectionViewMatrix.m[11] - projectionViewMatrix.m[9];
	_frustum[3][3] = projectionViewMatrix.m[15] - projectionViewMatrix.m[13];

	/* Normalize the result */
	t = sqrt(_frustum[3][0] * _frustum[3][0] + _frustum[3][1] * _frustum[3][1] + _frustum[3][2] * _frustum[3][2]);
	_frustum[3][0] /= t;
	_frustum[3][1] /= t;
	_frustum[3][2] /= t;
	_frustum[3][3] /= t;

	/* Extract the FAR plane */
	_frustum[4][0] = projectionViewMatrix.m[3] - projectionViewMatrix.m[2];
	_frustum[4][1] = projectionViewMatrix.m[7] - projectionViewMatrix.m[6];
	_frustum[4][2] = projectionViewMatrix.m[11] - projectionViewMatrix.m[10];
	_frustum[4][3] = projectionViewMatrix.m[15] - projectionViewMatrix.m[14];

	/* Normalize the result */
	t = sqrt(_frustum[4][0] * _frustum[4][0] + _frustum[4][1] * _frustum[4][1] + _frustum[4][2] * _frustum[4][2]);
	_frustum[4][0] /= t;
	_frustum[4][1] /= t;
	_frustum[4][2] /= t;
	_frustum[4][3] /= t;

	/* Extract the NEAR plane */
	_frustum[5][0] = projectionViewMatrix.m[3] + projectionViewMatrix.m[2];
	_frustum[5][1] = projectionViewMatrix.m[7] + projectionViewMatrix.m[6];
	_frustum[5][2] = projectionViewMatrix.m[11] + projectionViewMatrix.m[10];
	_frustum[5][3] = projectionViewMatrix.m[15] + projectionViewMatrix.m[14];

	/* Normalize the result */
	t = sqrt(_frustum[5][0] * _frustum[5][0] + _frustum[5][1] * _frustum[5][1] + _frustum[5][2] * _frustum[5][2]);
	_frustum[5][0] /= t;
	_frustum[5][1] /= t;
	_frustum[5][2] /= t;
	_frustum[5][3] /= t;

}

bool AnimaFrustum::SphereInFrustum(const AnimaVertex3f& center, AFloat radius) const
{
	for (AInt p = 0; p < 6; p++)
		if (_frustum[p][0] * center.x + _frustum[p][1] * center.y + _frustum[p][2] * center.z + _frustum[p][3] <= -radius)
			return false;
	return true;
}

END_ANIMA_ENGINE_NAMESPACE
