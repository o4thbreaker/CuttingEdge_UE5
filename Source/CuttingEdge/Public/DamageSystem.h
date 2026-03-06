// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FDamageInfo.h"
#include "DamageSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlocked, bool, CanBeParried, AActor*, DamageDealer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageResponse, EDamageResponse, DamageResponse, AActor*, DamageDealer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUTTINGEDGE_API UDamageSystem : public UActorComponent
{
	GENERATED_BODY()

/* VARIABLES */

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsInvincible = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsDead = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsInterruptible = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsBlocking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int AttackTokenCount = 1;

	UPROPERTY(BlueprintAssignable)
	FOnBlocked OnBlockedEvent;

	UPROPERTY(BlueprintAssignable)
	FOnDamageResponse OnDamageResponseEvent;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeathEvent;

/* FUNCTIONS */

public:
	// Sets default values for this component's properties
	UDamageSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(const FDamageInfo DamageInfo, AActor* DamageDealer, bool& WasDamaged);

	UFUNCTION(BlueprintCallable)
	void Heal(const float HealAmount, float& NewHealth);

	UFUNCTION(BlueprintCallable)
	void ReserveAttackToken(const int Amount, bool& Success);

	UFUNCTION(BlueprintCallable)
	void ReturnAttackToken(const int Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
