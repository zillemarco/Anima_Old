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
{
	_scene = scene;
}

AnimaTexturesManager::~AnimaTexturesManager()
{
	ClearTextures();
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator());
	texture->SetName(textureName);

	AInt newIndex = _textures.Add(textureName, texture);
	return _textures[newIndex];
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName, AUint width, AUint height, AUchar* data, ASizeT dataSize)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator(), textureName, width, height, data, dataSize);
	AInt newIndex = _textures.Add(textureName, texture);
	return _textures[newIndex];
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

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AUchar* data;
	AUint dataSize;
	AUint width;
	AUint height;
	AUint format;

	if (GetTextureDataFromFile(filePath, &data, dataSize, width, height, format))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, data, dataSize);
		
		if (texture != nullptr && format != 0)
			texture->SetFormat(format);

		free(data);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;
	
	AUchar* data;
	AUint dataSize;
	AUint width;
	AUint height;

	if (GetTextureDataFromBMPFile(filePath, &data, dataSize, width, height))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, data, dataSize);
		free(data);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AUchar* data;
	AUint dataSize;
	AUint width;
	AUint height;

	if (GetTextureDataFromTGAFile(filePath, &data, dataSize, width, height))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, data, dataSize);
		free(data);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;
	
	AUchar* data;
	AUint dataSize;
	AUint width;
	AUint height;
	AUint format;

	if (GetTextureDataFromDDSFile(filePath, &data, dataSize, width, height, format))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, data, dataSize);
		
		if (texture != nullptr && format != 0)
			texture->SetFormat(format);

		free(data);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(const AnimaString& textureName, AUchar* data, ASizeT dataSize, AUint width, AUint height)
{
	return CreateTexture(textureName, width, height, data, dataSize);
}

bool AnimaTexturesManager::GetTextureDataFromFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height, AUint& format)
{
	if (data == nullptr || *data != nullptr)
		return false;

	AInt pos = filePath.rfind('.');
	if (pos == -1)
		return false;

	pos++;
	AnimaString ext = filePath.substr(pos);

	format = 0;

	if (ext.compare("bmp") == 0)
		return GetTextureDataFromBMPFile(filePath, data, dataSize, width, height);
	else if (ext.compare("tga") == 0)
		return GetTextureDataFromTGAFile(filePath, data, dataSize, width, height);
	else if (ext.compare("dds") == 0)
		return GetTextureDataFromDDSFile(filePath, data, dataSize, width, height, format);

	return false;
}

bool AnimaTexturesManager::GetTextureDataFromBMPFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height)
{
	if (data == nullptr || *data != nullptr)
		return false;

	unsigned char header[54];
	unsigned int dataPos;
	
	FILE * file = fopen(filePath.c_str(), "rb");
	if (!file)
		return nullptr;

	if (fread(header, 1, 54, file) != 54)
		return nullptr;

	if (header[0] != 'B' || header[1] != 'M')
		return nullptr;

	dataPos = *(int*)&(header[0x0A]);
	dataSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (dataSize == 0)
		dataSize = width * height * 3;

	if (dataPos == 0)
		dataPos = 54;

	*data = (AUchar*)malloc(dataSize * sizeof(AUchar));
	fread(*data, 1, dataSize, file);
	fclose(file);

	return true;
}

