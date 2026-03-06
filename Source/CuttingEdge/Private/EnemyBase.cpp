#include "EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageSystem = CreateDefaultSubobject<UDamageSystem>(TEXT("DamageSystem"));
	CombatSystem = CreateDefaultSubobject<UCombatSystem>(TEXT("CombatSystem"));

	HighlightDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("HighlightDecal"));
	HighlightDecal->SetupAttachment(GetCapsuleComponent());

	LockOnDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("LockOnDecal"));
	LockOnDecal->SetupAttachment(HighlightDecal);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	check(DamageSystem);
	check(CombatSystem);
	check(HighlightDecal);

	// don't forget that u have to bind to event and not to _implementation!
	DamageSystem->UDamageSystem::OnDeathEvent.AddDynamic(this, &AEnemyBase::Death);
	DamageSystem->UDamageSystem::OnDamageResponseEvent.AddDynamic(this, &AEnemyBase::DamageResponse);
	DamageSystem->UDamageSystem::OnBlockedEvent.AddDynamic(this, &AEnemyBase::Blocked);
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::GetEnemyHitDirection(FVector HitLocation, EEnemyHitDirection& HitDirection, float ClosestArrowDistance)
{
	int8 ClosestArrowIndex = 0;
	bool DoOnce = true;
	TArray<float> ArrowDistanceArray;

	if (DoOnce)
	{
		DoOnce = false;

		float FrontArrowDistance = FVector::Distance(HitLocation, _FrontArrow->GetComponentLocation());
		float BackArrowDistance = FVector::Distance(HitLocation, _BackArrow->GetComponentLocation());
		float LeftArrowDistance = FVector::Distance(HitLocation, _LeftArrow->GetComponentLocation());
		float RightArrowDistance = FVector::Distance(HitLocation, _RightArrow->GetComponentLocation());

		ArrowDistanceArray.Add(FrontArrowDistance);
		ArrowDistanceArray.Add(BackArrowDistance);
		ArrowDistanceArray.Add(LeftArrowDistance);
		ArrowDistanceArray.Add(RightArrowDistance);

		for (int i = 0; i < ArrowDistanceArray.Num(); i++)
		{
			if (ArrowDistanceArray[i] < ClosestArrowDistance)
			{
				ClosestArrowDistance = ArrowDistanceArray[i];
				ClosestArrowIndex = i;
			}
		}

		DoOnce = true;
		HitDirection = ClosestArrowDistance == 0 ? EEnemyHitDirection::Front : ClosestArrowDistance == 1 ? EEnemyHitDirection::Back :
			ClosestArrowDistance == 2 ? EEnemyHitDirection::Left : ClosestArrowDistance == 3 ? EEnemyHitDirection::Right : EEnemyHitDirection::Front;
	}
}

void AEnemyBase::Death_Implementation()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Magenta, TEXT("EnemyBaseCpp - Death() has been called"));

	StopAnimMontage();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);


	TArray<AActor*> mapActorArray;
	ReservedAttackTokens.GenerateKeyArray(mapActorArray); // GetKeys is doing the same but also return number of unique keys

	for (AActor* keyActor : mapActorArray)
	{
		IIDamagable::Execute_ReturnAttackToken(keyActor, *ReservedAttackTokens.Find(keyActor));
	}
}

void AEnemyBase::DamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageDealer)
{
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Magenta, TEXT("EnemyBaseCpp - DamageResponse() has been called"));
}

void AEnemyBase::Blocked_Implementation(bool CanBeParried, AActor* DamageDealer)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Magenta, TEXT("EnemyBaseCpp - Blocked() has been called"));
}

void AEnemyBase::GetEnemyReference_Implementation(AEnemyBase*& EnemyReference)
{
	EnemyReference = this;
}

void AEnemyBase::DefaultAttack_Implementation(AActor* AttackTarget)
{
	IsAttacking = true;
}

void AEnemyBase::AttackStart_Implementation(AActor* AttackTarget, int TokensNeeded, bool& Success)
{
	bool isTokenReserved = false;
	IIDamagable::Execute_ReserveAttackToken(AttackTarget, TokensNeeded, isTokenReserved);

	if (isTokenReserved)
	{
		IEnemyInterface::Execute_StoreAttackTokens(this, AttackTarget, TokensNeeded);

		TokenUsedInCurrentAttack = TokensNeeded;

		Success = true;
	}
	else
	{
		Success = false;
	}
}

