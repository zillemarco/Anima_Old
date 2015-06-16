#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaShader
{
public:
	enum AnimaShaderType {
		INVALID = -1,
		VERTEX = GL_VERTEX_SHADER,
		TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
		TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER
	};

public:
	AnimaShader(AnimaAllocator* allocator);
	AnimaShader(AnimaAllocator* allocator, AnimaString shaderText, AnimaShaderType type);
	AnimaShader(AnimaAllocator* allocator, const char* shaderText, AnimaShaderType type);
	AnimaShader(const AnimaShader& src);
	AnimaShader(AnimaShader&& src);
	~AnimaShader();
	
	AnimaShader& operator=(const AnimaShader& src);
	AnimaShader& operator=(AnimaShader&& src);

	bool operator==(const AnimaShader& left);
	bool operator!=(const AnimaShader& left);

public:	
	void SetText(AnimaString text);
	void SetText(const char* text);
	AnimaString GetAnimaText();
	const char* GetText();

	void SetType(AnimaShaderType type);
	AnimaShaderType GetType();

	bool Create();
	bool Delete();
	bool Compile();

	bool IsCreated();
	bool IsCompiled();

	AInt GetID();

private:
	AnimaAllocator*	_allocator;
	AnimaShaderType _type;
	AnimaString		_text;

	AInt			_id;
	bool			_compiled;
};

END_ANIMA_ENGINE_NAMESPACE