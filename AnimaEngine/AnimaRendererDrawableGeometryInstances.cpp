//
//  AnimaRendererDrawableGeometryInstances.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaRendererDrawableGeometryInstances.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRendererDrawableGeometryInstances::AnimaRendererDrawableGeometryInstances()
{
	_program = nullptr;
}

AnimaRendererDrawableGeometryInstances::~AnimaRendererDrawableGeometryInstances()
{
}

AnimaRendererDrawableGeometryInstances::AnimaRendererDrawableGeometryInstances(const AnimaRendererDrawableGeometryInstances& src)
{
	if (this != &src)
	{
		_program = src._program;
		_geometriesInstances = src._geometriesInstances;
	}
}

AnimaRendererDrawableGeometryInstances::AnimaRendererDrawableGeometryInstances(const AnimaRendererDrawableGeometryInstances&& src)
{
	if (this != &src)
	{
		_program = src._program;
		_geometriesInstances = src._geometriesInstances;
	}
}

AnimaRendererDrawableGeometryInstances& AnimaRendererDrawableGeometryInstances::operator=(const AnimaRendererDrawableGeometryInstances& src)
{
	if (this != &src)
	{
		_program = src._program;
		_geometriesInstances = src._geometriesInstances;
	}

	return *this;
}

AnimaRendererDrawableGeometryInstances& AnimaRendererDrawableGeometryInstances::operator=(AnimaRendererDrawableGeometryInstances&& src)
{
	if (this != &src)
	{
		_program = src._program;
		_geometriesInstances = src._geometriesInstances;
	}

	return *this;
}

void AnimaRendererDrawableGeometryInstances::SetShaderProgram(AnimaShaderProgram* program)
{
	_program = program;
}

AnimaShaderProgram* AnimaRendererDrawableGeometryInstances::GetShaderProgram()
{
	return _program;
}

void AnimaRendererDrawableGeometryInstances::AddGeometryInstance(AnimaGeometryInstance* instance)
{
	_geometriesInstances.push_back(instance);
}

AnimaArray<AnimaGeometryInstance*>* AnimaRendererDrawableGeometryInstances::GetGeometriesInstances()
{
	return &_geometriesInstances;
}

END_ANIMA_ENGINE_NAMESPACE
