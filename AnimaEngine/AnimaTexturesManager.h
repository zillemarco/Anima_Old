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
	
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromFile(const char* filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath, const char* textureName);
	AnimaTexture* LoadTextureFromFile(const char* filePath, const char* textureName);

	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromBMPFile(const char* filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath, const char* textureName);
	AnimaTexture* LoadTextureFromBMPFile(const char* filePath, const char* textureName);

	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromTGAFile(const char* filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath, const char* textureName);
	AnimaTexture* LoadTextureFromTGAFile(const char* filePath, const char* textureName);

	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromDDSFile(const char* filePath, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath, const char* textureName);
	AnimaTexture* LoadTextureFromDDSFile(const char* filePath, const char* textureName);

	AnimaTexture* LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint format, const AnimaString& textureName);
	AnimaTexture* LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, const char* textureName);
	AnimaTexture* LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint format, const char* textureName);

	AnimaTexture* GetTexture(AUint index);
	AnimaTexture* GetTexture(const AnimaString& textureName);
	AnimaTexture* GetTexture(const char* textureName);
	
private:
	void ClearTextures(bool bDeleteObjects = true, bool bResetNumber = true);
	AnimaTexture* CreateTexture(const AnimaString& textureName);
	AnimaTexture* CreateTexture(const char* textureName);
	AnimaTexture* CreateTexture(AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format, const AnimaString& textureName);
	AnimaTexture* CreateTexture(AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format, const char* textureName);

	AnimaTexture* LoadUncompressedTGA(FILE* file, const AnimaString& textureName);
	AnimaTexture* LoadUncompressedTGA(FILE* file, const char* textureName);
	AnimaTexture* LoadCompressedTGA(FILE* file, const AnimaString& textureName);
	AnimaTexture* LoadCompressedTGA(FILE* file, const char* textureName);
		
private:
	AnimaEngine* _engine;

	AnimaTexture**	_textures;
	ASizeT			_texturesNumber;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _texturesMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaTexturesManager__) */
