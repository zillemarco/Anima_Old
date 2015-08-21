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
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaColorGenerator : public AnimaDataGenerator
{
	DECLARE_ANIMA_CLASS(AnimaColorGenerator);
public:
	AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator);
	AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaColor3f& color);
	AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat r, AFloat g, AFloat b);
	AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaColor4f& color);
	AnimaColorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat r, AFloat g, AFloat b, AFloat a);
	AnimaColorGenerator(const AnimaColorGenerator& src);
	AnimaColorGenerator(AnimaColorGenerator&& src);
	~AnimaColorGenerator();

	AnimaColorGenerator& operator=(const AnimaColorGenerator& src);
	AnimaColorGenerator& operator=(AnimaColorGenerator&& src);

public:
	void UpdateValue() override;

	void SetColor(const AnimaColor3f& color) override;
	void SetColor(AFloat r, AFloat g, AFloat b) override;
	void SetColor(const AnimaColor4f& color) override;
	void SetColor(AFloat r, AFloat g, AFloat b, AFloat a) override;
	AnimaColor3f GetColor3f() override;
	AnimaColor4f GetColor4f() override;

protected:
	AnimaColor4f _color;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaColorGenerator__) */
