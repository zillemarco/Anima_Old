#include "AnimaTexture.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AFloat Clamp(const AFloat& value, const AFloat& low, const AFloat& high)
{
	return value < low ? low : (value > high ? high : value);
}

AnimaTexture::AnimaTexture(AnimaAllocator* allocator)
{
	_allocator = allocator;

	_mipMapLevels = 0;
	_width = 0;
	_height = 0;
	_textureTarget = 0;	
	_textureID = 0;
	_data = nullptr;
	_dataSize = 0;
	_filter = 0;
	_internalFormat = 0;
	_format = 0;
	_dataType = 0;
	_clamp = 0;
	
	_texturesReady = false;
	_needsResize = false;
}

AnimaTexture::AnimaTexture(AnimaAllocator* allocator, AUint textureTarget, AUint width, AUint height, AUchar* data, ASizeT dataSize, AUint mipMapLevels, AUint filter, AUint internalFormat, AUint format, AUint dataType, AUint clamp, AUint attachment)
{
	_allocator = allocator;

	_data = nullptr;
	_dataSize = 0;

	SetData(data, dataSize);
	SetFilter(filter);
	SetFormat(format);
	SetInternalFormat(internalFormat);
	SetDataType(dataType);
	SetClamp(clamp);
	SetAttachment(attachment);

	_textureID = 0;
	
	_mipMapLevels = mipMapLevels;
	_width = width;
	_height = height;
	_textureTarget = textureTarget;
	_texturesReady = false;
	_needsResize = false;
}

AnimaTexture::AnimaTexture(const AnimaTexture& src)
{
	_allocator = src._allocator;

	_data = nullptr;
	_dataSize = 0;

	SetData(src._data, src._dataSize);
	SetFilter(src._filter);
	SetFormat(src._format);
	SetInternalFormat(src._internalFormat);
	SetDataType(src._dataType);
	SetClamp(src._clamp);
	SetAttachment(src._attachment);
	
	_textureID = 0;
	_mipMapLevels = src._mipMapLevels;
	_textureTarget = src._textureTarget;
	_width = src._width;
	_height = src._height;
	_texturesReady = false;
	_needsResize = false;
}

AnimaTexture::AnimaTexture(AnimaTexture&& src)
{
	_allocator = src._allocator;

	_data = nullptr;
	_dataSize = 0;

	SetData(src._data, src._dataSize);
	SetFilter(src._filter);
	SetFormat(src._format);
	SetInternalFormat(src._internalFormat);
	SetDataType(src._dataType);
	SetClamp(src._clamp);
	SetAttachment(src._attachment);

	_textureID = 0;
	_mipMapLevels = src._mipMapLevels;
	_textureTarget = src._textureTarget;
	_width = src._width;
	_height = src._height;
	_texturesReady = false;
	_needsResize = false;
}

AnimaTexture::~AnimaTexture()
{
	Unload();

	if (_data != nullptr)
	{
		_allocator->Deallocate(_data);
		_data = nullptr;
	}

	_dataSize = 0;
	_filter = 0;
	_format = 0;
	_internalFormat = 0;
	_dataType = 0;
	_clamp = 0;
}

AnimaTexture& AnimaTexture::operator=(const AnimaTexture& src)
{
	if (this != &src)
	{
		Unload();

		if (_data != nullptr)
		{
			_allocator->Deallocate(_data);
			_data = nullptr;
			_dataSize = 0;
		}

		_allocator = src._allocator;

		SetData(src._data, src._dataSize);
		SetFilter(src._filter);
		SetFormat(src._format);
		SetInternalFormat(src._internalFormat);
		SetDataType(src._dataType);
		SetClamp(src._clamp);
		SetAttachment(src._attachment);
		
		_textureID = 0;
		_mipMapLevels = src._mipMapLevels;
		_textureTarget = src._textureTarget;
		_width = src._width;
		_height = src._height;
		_texturesReady = false;
		_needsResize = false;
	}
	return *this;
}

AnimaTexture& AnimaTexture::operator=(AnimaTexture&& src)
{
	if (this != &src)
	{
		Unload();

		if (_data != nullptr)
		{
			_allocator->Deallocate(_data);
			_data = nullptr;
			_dataSize = 0;
		}

		_allocator = src._allocator;

		SetData(src._data, src._dataSize);
		SetFilter(src._filter);
		SetFormat(src._format);
		SetInternalFormat(src._internalFormat);
		SetDataType(src._dataType);
		SetClamp(src._clamp);
		SetAttachment(src._attachment);

		_textureID = 0;
		_mipMapLevels = src._mipMapLevels;
		_textureTarget = src._textureTarget;
		_width = src._width;
		_height = src._height;
		_texturesReady = false;
		_needsResize = false;
	}
	return *this;
}

AUint AnimaTexture::GetID(AUint index) const
{
	return _textureID;
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

void AnimaTexture::SetFilter(AUint filter)
{
	_filter = filter;
}

AUint AnimaTexture::GetFilter() const
{
	return _filter;
}

void AnimaTexture::SetAttachment(AUint attachment)
{
	_attachment = attachment;
}

AUint AnimaTexture::GetAttachment() const
{
	return _attachment;
}

void AnimaTexture::SetInternalFormat(AUint internalFormat)
{
	_internalFormat = internalFormat;
}

AUint AnimaTexture::GetInternalFormat() const
{
	return _internalFormat;
}

void AnimaTexture::SetDataType(AUint dataType)
{
	_dataType = dataType;
}

AUint AnimaTexture::GetDataType() const
{
	return _dataType;
}

void AnimaTexture::SetData(AUchar* data, ASizeT dataSize)
{
	if (data != nullptr && dataSize > 0)
	{
		if (_data != nullptr && _dataSize != dataSize)
		{
			_allocator->Deallocate(_data);

			_data = (AUchar*)(_allocator)->Allocate(sizeof(AUchar) * dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));
			_dataSize = dataSize;
		}
		else if (_data == nullptr)
		{
			_data = (AUchar*)(_allocator)->Allocate(sizeof(AUchar) * dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));
			_dataSize = dataSize;
		}

		memcpy(_data, data, sizeof(AUchar) * dataSize);
	}
	else
	{
		if (_data != nullptr)
		{
			_allocator->Deallocate(_data);
			_data = nullptr;
		}
		_dataSize = 0;
	}
}

