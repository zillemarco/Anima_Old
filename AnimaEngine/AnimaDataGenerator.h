//
//  AnimaDataGenerator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaDataGenerator__
#define __Anima__AnimaDataGenerator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaDataGenerator
{
public:
	AnimaDataGenerator(AnimaEngine* engine);
	AnimaDataGenerator(const AnimaDataGenerator& src);
	AnimaDataGenerator(AnimaDataGenerator&& src);
	virtual ~AnimaDataGenerator();

	AnimaDataGenerator& operator=(const AnimaDataGenerator& src);
	AnimaDataGenerator& operator=(AnimaDataGenerator&& src);

public:
	virtual void UpdateValue() = 0;

	virtual void SetColor3f(const AnimaColor3f& color);
	virtual void SetColor3f(AFloat r, AFloat g, AFloat b);
	virtual AnimaColor3f GetColor3f();

	virtual void StopValueUpdate();
	virtual void StartValueUpdate();

	bool CanUpdateValue();

protected:
	AnimaEngine* _engine;

	bool _canUpdateValue;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaDataGenerator__) */
