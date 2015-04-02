//
//  AnimaFace.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaFace__
#define __Anima__AnimaFace__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaFace
{
public:
	AnimaFace();
	AnimaFace(AUint indexes[3]);
	AnimaFace(AUint a, AUint b, AUint c);
	AnimaFace(const AnimaFace& src);
	AnimaFace(AnimaFace&& src);
	~AnimaFace();
	
	AnimaFace& operator=(const AnimaFace& src);
	AnimaFace& operator=(AnimaFace&& src);
	
public:
	void SetIndexes(AUint indexes[3]);
	void SetIndexes(AUint a, AUint b, AUint c);
	AUint GetIndex(AInt index) const;
	AUint* GetIndexes();
	const AUint* GetConstIndexes();
	bool GetIndexes(AUint* outIndexes, AInt& inOutSize);
	bool GetConstIndexes(AUint* outIndexes, AInt& inOutSize);
	AInt GetIndexesCount();
	bool HasIndex(AUint index) const;

	void SetNormal(const AnimaVertex3f& n);
	void SetNormal(const AFloat& x, const AFloat& y, const AFloat& z);
	AnimaVertex3f GetNormal() const;
				
protected:
	AUint _indexes[3];
	AnimaVertex3f _normal;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaFace__) */
