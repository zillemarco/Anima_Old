//
//  AnimaTexturesManager.cpp
//  Anima
//
//  Created by Marco Zille on 14/01/15.
//
//

#include "AnimaTexturesManager.h"
#include "AnimaXmlTranslators.h"
#include "AnimaTools.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

BEGIN_ANIMA_ENGINE_NAMESPACE

GLubyte animaUTGAcompareRGBA[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed RGB/RGBA TGA Header
GLubyte animaUTGAcompareBW[12] = { 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };	// Uncompressed BW TGA Header
GLubyte animaCTGAcompare[12] = { 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0 };		// Compressed TGA Header

enum DXGIFormat {
	DXGIFormat_FORCE_UINT = 0xffffffff
};

enum DXGIResourceDimension {
	DXGIResourceDimension_UNKNOWN = 0,
	DXGIResourceDimension_BUFFER = 1,
	DXGIResourceDimension_TEXTURE1D = 2,
	DXGIResourceDimension_TEXTURE2D = 3,
	DXGIResourceDimension_TEXTURE3D = 4
};

typedef struct {
	AUint dwSize;
	AUint dwFlags;
	AUint dwFourCC;
	AUint dwRGBBitCount;
	AUint dwRBitMask;
	AUint dwGBitMask;
	AUint dwBBitMask;
	AUint dwABitMask;
} DDSPixelFormat;

typedef struct {
	AUint			dwSize;
	AUint			dwFlags;
	AUint			dwHeight;
	AUint			dwWidth;
	AUint			dwPitchOrLinearSize;
	AUint			dwDepth;
	AUint			dwMipMapCount;
	AUint			dwReserved1[11];
	DDSPixelFormat	ddspf;
	AUint			dwCaps;
	AUint			dwCaps2;
	AUint			dwCaps3;
	AUint			dwCaps4;
	AUint			dwReserved2;
} DDSFileHeader;

typedef struct {
	DXGIFormat				dxgiFormat;
	DXGIResourceDimension	resourceDimension;
	AUint					miscFlag;
	AUint					arraySize;
	AUint					miscFlags2;
} DDSFileHeader10;

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

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName, AUint width, AUint height)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator(), textureName, width, height);
	AInt newIndex = _textures.Add(textureName, texture);
	return _textures[newIndex];
}

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName, AUint width, AUint height, AUint depth)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator(), textureName, width, height, depth);
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

