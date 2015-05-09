//
//  AnimaNamedObject.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaNamedObject__
#define __Anima__AnimaNamedObject__

#include "AnimaEngine.h"
#include "AnimaAllocator.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaNamedObject
{
public:
	AnimaNamedObject(const AnimaString& name, AnimaAllocator* allocator);
	AnimaNamedObject(const AnimaNamedObject& src);
	AnimaNamedObject(AnimaNamedObject&& src);
	virtual ~AnimaNamedObject();

	AnimaNamedObject& operator=(const AnimaNamedObject& src);
	AnimaNamedObject& operator=(AnimaNamedObject&& src);

public:
	void SetName(const AnimaString& name);
	void SetName(const char* name);

	AnimaString GetAnimaName() const;
	const char* GetName() const;

protected:
	AnimaAllocator* _allocator;
	AnimaString _name;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaNamedObject__) */
