//
//  AnimaScene.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaScene.h"
#include "AnimaMeshCreator.h"
#include "AnimaVertex.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

void AnimaMeshCreator::MakePlane(AnimaMesh* mesh, AnimaAllocator* allocator)
{
	mesh->ClearAll();
	
	AInt numeroVertici = 4;
	AInt numeroFacce = 2;
	AnimaVertex3f* vertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*allocator, numeroVertici);
	AnimaVertex3f* normali = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*allocator, numeroVertici);
	AnimaVertex2f* textCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*allocator, numeroVertici);
	AnimaFace* facce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*allocator, numeroFacce);
	
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
	
	facce[0].SetIndexes(0, 1, 2);
	facce[1].SetIndexes(0, 2, 3);
	
	mesh->SetVertices(vertici, numeroVertici);
	mesh->SetNormals(normali, numeroVertici);
	mesh->SetTextureCoords(textCoords, numeroVertici);
	mesh->SetFaces(facce, numeroFacce);
	
	AnimaAllocatorNamespace::DeallocateArray(*allocator, vertici);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, normali);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, textCoords);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, facce);
}

void AnimaMeshCreator::MakeCube(AnimaMesh* mesh, AnimaAllocator* allocator)
{
	mesh->ClearAll();

	AnimaArray<AnimaVertex3f> vertici;
	AnimaArray<AnimaVertex3f> normali;
	AnimaArray<AnimaVertex2f> textCoords;
	AnimaArray<AnimaFace> facce;

	// Faccia dietro
	vertici.push_back(AnimaVertex3f( 1.0f,  1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f, -1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, -1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f,  1.0f, -1.0f));

	normali.push_back(AnimaVertex3f(0.0f, 0.0f, -1.0f));
	normali.push_back(AnimaVertex3f(0.0f, 0.0f, -1.0f));
	normali.push_back(AnimaVertex3f(0.0f, 0.0f, -1.0f));
	normali.push_back(AnimaVertex3f(0.0f, 0.0f, -1.0f));
	
	textCoords.push_back(AnimaVertex2f(1.0f, 0.0f));
	textCoords.push_back(AnimaVertex2f(1.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 0.0f));

	facce.push_back(AnimaFace(0, 1, 2));
	facce.push_back(AnimaFace(0, 2, 3));

	// Faccia davanti
	vertici.push_back(AnimaVertex3f( 1.0f, -1.0f, 1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f,  1.0f, 1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f,  1.0f, 1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, -1.0f, 1.0f));

	normali.push_back(AnimaVertex3f(0.0f, 0.0f, 1.0f));
	normali.push_back(AnimaVertex3f(0.0f, 0.0f, 1.0f));
	normali.push_back(AnimaVertex3f(0.0f, 0.0f, 1.0f));
	normali.push_back(AnimaVertex3f(0.0f, 0.0f, 1.0f));

	textCoords.push_back(AnimaVertex2f(1.0f, 0.0f));
	textCoords.push_back(AnimaVertex2f(1.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 0.0f));

	facce.push_back(AnimaFace(4, 5, 6));
	facce.push_back(AnimaFace(4, 6, 7));

	// Faccia destra
	vertici.push_back(AnimaVertex3f( 1.0f, -1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f,  1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f,  1.0f,  1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f, -1.0f,  1.0f));

	normali.push_back(AnimaVertex3f(1.0f, 0.0f, 0.0f));
	normali.push_back(AnimaVertex3f(1.0f, 0.0f, 0.0f));
	normali.push_back(AnimaVertex3f(1.0f, 0.0f, 0.0f));
	normali.push_back(AnimaVertex3f(1.0f, 0.0f, 0.0f));

	textCoords.push_back(AnimaVertex2f(1.0f, 0.0f));
	textCoords.push_back(AnimaVertex2f(1.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 0.0f));

	facce.push_back(AnimaFace(8, 9, 10));
	facce.push_back(AnimaFace(8, 10, 11));

	// Faccia sinistra
	vertici.push_back(AnimaVertex3f(-1.0f, -1.0f,  1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f,  1.0f,  1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f,  1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, -1.0f, -1.0f));

	normali.push_back(AnimaVertex3f(-1.0f, 0.0f, 0.0f));
	normali.push_back(AnimaVertex3f(-1.0f, 0.0f, 0.0f));
	normali.push_back(AnimaVertex3f(-1.0f, 0.0f, 0.0f));
	normali.push_back(AnimaVertex3f(-1.0f, 0.0f, 0.0f));

	textCoords.push_back(AnimaVertex2f(1.0f, 0.0f));
	textCoords.push_back(AnimaVertex2f(1.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 0.0f));

	facce.push_back(AnimaFace(12, 13, 14));
	facce.push_back(AnimaFace(12, 14, 15));

	// Faccia sopra
	vertici.push_back(AnimaVertex3f( 1.0f, 1.0f,  1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f, 1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, 1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, 1.0f,  1.0f));

	normali.push_back(AnimaVertex3f(0.0f, 1.0f, 0.0f));
	normali.push_back(AnimaVertex3f(0.0f, 1.0f, 0.0f));
	normali.push_back(AnimaVertex3f(0.0f, 1.0f, 0.0f));
	normali.push_back(AnimaVertex3f(0.0f, 1.0f, 0.0f));

	textCoords.push_back(AnimaVertex2f(1.0f, 0.0f));
	textCoords.push_back(AnimaVertex2f(1.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 0.0f));

	facce.push_back(AnimaFace(16, 17, 18));
	facce.push_back(AnimaFace(16, 18, 19));

	// Faccia sotto
	vertici.push_back(AnimaVertex3f( 1.0f, -1.0f, -1.0f));
	vertici.push_back(AnimaVertex3f( 1.0f, -1.0f,  1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, -1.0f,  1.0f));
	vertici.push_back(AnimaVertex3f(-1.0f, -1.0f, -1.0f));

	normali.push_back(AnimaVertex3f(0.0f, -1.0f, 0.0f));
	normali.push_back(AnimaVertex3f(0.0f, -1.0f, 0.0f));
	normali.push_back(AnimaVertex3f(0.0f, -1.0f, 0.0f));
	normali.push_back(AnimaVertex3f(0.0f, -1.0f, 0.0f));

	textCoords.push_back(AnimaVertex2f(1.0f, 0.0f));
	textCoords.push_back(AnimaVertex2f(1.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 1.0f));
	textCoords.push_back(AnimaVertex2f(0.0f, 0.0f));

	facce.push_back(AnimaFace(20, 21, 22));
	facce.push_back(AnimaFace(20, 22, 23));

	mesh->SetVertices(vertici);
	mesh->SetNormals(normali);
	mesh->SetTextureCoords(textCoords);
	mesh->SetFaces(facce);
}

