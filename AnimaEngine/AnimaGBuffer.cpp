//
//  AnimaGBuffer.cpp
//  Anima
//
//  Created zy Marco Zille on 26/11/14.
//
//

#include "AnimaGBuffer.h"
#include "AnimaLogger.h"
using namespace boost::multi_index;

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaGBuffer::AnimaGBuffer(AnimaAllocator* allocator)
{
	_allocator = allocator;
	_width = 0;
	_height = 0;
	_frameBuffer = 0;
	_depthRenderBuffer = 0;
	_stencilRenderBuffer = 0;
	_created = false;
	_needsResize = false;
}

AnimaGBuffer::AnimaGBuffer(AnimaAllocator* allocator, AUint width, AUint height)
{
	_allocator = allocator;
	_width = width;
	_height = height;
	_frameBuffer = 0;
	_depthRenderBuffer = 0;
	_stencilRenderBuffer = 0;
	_created = false;
	_needsResize = false;
}

AnimaGBuffer::AnimaGBuffer(const AnimaGBuffer& src)
{
	_allocator = src._allocator;
	_texturesSet = src._texturesSet;
	_width = src._width;
	_height = src._height;

	_frameBuffer = 0;
	_depthRenderBuffer = 0;
	_stencilRenderBuffer = 0;
	_created = false;
	_needsResize = false;
}

AnimaGBuffer::AnimaGBuffer(AnimaGBuffer&& src)
{
	_allocator = src._allocator;
	_texturesSet = src._texturesSet;
	_width = src._width;
	_height = src._height;

	_frameBuffer = 0;
	_depthRenderBuffer = 0;
	_stencilRenderBuffer = 0;
	_created = false;
	_needsResize = false;
}

AnimaGBuffer::~AnimaGBuffer()
{
	ClearTextures();
	Destroy();
}

AnimaGBuffer& AnimaGBuffer::operator=(const AnimaGBuffer& src)
{
	if (this != &src)
	{
		ClearTextures();
		Destroy();

		_allocator = src._allocator;
		_texturesSet = src._texturesSet;
		_width = src._width;
		_height = src._height;
		_needsResize = false;
	}

	return *this;
}

AnimaGBuffer& AnimaGBuffer::operator=(AnimaGBuffer&& src)
{
	if (this != &src)
	{
		ClearTextures();
		Destroy();

		_allocator = src._allocator;
		_texturesSet = src._texturesSet;
		_width = src._width;
		_height = src._height;
		_needsResize = false;
	}

	return *this;
}

void AnimaGBuffer::ClearTextures()
{
	for (auto element : _texturesSet)
	{
		if (element != nullptr)
		{
			if (element->_texture != nullptr)
			{
				AnimaAllocatorNamespace::DeallocateObject(*_allocator, element->_texture);
				element->_texture = nullptr;
			}

			AnimaAllocatorNamespace::DeallocateObject(*_allocator, element);
			element = nullptr;
		}
	}

	_texturesSet.clear();
}

bool AnimaGBuffer::AddTexture(const AnimaString& name, AnimaTextureTarget target, AnimaTextureAttachment attachment, AnimaTextureInternalFormat internalFormat, AnimaTextureFormat format, AnimaTextureDataType dataType, AnimaTextureMinFilterMode minFilter, AnimaTextureMagFilterMode magFilter, AnimaTextureClampMode clampS, AnimaTextureClampMode clampT, AnimaTextureClampMode clampR)
{
	AnimaGBufferDataSetByName& nameIterator = get<1>(_texturesSet);
	if (nameIterator.find(name) != nameIterator.end())
		return false;

	AUint index = (AUint)_texturesSet.size();

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, name, _width, _height, nullptr, 0);
	texture->SetTextureTarget(target);
	texture->SetMinFilter(minFilter);
	texture->SetMagFilter(magFilter);
	texture->SetInternalFormat(internalFormat);
	texture->SetFormat(format);
	texture->SetDataType(dataType);
	texture->SetClampS(clampS);
	texture->SetClampT(clampT);
	texture->SetClampR(clampR);
	texture->SetAttachment(attachment);

	AnimaGBufferData* data = AnimaAllocatorNamespace::AllocateNew<AnimaGBufferData>(*_allocator, name, texture, index, attachment);
	_texturesSet.insert(data);
	
	return true;
}

