//
//  AnimaGBuffer.cpp
//  Anima
//
//  Created zy Marco Zille on 26/11/14.
//
//

#include "AnimaGBuffer.h"
using namespace boost::multi_index;

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaGBuffer::AnimaGBuffer(AnimaAllocator* allocator)
{
	_allocator = allocator;
	_width = 0;
	_height = 0;
	_frameBuffer = 0;
	_renderBuffer = 0;
	_created = false;
	_needsResize = false;
}

AnimaGBuffer::AnimaGBuffer(AnimaAllocator* allocator, AUint width, AUint height)
{
	_allocator = allocator;
	_width = width;
	_height = height;
	_frameBuffer = 0;
	_renderBuffer = 0;
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
	_renderBuffer = 0;
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
	_renderBuffer = 0;
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

bool AnimaGBuffer::AddTexture(const AnimaString& name, AUint target, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter, AUint clamp)
{
	AnimaGBufferDataSetByName& nameIterator = get<1>(_texturesSet);
	if (nameIterator.find(name) != nameIterator.end())
		return false;

	AUint index = (AUint)_texturesSet.size();

	AnimaTexture* texture = AnimaAllocatorNamespace::AllocateNew<AnimaTexture>(*_allocator, _allocator, name, target, _width, _height, nullptr, 0, 0, filter, internalFormat, format, dataType, clamp/*, attachment*/);
	AnimaGBufferData* data = AnimaAllocatorNamespace::AllocateNew<AnimaGBufferData>(*_allocator, name, texture, index, attachment);
	_texturesSet.insert(data);
	
	return true;
}

bool AnimaGBuffer::AddTexture(const char* name, AUint target, AUint attachment, AUint internalFormat, AUint format, AUint dataType, AUint filter, AUint clamp)
{
	AnimaString str = name;
	return AddTexture(str, target, attachment, internalFormat, format, dataType, filter, clamp);
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

			if (_renderBuffer != 0)
			{
				glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
			}

			_needsResize = false;
		}

		return true;
	}
	
	AUint size = (AUint)_texturesSet.size();
	if (size == 0)
		return false;
		
	AUint drawBuffers[32];
	ANIMA_ASSERT(size <= 32);
	
	bool hasDepth = false;
	int i = 0;
	AnimaGBufferDataSetByIndex& indexIterator = get<0>(_texturesSet);
	for (auto element = indexIterator.begin(), end = indexIterator.end(); element != end; element++, i++)
	{
		AnimaGBufferData* data = *element;
		
		ANIMA_ASSERT(data->_texture->Load());

		if (data->_attachment == GL_DEPTH_ATTACHMENT)
		{
			drawBuffers[i] = GL_NONE;
			hasDepth = true;
		}
		else
			drawBuffers[i] = data->_attachment;
	
		if (data->_attachment == GL_NONE)
			continue;
	
		if (_frameBuffer == 0)
		{
			glGenFramebuffers(1, &_frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		}
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, data->_attachment, data->_texture->GetTextureTarget(), data->_texture->GetID(), 0);
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
	
	glDrawBuffers(size, drawBuffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || glGetError() != GL_NO_ERROR)
	{
		ANIMA_ASSERT(false);
		_created = false;
		return false;
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

	if (_renderBuffer != 0)
	{
		glDeleteRenderbuffers(1, &_renderBuffer);
		_renderBuffer = 0;
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

AnimaTexture* AnimaGBuffer::GetTexture(const char* name)
{
	AnimaString str = name;
	return GetTexture(str);
}

void AnimaGBuffer::BindAsRenderTarget()
{
	ANIMA_ASSERT(Create());

	if(glGetError() != GL_NO_ERROR)
		return;
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	glViewport(0, 0, _width, _height);
	
	if(glGetError() != GL_NO_ERROR)
		return;
}

END_ANIMA_ENGINE_NAMESPACE