void AEnemyBase::AttackEnd_Implementation(AActor* AttackTarget)
{
	if (AttackTarget->Implements<UIDamagable>())
	{
		IIDamagable::Execute_ReturnAttackToken(AttackTarget, TokenUsedInCurrentAttack);

		IEnemyInterface::Execute_StoreAttackTokens(this, AttackTarget, TokenUsedInCurrentAttack * -1);

		IsAttacking = false;

		// equivalent to call event in bp
		OnAttackEndEvent.Broadcast();
	}
}

void AEnemyBase::GetTeamNumber_Implementation(int32& _TeamNumber)
{
	_TeamNumber = TeamNumber;
}

void AEnemyBase::Heal_Implementation(float HealAmount, float& NewHealth)
{
	DamageSystem->Heal(HealAmount, NewHealth);
}

void AEnemyBase::GetCurrentHealth_Implementation(float& Health)
{
	Health = DamageSystem->Health;
}

void AEnemyBase::GetMaxHealth_Implementation(float& MaxHealth)
{
	MaxHealth = DamageSystem->MaxHealth;
}

void AEnemyBase::IsDead_Implementation(bool& isDead)
{
	isDead = DamageSystem->IsDead;
}

void AEnemyBase::GetIdealRangeToAttack_Implementation(float& AttackRadius, float& DefendRadius)
{
	AttackRadius = 150.0f;
	DefendRadius = 300.0f;
}

void AEnemyBase::IsPerformingAttack_Implementation(bool& _IsAttacking)
{
	_IsAttacking = IsAttacking;
}

void AEnemyBase::IsSelected_Implementation(bool& IsSelected)
{

}

void AEnemyBase::SetMovementSpeed_Implementation(EMovementModes MovementMode, float& SpeedValue)
{
	switch (MovementMode)
	{
	case EMovementModes::Idle:
		SpeedValue = 0.0f;
		GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
		break;
	case EMovementModes::Walking:
		SpeedValue = 100.0f;
		GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
		break;
	case EMovementModes::Jogging:
		SpeedValue = 300.0f;
		GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
		break;
	case EMovementModes::Sprinting:
		SpeedValue = 500.0f;
		GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
		break;
	default:
		SpeedValue = 0.0f;
		GetCharacterMovement()->MaxWalkSpeed = SpeedValue;
		break;
	}
}

void AEnemyBase::TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCauser, bool& WasDamaged)
{
	DamageSystem->TakeDamage(DamageInfo, DamageCauser, WasDamaged);
}

void AEnemyBase::ReserveAttackToken_Implementation(const int Amount, bool& Success)
{
	DamageSystem->ReserveAttackToken(Amount, Success);
}

void AEnemyBase::ReturnAttackToken_Implementation(const int Amount)
{
	DamageSystem->ReturnAttackToken(Amount);
}

void AEnemyBase::StoreAttackTokens_Implementation(AActor* AttackTarget, int32 TokensAmount)
{
	if (ReservedAttackTokens.Find(AttackTarget))
	{
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Magenta, TEXT("Found Key"));
		ReservedAttackTokens.Add(AttackTarget, TokensAmount + ReservedAttackTokens.FindRef(AttackTarget));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor::Magenta, TEXT("Not Found Key"));
		ReservedAttackTokens.Add(AttackTarget, TokensAmount);
	}
}
	

void AEnemyBase::SelectThisActor_Implementation()
{
	if (!DamageSystem->IsDead)
	{
		HighlightDecal->USceneComponent::SetVisibility(true, false);
	}
}

void AEnemyBase::DeselectThisActor_Implementation()
{
	HighlightDecal->USceneComponent::SetVisibility(false, false);
}

void AEnemyBase::LockOnThisActor_Implementation()
{
	if (!DamageSystem->IsDead)
	{
		LockOnDecal->USceneComponent::SetVisibility(true, false);
	}
}

void AEnemyBase::LockOffThisActor_Implementation()
{
	LockOnDecal->USceneComponent::SetVisibility(false, false);
}








