//
//  AnimaMath.h
//  Anima
//
//  Created by Marco Zille on 17/12/14.
//
//

#ifndef __Anima__AnimaMath__
#define __Anima__AnimaMath__

#include "AnimaEngineCore.h"
#include "AnimaEngine.h"
#include "AnimaTypes.h"
#include "AnimaVertex.h"
#include "AnimaMatrix.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMath
{
public:
	static AnimaVertex3f MatrixMulVector(const AnimaMatrix& m, const AnimaVertex3f& v);
	static AnimaVertex4f MatrixMulVector(const AnimaMatrix& m, const AnimaVertex4f& v);
	static AnimaVertex3f MatrixDivVector(const AnimaMatrix& m, const AnimaVertex3f& v);
	static AnimaVertex4f MatrixDivVector(const AnimaMatrix& m, const AnimaVertex4f& v);

	static AFloat Dot(const AnimaVertex2f& v1, const AnimaVertex2f& v2);
	static AFloat Dot(const AnimaVertex3f& v1, const AnimaVertex3f& v2);
	static AFloat Dot(const AnimaVertex4f& v1, const AnimaVertex4f& v2);
	static AChar Dot(const AnimaColor3b& v1, const AnimaColor3b& v2);
	static AChar Dot(const AnimaColor4b& v1, const AnimaColor4b& v2);

	static AnimaVertex3f Cross(const AnimaVertex3f& v1, const AnimaVertex3f& v2);
	static AnimaVertex4f Cross(const AnimaVertex4f& v1, const AnimaVertex4f& v2);
};

END_ANIMA_ENGINE_NAMESPACE

Anima::AnimaVertex3f ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaMatrix& m, const Anima::AnimaVertex3f& v);
Anima::AnimaVertex4f ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaMatrix& m, const Anima::AnimaVertex4f& v);
Anima::AnimaVertex3f ANIMA_ENGINE_EXPORT operator/(const Anima::AnimaMatrix& m, const Anima::AnimaVertex3f& v);
Anima::AnimaVertex4f ANIMA_ENGINE_EXPORT operator/(const Anima::AnimaMatrix& m, const Anima::AnimaVertex4f& v);

void ANIMA_ENGINE_EXPORT operator*=(Anima::AnimaVertex3f& v, const Anima::AnimaMatrix& m);
void ANIMA_ENGINE_EXPORT operator*=(Anima::AnimaVertex4f& v, const Anima::AnimaMatrix& m);
void ANIMA_ENGINE_EXPORT operator/=(Anima::AnimaVertex3f& v, const Anima::AnimaMatrix& m);
void ANIMA_ENGINE_EXPORT operator/=(Anima::AnimaVertex4f& v, const Anima::AnimaMatrix& m);

Anima::AFloat ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaVertex2f& v1, const Anima::AnimaVertex2f& v2);
Anima::AFloat ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaVertex3f& v1, const Anima::AnimaVertex3f& v2);
Anima::AFloat ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaVertex4f& v1, const Anima::AnimaVertex4f& v2);
Anima::AChar ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaColor3b& v1, const Anima::AnimaColor3b& v2);
Anima::AChar ANIMA_ENGINE_EXPORT operator*(const Anima::AnimaColor4b& v1, const Anima::AnimaColor4b& v2);

Anima::AnimaVertex3f ANIMA_ENGINE_EXPORT operator^(const Anima::AnimaVertex3f& v1, const Anima::AnimaVertex3f& v2);
Anima::AnimaVertex4f ANIMA_ENGINE_EXPORT operator^(const Anima::AnimaVertex4f& v1, const Anima::AnimaVertex4f& v2);

#endif /* defined(__Anima__AnimaMath__) */
