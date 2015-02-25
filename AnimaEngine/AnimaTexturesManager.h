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
#include "AnimaStage.h"

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
	AnimaTexturesManager(AnimaStage* stage);
	~AnimaTexturesManager();
	
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromFile(const char* filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromFile(const AnimaString& filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromFile(const char* filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);

	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromBMPFile(const char* filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromBMPFile(const AnimaString& filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromBMPFile(const char* filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);

	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromTGAFile(const char* filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromTGAFile(const AnimaString& filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromTGAFile(const char* filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);

	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromDDSFile(const char* filePath, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromDDSFile(const AnimaString& filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromDDSFile(const char* filePath, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);

	AnimaTexture* LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromData(const char* textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadTextureFromData(const char* textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);

	AnimaTexture* GetTexture(AUint index);
	AnimaTexture* GetTexture(const AnimaString& textureName);
	AnimaTexture* GetTexture(const char* textureName);

	AnimaTexture* CreateTexture(const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp);
	AnimaTexture* CreateTexture(const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp);
	AnimaTexture* CreateTexture(const AnimaString& textureName, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp);
	AnimaTexture* CreateTexture(const char* textureName, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp);

private:
	void ClearTextures(bool bDeleteObjects = true, bool bResetNumber = true);

	AnimaTexture* LoadUncompressedTGA(FILE* file, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadUncompressedTGA(FILE* file, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadCompressedTGA(FILE* file, const AnimaString& textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
	AnimaTexture* LoadCompressedTGA(FILE* file, const char* textureName, AUint textureTarget = GL_TEXTURE_2D, AUint filter = GL_LINEAR, AUint internalFormat = GL_RGB, AUint format = GL_BGR, AUint dataType = GL_UNSIGNED_BYTE, AUint clamp = GL_REPEAT);
		
private:
	AnimaStage* _stage;

	AnimaTexture**	_textures;
	ASizeT			_texturesNumber;

#pragma warning (disable: 4251)
	boost::unordered_map<AnimaString, AUint, AnimaString::Hasher> _texturesMap;
#pragma warning (default: 4251) 
};

END_ANIMA_ENGINE_NAMESPACE

#endif  /* defined(__Anima__AnimaTexturesManager__) */
