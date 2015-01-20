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
	AnimaTexturesManager(AnimaEngine* engine);
	~AnimaTexturesManager();
	
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath);
	AnimaTexture* LoadTextureFromFile(const char* filePath);

	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath);
	AnimaTexture* LoadTextureFromBMPFile(const char* filePath);

	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath);
	AnimaTexture* LoadTextureFromTGAFile(const char* filePath);

	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath);
	AnimaTexture* LoadTextureFromDDSFile(const char* filePath);

	AnimaTexture* LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height);
	AnimaTexture* LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint format);
	
private:
	void ClearTextures(bool bDeleteObjects = true, bool bResetNumber = true);
	AnimaTexture* CreateTexture();
	AnimaTexture* CreateTexture(AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format);

	AnimaTexture* LoadUncompressedTGA(FILE* file);
	AnimaTexture* LoadCompressedTGA(FILE* file);
		
private:
	AnimaEngine* _engine;

	AnimaTexture**	_textures;
	ASizeT			_texturesNumber;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTexturesManager__) */