AUchar* AnimaTexture::GetData() const
{
	return _data;
}

const AUchar* AnimaTexture::GetConstData() const
{
	return const_cast<AUchar*>(_data);
}

void AnimaTexture::SetTextureTarget(AUint target)
{
	ANIMA_ASSERT(!IsReady());
	_textureTarget = target;
}

AUint AnimaTexture::GetTextureTarget() const
{
	return _textureTarget;
}

void AnimaTexture::SetClamp(AUint clamp)
{
	_clamp = clamp;
}

AUint AnimaTexture::GetClamp() const
{
	return _clamp;
}

bool AnimaTexture::Load()
{
	if (_texturesReady)
	{
		if (_needsResize)
		{
			glBindTexture(_textureTarget, _textureID);

			if (_mipMapLevels == 0)
				glTexImage2D(_textureTarget, 0, _internalFormat, _width, _height, 0, _format, _dataType, _data);
			else if (_mipMapLevels > 0)
			{
				unsigned int BlockSize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
				unsigned int offset = 0;

				AUint width = _width;
				AUint height = _height;

				for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
				{
					unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

					glCompressedTexImage2D(_textureTarget, level, _format, width, height, 0, size, _data + offset);

					offset += size;
					width /= 2;
					height /= 2;
				}
			}
			else
				return false;
			
			if (glGetError() != GL_NO_ERROR)
				return false;

			_needsResize = false;
		}

		return true;
	}

	glGenTextures(1, &_textureID);

	glBindTexture(_textureTarget, _textureID);

	glTexParameteri(_textureTarget, GL_TEXTURE_MAG_FILTER, _filter);
	glTexParameteri(_textureTarget, GL_TEXTURE_MIN_FILTER, _filter);
		
	glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_S, _clamp);
	glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_T, _clamp);
		
	if (_mipMapLevels == 0)
		glTexImage2D(_textureTarget, 0, _internalFormat, _width, _height, 0, _format, _dataType, _data);
	else if (_mipMapLevels > 0)
	{
		unsigned int BlockSize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
		unsigned int offset = 0;

		AUint width = _width;
		AUint height = _height;

		for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
		{
			unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

			glCompressedTexImage2D(_textureTarget, level, _format, width, height, 0, size, _data + offset);

			offset += size;
			width /= 2;
			height /= 2;
		}
	}
	else
		return false;

	if (_filter == GL_NEAREST_MIPMAP_NEAREST || _filter == GL_NEAREST_MIPMAP_LINEAR || _filter == GL_LINEAR_MIPMAP_NEAREST || _filter == GL_LINEAR_MIPMAP_LINEAR)
	{
		glGenerateMipmap(_textureTarget);
		AFloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, Clamp(0.0f, 8.0f, maxAnisotropy));
	}
	else
	{ 
		glTexParameteri(_textureTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
	}

	if (glGetError() != GL_NO_ERROR)
		return false;

	glBindTexture(_textureTarget, 0);

	_texturesReady = true;
	return true;
}

bool AnimaTexture::LoadRenderTargets()
{
	if (_renderTargetsReady && _texturesReady)
		return Load();
	
	if(_attachment == GL_NONE)
		return false;
	
	AUint drawBuffer;
	bool hasDepth = false;
	
	ANIMA_ASSERT(Load());
	
	if (_attachment == GL_DEPTH_ATTACHMENT)
	{
		drawBuffer = GL_NONE;
		hasDepth = true;
	}
	else
		drawBuffer = _attachment;
	
	if (_frameBuffer == 0)
	{
		glGenFramebuffers(1, &_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	}
		
	glFramebufferTexture2D(GL_FRAMEBUFFER, _attachment, _textureTarget, _textureID, 0);
	
	if (_frameBuffer == 0)
		return false;
	
	if (!hasDepth)
	{
		glGenRenderbuffers(1, &_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBuffer);
	}
	
	glDrawBuffers(1, &drawBuffer);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ANIMA_ASSERT(false);
		_renderTargetsReady = false;
		return false;
	}
	
	_renderTargetsReady = true;
	return true;
}

bool AnimaTexture::IsReady()
{
	return _texturesReady;
}

void AnimaTexture::Unload()
{
	if (!IsReady())
		return;

	if(_textureID != 0) 
		glDeleteTextures(1, &_textureID);
		
	_texturesReady = false;
}

void AnimaTexture::Resize(AUint width, AUint height)
{
	if (width != _width || height != _height)
	{
		_width = width;
		_height = height;
		_needsResize = true;
	}
}

void AnimaTexture::Bind(AUint unit) const
{
	ANIMA_ASSERT(_texturesReady);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(_textureTarget, _textureID);
}

void AnimaTexture::BindAsRenderTarget() const
{
	ANIMA_ASSERT(_renderTargetsReady);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glViewport(0, 0, _width, _height);
}

END_ANIMA_ENGINE_NAMESPACE