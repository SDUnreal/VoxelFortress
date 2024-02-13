// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MarchingChunk.h"
#include "../Util/VoxelDataSturct.h"

#include "GameFramework/Actor.h"
#include "ChunkWorld.generated.h"


UCLASS()
class AChunkWorld : public AActor
{

	GENERATED_BODY()

private:
	enum EAxis
	{
		XAxis = 0,
		YAxis = 1,
		ZAxis = 2
	};

public:
	// Sets default values for this actor's properties
	AChunkWorld();

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	TSubclassOf<AMarchingChunk> Chunk;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int DrawDistance = 5;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int ChunkSize = 64;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	int CubeSize = 100;

	UPROPERTY(EditAnywhere, Category = "ChunkWorld")
	float zPosition = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChunkWorld")
	FVector TargetVertex = FVector(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "editer")
	int targetSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "editer")
	int targetDepth = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "editer")
	int targetAxis = XAxis;

private:

	TArray<int> voxels; // �ʵ�����
	TArray<AMarchingChunk*> chunks;
	int chunkArrSize = 0;
	FVoxelDataSturct data;
	TArray<bool> updatedVoxels;

	// ����Ž���� ���� �����͵�
	TArray<FString> directories;
	FString directory;
	bool IsSelect;

	int targetVertexColorCount = 1;
	int debugDrawCount = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ChunkWorld")
	void BuildWorld();

	UFUNCTION(BlueprintCallable, Category = "ChunkWorld")
	void DrawVertex(float LifeTime);

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void MakeCrater(int Size, const FVector Location);

	void SetVoxels(const TArray<int>& Voxels);

	UFUNCTION(BlueprintCallable, Category = "ChunkWorld")
	const TArray<AMarchingChunk*> GetChunks() { return chunks; }

	int GetVoxelIndex(int x, int y, int z) const;

	int GetUpdatedVoxelIndex(int x, int y) const;

	UFUNCTION(BlueprintCallable, Category = "editor")
	void LoadMaps();
};

// UI -> ��Ÿ�ӿ��� �����°� �������? ��� ���� UI�� ���� ��������

// Enhanced Input
// target vertex �����̴�
// rf : �̵�? bool 
// space bar : toggle bool  
// ���� �ҷ������ -> ���콺�� UI Ŭ���ؼ� ����ϰų� bool
// ī�޶� ����?  wasd camera���� �������浹 -> �׽�Ʈ �ѹ� �غ��� -> �ٸ� ��� ������� �������� ��������Ҷ� ����.. ���⼭ ���� ��������Ű����ؿ�
// gizmo ���׶��� ���콺��  ī�޶󿡼� 



