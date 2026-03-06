// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UENUM(BlueprintType)
enum class EMovementModes : uint8
{
	Idle,
	Walking,
	Jogging,
	Sprinting
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class CUTTINGEDGE_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SelectThisActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DeselectThisActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IsSelected(bool& IsSelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetEnemyReference(AEnemyBase*& EnemyReference);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetPatrolRoute(APatrolRoute*& PatrolRoute);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMovementSpeed(EMovementModes MovementMode, float& SpeedValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetIdealRangeToAttack(float& AttackRadius, float& DefendRadius);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EquipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UnequipWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DefaultAttack(AActor* AttackTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackStart(AActor* AttackTarget, int TokensNeeded, bool& Success);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttackEnd(AActor* AttackTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StoreAttackTokens(AActor* AttackTarget, int32 TokensAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LockOnThisActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LockOffThisActor();

};
