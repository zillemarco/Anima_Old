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
		GEOMETRY = GL_GEOMETRY_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER
	};

public:
	AnimaShader(AnimaEngine* engine);
	AnimaShader(AnimaEngine* engine, AnimaString shaderText, AnimaShaderType type);
	AnimaShader(AnimaEngine* engine, const char* shaderText, AnimaShaderType type);
	AnimaShader(const AnimaShader& src);
	AnimaShader(AnimaShader&& src);
	~AnimaShader();
	
	AnimaShader& operator=(const AnimaShader& src);
	AnimaShader& operator=(AnimaShader&& src);

	inline bool operator==(const AnimaShader& left);
	inline bool operator!=(const AnimaShader& left);

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
	AnimaEngine*	_engine;
	AnimaShaderType _type;
	AnimaString		_text;

	AInt			_id;
	bool			_compiled;
};

END_ANIMA_ENGINE_NAMESPACE