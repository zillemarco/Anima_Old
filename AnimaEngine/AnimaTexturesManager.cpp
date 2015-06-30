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

GLubyte animaUTGAcompareRGBA[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed RGB/RGBA TGA Header
GLubyte animaUTGAcompareBW[12] = { 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed BW TGA Header
GLubyte animaCTGAcompare[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		// Compressed TGA Header

AnimaTexturesManager::AnimaTexturesManager(AnimaScene* scene)
	: _textures(scene->GetEngine()->GetTexturesAllocator())
{
	_scene = scene;
}

AnimaTexturesManager::~AnimaTexturesManager()
{
	ClearTextures();
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaTexture* oldTexture = _textures[textureName];
	if (oldTexture != nullptr)
		return oldTexture;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator());
	texture->SetName(textureName);
	texture->SetClamp(clamp);
	texture->SetFilter(filter);
	texture->SetInternalFormat(internalFormat);
	texture->SetFormat(format);
	texture->SetTextureTarget(textureTarget);
	texture->SetDataType(dataType);

	AInt newIndex = _textures.Add(textureName, texture);
	return _textures[newIndex];
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaTexture* oldTexture = _textures[textureName];
	if (oldTexture != nullptr)
		return oldTexture;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator(), textureName, textureTarget, width, height, data, dataSize, mipMapLevels, filter, internalFormat, format, dataType, clamp);
	AInt newIndex = _textures.Add(textureName, texture);
	return _textures[newIndex];
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return CreateTexture(str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const char* textureName, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return CreateTexture(str, textureTarget, width, height, data, dataSize, mipMapLevels, filter, internalFormat, format, dataType, clamp);
}

void AnimaTexturesManager::ClearTextures()
{
	AInt texturesCount = _textures.GetSize();
	for (AInt i = 0; i < texturesCount; i++)
	{
		AnimaTexture* texture = _textures[i];
		AnimaAllocatorNamespace::DeallocateObject(*(_scene->GetTexturesAllocator()), texture);
		texture = nullptr;
	}

	_textures.RemoveAll();
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaTexture* texture = _textures[textureName];
	if (texture != nullptr)
		return texture;

	AInt pos = filePath.rfind('.');

	if (pos == -1)
		return nullptr;

	pos++;
	AnimaString ext = filePath.substr(filePath.length() - pos);

	if (ext.compare("bmp") == 0)
		return LoadTextureFromBMPFile(filePath, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
	else if (ext.compare("tga") == 0)
		return LoadTextureFromTGAFile(filePath, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
	else if (ext.compare("dds") == 0)
		return LoadTextureFromDDSFile(filePath, textureName, textureTarget, filter, internalFormat, dataType, clamp);

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const char* filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = filePath;
	return LoadTextureFromFile(str, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const AnimaString& filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromFile(filePath, str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const char* filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromFile(filePath, str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaTexture* oldTexture = _textures[textureName];
	if (oldTexture != nullptr)
		return oldTexture;

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	
	unsigned char* data;

	FILE * file = fopen(filePath.c_str(), "rb");
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
	
	AnimaTexture* texture = LoadTextureFromData(textureName, data, imageSize, width, height, textureTarget, filter, internalFormat, format, dataType, clamp);

	delete[] data;

	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const char* filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = filePath;
	return LoadTextureFromBMPFile(str, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const AnimaString& filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromBMPFile(filePath, str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const char* filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromBMPFile(filePath, str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaTexture* texture = _textures[textureName];
	if (texture != nullptr)
		return texture;

	FILE* file;
	file = fopen(filePath.c_str(), "rb");

	if (file == NULL)
		return nullptr;

	AnimaTGAHeader tgaHeader;
	if (fread(&tgaHeader, sizeof(AnimaTGAHeader), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	if (memcmp(animaUTGAcompareRGBA, &tgaHeader, sizeof(tgaHeader)) == 0 || memcmp(animaUTGAcompareBW, &tgaHeader, sizeof(tgaHeader)) == 0)
		return LoadUncompressedTGA(file, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
	else if (memcmp(animaCTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)
		return LoadCompressedTGA(file, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
	else
	{
		fclose(file);
		return nullptr;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const char* filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = filePath;
	return LoadTextureFromTGAFile(str, textureName, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const AnimaString& filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromBMPFile(filePath, str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const char* filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromTGAFile(filePath, str, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadUncompressedTGA(FILE * file, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
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

	if ((width <= 0) || (height <= 0) || ((bpp != 8) && (bpp != 24) && (bpp != 32)))
	{
		if (file != NULL)
			fclose(file);
		return nullptr;
	}

	bool invert = true;

	if (bpp == 24)
		type = GL_RGB;
	else if (bpp == 32)
		type = GL_RGBA;
	else
	{
		type = GL_RGBA;
		invert = false;
	}

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

	for (GLuint cswap = 0; cswap < (int)tga.imageSize && invert; cswap += tga.bytesPerPixel)
		imageData[cswap] ^= imageData[cswap + 2] ^=	imageData[cswap] ^= imageData[cswap + 2];

	fclose(file);

	AnimaTexture* texture = LoadTextureFromData(textureName, imageData, tga.imageSize, width, height, textureTarget, filter, internalFormat, format, dataType, clamp);

	free(imageData);

	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadCompressedTGA(FILE * file, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
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

	AnimaTexture* texture = LoadTextureFromData(textureName, imageData, tga.imageSize, width, height, textureTarget, filter, internalFormat, format, dataType, clamp);
	free(imageData);
	
	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint dataType, AUint clamp)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return _textures[index];

	unsigned char Header[124];
	FILE * file;

	file = fopen(filePath.c_str(), "rb");
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

	AnimaTexture* texture = LoadTextureFromData(textureName, buffer, bufsize, width, height, mipMapCount, textureTarget, filter, internalFormat, format, dataType, clamp);

	free(buffer);

	return texture;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const char* filePath, const AnimaString& textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint dataType, AUint clamp)
{
	AnimaString str = filePath;
	return LoadTextureFromDDSFile(str, textureName, textureTarget, filter, internalFormat, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const AnimaString& filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromDDSFile(filePath, str, textureTarget, filter, internalFormat, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const char* filePath, const char* textureName, AUint textureTarget, AUint filter, AUint internalFormat, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromDDSFile(filePath, str, textureTarget, filter, internalFormat, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	return CreateTexture(textureName, textureTarget, width, height, data, dataSize, 0, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	return CreateTexture(textureName, textureTarget, width, height, data, dataSize, mipMapLevels, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(const char* textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromData(str, data, dataSize, width, height, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(const char* textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height, AInt mipMapLevels, AUint textureTarget, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp)
{
	AnimaString str = textureName;
	return LoadTextureFromData(str, data, dataSize, width, height, mipMapLevels, textureTarget, filter, internalFormat, format, dataType, clamp);
}

AInt AnimaTexturesManager::GetTexturesCount() const
{
	return _textures.GetSize();
}

AnimaTexture* AnimaTexturesManager::GetTexture(AUint index)
{
	return _textures[index];
}

AnimaTexture* AnimaTexturesManager::GetTexture(const AnimaString& textureName)
{
	return _textures[textureName];
}

AnimaTexture* AnimaTexturesManager::GetTexture(const char* textureName)
{
	AnimaString str = textureName;
	return GetTexture(str);
}

END_ANIMA_ENGINE_NAMESPACE