// Fill out your copyright notice in the Description page of Project Settings.

#include "MarchingChunk.h"

AMarchingChunk::AMarchingChunk()
{
	voxels.SetNum((chunkSize + 1) * (chunkSize + 1) * (chunkSize + 1));
}

void AMarchingChunk::GenerateHeightMap()
{
	const auto position = GetActorLocation() / 100;

	/*
	for (int x = 0; x <= size; x++)
	{
		for (int y = 0; y <= size; y++)
		{
			for (int z = 0; z <= size; z++)
			{
				//voxels[GetVoxelIndex(x, y, z)] = noise->GetNoise(x + position.X, y + position.Y, z + position.Z);
				if (x == 0 || x == size - 1 || y == 0 || y == size - 1 || z == 0 || z == size - 1)
				{
					// �ܰ��� ����
					voxels[GetVoxelIndex(x, y, z)] = 1.0f;
				}
				else
				{
					// ������ ����
					voxels[GetVoxelIndex(x, y, z)] = -1.0f;
				}
			}
		}
	}
	*/
}

void AMarchingChunk::GenerateMesh()
{
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
	for (int x = 0; x < chunkSize; x++)
	{
		for (int y = 0; y < chunkSize; y++)
		{
			for (int z = 0; z < chunkSize; z++)
			{
				for (int i = 0; i < 8; i++)
				{
					cube[i] = voxels[GetVoxelIndex(x + VertexOffset[i][0], y + VertexOffset[i][1], z + VertexOffset[i][2])]; // �� �������� heightmap���� cube �迭�� �����մϴ�.
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
	return z * (chunkSize + 1) * (chunkSize + 1) + y * (chunkSize + 1) + x;
}

float AMarchingChunk::GetInterPolationOffset(float V1, float V2) const
{
	const float delta = V2 - V1;
	return delta == 0.0f ? surfaceLevel : (surfaceLevel - V1) / delta;
}

