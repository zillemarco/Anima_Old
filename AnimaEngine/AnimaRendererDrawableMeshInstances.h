//
//  AnimaRendererDrawableMeshInstances.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaRendererDrawableMeshInstances__
#define __Anima__AnimaRendererDrawableMeshInstances__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMeshInstance.h"
#include "AnimaShaderProgram.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaRendererDrawableMeshInstances
{
public:
	AnimaRendererDrawableMeshInstances();
	~AnimaRendererDrawableMeshInstances();

	AnimaRendererDrawableMeshInstances(const AnimaRendererDrawableMeshInstances& src);
	AnimaRendererDrawableMeshInstances(const AnimaRendererDrawableMeshInstances&& src);

	AnimaRendererDrawableMeshInstances& operator=(const AnimaRendererDrawableMeshInstances& src);
	AnimaRendererDrawableMeshInstances& operator=(AnimaRendererDrawableMeshInstances&& src);

public:
	void SetShaderProgram(AnimaShaderProgram* program);
	AnimaShaderProgram* GetShaderProgram();

	void AddMeshInstance(AnimaMeshInstance* instance);
	AnimaArray<AnimaMeshInstance*>* GetMeshesInstances();

protected:
	AnimaArray<AnimaMeshInstance*>	_meshesInstances;
	AnimaShaderProgram*				_program;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaRendererDrawableMeshInstances__) */
