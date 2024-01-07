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
	* ���� ���⿡ material�� ������ �۾��� �ؾ��Ѵ�. (����)
	* ���� ���� material �����°� �۾� ����.
	*  -> ���Ŀ� ������ �ϼ��Ǹ� Niagara�� �ý����� �ٲ� ����.
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

	FVector startLoc = owner->GetActorLocation() + FVector(50, 0, 100);      // �߻� ����
	FVector targetLoc = owner->GetActorLocation() + FVector(0, 300, 0);  // Ÿ�� ����.
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	FVector outVelocity = FVector::ZeroVector;   // ��� Velocity
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(worldContextObject, outVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		FPredictProjectilePathParams predictParams(20.0f, startLoc, outVelocity, 15.0f);   // 20: tracing ������ ������Ÿ�� ũ��, 15: �ù����̼ǵǴ� Max �ð�(��)
		predictParams.DrawDebugTime = 2.0f;     //����� ���� �������� �ð� (��)
		predictParams.bTraceWithCollision = true;
		predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;  // DrawDebugTime �� �����ϸ� EDrawDebugTrace::Type::ForDuration �ʿ�.
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
	//TODO : ��ź�� �ڱ��ڽŰ� ������ trajectory ����� ���� �ȵǴ� ������ ���� �ذ��ؾ���
	FVector startLoc = owner->GetActorLocation() + startLocationMediator;
	FVector targetLoc = owner->GetActorLocation() + finishLocationMediator;
	float arcValue = 0.5f;                       // ArcParam (0.0-1.0)
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(worldContextObject, suggestedVelocity, startLoc, targetLoc, GetWorld()->GetGravityZ(), arcValue))
	{

		FPredictProjectilePathParams predictParams(20.0f, startLoc, suggestedVelocity * BulletSpeed, 15.0f);   // 20: tracing ������ ������Ÿ�� ũ��, 15: �ù����̼ǵǴ� Max �ð�(��)
		predictParams.DrawDebugTime = 2.0f;     //����� ���� �������� �ð� (��)
		predictParams.bTraceWithCollision = true;
		predictParams.DrawDebugType = EDrawDebugTrace::Type::None;  // DrawDebugTime �� �����ϸ� EDrawDebugTrace::Type::ForDuration �ʿ�.
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

		// �߻�� �� ��θ� �����ִ� mesh���� �� �������Ѿ��Ѵ�.
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
		// cylinder���� ������� �� ���� 
	// issue 1 :error with cylinders (fire�� �����ϸ� ���⼭ ��������)

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

