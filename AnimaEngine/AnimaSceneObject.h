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

class ANIMA_ENGINE_EXPORT AnimaSceneObject : public AnimaMappedValues
{
protected:
	AnimaSceneObject(AnimaAllocator* allocator, AnimaDataGeneratorsManager* dataGeneratorsManager, const AnimaString& name);
	
public:
	AnimaSceneObject(const AnimaSceneObject& src);
	AnimaSceneObject(AnimaSceneObject&& src);
	virtual ~AnimaSceneObject();

	AnimaSceneObject& operator=(const AnimaSceneObject& src);
	AnimaSceneObject& operator=(AnimaSceneObject&& src);

public:

public:

protected:
	AnimaTransformation _transformation;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaSceneObject__) */
