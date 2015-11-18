//
//  AnimaGeometryCreator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaGeometryCreator__
#define __Anima__AnimaGeometryCreator__

#include "AnimaEngine.h"
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaGeometry.h"
#include "AnimaAllocators.h"
#include "AnimaFace.h"
#include <map>

#ifndef _WIN32
#include <inttypes.h>
#define __int64 int64_t
#endif

#include <stdint.h>

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaGeometryCreator
{
public:
	static void MakePlane(AnimaGeometry* geometry, AnimaAllocator* allocator);
	static void MakeCube(AnimaGeometry* geometry, AnimaAllocator* allocator);
	static void MakeCylinder(AnimaGeometry* geometry, AFloat topRadius, AFloat bottomRadius, AFloat height, AUint radialSegments, AUint heightSegments, bool openEnded, AnimaAllocator* allocator);
	static void MakeIcosahedralSphere(AnimaGeometry* geometry, AUint recursionLevel, AnimaAllocator* allocator);
		
private:
	static AInt AddVertex(AnimaVertex3f vertex, std::vector<AnimaVertex3f>& vertices, AInt& index);
	static void AddFace(std::vector<AnimaFace*>& facce, AInt v1, AInt v2, AInt v3, AnimaAllocator* allocator);
	static AInt GetMiddlePoint(AInt p1, AInt p2, std::vector<AnimaVertex3f>& vertices, std::map<__int64, int>& cache, AInt& index);
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaGeometryCreator__) */
