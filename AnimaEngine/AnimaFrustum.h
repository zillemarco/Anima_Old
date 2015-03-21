//
//  AnimaFrustum.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFrustum__
#define __Anima__AnimaFrustum__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFrustum
{
protected:
	//class ANIMA_ENGINE_EXPORT AnimaFrustumPlane
	//{
	//public:
	//	AnimaFrustumPlane();
	//	AnimaFrustumPlane(const AnimaVertex3f& v1, const AnimaVertex3f& v2, const AnimaVertex3f& v3);
	//	~AnimaFrustumPlane();

	//public:
	//	void SetPoints(const AnimaVertex3f& v1, const AnimaVertex3f& v2, const AnimaVertex3f& v3);
	//	void SetNormalAndPoint(const AnimaVertex3f& normal, const AnimaVertex3f& point);
	//	void SetCoefficents(AFloat a, AFloat b, AFloat c, AFloat d);

	//	AFloat Distance(const AnimaVertex3f& p);

	//private:
	//	AnimaVertex3f	_normal;
	//	AnimaVertex3f	_point;
	//	AFloat			_d;
	//};

	enum { TOP = 0, BOTTOM, LEFT, RIGHT, NEARP, FARP };
	enum { TOPLEFT = 0, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT };

public:
	AnimaFrustum();
	AnimaFrustum(const AnimaFrustum& src);
	AnimaFrustum(AnimaFrustum&& src);
	virtual ~AnimaFrustum();

	AnimaFrustum& operator=(const AnimaFrustum& src);
	AnimaFrustum& operator=(AnimaFrustum&& src);

public:
	//void SetCameraProjection(AFloat fov, AFloat ratio, AFloat zNear, AFloat zFar);
	//void SetCameraView(const AnimaVertex3f& viewPoint, const AnimaVertex3f& forward, const AnimaVertex3f& up);
	void ComputeFrustum(const AnimaMatrix& projectionViewMatrix);

	bool SphereInFrustum(const AnimaVertex3f& center, AFloat radius);

	//bool PointInFrustum(const AnimaVertex3f& p);
	//bool SphereInFrustum(const AnimaVertex3f& p, AFloat radius);

protected:
	AFloat _frustum[6][4];

	//AnimaFrustumPlane	_planes[6];
	//AnimaVertex3f		_nearPlanePoints[4];
	//AnimaVertex3f		_farPlanePoints[4];
	//AnimaVertex3f		_xAxis;
	//AnimaVertex3f		_yAxis;
	//AnimaVertex3f		_zAxis;
	//AnimaVertex3f		_position;
	//AFloat				_zNear;
	//AFloat				_zFar;
	//AFloat				_ratio;
	//AFloat				_fov;
	//AnimaVertex2f		_sphereFactor;
	//AFloat				_tangent;
	//AFloat				_nearPlaneWidth;
	//AFloat				_nearPlaneHeight;
	//AFloat				_farPlaneWidth;
	//AFloat				_farPlaneHeight;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFrustum__) */
