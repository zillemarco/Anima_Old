#include "AnimaTexture.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AFloat Clamp(const AFloat& value, const AFloat& low, const AFloat& high)
{
	return value < low ? low : (value > high ? high : value);
}

AnimaTexture::AnimaTexture(AnimaAllocator* allocator, AUint texturesNumber)
{
	_allocator = allocator;

	_texturesNumber = texturesNumber;

	_textureIDs = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);
	_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);
	_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

	for (AUint i = 0; i < _texturesNumber; i++)
		_datas[i] = nullptr;

	_frameBuffer = 0;
	_renderBuffer = 0;
	
	_mipMapLevels = 0;
	_enableClamp = false;
	_width = 1;
	_height = 1;
	_textureTarget = 0;
	_texturesReady = false;
	_renderTargetsReady = false;
}

AnimaTexture::AnimaTexture(AnimaAllocator* allocator, AUint textureTarget, AUint width, AUint height, AUint texturesNumber, AUchar** data, ASizeT* dataSize, AUint mipMapLevels, AUint* filters, AUint* internalFormats, AUint* formats, bool clamp, AUint* attachments)
{
	_allocator = allocator;

	_texturesNumber = texturesNumber;

	_textureIDs = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);
	_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);
	_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

	for (AUint i = 0; i < _texturesNumber; i++)
		_datas[i] = nullptr;

	SetDatas(data, dataSize);
	SetFilters(filters);
	SetFormats(formats);
	SetInternalFormats(internalFormats);
	SetAttachments(attachments);

	_frameBuffer = 0;
	_renderBuffer = 0;

	_mipMapLevels = mipMapLevels;
	_enableClamp = clamp;
	_width = width;
	_height = height;
	_textureTarget = textureTarget;
	_texturesReady = false;
	_renderTargetsReady = false;
}

AnimaTexture::AnimaTexture(const AnimaTexture& src)
{
	_allocator = src._allocator;

	_texturesNumber = src._texturesNumber;

	_textureIDs = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);
	_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);
	_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

	for (AUint i = 0; i < _texturesNumber; i++)
		_datas[i] = nullptr;

	SetDatas(src._datas, src._datasSize);
	SetFilters(src._filters);
	SetFormats(src._formats);
	SetInternalFormats(src._internalFormats);
	SetAttachments(src._attachments);

	_frameBuffer = 0;
	_renderBuffer = 0;
	
	_mipMapLevels = src._mipMapLevels;
	_textureTarget = src._textureTarget;
	_enableClamp = src._enableClamp;
	_width = src._width;
	_height = src._height;
	_texturesReady = false;
	_renderTargetsReady = false;
}

AnimaTexture::AnimaTexture(AnimaTexture&& src)
{
	_allocator = src._allocator;

	_texturesNumber = src._texturesNumber;

	_textureIDs = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);
	_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);
	_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
	_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

	for (AUint i = 0; i < _texturesNumber; i++)
		_datas[i] = nullptr;

	SetDatas(src._datas, src._datasSize);
	SetFilters(src._filters);
	SetFormats(src._formats);
	SetInternalFormats(src._internalFormats);
	SetAttachments(src._attachments);

	_frameBuffer = 0;
	_renderBuffer = 0;
	
	_mipMapLevels = src._mipMapLevels;
	_textureTarget = src._textureTarget;
	_enableClamp = src._enableClamp;
	_width = src._width;
	_height = src._height;
	_texturesReady = false;
	_renderTargetsReady = false;
}

AnimaTexture::~AnimaTexture()
{
	Unload();

	for (AUint i = 0; i < _texturesNumber; i++)
	{
		if (_datas[i] != nullptr)
		{
			_allocator->Deallocate(_datas[i]);
			_datas[i] = nullptr;
		}
	}

	if (_textureIDs != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureIDs);
	if (_datas != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datas);
	if (_datasSize != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datasSize);
	if (_filters != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _filters);
	if (_formats != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _formats);
	if (_internalFormats != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _internalFormats);
	if (_attachments != nullptr)
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _attachments);

	_datas = nullptr;
	_datasSize = nullptr;
	_filters = nullptr;
	_formats = nullptr;
	_internalFormats = nullptr;
	_attachments = nullptr;
}