AnimaTexture* AnimaTexturesManager::CreateTexture(const AnimaString& textureName, AUint width, AUint height, AUint depth, AUchar* data, ASizeT dataSize)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*(_scene->GetTexturesAllocator()), _scene->GetTexturesAllocator(), textureName, width, height, depth, data, dataSize);
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

	AnimaArray<AUchar> data;
	AUint width;
	AUint height;

	// Provo a leggere una texture qualsiasi
	if (GetTextureDataFromFile(filePath, &data, width, height))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, &data[0], data.size());
		texture->SetSourceFileName(filePath);
		return texture;
	}
	// Se non ha funzionato provo a leggere la texture come file DDS
	else
		return LoadTextureFromDDSFile(filePath, textureName);

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromBMPFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaArray<AUchar> data;
	AUint width;
	AUint height;

	if (GetTextureDataFromBMPFile(filePath, &data, width, height))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, &data[0], data.size());
		texture->SetSourceFileName(filePath);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromTGAFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaArray<AUchar> data;
	AUint width;
	AUint height;

	if (GetTextureDataFromTGAFile(filePath, &data, width, height))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, &data[0], data.size());
		texture->SetSourceFileName(filePath);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromDDSFile(const AnimaString& filePath, const AnimaString& textureName)
{
	AInt index = _textures.Contains(textureName);
	if (index >= 0)
		return nullptr;

	AnimaArray<AnimaArray<AUchar> > data;
	AUint width;
	AUint height;
	AUint depth;
	AUint mipMapsCount;
	AUint imagesCount;
	AnimaTextureFormat format;
	AnimaTextureInternalFormat internalFormat;
	AnimaTextureTarget target;
	
	if (GetTextureDataFromDDSFile(filePath, &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
	{
		AnimaTexture* texture = CreateTexture(textureName, width, height, depth);
		if (texture != nullptr)
		{
			// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
			mipMapsCount = fmax(mipMapsCount, 1);

			if (format != 0 && internalFormat != 0)
			{
				texture->SetFormat(format);
				texture->SetInternalFormat(internalFormat);
			}

			texture->SetTextureTarget(target);
			texture->SetMipMapLevels(mipMapsCount);

			// se il target è una texture cube devo comportarmi diversamente
			if (target == TEXTURE_TARGET_CUBE)
			{
				AInt offset = 0;
				for (AUint i = 0; i < imagesCount; i++)
				{
					for (AUint j = 0; j < mipMapsCount; j++)
					{
						AUchar* buffer = &data[offset][0];
						AUint bufferSize = data[offset].size();

						texture->SetData(buffer, bufferSize, (AnimaTextureCubeIndex)i, j);

						offset++;
					}
				}
			}
			else
			{
				AInt offset = 0;
				for (AUint i = 0; i < imagesCount; i++)
				{
					for (AUint j = 0; j < mipMapsCount; j++)
					{
						AUchar* buffer = &data[offset][0];
						AUint bufferSize = data[offset].size();

						texture->SetData(buffer, bufferSize, j);

						offset++;
					}
				}
			}
		}
		
		texture->SetSourceFileName(filePath);
		return texture;
	}

	return nullptr;
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromData(const AnimaString& textureName, AUchar* data, AUint dataSize, AUint width, AUint height, AUint depth)
{
	return CreateTexture(textureName, width, height, depth, data, dataSize);
}

bool AnimaTexturesManager::GetTextureDataFromFile(const AnimaString& filePath, AnimaArray<AUchar>* data, AUint& width, AUint& height)
{
	AInt pos = filePath.rfind('.');
	if (pos == -1)
		return false;

	pos++;
	AnimaString ext = filePath.substr(pos);

	if (ext.compare("bmp") == 0)
		return GetTextureDataFromBMPFile(filePath, data, width, height);
	else if (ext.compare("tga") == 0)
		return GetTextureDataFromTGAFile(filePath, data, width, height);

	return false;
}

bool AnimaTexturesManager::GetTextureDataFromBMPFile(const AnimaString& filePath, AnimaArray<AUchar>* data, AUint& width, AUint& height)
{
	AUchar header[54];
	AUint dataPos;
	
	FILE * file = fopen(filePath.c_str(), "rb");
	if (!file)
		return false;

	if (fread(header, 1, 54, file) != 54)
		return false;

	if (header[0] != 'B' || header[1] != 'M')
		return false;

	dataPos = *(int*)&(header[0x0A]);
	AUint bufferSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (bufferSize == 0)
		bufferSize = width * height * 3;

	if (dataPos == 0)
		dataPos = 54;

	AUchar* buffer = (AUchar*)malloc(bufferSize * sizeof(AUchar));
	fread(buffer, 1, bufferSize, file);
	fclose(file);

	data->resize(bufferSize);
	data->assign(buffer, buffer + bufferSize);

	free(buffer);

	return true;
}

bool AnimaTexturesManager::GetTextureDataFromTGAFile(const AnimaString& filePath, AnimaArray<AUchar>* data, AUint& width, AUint& height)
{
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
		return GetUncompressedTGAData(file, data, width, height);
	else if (memcmp(animaCTGAcompare, &tgaHeader, sizeof(tgaHeader)) == 0)
		return GetCompressedTGAData(file, data, width, height);
	else
	{
		fclose(file);
		return false;
	}
	return false;
}

// riferimento pagina ftp://download.nvidia.com/developer/GPU_Gems/CD_Image/Image_Processing/Image_Processing_Framework/LIBS/src/nv_dds/nv_dds.cpp
bool AnimaTexturesManager::GetTextureDataFromDDSFile(const AnimaString& filePath, AnimaArray<AnimaArray<AUchar> >* data, AUint& imagesCount, AUint& width, AUint& height, AUint& depth, AUint& mipMapsCount, AnimaTextureFormat& format, AnimaTextureInternalFormat& internalFormat, AnimaTextureTarget& target)
{
	DDSFileHeader header;

	FILE * file;
	file = fopen(filePath.c_str(), "rb");
	if (file == NULL)
		return false;

	char filetype[4];
	fread(filetype, 1, 4, file);

	// Controllo sia effettivamente un file DDS
	if (strncmp(filetype, "DDS ", 4) != 0)
	{
		fclose(file);
		return false;
	}

	// Leggo la testata del file che è sempre di 124 bytes
	fread(&header, sizeof(DDSFileHeader), 1, file);

	DDSPixelFormat pixelFormat = header.ddspf;

	height = header.dwHeight;
	width = header.dwWidth;
	depth = fmax(1, header.dwDepth);
	mipMapsCount = header.dwMipMapCount;

	if (header.dwCaps2 & 0x200)
		target = TEXTURE_TARGET_CUBE;
	else if (header.dwCaps2 & 0x200000)
		target = TEXTURE_TARGET_3D;
	else
		target = TEXTURE_TARGET_2D;

	bool compressed = false;
	AUint blockSize = 16;
	AUint components = 4;

	// Controllo se nei flag c'è DDPF_FOURCC [0x4] e quindi la texture è compressa
	if (pixelFormat.dwFlags & 0x4)
	{
		compressed = true;

		AUint fourCC = pixelFormat.dwFourCC;
		switch (fourCC)
		{
		case FOURCC_DXT1:
			internalFormat = TEXTURE_INTERNAL_FORMAT_RGB8;
			format = TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			blockSize = 8;
			components = 3;
			break;
		case FOURCC_DXT3:
			internalFormat = TEXTURE_INTERNAL_FORMAT_RGBA8;
			format = TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			components = 4;
			break;
		case FOURCC_DXT5:
			internalFormat = TEXTURE_INTERNAL_FORMAT_RGBA8;
			format = TEXTURE_FORMAT_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			components = 4;
			break;
		default:
		{
			char buf[5];
			buf[0] = fourCC & 255;
			buf[1] = (fourCC >> 8) & 255;
			buf[2] = (fourCC >> 16) & 255;
			buf[3] = (fourCC >> 24) & 255;
			buf[4] = 0;

			printf("Error reading texture\n\t- File path: %s\n\t- Error: unknown compression format %s\n", filePath.c_str(), buf);

			fclose(file);
			return false;
			break;
		}
		}
	}
	else if (pixelFormat.dwRGBBitCount == 32)
	{
		internalFormat = TEXTURE_INTERNAL_FORMAT_RGBA8;
		format = TEXTURE_FORMAT_BGRA;
		components = 4;
	}
	else if (pixelFormat.dwRGBBitCount == 24)
	{
		internalFormat = TEXTURE_INTERNAL_FORMAT_RGB8;
		format = TEXTURE_FORMAT_BGR;
		components = 3;
	}
	else if (pixelFormat.dwRGBBitCount == 8)
	{
		printf("Error reading texture\n\t- File path: %s\n\t- Error: unable to load luminance texture\n", filePath.c_str());

		fclose(file);
		return false;
	}
	else
	{
		printf("Error reading texture\n\t- File path: %s\n\t- Error: unknown format\n", filePath.c_str());

		fclose(file);
		return false;
	}

	// Controllo se è una texture cube allora ha 6 immagini
	if (target == TEXTURE_TARGET_CUBE)
		imagesCount = 6;
	// altrimenti ne ha 1
	else
		imagesCount = 1;

	// Il numero di buffer è il numero di immagini per il numero di mip-map
	AUint buffersCount = imagesCount * (mipMapsCount > 0 ? mipMapsCount : 1);

	// Alloco l'array di buffer e anche l'array delle dimensioni
	data->resize(buffersCount);

	AInt numMipMaps = mipMapsCount > 0 ? mipMapsCount - 1 : 0;

	AInt offset = 0;
	for (AInt i = 0; i < imagesCount; i++)
	{
		AUint bufferSize = 0;

		// Calcolo la dimensione della prima immagine
		if (compressed)
			bufferSize = (((width + 3) / 4) * ((height + 3) / 4) * blockSize) * depth;
		else
			bufferSize = (width * height * components) * depth;

		// Leggo i dati della prima immagine (non ancora i mip)
		unsigned char *pixels = new unsigned char[bufferSize];
		fread(pixels, 1, bufferSize, file);
		
		(*data)[offset].resize(bufferSize);
		(*data)[offset].assign(pixels, pixels + bufferSize);
		
		delete[] pixels;

		offset++;

		// Divido le dimensioni per due
		AUint w = fmax(1, width >> 1);
		AUint h = fmax(1, height >> 1);
		AUint d = fmax(1, depth >> 1);

		for (AInt j = 0; j < numMipMaps && (w || h); j++)
		{
			// Calcolo la dimensione della prima immagine
			if (compressed)
				bufferSize = (((w + 3) / 4) * ((h + 3) / 4) * blockSize) * d;
			else
				bufferSize = (w * h * components) * d;

			pixels = new unsigned char[bufferSize];
			fread(pixels, 1, bufferSize, file);

			(*data)[offset].resize(bufferSize);
			(*data)[offset].assign(pixels, pixels + bufferSize);

			offset++;

			// Divido le dimensioni per due ancora
			w = fmax(1, w >> 1);
			h = fmax(1, h >> 1);
			d = fmax(1, d >> 1);
		}
	}

	fclose(file);
	return true;
}

bool AnimaTexturesManager::GetUncompressedTGAData(FILE * file, AnimaArray<AUchar>* data, AUint& width, AUint& height)
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
	AUint bufferSize = (tga.bytesPerPixel * tga.Width * tga.Height);
	AUchar* buffer = (AUchar*)malloc(bufferSize);

	if (buffer == NULL)
	{
		fclose(file);
		return false;
	}

	if (fread(buffer, 1, bufferSize, file) != bufferSize)
	{
		if (buffer != NULL)
			free(buffer);
		fclose(file);

		return false;
	}

	for (GLuint cswap = 0; cswap < (int)bufferSize && invert; cswap += tga.bytesPerPixel)
		buffer[cswap] ^= buffer[cswap + 2] ^= buffer[cswap] ^= buffer[cswap + 2];

	fclose(file);

	data->resize(bufferSize);
	data->assign(buffer, buffer + bufferSize);

	free(buffer);

	return true;
}

bool AnimaTexturesManager::GetCompressedTGAData(FILE* file, AnimaArray<AUchar>* data, AUint& width, AUint& height)
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
	AUchar* buffer = (AUchar*)malloc(tga.imageSize);

	if (buffer == NULL)
	{
		fclose(file);
		return false;
	}

	AUint pixelcount = tga.Height * tga.Width;
	AUint currentpixel = 0;
	AUint currentbyte = 0;
	AUchar* colorbuffer = (AUchar*)malloc(tga.bytesPerPixel);

	do
	{
		AUchar chunkheader = 0;

		if (fread(&chunkheader, sizeof(AUchar), 1, file) == 0)
		{
			if (file != NULL)
				fclose(file);
			
			if (buffer != NULL)
				free(buffer);

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

					if (buffer != NULL)
						free(buffer);

					return false;
				}

				buffer[currentbyte] = colorbuffer[2];
				buffer[currentbyte + 1] = colorbuffer[1];
				buffer[currentbyte + 2] = colorbuffer[0];

				if (tga.bytesPerPixel == 4)
					buffer[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);

					if (buffer != NULL)
						free(buffer);

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

				if (buffer != NULL)
					free(buffer);

				return false;
			}

			for (short counter = 0; counter < chunkheader; counter++)
			{
				buffer[currentbyte] = colorbuffer[2];
				buffer[currentbyte + 1] = colorbuffer[1];
				buffer[currentbyte + 2] = colorbuffer[0];

				if (tga.bytesPerPixel == 4)
					buffer[currentbyte + 3] = colorbuffer[3];

				currentbyte += tga.bytesPerPixel;
				currentpixel++;

				if (currentpixel > pixelcount)
				{
					if (file != NULL)
						fclose(file);

					if (colorbuffer != NULL)
						free(colorbuffer);

					if (buffer != NULL)
						free(buffer);

					return false;
				}
			}
		}
	}

	while (currentpixel < pixelcount);
	fclose(file);

	data->resize(tga.imageSize);
	data->assign(buffer, buffer + tga.imageSize);

	free(buffer);
	free(colorbuffer);

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

