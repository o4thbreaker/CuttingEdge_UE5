// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackSystem.h"


// Sets default values for this component's properties
UPlayerAttackSystem::UPlayerAttackSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	MontageController = CreateDefaultSubobject<APlayMontageController>(TEXT("MontageController"));

	// ...
}


// Called when the game starts
void UPlayerAttackSystem::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	check(MontageController);

	OnDefaultAttackInterrupted.BindUFunction(this, "HandleOnDefaultAttackInterrupted");
	MontageController->APlayMontageController::OnMontageInterrupted.Add(OnDefaultAttackInterrupted);

	OnDefaultAttackCompleted.BindUFunction(this, "HandleOnDefaultAttackCompleted");
	MontageController->APlayMontageController::OnMontageCompleted.Add(OnDefaultAttackCompleted);

	OnParryAttackInterrupted.BindUFunction(this, "HandleOnParryAttackInterrupted");
	MontageController->APlayMontageController::OnMontageInterrupted.Add(OnParryAttackInterrupted);

	OnParryAttackCompleted.BindUFunction(this, "HandleOnParryAttackCompleted");
	MontageController->APlayMontageController::OnMontageCompleted.Add(OnParryAttackCompleted);
}


// Called every frame
void UPlayerAttackSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerAttackSystem::DefaultMeleeAttack(class USkeletalMeshComponent* InSkeletalMeshComponent, float PlayRate = 1.0f)
{
	if (!IsDefaultAttackInProgress)
	{
		IsDefaultAttackInProgress = true;

		if (AttackCount > ComboAttackMontages.Num() - 1)
		{
			ResetAttackCount();
		}

		MontageController->APlayMontageController::PlayMontage(InSkeletalMeshComponent, ComboAttackMontages[AttackCount], PlayRate);

		//clear an array
		TempHitArray.Empty();

		AttackCount++;
	}
}

void UPlayerAttackSystem::DashAttack(ExecDashAttackPins& pin, float DashAttackPlayRate = 0.8f)
{
	if (CanDash)
	{
		pin = ExecDashAttackPins::DashAttack;

		check(CharacterOwner);
		CharacterOwner->PlayAnimMontage(DashAttackAnimMontage, DashAttackPlayRate);
	}
	else
	{
		pin = ExecDashAttackPins::NoAttack;
	}
}

void UPlayerAttackSystem::StartDashAttack(ExecAimPins& pin, float PlayRate = 0.8f)
{
	if (PlayerStance == PlayerStances::CombatStance)
	{
		pin = ExecAimPins::SwordIsEquiped;

		IsMovementAvailable = false;

		check(CharacterOwner);
		CharacterOwner->bUseControllerRotationYaw = true;
		CharacterOwner->PlayAnimMontage(DashAimAnimMontage, PlayRate);

		IsAiming = true;
		CanDash = true;
	}
	else
	{
		pin = ExecAimPins::SwordIsNotEquiped;
	}
}

void UPlayerAttackSystem::EndDashAttack(float DashAimCancelPlayRate = 0.8f)
{
	IsMovementAvailable = true;
	check(CharacterOwner);
	CharacterOwner->bUseControllerRotationYaw = false;
	IsAiming = false;

	CharacterOwner->StopAnimMontage(DashAimAnimMontage);
	if (CanDash)
	{
		CharacterOwner->PlayAnimMontage(DashAimCancelAnimMontage, DashAimCancelPlayRate);
		CanDash = false;
	}

}

void UPlayerAttackSystem::SpecialParryAttack(class USkeletalMeshComponent* InSkeletalMeshComponent, float PlayRate = 1.0f)
{
	if (!IsParryAttackInProgress)
	{
		MontageController->APlayMontageController::PlayMontage(InSkeletalMeshComponent, ParryAttackMontage, PlayRate);
		TempHitArray.Empty();
	}
}

void UPlayerAttackSystem::ChangeStance(PlayerStances NewStance, float MoveSpeed)
{
	PlayerStance = NewStance;

	check(CharacterOwner);
	CharacterOwner->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

void UPlayerAttackSystem::HandleOnDefaultAttackInterrupted(FName NotifyName, UAnimMontage* PlayedMontage)
{
	if (ComboAttackMontages.Contains(PlayedMontage))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, *(FString::Printf(TEXT("DEFAULT ATTACK INTERRUPTED"))));
		check(GetWorld());
		GetWorld()->GetTimerManager().SetTimer(InterruptedDelayHandle, this, &UPlayerAttackSystem::OnDefaultAttackTimerEnd, 1.2f, false);
	}
}

void UPlayerAttackSystem::HandleOnDefaultAttackCompleted(FName NotifyName, UAnimMontage* PlayedMontage)
{
	if (ComboAttackMontages.Contains(PlayedMontage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, PlayedMontage->GetName());

		ResetAttackCount();

		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, *(FString::Printf(TEXT("DEFAULT ATTACK COMPLETE"))));
	}
}

void UPlayerAttackSystem::HandleOnParryAttackInterrupted(FName NotifyName, UAnimMontage* PlayedMontage)
{
	if (ParryAttackMontage == PlayedMontage)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, PlayedMontage->GetName());

		IsParryAttackInProgress = false;
	}
}

void UPlayerAttackSystem::HandleOnParryAttackCompleted(FName NotifyName, UAnimMontage* PlayedMontage)
{
	if (ParryAttackMontage == PlayedMontage)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, PlayedMontage->GetName());

		IsParryAttackInProgress = false;
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, *(FString::Printf(TEXT("PARRY COMPLETE"))));
	}

}

void UPlayerAttackSystem::ResetAttackCount()
{
	AttackCount = 0;
}

void UPlayerAttackSystem::OnDefaultAttackTimerEnd()
{
	IsDefaultAttackInProgress = false;
	ResetAttackCount();
}


