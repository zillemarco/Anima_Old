//
//  AnimaRendererDrawableGeometry.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaRendererDrawableGeometry.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRendererDrawableGeometry::AnimaRendererDrawableGeometry()
{
	_geometry = nullptr;
}

AnimaRendererDrawableGeometry::~AnimaRendererDrawableGeometry()
{
}

AnimaRendererDrawableGeometry::AnimaRendererDrawableGeometry(const AnimaRendererDrawableGeometry& src)
{
	if (this != &src)
	{
		_geometry = src._geometry;
		_drawableGeometriesInstances = src._drawableGeometriesInstances;
	}
}

AnimaRendererDrawableGeometry::AnimaRendererDrawableGeometry(const AnimaRendererDrawableGeometry&& src)
{
	if (this != &src)
	{
		_geometry = src._geometry;
		_drawableGeometriesInstances = src._drawableGeometriesInstances;
	}
}

AnimaRendererDrawableGeometry& AnimaRendererDrawableGeometry::operator=(const AnimaRendererDrawableGeometry& src)
{
	if (this != &src)
	{
		_geometry = src._geometry;
		_drawableGeometriesInstances = src._drawableGeometriesInstances;
	}

	return *this;
}

AnimaRendererDrawableGeometry& AnimaRendererDrawableGeometry::operator=(AnimaRendererDrawableGeometry&& src)
{
	if (this != &src)
	{
		_geometry = src._geometry;
		_drawableGeometriesInstances = src._drawableGeometriesInstances;
	}

	return *this;
}

AnimaArray<AnimaRendererDrawableGeometryInstances>* AnimaRendererDrawableGeometry::GetDrawableGeometryInstances()
{
	return &_drawableGeometriesInstances;
}

void AnimaRendererDrawableGeometry::SetGeometry(AnimaGeometry* geometry)
{
	_geometry = geometry;
}

AnimaGeometry* AnimaRendererDrawableGeometry::GetGeometry()
{
	return _geometry;
}

END_ANIMA_ENGINE_NAMESPACE