bool AnimaTexturesManager::GetTextureDataFromTGAFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height)
{
	if (data == nullptr || *data != nullptr)
		return false;

	FILE* file;
	file = fopen(filePath.c_str(), "rb");

	if (file == NULL)
		return false;

	AnimaTGAHeader tgaHeader;
	if (fread(&tgaHeader, sizeof(AnimaTGAHeader), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return false;
	}

	if (memcmp(animaUTGAcompareRGBA, &tgaHeader, sizeof(tgaHeader)) == 0 || memcmp(animaUTGAcompareBW, &tgaHeader, sizeof(tgaHeader)) == 0)
		return GetUncompressedTGAData(file, data, dataSize, width, height);
	else if (memcmp(animaCTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)
		return GetCompressedTGAData(file, data, dataSize, width, height);
	else
	{
		fclose(file);
		return false;
	}
	return false;
}

bool AnimaTexturesManager::GetTextureDataFromDDSFile(const AnimaString& filePath, AUchar** data, AUint& dataSize, AUint& width, AUint& height, AUint& format)
{
	if (data == nullptr || *data != nullptr)
		return false;

	unsigned char Header[124];

	FILE * file;
	file = fopen(filePath.c_str(), "rb");
	if (file == NULL)
		return false;

	char filetype[4];
	fread(filetype, 1, 4, file);
	if (strncmp(filetype, "DDS ", 4) != 0)
	{
		fclose(file);
		return false;
	}

	fread(&Header, 124, 1, file);

	height = *(unsigned int*)&(Header[8]);
	width = *(unsigned int*)&(Header[12]);
	unsigned int linearSize = *(unsigned int*)&(Header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(Header[24]);
	unsigned int fourCC = *(unsigned int*)&(Header[80]);

	dataSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	*data = (AUchar*)malloc(dataSize * sizeof(AUchar));

	fread(*data, 1, dataSize, file);
	fclose(file);

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
		free(*data);
		return false;
	}

	return true;
}

bool AnimaTexturesManager::GetUncompressedTGAData(FILE * file, AUchar** data, AUint& dataSize, AUint& width, AUint& height)
{
	AnimaTGA tga;
	if (fread(tga.header, sizeof(tga.header), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return false;
	}

	width = tga.header[1] * 256 + tga.header[0];
	height = tga.header[3] * 256 + tga.header[2];
	AUint bpp = tga.header[4];
	AUint type;

	tga.Width = width;
	tga.Height = height;
	tga.Bpp = bpp;

	if ((width <= 0) || (height <= 0) || ((bpp != 8) && (bpp != 24) && (bpp != 32)))
	{
		if (file != NULL)
			fclose(file);
		return false;
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
	dataSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	*data = (GLubyte*)malloc(dataSize);

	if (*data == NULL)
	{
		fclose(file);
		return false;
	}

	if (fread(*data, 1, dataSize, file) != dataSize)
	{
		if (*data != NULL)
			free(*data);
		fclose(file);

		return false;
	}

	for (GLuint cswap = 0; cswap < (int)dataSize && invert; cswap += tga.bytesPerPixel)
		(*data)[cswap] ^= (*data)[cswap + 2] ^= (*data)[cswap] ^= (*data)[cswap + 2];

	fclose(file);

	return true;
}

bool AnimaTexturesManager::GetCompressedTGAData(FILE* file, AUchar** data, AUint& dataSize, AUint& width, AUint& height)
{
	AnimaTGA tga;
	if (fread(tga.header, sizeof(tga.header), 1, file) == 0)
	{
		if (file != NULL)
			fclose(file);
		return false;
	}

	width = tga.header[1] * 256 + tga.header[0];
	height = tga.header[3] * 256 + tga.header[2];
	AUint bpp = tga.header[4];
	AUint type;

	tga.Width = width;
	tga.Height = height;
	tga.Bpp = bpp;

	if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp != 32)))
	{
		if (file != NULL)
			fclose(file);
		return false;
	}

	if (bpp == 24)
		type = GL_RGB;
	else
		type = GL_RGBA;

	tga.bytesPerPixel = (tga.Bpp / 8);
	tga.imageSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	*data = (GLubyte *)malloc(tga.imageSize);

	if (*data == NULL)
	{
		fclose(file);
		return false;
	}

	GLuint pixelcount = tga.Height * tga.Width;
	GLuint currentpixel = 0;
	GLuint currentbyte = 0;
	GLubyte* colorbuffer = (GLubyte *)malloc(tga.bytesPerPixel);

	do
	{
		GLubyte chunkheader = 0;

		if (fread(&chunkheader, sizeof(GLubyte), 1, file) == 0)
		{
			if (file != NULL)
				fclose(file);
			
			if (*data != NULL)
				free(*data);

			return false;
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

					if (*data != NULL)
						free(*data);

					return false;
				}

				*data[currentbyte] = colorbuffer[2];
				*data[currentbyte + 1] = colorbuffer[1];
				*data[currentbyte + 2] = colorbuffer[0];

				if (tga.bytesPerPixel == 4)
					*data[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);

					if (*data != NULL)
						free(*data);

					return false;
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

				if (*data != NULL)
					free(*data);

				return false;
			}

			for (short counter = 0; counter < chunkheader; counter++)
			{
				*data[currentbyte] = colorbuffer[2];
				*data[currentbyte + 1] = colorbuffer[1];
				*data[currentbyte + 2] = colorbuffer[0];

				if (tga.bytesPerPixel == 4)
					*data[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);

					if (*data != NULL)
						free(*data);

					return false;
				}
			}
		}
	}

	while (currentpixel < pixelcount);
	fclose(file);

	return true;
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

END_ANIMA_ENGINE_NAMESPACE