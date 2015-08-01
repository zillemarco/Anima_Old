#include "AnimaTexture.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AFloat Clamp(const AFloat& value, const AFloat& low, const AFloat& high)
{
	return value < low ? low : (value > high ? high : value);
}

AnimaTexture::AnimaTexture(AnimaAllocator* allocator)
	: AnimaNamedObject(AnimaString("AnimaTexture"), allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaTexture);

	_mipMapLevels = 0;
	_width = 0;
	_height = 0;
	_textureTarget = TEXTURE_2D;
	_textureID = 0;
	_filter = LINEAR;
	_internalFormat = GL_RGB;
	_format = GL_RGB;
	_dataType = GL_UNSIGNED_BYTE;
	_clamp = REPEAT;
	_frameBuffer = 0;
	_renderBuffer = 0;
	_attachment = GL_NONE;
	_borderColor = AnimaColor4f(0.0f);
	_generateMipMaps = false;
	
	_renderTargetsReady = false;
	_texturesReady = false;
	_needsResize = false;
}

AnimaTexture::AnimaTexture(AnimaAllocator* allocator, const AnimaString& name, AUint width, AUint height, AUchar* data, AUint dataSize)
	: AnimaNamedObject(name, allocator)
{
	IMPLEMENT_ANIMA_CLASS(AnimaTexture);

	SetData(data, dataSize);

	_textureTarget = TEXTURE_2D;
	_filter = LINEAR;
	_internalFormat = GL_RGB;
	_format = GL_RGB;
	_dataType = GL_UNSIGNED_BYTE;
	_clamp = REPEAT;
	_attachment = GL_NONE;
	_borderColor = AnimaColor4f(0.0f);
	_generateMipMaps = false;

	_textureID = 0;
	_frameBuffer = 0;
	_renderBuffer = 0;

	_name = name;
		
	_mipMapLevels = 0;
	_width = width;
	_height = height;
	_texturesReady = false;
	_needsResize = false;
	_renderTargetsReady = false;
}

AnimaTexture::AnimaTexture(const AnimaTexture& src)
	: AnimaNamedObject(src)
{
	SetFilter(src._filter);
	SetFormat(src._format);
	SetInternalFormat(src._internalFormat);
	SetDataType(src._dataType);
	SetClamp(src._clamp);
	SetAttachment(src._attachment);
	SetBorderColor(src._borderColor);
	
	_textureID = 0;
	_frameBuffer = 0;
	_renderBuffer = 0;

	_mipMapLevels = src._mipMapLevels;
	_generateMipMaps = src._generateMipMaps;
	_textureTarget = src._textureTarget;
	_width = src._width;
	_height = src._height;
	_texturesReady = false;
	_needsResize = false;
	_renderTargetsReady = false;

	CopyData(src);
}

AnimaTexture::AnimaTexture(AnimaTexture&& src)
	: AnimaNamedObject(src)
{
	SetFilter(src._filter);
	SetFormat(src._format);
	SetInternalFormat(src._internalFormat);
	SetDataType(src._dataType);
	SetClamp(src._clamp);
	SetAttachment(src._attachment);
	SetBorderColor(src._borderColor);

	_textureID = 0;
	_frameBuffer = 0;
	_renderBuffer = 0;

	_mipMapLevels = src._mipMapLevels;
	_generateMipMaps = src._generateMipMaps;
	_textureTarget = src._textureTarget;
	_width = src._width;
	_height = src._height;
	_texturesReady = false;
	_needsResize = false;
	_renderTargetsReady = false;

	CopyData(src);
}

AnimaTexture::~AnimaTexture()
{
	Unload();
}

AnimaTexture& AnimaTexture::operator=(const AnimaTexture& src)
{
	if (this != &src)
	{
		Unload();
		
		AnimaNamedObject::operator=(src);

		SetFilter(src._filter);
		SetFormat(src._format);
		SetInternalFormat(src._internalFormat);
		SetDataType(src._dataType);
		SetClamp(src._clamp);
		SetAttachment(src._attachment);
		SetBorderColor(src._borderColor);
		
		_textureID = 0;
		_frameBuffer = 0;
		_renderBuffer = 0;
		
		_mipMapLevels = src._mipMapLevels;
		_generateMipMaps = src._generateMipMaps;
		_textureTarget = src._textureTarget;
		_width = src._width;
		_height = src._height;
		_texturesReady = false;
		_needsResize = false;
		_renderTargetsReady = false;

		CopyData(src);
	}
	return *this;
}

