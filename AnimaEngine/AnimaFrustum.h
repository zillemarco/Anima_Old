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

	AnimaVertex3f GetFrustumVertex(AInt index) const;
	void GetFrustumVertices(AnimaVertex3f vertices[8]) const;

	AnimaVertex3f GetBoundingBoxMin() const;
	AnimaVertex3f GetBoundingBoxMax() const;
	AnimaVertex3f GetBoundingBoxCenter() const;

public:
	void ComputeFrustum(const AnimaMatrix& projectionViewMatrix);

public:
	bool SphereInFrustum(const AnimaVertex3f& center, AFloat radius) const;

protected:
	AFloat			_frustum[6][4];
	AnimaVertex3f	_frustumVertices[8];

	AnimaVertex3f	_boundingBoxMin;
	AnimaVertex3f	_boundingBoxMax;
	AnimaVertex3f	_boundingBoxCenter;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFrustum__) */
