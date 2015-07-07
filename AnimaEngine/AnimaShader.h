#pragma once

#include "AnimaEngineCore.h"
#include "AnimaAllocators.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaString.h"
#include "AnimaNamedObject.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

enum AnimaShaderType {
	INVALID = -1,
	VERTEX = GL_VERTEX_SHADER,
	TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
	TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};

class ANIMA_ENGINE_EXPORT AnimaShader : public AnimaNamedObject
{
public:
	AnimaShader(const AnimaString& name, AnimaAllocator* allocator);
	AnimaShader(const AnimaString& name, AnimaAllocator* allocator, AnimaString shaderText, AnimaShaderType type);
	AnimaShader(const AnimaString& name, AnimaAllocator* allocator, const char* shaderText, AnimaShaderType type);
	AnimaShader(const AnimaShader& src);
	AnimaShader(AnimaShader&& src);
	~AnimaShader();
	
	AnimaShader& operator=(const AnimaShader& src);
	AnimaShader& operator=(AnimaShader&& src);

	bool operator==(const AnimaShader& left);
	bool operator!=(const AnimaShader& left);

public:	
	void SetText(AnimaString text);
	AnimaString GetText();

	void SetType(AnimaShaderType type);
	AnimaShaderType GetType();

	bool Create();
	bool Delete();
	bool Compile();

	bool IsCreated();
	bool IsCompiled();

	AInt GetID();

private:
	AnimaShaderType _type;
	AnimaString		_text;

	AInt			_id;
	bool			_compiled;
};

END_ANIMA_ENGINE_NAMESPACE