#include "DodgeSystem.h"

// Sets default values for this component's properties
UDodgeSystem::UDodgeSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter = Character;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "ERROR: CANT CAST COMBAT MODE TO CHARACTER");
	}
}

// Called when the game starts
void UDodgeSystem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDodgeSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDodgeSystem::DodgeBegin()
{
	// TODO: get rid off magic numbers
	float DodgePlayRate = 1.1f / DodgeDuration / 2.0f;

	MontageController->PlayMontage(OwnerCharacter->GetMesh(), DodgeMontage, DodgePlayRate);
}

void UDodgeSystem::DodgeOverTime(FVector Location)
{
	FVector dodgeStartPosition = GetOwner()->GetActorLocation();

	FRotator lookAtRotation = UKismetMathLibrary::FindLookAtRotation(dodgeStartPosition, Location);
	GetOwner()->SetActorRotation(lookAtRotation);

	// TODO: make dodge timeline
}

FVector UDodgeSystem::GetFurthestDodgeLocation(FVector Start, FVector End)
{
	FVector direction = UKismetMathLibrary::GetDirectionUnitVector(Start, End);
	float capsuleDiameter = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f;

	int capsuleCount = FVector::Distance(Start, End) / capsuleDiameter;

	for (int i = 0; i <= capsuleCount; i++)
	{
		FVector currentCapsuleLocation = Start + (capsuleCount - i) * direction * capsuleDiameter;

		float traceRadius = 0.0f;
		float traceHalfHeight = 0.0f;
		OwnerCharacter->GetCapsuleComponent()->GetUnscaledCapsuleSize(traceRadius, traceHalfHeight);

		TArray<FHitResult> HitArray;
		TArray<AActor*> ActorsToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		const bool Hit = UKismetSystemLibrary::CapsuleTraceMultiForObjects(GetWorld(), currentCapsuleLocation, currentCapsuleLocation, traceRadius, traceHalfHeight,
			ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, HitArray, true);

		if (IsValidDodgeLocation(currentCapsuleLocation, HitArray))
		{
			return currentCapsuleLocation;
		}
	}

	return Start;
}

bool UDodgeSystem::IsValidDodgeLocation(FVector Location, TArray<FHitResult> Hits)
{
	FHitResult* HitResult = nullptr;
	TArray<AActor*> ActorsToIgnore;
	const bool Hit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Location, (Location + FVector(0.0f, 0.0f, -500.0f)), 
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, *HitResult, true);

	if (Hit)
	{
		if (Hits.IsEmpty())
		{
			return true;
		}
		else
		{
			for (FHitResult hit : Hits)
			{
				return hit.Component->GetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic) == ECollisionResponse::ECR_Block;
			}

			return true;
		}
	}
	else
	{
		return false;
	}
}

FVector UDodgeSystem::CalculateDodgeTargetLocation()
{
	FVector actorOrMovementVector = UKismetMathLibrary::SelectVector(GetOwner()->GetActorForwardVector(), 
		Cast<APawn>(GetOwner())->GetLastMovementInputVector(), Cast<APawn>(GetOwner())->GetLastMovementInputVector() == FVector::ZeroVector);

	return GetOwner()->GetActorLocation() + (actorOrMovementVector * MaxDodgeDistance);
}

