//
//  AnimaVectorGenerator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaVectorGenerator__
#define __Anima__AnimaVectorGenerator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaDataGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaVectorGenerator : public AnimaDataGenerator
{
public:
	AnimaVectorGenerator(AnimaEngine* engine);
	AnimaVectorGenerator(AnimaEngine* engine, const AnimaVertex2f& color);
	AnimaVectorGenerator(AnimaEngine* engine, AFloat x, AFloat y);
	AnimaVectorGenerator(AnimaEngine* engine, const AnimaVertex3f& color);
	AnimaVectorGenerator(AnimaEngine* engine, AFloat x, AFloat y, AFloat z);
	AnimaVectorGenerator(AnimaEngine* engine, const AnimaVertex4f& color);
	AnimaVectorGenerator(AnimaEngine* engine, AFloat x, AFloat y, AFloat z, AFloat w);
	AnimaVectorGenerator(const AnimaVectorGenerator& src);
	AnimaVectorGenerator(AnimaVectorGenerator&& src);
	~AnimaVectorGenerator();

	AnimaVectorGenerator& operator=(const AnimaVectorGenerator& src);
	AnimaVectorGenerator& operator=(AnimaVectorGenerator&& src);

public:
	void UpdateValue() override;

	void SetVector(const AnimaVertex2f& color) override;
	void SetVector(AFloat x, AFloat y) override;
	void SetVector(const AnimaVertex3f& color) override;
	void SetVector(AFloat x, AFloat y, AFloat z) override;
	void SetVector(const AnimaVertex4f& color) override;
	void SetVector(AFloat x, AFloat y, AFloat z, AFloat w) override;
	AnimaVertex2f GetVector2f() override;
	AnimaVertex3f GetVector3f() override;
	AnimaVertex4f GetVector4f() override;

protected:
	AnimaVertex4f _vector;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaVectorGenerator__) */
