//
//  AnimaTexturesManager.h
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#ifndef __Anima__AnimaTexturesManager__
#define __Anima__AnimaTexturesManager__

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaTexture.h"
#include "AnimaMappedArray.h"
#include "AnimaScene.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

struct AnimaTGAHeader {
	GLubyte Header[12];
};

struct AnimaTGA {
	GLubyte		header[6];
	GLuint		bytesPerPixel;
	GLuint		imageSize;
	GLuint		temp;
	GLuint		type;
	GLuint		Height;
	GLuint		Width;
	GLuint		Bpp;
};

class ANIMA_ENGINE_EXPORT AnimaTexturesManager
{
public:
	AnimaTexturesManager(AnimaScene* scene);
	~AnimaTexturesManager();

	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height);

	AInt GetTexturesCount() const;
	AnimaTexture* GetTexture(AUint index);
	AnimaTexture* GetTexture(const AnimaString& textureName);

	AnimaTexture* CreateTexture(const AnimaString& textureName);
	AnimaTexture* CreateTexture(const AnimaString& textureName, AUint width, AUint height, AUchar* data, ASizeT dataSize);
	
private:
	void ClearTextures();

	AnimaTexture* LoadUncompressedTGA(FILE* file, const AnimaString& textureName);
	AnimaTexture* LoadCompressedTGA(FILE* file, const AnimaString& textureName);
	
private:
	AnimaScene* _scene;

	AnimaMappedArray<AnimaTexture*> _textures;
};

END_ANIMA_ENGINE_NAMESPACE

#endif  /* defined(__Anima__AnimaTexturesManager__) */
