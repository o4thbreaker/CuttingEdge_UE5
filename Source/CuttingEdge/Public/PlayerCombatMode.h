#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "PlayerCombatMode.generated.h"

/* SYSTEM LOGIC INSPIRED BY https://www.youtube.com/watch?v=odzUeZhJ-WM&list=PLn6mfStdbB_7BBWCkEVztYXx0cbcCJfV2&index=4&ab_channel=DeathDiary */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUTTINGEDGE_API UPlayerCombatMode : public UActorComponent
{
	GENERATED_BODY()

/* VARIABLES */
public:	
	UPROPERTY(BlueprintReadWrite)
	bool IsCombatMode = false;

	UPROPERTY(BlueprintReadWrite)
	TArray<AEnemyBase*> EnemyList;

	UPROPERTY(BlueprintReadWrite)
	AEnemyBase* FocusEnemy;

	UPROPERTY(BlueprintReadWrite)
	bool ShouldAutoFindEnemy = true;

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	FTimerHandle CombatTimerHandle;

/* FUNCTIONS */
public:	
	// Sets default values for this component's properties
	UPlayerCombatMode();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Method to detect enemies in certain radius (TraceRadius)
	UFUNCTION(BlueprintCallable, Category = "Combat System")
	void UpdateCombatModeTick();

	// Try to figure should enemy be detected automatically or manual
	UFUNCTION(BlueprintCallable, Category = "Combat System")
	void CheckAutoFindEnemy();

	// Selects enemy to be focused and passes ref to FocusEnemy
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat System")
	void SelectEnemyToFocus(AEnemyBase* EnemyReference);
	void SelectEnemyToFocus_Implementation(AEnemyBase* EnemyReference);

	// Called during tick to focus enemy
	UFUNCTION(BlueprintCallable, Category = "Combat System")
	void FindEnemyInCombatMode();

	// Starts timer to decrease tick rate
	UFUNCTION(BlueprintCallable, Category = "Combat System")
	void StartCombatModeDetectorTimer(float LoopTime = 1.0f);

	// Ends timer to decrease tick rate
	UFUNCTION(BlueprintCallable, Category = "Combat System")
	void StopCombatModeDetectorTimer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
