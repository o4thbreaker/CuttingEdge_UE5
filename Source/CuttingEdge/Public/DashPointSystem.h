	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashPointSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPointEarnedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPointsLostDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPointsEarnedDelegate);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CUTTINGEDGE_API UDashPointSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintType, BlueprintReadWrite, EditAnywhere, Category = "DashPoint")
	int dashPointCount = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DashPoint", meta = (ExposeOnSpawn = "true"))
	int maxDashPoints = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DashPoint")
	bool isDashEnabled = false;

	UPROPERTY(BlueprintAssignable)
	FOnPointEarnedDelegate OnPointEarned;

	UPROPERTY(BlueprintAssignable)
	FOnAllPointsLostDelegate OnAllPointsLost;

	UPROPERTY(BlueprintAssignable)
	FOnAllPointsEarnedDelegate OnAllPointsEarned;

public:
	// Sets default values for this component's properties
	UDashPointSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Set dash points to zero and fire delegate
	UFUNCTION(BlueprintCallable, Category = "DashPoint")
	void LoseAllDashPoints();

	// Increment dash points count and fire delegate
	UFUNCTION(BlueprintCallable, Category = "DashPoint")
	void GetDashPoint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
