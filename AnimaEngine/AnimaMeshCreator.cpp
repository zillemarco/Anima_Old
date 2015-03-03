//
//  AnimaStage.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaStage.h"
#include "AnimaMeshCreator.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AInt AnimaMeshCreator::_index;

void AnimaMeshCreator::MakePlane(AnimaMesh* mesh, AnimaAllocator* allocator)
{
	mesh->ClearAll();
	
	AInt numeroVertici = 4;
	AInt numeroFacce = 2;
	AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*allocator, numeroVertici);
	AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*allocator, numeroVertici);
	AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*allocator, numeroVertici);
	AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*allocator, numeroFacce, allocator);
	
	vertici[0] = AnimaVertex3f(-1.0f, 0.0f, -1.0f);
	vertici[1] = AnimaVertex3f(-1.0f, 0.0f, 1.0f);
	vertici[2] = AnimaVertex3f(1.0f, 0.0f, 1.0f);
	vertici[3] = AnimaVertex3f(1.0f, 0.0f, -1.0f);
	
	normali[0] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	normali[1] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	normali[2] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	normali[3] = AnimaVertex3f(0.0f, 1.0f, 0.0f);
	
	textCoords[0] = AnimaVertex2f(1.0f, 0.0f);
	textCoords[1] = AnimaVertex2f(1.0f, 1.0f);
	textCoords[2] = AnimaVertex2f(0.0f, 1.0f);
	textCoords[3] = AnimaVertex2f(0.0f, 0.0f);
	
	AUint indexes0[] = { 0, 1, 2 };
	AUint indexes1[] = { 0, 2, 3 };
	facce[0].SetIndexes(indexes0, 3);
	facce[1].SetIndexes(indexes1, 3);
	
	mesh->SetVertices(vertici, numeroVertici);
	mesh->SetNormals(normali, numeroVertici);
	mesh->SetTextureCoords(textCoords, numeroVertici);
	mesh->SetFaces(facce, numeroFacce);
	
	AnimaAllocatorNamespace::DeallocateArray(*allocator, vertici);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, normali);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, textCoords);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, facce);
}

void AnimaMeshCreator::MakeIcosahedralSphere(AnimaMesh* mesh, AUint recursionLevel, AnimaAllocator* allocator)
{
	mesh->ClearAll();
	
	std::vector<AnimaVertex3f> vertici;
	std::vector<AnimaFace*> facce;
	std::map<__int64, AInt> middlePointIndexCache;
	_index = 0;
	
	float t = (1.0f + sqrtf(5.0f)) / 2.0f;
	
	AddVertex(AnimaVertex3f(-1.0f, t, 0.0f), vertici);
	AddVertex(AnimaVertex3f(1.0f, t, 0.0f), vertici);
	AddVertex(AnimaVertex3f(-1.0f, -t, 0.0f), vertici);
	AddVertex(AnimaVertex3f(1.0f, -t, 0.0f), vertici);
	
	AddVertex(AnimaVertex3f(0.0f, -1.0f, t), vertici);
	AddVertex(AnimaVertex3f(0.0f, 1.0f, t), vertici);
	AddVertex(AnimaVertex3f(0.0f, -1.0f, -t), vertici);
	AddVertex(AnimaVertex3f(0.0f, 1.0f, -t), vertici);
	
	AddVertex(AnimaVertex3f(t, 0.0f, -1.0f), vertici);
	AddVertex(AnimaVertex3f(t, 0.0f, 1.0f), vertici);
	AddVertex(AnimaVertex3f(-t, 0.0f, -1.0f), vertici);
	AddVertex(AnimaVertex3f(-t, 0.0f, 1.0f), vertici);
	
	AddFace(facce, 0, 11, 5, allocator);
	AddFace(facce, 0, 5, 1, allocator);
	AddFace(facce, 0, 1, 7, allocator);
	AddFace(facce, 0, 7, 10, allocator);
	AddFace(facce, 0, 10, 11, allocator);
	
	AddFace(facce, 1, 5, 9, allocator);
	AddFace(facce, 5, 11, 4, allocator);
	AddFace(facce, 11, 10, 2, allocator);
	AddFace(facce, 10, 7, 6, allocator);
	AddFace(facce, 7, 1, 8, allocator);
	
	AddFace(facce, 3, 9, 4, allocator);
	AddFace(facce, 3, 4, 2, allocator);
	AddFace(facce, 3, 2, 6, allocator);
	AddFace(facce, 3, 6, 8, allocator);
	AddFace(facce, 3, 8, 9, allocator);
	
	AddFace(facce, 4, 9, 5, allocator);
	AddFace(facce, 2, 4, 11, allocator);
	AddFace(facce, 6, 2, 10, allocator);
	AddFace(facce, 8, 6, 7, allocator);
	AddFace(facce, 9, 8, 1, allocator);
	
	for(AUint i = 0; i < recursionLevel; i++)
	{
		std::vector<AnimaFace*> facce2;
		for(auto face : facce)
		{
			AUint* indexes = face->GetIndexes();
			AInt a = GetMiddlePoint(indexes[0], indexes[1], vertici, middlePointIndexCache);
			AInt b = GetMiddlePoint(indexes[1], indexes[2], vertici, middlePointIndexCache);
			AInt c = GetMiddlePoint(indexes[2], indexes[0], vertici, middlePointIndexCache);
			
			AddFace(facce2, indexes[0], a, c, allocator);
			AddFace(facce2, indexes[1], b, a, allocator);
			AddFace(facce2, indexes[2], c, b, allocator);
			AddFace(facce2, a, b, c, allocator);
		}
		
		for(auto faccia : facce)
		{
			AnimaAllocatorNamespace::DeallocateObject(*allocator, faccia);
			faccia = nullptr;
		}
		
		facce.clear();
		facce = facce2;
	}
	
	AnimaVertex3f* meshVertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*allocator, vertici.size());
	AnimaFace* meshFacce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*allocator, facce.size(), allocator);
	
	AInt offset = 0;
	for(auto vertice : vertici)
		meshVertici[offset++] = vertice;
	
	offset = 0;
	for(auto faccia : facce)
	{
		meshFacce[offset++].SetIndexes(faccia->GetIndexes(), faccia->GetIndexesCount());
		AnimaAllocatorNamespace::DeallocateObject(*allocator, faccia);
		faccia = nullptr;
	}
	
	mesh->SetVertices(meshVertici, vertici.size());
	mesh->SetFaces(meshFacce, facce.size());
	
	vertici.clear();
	facce.clear();
	AnimaAllocatorNamespace::DeallocateArray(*allocator, meshVertici);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, meshFacce);
}

