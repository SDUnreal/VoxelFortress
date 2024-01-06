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
	voxels.Init(-1, (ChunkSize + 1) * (ChunkSize + 1) * (ChunkSize + 1));
	chunks.Init(nullptr, (2 * DrawDistance) * (2 * DrawDistance));

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
	int i = 0;
	if (chunks[i])
	{
		chunks[i]->Destroy();
	}
	chunks[i] = GetWorld()->SpawnActor<class AMarchingChunk>(Chunk, FVector(0, 0, zPosition), FRotator::ZeroRotator);
	chunks[i]->SetChunkSize(ChunkSize);
	chunks[i]->SetCubeSize(CubeSize);
	chunks[i]->SetVoxels(voxels);
	chunks[i]->GenerateTerrian();
}

void AChunkWorld::DrawVertex(float LifeTime)
{
	FColor color;
	float pointSize = 10.0f;
	FVector position;
	FlushPersistentDebugLines(GetWorld());

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
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

void AChunkWorld::SetVoxels(const TArray<float>& Voxels)
{
	this->voxels = Voxels;
}

int AChunkWorld::GetVoxelIndex(int x, int y, int z) const
{
	return z * (ChunkSize + 1) * (ChunkSize + 1) + y * (ChunkSize + 1) + x;
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