AnimaTexture& AnimaTexture::operator=(const AnimaTexture& src)
{
	if (this != &src)
	{
		Unload();

		if (_texturesNumber != src._texturesNumber)
		{
			for (AUint i = 0; i < _texturesNumber; i++)
			{
				if (_datas[i] != nullptr)
				{
					_allocator->Deallocate(_datas[i]);
					_datas[i] = nullptr;
				}
			}

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datas);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datasSize);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _filters);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _formats);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _internalFormats);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _attachments);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureIDs);
			
			_texturesNumber = src._texturesNumber;

			_textureIDs = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);
			_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);
			_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

			for (AUint i = 0; i < _texturesNumber; i++)
				_datas[i] = nullptr;
		}

		SetDatas(src._datas, src._datasSize);
		SetFilters(src._filters);
		SetFormats(src._formats);
		SetInternalFormats(src._internalFormats);
		SetAttachments(src._attachments);

		_mipMapLevels = src._mipMapLevels;
		_enableClamp = src._enableClamp;
		_textureTarget = src._textureTarget;
		_width = src._width;
		_height = src._height;
		_texturesReady = false;
		_renderTargetsReady = false;
	}
	return *this;
}

AnimaTexture& AnimaTexture::operator=(AnimaTexture&& src)
{
	if (this != &src)
	{
		Unload();

		if (_texturesNumber != src._texturesNumber)
		{
			for (AUint i = 0; i < _texturesNumber; i++)
			{
				if (_datas[i] != nullptr)
				{
					_allocator->Deallocate(_datas[i]);
					_datas[i] = nullptr;
				}
			}

			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datas);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datasSize);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _filters);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _formats);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _internalFormats);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _attachments);
			AnimaAllocatorNamespace::DeallocateArray(*_allocator, _textureIDs);

			_texturesNumber = src._texturesNumber;

			_textureIDs = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);
			_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);
			_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);
			_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

			for (AUint i = 0; i < _texturesNumber; i++)
				_datas[i] = nullptr;
		}

		SetDatas(src._datas, src._datasSize);
		SetFilters(src._filters);
		SetFormats(src._formats);
		SetInternalFormats(src._internalFormats);
		SetAttachments(src._attachments);

		_mipMapLevels = src._mipMapLevels;
		_enableClamp = src._enableClamp;
		_textureTarget = src._textureTarget;
		_width = src._width;
		_height = src._height;
		_texturesReady = false;
		_renderTargetsReady = false;
	}
	return *this;
}

AUint AnimaTexture::GetID(AUint index) const
{
	return _textureIDs[index];
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

void AnimaTexture::SetFormats(AUint* formats)
{
	if (formats != nullptr)
	{
		if (_formats == nullptr)
			_formats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

		for (AUint i = 0; i < _texturesNumber; i++)
			_formats[i] = formats[i];
	}
	else
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _formats);
		_formats = nullptr;
	}
}

void AnimaTexture::SetFormat(AUint format, AUint index)
{
	_formats[index] = format;
}

AUint AnimaTexture::GetFormat(AUint index) const
{
	return _formats[index];
}

AUint* AnimaTexture::GetFormats() const
{
	return _formats;
}

void AnimaTexture::SetFilters(AUint* filters)
{
	if (filters != nullptr)
	{
		if (_filters == nullptr)
			_filters = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

		for (AUint i = 0; i < _texturesNumber; i++)
			_filters[i] = filters[i];
	}
	else
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _filters);
		_filters = nullptr;
	}
}

void AnimaTexture::SetFilter(AUint filter, AUint index)
{
	_filters[index] = filter;
}

