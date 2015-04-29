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
#include "AnimaMappedArray.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaShaderProgram;
class AnimaScene;

class ANIMA_ENGINE_EXPORT AnimaSceneObject : public AnimaMappedValues
{
	friend class AnimaShaderProgram;
	friend class AnimaScene;
	friend class AnimaMappedValues;
	friend class AnimaTransformation;

protected:
	AnimaSceneObject(const AnimaString& name, AnimaDataGeneratorsManager* dataGeneratorsManager, AnimaAllocator* allocator);
	
public:
	AnimaSceneObject(AnimaSceneObject&& src);
	AnimaSceneObject(const AnimaSceneObject& src);
	virtual ~AnimaSceneObject();

	AnimaSceneObject& operator=(const AnimaSceneObject& src);
	AnimaSceneObject& operator=(AnimaSceneObject&& src);
	
public:
	virtual void SetPosition(const AnimaVertex3f& position);

public:
	virtual AnimaVertex3f GetPosition();
	virtual void SetPosition(AFloat x, AFloat y, AFloat z);

	virtual void SetParentObject(AnimaSceneObject* parentObject);
	virtual AnimaSceneObject* GetParentObject() const;

	virtual AInt AddChild(AnimaSceneObject* child);
	virtual AInt GetChildrenNumber();
	virtual AnimaSceneObject* GetChild(const AnimaString& name);
	virtual AnimaSceneObject* GetChild(const char* name);
	virtual AnimaSceneObject* GetChild(AInt index);
	virtual bool RemoveChild(AnimaSceneObject* object);
	virtual bool RemoveChild(const AnimaString& name);
	virtual bool RemoveChild(const char* name);
	virtual bool RemoveChild(AInt index);
	virtual AInt HasChild(AnimaSceneObject* object);
	virtual AInt HasChild(const AnimaString& name);
	virtual AInt HasChild(const char* name);

	AnimaTransformation* GetTransformation();
	AnimaTransformation GetTransformationCopy();

	virtual const char* GetShaderPrefix() = 0;

protected:
	virtual void UpdateChildrenTransformation();
	
protected:
	AnimaTransformation _transformation;

	AnimaSceneObject* _parentObject;
	AnimaMappedArray<AnimaSceneObject*> _children;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaSceneObject__) */