void AnimaMeshCreator::MakeCylinder(AnimaMesh* mesh, AFloat topRadius, AFloat bottomRadius, AFloat height, AUint radialSegments, AUint heightSegments, bool openEnded, AnimaAllocator* allocator)
{
	mesh->ClearAll();

	AFloat thetaStart = 0.0f;
	AFloat thetaLength = M_2PI;
	
	AFloat offsetX = 0.0f;
	AFloat offsetY = -height / 2.0f;
	AFloat offsetZ = 0.0f;

	AFloat heightHalf = height / 2.0f;
	std::vector<AnimaVertex3f> vertici;
	std::vector<AnimaFace*> facce;

	std::vector<std::vector<ASizeT> > righeVertici;

	for (AUint y = 0; y <= heightSegments; y++)
	{
		std::vector<ASizeT> rigaVertici;
		AFloat v = (AFloat)y / (AFloat)heightSegments;
		AFloat radius = v * (bottomRadius - topRadius) + topRadius;

		for (AUint x = 0; x <= radialSegments; x++)
		{
			AFloat u = (AFloat)x / (AFloat)radialSegments;
			AnimaVertex3f vertice;
			vertice.x = radius * sinf(u * thetaLength + thetaStart) + offsetX;
			vertice.y = -v * height + heightHalf + offsetY;
			vertice.z = radius * cosf(u * thetaLength + thetaStart) + offsetZ;

			vertici.push_back(vertice);
			rigaVertici.push_back(vertici.size() - 1);
		}

		righeVertici.push_back(rigaVertici);
	}

//	AFloat tanTheta = ((AFloat)bottomRadius - (AFloat)topRadius) / (AFloat)height;
	AnimaVertex3f na, nb;

	for (AUint x = 0; x < radialSegments; x++)
	{
		for (AUint y = 0; y < heightSegments; y++)
		{
			AUint v1 = (AUint)righeVertici[y][x];
			AUint v2 = (AUint)righeVertici[y + 1][x];
			AUint v3 = (AUint)righeVertici[y + 1][x + 1];
			AUint v4 = (AUint)righeVertici[y][x + 1];

			AnimaFace* face1 = AnimaAllocatorNamespace::AllocateNew<AnimaFace>(*allocator);
			face1->SetIndexes(v1, v2, v4);

			AnimaFace* face2 = AnimaAllocatorNamespace::AllocateNew<AnimaFace>(*allocator);
			face2->SetIndexes(v2, v3, v4);

			facce.push_back(face1);
			facce.push_back(face2);
		}
	}

	if (openEnded == false && topRadius > 0.0f)
	{
		vertici.push_back(AnimaVertex3f(0.0f + offsetX, heightHalf + offsetY, 0.0f + offsetZ));

		for (AUint x = 0; x < radialSegments; x++)
		{
			AUint v1 = (AUint)righeVertici[0][x];
			AUint v2 = (AUint)righeVertici[0][x + 1];
			AUint v3 = (AUint)vertici.size() - 1;

			AnimaFace* face = AnimaAllocatorNamespace::AllocateNew<AnimaFace>(*allocator);
			face->SetIndexes(v1, v2, v3);

			facce.push_back(face);
		}
	}

	if (openEnded == false && bottomRadius > 0.0f)
	{
		vertici.push_back(AnimaVertex3f(0.0f + offsetX, -heightHalf + offsetY, 0.0f + offsetZ));

		for (AUint x = 0; x < radialSegments; x++)
		{
			AUint v1 = (AUint)righeVertici[heightSegments][x + 1];
			AUint v2 = (AUint)righeVertici[heightSegments][x];
			AUint v3 = (AUint)vertici.size() - 1;

			AnimaFace* face = AnimaAllocatorNamespace::AllocateNew<AnimaFace>(*allocator);
			face->SetIndexes(v1, v2, v3);

			facce.push_back(face);
		}
	}

	AnimaVertex3f* meshVertici = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*allocator, vertici.size());
	AnimaFace* meshFacce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*allocator, facce.size());

	AInt offset = 0;
	for (auto vertice : vertici)
		meshVertici[offset++] = vertice;

	offset = 0;
	for (auto faccia : facce)
	{
		meshFacce[offset++].SetIndexes(faccia->GetIndexes());
		AnimaAllocatorNamespace::DeallocateObject(*allocator, faccia);
		faccia = nullptr;
	}

	mesh->SetVertices(meshVertici, (AInt)vertici.size());
	mesh->SetFaces(meshFacce, (AInt)facce.size());

	vertici.clear();
	facce.clear();
	AnimaAllocatorNamespace::DeallocateArray(*allocator, meshVertici);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, meshFacce);
}