AUint AnimaTexture::GetFilter(AUint index) const
{
	return _filters[index];
}

AUint* AnimaTexture::GetFilters() const
{
	return _filters;
}

void AnimaTexture::SetAttachments(AUint* attachments)
{
	if (attachments != nullptr)
	{
		if (_attachments == nullptr)
			_attachments = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

		for (AUint i = 0; i < _texturesNumber; i++)
			_attachments[i] = attachments[i];
	}
	else
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _attachments);
		_attachments = nullptr;
	}
}

void AnimaTexture::SetAttachment(AUint attachment, AUint index)
{
	_attachments[index] = attachment;
}

AUint AnimaTexture::GetAttachment(AUint index) const
{
	return _attachments[index];
}

AUint* AnimaTexture::GetAttachments() const
{
	return _attachments;
}

void AnimaTexture::SetInternalFormats(AUint* internalFormats)
{
	if (internalFormats != nullptr)
	{
		if (_internalFormats == nullptr)
			_internalFormats = AnimaAllocatorNamespace::AllocateArray<AUint>(*_allocator, _texturesNumber);

		for (AUint i = 0; i < _texturesNumber; i++)
			_internalFormats[i] = internalFormats[i];
	}
	else
	{
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _internalFormats);
		_internalFormats = nullptr;
	}
}

void AnimaTexture::SetInternalFormat(AUint internalFormat, AUint index)
{
	_internalFormats[index] = internalFormat;
}

AUint AnimaTexture::GetInternalFormat(AUint index) const
{
	return _internalFormats[index];
}

AUint* AnimaTexture::GetInternalFormats() const
{
	return _internalFormats;
}

void AnimaTexture::SetDatas(AUchar** datas, ASizeT* datasSize)
{
	if (datas != nullptr)
	{
		if (_datas == nullptr)
			_datas = AnimaAllocatorNamespace::AllocateArray<AUchar*>(*_allocator, _texturesNumber);

		if (_datasSize == nullptr)
			_datasSize = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*_allocator, _texturesNumber);

		for (AUint i = 0; i < _texturesNumber; i++)
			SetData(datas[i], datasSize[i], i);
	}
	else
	{
		for (AUint i = 0; i < _texturesNumber; i++)
		{
			if (_datas[i] != nullptr)
			{
				_allocator->Deallocate(_datas[i]);
				_datas[i] = nullptr;
			}
		}

		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datas);
		AnimaAllocatorNamespace::DeallocateArray(*_allocator, _datasSize);

		_datas = nullptr;
		_datasSize = nullptr;
	}
}

void AnimaTexture::SetData(AUchar* data, ASizeT dataSize, AUint index)
{
	if (data != nullptr && dataSize > 0)
	{
		if (_datas[index] != nullptr && _datasSize[index] != dataSize)
		{
			_allocator->Deallocate(_datas[index]);

			_datas[index] = (AUchar*)(_allocator)->Allocate(sizeof(AUchar) * dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));
			_datasSize[index] = dataSize;
		}
		else if (_datas[index] == nullptr)
		{
			_datas[index] = (AUchar*)(_allocator)->Allocate(sizeof(AUchar) * dataSize, ANIMA_ENGINE_ALIGN_OF(AUchar));
			_datasSize[index] = dataSize;
		}

		memcpy(_datas[index], data, sizeof(AUchar) * dataSize);
	}
	else
	{
		if (_datas[index] != nullptr)
		{
			_allocator->Deallocate(_datas[index]);
			_datas[index] = nullptr;
		}
		_datasSize[index] = 0;
	}
}

AUchar* AnimaTexture::GetData(AUint index) const
{
	return _datas[index];
}

const AUchar* AnimaTexture::GetConstData(AUint index) const
{
	return const_cast<AUchar*>(_datas[index]);
}

AUchar** AnimaTexture::GetDatas() const
{
	return _datas;
}

