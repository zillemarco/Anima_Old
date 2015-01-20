#include "AnimaTexture.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaTexture::AnimaTexture(AnimaEngine* engine)
{
	_engine = engine;
	_id = 0;
	_width = 0;
	_height = 0;
	_mipMapLevels = 0;
	_format = 0;
	_isLoaded = false;

	_data = nullptr;
	_dataSize = 0;
}

AnimaTexture::AnimaTexture(AnimaEngine* engine, AUchar* data, ASizeT dataSize, AUint width, AUint height, AUint mipMapLevels, AUint format)
{
	_engine = engine;
	_id = 0;
	_width = width;
	_height = height;
	_mipMapLevels = mipMapLevels;
	_format = format;
	_isLoaded = false;

	_dataSize = dataSize;
	_data = (AUchar*)(_engine->GetGenericAllocator())->Allocate(sizeof(AUchar) * _dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));

	SetData(data, dataSize);
}

AnimaTexture::AnimaTexture(const AnimaTexture& src)
{
	_engine = src._engine;
	_id = src._id;
	_width = src._width;
	_height = src._height;
	_mipMapLevels = src._mipMapLevels;
	_format = src._format;
	_isLoaded = src._isLoaded;

	_dataSize = src._dataSize;
	_data = (AUchar*)(_engine->GetGenericAllocator())->Allocate(sizeof(AUchar) * _dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));

	SetData(src._data, src._dataSize);
}

AnimaTexture::AnimaTexture(AnimaTexture&& src)
{
	_engine = src._engine;
	_id = src._id;
	_width = src._width;
	_height = src._height;
	_mipMapLevels = src._mipMapLevels;
	_format = src._format;
	_isLoaded = src._isLoaded;

	_dataSize = src._dataSize;
	_data = (AUchar*)(_engine->GetGenericAllocator())->Allocate(sizeof(AUchar) * _dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));

	SetData(src._data, src._dataSize);
}

AnimaTexture::~AnimaTexture()
{
	Unload();

	if (_data != nullptr)
	{
		(_engine->GetGenericAllocator())->Deallocate(_data);
		_data = nullptr;

		_dataSize = 0;
	}
}

AnimaTexture& AnimaTexture::operator=(const AnimaTexture& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_id = src._id;
		_width = src._width;
		_height = src._height;
		_mipMapLevels = src._mipMapLevels;
		_format = src._format;
		_isLoaded = src._isLoaded;

		_dataSize = src._dataSize;
		_data = (AUchar*)(_engine->GetGenericAllocator())->Allocate(sizeof(AUchar) * _dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));

		SetData(src._data, src._dataSize);
	}

	return *this;
}

AnimaTexture& AnimaTexture::operator=(AnimaTexture&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_id = src._id;
		_width = src._width;
		_height = src._height;
		_mipMapLevels = src._mipMapLevels;
		_format = src._format;
		_isLoaded = src._isLoaded;

		_dataSize = src._dataSize;
		_data = (AUchar*)(_engine->GetGenericAllocator())->Allocate(sizeof(AUchar) * _dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));

		SetData(src._data, src._dataSize);
	}

	return *this;
}

AUint AnimaTexture::GetID() const
{
	return _id;
}

void AnimaTexture::SetWidth(AUint width)
{
	_width = width;
}

AUint AnimaTexture::GetWidth() const
{
	return _width;
}

void AnimaTexture::SetHeight(AUint height)
{
	_height = height;
}

AUint AnimaTexture::GetHeight() const
{
	return _height;
}

void AnimaTexture::SetMipMapLevels(AUint levels)
{
	_mipMapLevels = levels;
}

AUint AnimaTexture::GetMipMapLevels() const
{
	return _mipMapLevels;
}

void AnimaTexture::SetFormat(AUint format)
{
	_format = format;
}

AUint AnimaTexture::GetFormat() const
{
	return _format;
}

void AnimaTexture::SetData(AUchar* data, ASizeT dataSize)
{
	if (_data != nullptr && _dataSize != dataSize)
	{
		_dataSize = dataSize;

		(_engine->GetGenericAllocator())->Deallocate(_data);
		_data = nullptr;

		_data = (AUchar*)(_engine->GetGenericAllocator())->Allocate(sizeof(AUchar) * _dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));
	}

	memcpy(_data, data, sizeof(AUchar) * dataSize);
}

bool AnimaTexture::Load()
{
	if (IsLoaded())
		return true;

	glGenTextures(1, &_id);

	if (glGetError() != GL_NO_ERROR)
		return false;

	glBindTexture(GL_TEXTURE_2D, _id);

	if (_mipMapLevels == 0)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, _data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (_mipMapLevels > 0)
	{
		unsigned int BlockSize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		int width = _width;
		int height = _height;

		for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
		{
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

			glCompressedTexImage2D(GL_TEXTURE_2D, level, _format, width, height, 0, size, _data + offset);

			offset += size;
			width /= 2;
			height /= 2;
		}
	}
	else
		return false;

	if (glGetError() != GL_NO_ERROR)
		return false;

	_isLoaded = true;
	return true;
}

bool AnimaTexture::IsLoaded()
{
	return _isLoaded;
}

void AnimaTexture::Unload()
{
	if (!IsLoaded())
		return;

	glDeleteTextures(1, &_id);
}

void AnimaTexture::Bind()
{
	if (IsLoaded())
		glBindTexture(GL_TEXTURE_2D, _id);
}

END_ANIMA_ENGINE_NAMESPACE