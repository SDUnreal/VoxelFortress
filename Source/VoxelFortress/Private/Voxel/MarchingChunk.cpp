// Fill out your copyright notice in the Description page of Project Settings.

#include "MarchingChunk.h"

AMarchingChunk::AMarchingChunk()
{
}

void AMarchingChunk::GenerateHeightMap()
{
}

void AMarchingChunk::GenerateMesh()
{

	int XStart = (chunkNumber % drawDistance) * (chunkSize); //청크별 좌표잡아주는 친구
	int YStart = (chunkNumber / drawDistance) * (chunkSize); //

	if (surfaceLevel > 0.0f)
	{
		triangleOrder[0] = 0;
		triangleOrder[1] = 1;
		triangleOrder[2] = 2;
	}
	else
	{
		triangleOrder[0] = 2;
		triangleOrder[1] = 1;
		triangleOrder[2] = 0;
	}


	float cube[8];	//현재 복셀의 꼭짓점 값을 저장하는 배열

	//각 복셀 순회
	for (int z = 0; z < chunkSize; z++)
	{
		for (int y = YStart; y < chunkSize + YStart; y++)
		{
			for (int x = XStart; x < chunkSize + XStart; x++)
			{
				for (int i = 0; i < 8; i++)
				{
					//if(x<(chunkSize-1) && y< (chunkSize - 1) && z< (chunkSize - 1))
					cube[i] = (*voxelsPtr)[GetVoxelIndex(x + VertexOffset[i][0], y + VertexOffset[i][1], z + VertexOffset[i][2])]; // 각 꼭짓점의 heightmap값을 cube 배열에 저장합니다.
				}

				//현재 큐브에 대하여 메시를 생성함.
				March(x, y, z, cube);
			}
		}
	}
}


void AMarchingChunk::March(int x, int y, int z, const float cube[8])
{
	int VertexMask = 0;
	FVector EdgeVertex[12];

	// 각 꼭짓점이 표면 위에 있는지 아래에 있는지 확인.
	for (int i = 0; i < 8; ++i)
	{
		if (cube[i] <= surfaceLevel) VertexMask |= 1 << i;
	}

	// 현재 복셀에서 생성되는 폴리곤의 형태를 결정하는 데 사용되는 테이블을 조회
	const int EdgeMask = CubeEdgeFlags[VertexMask];

	if (EdgeMask == 0) return;

	// 엣지의 교차점 탐색
	for (int i = 0; i < 12; ++i)
	{
		if ((EdgeMask & 1 << i) != 0)
		{
			const float Offset = interpolation ? GetInterPolationOffset(cube[EdgeConnection[i][0]], cube[EdgeConnection[i][1]]) : 0.5f;

			EdgeVertex[i].X = x + (VertexOffset[EdgeConnection[i][0]][0] + Offset * EdgeDirection[i][0]);
			EdgeVertex[i].Y = y + (VertexOffset[EdgeConnection[i][0]][1] + Offset * EdgeDirection[i][1]);
			EdgeVertex[i].Z = z + (VertexOffset[EdgeConnection[i][0]][2] + Offset * EdgeDirection[i][2]);
		}
	}
	//쿄자점을 이용해 삼각형 생성
	for (int i = 0; i < 5; i++)
	{
		if (TriangleConnectionTable[VertexMask][3 * i] < 0) break;

		//삼각형 꼭짓점 계산
		auto v1 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i]] * cubeSize;
		auto v2 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 1]] * cubeSize;
		auto v3 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 2]] * cubeSize;

		//삼각형 노멀 계산
		auto normal = FVector::CrossProduct(v2 - v1, v3 - v1);
		normal.Normalize();

		//메시데이터에 추가
		meshData.Vertices.Add(v1);
		meshData.Vertices.Add(v2);
		meshData.Vertices.Add(v3);

		meshData.Triangles.Add(vertexCount + triangleOrder[0]);
		meshData.Triangles.Add(vertexCount + triangleOrder[1]);
		meshData.Triangles.Add(vertexCount + triangleOrder[2]);

		meshData.Normals.Add(normal);
		meshData.Normals.Add(normal);
		meshData.Normals.Add(normal);

		vertexCount += 3;
	}
}

int AMarchingChunk::GetVoxelIndex(int x, int y, int z) const
{

	//'return z * (chunkSize) * (chunkSize)+(chunkSize)*y + x;
	//3차원 배열을 1차원으로 표현하기에 이 함수를 이용해 인덱스를 검색합니다.
	return z * (chunkSize) * (chunkSize) * (drawDistance * drawDistance) + y * (chunkSize)*drawDistance + x;
}

float AMarchingChunk::GetInterPolationOffset(float V1, float V2) const
{
	const float delta = V2 - V1;
	return delta == 0.0f ? surfaceLevel : (surfaceLevel - V1) / delta;
}


bool AMarchingChunk::CompareVoxels(TArray<int> Voxels)
{
	int XStart = (chunkNumber % drawDistance) * (chunkSize);
	int YStart = (chunkNumber / drawDistance) * (chunkSize);

	for (int z = 0; z < chunkSize; z++)
	{
		for (int y = YStart; y < chunkSize + YStart; y++)
		{
			for (int x = XStart; x < chunkSize + XStart; x++)
			{
				if ((*voxelsPtr)[GetVoxelIndex(x, y, z)] != Voxels[GetVoxelIndex(x, y, z)])
					return false;
			}
		}
	}
	return true;
}

