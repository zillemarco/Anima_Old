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

class AnimaTexturesManager;

class ANIMA_ENGINE_EXPORT AnimaTextureGenerator : public AnimaDataGenerator
{
	DECLARE_ANIMA_CLASS(AnimaTextureGenerator);
public:
	AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator, AnimaTexturesManager* texturesManager);
	AnimaTextureGenerator(const AnimaString& name, AnimaAllocator* allocator, AnimaTexture* texture, AnimaTexturesManager* texturesManager);
	AnimaTextureGenerator(const AnimaTextureGenerator& src);
	AnimaTextureGenerator(AnimaTextureGenerator&& src);
	~AnimaTextureGenerator();

	AnimaTextureGenerator& operator=(const AnimaTextureGenerator& src);
	AnimaTextureGenerator& operator=(AnimaTextureGenerator&& src);
	
public:
	ptree GetObjectTree(bool saveName = true) const override;
	bool ReadObject(const ptree& objectTree, bool readName = true) override;
	
public:
	void UpdateValue() override;

	void SetTexture(AnimaTexture* texture) override;
	AnimaTexture* GetTexture() override;

protected:
	AnimaTexture* _texture;
	AnimaTexturesManager* _texturesManager;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTextureGenerator__) */