AnimaTexture& AnimaTexture::operator=(AnimaTexture&& src)
{
	if (this != &src)
	{
		Unload();

		AnimaNamedObject::operator=(src);

		SetFilter(src._filter);
		SetFormat(src._format);
		SetInternalFormat(src._internalFormat);
		SetDataType(src._dataType);
		SetClamp(src._clamp);
		SetAttachment(src._attachment);
		SetBorderColor(src._borderColor);

		_textureID = 0;
		_frameBuffer = 0;
		_renderBuffer = 0;
		
		_mipMapLevels = src._mipMapLevels;
		_generateMipMaps = src._generateMipMaps;
		_textureTarget = src._textureTarget;
		_width = src._width;
		_height = src._height;
		_texturesReady = false;
		_needsResize = false;
		_renderTargetsReady = false;

		CopyData(src);
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

void AnimaTexture::SetMipMapLevels(AUint levels, bool generate)
{
	_mipMapLevels = levels;
	_generateMipMaps = generate;
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

void AnimaTexture::SetFilter(AnimaTextureFilterMode filter)
{
	_filter = filter;
}

AnimaTextureFilterMode AnimaTexture::GetFilter() const
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

bool AnimaTexture::SetData(AUchar* data, AUint dataSize)
{
	if (_textureTarget == TEXTURE_2D)
	{
		if (_data.size() != 1)
		{
			AInt count = _data.size();
			for (AInt i = 0; i < count; i++)
				_data[i].clear();
			_data.clear();

			_data.resize(1);
		}

		AnimaArray<AUchar> newData;

		if (data != nullptr && dataSize > 0)
		{
			newData.resize(dataSize);
			newData.assign(data, data + dataSize);
		}

		_data[0] = newData;

		return true;
	}

	return false;
}

bool AnimaTexture::SetData(AUchar* data, AUint dataSize, AnimaTextureCubeIndex index)
{
	if (_textureTarget == TEXTURE_CUBE)
	{
		// se l'array ha un solo elemento al momento vuol dire che prima la texture non era 3D
		// e quindi vado a ripulire tutto e importare l'array per avere 6 elementi
		if (_data.size() != 6)
		{
			AInt count = _data.size();
			for (AInt i = 0; i < count; i++)
				_data[i].clear();
			_data.clear();

			_data.resize(6);
		}

		AnimaArray<AUchar> newData;
		if (data != nullptr && dataSize > 0)
		{
			newData.resize(dataSize);
			newData.assign(data, data + dataSize);
		}

		_data[index] = newData;

		return true;
	}

	return false;
}

const AUchar* AnimaTexture::GetData() const
{
	// Posso tornare il valore solamente se la texture è di tipo 2D e l'array di buffer ha un solo elemento
	if (_textureTarget == TEXTURE_2D && _data.size() == 1)
	{
		// Devo controllare anche che il buffer non sia vuoto
		const AnimaArray<AUchar>* data = &_data[0];
		if (data->size() > 0)
			return &(*data)[0];
	}

	return nullptr;
}

const AUchar* AnimaTexture::GetData(AnimaTextureCubeIndex index) const
{
	// Posso tornare il valore solamente se la texture è di tipo 3D e l'array di buffer ha sei elementi
	if (_textureTarget == TEXTURE_3D && _data.size() == 6)
	{
		// Devo controllare anche che il buffer non sia vuoto
		const AnimaArray<AUchar>* data = &_data[index];
		if (data->size() > 0)
			return &(*data)[0];
	}

	return nullptr;
}

void AnimaTexture::SetTextureTarget(AnimaTextureTarget target)
{
	if (target != _textureTarget)
	{
		ANIMA_ASSERT(!IsReady());

		AInt count = _data.size();
		for (AInt i = 0; i < count; i++)
			_data[i].clear();
		_data.clear();

		if (target == TEXTURE_2D)
			_data.resize(1);
		else if (target == TEXTURE_CUBE)
			_data.resize(6);

		_textureTarget = target;
	}
}

AnimaTextureTarget AnimaTexture::GetTextureTarget() const
{
	return _textureTarget;
}

void AnimaTexture::SetClamp(AnimaTextureClampMode clamp)
{
	_clamp = clamp;
}

AnimaTextureClampMode AnimaTexture::GetClamp() const
{
	return _clamp;
}

void AnimaTexture::SetBorderColor(const AnimaColor4f& color)
{
	_borderColor = color;
}

void AnimaTexture::SetBorderColor(const AFloat& r, const AFloat& g, const AFloat& b, const AFloat& a)
{
	_borderColor = AnimaColor4f(r, g, b, a);
}

AnimaColor4f AnimaTexture::GetColor() const
{
	return _borderColor;
}

bool AnimaTexture::Load()
{
	if (_texturesReady)
	{
		if (_needsResize)
			return ResizeTexture();
		return true;
	}

	AUint target = TargetToPlatform(_textureTarget);
	AUint clamp = ClampToPlatform(_clamp);
	AUint filter = FilterToPlatform(_filter);
	
	glGenTextures(1, &_textureID);

	glBindTexture(target, _textureID);
	
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
	
	glTexParameteri(target, GL_TEXTURE_WRAP_S, clamp);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, clamp);

	glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, _borderColor.vec);
	
	if (_textureTarget == TEXTURE_2D)
	{
		if (_mipMapLevels == 0 || _generateMipMaps)
			glTexImage2D(target, 0, _internalFormat, _width, _height, 0, _format, _dataType, &_data[0][0]);
		else if (_mipMapLevels > 0)
		{
			unsigned int BlockSize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			AUint width = _width;
			AUint height = _height;

			for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
			{
				unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

				//glCompressedTexImage2D(target, level, _format, width, height, 0, size, &_data[0][0] + offset);
				glTexImage2D(target, level, _internalFormat, width, height, 0, _format, _dataType, &_data[0][0] + offset);

				offset += size;
				width /= 2;
				height /= 2;
			}
		}
		else
			return false;
	}
	else if (_textureTarget == TEXTURE_CUBE)
	{
		if (_mipMapLevels == 0 || _generateMipMaps)
		{
			for (AInt i = 0; i < 6; i++)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _internalFormat, _width, _height, 0, _format, _dataType, &_data[i][0]);
			}
		}
		else if (_mipMapLevels > 0)
		{
			for (AInt i = 0; i < 6; i++)
			{
				unsigned int BlockSize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
				unsigned int offset = 0;

				AUint width = _width;
				AUint height = _height;

				for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
				{
					unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

					//glCompressedTexImage2D(target, level, _format, width, height, 0, size, &_data[0][0] + offset);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, _internalFormat, width, height, 0, _format, _dataType, &_data[i][0] + offset);

					offset += size;
					width /= 2;
					height /= 2;
				}
			}
		}
	}
	
	if (_generateMipMaps || (_mipMapLevels == 0  && (filter == GL_NEAREST_MIPMAP_NEAREST || filter == GL_NEAREST_MIPMAP_LINEAR || filter == GL_LINEAR_MIPMAP_NEAREST || filter == GL_LINEAR_MIPMAP_LINEAR)))
	{
		glGenerateMipmap(target);

		AFloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, Clamp(0.0f, 8.0f, maxAnisotropy));
	}
	else
	{ 
		glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, 0);
	}

	if (glGetError() != GL_NO_ERROR)
		return false;

	glBindTexture(target, 0);

	_texturesReady = true;
	return true;
}

