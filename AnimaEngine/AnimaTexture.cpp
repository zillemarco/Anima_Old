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
	_textureTarget = TARGET_NONE;
	_textureID = 0;
	_filter = LINEAR;
	_internalFormat = IF_RGB;
	_format = RGB;
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

	_textureTarget = TARGET_NONE;
	_filter = LINEAR;
	_internalFormat = IF_RGB;
	_format = RGB;
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

void AnimaTexture::SetFormat(AnimaTextureFormat format)
{
	_format = format;
}

AnimaTextureFormat AnimaTexture::GetFormat() const
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

void AnimaTexture::SetInternalFormat(AnimaTextureInternalFormat internalFormat)
{
	_internalFormat = internalFormat;
}

AnimaTextureInternalFormat AnimaTexture::GetInternalFormat() const
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
	
	if (glGetError() != GL_NO_ERROR)
		return false;

	AUint target = TargetToPlatform(_textureTarget);
	AUint clamp = ClampToPlatform(_clamp);
	AUint filter = FilterToPlatform(_filter);
	AUint format = FormatToPlatform(_format);
	AUint internalFormat = InternalFormatToPlatform(_internalFormat);
	
	glGenTextures(1, &_textureID);

	glBindTexture(target, _textureID);
	
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
	if (glGetError() != GL_NO_ERROR)
		return false;
	
	glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, _borderColor.vec);
	if (glGetError() != GL_NO_ERROR)
		return false;
	
	if (_textureTarget == TEXTURE_2D)
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_S, clamp);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, clamp);
		if (glGetError() != GL_NO_ERROR)
			return false;

		AUchar* data = nullptr;
		if (_data.size() > 0 && _data[0].size() > 0)
			data = &_data[0][0];

		if (_mipMapLevels == 0 || _generateMipMaps || data == nullptr)
			glTexImage2D(target, 0, internalFormat, _width, _height, 0, format, _dataType, data);
		else if (_mipMapLevels > 0)
		{
			unsigned int BlockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			AUint width = _width;
			AUint height = _height;

			for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
			{
				unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

				//glCompressedTexImage2D(target, level, format, width, height, 0, size, &_data[0][0] + offset);
				glTexImage2D(target, level, internalFormat, width, height, 0, format, _dataType, data + offset);

				offset += size;
				width /= 2;
				height /= 2;
			}
		}
		else
		{
			return false;
		}
	}
	else if (_textureTarget == TEXTURE_CUBE)
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_S, clamp);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, clamp);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, clamp);
		if (glGetError() != GL_NO_ERROR)
			return false;

		for (AInt i = 0; i < 6; i++)
		{
			AUchar* data = nullptr;
			if (_data.size() > i && _data[i].size() > 0)
				data = &_data[i][0];

			if (_mipMapLevels == 0 || _generateMipMaps || data == nullptr)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, _width, _height, 0, format, _dataType, data);
			}
			else if (_mipMapLevels > 0)
			{
				unsigned int BlockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
				unsigned int offset = 0;

				AUint width = _width;
				AUint height = _height;

				for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
				{
					unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

					//glCompressedTexImage2D(target, level, _format, width, height, 0, size, &_data[0][0] + offset);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, internalFormat, width, height, 0, format, _dataType, &_data[i][0] + offset);

					offset += size;
					width /= 2;
					height /= 2;
				}
			}
			else
			{
				return false;
			}

			if (glGetError() != GL_NO_ERROR)
				return false;
		}
	}

	if (glGetError() != GL_NO_ERROR)
		return false;
	
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
		
	glFramebufferTexture2D(GL_FRAMEBUFFER, _attachment, TargetToPlatform(_textureTarget), _textureID, 0);
	
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
	AUint format = FormatToPlatform(_format);
	AUint internalFormat = InternalFormatToPlatform(_internalFormat);

	glBindTexture(target, _textureID);

	if (_textureTarget == TEXTURE_2D)
	{
		AUchar* data = nullptr;
		if (_data.size() > 0 && _data[0].size() > 0)
			data = &_data[0][0];

		if (_mipMapLevels == 0 || _generateMipMaps || data == nullptr)
			glTexImage2D(target, 0, internalFormat, _width, _height, 0, format, _dataType, data);
		else if (_mipMapLevels > 0)
		{
			unsigned int BlockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
			unsigned int offset = 0;

			AUint width = _width;
			AUint height = _height;

			for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
			{
				unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

				//glCompressedTexImage2D(target, level, format, width, height, 0, size, &_data[0][0] + offset);
				glTexImage2D(target, level, internalFormat, width, height, 0, format, _dataType, data + offset);

				offset += size;
				width /= 2;
				height /= 2;
			}
		}
		else
		{
			return false;
		}
	}
	else if (_textureTarget == TEXTURE_CUBE)
	{
		for (AInt i = 0; i < 6; i++)
		{
			AUchar* data = nullptr;
			if (_data.size() > i && _data[i].size() > 0)
				data = &_data[i][0];

			if (_mipMapLevels == 0 || _generateMipMaps || data == nullptr)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, _width, _height, 0, format, _dataType, data);
			}
			else if (_mipMapLevels > 0)
			{
				unsigned int BlockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
				unsigned int offset = 0;

				AUint width = _width;
				AUint height = _height;

				for (unsigned int level = 0; level < _mipMapLevels && (width || height); level++)
				{
					unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * BlockSize;

					//glCompressedTexImage2D(target, level, _format, width, height, 0, size, &_data[0][0] + offset);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, internalFormat, width, height, 0, format, _dataType, &_data[i][0] + offset);

					offset += size;
					width /= 2;
					height /= 2;
				}
			}
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

