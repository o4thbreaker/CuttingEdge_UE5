// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FDamageInfo.h"
#include "IDamagable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for Damagable Entities (such as Characters, Items, Obstacles, etc...)
 */
class CUTTINGEDGE_API IIDamagable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetDamagableReference(TScriptInterface<IIDamagable>& IDamagable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetCurrentHealth(float& Health);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetMaxHealth(float& MaxHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Heal(float HealAmount, float &NewHealth);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TakeDamage(FDamageInfo DamageInfo, AActor* DamageCauser, bool& WasDamaged);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IsDead(bool& isDead); 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IsPerformingAttack(bool& IsAttacking);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReserveAttackToken(int32 Amount, bool& Success);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReturnAttackToken(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetTeamNumber(int32& TeamNumber);

};
