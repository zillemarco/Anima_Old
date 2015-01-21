//
//  AnimaColorGenerator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaColorGenerator__
#define __Anima__AnimaColorGenerator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaDataGenerator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaColorGenerator : public AnimaDataGenerator
{
public:
	AnimaColorGenerator(AnimaEngine* engine);
	AnimaColorGenerator(AnimaEngine* engine, const AnimaColor3f& color);
	AnimaColorGenerator(AnimaEngine* engine, AFloat r, AFloat g, AFloat b);
	AnimaColorGenerator(const AnimaColorGenerator& src);
	AnimaColorGenerator(AnimaColorGenerator&& src);
	~AnimaColorGenerator();

	AnimaColorGenerator& operator=(const AnimaColorGenerator& src);
	AnimaColorGenerator& operator=(AnimaColorGenerator&& src);

public:
	void UpdateValue() override;

	void SetColor3f(const AnimaColor3f& color) override;
	void SetColor3f(AFloat r, AFloat g, AFloat b) override;
	AnimaColor3f GetColor3f() override;

protected:
	AnimaColor3f _color;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaColorGenerator__) */
