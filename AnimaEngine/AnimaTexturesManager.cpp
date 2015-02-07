//
//  AnimaTexturesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaTexturesManager.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

BEGIN_ANIMA_ENGINE_NAMESPACE

GLubyte animaUTGAcompare[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed TGA Header
GLubyte animaCTGAcompare[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Compressed TGA Header

AnimaTexturesManager::AnimaTexturesManager(AnimaStage* stage)
{
	_stage = stage;

	_textures = nullptr;
	_texturesNumber = 0;
}

AnimaTexturesManager::~AnimaTexturesManager()
{
	ClearTextures();
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName)
{
	if (_texturesMap.find(textureName) != _texturesMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_texturesNumber > 0)
	{
		AnimaTexture** tmpOldTextures = AnimaAllocatorNamespace::AllocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _texturesNumber);

		for (int i = 0; i < _texturesNumber; i++)
			tmpOldTextures[i] = _textures[i];

		ClearTextures(false, false);

		_texturesNumber++;
		_textures = AnimaAllocatorNamespace::AllocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _texturesNumber);

		for (int i = 0; i < _texturesNumber - 1; i++)
			_textures[i] = tmpOldTextures[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetTexturesAllocator()), tmpOldTextures);
		tmpOldTextures = nullptr;
	}
	else
	{
		_texturesNumber++;
		_textures = AnimaAllocatorNamespace::AllocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _texturesNumber);
	}

	_textures[_texturesNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_stage->GetTexturesAllocator()), _stage->GetTexturesAllocator());

	_texturesMap[textureName] = (AUint)(_texturesNumber - 1);

	return _textures[_texturesNumber - 1];
}

AnimaTexture* AnimaTexturesManager::CreateTexture(AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format, const AnimaString& textureName)
{
	if (_texturesMap.find(textureName) != _texturesMap.end())
		return nullptr;

	ANIMA_ASSERT(_stage != nullptr);
	if (_texturesNumber > 0)
	{
		AnimaTexture** tmpOldTextures = AnimaAllocatorNamespace::AllocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _texturesNumber);

		for (int i = 0; i < _texturesNumber; i++)
			tmpOldTextures[i] = _textures[i];

		ClearTextures(false, false);

		_texturesNumber++;
		_textures = AnimaAllocatorNamespace::AllocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _texturesNumber);

		for (int i = 0; i < _texturesNumber - 1; i++)
			_textures[i] = tmpOldTextures[i];

		AnimaAllocatorNamespace::DeallocateArray(*(_stage->GetTexturesAllocator()), tmpOldTextures);
		tmpOldTextures = nullptr;
	}
	else
	{
		_texturesNumber++;
		_textures = AnimaAllocatorNamespace::AllocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _texturesNumber);
	}

	_textures[_texturesNumber - 1] = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_stage->GetTexturesAllocator()), _stage->GetTexturesAllocator(), data, dataSize, width, height, mipMapLevels, format);

	_texturesMap[textureName] = (AUint)(_texturesNumber - 1);

	return _textures[_texturesNumber - 1];
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return CreateTexture(str);
}

AnimaTexture* AnimaTexturesManager::CreateTexture(AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return CreateTexture(data, dataSize, width, height, mipMapLevels, format, str);
}