const AUchar** AnimaTexture::GetConstDatas() const
{
	return const_cast<const AUchar**>(_datas);
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

void AnimaTexture::EnableClamp(bool bEnable)
{
	_enableClamp = bEnable;
}

bool AnimaTexture::ClampEnabled() const
{
	return _enableClamp;
}

bool AnimaTexture::LoadTextures()
{
	if (_texturesReady)
		return true;

	glGenTextures(_texturesNumber, _textureIDs);

	for (AUint i = 0; i < _texturesNumber; i++)
	{
		glBindTexture(_textureTarget, _textureIDs[i]);

		glTexParameteri(_textureTarget, GL_TEXTURE_MAG_FILTER, _filters[i]);
		glTexParameteri(_textureTarget, GL_TEXTURE_MIN_FILTER, _filters[i]);
		
		if (_enableClamp)
		{
			glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		if (_mipMapLevels == 0)
			glTexImage2D(_textureTarget, 0, _internalFormats[i], _width, _height, 0, _formats[i], GL_UNSIGNED_BYTE, _datas[i]);
		else if (_mipMapLevels > 0)
		{
			unsigned int BlockSize = (_formats[i] == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			AUint width = _width;
			AUint height = _height;

			for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
			{
				unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

				glCompressedTexImage2D(_textureTarget, level, _formats[i], width, height, 0, size, _datas[i] + offset);

				offset += size;
				width /= 2;
				height /= 2;
			}
		}
		else
			return false;

		if (_filters[i] == GL_NEAREST_MIPMAP_NEAREST || _filters[i] == GL_NEAREST_MIPMAP_LINEAR ||
			_filters[i] == GL_LINEAR_MIPMAP_NEAREST || _filters[i] == GL_LINEAR_MIPMAP_LINEAR)
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
	}

	glBindTexture(_textureTarget, 0);

	_texturesReady = true;
	_renderTargetsReady = false;
	return true;
}

bool AnimaTexture::LoadRenderTargets()
{
	if (_renderTargetsReady)
		return true;

	if (_attachments == nullptr)
		return false;

	LoadTextures();

	AUint drawBuffers[32];
	ANIMA_ASSERT(_texturesNumber <= 32);

	bool hasDepth = false;
	for (AUint i = 0; i < _texturesNumber; i++)
	{
		if (_attachments[i] == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
			drawBuffers[i] = _attachments[i];

		if (_attachments[i] == GL_NONE)
			continue;

		if (_frameBuffer == 0)
		{
			glGenFramebuffers(1, &_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, _attachments[i], _textureTarget, _textureIDs[i], 0);
	}

	if (_frameBuffer == 0)
		return false;

	if (!hasDepth)
	{
		glGenRenderbuffers(1, &_renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _renderBuffer);
	}

	glDrawBuffers(_texturesNumber, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		ANIMA_ASSERT(false);
		_renderTargetsReady = false;
		_texturesReady = false;
		return false;
	}

	_renderTargetsReady = true;
	_texturesReady = false;
	return true;
}

bool AnimaTexture::IsReady()
{
	return _texturesReady || _renderTargetsReady;
}

void AnimaTexture::Unload()
{
	if (!IsReady())
		return;

	if(_textureIDs != nullptr) 
		glDeleteTextures(_texturesNumber, _textureIDs);

	if (_frameBuffer != 0)
		glDeleteFramebuffers(1, &_frameBuffer);

	if (_renderBuffer != 0)
		glDeleteRenderbuffers(1, &_renderBuffer);
	
	_texturesReady = false;
	_renderTargetsReady = false;
}

void AnimaTexture::Bind(AUint unit, AUint index) const
{
	ANIMA_ASSERT(_texturesReady);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(_textureTarget, _textureIDs[index]);
}

void AnimaTexture::BindAsRenderTarget() const
{
	ANIMA_ASSERT(_renderTargetsReady);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glViewport(0, 0, _width, _height);
}

END_ANIMA_ENGINE_NAMESPACE