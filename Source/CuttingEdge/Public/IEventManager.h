// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEventManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIEventManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUTTINGEDGE_API IIEventManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Fires delegate to signal any (uncertain) enemy death */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallEnemyDeath();

	/** Fires delegate to signal any (uncertain) enemy spawn */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallEnemySpawn();

	/** Fires delegate to signal that player parry function called */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallPlayerParry();

	/** Fires delegate to signal that player is dead */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallPlayerDeath();

	/** Fires delegate to trigger enter level functionality */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CallLevelStart();

};