bool AnimaGBuffer::Create()
{
	if (_created)
	{
		if (_needsResize)
		{
			AnimaGBufferDataSetByIndex& indexIterator = get<0>(_texturesSet);
			for (auto element = indexIterator.begin(), end = indexIterator.end(); element != end; element++)
				(*element)->_texture->Load();

			if (_depthRenderBuffer != 0)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
			}

			if (_stencilRenderBuffer != 0)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, _stencilRenderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, _width, _height);
			}
			
			_needsResize = false;
		}

		return true;
	}
	
	AUint size = (AUint)_texturesSet.size();
	if (size == 0)
	{
		_created = false;
		return false;
	}
		
	AUint drawBuffers[32];
	ANIMA_ASSERT(size <= 32);
	
	bool hasDepth = false;
	bool hasStencil = false;
	int i = 0;
	AnimaGBufferDataSetByIndex& indexIterator = get<0>(_texturesSet);
	for (auto element = indexIterator.begin(), end = indexIterator.end(); element != end; element++, i++)
	{
		AnimaGBufferData* data = *element;
		AUint dataAttachment = AnimaTexture::AttachmentToPlatform(data->_attachment);
		
		ANIMA_ASSERT(data->_texture->Load());

		if (dataAttachment == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else if (dataAttachment == GL_STENCIL_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasStencil = true;
		}
		else if (dataAttachment == GL_DEPTH_STENCIL_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
			hasStencil = true;
		}
		else
			drawBuffers[i] = dataAttachment;
	
		if (dataAttachment == GL_NONE)
			continue;
	
		if (_frameBuffer == 0)
		{
			glGenFramebuffers(1, &_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

			if (glGetError() != GL_NO_ERROR)
			{
				_created = false;
				return false;
			}
		}
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, dataAttachment, AnimaTexture::TargetToPlatform(data->_texture->GetTextureTarget()), data->_texture->GetID(), 0);

		if (glGetError() != GL_NO_ERROR)
		{
			_created = false;
			return false;
		}
	}
	
	if (_frameBuffer == 0)
	{
		_created = false;
		return false;
	}
	
	if (!hasDepth && !hasStencil)
	{
		glGenRenderbuffers(1, &_depthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);

		if (glGetError() != GL_NO_ERROR)
		{
			_created = false;
			return false;
		}
	}
	else if (!hasDepth)
	{
		glGenRenderbuffers(1, &_depthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);

		if (glGetError() != GL_NO_ERROR)
		{
			_created = false;
			return false;
		}
	}
	
	glDrawBuffers(size, drawBuffers);

	if (glGetError() != GL_NO_ERROR)
	{
		_created = false;
		return false;
	}

	AUint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		printf("AnimaGBuffer error: unsupported extension\n");
		_created = false;
		return false;
		break;
	default:
		printf("AnimaGBuffer error: unable to create the FBO\n");
		_created = false;
		return false;
		break;
	}
	
	_created = true;

	return true;
}

void AnimaGBuffer::Destroy()
{
	if (_frameBuffer != 0)
	{
		glDeleteFramebuffers(1, &_frameBuffer);
		_frameBuffer = 0;
	}

	if (_depthRenderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &_depthRenderBuffer);
		_depthRenderBuffer = 0;
	}

	if (_stencilRenderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &_stencilRenderBuffer);
		_stencilRenderBuffer = 0;
	}

	_created = false;
}

void AnimaGBuffer::Resize(AUint width, AUint height)
{
	if (width != _width || height != _height)
	{
		_width = width;
		_height = height;

		_needsResize = true;

		//Destroy();

		AnimaGBufferDataSetByIndex& indexIterator = get<0>(_texturesSet);
		for (auto element = indexIterator.begin(), end = indexIterator.end(); element != end; element++)
			(*element)->_texture->Resize(_width, _height);
	}
}

AnimaTexture* AnimaGBuffer::GetTexture(const AnimaString& name)
{
	AnimaGBufferDataSetByName& nameIterator = get<1>(_texturesSet);
	auto element = nameIterator.find(name);
	if (element != nameIterator.end())
		return (*element)->_texture;
	
	return nullptr;
}

void AnimaGBuffer::BindAsRenderTarget()
{
	ANIMA_ASSERT(Create());

	//if(glGetError() != GL_NO_ERROR)
	//	return;
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glViewport(0, 0, _width, _height);
	
	//if(glGetError() != GL_NO_ERROR)
	//	return;
}

void AnimaGBuffer::BindAsReadingSource()
{
	ANIMA_ASSERT(Create());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);

	//AnimaLogger::CheckGraphicErrors();
}

END_ANIMA_ENGINE_NAMESPACE
