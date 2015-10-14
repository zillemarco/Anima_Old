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
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaVectorGenerator : public AnimaDataGenerator
{
	DECLARE_ANIMA_CLASS(AnimaVectorGenerator);
public:
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator);
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaVertex2f& color);
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat x, AFloat y);
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaVertex3f& color);
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat x, AFloat y, AFloat z);
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, const AnimaVertex4f& color);
	AnimaVectorGenerator(const AnimaString& name, AnimaAllocator* allocator, AFloat x, AFloat y, AFloat z, AFloat w);
	AnimaVectorGenerator(const AnimaVectorGenerator& src);
	AnimaVectorGenerator(AnimaVectorGenerator&& src);
	~AnimaVectorGenerator();

	AnimaVectorGenerator& operator=(const AnimaVectorGenerator& src);
	AnimaVectorGenerator& operator=(AnimaVectorGenerator&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, AnimaScene* scene, bool readName = true) override;
	
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
