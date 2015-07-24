//
//  AnimaTextureGenerator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaTextureGenerator__
#define __Anima__AnimaTextureGenerator__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaTexture.h"
#include "AnimaDataGenerator.h"
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaTextureGenerator : public AnimaDataGenerator
{
public:
	AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator);
	AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator, AnimaTexture* texture);
	AnimaTextureGenerator(const AnimaTextureGenerator& src);
	AnimaTextureGenerator(AnimaTextureGenerator&& src);
	~AnimaTextureGenerator();

	AnimaTextureGenerator& operator=(const AnimaTextureGenerator& src);
	AnimaTextureGenerator& operator=(AnimaTextureGenerator&& src);

public:
	void UpdateValue() override;

	void SetTexture(AnimaTexture* texture) override;
	AnimaTexture* GetTexture() override;

protected:
	AnimaTexture* _texture;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTextureGenerator__) */
