// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/ProjectileManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UProjectileManager::UProjectileManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	startLocationMediator = FVector(50, 0, 100);
	finishLocationMediator = FVector(0, 300, 0);
	BulletSpeed = 10;
	// ...


}


// Called when the game starts
void UProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	this->BeginReplication();
	// ...
}


// Called every frame
void UProjectileManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bAiming)
	{

	}
	// ...
}

void UProjectileManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UProjectileManager, predictParam)
		DOREPLIFETIME(UProjectileManager, predictResult)
		DOREPLIFETIME(UProjectileManager, startLocationMediator)
		DOREPLIFETIME(UProjectileManager, finishLocationMediator)
		DOREPLIFETIME(UProjectileManager, suggestedVelocity)
		DOREPLIFETIME(UProjectileManager, bCanIFire)
		DOREPLIFETIME(UProjectileManager, CurLocation)


}


bool UProjectileManager::SwitchAimingStation()
{
	bAiming ^= true;
	return bAiming;
}





bool UProjectileManager::Server_Fire_Validate()
{
	return true;
}

void UProjectileManager::Server_Fire_Implementation()
{
	BulletSpawning();
}

void UProjectileManager::UpdateProjectilePath(const TArray<FPredictProjectilePathPointData>& PathData)
{
	/* TODO
	* 이제 여기에 material을 입히는 작업을 해야한다. (성공)
	* 좀더 예쁜 material 입히는거 작업 시작.
	*  -> 추후에 게임이 완성되면 Niagara로 시스템을 바꿀 예정.
	*/


	int interval = 0;

	for (const FPredictProjectilePathPointData& Point : PathData)
	{
		if (interval % 4 == 1)
		{
			ATrajectory_Cylinder* MeshActor = GetWorld()->SpawnActor<ATrajectory_Cylinder>(Point.Location, Point.Velocity.Rotation());
			MeshActor->SetMaterialColor(FLinearColor(0, 0, 1, 0));
			cylinders.Add(MeshActor);
		}
		interval++;
	}
}

//this is test code. maybe will deleted
bool UProjectileManager::testFunc_ProjectilePredict()
{
	worldContextObject = GetOwner();

	AActor* owner = GetOwner();
	predictParam = FPredictProjectilePathParams();

	FVector startLoc = owner->GetActorLocation() + FVector(50, 0, 100);      // 발사 지점
	FVector targetLoc = owner->GetActorLocation() + FVector(0, 300, 0);  // 타겟 지점.
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	FVector outVelocity = FVector::ZeroVector;   // 결과 Velocity
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(worldContextObject, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
		predictParams.DrawDebugTime = 2.0f;     //디버그 라인 보여지는 시간 (초)
		predictParams.bTraceWithCollision = true;
		predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
		predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		FPredictProjectilePathResult result;
		bool bSuccess = UGameplayStatics::PredictProjectilePath(worldContextObject, predictParams, result);


		UE_LOG(LogTemp, Warning, TEXT("%s"), *result.HitResult.HitObjectHandle.GetName());
		return bSuccess;

	}
	UE_LOG(LogTemp, Warning, TEXT("FAILED"));
	return false;


}

bool UProjectileManager::TrajectoryCalculator()
{
	worldContextObject = GetOwner();

	AActor* owner = GetOwner();
	predictParam = FPredictProjectilePathParams();
	finishLocationMediator = owner->GetActorForwardVector() * 200;
	//TODO : 포탄이 자기자신과 맞을때 trajectory 계산이 전혀 안되는 에러를 조금 해결해야함
	FVector startLoc = owner->GetActorLocation() + startLocationMediator;
	FVector targetLoc = owner->GetActorLocation() + finishLocationMediator;
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(worldContextObject, suggestedVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{

		FPredictProjectilePathParams predictParams(20.0f, startLoc, suggestedVelocity * BulletSpeed, 15.0f);   // 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
		predictParams.DrawDebugTime = 2.0f;     //디버그 라인 보여지는 시간 (초)
		predictParams.bTraceWithCollision = true;
		predictParams.DrawDebugType = EDrawDebugTrace::Type::None;  // DrawDebugTime 을 지정하면 EDrawDebugTrace::Type::ForDuration 필요.
		predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		bool bSuccess = UGameplayStatics::PredictProjectilePath(worldContextObject, predictParams, predictResult);
		collideObjectName = predictResult.HitResult.HitObjectHandle.GetName();

		TArray<FPredictProjectilePathPointData> PathData = predictResult.PathData;
		UpdateProjectilePath(PathData);

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *predictResult.HitResult.HitObjectHandle.GetName());
		bCanIFire = true;
		return bSuccess;
	}
	return false;
}

bool UProjectileManager::Fire()
{

	if (!cylinders.IsEmpty())
	{
		FRotator bulletRotation = cylinders[0]->GetVelocity().Rotation();

		// 발사될 때 경로를 보여주는 mesh들을 다 삭제시켜야한다.
		for (ATrajectory_Cylinder* actor : cylinders)
		{
			if (actor)
			{
				actor->Destroy();
			}
		}
	}
	else
	{
		FRotator bulletRotation = FRotator(0, 0, 0);
	}
	AActor* owner = GetOwner();
	CurLocation = owner->GetActorLocation();
	BulletSpawning();
	return true;
}

void UProjectileManager::BulletSpawning()
{
	if (GetOwner()->HasAuthority())
	{
		// cylinder들이 사라지기 전 저장 
	// issue 1 :error with cylinders (fire를 많이하면 여기서 에러생김)

		ABullet* bullet = GetOwner()->GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), CurLocation + startLocationMediator, FRotator::ZeroRotator);
		if (bullet)
		{
			//bullet->SetActorRotation(bulletRotation);
			UProjectileMovementComponent* ProjectileMovementComponent = bullet->FindComponentByClass<UProjectileMovementComponent>();
			if (ProjectileMovementComponent)
			{
				ProjectileMovementComponent->Velocity = suggestedVelocity * BulletSpeed;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FireFailed"))
		}
		bCanIFire = false;


	}
	else
	{
		Server_Fire();
	}
}

