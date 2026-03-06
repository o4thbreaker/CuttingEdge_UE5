#include "PlayerCombatMode.h"

// Sets default values for this component's properties
UPlayerCombatMode::UPlayerCombatMode()
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
void UPlayerCombatMode::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerCombatMode::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerCombatMode::UpdateCombatModeTick()
{
	const float TraceRadius = 2000.0f;
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = GetOwner()->GetActorLocation();
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitArray;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;

	ActorsToIgnore.Add(GetOwner());
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	const bool Hit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(), Start, End, TraceRadius, ObjectTypesArray, false, ActorsToIgnore, EDrawDebugTrace::None, HitArray, true);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, *(FString::Printf(TEXT("Enemy count: %d"), EnemyList.Num())));

	if (Hit)
	{
		IsCombatMode = true;

		for (FHitResult& hit : HitArray)
		{
			AEnemyBase* EnemyReference;
			IEnemyInterface::Execute_GetEnemyReference(hit.GetActor(), EnemyReference);

			if (!EnemyList.Contains(EnemyReference))
			{
				EnemyList.Add(EnemyReference);
			}
		}
	}
	else
	{
		EnemyList.Empty();
		IsCombatMode = false;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, *(FString::Printf(TEXT("Enemy count: %d"), EnemyList.Num())));
}

void UPlayerCombatMode::CheckAutoFindEnemy()
{
	float DesiredInputVectorMinLength = 0.8f;
	float DesiredAimYawLimit = 35.0f;

	float InputVectorLength = OwnerCharacter->GetCharacterMovement()->GetLastInputVector().Size();

	float SavedControlRotationYaw = 0.0f;

	bool DoOnce = true;
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (InputVectorLength > DesiredInputVectorMinLength)
	{
		if (DoOnce)
		{
			SavedControlRotationYaw = controller->GetControlRotation().Yaw;

			ShouldAutoFindEnemy = false;
			DoOnce = false;
		}
	}
	else
	{
		if ((controller->GetControlRotation().Yaw > SavedControlRotationYaw + DesiredAimYawLimit) or
			(controller->GetControlRotation().Yaw) < SavedControlRotationYaw - DesiredAimYawLimit)
		{
			ShouldAutoFindEnemy = true;
			DoOnce = true;
		}
	}
}

void UPlayerCombatMode::SelectEnemyToFocus_Implementation(AEnemyBase* EnemyReference)
{
	if (EnemyReference != nullptr)
	{
		if (FocusEnemy != nullptr)
		{
			IEnemyInterface::Execute_LockOffThisActor(FocusEnemy);
			FocusEnemy = EnemyReference;
		}
		else
		{
			FocusEnemy = EnemyReference;
		}

		IEnemyInterface::Execute_LockOnThisActor(FocusEnemy);

	}
	else
	{
		if (FocusEnemy != nullptr)
		{
			IEnemyInterface::Execute_LockOffThisActor(FocusEnemy);
		}
	}
}

void UPlayerCombatMode::FindEnemyInCombatMode()
{
	if (IsCombatMode)
	{
		float TempFoundDot = 0.0f;
		AEnemyBase* TempFoundEnemy = nullptr;

		if (EnemyList.Num() > 0)
		{
			CheckAutoFindEnemy();

			for (AEnemyBase* enemy : EnemyList)
			{
				if (enemy != nullptr and enemy->GetDistanceTo(GetOwner()) < 600.0f)
				{
					FVector EnemyToPlayerDistanceVector = (enemy->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal(0.0001f);

					APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
					FVector CameraOrInputVector = (ShouldAutoFindEnemy ? CameraManager->GetActorForwardVector() : 
						OwnerCharacter->GetCharacterMovement()->GetLastInputVector()).GetSafeNormal(0.0001f);

					float TempDot = FVector::DotProduct(EnemyToPlayerDistanceVector, CameraOrInputVector);

					if (TempDot > TempFoundDot)
					{
						TempFoundDot = TempDot;
						TempFoundEnemy = enemy;
					}
				}
			}

			if (TempFoundEnemy != nullptr and TempFoundDot > 0.5f)
			{
				SelectEnemyToFocus(TempFoundEnemy);
			}
			else
			{
				TempFoundDot = 0.0f;
				TempFoundEnemy = nullptr;
			}

		}
		else
		{
			SelectEnemyToFocus(nullptr);
		}
	}
}

void UPlayerCombatMode::StartCombatModeDetectorTimer(float LoopTime)
{
	if (!CombatTimerHandle.IsValid())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, *(FString::Printf(TEXT("Setting timer"))));

		GetOwner()->GetWorldTimerManager().SetTimer(CombatTimerHandle, this, &UPlayerCombatMode::UpdateCombatModeTick, LoopTime, true);
	}
}

void UPlayerCombatMode::StopCombatModeDetectorTimer()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, *(FString::Printf(TEXT("Clearing timer"))));

	GetOwner()->GetWorldTimerManager().ClearTimer(CombatTimerHandle);
}

