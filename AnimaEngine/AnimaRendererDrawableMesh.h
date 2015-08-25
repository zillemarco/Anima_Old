//
//  AnimaRendererDrawableMesh.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaRendererDrawableMesh__
#define __Anima__AnimaRendererDrawableMesh__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaMesh.h"
#include "AnimaRendererDrawableMeshInstances.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaRendererDrawableMesh
{
public:
	AnimaRendererDrawableMesh();
	~AnimaRendererDrawableMesh();

	AnimaRendererDrawableMesh(const AnimaRendererDrawableMesh& src);
	AnimaRendererDrawableMesh(const AnimaRendererDrawableMesh&& src);

	AnimaRendererDrawableMesh& operator=(const AnimaRendererDrawableMesh& src);
	AnimaRendererDrawableMesh& operator=(AnimaRendererDrawableMesh&& src);

public:
	void SetMesh(AnimaMesh* mesh);
	AnimaMesh* GetMesh();

	AnimaArray<AnimaRendererDrawableMeshInstances>* GetDrawableMeshInstances();

protected:
	AnimaArray<AnimaRendererDrawableMeshInstances>	_drawableMeshesInstances;
	AnimaMesh*										_mesh;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaRendererDrawableMesh__) */