AUint AnimaTexture::FormatToPlatform(const AnimaTextureFormat& format)
{
	switch (format)
	{
	case RED:		return GL_RED; break;
	case GREEN:		return GL_GREEN; break;
	case BLUE:		return GL_BLUE; break;
	case RG:			return GL_RG; break;
	case RGB:		return GL_RGB; break;
	case RGBA:		return GL_RGBA; break;
	case BGR:		return GL_BGR; break;
	case BGRA:		return GL_BGRA; break;
	case RED_INT:	return GL_RED_INTEGER; break;
	case GREEN_INT:	return GL_GREEN_INTEGER; break;
	case BLUE_INT:	return GL_BLUE_INTEGER; break;
	case RG_INT:		return GL_RG_INTEGER; break;
	case RGB_INT:	return GL_RGB_INTEGER; break;
	case RGBA_INT:	return GL_RGBA_INTEGER; break;
	case BGR_INT:	return GL_BGR_INTEGER; break;
	case BGRA_INT:	return GL_BGRA_INTEGER; break;
	case DEPTH:		return GL_DEPTH_COMPONENT; break;
	default:		return GL_NONE;
	}
}

AUint AnimaTexture::InternalFormatToPlatform(const AnimaTextureInternalFormat& internalFormat)
{
	switch (internalFormat)
	{
	case R8:				return GL_R8; break;
	case R8_SNORM:			return GL_R8_SNORM; break;
	case R16:				return GL_R16; break;
	case R16_SNORM:			return GL_R16_SNORM; break;
	case RG8:				return GL_RG8; break;
	case RG8_SNORM:			return GL_RG8_SNORM; break;
	case RG16:				return GL_RG16; break;
	case RG16_SNORM:		return GL_RG16_SNORM; break;
	case R3_G3_B2:			return GL_R3_G3_B2; break;
	case RGB4:				return GL_RGB4; break;
	case RGB5:				return GL_RGB5; break;
	case RGB565:			return GL_RGB565; break;
	case RGB8:				return GL_RGB8; break;
	case RGB8_SNORM:		return GL_RGB8_SNORM; break;
	case RGB10:				return GL_RGB10; break;
	case RGB12:				return GL_RGB12; break;
	case RGB16:				return GL_RGB16; break;
	case RGB16_SNORM:		return GL_RGB16_SNORM; break;
	case RGBA2:				return GL_RGBA2; break;
	case RGBA4:				return GL_RGBA4; break;
	case RGB5_A1:			return GL_RGB5_A1; break;
	case RGBA8:				return GL_RGBA8; break;
	case RGBA8_SNORM:		return GL_RGBA8_SNORM; break;
	case RGB10_A2:			return GL_RGB10_A2; break;
	case RGB10_A2UI:		return GL_RGB10_A2UI; break;
	case RGBA12:			return GL_RGBA12; break;
	case RGBA16:			return GL_RGBA16; break;
	case RGBA16_SNORM:		return GL_RGBA16_SNORM; break;
	case SRGB8:				return GL_SRGB8; break;
	case SRGB8_A8:			return GL_SRGB8_ALPHA8; break;
	case R16F:				return GL_R16F; break;
	case RG16F:				return GL_RG16F; break;
	case RGB16F:			return GL_RGB16F; break;
	case RGBA16F:			return GL_RGBA16F; break;
	case R32F:				return GL_R32F; break;
	case RG32F:				return GL_RG32F; break;
	case RGB32F:			return GL_RGB32F; break;
	case RGBA32F:			return GL_RGBA32F; break;
	case R11F_G11F_B10F:	return GL_R11F_G11F_B10F; break;
	case RGB9_E5:			return GL_RGB9_E5; break;
	case R8I:				return GL_R8I; break;
	case R8UI:				return GL_R8UI; break;
	case R16I:				return GL_R16I; break;
	case R16UI:				return GL_R16UI; break;
	case R32I:				return GL_R32I; break;
	case R32UI:				return GL_R32UI; break;
	case RG8I:				return GL_RG8I; break;
	case RG8UI:				return GL_RG8UI; break;
	case RG16I:				return GL_RG16I; break;
	case RG16UI:			return GL_RG16UI; break;
	case RG32I:				return GL_RG32I; break;
	case RG32UI:			return GL_RG32UI; break;
	case RGB8I:				return GL_RGB8I; break;
	case RGB8UI:			return GL_RGB8UI; break;
	case RGB16I:			return GL_RGB16I; break;
	case RGB16UI:			return GL_RGB16UI; break;
	case RGB32I:			return GL_RGB32I; break;
	case RGB32UI:			return GL_RGB32UI; break;
	case RGBA8I:			return GL_RGBA8I; break;
	case RGBA8UI:			return GL_RGBA8UI; break;
	case RGBA16I:			return GL_RGBA16I; break;
	case RGBA16UI:			return GL_RGBA16UI; break;
	case RGBA32I:			return GL_RGBA32I; break;
	case RGBA32UI:			return GL_RGBA32UI; break;
	case DEPTH16:			return GL_DEPTH_COMPONENT16; break;
	case DEPTH24:			return GL_DEPTH_COMPONENT24; break;
	case DEPTH32:			return GL_DEPTH_COMPONENT32; break;
	case DEPTH32F:			return GL_DEPTH_COMPONENT32F; break;
	case IF_RED:			return GL_RED; break;
	case IF_RG:				return GL_RG; break;
	case IF_RGB:			return GL_RGB; break;
	case IF_RGBA:			return GL_RGBA; break;
	default:				return GL_NONE;
	}
}

END_ANIMA_ENGINE_NAMESPACE