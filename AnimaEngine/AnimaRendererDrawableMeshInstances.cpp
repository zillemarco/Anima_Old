//
//  AnimaRendererDrawableMeshInstances.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaRendererDrawableMeshInstances.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRendererDrawableMeshInstances::AnimaRendererDrawableMeshInstances()
{
	_program = nullptr;
}

AnimaRendererDrawableMeshInstances::~AnimaRendererDrawableMeshInstances()
{
}

AnimaRendererDrawableMeshInstances::AnimaRendererDrawableMeshInstances(const AnimaRendererDrawableMeshInstances& src)
{
	if (this != &src)
	{
		_program = src._program;
		_meshesInstances = src._meshesInstances;
	}
}

AnimaRendererDrawableMeshInstances::AnimaRendererDrawableMeshInstances(const AnimaRendererDrawableMeshInstances&& src)
{
	if (this != &src)
	{
		_program = src._program;
		_meshesInstances = src._meshesInstances;
	}
}

AnimaRendererDrawableMeshInstances& AnimaRendererDrawableMeshInstances::operator=(const AnimaRendererDrawableMeshInstances& src)
{
	if (this != &src)
	{
		_program = src._program;
		_meshesInstances = src._meshesInstances;
	}

	return *this;
}

AnimaRendererDrawableMeshInstances& AnimaRendererDrawableMeshInstances::operator=(AnimaRendererDrawableMeshInstances&& src)
{
	if (this != &src)
	{
		_program = src._program;
		_meshesInstances = src._meshesInstances;
	}

	return *this;
}

void AnimaRendererDrawableMeshInstances::SetShaderProgram(AnimaShaderProgram* program)
{
	_program = program;
}

AnimaShaderProgram* AnimaRendererDrawableMeshInstances::GetShaderProgram()
{
	return _program;
}

void AnimaRendererDrawableMeshInstances::AddMeshInstance(AnimaMeshInstance* instance)
{
	_meshesInstances.push_back(instance);
}

AnimaArray<AnimaMeshInstance*>* AnimaRendererDrawableMeshInstances::GetMeshesInstances()
{
	return &_meshesInstances;
}

END_ANIMA_ENGINE_NAMESPACE
