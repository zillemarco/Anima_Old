//
//  AnimaRendererDrawableGeometry.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaRendererDrawableGeometry__
#define __Anima__AnimaRendererDrawableGeometry__

#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaEngine.h"
#include "AnimaGeometry.h"
#include "AnimaRendererDrawableGeometryInstances.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class AnimaRendererDrawableGeometry
{
public:
	AnimaRendererDrawableGeometry();
	~AnimaRendererDrawableGeometry();

	AnimaRendererDrawableGeometry(const AnimaRendererDrawableGeometry& src);
	AnimaRendererDrawableGeometry(const AnimaRendererDrawableGeometry&& src);

	AnimaRendererDrawableGeometry& operator=(const AnimaRendererDrawableGeometry& src);
	AnimaRendererDrawableGeometry& operator=(AnimaRendererDrawableGeometry&& src);

public:
	void SetGeometry(AnimaGeometry* geometry);
	AnimaGeometry* GetGeometry();

	AnimaArray<AnimaRendererDrawableGeometryInstances>* GetDrawableGeometryInstances();

protected:
	AnimaArray<AnimaRendererDrawableGeometryInstances>	_drawableGeometriesInstances;
	AnimaGeometry*										_geometry;
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaRendererDrawableGeometry__) */