void AnimaTexturesManager::ClearTextures(bool bDeleteObjects, bool bResetNumber)
{
	if (_textures != nullptr)
	{
		if (bDeleteObjects)
		{
			for (int i = 0; i < (int)_texturesNumber; i++)
			{
				AnimaAllocatorNamespace::DeallocateObject(*(_stage->GetTexturesAllocator()), _textures[i]);
				_textures[i] = nullptr;
			}
		}
		
		AnimaAllocatorNamespace::DeallocateArray<AnimaTexture*>(*(_stage->GetTexturesAllocator()), _textures);
		_textures = nullptr;
	}
	
	if (bResetNumber)
		_texturesNumber = 0;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt pos = filePath.ReverseFind('.');

	if (pos == -1)
		return nullptr;

	pos++;
	AnimaString ext = filePath.Right(filePath.GetBufferLength() - pos);

	if (ext.CompareNoCase("bmp"))
		return LoadTextureFromBMPFile(filePath, textureName);
	else if (ext.CompareNoCase("tga"))
		return LoadTextureFromTGAFile(filePath, textureName);
	else if (ext.CompareNoCase("dds"))
		return LoadTextureFromDDSFile(filePath, textureName);

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const char* filePath, const AnimaString& textureName)
{
	AnimaString str(filePath, _stage->GetStringAllocator());
	return LoadTextureFromFile(str, textureName);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const AnimaString& filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const char* filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	
	unsigned char* data;

	FILE * file = fopen(filePath.GetConstBuffer(), "rb");
	if (!file)
		return nullptr;

	if (fread(header, 1, 54, file) != 54)
		return nullptr;

	if (header[0] != 'B' || header[1] != 'M')
		return nullptr;

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = width * height * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);
	
	fclose(file);

	AnimaTexture* texture = LoadTextureFromData(data, imageSize, width, height, textureName);

	delete[] data;

	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const char* filePath, const AnimaString& textureName)
{
	AnimaString str(filePath, _stage->GetStringAllocator());
	return LoadTextureFromBMPFile(str, textureName);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const AnimaString& filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromBMPFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const char* filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromBMPFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName)
{
	FILE* file;
	file = fopen(filePath.GetConstBuffer(), "rb");

	if (file == NULL)
		return nullptr;

	AnimaTGAHeader tgaHeader;
	if (fread(&tgaHeader, sizeof(AnimaTGAHeader), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	if (memcmp(animaUTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)
		return LoadUncompressedTGA(file, textureName);
	else if (memcmp(animaCTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)
		return LoadCompressedTGA(file, textureName);
	else
	{
		fclose(file);
		return nullptr;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const char* filePath, const AnimaString& textureName)
{
	AnimaString str(filePath, _stage->GetStringAllocator());
	return LoadTextureFromTGAFile(str, textureName);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const AnimaString& filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromBMPFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const char* filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromTGAFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadUncompressedTGA(FILE * file, const AnimaString& textureName)
{
	AnimaTGA tga;
	if (fread(tga.header, sizeof(tga.header), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	AUint width = tga.header[1] * 256 + tga.header[0];
	AUint height = tga.header[3] * 256 + tga.header[2];
	AUint bpp = tga.header[4];
	AUint type;
	unsigned char* imageData = NULL;

	tga.Width = width;
	tga.Height = height;
	tga.Bpp = bpp;

	if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp != 32)))
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	if (bpp == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;

	tga.bytesPerPixel = (tga.Bpp / 8);
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	imageData = (GLubyte *)malloc(tga.imageSize);

	if (imageData == NULL)
	{
		fclose(file);
		return nullptr;
	}

	if (fread(imageData, 1, tga.imageSize, file) != tga.imageSize)
	{
		if (imageData != NULL)
			free(imageData);
		fclose(file);
		return nullptr;
	}

	for (GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
		imageData[cswap] ^= imageData[cswap + 2] ^=	imageData[cswap] ^= imageData[cswap + 2];

	fclose(file);

	AnimaTexture* texture = LoadTextureFromData(imageData, tga.imageSize, width, height, textureName);

	free(imageData);

	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadCompressedTGA(FILE * file, const AnimaString& textureName)
{
	AnimaTGA tga;
	if (fread(tga.header, sizeof(tga.header), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	AUint width = tga.header[1] * 256 + tga.header[0];
	AUint height = tga.header[3] * 256 + tga.header[2];
	AUint bpp = tga.header[4];
	AUint type;
	unsigned char* imageData = NULL;

	tga.Width = width;
	tga.Height = height;
	tga.Bpp = bpp;

	if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp != 32)))
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	if (bpp == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;

	tga.bytesPerPixel = (tga.Bpp / 8);
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	imageData = (GLubyte *)malloc(tga.imageSize);

	if (imageData == NULL)
	{
		fclose(file);
		return nullptr;
	}

	GLuint pixelcount = tga.Height * tga.Width;
	GLuint currentpixel = 0;
	GLuint currentbyte = 0;
	GLubyte * colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);

	do
	{
		GLubyte chunkheader = 0;

		if (fread(&chunkheader, sizeof(GLubyte), 1, file) == 0)
		{
			if (file != NULL)
				fclose(file);
			if (imageData != NULL)
				free(imageData);
			return nullptr;
		}

		if (chunkheader < 128)
		{
			chunkheader++;
			for (short counter = 0; counter < chunkheader; counter++)
			{
				if (fread(colorbuffer, 1, tga.bytesPerPixel, file) != tga.bytesPerPixel)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);	

					if (imageData != NULL)
						free(imageData);

					return nullptr;
				}

				imageData[currentbyte] = colorbuffer[2];
				imageData[currentbyte + 1] = colorbuffer[1];
				imageData[currentbyte + 2] = colorbuffer[0];

				if (tga.bytesPerPixel == 4)
					imageData[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);

					if (imageData != NULL)
						free(imageData);

					return nullptr;
				}
			}
		}
		else
		{
			chunkheader -= 127;
			if (fread(colorbuffer, 1, tga.bytesPerPixel, file) != tga.bytesPerPixel)
			{
				if (file != NULL)
					fclose(file);

				if (colorbuffer != NULL)
					free(colorbuffer);

				if (imageData != NULL)
					free(imageData);

				return nullptr;
			}

			for (short counter = 0; counter < chunkheader; counter++)
			{
				imageData[currentbyte] = colorbuffer[2];
				imageData[currentbyte + 1] = colorbuffer[1];
				imageData[currentbyte + 2] = colorbuffer[0];

				if (tga.bytesPerPixel == 4)
					imageData[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);

					if (imageData != NULL)
						free(imageData);

					return nullptr;
				}
			}
		}
	}

	while (currentpixel < pixelcount);
	fclose(file);

	AnimaTexture* texture = LoadTextureFromData(imageData, tga.imageSize, width, height, textureName);
	free(imageData);
	
	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName)
{
	unsigned char Header[124];
	FILE * file;

	file = fopen(filePath.GetConstBuffer(), "rb");
	if (file == NULL)
		return nullptr;

	char filetype[4];
	fread(filetype, 1, 4, file);
	if (strncmp(filetype, "DDS ", 4) != 0)
	{
		fclose(file);
		return nullptr;
	}

	fread(&Header, 124, 1, file);

	unsigned int height = *(unsigned int*)&(Header[8]);
	unsigned int width = *(unsigned int*)&(Header[12]);
	unsigned int linearSize = *(unsigned int*)&(Header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(Header[24]);
	unsigned int fourCC = *(unsigned int*)&(Header[80]);

	unsigned char* buffer;
	unsigned int bufsize;

	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char *)malloc(bufsize * sizeof(unsigned char));

	fread(buffer, 1, bufsize, file);
	fclose(file);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;

	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return nullptr;
	}

	AnimaTexture* texture = LoadTextureFromData(buffer, bufsize, width, height, mipMapCount, format, textureName);

	free(buffer);

	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const char* filePath, const AnimaString& textureName)
{
	AnimaString str(filePath, _stage->GetStringAllocator());
	return LoadTextureFromDDSFile(str, textureName);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const AnimaString& filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromDDSFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const char* filePath, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromDDSFile(filePath, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, const AnimaString& textureName)
{
	return CreateTexture(data, dataSize, width, height, 0, 0, textureName);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint format, const AnimaString& textureName)
{
	return CreateTexture(data, dataSize, width, height, mipMapLevels, format, textureName);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromData(data, dataSize, width, height, str);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint format, const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return LoadTextureFromData(data, dataSize, width, height, mipMapLevels, format, str);
}

AnimaTexture* AnimaTexturesManager::GetTexture(AUint index)
{
	ANIMA_ASSERT(index >= 0 && index < _texturesNumber);
	return _textures[index];
}

AnimaTexture* AnimaTexturesManager::GetTexture(const AnimaString& textureName)
{
	if (_texturesMap.find(textureName) == _texturesMap.end())
		return nullptr;

	return GetTexture(_texturesMap[textureName]);
}

AnimaTexture* AnimaTexturesManager::GetTexture(const char* textureName)
{
	AnimaString str(textureName, _stage->GetStringAllocator());
	return GetTexture(str);
}

END_ANIMA_ENGINE_NAMESPACE