//
//  AnimaRendererDrawableGeometryInstances.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaRendererDrawableGeometryInstances__
#define __Anima__AnimaRendererDrawableGeometryInstances__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaGeometryInstance.h"
#include "AnimaShaderProgram.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaRendererDrawableGeometryInstances
{
public:
	AnimaRendererDrawableGeometryInstances();
	~AnimaRendererDrawableGeometryInstances();

	AnimaRendererDrawableGeometryInstances(const AnimaRendererDrawableGeometryInstances& src);
	AnimaRendererDrawableGeometryInstances(const AnimaRendererDrawableGeometryInstances&& src);

	AnimaRendererDrawableGeometryInstances& operator=(const AnimaRendererDrawableGeometryInstances& src);
	AnimaRendererDrawableGeometryInstances& operator=(AnimaRendererDrawableGeometryInstances&& src);

public:
	void SetShaderProgram(AnimaShaderProgram* program);
	AnimaShaderProgram* GetShaderProgram();

	void AddGeometryInstance(AnimaGeometryInstance* instance);
	AnimaArray<AnimaGeometryInstance*>* GetGeometriesInstances();

protected:
	AnimaArray<AnimaGeometryInstance*>	_geometriesInstances;
	AnimaShaderProgram*				_program;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaRendererDrawableGeometryInstances__) */
