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

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFrustum::AnimaFrustum()
{
}

AnimaFrustum::AnimaFrustum(const AnimaFrustum& src)
{
}

AnimaFrustum::AnimaFrustum(AnimaFrustum&& src)
{
}

AnimaFrustum::~AnimaFrustum()
{
}

AnimaFrustum& AnimaFrustum::operator=(const AnimaFrustum& src)
{
	if (this != &src)
	{
	}

	return *this;
}

AnimaFrustum& AnimaFrustum::operator=(AnimaFrustum&& src)
{
	if (this != &src)
	{
	}

	return *this;
}

void AnimaFrustum::ComputeFrustum(const AnimaMatrix& projectionViewMatrix)
{
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

bool AnimaFrustum::SphereInFrustum(const AnimaVertex3f& center, AFloat radius)
{
	for (AInt p = 0; p < 6; p++)
		if (_frustum[p][0] * center.x + _frustum[p][1] * center.y + _frustum[p][2] * center.z + _frustum[p][3] <= -radius)
			return false;
	return true;
}

//void AnimaFrustum::SetCameraProjection(AFloat fov, AFloat ratio, AFloat zNear, AFloat zFar)
//{
//	this->_ratio	= ratio;
//	this->_fov		= fov;
//	this->_zNear	= zNear;
//	this->_zFar		= zFar;
//
//	_tangent = tanf(this->_fov);
//	_sphereFactor.y = 1.0f / cosf(this->_fov);
//
//	AFloat anglex = atanf(_tangent * ratio);
//	_sphereFactor.x = 1.0f / cosf(anglex);
//
//	_nearPlaneHeight	= zNear * _tangent;
//	_nearPlaneWidth = _nearPlaneHeight * ratio;
//
//	_farPlaneHeight = zFar * _tangent;
//	_farPlaneWidth = _farPlaneWidth * ratio;
//}
//
//void AnimaFrustum::SetCameraView(const AnimaVertex3f& viewPoint, const AnimaVertex3f& forward, const AnimaVertex3f& up)
//{
//	AnimaVertex3f dir, nc, fc;
//
//	_position = viewPoint;
//
//	_zAxis = forward;
//	_zAxis.Normalize();
//
//	_xAxis = up ^ _zAxis;
//	_xAxis.Normalize();
//
//	_yAxis = _zAxis ^ _xAxis;
//
//	nc = viewPoint - _zAxis * _zNear;
//	fc = viewPoint - _zAxis * _zFar;
//
//	_nearPlanePoints[TOPLEFT]		= nc + _yAxis * _nearPlaneHeight - _xAxis * _nearPlaneWidth;
//	_nearPlanePoints[TOPRIGHT]		= nc + _yAxis * _nearPlaneHeight + _xAxis * _nearPlaneWidth;
//	_nearPlanePoints[BOTTOMLEFT]	= nc - _yAxis * _nearPlaneHeight - _xAxis * _nearPlaneWidth;
//	_nearPlanePoints[BOTTOMRIGHT]	= nc - _yAxis * _nearPlaneHeight + _xAxis * _nearPlaneWidth;
//
//	_farPlanePoints[TOPLEFT]		= fc + _yAxis * _farPlaneHeight - _xAxis * _farPlaneWidth;
//	_farPlanePoints[BOTTOMRIGHT]	= fc - _yAxis * _farPlaneHeight + _xAxis * _farPlaneWidth;
//	_farPlanePoints[TOPRIGHT]		= fc + _yAxis * _farPlaneHeight + _xAxis * _farPlaneWidth;
//	_farPlanePoints[BOTTOMLEFT]		= fc - _yAxis * _farPlaneHeight - _xAxis * _farPlaneWidth;
//
//	_planes[TOP].SetPoints(_nearPlanePoints[TOPRIGHT], _nearPlanePoints[TOPLEFT], _farPlanePoints[TOPLEFT]);
//	_planes[BOTTOM].SetPoints(_nearPlanePoints[BOTTOMLEFT], _nearPlanePoints[BOTTOMRIGHT], _farPlanePoints[BOTTOMRIGHT]);
//	_planes[LEFT].SetPoints(_nearPlanePoints[TOPLEFT], _nearPlanePoints[BOTTOMLEFT], _farPlanePoints[BOTTOMLEFT]);
//	_planes[RIGHT].SetPoints(_nearPlanePoints[BOTTOMRIGHT], _nearPlanePoints[TOPRIGHT], _farPlanePoints[BOTTOMRIGHT]);
//
//	_planes[NEARP].SetNormalAndPoint(-_zAxis, nc);
//	_planes[FARP].SetNormalAndPoint(_zAxis, fc);
//
//	AnimaVertex3f aux, normal;
//
//	aux = (nc + _yAxis * _nearPlaneHeight) - viewPoint;
//	normal = aux * _xAxis;
//	_planes[TOP].SetNormalAndPoint(normal, nc + _yAxis * _nearPlaneHeight);
//
//	aux = (nc - _yAxis * _nearPlaneHeight) - viewPoint;
//	normal = _xAxis * aux;
//	_planes[BOTTOM].SetNormalAndPoint(normal, nc - _yAxis * _nearPlaneHeight);
//
//	aux = (nc - _xAxis * _nearPlaneWidth) - viewPoint;
//	normal = aux * _yAxis;
//	_planes[LEFT].SetNormalAndPoint(normal, nc - _xAxis * _nearPlaneWidth);
//
//	aux = (nc + _xAxis * _nearPlaneWidth) - viewPoint;
//	normal = _yAxis * aux;
//	_planes[RIGHT].SetNormalAndPoint(normal, nc + _xAxis * _nearPlaneWidth);
//}
//
//bool AnimaFrustum::PointInFrustum(const AnimaVertex3f& p)
//{
//	AFloat pcz, pcx, pcy, aux;
//	AnimaVertex3f v = p - _position;
//
//	pcz = v.InnerProduct(-_zAxis);
//	if (pcz > _zFar || pcz < _zNear)
//		return false;
//
//	pcy = v.InnerProduct(_yAxis);
//	aux = pcz * _tangent;
//	if (pcy > aux || pcy < -aux)
//		return false;
//
//	pcx = v.InnerProduct(_xAxis);
//	aux = aux * _ratio;
//	if (pcx > aux || pcx < -aux)
//		return false;
//	
//	return true;
//}
//
//bool AnimaFrustum::SphereInFrustum(const AnimaVertex3f& p, AFloat radius)
//{
//	AFloat d1, d2;
//	AFloat az, ax, ay, zz1, zz2;
//
//	AnimaVertex3f v = p - _position;
//
//	az = v.InnerProduct(-_zAxis);
//	if (az > _zFar + radius || az < _zNear - radius)
//		return false;
//
//	ax = v.InnerProduct(_xAxis);
//	zz1 = az * _tangent * _ratio;
//	d1 = _sphereFactor.x * radius;
//	if (ax > zz1 + d1 || ax < -zz1 - d1)
//		return false;
//
//	ay = v.InnerProduct(_yAxis);
//	zz2 = az * _tangent;
//	d2 = _sphereFactor.y * radius;
//	if (ay > zz2 + d2 || ay < -zz2 - d2)
//		return false;
//
//	// Questo testa le intersezioni
//	if (az > _zFar - radius || az < _zNear + radius)
//		return true;
//	if (ay > zz2 - d2 || ay < -zz2 + d2)
//		return true;
//	if (ax > zz1 - d1 || ax < -zz1 + d1)
//		return true;
//
//	return true;
//}
//
//AnimaFrustum::AnimaFrustumPlane::AnimaFrustumPlane()
//{
//}
//
//AnimaFrustum::AnimaFrustumPlane::AnimaFrustumPlane(const AnimaVertex3f& v1, const AnimaVertex3f& v2, const AnimaVertex3f& v3)
//{
//	SetPoints(v1, v2, v3);
//}
//
//AnimaFrustum::AnimaFrustumPlane::~AnimaFrustumPlane()
//{
//}
//
//void AnimaFrustum::AnimaFrustumPlane::SetPoints(const AnimaVertex3f& v1, const AnimaVertex3f& v2, const AnimaVertex3f& v3)
//{
//	AnimaVertex3f aux1, aux2;
//
//	aux1 = v1 - v2;
//	aux2 = v3 - v2;
//
//	_normal = aux1 ^ aux2;
//	_normal.Normalize();
//
//	_point = v1;
//
//	_d = -(_normal.InnerProduct(_point));
//}
//
//void AnimaFrustum::AnimaFrustumPlane::SetNormalAndPoint(const AnimaVertex3f& normal, const AnimaVertex3f& point)
//{
//	_normal = normal;
//	_normal.Normalize();
//	_d = -(_normal.InnerProduct(point));
//}
//
//void AnimaFrustum::AnimaFrustumPlane::SetCoefficents(AFloat a, AFloat b, AFloat c, AFloat d)
//{
//	_normal.Fill(a, b, c);
//	
//	AFloat l = _normal.Length();
//	_normal.Fill(a / l, b / l, c / l);
//
//	this->_d = d / l;
//}
//
//AFloat AnimaFrustum::AnimaFrustumPlane::Distance(const AnimaVertex3f& p)
//{
//	return (_d + _normal.InnerProduct(p));
//}

END_ANIMA_ENGINE_NAMESPACE
