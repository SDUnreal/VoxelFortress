// Fill out your copyright notice in the Description page of Project Settings.


#include "Voxel/ChunkWorld.h"

#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "Templates/SharedPointer.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"


// Sets default values
AChunkWorld::AChunkWorld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	data.MapName = TEXT("DefaultMap");
	data.TestNum = 1;
}

// Called when the game starts or when spawned
void AChunkWorld::BeginPlay()
{
	chunkArrSize = (DrawDistance) * (DrawDistance);
	voxels.Init(-1, chunkArrSize * (ChunkSize + 1) * (ChunkSize + 1) * (ChunkSize + 1));
	for (int i = 0; i < (ChunkSize + 1) * (ChunkSize + 1) * DrawDistance * DrawDistance; i++)
	{
		voxels[i] = 1;
	}
	chunks.Init(0, chunkArrSize);
	updatedVoxels.Init(true, chunkArrSize);

	Super::BeginPlay();
	directory = TEXT("");
	IsSelect = false;
	SetVoxels(voxels);
	data.voxels = voxels;
}

// Called every frame
void AChunkWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunkWorld::BuildWorld()
{
	for (int y = 0; y < DrawDistance; y++)
	{
		for (int x = 0; x < DrawDistance; x++)
		{
			int tempIndex = GetUpdatedVoxelIndex(x, y);
			if (updatedVoxels[tempIndex])
			{
				if (chunks[tempIndex])
				{
					chunks[tempIndex]->Destroy();
				}	
				chunks[tempIndex] = GetWorld()->SpawnActor<class AMarchingChunk>(Chunk, FVector(x, y, zPosition), FRotator::ZeroRotator);
				chunks[tempIndex]->SetChunkNumber(tempIndex);
				chunks[tempIndex]->SetDrawDistance(DrawDistance);
				chunks[tempIndex]->SetChunkSize(ChunkSize);
				chunks[tempIndex]->SetCubeSize(CubeSize);
				chunks[tempIndex]->SetVoxels(voxels);
				chunks[tempIndex]->GenerateTerrian();
				updatedVoxels[tempIndex] = false;
			}		
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("buildfinish"));
}

void AChunkWorld::DrawVertex(float LifeTime)
{
	FColor color;
	float pointSize = 10.0f;
	FVector position;
	FlushPersistentDebugLines(GetWorld());

	for (int x = 0; x < ChunkSize * DrawDistance; x++)
	{
		for (int y = 0; y < ChunkSize * DrawDistance; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				position = FVector(x * CubeSize, y * CubeSize, z * CubeSize);

				if (TargetVertex == FVector(x, y, z) && targetVertexColorCount == 1)
					color = FColor::Red;
				else if (voxels[GetVoxelIndex(x, y, z)] == -1)
					color = FColor::Blue;
				else
					color = FColor::Green;
				DrawDebugPoint(GetWorld(), position, pointSize, color, true, LifeTime);
			}
		}
	}
	targetVertexColorCount *= -1;
}

void AChunkWorld::MakeCrater(int Size, const FVector Location)
{
	FVector Target;
	Target.X = round(Location.X / CubeSize);
	Target.Y = round(Location.Y / CubeSize);
	Target.Z = round(Location.Z / CubeSize);

	UE_LOG(LogTemp, Log, TEXT("Location : %s, TargetLocation : %s"), *Location.ToString(), *Target.ToString());

	int TargetIndex = 0;

	int minX = FMath::Max(0, Target.X - Size / 2);
	int maxX = FMath::Min(ChunkSize * DrawDistance, Target.X + Size / 2);

	int minY = FMath::Max(0, Target.Y - Size / 2);
	int maxY = FMath::Min(ChunkSize * DrawDistance, Target.Y + Size / 2);

	int minZ = FMath::Max(0, Target.Z - Size / 2);
	int maxZ = FMath::Min(ChunkSize * DrawDistance, Target.Z + Size / 2);

	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			for (int z = minZ; z <= maxZ; z++)
			{
				int index = GetVoxelIndex(x, y, z);
				if (voxels[index] == 1)
				{
					voxels[index] = -1;
					updatedVoxels[GetUpdatedVoxelIndex(x, y)] = true;
					UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x , y));
					if (x > maxX && x < ChunkSize * DrawDistance)
					{
						voxels[GetVoxelIndex(x + 1, y, z)] = 1;
						updatedVoxels[GetUpdatedVoxelIndex(x+1, y)] = true;
						UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x + 1, y));
					}
					else if (x < minX && x > 0)
					{
						voxels[GetVoxelIndex(x - 1, y, z)] = 1;
						updatedVoxels[GetUpdatedVoxelIndex(x-1, y)] = true;
						UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x - 1, y));
					}

					if (y > maxY && y < ChunkSize * DrawDistance)
					{
						voxels[GetVoxelIndex(x , y + 1, z)] = 1;
						updatedVoxels[GetUpdatedVoxelIndex(x, y+1)] = true;
						UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x, y+1));
					}
					else if (y < minY && y > 0)
					{
						voxels[GetVoxelIndex(x, y - 1, z)] = 1;
						updatedVoxels[GetUpdatedVoxelIndex(x, y-1)] = true;
						UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x , y-1));
					}

					if (z+1 > maxZ && z+1 < ChunkSize * DrawDistance)
					{
						if (voxels[GetVoxelIndex(x, y, z + 2)] == 1)
						{
							voxels[GetVoxelIndex(x, y, z)] = 1;
							UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x, y));
						}
					}
					else if (z < minZ && z > 0)
					{
						voxels[GetVoxelIndex(x, y, z - 1)] = 1;
						UE_LOG(LogTemp, Log, TEXT("changedIndex : % d"), GetUpdatedVoxelIndex(x, y));
					}
				}				
			}
		}
	}
}

void AChunkWorld::SetVoxels(const TArray<int>& Voxels)
{
	this->voxels = Voxels;
}

int AChunkWorld::GetVoxelIndex(int x, int y, int z) const
{
	return z * (ChunkSize) * (ChunkSize)*DrawDistance * DrawDistance + y * (ChunkSize)*DrawDistance + x;
}

int AChunkWorld::GetUpdatedVoxelIndex(int x, int y) const
{
	return y * DrawDistance + x;
}


void AChunkWorld::LoadMaps()
{

	FString MapsFolder = FPaths::Combine(FPaths::ProjectDir() , TEXT("Maps"));
	FString YourDesiredMap = FPaths::Combine(MapsFolder, TEXT("03.json"));

	directory = YourDesiredMap;
	FString JsonString;
	FFileHelper::LoadFileToString(JsonString, *(directory));
	TSharedRef<TJsonReader<TCHAR>>  JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		data.MapName = JsonObject->GetStringField("mapName");
		data.TestNum = JsonObject->GetIntegerField("testNum");
		TArray<TSharedPtr<FJsonValue>> JsonValueArray = JsonObject->GetArrayField("voxels");

		for (int32 i = 0; i < JsonValueArray.Num(); i++)
		{
			data.voxels[i] = JsonValueArray[i]->AsNumber();
		}
		voxels = data.voxels;
		UE_LOG(LogTemp, Warning, TEXT("SD Unreal : Deserialize success!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SD Unreal : Deserialize failure"));
	}
	directory = TEXT("");
}