AnimaTexture* AnimaTexturesManager::GetTextureFromName(const AnimaString& textureName)
{
	return _textures[textureName];
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromFile(const AnimaString& filePath)
{
	std::ifstream fileStream(filePath);
	AnimaString xml((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
	fileStream.close();
	
	return LoadTextureFromXml(xml);
}

AnimaTexture* AnimaTexturesManager::LoadTextureFromXml(const AnimaString& textureXmlDefinition)
{
	boost::property_tree::ptree pt;
	std::stringstream ss(textureXmlDefinition);
	boost::property_tree::read_xml(ss, pt);
	
	return LoadTextureFromXml(pt);

}

AnimaTexture* AnimaTexturesManager::LoadTextureFromXml(const boost::property_tree::ptree& xmlTree)
{
	AnimaTexture* texture = nullptr;
	
	AnimaString name = xmlTree.get<AnimaString>("AnimaTexture.Name");
	texture = CreateTexture(name);
	
	if (texture)
	{
		texture->ReadObject(xmlTree, false);
		
//		AnimaTextureTarget target;
//		AnimaTextureFormat format;
//		AnimaTextureInternalFormat internalFormat;
//
//		try
//		{
//			target = xmlTree.get<AnimaTextureTarget>("AnimaTexture.Target");
//			format = xmlTree.get<AnimaTextureFormat>("AnimaTexture.Format");
//			internalFormat = xmlTree.get<AnimaTextureInternalFormat>("AnimaTexture.InternalFormat");
//		}
//		catch (boost::property_tree::ptree_bad_path& exception)
//		{
//			printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: %s\n", name.c_str(), exception.what());
//			return nullptr;
//		}
//		catch (boost::property_tree::ptree_bad_data& exception)
//		{
//			printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: %s\n", name.c_str(), exception.what());
//			return nullptr;
//		}
//
//		if (target == TEXTURE_TARGET_CUBE)
//		{
//			//bool singleFile = false;
//			//AnimaString filePath, filePathPX, filePathNX, filePathPY, filePathNY, filePathPZ, filePathNZ;
//
//			//try
//			//{
//			//	filePath = xmlTree.get<AnimaString>("AnimaTexture.File");
//			//	singleFile = true;
//			//}
//			//catch (boost::property_tree::ptree_bad_path& exception)
//			//{
//			//}
//			//catch (boost::property_tree::ptree_bad_data& exception)
//			//{
//			//}
//
//			//if (singleFile)
//			//{
//			//	AUchar** data;
//			//	AUint* dataSize;
//			//	AUint width;
//			//	AUint height;
//			//	AUint depth;
//			//	AUint mipMapsCount;
//			//	AUint imagesCount;
//			//	AnimaTextureFormat format;
//			//	AnimaTextureInternalFormat internalFormat;
//			//	AnimaTextureTarget target;
//
//			//	if (GetTextureDataFromDDSFile(filePath, &data, &dataSize, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
//			//	{
//			//		// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
//			//		mipMapsCount = max(mipMapsCount, 1);
//
//			//		if (format != 0 && internalFormat != 0)
//			//		{
//			//			texture->SetFormat(format);
//			//			texture->SetInternalFormat(internalFormat);
//			//		}
//
//			//		texture->SetTextureTarget(target);
//			//		texture->SetMipMapLevels(mipMapsCount);
//
//			//		// se il target è una texture cube devo comportarmi diversamente
//			//		if (target == TEXTURE_CUBE)
//			//		{
//			//			AInt offset = 0;
//			//			for (AUint i = 0; i < imagesCount; i++)
//			//			{
//			//				for (AUint j = 0; j < mipMapsCount; j++)
//			//				{
//			//					AUchar* buffer = data[offset];
//			//					AUint bufferSize = dataSize[offset];
//
//			//					texture->SetData(buffer, bufferSize, (AnimaTextureCubeIndex)i, j);
//
//			//					free(data[offset]);
//
//			//					offset++;
//			//				}
//			//			}
//			//		}
//			//		else
//			//		{
//			//			AInt offset = 0;
//			//			for (AUint i = 0; i < imagesCount; i++)
//			//			{
//			//				for (AUint j = 0; j < mipMapsCount; j++)
//			//				{
//			//					AUchar* buffer = data[offset];
//			//					AUint bufferSize = dataSize[offset];
//
//			//					texture->SetData(buffer, bufferSize, j);
//
//			//					free(data[offset]);
//
//			//					offset++;
//			//				}
//			//			}
//			//		}
//
//			//		delete[] * data;
//			//		delete[] dataSize;
//			//	}
//			//	else
//			//	{
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n", name.c_str());
//			//		return nullptr;
//			//	}
//			//}
//			//else
//			//{
//			//	try
//			//	{
//			//		filePathPX = xmlTree.get<AnimaString>("AnimaTexture.FilePX");
//			//		filePathNX = xmlTree.get<AnimaString>("AnimaTexture.FileNX");
//			//		filePathPY = xmlTree.get<AnimaString>("AnimaTexture.FilePY");
//			//		filePathNY = xmlTree.get<AnimaString>("AnimaTexture.FileNY");
//			//		filePathPZ = xmlTree.get<AnimaString>("AnimaTexture.FilePZ");
//			//		filePathNZ = xmlTree.get<AnimaString>("AnimaTexture.FileNZ");
//			//	}
//			//	catch (boost::property_tree::ptree_bad_path& exception)
//			//	{
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: %s\n", name.c_str(), exception.what());
//			//		return nullptr;
//			//	}
//			//	catch (boost::property_tree::ptree_bad_data& exception)
//			//	{
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: %s\n", name.c_str(), exception.what());
//			//		return nullptr;
//			//	}
//
//			//	AUint width, height, depth, mipMapsCount;
//			//	AnimaTextureFormat guessedFormat;
//			//	AnimaTextureInternalFormat guessedInternalFormat;
//			//	AUchar *dataPX = nullptr, *dataNX = nullptr, *dataPY = nullptr, *dataNY = nullptr, *dataPZ = nullptr, *dataNZ = nullptr;
//			//	AUint dataSizePX, dataSizeNX, dataSizePY, dataSizeNY, dataSizePZ, dataSizeNZ;
//
//			//	GetTextureDataFromFile(filePathPX, &dataPX, dataSizePX, width, height, depth, mipMapsCount, guessedFormat, guessedInternalFormat);
//			//	if (dataPX == nullptr || dataSizePX <= 0)
//			//	{
//			//		free(dataPX);
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: unable to read the file '%s'\n", name.c_str(), filePathPX.c_str());
//			//		return nullptr;
//			//	}
//
//			//	GetTextureDataFromFile(filePathNX, &dataNX, dataSizeNX, width, height, depth, mipMapsCount, guessedFormat, guessedInternalFormat);
//			//	if (dataNX == nullptr || dataSizeNX <= 0)
//			//	{
//			//		free(dataPX);
//			//		free(dataNX);
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: unable to read the file '%s'\n", name.c_str(), filePathNX.c_str());
//			//		return nullptr;
//			//	}
//
//			//	GetTextureDataFromFile(filePathPY, &dataPY, dataSizePY, width, height, depth, mipMapsCount, guessedFormat, guessedInternalFormat);
//			//	if (dataPY == nullptr || dataSizePY <= 0)
//			//	{
//			//		free(dataPX);
//			//		free(dataNX);
//			//		free(dataPY);
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: unable to read the file '%s'\n", name.c_str(), filePathPY.c_str());
//			//		return nullptr;
//			//	}
//
//			//	GetTextureDataFromFile(filePathNY, &dataNY, dataSizeNY, width, height, depth, mipMapsCount, guessedFormat, guessedInternalFormat);
//			//	if (dataNY == nullptr || dataSizeNY <= 0)
//			//	{
//			//		free(dataPX);
//			//		free(dataNX);
//			//		free(dataPY);
//			//		free(dataNY);
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: unable to read the file '%s'\n", name.c_str(), filePathNY.c_str());
//			//		return nullptr;
//			//	}
//
//			//	GetTextureDataFromFile(filePathPZ, &dataPZ, dataSizePZ, width, height, depth, mipMapsCount, guessedFormat, guessedInternalFormat);
//			//	if (dataPZ == nullptr || dataSizePZ <= 0)
//			//	{
//			//		free(dataPX);
//			//		free(dataNX);
//			//		free(dataPY);
//			//		free(dataNY);
//			//		free(dataPZ);
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: unable to read the file '%s'\n", name.c_str(), filePathPZ.c_str());
//			//		return nullptr;
//			//	}
//
//			//	GetTextureDataFromFile(filePathNZ, &dataNZ, dataSizeNZ, width, height, depth, mipMapsCount, guessedFormat, guessedInternalFormat);
//			//	if (dataNZ == nullptr || dataSizeNZ <= 0)
//			//	{
//			//		free(dataPX);
//			//		free(dataNX);
//			//		free(dataPY);
//			//		free(dataNY);
//			//		free(dataPZ);
//			//		free(dataNZ);
//			//		printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: unable to read the file '%s'\n", name.c_str(), filePathNZ.c_str());
//			//		return nullptr;
//			//	}
//
//			//	if (guessedFormat == FORMAT_NONE || guessedFormat == format)
//			//		texture->SetFormat(format);
//			//	else
//			//		texture->SetFormat(guessedFormat);
//
//			//	texture->SetInternalFormat(internalFormat);
//			//	texture->SetDataType(GL_UNSIGNED_BYTE);
//			//	texture->SetWidth(width);
//			//	texture->SetHeight(height);
//			//	texture->SetFilter(LINEAR);
//			//	texture->SetClamp(TO_EDGE);
//			//	texture->SetTextureTarget(target);
//
//			//	texture->SetData(dataPX, dataSizePX, POSITIVE_X, 0);
//			//	texture->SetData(dataNX, dataSizeNX, NEGATIVE_X, 0);
//			//	texture->SetData(dataPY, dataSizePY, POSITIVE_Y, 0);
//			//	texture->SetData(dataNY, dataSizeNY, NEGATIVE_Y, 0);
//			//	texture->SetData(dataPZ, dataSizePZ, POSITIVE_Z, 0);
//			//	texture->SetData(dataNZ, dataSizeNZ, NEGATIVE_Z, 0);
//			//}
//		}
//		else
//		{
//			AnimaString filePath;
//
//			try
//			{
//				filePath = xmlTree.get<AnimaString>("AnimaTexture.File");
//			}
//			catch (boost::property_tree::ptree_bad_path& exception)
//			{
//				printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: %s\n", name.c_str(), exception.what());
//				return nullptr;
//			}
//			catch (boost::property_tree::ptree_bad_data& exception)
//			{
//				printf("[AnimaTexturesManager] Error reading AnimaTexture data:\n\t- Texture name '%s'\n\t- Error: %s\n", name.c_str(), exception.what());
//				return nullptr;
//			}
//
//			AnimaArray<AnimaArray<AUchar> > data;
//			AUint width;
//			AUint height;
//			AUint depth;
//			AUint mipMapsCount;
//			AUint imagesCount;
//			AnimaTextureFormat format;
//			AnimaTextureInternalFormat internalFormat;
//			AnimaTextureTarget target;
//
//			if (GetTextureDataFromDDSFile(filePath, &data, imagesCount, width, height, depth, mipMapsCount, format, internalFormat, target))
//			{
//				texture->SetWidth(width);
//				texture->SetHeight(height);
//				texture->SetDepth(depth);
//				
//				// Forzo ad avere come minimo un livello di mip-map per evitare condizioni dopo sui cicli
//				mipMapsCount = fmax(mipMapsCount, 1);
//
//				if (format != 0 && internalFormat != 0)
//				{
//					texture->SetFormat(format);
//					texture->SetInternalFormat(internalFormat);
//				}
//
//				texture->SetTextureTarget(target);
//				texture->SetMipMapLevels(mipMapsCount);
//
//				Anima::AInt offset = 0;
//				for (Anima::AUint i = 0; i < imagesCount; i++)
//				{
//					for (Anima::AUint j = 0; j < mipMapsCount; j++)
//					{
//						Anima::AUchar* buffer = &data[offset][0];
//						Anima::AUint bufferSize = data[offset].size();
//
//						texture->SetData(buffer, bufferSize, j);
//
//						offset++;
//					}
//				}
//
//				texture->SetMinFilter(TEXTURE_MIN_FILTER_MODE_LINEAR);
//				texture->SetMagFilter(TEXTURE_MAG_FILTER_MODE_LINEAR);
//				texture->SetClampR(TEXTURE_CLAMP_REPEAT);
//				texture->SetClampS(TEXTURE_CLAMP_REPEAT);
//				texture->SetClampT(TEXTURE_CLAMP_REPEAT);
//				texture->SetDataType(TEXTURE_DATA_TYPE_UNSIGNED_BYTE);
//			}
//			texture->SetSourceFileName(filePath);
//		}
	}
	
	return texture;
}

bool AnimaTexturesManager::LoadTextures(const AnimaString& texturesPath)
{
	namespace fs = boost::filesystem;
	fs::path directory(texturesPath);
	
	bool returnValue = true;
	
	if (fs::exists(directory) && fs::is_directory(directory))
	{
		fs::directory_iterator endIterator;
		for (fs::directory_iterator directoryIterator(directory); directoryIterator != endIterator; directoryIterator++)
		{
			if (directoryIterator->path().extension().string() == ".atexture")
			{
				if (LoadTextureFromFile(directoryIterator->path().string().c_str()) == nullptr)
					returnValue = false;
			}
		}
	}
	
	return returnValue;
}

void AnimaTexturesManager::SaveTextureToFile(const AnimaString& textureName, const AnimaString& destinationPath, bool createFinalPath)
{
	return SaveTextureToFile(GetTextureFromName(textureName), destinationPath, createFinalPath);
}

void AnimaTexturesManager::SaveTextureToFile(AnimaTexture* texture, const AnimaString& destinationPath, bool createFinalPath)
{
	if(texture == nullptr)
		return;
	
	namespace fs = boost::filesystem;
	
	AnimaString saveFileName = destinationPath;
	if(createFinalPath)
	{
		fs::path firstPart(destinationPath);
		fs::path secondPart(texture->GetName() + ".atexture");
		fs::path completePath = firstPart / secondPart;
		
		saveFileName = completePath.string();
	}
	
	texture->SaveObject(saveFileName);
	
//	AnimaString saveDirectory = fs::path(saveFileName).parent_path().string();
//	
//	using ptree = boost::property_tree::ptree;
//	ptree textureTree;
//	
//	textureTree.add("AnimaTexture.Name", texture->GetName());
//	textureTree.add("AnimaTexture.Target", texture->GetTextureTarget());
//	textureTree.add("AnimaTexture.Format", texture->GetFormat());
//	textureTree.add("AnimaTexture.InternalFormat", texture->GetInternalFormat());
//	textureTree.add("AnimaTexture.MinFilter", texture->GetMinFilter());
//	textureTree.add("AnimaTexture.MagFilter", texture->GetMagFilter());
//	textureTree.add("AnimaTexture.Attachment", texture->GetAttachment());
//	textureTree.add("AnimaTexture.DataType", texture->GetDataType());
//	textureTree.add("AnimaTexture.ClampS", texture->GetClampS());
//	textureTree.add("AnimaTexture.ClampT", texture->GetClampT());
//	textureTree.add("AnimaTexture.ClampR", texture->GetClampR());
//	textureTree.add("AnimaTexture.MipMapLevels", texture->GetMipMapLevels());
//	textureTree.add("AnimaTexture.BorderColor", texture->GetBorderColor());
//	textureTree.add("AnimaTexture.GenerateMipMaps", texture->IsGenerateMipMap());
//	
//	AnimaString textureSourceFileName = texture->GetSourceFileName();
//	if(textureSourceFileName.empty() || texture->GetTextureTarget() == TEXTURE_TARGET_CUBE)
//	{
//		AUint mipMapLevels = fmax(texture->GetMipMapLevels(), 1);
//		
//		if(texture->GetTextureTarget() == TEXTURE_TARGET_CUBE)
//		{
//			ptree cubeTree;
//			for(AInt cubeIndex = 0; cubeIndex < 6; cubeIndex++)
//			{
//				AnimaTextureCubeIndex textureCubeIndex = (AnimaTextureCubeIndex)cubeIndex;
//				ptree cubeFaceTree;
//				cubeFaceTree.add("Index", textureCubeIndex);
//				
//				ptree surfacesTree;
//				
//				for(AInt mipIndex = 0; mipIndex < mipMapLevels; mipIndex++)
//				{
//					AnimaArray<AUchar>* data = texture->GetDataAsArray(textureCubeIndex, mipIndex);
//					
//					if(data != nullptr)
//					{
//						AnimaString base64Data = AnimaTools::Base64Encode(*data);
//						surfacesTree.add("Surface.Data", base64Data);
//					}
//					else
//					{
//						surfacesTree.add("Surface.Data", "");
//					}
//				}
//				cubeFaceTree.add_child("Sufaces", surfacesTree);
//				
//				cubeTree.add_child("CubeFace", cubeFaceTree);
//			}
//			textureTree.add_child("AnimaTextures.CubeFaces", cubeTree);
//		}
//		else
//		{
//			ptree surfacesTree;
//			
//			for(AInt mipIndex = 0; mipIndex < mipMapLevels; mipIndex++)
//			{
//				AnimaArray<AUchar>* data = texture->GetDataAsArray(mipIndex);
//				
//				if(data != nullptr)
//				{
//					AnimaString base64Data = AnimaTools::Base64Encode(*data);
//					surfacesTree.add("Surface.Data", base64Data);
//				}
//				else
//				{
//					surfacesTree.add("Surface.Data", "");
//				}
//			}
//			
//			textureTree.add_child("AnimaTexture.Surfaces", surfacesTree);
//		}
//	}
//	else
//	{
//		fs::path sourcePath(textureSourceFileName);
//		AnimaString destinationFileName = (fs::path(saveDirectory) / sourcePath.filename()).string();
//
//		fs::copy_file(fs::path(textureSourceFileName), fs::path(destinationFileName), fs::copy_option::overwrite_if_exists);
//		textureTree.add("AnimaTexture.FileName", destinationFileName);
//	}
//	
//	boost::property_tree::write_xml(saveFileName, textureTree, std::locale(), boost::property_tree::xml_writer_make_settings<ptree::key_type>('\t', 1));
}

void AnimaTexturesManager::SaveTextures(const AnimaString& destinationPath)
{
	AInt count = _textures.GetSize();
	for (AInt i = 0; i < count; i++)
	{
		SaveTextureToFile(_textures[i], destinationPath, true);
	}
}

END_ANIMA_ENGINE_NAMESPACE