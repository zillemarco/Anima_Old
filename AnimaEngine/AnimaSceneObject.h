//
//  AnimaSceneObject.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaSceneObject__
#define __Anima__AnimaSceneObject__

#include "AnimaEngine.h"
#include "AnimaVertex.h"
#include "AnimaTypes.h"
#include "AnimaMatrix.h"
#include "AnimaMappedValues.h"
#include "AnimaDataGeneratorsManager.h"
#include "AnimaTransformation.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;

class ANIMA_ENGINE_EXPORT AnimaSceneObject : protected AnimaMappedValues
{
	friend class AnimaShaderProgram;

protected:
	AnimaSceneObject(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	
public:
	AnimaSceneObject(const AnimaSceneObject& src);
	AnimaSceneObject(AnimaSceneObject&& src);
	virtual ~AnimaSceneObject();

	AnimaSceneObject& operator=(const AnimaSceneObject& src);
	AnimaSceneObject& operator=(AnimaSceneObject&& src);

public:
	virtual void SetPosition(const AnimaVertex3f& position);
	virtual void SetPosition(AFloat x, AFloat y, AFloat z);
	virtual AnimaVertex3f GetPosition();

	virtual const char* GetShaderPrefix() = 0;

protected:
	AnimaTransformation _transformation;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaSceneObject__) */
