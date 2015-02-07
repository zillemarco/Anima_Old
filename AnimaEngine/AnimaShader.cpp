#include "AnimaShader.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaShader::AnimaShader(AnimaAllocator* allocator)
	: _text(allocator)
{
	_allocator = allocator;
	_id = 0;
	_compiled = false;

	_type = INVALID;
}

AnimaShader::AnimaShader(AnimaAllocator* allocator, AnimaString shaderText, AnimaShaderType type)
	: _text(allocator)
{
	_allocator = allocator;
	_id = 0;
	_compiled = false;

	_type = type;
	_text = shaderText;
}

AnimaShader::AnimaShader(AnimaAllocator* allocator, const char* shaderText, AnimaShaderType type)
	: _text(allocator)
{
	_allocator = allocator;
	_id = 0;
	_compiled = false;

	_type = type;
	_text = shaderText;
}

AnimaShader::AnimaShader(const AnimaShader& src)
	: _text(src._text)
{
	_allocator = src._allocator;
	_id = src._id;
	_type = src._type;
	_compiled = src._compiled;
}

AnimaShader::AnimaShader(AnimaShader&& src)
	: _text(src._text)
	, _id(src._id)
	, _type(src._type)
	, _compiled(src._compiled)
	, _allocator(src._allocator)
{
	src._id = 0;
	src._compiled = false;
}

AnimaShader::~AnimaShader()
{
	Delete();
}

AnimaShader& AnimaShader::operator=(const AnimaShader& src)
{
	if (this != &src)
	{
		_text = src._text;
		_id = src._id;
		_type = src._type;
		_compiled = src._compiled;
	}

	return *this;
}

AnimaShader& AnimaShader::operator=(AnimaShader&& src)
{
	if (this != &src)
	{
		_text = src._text;
		_id = src._id;
		_type = src._type;
		_compiled = src._compiled;

		src._id = 0;
	}

	return *this;
}

inline bool AnimaShader::operator==(const AnimaShader& left)
{
	if (_id != left._id) return false;
	if (_type != left._type) return false;
	if (_text != left._text) return false;
	if (_allocator != left._allocator) return false;
	if (_compiled != left._compiled) return false;

	return true;
}

inline bool AnimaShader::operator!=(const AnimaShader& left)
{
	if (_id != left._id) return true;
	if (_type != left._type) return true;
	if (_text != left._text) return true;
	if (_allocator != left._allocator) return true;
	if (_compiled != left._compiled) return true;

	return false;
}

void AnimaShader::SetText(AnimaString text)
{
	_text = text;
}

void AnimaShader::SetText(const char* text)
{
	_text = text;
}

AnimaString AnimaShader::GetAnimaText()
{
	return _text;
}

const char* AnimaShader::GetText()
{
	return _text.GetConstBuffer();
}

void AnimaShader::SetType(AnimaShaderType type)
{
	_type = type;
}

AnimaShader::AnimaShaderType AnimaShader::GetType()
{
	return _type;
}

AInt AnimaShader::GetID()
{
	return _id;
}

bool AnimaShader::Create()
{
	if (_id > 0)
		return true;

	_id = glCreateShader(_type);

	if (_id == 0)
		return false;
	return true;
}

bool AnimaShader::Compile()
{
	if (!IsCreated())
	{
		if (!Create())
			return false;
	}

	if (IsCompiled())
		return true;

	const GLchar* source = (const GLchar*)_text.GetConstBuffer();
	glShaderSource(_id, 1, &source, NULL);
	
	glCompileShader(_id);
	
	GLint isCompiled = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

		char* infoLog = new char[maxLength];
		glGetShaderInfoLog(_id, maxLength, &maxLength, &infoLog[0]);

		ANIMA_ASSERT(false);// , infoLog);

		Delete();

		_compiled = false;
	}
	else
	{
		_compiled = true;
	}

	return _compiled;
}

bool AnimaShader::Delete()
{
	if (_id > 0)
	{
		glDeleteShader(_id);
		if (glGetError() != GL_NO_ERROR)
			return false;

		_id = 0;
	}
	return true;
}

bool AnimaShader::IsCreated()
{
	return _id > 0;
}

bool AnimaShader::IsCompiled()
{
	return _compiled;
}

END_ANIMA_ENGINE_NAMESPACE