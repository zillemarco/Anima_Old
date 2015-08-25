//
//  AnimaRendererDrawableMesh.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaRendererDrawableMesh.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaRendererDrawableMesh::AnimaRendererDrawableMesh()
{
	_mesh = nullptr;
}

AnimaRendererDrawableMesh::~AnimaRendererDrawableMesh()
{
}

AnimaRendererDrawableMesh::AnimaRendererDrawableMesh(const AnimaRendererDrawableMesh& src)
{
	if (this != &src)
	{
		_mesh = src._mesh;
		_drawableMeshesInstances = src._drawableMeshesInstances;
	}
}

AnimaRendererDrawableMesh::AnimaRendererDrawableMesh(const AnimaRendererDrawableMesh&& src)
{
	if (this != &src)
	{
		_mesh = src._mesh;
		_drawableMeshesInstances = src._drawableMeshesInstances;
	}
}

AnimaRendererDrawableMesh& AnimaRendererDrawableMesh::operator=(const AnimaRendererDrawableMesh& src)
{
	if (this != &src)
	{
		_mesh = src._mesh;
		_drawableMeshesInstances = src._drawableMeshesInstances;
	}

	return *this;
}

AnimaRendererDrawableMesh& AnimaRendererDrawableMesh::operator=(AnimaRendererDrawableMesh&& src)
{
	if (this != &src)
	{
		_mesh = src._mesh;
		_drawableMeshesInstances = src._drawableMeshesInstances;
	}

	return *this;
}

AnimaArray<AnimaRendererDrawableMeshInstances>* AnimaRendererDrawableMesh::GetDrawableMeshInstances()
{
	return &_drawableMeshesInstances;
}

void AnimaRendererDrawableMesh::SetMesh(AnimaMesh* mesh)
{
	_mesh = mesh;
}

AnimaMesh* AnimaRendererDrawableMesh::GetMesh()
{
	return _mesh;
}

END_ANIMA_ENGINE_NAMESPACE
