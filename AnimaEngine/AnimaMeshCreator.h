//
//  AnimaMeshCreator.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#ifndef __Anima__AnimaMeshCreator__
#define __Anima__AnimaMeshCreator__

#include "AnimaEngine.h"
#include "AnimaEngineCore.h"
#include "AnimaTypes.h"
#include "AnimaMesh.h"
#include "AnimaAllocators.h"
#include "AnimaFace.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMeshCreator
{
public:
	static void MakePlane(AnimaMesh* mesh, AnimaAllocator* allocator);
	static void MakeIcosahedralSphere(AnimaMesh* mesh, AUint recursionLevel, AnimaAllocator* allocator);
	
	static AnimaMesh* CreatePlane(AnimaAllocator* allocator);
	static AnimaMesh* CreateIcosahedralSphere(AUint recursionLevel, AnimaAllocator* allocator);
	
private:
	static void AddVertex(AnimaVertex3f vertex, std::vector<AnimaVertex3f>& vertices, AInt& index);
	static void AddFace(std::vector<AnimaFace*>& facce, AInt v1, AInt v2, AInt v3, AnimaAllocator* allocator);
	static AInt GetMiddlePoint(AInt p1, AInt p2, std::vector<AnimaVertex3f>& vertices, std::map<long, int>& cache, AInt& index);
};

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMeshCreator__) */
