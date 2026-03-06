// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayMontageController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerAttackSystem.generated.h"


UENUM()
enum class ExecAimPins : uint8
{
	SwordIsEquiped,
	SwordIsNotEquiped
};

UENUM()
enum class ExecDashAttackPins : uint8
{
	DashAttack,
	NoAttack
};

UENUM(BlueprintType, Category = "Combat")
enum class PlayerStances : uint8
{
	DefaultStance,
	CombatStance
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUTTINGEDGE_API UPlayerAttackSystem : public UActorComponent
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Movement")
	bool IsMovementAvailable = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|Stance")
	PlayerStances PlayerStance;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|Misc")
	TArray<AActor*> TempHitArray;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|Misc")
	int AttackCount = 0;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|Misc")
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|DefaultAttack")
	bool IsDefaultAttackInProgress = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|DefaultAttack")
	TArray<UAnimMontage*> ComboAttackMontages;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|Parry")
	bool IsParryAttackInProgress = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Parry")
	UAnimMontage* ParryAttackMontage;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|DashAttack")
	bool IsAiming = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat|DashAttack")
	bool CanDash = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|DashAttack")
	UAnimMontage* DashAttackAnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|DashAttack")
	UAnimMontage* DashAimAnimMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|DashAttack")
	UAnimMontage* DashAimCancelAnimMontage;

private:
	UPROPERTY()
	APlayMontageController* MontageController;

	UPROPERTY()
	AMainCharacter* CharacterOwner;

	UPROPERTY()
	FTimerHandle InterruptedDelayHandle;

	TScriptDelegate <FWeakObjectPtr> OnDefaultAttackInterrupted;
	TScriptDelegate <FWeakObjectPtr> OnDefaultAttackCompleted;

	TScriptDelegate <FWeakObjectPtr> OnParryAttackInterrupted;
	TScriptDelegate <FWeakObjectPtr> OnParryAttackCompleted;

public:
	// Sets default values for this component's properties
	UPlayerAttackSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Combat|DefaultAttack")
	void DefaultMeleeAttack(class USkeletalMeshComponent* InSkeletalMeshComponent, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat|Parry")
	void SpecialParryAttack(class USkeletalMeshComponent* InSkeletalMeshComponent, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat|DashAttack", meta = (ExpandEnumAsExecs = "pin"))
	void DashAttack(ExecDashAttackPins& pin, float dashAttackPlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat|DashAttack", meta = (ExpandEnumAsExecs = "pin"))
	void StartDashAttack(ExecAimPins& pin, float PlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat|DashAttack")
	void EndDashAttack(float DashAimCancelPlayRate);

	UFUNCTION(BlueprintCallable, Category = "Combat|Stance")
	void ChangeStance(PlayerStances NewStance, float MoveSpeed);

	UFUNCTION()
	void OnDefaultAttackTimerEnd();

private:
	UFUNCTION()
	void HandleOnDefaultAttackInterrupted(FName NotifyName, UAnimMontage* PlayedMontage);

	UFUNCTION()
	void HandleOnDefaultAttackCompleted(FName NotifyName, UAnimMontage* PlayedMontage);

	UFUNCTION()
	void HandleOnParryAttackInterrupted(FName NotifyName, UAnimMontage* PlayedMontage);

	UFUNCTION()
	void HandleOnParryAttackCompleted(FName NotifyName, UAnimMontage* PlayedMontage);

	UFUNCTION(BlueprintCallable)
	void ResetAttackCount();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