AInt AnimaMeshCreator::GetMiddlePoint(AInt p1, AInt p2, std::vector<AnimaVertex3f>& vertices, std::map<__int64, int>& cache)
{
	bool firstIsSmaller = p1 < p2;
	__int64 smallerIndex = firstIsSmaller ? p1 : p2;
	__int64 greaterIndex = firstIsSmaller ? p2 : p1;
	__int64 key = (smallerIndex << 32) + greaterIndex;
	
	auto ret = cache.find(key);
	if(ret != cache.end())
		return ret->second;
	
	AnimaVertex3f point1 = vertices[p1];
	AnimaVertex3f point2 = vertices[p2];
	AnimaVertex3f middle = (point1 + point2) * 0.5f;
	
	AInt i = AddVertex(middle, vertices);
	cache[key] = i;
	
	return i;
}

AInt AnimaMeshCreator::AddVertex(AnimaVertex3f vertex, std::vector<AnimaVertex3f>& vertices)
{
	vertices.push_back(vertex.Normalized());
	return _index++;
}

void AnimaMeshCreator::AddFace(std::vector<AnimaFace*>& facce, AInt v1, AInt v2, AInt v3, AnimaAllocator* allocator)
{
	AnimaFace* face = AnimaAllocatorNamespace::AllocateNew<AnimaFace>(*allocator, allocator);
	face->AddIndex(v1);
	face->AddIndex(v2);
	face->AddIndex(v3);
	
	facce.push_back(face);
}

AnimaMesh* AnimaMeshCreator::CreatePlane(AnimaAllocator* allocator)
{
	AnimaMesh* mesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*allocator, allocator);
	MakePlane(mesh, allocator);	
	return mesh;
}

AnimaMesh* AnimaMeshCreator::CreateIcosahedralSphere(AUint recursionLevel, AnimaAllocator* allocator)
{
	AnimaMesh* mesh = AnimaAllocatorNamespace::AllocateNew<AnimaMesh>(*allocator, allocator);
	MakeIcosahedralSphere(mesh, recursionLevel, allocator);
	return mesh;
}

END_ANIMA_ENGINE_NAMESPACE