void AnimaMeshCreator::MakeIcosahedralSphere(AnimaMesh* mesh, AUint recursionLevel, AnimaAllocator* allocator)
{
	mesh->ClearAll();
	
	std::vector<AnimaVertex3f> vertici;
	std::vector<AnimaFace*> facce;
	std::map<__int64, AInt> middlePointIndexCache;
	AInt index = 0;
	
	float t = (1.0f + sqrtf(5.0f)) / 2.0f;
	
	AddVertex(AnimaVertex3f(-1.0f, t, 0.0f), vertici, index);
	AddVertex(AnimaVertex3f(1.0f, t, 0.0f), vertici, index);
	AddVertex(AnimaVertex3f(-1.0f, -t, 0.0f), vertici, index);
	AddVertex(AnimaVertex3f(1.0f, -t, 0.0f), vertici, index);
	
	AddVertex(AnimaVertex3f(0.0f, -1.0f, t), vertici, index);
	AddVertex(AnimaVertex3f(0.0f, 1.0f, t), vertici, index);
	AddVertex(AnimaVertex3f(0.0f, -1.0f, -t), vertici, index);
	AddVertex(AnimaVertex3f(0.0f, 1.0f, -t), vertici, index);
	
	AddVertex(AnimaVertex3f(t, 0.0f, -1.0f), vertici, index);
	AddVertex(AnimaVertex3f(t, 0.0f, 1.0f), vertici, index);
	AddVertex(AnimaVertex3f(-t, 0.0f, -1.0f), vertici, index);
	AddVertex(AnimaVertex3f(-t, 0.0f, 1.0f), vertici, index);
	
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
			AInt a = GetMiddlePoint(indexes[0], indexes[1], vertici, middlePointIndexCache, index);
			AInt b = GetMiddlePoint(indexes[1], indexes[2], vertici, middlePointIndexCache, index);
			AInt c = GetMiddlePoint(indexes[2], indexes[0], vertici, middlePointIndexCache, index);
			
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
	AnimaFace* meshFacce = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*allocator, facce.size());
	
	AInt offset = 0;
	for(auto vertice : vertici)
		meshVertici[offset++] = vertice;
	
	offset = 0;
	for(auto faccia : facce)
	{
		meshFacce[offset++].SetIndexes(faccia->GetIndexes());
		AnimaAllocatorNamespace::DeallocateObject(*allocator, faccia);
		faccia = nullptr;
	}
	
	mesh->SetVertices(meshVertici, (AInt)vertici.size());
	mesh->SetFaces(meshFacce, (AInt)facce.size());
	
	vertici.clear();
	facce.clear();
	AnimaAllocatorNamespace::DeallocateArray(*allocator, meshVertici);
	AnimaAllocatorNamespace::DeallocateArray(*allocator, meshFacce);
}

AInt AnimaMeshCreator::GetMiddlePoint(AInt p1, AInt p2, std::vector<AnimaVertex3f>& vertices, std::map<__int64, int>& cache, AInt& index)
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
	
	AInt i = AddVertex(middle, vertices, index);
	cache[key] = i;
	
	return i;
}

AInt AnimaMeshCreator::AddVertex(AnimaVertex3f vertex, std::vector<AnimaVertex3f>& vertices, AInt& index)
{
	vertices.push_back(vertex.Normalized());
	return index++;
}

void AnimaMeshCreator::AddFace(std::vector<AnimaFace*>& facce, AInt v1, AInt v2, AInt v3, AnimaAllocator* allocator)
{
	AnimaFace* face = AnimaAllocatorNamespace::AllocateNew<AnimaFace>(*allocator);
	face->SetIndexes(v1, v2, v3);
	
	facce.push_back(face);
}

END_ANIMA_ENGINE_NAMESPACE
