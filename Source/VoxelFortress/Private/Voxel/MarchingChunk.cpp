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

	int XStart = (chunkNumber % drawDistance) * (chunkSize); //ûũ�� ��ǥ����ִ� ģ��
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


	float cube[8];	//���� ������ ������ ���� �����ϴ� �迭

	//�� ���� ��ȸ
	for (int z = 0; z < chunkSize; z++)
	{
		for (int y = YStart; y < chunkSize + YStart; y++)
		{
			for (int x = XStart; x < chunkSize + XStart; x++)
			{
				for (int i = 0; i < 8; i++)
				{
					//if(x<(chunkSize-1) && y< (chunkSize - 1) && z< (chunkSize - 1))
					cube[i] = (*voxelsPtr)[GetVoxelIndex(x + VertexOffset[i][0], y + VertexOffset[i][1], z + VertexOffset[i][2])]; // �� �������� heightmap���� cube �迭�� �����մϴ�.
				}

				//���� ť�꿡 ���Ͽ� �޽ø� ������.
				March(x, y, z, cube);
			}
		}
	}
}


void AMarchingChunk::March(int x, int y, int z, const float cube[8])
{
	int VertexMask = 0;
	FVector EdgeVertex[12];

	// �� �������� ǥ�� ���� �ִ��� �Ʒ��� �ִ��� Ȯ��.
	for (int i = 0; i < 8; ++i)
	{
		if (cube[i] <= surfaceLevel) VertexMask |= 1 << i;
	}

	// ���� �������� �����Ǵ� �������� ���¸� �����ϴ� �� ���Ǵ� ���̺��� ��ȸ
	const int EdgeMask = CubeEdgeFlags[VertexMask];

	if (EdgeMask == 0) return;

	// ������ ������ Ž��
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
	//�������� �̿��� �ﰢ�� ����
	for (int i = 0; i < 5; i++)
	{
		if (TriangleConnectionTable[VertexMask][3 * i] < 0) break;

		//�ﰢ�� ������ ���
		auto v1 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i]] * cubeSize;
		auto v2 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 1]] * cubeSize;
		auto v3 = EdgeVertex[TriangleConnectionTable[VertexMask][3 * i + 2]] * cubeSize;

		//�ﰢ�� ��� ���
		auto normal = FVector::CrossProduct(v2 - v1, v3 - v1);
		normal.Normalize();

		//�޽õ����Ϳ� �߰�
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
	//3���� �迭�� 1�������� ǥ���ϱ⿡ �� �Լ��� �̿��� �ε����� �˻��մϴ�.
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

