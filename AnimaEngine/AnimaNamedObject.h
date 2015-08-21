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
#include "AnimaString.h"
#include "AnimaAllocator.h"
#include "AnimaArray.h"
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

#define DECLARE_ANIMA_CLASS(class_name)		public:																		\
												static AnimaString _sGetClassName() { return AnimaString(#class_name); }

#define IMPLEMENT_ANIMA_CLASS(class_name)	_AddDerivedClassName(class_name::_sGetClassName());	\
											_SetClassName(class_name::_sGetClassName());

#define ANIMA_CLASS_NAME(class_name)		class_name::_sGetClassName()

class ANIMA_ENGINE_EXPORT AnimaNamedObject
{
	DECLARE_ANIMA_CLASS(AnimaNamedObject);

public:
	AnimaNamedObject(const AnimaString& name, AnimaAllocator* allocator);
	AnimaNamedObject(const AnimaNamedObject& src);
	AnimaNamedObject(AnimaNamedObject&& src);
	virtual ~AnimaNamedObject();

	AnimaNamedObject& operator=(const AnimaNamedObject& src);
	AnimaNamedObject& operator=(AnimaNamedObject&& src);

public:
	virtual void SetName(const AnimaString& name);
	AnimaString GetName() const;
	
protected:
	void _AddDerivedClassName(AnimaString derivedClassName);
	void _SetClassName(AnimaString className);

public:
	bool IsOfClass(AnimaString className);
	AnimaString _GetClassName() { return _className; }

protected:
	AnimaAllocator* _allocator;
	AnimaString _name;

	AnimaString _className;
	AnimaArray<AnimaString> _derivedClassNames;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaNamedObject__) */