bool AnimaTexture::LoadRenderTargets()
{
	if (_renderTargetsReady && _texturesReady)
	{
		bool needsResize = _needsResize;
		bool loadResult = Load();

		if (loadResult && needsResize)
		{
			if (_renderBuffer != 0)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
			}
		}

		return loadResult;
	}
	
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
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || glGetError() != GL_NO_ERROR)
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

bool AnimaTexture::AreRenderTargetsReady()
{
	return _texturesReady && _renderTargetsReady;
}

void AnimaTexture::Unload()
{
	if (!IsReady())
		return;

	if (_textureID != 0)
	{
		glDeleteTextures(1, &_textureID);
		_textureID = 0;
	}

	if (AreRenderTargetsReady())
	{
		glDeleteFramebuffers(1, &_frameBuffer);
		glDeleteRenderbuffers(1, &_renderBuffer);
		_frameBuffer = 0;
		_renderBuffer = 0;
	}
		
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

bool AnimaTexture::ResizeTexture()
{
	AUint target = TargetToPlatform(_textureTarget);
	glBindTexture(target, _textureID);

	if (_textureTarget == TEXTURE_2D)
	{
		if (_mipMapLevels == 0)
			glTexImage2D(target, 0, _internalFormat, _width, _height, 0, _format, _dataType, &_data[0][0]);
		else if (_mipMapLevels > 0)
		{
			unsigned int BlockSize = (_format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			AUint width = _width;
			AUint height = _height;

			for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
			{
				unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

				glCompressedTexImage2D(target, level, _format, width, height, 0, size, &_data[0][0] + offset);

				offset += size;
				width /= 2;
				height /= 2;
			}
		}
		else
		{
			glBindTexture(target, 0);
			return false;
		}
	}

	if (glGetError() != GL_NO_ERROR)
	{
		glBindTexture(target, 0);
		return false;
	}

	glBindTexture(target, 0);
	_needsResize = false;

	return true;
}

void AnimaTexture::CopyData(const AnimaTexture& src)
{
	AInt count = _data.size();
	for (AInt i = 0; i < count; i++)
		_data[i].clear();
	_data.clear();

	AInt srcDataSize = src._data.size();
	
	_data.resize(srcDataSize);

	for (AInt i = 0; i < count; i++)
		_data[i] = src._data[i];
}

void AnimaTexture::Bind(AUint unit) const
{
	ANIMA_ASSERT(_texturesReady);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(TargetToPlatform(_textureTarget), _textureID);
}

void AnimaTexture::BindAsRenderTarget()
{
	ANIMA_ASSERT(LoadRenderTargets());
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glViewport(0, 0, _width, _height);
}

AUint AnimaTexture::TargetToPlatform(const AnimaTextureTarget& target)
{
	switch (target)
	{
	case TEXTURE_2D:	return GL_TEXTURE_2D;
	case TEXTURE_3D:	return GL_TEXTURE_3D;
	case TEXTURE_CUBE:	return GL_TEXTURE_CUBE_MAP;
	default:			return GL_NONE;
	}
}

AUint AnimaTexture::ClampToPlatform(const AnimaTextureClampMode& clamp)
{
	switch (clamp)
	{
	case REPEAT:			return GL_REPEAT;
	case MIRRORED_REPEAT:	return GL_MIRRORED_REPEAT;
	case TO_EDGE:			return GL_CLAMP_TO_EDGE;
	case TO_BORDER:			return GL_CLAMP_TO_BORDER;
	default:				return GL_NONE;
	}
}

AUint AnimaTexture::FilterToPlatform(const AnimaTextureFilterMode& filter)
{
	switch (filter)
	{
	case NEAREST:					return GL_NEAREST;
	case LINEAR:					return GL_LINEAR;
	case NEAREST_MIPMAP_NEAREST:	return GL_NEAREST_MIPMAP_NEAREST;
	case NEAREST_MIPMAP_LINEAR:		return GL_NEAREST_MIPMAP_LINEAR;
	case LINEAR_MIPMAP_NEAREST:		return GL_LINEAR_MIPMAP_NEAREST;
	case LINEAR_MIPMAP_LINEAR:		return GL_LINEAR_MIPMAP_LINEAR;
	default:						return GL_NONE;
	}
}

AUint AnimaTexture::CubeIndexToPlatform(const AnimaTextureCubeIndex& index)
{
	switch (index)
	{
	case POSITIVE_X:	return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	case NEGATIVE_X:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	case POSITIVE_Y:	return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	case NEGATIVE_Y:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	case POSITIVE_Z:	return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	case NEGATIVE_Z:	return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	default:			return GL_NONE;
	}
}

END_ANIMA_ENGINE_NAMESPACE